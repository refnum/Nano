/*	NAME:
		NDBFile.h

	DESCRIPTION:
		Database file.
	
	COPYRIGHT:
		Copyright (c) 2007-2008, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBFILE_HDR
#define NDBFILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NFunctor.h"
#include "NDBResult.h"
#include "NDBStatement.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Functors
typedef nfunctor<bool (void)>								NDBProgressFunctor;
typedef nfunctor<void (const NDBResult &theRow)>			NDBResultFunctor;


// Errors
typedef int SQLError;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBFile {
public:
										NDBFile(void);
	virtual								~NDBFile(void);


	// Is the database open?
	bool								IsOpen(void) const;


	// Open/close the database
	OSStatus							Open(const NFile &theFile, bool readOnly=false, const NString &theVFS="");
	void								Close(void);


	// Get/set the progress functor
	NDBProgressFunctor					GetProgressFunctor(void) const;
	void								SetProgressFunctor(const NDBProgressFunctor &theFunctor);


	// Execute a statement
	//
	// Each database connection may be used by one thread at a time. If multiple threads
	// require access to the database, an external lock must be used to serialise access.
	// Multiple connections to the database, across several threads, can be obtained with
	// an NDBFilePool.
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


	// Cancel the current operation
	void								Cancel(void);


public:
	// Get the underlying database
	//
	// This method should be used as a last resort, to extend the database in ways that can not
	// be directly exposed through NDBFile. The reference returned is implementation-specific.
	void								*GetDatabase(void);


private:
	OSStatus							GetSQLError(SQLError sqlErr);
	sqlite3_stmt						*GetSQLStatement(const NDBStatement &theStatement);

	void								BindParameters(sqlite3_stmt *sqlStatement, const NDictionary &theParameters);
	
	static SQLError						DBProgress(void *userData);


private:
	sqlite3								*mDatabase;

	NDBProgressFunctor					mProgress;
};




#endif // NDBFILE_HDR


