/*	NAME:
		NArray.cpp

	DESCRIPTION:
		Array object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NDictionary.h"
#include "NNumber.h"
#include "NArray.h"





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(const SInt32List &theValues)
{


	// Initialize ourselves
	SetValuesSInt32(theValues);
}





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(const SInt64List &theValues)
{


	// Initialize ourselves
	SetValuesSInt64(theValues);
}





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(const Float32List &theValues)
{


	// Initialize ourselves
	SetValuesFloat32(theValues);
}





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(const Float64List &theValues)
{


	// Initialize ourselves
	SetValuesFloat64(theValues);
}





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(const NStringList &theValues)
{


	// Initialize ourselves
	SetValuesString(theValues);
}





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(void)
{
}





//============================================================================
//		NArray::~NArray : Destructor.
//----------------------------------------------------------------------------
NArray::~NArray(void)
{
}





//============================================================================
//		NArray::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NArray::GetSize(void) const
{


	// Get the size
	return(GetImmutable()->size());
}





//============================================================================
//		NArray::Join : Join two arrays.
//----------------------------------------------------------------------------
void NArray::Join(const NArray &theValue)
{	NArrayValue		*theArray;



	// Get the state we need
	theArray = GetMutable();



	// Join the arrays
	append(*theArray, *(theValue.GetImmutable()));
}





//============================================================================
//		NArray::Sort : Sort the array.
//----------------------------------------------------------------------------
// dair
/*
void NArray::Sort(CFComparatorFunction sortFunc, void *sortData, const CFRange &theRange)
{	CFRange		sortRange;



	// Prepare the range
	sortRange = theRange;
	if (sortRange.length == -1)
		sortRange.length = GetSize();



	// Sort the array
	if (MakeMutable())
		CFArraySortValues(*this, sortRange, sortFunc, sortData);
}
*/





//============================================================================
//		NArray::HasValue : Does a value exist?
//----------------------------------------------------------------------------
bool NArray::HasValue(const NVariant &theValue) const
{	NIndex				n, numValues;
	const NArrayValue	*theArray;



	// Get the state we need
	theArray  = GetImmutable();
	numValues = GetSize();
	


	// Find the value
	for (n = 0; n < numValues; n++)
		{
		if (theArray->at(n) == theValue)
			return(true);
		}

	return(false);
}





//============================================================================
//		NArray::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NArray::GetValue(NIndex theIndex) const
{	const NArrayValue	*theArray;
	NVariant			theValue;



	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < GetSize());



	// Get the value
	theArray = GetImmutable();
	theValue = theArray->at(theIndex);

	NN_ASSERT(theValue.IsValid());
	return(theValue);
}





//============================================================================
//		NArray::SetValue : Set a value.
//----------------------------------------------------------------------------
void NArray::SetValue(NIndex theIndex, const NVariant &theValue)
{	NArrayValue		*theArray;



	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < GetSize());
	NN_ASSERT(theValue.IsValid());



	// Set the value
	theArray               = GetMutable();
	theArray->at(theIndex) = theValue;
}





//============================================================================
//		NArray::AppendValue : Append a value.
//----------------------------------------------------------------------------
void NArray::AppendValue(const NVariant &theValue)
{	NArrayValue		*theArray;



	// Validate our parameters
 	NN_ASSERT(theValue.IsValid());
	


	// Append the value
	theArray = GetMutable();
	theArray->push_back(theValue);
}





