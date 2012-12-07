/*	NAME:
		NDBHandle.cpp

	DESCRIPTION:
		Database handle.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "nano_sqlite.h"

#include "NMathUtilities.h"
#include "NTimeUtilities.h"
#include "NDBHandle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32 kProgressUpdate									= 1000;
static const NIndex kMaxLockedTries									= 10;
static const UInt32 kBackupPageCount								= 100;
static const char   kNamePrefix										= ':';





//============================================================================
//		Internal statics
//----------------------------------------------------------------------------
NThreadLocalRef				NDBHandle::sThreadKey					= NThread::CreateLocal();
NDBHandleThreadCacheList	NDBHandle::sThreadCaches;





//============================================================================
//		NDBHandle::NDBHandle : Constructor.
//----------------------------------------------------------------------------
NDBHandle::NDBHandle(void)
{


	// Validate our state
	NN_ASSERT(sqlite3_threadsafe());



	// Initialize ourselves
	mFlags    = kNDBNone;
	mDatabase = NULL;
}





//============================================================================
//		NDBHandle::~NDBHandle : Destructor.
//----------------------------------------------------------------------------
NDBHandle::~NDBHandle(void)
{


	// Clean up
	if (IsOpen())
		Close();
}





//============================================================================
//		NDBHandle::IsOpen : Is the database open?
//----------------------------------------------------------------------------
bool NDBHandle::IsOpen(void) const
{


	// Get our state
	return(mDatabase != NULL);
}





//============================================================================
//		NDBHandle::IsMutable : Is the database mutable?
//----------------------------------------------------------------------------
bool NDBHandle::IsMutable(void) const
{


	// Get our state
	return((mFlags & kNDBReadOnly) == kNDBNone);
}





//============================================================================
//		NDBHandle::GetFile : Get the file.
//----------------------------------------------------------------------------
NFile NDBHandle::GetFile(void) const
{


	// Get the file
	return(mFile);
}





//============================================================================
//		NDBHandle::Open : Open the database.
//----------------------------------------------------------------------------
NStatus NDBHandle::Open(const NFile &theFile, NDBFlags theFlags, const NString &theVFS)
{	int				sqlFlags;
	const char		*vfsName;
	NString			thePath;
	sqlite3			*sqlDB;
	NDBStatus		dbErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the state we need
	vfsName  = theVFS.IsEmpty() ? NULL : theVFS.GetUTF8();
	thePath  = theFile.GetPath();
	sqlDB    = NULL;
	sqlFlags = 0;

	if (theFlags & kNDBReadOnly)
		sqlFlags |= SQLITE_OPEN_READONLY;
	else
		sqlFlags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;



	// Open the database
	dbErr = sqlite3_open_v2(thePath.GetUTF8(), &sqlDB, sqlFlags, vfsName);
	if (dbErr != kNoErr)
		NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));



	// Update our state
	if (dbErr == kNoErr)
		{
		mFile     = theFile;
		mFlags    = theFlags;
		mDatabase = sqlDB;
		}

	return(SQLiteGetStatus(dbErr));
}





//============================================================================
//		NDBHandle::Close : Close the database.
//----------------------------------------------------------------------------
void NDBHandle::Close(void)
{	sqlite3				*sqlDB;
	NDBStatus			dbErr;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Clean up
	SQLiteDestroyQueries();



	// Close the database
	sqlDB = (sqlite3 *) mDatabase;
	dbErr = sqlite3_close(sqlDB);

	if (dbErr != kNoErr)
		NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));



	// Reset our state
	mFile.Clear();
	mFlags    = kNDBNone;
	mDatabase = NULL;
}





//============================================================================
//		NDBHandle::CreateBackup : Create a backup.
//----------------------------------------------------------------------------
NStatus NDBHandle::CreateBackup(const NFile &dstFile)
{	int					sqlFlags, pagesTotal, pagesLeft, pagesDone;
	sqlite3				*srcDB, *dstDB;
	sqlite3_backup		*sqlBackup;
	NString				thePath;
	NDBStatus			dbErr;



	// Get the state we need
	srcDB = (sqlite3 *) mDatabase;
	dstDB = NULL;
	
	thePath   = dstFile.GetPath();
	sqlFlags  = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	sqlBackup = NULL;



	// Open the destination database
	dbErr = sqlite3_open_v2(thePath.GetUTF8(), &dstDB, sqlFlags, NULL);

	if (dbErr == kNoErr)
		sqlBackup = sqlite3_backup_init(dstDB, "main", srcDB, "main");

	if (dbErr != kNoErr || sqlBackup == NULL)
		NN_LOG("SQLite: %s", sqlite3_errmsg(dstDB));



	// Perform the backup
	if (sqlBackup != NULL)
		{
		BeginProgress();

		do
			{
			// Copy some pages
			dbErr = sqlite3_backup_step(sqlBackup, kBackupPageCount);

			pagesTotal = sqlite3_backup_pagecount(sqlBackup);
			pagesLeft  = sqlite3_backup_remaining(sqlBackup);
			pagesDone  = pagesTotal - pagesLeft;

			ContinueProgress(pagesDone, pagesTotal);
			
			
			// Give up some time
			if (dbErr == SQLITE_BUSY || dbErr== SQLITE_LOCKED)
				NThread::Sleep();
			}
		while (dbErr == SQLITE_OK || dbErr == SQLITE_BUSY || dbErr == SQLITE_LOCKED);
		
		(void) sqlite3_backup_finish(sqlBackup);
		EndProgress();
		}



	// Clean up
	if (dstDB != NULL)
		{
		dbErr = sqlite3_close(dstDB);
		if (dbErr != kNoErr)
			NN_LOG("SQLite: %s", sqlite3_errmsg(dstDB));
		}

	return(SQLiteGetStatus(dbErr));
}





//============================================================================
//		NDBHandle::Execute : Execute a query.
//----------------------------------------------------------------------------
NStatus NDBHandle::Execute(const NDBQuery &theQuery, const NDBResultFunctor &theResult, NTime waitFor)
{	NStatus			theErr;
	NDBStatus		dbErr;
	NIndex			n;



	// Compiler warning
	dbErr = SQLITE_ERROR;



	// Execute the query
	//
	// SQLITE_LOCKED may be returned when using a shared page cache; unlike SQLITE_BUSY
	// we need to retry the entire query, rather than the current statement.
	//
	// Since sqlite3_unlock_notify only supports a single pending unlock notification, each
	// Execute() performs its own try-sleep-try loop until it succeeds or hits some limit to
	// allow multiple threads to be retry in parallel.
	SQLiteProgressBegin();
	
	for (n = 0; n < kMaxLockedTries; n++)
		{
		dbErr = SQLiteExecute(theQuery, theResult, waitFor);
		if (dbErr != SQLITE_LOCKED)
			break;

		NThread::Sleep();
		}
	
	SQLiteProgressEnd();



	// Get the result
	theErr = SQLiteGetStatus(dbErr);
	
	return(theErr);
}





//============================================================================
//		NDBHandle::ExecuteSInt32 : Execute a query to obtain an SInt32.
//----------------------------------------------------------------------------
SInt32 NDBHandle::ExecuteSInt32(const NDBQuery &theQuery)
{	SInt32		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueSInt32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteSInt64 : Execute a query to obtain an SInt64.
//----------------------------------------------------------------------------
SInt64 NDBHandle::ExecuteSInt64(const NDBQuery &theQuery)
{	SInt64		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueSInt64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteFloat32 : Execute a query to obtain a Float32.
//----------------------------------------------------------------------------
Float32 NDBHandle::ExecuteFloat32(const NDBQuery &theQuery)
{	Float32		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0.0f;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueFloat32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteFloat64 : Execute a query to obtain a Float64.
//----------------------------------------------------------------------------
Float64 NDBHandle::ExecuteFloat64(const NDBQuery &theQuery)
{	Float64		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0.0;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueFloat64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteString : Execute a query to obtain a string.
//----------------------------------------------------------------------------
NString NDBHandle::ExecuteString(const NDBQuery &theQuery)
{	NString		theValue;
	NStatus		theErr;



	// Execute the query
	theErr = Execute(theQuery, BindFunction(NDBResult::GetRowValueString, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteData : Execute a query to obtain data.
//----------------------------------------------------------------------------
NData NDBHandle::ExecuteData(const NDBQuery &theQuery)
{	NData		theValue;
	NStatus		theErr;



	// Execute the query
	theErr = Execute(theQuery, BindFunction(NDBResult::GetRowValueData, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::HasTable : Does a table exist?
//----------------------------------------------------------------------------
bool NDBHandle::HasTable(const NString &theTable)
{	SInt32		theResult;
	NString		theSQL;



	// Check for the table
	//
	// SQLite does not support "IF NOT EXISTS" for virtual tables, however
	// we can identify the presence of tables by querying the master table.
	theSQL.Format("SELECT count() FROM sqlite_master WHERE name=\"%@\";", theTable);
	theResult = ExecuteSInt32(theSQL);
	
	return(theResult != 0);
}





//============================================================================
//		NDBHandle::Cancel : Cancel the current operation.
//----------------------------------------------------------------------------
void NDBHandle::Cancel(void)
{


	// Validate our state
	NN_ASSERT(IsOpen());



	// Cancel the operation
	sqlite3_interrupt((sqlite3 *) mDatabase);
}





//============================================================================
//		NDBHandle::GetDatabase : Get the database.
//----------------------------------------------------------------------------
void *NDBHandle::GetDatabase(void)
{


	// Get the database
	return(mDatabase);
}





//============================================================================
//		NDBHandle::SQLiteExecute : Execute a query.
//----------------------------------------------------------------------------
#pragma mark -
NDBStatus NDBHandle::SQLiteExecute(const NDBQuery &theQuery, const NDBResultFunctor &theResult, NTime waitFor)
{	bool				areDone, waitForever;
	sqlite3_stmt		*sqlQuery;
	NTime				startTime;
	NDBStatus			dbErr;



	// Get the state we need
	waitForever = NMathUtilities::AreEqual(waitFor, kNTimeForever);
	sqlQuery    = (sqlite3_stmt *) SQLiteFetchQuery(theQuery);

	SQLiteBindParameters(sqlQuery, theQuery.GetParameters());



	// Execute the query
	startTime = NTimeUtilities::GetTime();
	dbErr     = SQLITE_OK;
	areDone   = false;
	
	while (!areDone)
		{
		// Execute the query
		dbErr = sqlite3_step(sqlQuery);

		switch (dbErr) {
			case SQLITE_ROW:
				if (theResult != NULL)
					theResult((NDBQueryRef) sqlQuery);
				break;
			
			case SQLITE_BUSY:
				areDone = !waitForever && (NTimeUtilities::GetTime() >= (startTime + waitFor));
				if (!areDone)
					NThread::Sleep();
				break;
			
			default:
				areDone = true;
				break;
			}
		}

	return(dbErr);
}





//============================================================================
//		NDBHandle::SQLiteFetchQuery : Fetch the query.
//----------------------------------------------------------------------------
NDBQueryRef NDBHandle::SQLiteFetchQuery(const NDBQuery &theQuery)
{	NDBHandleThreadCacheMap				*threadCache;
	NDBCachedQuery						cachedQuery;
	NDBHandleThreadCacheMapIterator		theIter;
	NString								theSQL;



	// Get the state we need
	theSQL      = theQuery.GetValue();
	threadCache = (NDBHandleThreadCacheMap *) NThread::GetLocalValue(sThreadKey);



	// Create the cache
	//
	// We cache the last query performed, allowing us to reuse the SQLite
	// statement if the SQL is the same.
	//
	// This can be a significant optimisation when performing large numbers
	// of identical statements (e.g., populating a DB where only the bound
	// parameters are changed on each execution).
	//
	//
	// Since multiple threads may be querying the database we need to use a
	// per-thread cache or one thread may reset a query being used by another.
	//
	// Each thread maintains a per-thread list of file->cache state, with a
	// lock around the overall cache but lock-less access from within a thread
	// due to the use of a thread-local value to hold this thread's cache.
	if (threadCache == NULL)
		{
		threadCache = new NDBHandleThreadCacheMap;

		sThreadCaches.PushBack(threadCache);
		NThread::SetLocalValue(sThreadKey, threadCache);
		}



	// Populate the cache
	theIter = threadCache->find(this);

	if (theIter == threadCache->end())
		{
		cachedQuery.theSQL   = theSQL;
		cachedQuery.theQuery = SQLiteCreateQuery(theQuery);
		(*threadCache)[this] = cachedQuery;
		}


	// Update the cache
	else
		{
		cachedQuery = theIter->second;
		
		if (cachedQuery.theSQL == theSQL)
			sqlite3_reset((sqlite3_stmt *) cachedQuery.theQuery);
		else
			{
			SQLiteDestroyQuery(cachedQuery.theQuery);

			cachedQuery.theSQL   = theSQL;
			cachedQuery.theQuery = SQLiteCreateQuery(theQuery);
			(*threadCache)[this] = cachedQuery;
			}
		}

	return(cachedQuery.theQuery);
}





//============================================================================
//		NDBHandle::SQLiteCreateQuery : Get an SQLite query.
//----------------------------------------------------------------------------
NDBQueryRef NDBHandle::SQLiteCreateQuery(const NDBQuery &theQuery)
{	const char			*textUTF8;
	sqlite3_stmt		*sqlQuery;
	const char			*sqlTail;
	NString				theValue;
	sqlite3				*sqlDB;
	NDBStatus			dbErr;



	// Validate our parameters and state
	NN_ASSERT(IsOpen());
	NN_ASSERT(!theQuery.GetValue().IsEmpty());



	// Get the state we need
	theValue = theQuery.GetValue();
	textUTF8 = theValue.GetUTF8();
	
	sqlDB    = (sqlite3 *) mDatabase;
	sqlQuery = NULL;
	sqlTail  = NULL;



	// Create the query
	//
	// Preparing a query may require a lock, so we may need to spin if the
	// database is currently locked.
	//
	// Ideally this would also honour the timeout passed to Execute, but this
	// happens rarely enough that for now we treat it as uninterruptable.
	do
		{
		dbErr = sqlite3_prepare_v2(sqlDB, textUTF8, strlen(textUTF8), &sqlQuery, &sqlTail);
		if (dbErr == SQLITE_BUSY)
			NThread::Sleep();
		}
	while (dbErr == SQLITE_BUSY);



	// Handle failure
	if (dbErr != kNoErr)
		NN_LOG("SQLite: %s (%s)", sqlite3_errmsg(sqlDB), textUTF8);

	if (sqlQuery == NULL)
		return(NULL);

	return(sqlQuery);
}





//============================================================================
//		NDBHandle::SQLiteDestroyQuery : Destroy an SQLite query.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteDestroyQuery(NDBQueryRef theQuery)
{	sqlite3_stmt	*sqlQuery;
	NDBStatus		dbErr;



	// Get the state we need
	sqlQuery = (sqlite3_stmt *) theQuery;



	// Destroy the query
	if (sqlQuery != NULL)
		{
		dbErr = sqlite3_finalize(sqlQuery);
		NN_ASSERT(dbErr == SQLITE_OK);
		}
}





//============================================================================
//		NDBHandle::SQLiteDestroyQueries : Destroy any cached queries.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteDestroyQueries(void)
{	NDBHandleThreadCacheMap				*threadCache;
	NIndex								n, numItems;
	NDBHandleThreadCacheMapIterator		theIter;



	// Prepare to update
	sThreadCaches.Lock();



	// Update the table
	//
	// The cache table contains a list of per-thread state, where each thread
	// maintains a per-file cache that it can update without locks.
	//
	// When a file is closed, all of the references to that file must be purged
	// from any thread that has acessed it (and once all references from a thread
	// have been removed, the thread's entry can be purged).
	numItems = sThreadCaches.GetSize();
	n        = 0;

	while (n < numItems)
		{
		// Get the state we need
		threadCache = sThreadCaches.GetValue(n);
		theIter     = threadCache->find(this);


		// Purge this file
		//
		// If a thread has accessed this file, the entry must be removed.
		if (theIter != threadCache->end())
			{
			SQLiteDestroyQuery(theIter->second.theQuery);
			threadCache->erase(theIter);
			}


		// Purge the thread
		//
		// If the thread hasn't referenced any other files, it can be removed.
		if (threadCache->empty())
			{
			sThreadCaches.RemoveValue(threadCache);
			NThread::SetLocalValue(sThreadKey, NULL);

			delete threadCache;
			numItems--;
			}
		else
			n++;
		}



	// Finish the update
	sThreadCaches.Unlock();
}





//============================================================================
//		NDBHandle::SQLiteBindParameters : Bind the query parameters.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteBindParameters(NDBQueryRef theQuery, const NVariant &theParameters)
{	NArray			paramsArray;
	NDictionary		paramsDict;
	sqlite3_stmt	*sqlQuery;



	// Get the state we need
	sqlQuery = (sqlite3_stmt *) theQuery;



	// Bind the parameters
	sqlite3_clear_bindings(sqlQuery);
	
	if (theParameters.IsValid())
		{
		if (theParameters.GetValue(paramsArray))
			paramsArray.ForEach(BindSelf(NDBHandle::SQLiteBindParameterByIndex, sqlQuery, _1, _2));

		else if (theParameters.GetValue(paramsDict))
			paramsDict.ForEach(BindSelf(NDBHandle::SQLiteBindParameterByKey,    sqlQuery, _1, _2));
		}
}





//============================================================================
//		NDBHandle::SQLiteBindParameterByIndex : Bind a parameter to a query.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteBindParameterByIndex(NDBQueryRef theQuery, NIndex theIndex, const NVariant &theValue)
{	Float64				valueFloat64;
	SInt64				valueSInt64;
	NString				valueString;
	NNumber				valueNumber;
	NData				valueData;
	bool				valueBool;
	sqlite3_stmt		*sqlQuery;
	sqlite3				*sqlDB;
	NDBStatus			dbErr;



	// Validate our parameters
	NN_ASSERT(theIndex >= 0);



	// Get the state we need
	sqlQuery = (sqlite3_stmt *) theQuery;
	sqlDB    = (sqlite3      *) mDatabase;
	dbErr    = kNoErr;
	
	theIndex += 1;



	// Bind the parameter
	if (theValue.IsNumeric())
		{
		valueNumber = NNumber(theValue);
		if (valueNumber.IsInteger())
			{
			valueSInt64 = valueNumber.GetSInt64();
			dbErr       = sqlite3_bind_int64(sqlQuery, theIndex, valueSInt64);
			}
		else
			{
			valueFloat64 = valueNumber.GetFloat64();
			dbErr        = sqlite3_bind_double(sqlQuery, theIndex, valueFloat64);
			}
		}

	else if (theValue.GetValue(valueBool))
		dbErr = sqlite3_bind_int(sqlQuery, theIndex, valueBool ? 1 : 0);

	else if (theValue.GetValue(valueString))
		{
		valueData = valueString.GetData(kNStringEncodingUTF8);
		dbErr     = sqlite3_bind_text(sqlQuery, theIndex, (const char *) valueData.GetData(), valueData.GetSize(), SQLITE_TRANSIENT);
		}

	else if (theValue.GetValue(valueData))
		dbErr = sqlite3_bind_blob(sqlQuery, theIndex, valueData.GetData(), valueData.GetSize(), SQLITE_TRANSIENT);
	
	else
		NN_LOG("Unable to bind index '%ld' to query", theIndex);



	// Handle failure
	if (dbErr != kNoErr)
		NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));
}





//============================================================================
//		NDBHandle::SQLiteBindParameterByKey : Bind a parameter to a query.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteBindParameterByKey(NDBQueryRef theQuery, const NString &theKey, const NVariant &theValue)
{	UTF8List			textBuffer;
	const char			*textUTF8;
	sqlite3_stmt		*sqlQuery;
	NIndex				theIndex;



	// Get the state we need
	sqlQuery = (sqlite3_stmt *) theQuery;



	// Construct the name
	//
	// Using an NString to construct the name can be a performance bottleneck
	// when performing a very large number of queries (e.g., storing a GPS
	// track's points into a database).
	//
	// As such we use a simple UTF8Char array to do the append, since we know
	// we need the string encoded to UTF8 anyway.
	textUTF8 = theKey.GetUTF8();
	textBuffer.resize(strlen(textUTF8) + 2);

	textBuffer[0] = kNamePrefix;
	strcpy((char *) &textBuffer[1], textUTF8);



	// Get the index
	//
	// Unused parameters are ignored, to allow the same dictionary to be used
	// for multiple queries (not all of which may use all of the values).
	theIndex = sqlite3_bind_parameter_index(sqlQuery, (const char *) &textBuffer[0]);
	if (theIndex == 0)
		return;



	// Bind the parameter
	//
	// SQLite parameters are 1-based, but as SQLiteBindParameterByIndex is also
	// called with 0-based indices from NArray::ForEach we need to adjust.
	SQLiteBindParameterByIndex(theQuery, theIndex-1, theValue);
}





//============================================================================
//		NDBHandle::SQLiteGetStatus : Convert an SQLite status.
//----------------------------------------------------------------------------
NStatus NDBHandle::SQLiteGetStatus(NDBStatus dbErr)
{	NStatus	theErr;



	// Translate the error
	switch (dbErr) {
		case SQLITE_OK:
		case SQLITE_DONE:
			theErr = kNoErr;
			break;

		case SQLITE_ERROR:
		case SQLITE_INTERNAL:
			theErr = kNErrParam;
			break;

		case SQLITE_PERM:
		case SQLITE_READONLY:
			theErr = kNErrPermission;
			break;
		
		case SQLITE_ABORT:
		case SQLITE_INTERRUPT:
			theErr = kNErrCancelled;
			break;
		
		case SQLITE_BUSY:
			theErr = kNErrBusy;
			break;
		
		case SQLITE_LOCKED:
			theErr = kNErrLocked;
			break;
		
		case SQLITE_NOMEM:
			theErr = kNErrMemory;
			break;

		case SQLITE_IOERR:
		case SQLITE_CORRUPT:
			theErr = kNErrMalformed;
			break;
		
		case SQLITE_NOTFOUND:
		case SQLITE_CANTOPEN:
			theErr = kNErrNotFound;
			break;
		
		default:
			NN_LOG("NDBHandle unable to map error %d", dbErr);
			theErr = kNErrInternal;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NDBHandle::SQLiteProgressBegin : Begin an SQLite operation.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteProgressBegin(void)
{	sqlite3		*sqlDB;



	// Get the state we need
	sqlDB = (sqlite3 *) mDatabase;



	// Begin the operation
	sqlite3_progress_handler(sqlDB, kProgressUpdate, SQLiteProgressContinue, this);
	
	BeginProgress(kNProgressUnknown);
}





//============================================================================
//		NDBHandle::SQLiteProgressEnd : End an SQLite operation.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteProgressEnd(void)
{	sqlite3		*sqlDB;



	// Get the state we need
	sqlDB = (sqlite3 *) mDatabase;



	// End the operation
	sqlite3_progress_handler(sqlDB, 0, NULL, NULL);
	
	EndProgress();
}





//============================================================================
//		NDBHandle::SQLiteProgressContinue : SQLite progress callback.
//----------------------------------------------------------------------------
int NDBHandle::SQLiteProgressContinue(void *userData)
{	NDBHandle		*thisPtr;
	NStatus			theErr;



	// Update the progress
	thisPtr = (NDBHandle *) userData;
	theErr  = thisPtr->ContinueProgress(kNProgressUnknown);

	return((theErr == kNoErr) ? SQLITE_OK : SQLITE_ABORT);
}




