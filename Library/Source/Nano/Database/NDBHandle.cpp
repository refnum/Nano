/*	NAME:
		NDBHandle.cpp

	DESCRIPTION:
		Database handle.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
#include "NThread.h"
#include "NDBHandle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32  kProgressUpdate								= 1000;
static const NString kNamePrefix									= ":";





//============================================================================
//		NDBHandle::NDBHandle : Constructor.
//----------------------------------------------------------------------------
NDBHandle::NDBHandle(void)
{


	// Validate our state
	NN_ASSERT(sqlite3_threadsafe());



	// Initialize ourselves
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
//		NDBHandle::Open : Open the database.
//----------------------------------------------------------------------------
NStatus NDBHandle::Open(const NFile &theFile, bool readOnly, const NString &theVFS)
{	int				theFlags;
	const char		*vfsName;
	NString			thePath;
	sqlite3			*sqlDB;
	NDBStatus		dbErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the state we need
	vfsName  = theVFS.IsEmpty() ? NULL : theVFS.GetUTF8();
	theFlags = readOnly ? SQLITE_OPEN_READONLY : (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
	thePath  = theFile.GetPath();



	// Open the database
	dbErr = sqlite3_open_v2(thePath.GetUTF8(), &sqlDB, theFlags, vfsName);
	if (dbErr != kNoErr)
		NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));

	if (dbErr == SQLITE_OK)
		sqlite3_progress_handler(sqlDB, kProgressUpdate, SQLiteProgress, this);
	
	
	
	// Update our state
	mDatabase = sqlDB;
	
	return(SQLiteGetStatus(dbErr));
}





//============================================================================
//		NDBHandle::Close : Close the database.
//----------------------------------------------------------------------------
void NDBHandle::Close(void)
{	sqlite3		*sqlDB;
	NDBStatus	dbErr;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Close the database
	sqlDB = (sqlite3 *) mDatabase;
	dbErr = sqlite3_close(sqlDB);

	if (dbErr != kNoErr)
		NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));



	// Reset our state
	mDatabase = NULL;
}





//============================================================================
//		NDBHandle::GetProgressFunctor : Get the progress functor.
//----------------------------------------------------------------------------
NDBProgressFunctor NDBHandle::GetProgressFunctor(void) const
{


	// Get the progress functor
	return(mProgress);
}





//============================================================================
//		NDBHandle::SetProgressFunctor : Set the progress functor.
//----------------------------------------------------------------------------
void NDBHandle::SetProgressFunctor(const NDBProgressFunctor &theFunctor)
{


	// Set the progress functor
	mProgress = theFunctor;
}





//============================================================================
//		NDBHandle::Execute : Execute a statement.
//----------------------------------------------------------------------------
NStatus NDBHandle::Execute(const NDBStatement &theStatement, const NDBResultFunctor &theResult, NTime waitFor)
{	bool				areDone, waitForever;
	sqlite3_stmt		*sqlStatement;
	NTime				startTime;
	NDBStatus			dbErr;



	// Get the state we need
	waitForever  = NMathUtilities::AreEqual(waitFor, kNTimeForever);
	sqlStatement = (sqlite3_stmt *) SQLiteCreateStatement(theStatement);

	if (sqlStatement == NULL)
		return(kNErrParam);



	// Execute the statement
	startTime = NTimeUtilities::GetTime();
	areDone   = false;
	
	while (!areDone)
		{
		// Execute the statement
		dbErr = sqlite3_step(sqlStatement);

		switch (dbErr) {
			case SQLITE_ROW:
				if (theResult != NULL)
					theResult((NDBStatementRef) sqlStatement);
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



	// Clean up
	(void) sqlite3_finalize(sqlStatement);

	return(SQLiteGetStatus(dbErr));
}





//============================================================================
//		NDBHandle::ExecuteSInt32 : Execute a statement to obtain an SInt32.
//----------------------------------------------------------------------------
SInt32 NDBHandle::ExecuteSInt32(const NDBStatement &theStatement)
{	SInt32		theValue;
	NStatus		theErr;



	// Execute the statement
	theValue = 0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueSInt32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteSInt64 : Execute a statement to obtain an SInt64.
//----------------------------------------------------------------------------
SInt64 NDBHandle::ExecuteSInt64(const NDBStatement &theStatement)
{	SInt64		theValue;
	NStatus		theErr;



	// Execute the statement
	theValue = 0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueSInt64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteFloat32 : Execute a statement to obtain a Float32.
//----------------------------------------------------------------------------
Float32 NDBHandle::ExecuteFloat32(const NDBStatement &theStatement)
{	Float32		theValue;
	NStatus		theErr;



	// Execute the statement
	theValue = 0.0f;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueFloat32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteFloat64 : Execute a statement to obtain a Float64.
//----------------------------------------------------------------------------
Float64 NDBHandle::ExecuteFloat64(const NDBStatement &theStatement)
{	Float64		theValue;
	NStatus		theErr;



	// Execute the statement
	theValue = 0.0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueFloat64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteString : Execute a statement to obtain a string.
//----------------------------------------------------------------------------
NString NDBHandle::ExecuteString(const NDBStatement &theStatement)
{	NString		theValue;
	NStatus		theErr;



	// Execute the statement
	theErr = Execute(theStatement, BindFunction(NDBResult::GetRowValueString, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandle::ExecuteData : Execute a statement to obtain data.
//----------------------------------------------------------------------------
NData NDBHandle::ExecuteData(const NDBStatement &theStatement)
{	NData		theValue;
	NStatus		theErr;



	// Execute the statement
	theErr = Execute(theStatement, BindFunction(NDBResult::GetRowValueData, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
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
//		NDBHandle::SQLiteCreateStatement : Get an SQLite statement.
//----------------------------------------------------------------------------
#pragma mark -
NDBStatementRef NDBHandle::SQLiteCreateStatement(const NDBStatement &theStatement)
{	sqlite3_stmt		*sqlStatement;
	NDictionary			theParameters;
	const char			*valueUTF8;
	const char			*sqlTail;
	NString				theValue;
	sqlite3				*sqlDB;
	NDBStatus			dbErr;



	// Validate our parameters and state
	NN_ASSERT(IsOpen());
	NN_ASSERT(!theStatement.GetValue().IsEmpty());



	// Get the state we need
	theParameters = theStatement.GetParameters();
	theValue      = theStatement.GetValue();
	valueUTF8     = theValue.GetUTF8();
	
	sqlDB        = (sqlite3 *) mDatabase;
	sqlStatement = NULL;
	sqlTail      = NULL;



	// Create the statement
	//
	// Preparing a statement may require a lock, so we may need to spin if
	// the database is currently locked.
	//
	// Ideally this would also honour the timeout passed to Execute, but this
	// happens rarely enough that for now we treat it as uninterruptable.
	do
		{
		dbErr = sqlite3_prepare_v2(sqlDB, valueUTF8, strlen(valueUTF8), &sqlStatement, &sqlTail);
		if (dbErr == SQLITE_BUSY)
			NThread::Sleep();
		}
	while (dbErr == SQLITE_BUSY);



	// Handle failure
	if (dbErr != kNoErr)
		NN_LOG("SQLite: %s (%s)", sqlite3_errmsg(sqlDB), valueUTF8);

	if (sqlStatement == NULL)
		return(NULL);



	// Bind the parameters
	if (!theParameters.IsEmpty())
		SQLiteBindParameters(sqlStatement, theParameters);
	
	return(sqlStatement);
}





//============================================================================
//		NDBHandle::SQLiteBindParameters : Bind parameters to a statement.
//----------------------------------------------------------------------------
void NDBHandle::SQLiteBindParameters(NDBStatementRef theStatement, const NDictionary &theParameters)
{	NString							theKey, theName;
	sqlite3_stmt					*sqlStatement;
	Float64							valueFloat64;
	SInt64							valueSInt64;
	NString							valueString;
	NNumber							valueNumber;
	NData							valueData;
	int								theIndex;
	NVariant						theValue;
	NStringList						theKeys;
	NStringListConstIterator		theIter;
	sqlite3							*sqlDB;
	NDBStatus						dbErr;



	// Get the state we need
	theKeys      = theParameters.GetKeys();
	sqlStatement = (sqlite3_stmt *) theStatement;
	sqlDB        = (sqlite3      *) mDatabase;
	


	// Bind the parameters
	for (theIter = theKeys.begin(); theIter != theKeys.end(); theIter++)
		{
		// Get the state we need
		theKey   = *theIter;
		theValue = theParameters.GetValue(theKey);



		// Get the index
		//
		// Unused parameters are ignored, to allow the same dictionary to be used
		// for multiple statements (not all of which may use all of the values).
		theName  = kNamePrefix + theKey;
		theIndex = sqlite3_bind_parameter_index(sqlStatement, theName.GetUTF8());
		
		if (theIndex == 0)
			continue;



		// Bind a number
		if (theValue.IsNumeric())
			{
			valueNumber = NNumber(theValue);
			
			if (valueNumber.GetSInt64(valueSInt64))
				{
				dbErr= sqlite3_bind_int64(sqlStatement, theIndex, valueSInt64);
				if (dbErr != kNoErr)
					NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));
				}

			else if (valueNumber.GetFloat64(valueFloat64))
				{
				dbErr = sqlite3_bind_double(sqlStatement, theIndex, valueFloat64);
				if (dbErr != kNoErr)
					NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));
				}
			
			else
				NN_LOG("Unable to convert '%@' to number", theKey);
			}
		
		
		// Bind a string
		else if (theValue.GetValue(valueString))
			{
			dbErr = sqlite3_bind_text(sqlStatement, theIndex, valueString.GetUTF8(), strlen(valueString.GetUTF8()), SQLITE_TRANSIENT);
			if (dbErr != kNoErr)
				NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));
			}
		
		
		// Bind some data
		else if (theValue.GetValue(valueData))
			{
			dbErr = sqlite3_bind_blob(sqlStatement, theIndex, valueData.GetData(), valueData.GetSize(), SQLITE_TRANSIENT);
			if (dbErr != kNoErr)
				NN_LOG("SQLite: %s", sqlite3_errmsg(sqlDB));
			}
		
		else
			NN_LOG("Unable to bind '%@' to statement", theKey);
		}
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
//		NDBHandle::SQLiteProgress : SQLite progress callback.
//----------------------------------------------------------------------------
int NDBHandle::SQLiteProgress(void *userData)
{	bool			canContinue;
	NDBHandle		*thisPtr;



	// Get the state we need
	thisPtr     = (NDBHandle *) userData;
	canContinue = true;



	// Invoke the functor
	if (thisPtr->mProgress != NULL)
		canContinue = thisPtr->mProgress();

	return(canContinue ? SQLITE_OK : SQLITE_ABORT);
}




