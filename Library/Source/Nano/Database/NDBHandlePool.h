/*	NAME:
		NDBHandlePool.h

	DESCRIPTION:
		Pooled access to a database handle.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBHANDLEPOOL_HDR
#define NDBHANDLEPOOL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAtomicList.h"
#include "NDBHandle.h"
#include "NFile.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Misc
typedef NDBHandle												*NDBHandlePtr;
typedef nfunctor<void (NDBHandle &dbFile)>						NDBHandleConnector;


// Lists
typedef NAtomicList<NDBHandlePtr>								NDBHandlePoolList;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBHandlePool {
public:
										NDBHandlePool( void);
	virtual								~NDBHandlePool(void);


	// Is the handle open?
	bool								IsOpen(void) const;


	// Get/set the connector
	//
	// The connector functor is invoked once for each new database
	// connection, to allow the database to be customised before use.
	NDBHandleConnector					GetConnector(void);
	void								SetConnector(const NDBHandleConnector &theConnector);
	

	// Open/close the database
	NStatus								Open(const NFile &theFile, bool connectOnce=false, bool readOnly=false, const NString &theVFS="");
	void								Close(void);


	// Acquire/release a connection
	//
	// All connections must be released before the database is closed.
	bool								AcquireConnection(NDBHandlePtr &dbHandle);
	void								ReleaseConnection(NDBHandlePtr &dbHandle);


	// Execute a statement
	//
	// If the database is busy, the current thread will be blocked until the timeout occurs.
	NStatus								Execute(const NDBStatement		&theStatement,
												const NDBResultFunctor	&theResult = NULL,
												NTime					waitFor    = kNTimeForever);


	// Execute a return-one-value statement
	//
	// Returns the first value of the statement result, or 0/empty on failure.
	SInt32								ExecuteSInt32( const NDBStatement &theStatement);
	SInt64								ExecuteSInt64( const NDBStatement &theStatement);
	Float32								ExecuteFloat32(const NDBStatement &theStatement);
	Float64								ExecuteFloat64(const NDBStatement &theStatement);
	NString								ExecuteString( const NDBStatement &theStatement);
	NData								ExecuteData(   const NDBStatement &theStatement);


private:
	OSStatus							CreateConnection(NDBHandlePtr &dbFile);


private:
	bool								mIsOpen;
	NDBHandlePoolList					mPool;
	NDBHandleConnector					mConnector;

	bool								mConnectOnce;
	bool								mReadOnly;
	NFile								mFile;
	NString								mVFS;
};




#endif // NDBHANDLEPOOL_HDR


