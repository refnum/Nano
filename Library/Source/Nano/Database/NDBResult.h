/*	NAME:
		NDBResult.h

	DESCRIPTION:
		Database result.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
										NDBResult(const NDBResult &theValue);

										NDBResult(NDBQueryRef theResult);
	virtual							   ~NDBResult(void);


	// Get the number of columns
	NIndex								GetSize(void) const;


	// Get the name of a column
	NString								GetName(NIndex theIndex) const;


	// Get a value
	int32_t								GetValueInt32(  NIndex theIndex) const;
	int64_t								GetValueInt64(  NIndex theIndex) const;
	float32_t							GetValueFloat32(NIndex theIndex) const;
	float64_t							GetValueFloat64(NIndex theIndex) const;
	NString								GetValueString( NIndex theIndex) const;
	NData								GetValueData(   NIndex theIndex) const;


	// Functors
	static void							GetRowValueInt32(  const NDBResult &theRow, NIndex theIndex, int32_t   *theValue);
	static void							GetRowValueInt64(  const NDBResult &theRow, NIndex theIndex, int64_t   *theValue);
	static void							GetRowValueFloat32(const NDBResult &theRow, NIndex theIndex, float32_t *theValue);
	static void							GetRowValueFloat64(const NDBResult &theRow, NIndex theIndex, float64_t *theValue);
	static void							GetRowValueString( const NDBResult &theRow, NIndex theIndex, NString   *theValue);
	static void							GetRowValueData(   const NDBResult &theRow, NIndex theIndex, NData     *theValue);

	static void							GetRowValuesInt32(  const NDBResult &theRow, NIndex theIndex, Int32List   *theValues);
	static void							GetRowValuesInt64(  const NDBResult &theRow, NIndex theIndex, Int64List   *theValues);
	static void							GetRowValuesFloat32(const NDBResult &theRow, NIndex theIndex, Float32List *theValues);
	static void							GetRowValuesFloat64(const NDBResult &theRow, NIndex theIndex, Float64List *theValues);
	static void							GetRowValuesString( const NDBResult &theRow, NIndex theIndex, NStringList *theValues);
	static void							GetRowValuesData(   const NDBResult &theRow, NIndex theIndex, NDataList   *theValues);


	// Operators
	const NDBResult&					operator = (const NDBResult &theObject);


private:
	NDBQueryRef							mResult;
};




#endif // NDBRESULT_HDR



