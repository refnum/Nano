/*	NAME:
		NDBFilePool.cpp

	DESCRIPTION:
		Pooled access to a database file.
		
		Provides thread-safe access to a database file, either as a single
		exclusive connection or a pool of connections shared between threads.

	COPYRIGHT:
		Copyright (c) 2007-2008, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDBFilePool.h"





//============================================================================
//		NDBFilePool::NDBFilePool : Constructor.
//----------------------------------------------------------------------------
NDBFilePool::NDBFilePool(void)
{


	// Initialise ourselves
	mIsOpen = false;
	
	mConnectOnce = false;
	mReadOnly    = false;
}





//============================================================================
//		NDBFilePool::~NDBFilePool : Destructor.
//----------------------------------------------------------------------------
NDBFilePool::~NDBFilePool(void)
{


	// Clean up
	if (IsOpen())
		Close();
}





//============================================================================
//		NDBFilePool::IsOpen : Is the database open?
//----------------------------------------------------------------------------
bool NDBFilePool::IsOpen(void) const
{


	// Get our state
	return(mIsOpen);
}





//============================================================================
//		NDBFilePool::GetConnector : Get the connector.
//----------------------------------------------------------------------------
NDBFileConnector NDBFilePool::GetConnector(void)
{


	// Get the connector
	return(mConnector);
}





//============================================================================
//		NDBFilePool::SetConnector : Set the connector.
//----------------------------------------------------------------------------
void NDBFilePool::SetConnector(const NDBFileConnector &theConnector)
{


	// Set the connector
	mConnector = theConnector;
}





//============================================================================
//		NDBFilePool::Open : Open the database.
//----------------------------------------------------------------------------
OSStatus NDBFilePool::Open(const NFile &theFile, bool connectOnce, bool readOnly, const NString &theVFS)
{	NDBFilePtr		dbFile;
	OSStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(theFile.IsValid());
	NN_ASSERT(!IsOpen());
	


	// Update our state
	mFile        = theFile;
	mConnectOnce = connectOnce;
	mReadOnly    = readOnly;
	mVFS         = theVFS;



	// Create the initial connection
	//
	// The initial connection allows us to test that we can actually open the
	// database, and may be all that we need if we're only to connect once.
	theErr = CreateConnection(dbFile);
	NN_ASSERT_NOERR(theErr);

	mIsOpen = (dbFile != NULL);

	if (mIsOpen)
		mPool.PushBack(dbFile);

	return(theErr);
}





//============================================================================
//		NDBFilePool::Close : Close the database.
//----------------------------------------------------------------------------
void NDBFilePool::Close(void)
{	NDBFilePtr	theFile;



	// Validate our state
	NN_ASSERT(mIsOpen);



	// Clean up
	//
	// All database connections must be released at this point, so we can
	// empty the pool to close the connections.
	while (mPool.PopFront(theFile))
		delete theFile;



	// Reset our state
	mIsOpen = false;
}





//============================================================================
//		NDBFilePool::AcquireConnection : Acquire a connection.
//----------------------------------------------------------------------------
bool NDBFilePool::AcquireConnection(NDBFilePtr &dbFile)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mIsOpen);



	// Acquire a single connection
	//
	// A pool always starts with one connection, so we can block until we
	// are able to acquire the lock and extract this connection.
	//
	// The list will be unlocked when we release the connection, allowing
	// one of any of the threads then blocked on the lock to continue.
	if (mConnectOnce)
		{
		mPool.Lock();

		dbFile = mPool.GetValue(0);
		NN_ASSERT(dbFile != NULL);
		}



	// Acquire one of multiple connections
	//
	// Connections are created on demand, with no cap (the assumption is that
	// the number of threads acquiring connections will be capped externally
	// via an NThreadPool or similar).
	//
	// Newly created connections are owned by the caller, so will be added to
	// the pool when that connection is eventually released.
	else
		{
		if (!mPool.PopFront(dbFile))
			{
			theErr = CreateConnection(dbFile);
			NN_ASSERT_NOERR(theErr);
			}
		}

	return(dbFile != NULL);
}





//============================================================================
//		NDBFilePool::ReleaseConnection : Releae a connection.
//----------------------------------------------------------------------------
void NDBFilePool::ReleaseConnection(NDBFilePtr &dbFile)
{


	// Validate our parameters and state
	NN_ASSERT(dbFile != NULL);
	NN_ASSERT(mIsOpen);



	// Release a single connection
	if (mConnectOnce)
		{
		NN_ASSERT(dbFile == mPool.GetValue(0));
		mPool.Unlock();
		}
	
	
	
	// Release one of multiple connections
	else
		mPool.PushBack(dbFile);



	// Clear the parameter
	dbFile = NULL;
}





//============================================================================
//		NDBFilePool::Execute : Execute a statement.
//----------------------------------------------------------------------------
OSStatus NDBFilePool::Execute(const NDBStatement &theStatement, const NDBResultFunctor &theResult, EventTime waitFor)
{	NDBFile		*dbFile;
	OSStatus	theErr;



	// Execute the statement
	theErr = memFullErr;
	
	if (AcquireConnection(dbFile))
		{
		theErr = dbFile->Execute(theStatement, theResult, waitFor);
		ReleaseConnection(dbFile);
		}
	
	return(theErr);
}





//============================================================================
//		NDBFilePool::ExecuteSInt32 : Execute a statement to obtain an SInt32.
//----------------------------------------------------------------------------
SInt32 NDBFilePool::ExecuteSInt32(const NDBStatement &theStatement)
{	SInt32		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueSInt32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFilePool::ExecuteSInt64 : Execute a statement to obtain an SInt64.
//----------------------------------------------------------------------------
SInt64 NDBFilePool::ExecuteSInt64(const NDBStatement &theStatement)
{	SInt64		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueSInt64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFilePool::ExecuteFloat32 : Execute a statement to obtain a Float32.
//----------------------------------------------------------------------------
Float32 NDBFilePool::ExecuteFloat32(const NDBStatement &theStatement)
{	Float32		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0.0f;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueFloat32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFilePool::ExecuteFloat64 : Execute a statement to obtain a Float64.
//----------------------------------------------------------------------------
Float64 NDBFilePool::ExecuteFloat64(const NDBStatement &theStatement)
{	Float64		theValue;
	OSStatus	theErr;



	// Execute the statement
	theValue = 0.0;
	theErr   = Execute(theStatement, BindFunction(NDBResult::GetRowValueFloat64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFilePool::ExecuteString : Execute a statement to obtain a string.
//----------------------------------------------------------------------------
NString NDBFilePool::ExecuteString(const NDBStatement &theStatement)
{	NString		theValue;
	OSStatus	theErr;



	// Execute the statement
	theErr = Execute(theStatement, BindFunction(NDBResult::GetRowValueString, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFilePool::ExecuteData : Execute a statement to obtain data.
//----------------------------------------------------------------------------
NData NDBFilePool::ExecuteData(const NDBStatement &theStatement)
{	NData		theValue;
	OSStatus	theErr;



	// Execute the statement
	theErr = Execute(theStatement, BindFunction(NDBResult::GetRowValueData, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBFilePool::CreateConnection : Create a connection.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NDBFilePool::CreateConnection(NDBFilePtr &dbFile)
{	OSStatus	theErr;



	// Create the connection
	dbFile = new NDBFile;
	theErr = (dbFile == NULL) ? (OSStatus) memFullErr : (OSStatus) noErr;

	if (theErr == noErr)
		{
		theErr = dbFile->Open(mFile, mReadOnly, mVFS);
		NN_ASSERT_NOERR(theErr);
		}



	// Apply the connector
	if (theErr == noErr && mConnector != NULL)
		mConnector(*dbFile);



	// Handle failure
	if (theErr != noErr)
		{
		delete dbFile;
		dbFile = NULL;
		}
	
	return(theErr);
}

