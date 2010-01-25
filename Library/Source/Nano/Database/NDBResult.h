/*	NAME:
		NDBResult.h

	DESCRIPTION:
		Database result.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDBRESULT_HDR
#define NDBRESULT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDBQuery.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDBResult {
public:
										NDBResult(NDBQueryRef theResult);
	virtual							   ~NDBResult(void);


	// Get the number of columns
	NIndex								GetSize(void) const;


	// Get the name of a column
	NString								GetName(NIndex theIndex) const;


	// Get a value
	SInt32								GetValueSInt32( NIndex theIndex) const;
	SInt64								GetValueSInt64( NIndex theIndex) const;
	Float32								GetValueFloat32(NIndex theIndex) const;
	Float64								GetValueFloat64(NIndex theIndex) const;
	NString								GetValueString( NIndex theIndex) const;
	NData								GetValueData(   NIndex theIndex) const;


	// Functors
	static void							GetRowValueSInt32( const NDBResult &theRow, NIndex theIndex, SInt32  *theValue);
	static void							GetRowValueSInt64( const NDBResult &theRow, NIndex theIndex, SInt64  *theValue);
	static void							GetRowValueFloat32(const NDBResult &theRow, NIndex theIndex, Float32 *theValue);
	static void							GetRowValueFloat64(const NDBResult &theRow, NIndex theIndex, Float64 *theValue);
	static void							GetRowValueString( const NDBResult &theRow, NIndex theIndex, NString *theValue);
	static void							GetRowValueData(   const NDBResult &theRow, NIndex theIndex, NData   *theValue);

	static void							GetRowValuesSInt32( const NDBResult &theRow, NIndex theIndex, SInt32List  *theValues);
	static void							GetRowValuesSInt64( const NDBResult &theRow, NIndex theIndex, SInt64List  *theValues);
	static void							GetRowValuesFloat32(const NDBResult &theRow, NIndex theIndex, Float32List *theValues);
	static void							GetRowValuesFloat64(const NDBResult &theRow, NIndex theIndex, Float64List *theValues);
	static void							GetRowValuesString( const NDBResult &theRow, NIndex theIndex, NStringList *theValues);
	static void							GetRowValuesData(   const NDBResult &theRow, NIndex theIndex, NDataList   *theValues);
	

private:
	NDBQueryRef							mResult;
};




#endif // NDBRESULT_HDR



