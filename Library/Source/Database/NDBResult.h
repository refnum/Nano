/*	NAME:
		NDBResult.h

	DESCRIPTION:
		Database result.
	
	COPYRIGHT:
		Copyright (c) 2007-2008, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBRESULT_HDR
#define NDBRESULT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "sqlite_nano.h"

#include "NSystemUtilities.h"
#include "NString.h"
#include "NData.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBResult {
public:
										NDBResult(sqlite3_stmt *sqResult);
	virtual								~NDBResult(void);


	// Get the number of columns
	UInt32								GetSize(void) const;


	// Get the name of a column
	NString								GetName(UInt32 theIndex) const;


	// Get a value
	SInt32								GetValueSInt32( UInt32 theIndex) const;
	SInt64								GetValueSInt64( UInt32 theIndex) const;
	Float32								GetValueFloat32(UInt32 theIndex) const;
	Float64								GetValueFloat64(UInt32 theIndex) const;
	NString								GetValueString( UInt32 theIndex) const;
	NData								GetValueData(   UInt32 theIndex) const;


	// Functors
	static void							GetRowValueSInt32( const NDBResult &theRow, UInt32 theIndex, SInt32  *theValue);
	static void							GetRowValueSInt64( const NDBResult &theRow, UInt32 theIndex, SInt64  *theValue);
	static void							GetRowValueFloat32(const NDBResult &theRow, UInt32 theIndex, Float32 *theValue);
	static void							GetRowValueFloat64(const NDBResult &theRow, UInt32 theIndex, Float64 *theValue);
	static void							GetRowValueString( const NDBResult &theRow, UInt32 theIndex, NString *theValue);
	static void							GetRowValueData(   const NDBResult &theRow, UInt32 theIndex, NData   *theValue);

	static void							GetRowValuesSInt32( const NDBResult &theRow, UInt32 theIndex, SInt32List  *theValues);
	static void							GetRowValuesSInt64( const NDBResult &theRow, UInt32 theIndex, SInt64List  *theValues);
	static void							GetRowValuesFloat32(const NDBResult &theRow, UInt32 theIndex, Float32List *theValues);
	static void							GetRowValuesFloat64(const NDBResult &theRow, UInt32 theIndex, Float64List *theValues);
	static void							GetRowValuesString( const NDBResult &theRow, UInt32 theIndex, NStringList *theValues);
	static void							GetRowValuesData(   const NDBResult &theRow, UInt32 theIndex, NDataList   *theValues);
	

private:
	sqlite3_stmt						*mResult;
};




#endif // NDBRESULT_HDR



