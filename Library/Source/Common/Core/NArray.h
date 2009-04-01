/*	NAME:
		NArray.h

	DESCRIPTION:
		Array object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NARRAY_HDR
#define NARRAY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSharedValue.h"
#include "NContainer.h"
#include "NObject.h"
#include "NVariant.h"
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NString.h"
#include "NColor.h"
#include "NDate.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NDictionary;

typedef std::vector<NVariant>										NArrayValue;
typedef NArrayValue::iterator										NArrayValueIterator;
typedef NArrayValue::const_iterator									NArrayValueConstIterator;

typedef NSharedValue<NArrayValue>									NSharedValueArray;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NArray :	public NObject,
				public NContainer,
				public NSharedValueArray {
public:
										NArray(const SInt32List  &theValues);
										NArray(const SInt64List  &theValues);
										NArray(const Float32List &theValues);
										NArray(const Float64List &theValues);
										NArray(const NStringList &theValues);

										 NArray(void);
	virtual								~NArray(void);


	// Get the size
	NIndex								GetSize(void) const;


	// Join two arrays
	void								Join(const NArray &theValue);



	// Sort the array
// dair
//	void								Sort(CFComparatorFunction sortFunc, void *sortData=NULL, const CFRange &theRange=kCFRangeAll);


	// Does a value exist?
	bool								HasValue(const NVariant &theValue) const;
	

	// Get/set a value
	NVariant							GetValue(NIndex theIndex) const;
	void								SetValue(NIndex theIndex, const NVariant &theValue);


	// Append a value
	void								AppendValue(const NVariant &theValue);


	// Get a type'd value
	//
	// If the value can not be returned as the specified type, 0/empty is returned.
	bool								GetValueBoolean(   NIndex theIndex) const;
	SInt32								GetValueSInt32(    NIndex theIndex) const;
	SInt64								GetValueSInt64(    NIndex theIndex) const;
	Float32								GetValueFloat32(   NIndex theIndex) const;
	Float64								GetValueFloat64(   NIndex theIndex) const;
	NString								GetValueString(    NIndex theIndex) const;
	NPoint								GetValuePoint(     NIndex theIndex) const;
	NSize								GetValueSize(      NIndex theIndex) const;
	NRectangle							GetValueRectangle( NIndex theIndex) const;
	NColor								GetValueColor(     NIndex theIndex) const;
	NDate								GetValueDate(      NIndex theIndex) const;
	NArray								GetValueArray(     NIndex theIndex) const;
	NDictionary							GetValueDictionary(NIndex theIndex) const;
	NData								GetValueData(      NIndex theIndex) const;


	// Get/set an array of values
	SInt32List							GetValuesSInt32( void) const;
	SInt64List							GetValuesSInt64( void) const;
	Float32List							GetValuesFloat32(void) const;
	Float64List							GetValuesFloat64(void) const;
	NStringList							GetValuesString( void) const;

	void								SetValuesSInt32( const SInt32List  &theValues);
	void								SetValuesSInt64( const SInt64List  &theValues);
	void								SetValuesFloat32(const Float32List &theValues);
	void								SetValuesFloat64(const Float64List &theValues);
	void								SetValuesString( const NStringList &theValues);


protected:
	// Get the null value
	const NArrayValue					*GetNullValue(void) const;
};





#endif // NARRAY_HDR


