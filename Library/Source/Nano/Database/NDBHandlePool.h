/*	NAME:
		NDBHandlePool.h

	DESCRIPTION:
		Pooled access to a database handle.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
										NDBHandlePool(void);
	virtual							   ~NDBHandlePool(void);


	// Is the handle open?
	bool								IsOpen(void) const;


	// Is the database mutable?
	bool								IsMutable(void) const;


	// Get/set the connector
	//
	// The connector functor is invoked once for each new database
	// connection, to allow the database to be customised before use.
	NDBHandleConnector					GetConnector(void);
	void								SetConnector(const NDBHandleConnector &theConnector);
	

	// Open/close the database
	NStatus								Open(const NFile &theFile, NDBFlags theFlags=kNDBNone, const NString &theVFS="");
	void								Close(void);


	// Acquire/release a connection
	//
	// All connections must be released before the database is closed.
	bool								AcquireConnection(NDBHandlePtr &dbHandle);
	void								ReleaseConnection(NDBHandlePtr &dbHandle);


	// Execute a query
	//
	// If the database is busy, the current thread will be blocked until the timeout occurs.
	NStatus								Execute(const NDBQuery			&theQuery,
												const NDBResultFunctor	&theResult = NULL,
												NTime					waitFor    = kNTimeForever);


	// Execute a return-one-value query
	//
	// Returns the first value of the query result, or 0/empty on failure.
	int32_t								ExecuteInt32(  const NDBQuery &theQuery);
	int64_t								ExecuteInt64(  const NDBQuery &theQuery);
	float32_t							ExecuteFloat32(const NDBQuery &theQuery);
	float64_t							ExecuteFloat64(const NDBQuery &theQuery);
	NString								ExecuteString( const NDBQuery &theQuery);
	NData								ExecuteData(   const NDBQuery &theQuery);


private:
	NStatus								CreateConnection(NDBHandlePtr &dbFile);


private:
	bool								mIsOpen;
	NDBHandlePoolList					mPool;
	NDBHandleConnector					mConnector;

	NFile								mFile;
	NDBFlags							mFlags;
	NString								mVFS;
};




#endif // NDBHANDLEPOOL_HDR