//============================================================================
//		NArray::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NArray::GetValueBoolean(NIndex theIndex) const
{	NNumber		theNumber;
	NVariant	rawValue;
	bool		theValue;



	// Get the value
	theValue = 0;
	rawValue = GetValue(theIndex);

	if (!theNumber.SetValue(rawValue) || !theNumber.GetValueBoolean(theValue))
		NN_LOG("Unable to convert [%d] to bool", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NArray::GetValueSInt32(NIndex theIndex) const
{	NNumber		theNumber;
	NVariant	rawValue;
	SInt32		theValue;



	// Get the value
	theValue = 0;
	rawValue = GetValue(theIndex);

	if (!theNumber.SetValue(rawValue) || !theNumber.GetValueSInt32(theValue))
		NN_LOG("Unable to convert [%d] to SInt32", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NArray::GetValueSInt64(NIndex theIndex) const
{	NNumber		theNumber;
	NVariant	rawValue;
	SInt64		theValue;



	// Get the value
	theValue = 0;
	rawValue = GetValue(theIndex);

	if (!theNumber.SetValue(rawValue) || !theNumber.GetValueSInt64(theValue))
		NN_LOG("Unable to convert [%d] to SInt64", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NArray::GetValueFloat32(NIndex theIndex) const
{	NNumber		theNumber;
	NVariant	rawValue;
	Float32		theValue;



	// Get the value
	theValue = 0.0f;
	rawValue = GetValue(theIndex);

	if (!theNumber.SetValue(rawValue) || !theNumber.GetValueFloat32(theValue))
		NN_LOG("Unable to convert [%d] to Float32", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NArray::GetValueFloat64(NIndex theIndex) const
{	NNumber		theNumber;
	NVariant	rawValue;
	Float64		theValue;



	// Get the value
	theValue = 0.0;
	rawValue = GetValue(theIndex);

	if (!theNumber.SetValue(rawValue) || !theNumber.GetValueFloat64(theValue))
		NN_LOG("Unable to convert [%d] to Float64", theIndex);

	return(theValue);

}





//============================================================================
//		NArray::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NArray::GetValueString(NIndex theIndex) const
{	NString		theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NString", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
NPoint NArray::GetValuePoint(NIndex theIndex) const
{	NPoint		theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NPoint", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
NSize NArray::GetValueSize(NIndex theIndex) const
{	NSize		theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NSize", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueRectangle : Get a rectangle value.
//----------------------------------------------------------------------------
NRectangle NArray::GetValueRectangle(NIndex theIndex) const
{	NRectangle	theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NRectangle", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NArray::GetValueColor(NIndex theIndex) const
{	NColor		theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NColor", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NDate NArray::GetValueDate(NIndex theIndex) const
{	NDate		theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NDate", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NArray::GetValueArray(NIndex theIndex) const
{	NArray		theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NArray", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NArray::GetValueDictionary(NIndex theIndex) const
{	NDictionary		theValue;
	NVariant		rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NDictionary", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NArray::GetValueData(NIndex theIndex) const
{	NData		theValue;
	NVariant	rawValue;



	// Get the value
	rawValue = GetValue(theIndex);

	if (!rawValue.GetValue(theValue))
		NN_LOG("Unable to convert [%d] to NData", theIndex);

	return(theValue);
}





//============================================================================
//		NArray::GetValuesSInt32 : Get a SInt32List.
//----------------------------------------------------------------------------
SInt32List NArray::GetValuesSInt32(void) const
{	NIndex			n, numItems;
	SInt32List		theResult;



	// Get the values
	numItems = GetSize();
	
	for (n = 0; n < numItems; n++)
		theResult.push_back(GetValueSInt32(n));
	
	return(theResult);
}





//============================================================================
//		NArray::GetValuesSInt64 : Get a SInt64List.
//----------------------------------------------------------------------------
SInt64List NArray::GetValuesSInt64(void) const
{	NIndex			n, numItems;
	SInt64List		theResult;



	// Get the values
	numItems = GetSize();
	
	for (n = 0; n < numItems; n++)
		theResult.push_back(GetValueSInt64(n));
	
	return(theResult);
}





//============================================================================
//		NArray::GetValuesFloat32 : Get a Float32List.
//----------------------------------------------------------------------------
Float32List NArray::GetValuesFloat32(void) const
{	NIndex			n, numItems;
	Float32List		theResult;



	// Get the values
	numItems = GetSize();
	
	for (n = 0; n < numItems; n++)
		theResult.push_back(GetValueFloat32(n));
	
	return(theResult);
}





//============================================================================
//		NArray::GetValuesFloat64 : Get a Float64List.
//----------------------------------------------------------------------------
Float64List NArray::GetValuesFloat64(void) const
{	NIndex			n, numItems;
	Float64List		theResult;



	// Get the values
	numItems = GetSize();
	
	for (n = 0; n < numItems; n++)
		theResult.push_back(GetValueFloat64(n));
	
	return(theResult);
}





//============================================================================
//		NArray::GetValuesString : Get a StringList.
//----------------------------------------------------------------------------
NStringList NArray::GetValuesString(void) const
{	NIndex			n, numItems;
	NStringList		theResult;



	// Get the values
	numItems = GetSize();
	
	for (n = 0; n < numItems; n++)
		theResult.push_back(GetValueString(n));
	
	return(theResult);
}





//============================================================================
//		NArray::SetValuesSInt32 : Set a SInt32List.
//----------------------------------------------------------------------------
void NArray::SetValuesSInt32(const SInt32List &theValues)
{	SInt32ListConstIterator		theIter;



	// Set the values
	Clear();
	
	for (theIter = theValues.begin(); theIter != theValues.end(); theIter++)
		AppendValue(*theIter);
}





//============================================================================
//		NArray::SetValuesSInt64 : Set a SInt64List.
//----------------------------------------------------------------------------
void NArray::SetValuesSInt64(const SInt64List &theValues)
{	SInt64ListConstIterator		theIter;



	// Set the values
	Clear();
	
	for (theIter = theValues.begin(); theIter != theValues.end(); theIter++)
		AppendValue(*theIter);
}





//============================================================================
//		NArray::SetValuesFloat32 : Set a Float32List.
//----------------------------------------------------------------------------
void NArray::SetValuesFloat32(const Float32List &theValues)
{	Float32ListConstIterator	theIter;



	// Set the values
	Clear();
	
	for (theIter = theValues.begin(); theIter != theValues.end(); theIter++)
		AppendValue(*theIter);
}





//============================================================================
//		NArray::SetValuesFloat64 : Set a Float64List.
//----------------------------------------------------------------------------
void NArray::SetValuesFloat64(const Float64List &theValues)
{	Float64ListConstIterator	theIter;



	// Set the values
	Clear();
	
	for (theIter = theValues.begin(); theIter != theValues.end(); theIter++)
		AppendValue(*theIter);
}





//============================================================================
//		NArray::SetValuesString : Set a NStringList.
//----------------------------------------------------------------------------
void NArray::SetValuesString(const NStringList &theValues)
{	NStringListConstIterator	theIter;



	// Set the values
	Clear();
	
	for (theIter = theValues.begin(); theIter != theValues.end(); theIter++)
		AppendValue(*theIter);
}





//============================================================================
//		NArray::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
const NArrayValue *NArray::GetNullValue(void) const
{	static NArrayValue		sNullValue;



	// Get the value
	return(&sNullValue);
}





