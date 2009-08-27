/*	NAME:
		NDBHandle.h

	DESCRIPTION:
		Database handle.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBHANDLE_HDR
#define NDBHANDLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"
#include "NFile.h"
#include "NDBQuery.h"
#include "NDBResult.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef nfunctor<bool (void)>								NDBProgressFunctor;
typedef nfunctor<void (const NDBResult &theRow)>			NDBResultFunctor;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBHandle {
public:
										NDBHandle(void);
	virtual							   ~NDBHandle(void);


	// Is the handle open?
	bool								IsOpen(void) const;


	// Open/close the database
	OSStatus							Open(const NFile &theFile, bool readOnly=false, const NString &theVFS="");
	void								Close(void);


	// Get/set the progress functor
	NDBProgressFunctor					GetProgressFunctor(void) const;
	void								SetProgressFunctor(const NDBProgressFunctor &theFunctor);


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


	// Cancel the current operation
	void								Cancel(void);


public:
	// Get the underlying database
	//
	// This method should be used as a last resort, to extend the database in ways that can not
	// be directly exposed through NDBHandle. The reference returned is implementation-specific.
	NDBHandleRef						GetDatabase(void);


private:
	NDBQueryRef							SQLiteCreateQuery(const NDBQuery &theQuery);
	void								SQLiteBindParameters(NDBQueryRef theQuery, const NDictionary &theParameters);
	NStatus								SQLiteGetStatus(NDBStatus dbErr);
	static int							SQLiteProgress(void *userData);


private:
	NDBHandleRef						mDatabase;
	NDBProgressFunctor					mProgress;
};




#endif // NDBHANDLE_HDR


