/*	NAME:
		NDBFilePoolPool.h

	DESCRIPTION:
		Pooled access to a database file.
	
	COPYRIGHT:
		Copyright (c) 2007-2008, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBFILEPOOL_HDR
#define NDBFILEPOOL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAtomicList.h"
#include "NDBFile.h"
#include "NFile.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Misc
typedef NDBFile												*NDBFilePtr;
typedef nfunctor<void (NDBFile &dbFile)>					NDBFileConnector;


// Lists
typedef NAtomicList<NDBFilePtr>								NDBFilePoolList;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBFilePool {
public:
										NDBFilePool( void);
	virtual								~NDBFilePool(void);


	// Is the database open?
	bool								IsOpen(void) const;


	// Get/set the connector
	//
	// The connector functor is invoked once for each new database
	// connection, to allow the database to be customised before use.
	NDBFileConnector					GetConnector(void);
	void								SetConnector(const NDBFileConnector &theConnector);
	

	// Open/close the database
	OSStatus							Open(const NFile &theFile, bool connectOnce=false, bool readOnly=false, const NString &theVFS="");
	void								Close(void);


	// Acquire/release a connection
	//
	// All connections must be released before the database is closed.
	bool								AcquireConnection(NDBFilePtr &dbFile);
	void								ReleaseConnection(NDBFilePtr &dbFile);


	// Execute a statement
	//
	// If the database is busy, the current thread will be blocked until the timeout occurs.
	OSStatus							Execute(const NDBStatement     &theStatement,
												const NDBResultFunctor &theResult = NULL,
												EventTime				waitFor   = kEventDurationForever);


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
	OSStatus							CreateConnection(NDBFilePtr &dbFile);


private:
	bool								mIsOpen;
	NDBFilePoolList						mPool;
	NDBFileConnector					mConnector;

	NFile								mFile;
	bool								mConnectOnce;
	bool								mReadOnly;
	NString								mVFS;
};




#endif // NDBFILEPOOL_HDR


