/*	NAME:
		NDBFile.cpp

	DESCRIPTION:
		Database file.
	
	COPYRIGHT:
		Copyright (c) 2007-2008, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NThread.h"
#include "NDBFile.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32  kProgressUpdate								= 1000;
static const NString kNamePrefix									= ":";





//============================================================================
//		NDBFile::NDBFile : Constructor.
//----------------------------------------------------------------------------
NDBFile::NDBFile(void)
{


	// Validate our state
	NN_ASSERT(sqlite3_threadsafe());



	// Initialize ourselves
	mDatabase = NULL;
}





//============================================================================
//		NDBFile::~NDBFile : Destructor.
//----------------------------------------------------------------------------
NDBFile::~NDBFile(void)
{


	// Clean up
	if (IsOpen())
		Close();
}





//============================================================================
//		NDBFile::IsOpen : Is the database open?
//----------------------------------------------------------------------------
bool NDBFile::IsOpen(void) const
{


	// Get our state
	return(mDatabase != NULL);
}





//============================================================================
//		NDBFile::Open : Open the database.
//----------------------------------------------------------------------------
OSStatus NDBFile::Open(const NFile &theFile, bool readOnly, const NString &theVFS)
{	int				theFlags;
	const char		*vfsName;
	NString			thePath;
	SQLError		sqlErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the state we need
	vfsName  = theVFS.IsEmpty() ? NULL : theVFS.GetUTF8();
	theFlags = readOnly ? SQLITE_OPEN_READONLY : (SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
	thePath  = theFile.GetPath();



	// Open the database
	sqlErr  = sqlite3_open_v2(thePath.GetUTF8(), &mDatabase, theFlags, vfsName);
	if (sqlErr != noErr)
		NN_LOG("SQLite: %s", sqlite3_errmsg(mDatabase));

	if (sqlErr == SQLITE_OK)
		sqlite3_progress_handler(mDatabase, kProgressUpdate, DBProgress, this);
	
	return(GetSQLError(sqlErr));
}





//============================================================================
//		NDBFile::Close : Close the database.
//----------------------------------------------------------------------------
void NDBFile::Close(void)
{	SQLError	sqlErr;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Close the database
	sqlErr = sqlite3_close(mDatabase);
	if (sqlErr != noErr)
		NN_LOG("SQLite: %s", sqlite3_errmsg(mDatabase));



	// Reset our state
	mDatabase = NULL;
}





//============================================================================
//		NDBFile::GetProgressFunctor : Get the progress functor.
//----------------------------------------------------------------------------
NDBProgressFunctor NDBFile::GetProgressFunctor(void) const
{


	// Get the progress functor
	return(mProgress);
}





//============================================================================
//		NDBFile::SetProgressFunctor : Set the progress functor.
//----------------------------------------------------------------------------
void NDBFile::SetProgressFunctor(const NDBProgressFunctor &theFunctor)
{


	// Set the progress functor
	mProgress = theFunctor;
}





//============================================================================
//		NDBFile::Execute : Execute a statement.
//----------------------------------------------------------------------------
OSStatus NDBFile::Execute(const NDBStatement &theStatement, const NDBResultFunctor &theResult, EventTime waitFor)
{	bool			areDone, waitForever;
	sqlite3_stmt	*sqlStatement;
	EventTime		startTime;
	SQLError		sqlErr;



	// Get the state we need
	waitForever  = NMathUtilities::AreEqual(waitFor, kEventDurationForever);
	sqlStatement = GetSQLStatement(theStatement);

	if (sqlStatement == NULL)
		return(paramErr);



	// Execute the statement
	startTime = GetCurrentEventTime();
	areDone   = false;
	
	while (!areDone)
		{
		// Execute the statement
		sqlErr = sqlite3_step(sqlStatement);

		switch (sqlErr) {
			case SQLITE_ROW:
				if (theResult != NULL)
					theResult(sqlStatement);
				break;
			
			case SQLITE_BUSY:
				areDone = !waitForever && (GetCurrentEventTime() >= (startTime + waitFor));
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

	return(GetSQLError(sqlErr));
}





//============================================================================
//		NDBFile::ExecuteSInt32 : Execute a statement to obtain an SInt32.
//----------------------------------------------------------------------------
SInt32 NDBFile::ExecuteSInt32(const NDBStatement &theStatement)
{	SInt32		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueSInt32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFile::ExecuteSInt64 : Execute a statement to obtain an SInt64.
//----------------------------------------------------------------------------
SInt64 NDBFile::ExecuteSInt64(const NDBStatement &theStatement)
{	SInt64		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueSInt64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFile::ExecuteFloat32 : Execute a statement to obtain a Float32.
//----------------------------------------------------------------------------
Float32 NDBFile::ExecuteFloat32(const NDBStatement &theStatement)
{	Float32		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0.0f;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueFloat32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFile::ExecuteFloat64 : Execute a statement to obtain a Float64.
//----------------------------------------------------------------------------
Float64 NDBFile::ExecuteFloat64(const NDBStatement &theStatement)
{	Float64		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0.0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueFloat64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFile::ExecuteString : Execute a statement to obtain a string.
//----------------------------------------------------------------------------
NString NDBFile::ExecuteString(const NDBStatement &theStatement)
{	NString		theValue;
	OSStatus	theErr;



	// Execute the statement
	theErr = Execute(theStatement, BindFunction(NDBResult::GetRowValueString, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFile::ExecuteData : Execute a statement to obtain data.
//----------------------------------------------------------------------------
NData NDBFile::ExecuteData(const NDBStatement &theStatement)
{	NData		theValue;
	OSStatus	theErr;



	// Execute the statement
	theErr = Execute(theStatement, BindFunction(NDBResult::GetRowValueData, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFile::Cancel : Cancel the current operation.
//----------------------------------------------------------------------------
void NDBFile::Cancel(void)
{


	// Validate our state
	NN_ASSERT(IsOpen());



	// Cancel the operation
	sqlite3_interrupt(mDatabase);
}





//============================================================================
//		NDBFile::GetDatabase : Get the database.
//----------------------------------------------------------------------------
void *NDBFile::GetDatabase(void)
{


	// Get the database
	return(mDatabase);
}





//============================================================================
//		NDBFile::GetSQLError : Get an SQLite error code.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NDBFile::GetSQLError(SQLError sqlErr)
{	OSStatus	theErr;



	// Translate the error
	switch (sqlErr) {
		case SQLITE_OK:
		case SQLITE_DONE:
			theErr = noErr;
			break;

		case SQLITE_ERROR:
		case SQLITE_INTERNAL:
			theErr = paramErr;
			break;

		case SQLITE_PERM:
			theErr = permErr;
			break;
		
		case SQLITE_ABORT:
		case SQLITE_INTERRUPT:
			theErr = userCanceledErr;
			break;
		
		case SQLITE_BUSY:
			theErr = fBsyErr;
			break;
		
		case SQLITE_LOCKED:
			theErr = fLckdErr;
			break;
		
		case SQLITE_NOMEM:
			theErr = memFullErr;
			break;
		
		case SQLITE_READONLY:
			theErr = wrPermErr;
			break;

		case SQLITE_IOERR:
			theErr = ioErr;
			break;

		case SQLITE_CORRUPT:
			theErr = badFileFormat;
			break;
		
		case SQLITE_NOTFOUND:
			theErr = resNotFound;
			break;
		
		case SQLITE_CANTOPEN:
			theErr = openErr;
			break;
		
		default:
			NN_LOG("NDBFile unable to map error %d", sqlErr);
			theErr = corErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NDBFile::GetSQLStatement : Get an SQLite statement.
//----------------------------------------------------------------------------
sqlite3_stmt *NDBFile::GetSQLStatement(const NDBStatement &theStatement)
{	sqlite3_stmt		*sqlStatement;
	NDictionary			theParameters;
	NString				theValue;
	const char			*sqlTail;
	SQLError			sqlErr;



	// Validate our parameters and state
	NN_ASSERT(IsOpen());
	NN_ASSERT(theStatement.GetValue().IsNotEmpty());



	// Get the state we need
	theValue      = theStatement.GetValue();
	theParameters = theStatement.GetParameters();

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
		sqlErr = sqlite3_prepare_v2(mDatabase, theValue.GetUTF8(), theValue.GetSizeBytes(), &sqlStatement, &sqlTail);
		if (sqlErr == SQLITE_BUSY)
			NThread::Sleep();
		}
	while (sqlErr == SQLITE_BUSY);



	// Handle failure
	if (sqlErr != noErr)
		NN_LOG("SQLite: %s (%s)", sqlite3_errmsg(mDatabase), theValue.GetUTF8());

	if (sqlStatement == NULL)
		return(NULL);



	// Bind the parameters
	if (theParameters.IsNotEmpty())
		BindParameters(sqlStatement, theParameters);
	
	return(sqlStatement);
}





//============================================================================
//		NDBFile::BindParameters : Bind parameters to a statement.
//----------------------------------------------------------------------------
void NDBFile::BindParameters(sqlite3_stmt *sqlStatement, const NDictionary &theParameters)
{	NString							theKey, theName;
	SInt64							valueInteger;
	NString							valueString;
	Float64							valueFloat;
	NData							valueData;
	int								theIndex;
	NCFObject						theValue;
	bool							isValid;
	NStringList						theKeys;
	NStringListConstIterator		theIter;
	CFTypeID						theType;
	SQLError						sqlErr;



	// Get the state we need
	theKeys = theParameters.GetKeys();



	// Bind the parameters
	for (theIter = theKeys.begin(); theIter != theKeys.end(); theIter++)
		{
		// Get the state we need
		theKey   = *theIter;
		theValue = theParameters.GetValue(theKey);
		theType  = theValue.GetType();



		// Get the index
		//
		// Unused parameters are ignored, to allow the same dictionary to be used
		// for multiple statements (not all of which may use all of the values).
		theName  = kNamePrefix + theKey;
		theIndex = sqlite3_bind_parameter_index(sqlStatement, theName.GetUTF8());
		
		if (theIndex == 0)
			continue;



		// Bind a number
		if (theType == CFNumberGetTypeID())
			{
			if (CFNumberIsFloatType(theValue))
				{
				isValid = CFNumberGetValue(theValue, kCFNumberFloat64Type, &valueFloat);
				sqlErr  = sqlite3_bind_double(sqlStatement, theIndex, valueFloat);

				NN_ASSERT(isValid);
				if (sqlErr != noErr)
					NN_LOG("SQLite: %s", sqlite3_errmsg(mDatabase));
				}
			else
				{
				isValid = CFNumberGetValue(theValue, kCFNumberSInt64Type, &valueInteger);
				sqlErr  = sqlite3_bind_int64(sqlStatement, theIndex, valueInteger);

				NN_ASSERT(isValid);
				if (sqlErr != noErr)
					NN_LOG("SQLite: %s", sqlite3_errmsg(mDatabase));
				}
			}
		
		
		// Bind a string
		else if (theType == CFStringGetTypeID())
			{
			valueString = NString(theValue);
			sqlErr      = sqlite3_bind_text(sqlStatement, theIndex, valueString.GetUTF8(), valueString.GetSizeBytes(), SQLITE_TRANSIENT);
			if (sqlErr != noErr)
				NN_LOG("SQLite: %s", sqlite3_errmsg(mDatabase));
			}
		
		
		// Bind some data
		else if (theType == CFDataGetTypeID())
			{
			valueData = NData(theValue);
			sqlErr    = sqlite3_bind_blob(sqlStatement, theIndex, valueData.GetData(), valueData.GetSize(), SQLITE_TRANSIENT);
			if (sqlErr != noErr)
				NN_LOG("SQLite: %s", sqlite3_errmsg(mDatabase));
			}
		
		else
			NN_LOG("Unable to bind '%s' (%d) to statement", theKey.GetUTF8(), theType);
		}
}





//============================================================================
//		NDBFile::DBProgress : Progress callback.
//----------------------------------------------------------------------------
SQLError NDBFile::DBProgress(void *userData)
{	bool		canContinue;
	NDBFile		*thisPtr;



	// Get the state we need
	thisPtr     = (NDBFile *) userData;
	canContinue = true;



	// Invoke the functor
	if (thisPtr->mProgress != NULL)
		canContinue = thisPtr->mProgress();

	return(canContinue ? SQLITE_OK : SQLITE_ABORT);
}




