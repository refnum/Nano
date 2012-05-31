/*	NAME:
		NDBHandle.h

	DESCRIPTION:
		Database handle.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBHANDLE_HDR
#define NDBHANDLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NProgressable.h"
#include "NUncopyable.h"
#include "NFunctor.h"
#include "NFile.h"
#include "NDBQuery.h"
#include "NDBResult.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef NBitfield NDBFlags;

static const NDBFlags kNDBNone										= 0;
static const NDBFlags kNDBReadOnly									= (1 << 0);
static const NDBFlags kNDBPoolConnectOnce							= (1 << 1);





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef nfunctor<void (const NDBResult &theRow)>			NDBResultFunctor;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBHandle :	public NProgressable,
					public NUncopyable {
public:
										NDBHandle(void);
	virtual							   ~NDBHandle(void);


	// Is the handle open?
	bool								IsOpen(void) const;


	// Is the database mutable?
	bool								IsMutable(void) const;


	// Get the database file
	NFile								GetFile(void) const;


	// Open/close the database
	NStatus								Open(const NFile &theFile, NDBFlags theFlags=kNDBNone, const NString &theVFS="");
	void								Close(void);


	// Execute a query
	//
	// Each database connection may be used by one thread at a time. If multiple threads
	// require access to the database, an external lock must be used to serialise access.
	// Multiple connections to the database, across several threads, can be obtained with
	// an NDBHandlePool.
	//
	// If the database is busy, the current thread will be blocked until the timeout occurs.
	NStatus								Execute(const NDBQuery			&theQuery,
												const NDBResultFunctor	&theResult = NULL,
												NTime					waitFor    = kNTimeForever);


	// Execute a return-one-value query
	//
	// Returns the first value of the query result, or 0/empty on failure.
	SInt32								ExecuteSInt32( const NDBQuery &theQuery);
	SInt64								ExecuteSInt64( const NDBQuery &theQuery);
	Float32								ExecuteFloat32(const NDBQuery &theQuery);
	Float64								ExecuteFloat64(const NDBQuery &theQuery);
	NString								ExecuteString( const NDBQuery &theQuery);
	NData								ExecuteData(   const NDBQuery &theQuery);


	// Does a table exist?
	bool								HasTable(const NString &theTable);
	

	// Cancel the current operation
	void								Cancel(void);


public:
	// Get the underlying database
	//
	// This method should be used as a last resort, to extend the database in ways that can not
	// be directly exposed through NDBHandle. The reference returned is implementation-specific.
	NDBHandleRef						GetDatabase(void);


private:
	NDBStatus							SQLiteExecute(    const NDBQuery   &theQuery, const NDBResultFunctor &theResult, NTime waitFor);
	NDBQueryRef							SQLiteCreateQuery(const NDBQuery   &theQuery);
	void								SQLiteDestroyQuery(     NDBQueryRef theQuery);

	void								SQLiteBindParameters(      NDBQueryRef theQuery, const NVariant &theParameters);
	void								SQLiteBindParameterByIndex(NDBQueryRef theQuery,       NIndex   theIndex, const NVariant &theValue);
	void								SQLiteBindParameterByKey(  NDBQueryRef theQuery, const NString &theKey,   const NVariant &theValue);

	NStatus								SQLiteGetStatus(NDBStatus dbErr);

	void								SQLiteProgressBegin(void);
	void								SQLiteProgressEnd(  void);
	static int							SQLiteProgressContinue(void *userData);


private:
	NFile								mFile;
	NDBFlags							mFlags;
	NDBHandleRef						mDatabase;
	
	NDBQueryRef							mCacheQuery;
	NString								mCacheSQL;
};




#endif // NDBHANDLE_HDR


