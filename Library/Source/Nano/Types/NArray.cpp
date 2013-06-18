/*	NAME:
		NArray.cpp

	DESCRIPTION:
		Array object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NSTLUtilities.h"
#include "NDictionary.h"
#include "NEncoder.h"
#include "NNumber.h"
#include "NArray.h"





//============================================================================
//		Internal classes
//----------------------------------------------------------------------------
class NSortArray {
public:
	NSortArray(const NArrayCompareFunctor &theFunctor)
	{
		mFunctor = theFunctor;
	}

	bool operator()(const NVariant &a, const NVariant &b)
	{
		return(mFunctor(a, b) == kNCompareLessThan);
	}

private:
	NArrayCompareFunctor	mFunctor;
};





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NArray);





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
	return((NIndex) GetImmutable()->size());
}





//============================================================================
//		NArray::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NArray::Compare(const NArray &theValue) const
{	const NArrayValue		*ourValue, *otherValue;
	NIndex					ourSize, otherSize;
	NComparison				theResult;



	// Get the state we need
	ourValue   =          GetImmutable();
	otherValue = theValue.GetImmutable();

	ourSize   = (NIndex) ourValue->size();
	otherSize = (NIndex) otherValue->size();



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(ourSize, otherSize);

	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(ourValue, otherValue);

	return(theResult);
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
void NArray::Sort(const NArrayCompareFunctor &theFunctor, const NRange &theRange)
{	NArrayValueIterator			iterFirst, iterLast;
	NArrayCompareFunctor		compareWith;
	NRange						finalRange;
	NArrayValue					*theArray;



	// Get the state we need
	theArray    = GetMutable();
	compareWith = GetCompareFunctor(theFunctor);

	finalRange = GetNormalized(theRange);
	iterFirst  = theArray->begin() + finalRange.GetFirst();
	iterLast   = theArray->begin() + finalRange.GetNext();


	// Sort the array
	std::sort(iterFirst, iterLast, NSortArray(compareWith));
}





//============================================================================
//		NArray::ForEach : Process each item.
//----------------------------------------------------------------------------
void NArray::ForEach(const NArrayForEachFunctor &theFunctor, const NRange &theRange) const
{	NIndex						n, minIndex, maxIndex;
	NRange						finalRange;
	const NArrayValue			*theArray;



	// Get the state we need
	theArray   = GetImmutable();
	finalRange = GetNormalized(theRange);



	// Process the array
	if (!finalRange.IsEmpty())
		{
		minIndex = finalRange.GetFirst();
		maxIndex = finalRange.GetNext();
		
		for (n = minIndex; n < maxIndex; n++)
			theFunctor(n, theArray->at(n));
		}
}





//============================================================================
//		NArray::HasValue : Does a value exist?
//----------------------------------------------------------------------------
bool NArray::HasValue(const NVariant &theValue, const NArrayCompareFunctor &theFunctor) const
{	NIndex						n, numValues;
	NArrayCompareFunctor		compareWith;
	const NArrayValue			*theArray;



	// Get the state we need
	theArray    = GetImmutable();
	compareWith = GetCompareFunctor(theFunctor);
	numValues   = GetSize();
	


	// Find the value
	for (n = 0; n < numValues; n++)
		{
		if (compareWith(theArray->at(n), theValue) == kNCompareEqualTo)
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
{


	// Get the value
	return(NSystemUtilities::GetBoolean(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NArray::GetValueSInt32(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetSInt32(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NArray::GetValueSInt64(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetSInt64(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NArray::GetValueFloat32(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetFloat32(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NArray::GetValueFloat64(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetFloat64(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NArray::GetValueString(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetString(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NArray::GetValueData(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetData(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NDate NArray::GetValueDate(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetDate(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NArray::GetValueColor(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetColor(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
NPoint NArray::GetValuePoint(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetPoint(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
NSize NArray::GetValueSize(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetSize(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueRectangle : Get a rectangle value.
//----------------------------------------------------------------------------
NRectangle NArray::GetValueRectangle(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetRectangle(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueVector : Get a vector value.
//----------------------------------------------------------------------------
NVector NArray::GetValueVector(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetVector(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NArray::GetValueArray(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetArray(GetValue(theIndex), GetDebugID(theIndex)));
}





//============================================================================
//		NArray::GetValueDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NArray::GetValueDictionary(NIndex theIndex) const
{


	// Get the value
	return(NSystemUtilities::GetDictionary(GetValue(theIndex), GetDebugID(theIndex)));
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
//		NArray::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NArray::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{size=%ld}", GetSize());

	return(theResult);
}





#pragma mark protected
//============================================================================
//		NArray::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
const NArrayValue *NArray::GetNullValue(void) const
{	static NArrayValue		sNullValue;



	// Get the value
	return(&sNullValue);
}





//============================================================================
//      NArray::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NArray::EncodeSelf(NEncoder &theEncoder) const
{	bool					valueBoolean;
	NString					valueString;
	NIndex					n, numItems;
	const NArrayValue		*theArray;
	NData					valueData;
	NVariant				theValue;
	NString					theKey;



	// Get the state we need
	theArray = GetImmutable();
	numItems = (NIndex) theArray->size();



	// Encode the object
	for (n = 0; n < numItems; n++)
		{
		theKey.Format("%ld", n);
		theValue = theArray->at(n);
		
		if (theValue.GetValue(valueBoolean))
			theEncoder.EncodeBoolean(theKey, valueBoolean);

		else if (theValue.IsNumeric())
			theEncoder.EncodeNumber(theKey, NNumber(theValue));

		else if (theValue.GetValue(valueString))
			theEncoder.EncodeString(theKey, valueString);

		else if (theValue.GetValue(valueData))
			theEncoder.EncodeData(theKey, valueData);

		else
			{
			if (NEncoder::CanEncode(theValue))
				theEncoder.EncodeObject(theKey, theValue);
			else
				NN_LOG("Unable to encode object %d (%s)", n, theValue.GetType().name());
			}
		}
}





//============================================================================
//      NArray::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NArray::DecodeSelf(const NEncoder &theEncoder)
{	NIndex							n, numItems;
	NNumber							theNumber;
	NStringList						theKeys;
	NEncodedType					theType;
	NStringListConstIterator		theIter;
	NString							theKey;



	// Get the state we need
	theKeys = theEncoder.GetKeys();
	numItems = (NIndex) theKeys.size();
	
	for (n = 0; n < numItems; n++)
		AppendValue(0);



	// Decode the object
	for (theIter = theKeys.begin(); theIter != theKeys.end(); theIter++)
		{
		// Get the state we need
		theKey  = *theIter;
		theType = theEncoder.GetValueType(theKey);
		
		if (!theNumber.SetValue(theKey))
			{
			NN_LOG("Unknown index format (%@) - skipping", theKey);
			return;
			}



		// Decode the value
		n = theNumber.GetSInt32();

		switch (theType) {
			case kNEncodedBoolean:
				SetValue(n, theEncoder.DecodeBoolean(theKey));
				break;

			case kNEncodedNumber:
				SetValue(n, theEncoder.DecodeNumber(theKey));
				break;

			case kNEncodedString:
				SetValue(n, theEncoder.DecodeString(theKey));
				break;

			case kNEncodedData:
				SetValue(n, theEncoder.DecodeData(theKey));
				break;

			case kNEncodedObject:
				SetValue(n, theEncoder.DecodeObject(theKey));
				break;
			
			case kNEncodedUnknown:
			default:
				NN_LOG("Unknown encoder type (%d)(%d) - skipping", theType, n);
				break;
			}
		}
}





#pragma mark private
//============================================================================
//		NArray::GetCompareFunctor : Get a comparison functor.
//----------------------------------------------------------------------------
NArrayCompareFunctor NArray::GetCompareFunctor(const NArrayCompareFunctor &theFunctor) const
{	NArrayCompareFunctor	compareWith;



	// Get the functor
	if (theFunctor != NULL)
		compareWith = theFunctor;
	else
		compareWith = BindFunction(NVariant::CompareValues, _1, _2);
	
	return(compareWith);
}





//============================================================================
//		NArray::GetDebugID : Get the debug ID for a value.
//----------------------------------------------------------------------------
NString NArray::GetDebugID(NIndex theIndex) const
{	NString		theID;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());
	
	NN_UNUSED(theIndex);



	// Get the ID
#if NN_DEBUG
	theID.Format("%ld", theIndex);
#endif

	return(theID);
}


