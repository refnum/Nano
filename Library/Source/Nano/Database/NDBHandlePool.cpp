/*	NAME:
		NDBHandlePool.cpp

	DESCRIPTION:
		Pooled access to a database handle.
		
		Provides thread-safe access to a database handle, either as a single
		exclusive connection or a pool of connections shared between threads.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDBHandlePool.h"





//============================================================================
//		NDBHandlePool::NDBHandlePool : Constructor.
//----------------------------------------------------------------------------
NDBHandlePool::NDBHandlePool(void)
{


	// Initialise ourselves
	mIsOpen = false;
	
	mConnectOnce = false;
	mIsMutable   = false;
}





//============================================================================
//		NDBHandlePool::~NDBHandlePool : Destructor.
//----------------------------------------------------------------------------
NDBHandlePool::~NDBHandlePool(void)
{


	// Clean up
	if (IsOpen())
		Close();
}





//============================================================================
//		NDBHandlePool::IsOpen : Is the database open?
//----------------------------------------------------------------------------
bool NDBHandlePool::IsOpen(void) const
{


	// Get our state
	return(mIsOpen);
}





//============================================================================
//		NDBHandlePool::IsMutable : Is the database mutable?
//----------------------------------------------------------------------------
bool NDBHandlePool::IsMutable(void) const
{


	// Get our state
	return(mIsMutable);
}





//============================================================================
//		NDBHandlePool::GetConnector : Get the connector.
//----------------------------------------------------------------------------
NDBHandleConnector NDBHandlePool::GetConnector(void)
{


	// Get the connector
	return(mConnector);
}





//============================================================================
//		NDBHandlePool::SetConnector : Set the connector.
//----------------------------------------------------------------------------
void NDBHandlePool::SetConnector(const NDBHandleConnector &theConnector)
{


	// Set the connector
	mConnector = theConnector;
}





//============================================================================
//		NDBHandlePool::Open : Open the database.
//----------------------------------------------------------------------------
NStatus NDBHandlePool::Open(const NFile &theFile, bool connectOnce, bool readOnly, const NString &theVFS)
{	NDBHandlePtr		dbHandle;
	NStatus				theErr;



	// Validate our parameters and state
	NN_ASSERT(theFile.IsValid());
	NN_ASSERT(!IsOpen());
	


	// Update our state
	mConnectOnce = connectOnce;
	mIsMutable   = !readOnly;
	mFile        = theFile;
	mVFS         = theVFS;



	// Create the initial connection
	//
	// The initial connection allows us to test that we can actually open the
	// database, and may be all that we need if we're only to connect once.
	theErr = CreateConnection(dbHandle);
	NN_ASSERT_NOERR(theErr);

	mIsOpen = (dbHandle != NULL);

	if (mIsOpen)
		mPool.PushBack(dbHandle);

	return(theErr);
}





//============================================================================
//		NDBHandlePool::Close : Close the database.
//----------------------------------------------------------------------------
void NDBHandlePool::Close(void)
{	NDBHandlePtr	theFile;



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
//		NDBHandlePool::AcquireConnection : Acquire a connection.
//----------------------------------------------------------------------------
bool NDBHandlePool::AcquireConnection(NDBHandlePtr &dbHandle)
{	NStatus		theErr;



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

		dbHandle = mPool.GetValue(0);
		NN_ASSERT(dbHandle != NULL);
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
		if (!mPool.PopFront(dbHandle))
			{
			theErr = CreateConnection(dbHandle);
			NN_ASSERT_NOERR(theErr);
			}
		}

	return(dbHandle != NULL);
}





//============================================================================
//		NDBHandlePool::ReleaseConnection : Releae a connection.
//----------------------------------------------------------------------------
void NDBHandlePool::ReleaseConnection(NDBHandlePtr &dbHandle)
{


	// Validate our parameters and state
	NN_ASSERT(dbHandle != NULL);
	NN_ASSERT(mIsOpen);



	// Release a single connection
	if (mConnectOnce)
		{
		NN_ASSERT(dbHandle == mPool.GetValue(0));
		mPool.Unlock();
		}
	
	
	
	// Release one of multiple connections
	else
		mPool.PushBack(dbHandle);



	// Clear the parameter
	dbHandle = NULL;
}





//============================================================================
//		NDBHandlePool::Execute : Execute a query.
//----------------------------------------------------------------------------
NStatus NDBHandlePool::Execute(const NDBQuery &theQuery, const NDBResultFunctor &theResult, NTime waitFor)
{	NDBHandle		*dbHandle;
	NStatus			theErr;



	// Execute the query
	theErr = kNErrMemory;
	
	if (AcquireConnection(dbHandle))
		{
		theErr = dbHandle->Execute(theQuery, theResult, waitFor);
		ReleaseConnection(dbHandle);
		}
	
	return(theErr);
}





//============================================================================
//		NDBHandlePool::ExecuteSInt32 : Execute a query to obtain an SInt32.
//----------------------------------------------------------------------------
SInt32 NDBHandlePool::ExecuteSInt32(const NDBQuery &theQuery)
{	SInt32		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueSInt32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandlePool::ExecuteSInt64 : Execute a query to obtain an SInt64.
//----------------------------------------------------------------------------
SInt64 NDBHandlePool::ExecuteSInt64(const NDBQuery &theQuery)
{	SInt64		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueSInt64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandlePool::ExecuteFloat32 : Execute a query to obtain a Float32.
//----------------------------------------------------------------------------
Float32 NDBHandlePool::ExecuteFloat32(const NDBQuery &theQuery)
{	Float32		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0.0f;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueFloat32, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandlePool::ExecuteFloat64 : Execute a query to obtain a Float64.
//----------------------------------------------------------------------------
Float64 NDBHandlePool::ExecuteFloat64(const NDBQuery &theQuery)
{	Float64		theValue;
	NStatus		theErr;



	// Execute the query
	theValue = 0.0;
	theErr   = Execute(theQuery, BindFunction(NDBResult::GetRowValueFloat64, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandlePool::ExecuteString : Execute a query to obtain a string.
//----------------------------------------------------------------------------
NString NDBHandlePool::ExecuteString(const NDBQuery &theQuery)
{	NString		theValue;
	NStatus		theErr;



	// Execute the query
	theErr = Execute(theQuery, BindFunction(NDBResult::GetRowValueString, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandlePool::ExecuteData : Execute a query to obtain data.
//----------------------------------------------------------------------------
NData NDBHandlePool::ExecuteData(const NDBQuery &theQuery)
{	NData		theValue;
	NStatus		theErr;



	// Execute the query
	theErr = Execute(theQuery, BindFunction(NDBResult::GetRowValueData, _1, 0, &theValue));
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDBHandlePool::CreateConnection : Create a connection.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NDBHandlePool::CreateConnection(NDBHandlePtr &dbHandle)
{	NStatus		theErr;



	// Create the connection
	dbHandle = new NDBHandle;
	theErr   = (dbHandle == NULL) ? kNErrMemory : kNoErr;

	if (theErr == kNoErr)
		{
		theErr = dbHandle->Open(mFile, !mIsMutable, mVFS);
		NN_ASSERT_NOERR(theErr);
		}



	// Apply the connector
	if (theErr == noErr && mConnector != NULL)
		mConnector(*dbHandle);



	// Handle failure
	if (theErr != kNoErr)
		{
		delete dbHandle;
		dbHandle = NULL;
		}
	
	return(theErr);
}

