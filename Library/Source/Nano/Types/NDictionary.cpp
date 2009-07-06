/*	NAME:
		NDictionary.cpp

	DESCRIPTION:
		Dictionary object.
	
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
#include "NNumber.h"
#include "NDictionary.h"





//============================================================================
//		NDictionary::NDictionary : Constructor.
//----------------------------------------------------------------------------
NDictionary::NDictionary(void)
{
}





//============================================================================
//		NDictionary::~NDictionary : Destructor.
//----------------------------------------------------------------------------
NDictionary::~NDictionary(void)
{
}





//============================================================================
//		NDictionary::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NDictionary::GetSize(void) const
{


	// Get the size
	return((NIndex) GetImmutable()->size());
}





//============================================================================
//		NDictionary::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NDictionary::Compare(const NDictionary &theValue) const
{	const NDictionaryValue		*ourValue, *otherValue;
	NIndex						ourSize, otherSize;
	NComparison					theResult;



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
//		NDictionary::Join : Join two dictionaries.
//----------------------------------------------------------------------------
void NDictionary::Join(const NDictionary &theValue)
{	const NDictionaryValue				*theDict;
	NDictionaryValueConstIterator		theIter;



	// Get the state we need
	theDict = theValue.GetImmutable();



	// Join the dictionaries
	for (theIter = theDict->begin(); theIter != theDict->end(); theIter++)
		SetValue(theIter->first, theIter->second);
}





//============================================================================
//		NDictionary::Invert : Invert the dictinary.
//----------------------------------------------------------------------------
bool NDictionary::Invert(void)
{	NString							theKey, theValue;
	NDictionary						theResult;
	bool							canInvert;
	NStringList						theKeys;
	NStringListConstIterator		theIter;



	// Get the state we need
	theKeys   = GetKeys();
	canInvert = true;



	// Create the inverted dictionary
	for (theIter = theKeys.begin(); theIter != theKeys.end() && canInvert; theIter++)
		{
		theKey   = *theIter;
		theValue = GetValueString(theKey);

		canInvert = theValue.IsNotEmpty() && !theResult.HasKey(theValue);
		if (canInvert)
			theResult.SetValue(theValue, theKey);
		}
	
	
	
	// Update our state
	if (canInvert)
		*this = theResult;
	
	return(canInvert);
}





//============================================================================
//		NDictionary::ForEach : Process each item.
//----------------------------------------------------------------------------
void NDictionary::ForEach(const NDictionaryForEachFunctor &theFunctor)
{	const	NDictionaryValue			*theDict;
	NDictionaryValueConstIterator		theIter;



	// Get the state we need
	theDict = GetImmutable();



	// Process the array
	for (theIter = theDict->begin(); theIter != theDict->end(); theIter++)
		theFunctor(theIter->first, theIter->second);
}





//============================================================================
//		NDictionary::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NDictionary::HasKey(const NString &theKey) const
{	bool								hasValue;
	const NDictionaryValue				*theDict;
	NDictionaryValueConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Find the value
	theDict  = GetImmutable();
	theIter  = theDict->find(theKey);
	hasValue = (theIter != theDict->end());

	return(hasValue);
}





//============================================================================
//		NDictionary::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NDictionary::RemoveKey(const NString &theKey)
{	NDictionaryValue	*theDict;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the state we need
	theDict = GetMutable();



	// Remove the key
	theDict->erase(theKey);
}





//============================================================================
//		NDictionary::GetKeys : Get the keys.
//----------------------------------------------------------------------------
NStringList NDictionary::GetKeys(bool sortKeys) const
{	const NDictionaryValue				*theDict;
	NDictionaryValueConstIterator		theIter;
	NStringList							theKeys;



	// Get the state we need
	theDict = GetImmutable();



	// Get the keys
	for (theIter = theDict->begin(); theIter != theDict->end(); theIter++)
		theKeys.push_back(theIter->first);

	if (sortKeys)
		sort(theKeys);

	return(theKeys);
}





//============================================================================
//		NDictionary::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NDictionary::GetValue(const NString &theKey) const
{	const NDictionaryValue				*theDict;
	NVariant							theValue;
	NDictionaryValueConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theDict = GetImmutable();
	theIter = theDict->find(theKey);

	if (theIter != theDict->end())
		{
		theValue = theIter->second;
		NN_ASSERT(theValue.IsValid());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::SetValue : Set a value.
//----------------------------------------------------------------------------
void NDictionary::SetValue(const NString &theKey, const NVariant &theValue)
{	NDictionaryValue		*theDict;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(theValue.IsValid());



	// Set the value
	theDict            = GetMutable();
	(*theDict)[theKey] = theValue;
}





//============================================================================
//		NDictionary::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NDictionary::GetValueBoolean(const NString &theKey) const
{	NNumber		theNumber;
	NVariant	rawValue;
	bool		theValue;



	// Get the value
	theValue = 0;
	
	if (GetValue(theKey, rawValue))
		{
		if (!theNumber.SetValue(rawValue) || !theNumber.GetValueBoolean(theValue))
			NN_LOG("Unable to convert [%s] to bool", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NDictionary::GetValueSInt32(const NString &theKey) const
{	NNumber		theNumber;
	NVariant	rawValue;
	SInt32		theValue;



	// Get the value
	theValue = 0;
	
	if (GetValue(theKey, rawValue))
		{
		if (!theNumber.SetValue(rawValue) || !theNumber.GetValueSInt32(theValue))
			NN_LOG("Unable to convert [%s] to SInt32", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NDictionary::GetValueSInt64(const NString &theKey) const
{	NNumber		theNumber;
	NVariant	rawValue;
	SInt64		theValue;



	// Get the value
	theValue = 0;
	
	if (GetValue(theKey, rawValue))
		{
		if (!theNumber.SetValue(rawValue) || !theNumber.GetValueSInt64(theValue))
			NN_LOG("Unable to convert [%s] to SInt64", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NDictionary::GetValueFloat32(const NString &theKey) const
{	NNumber		theNumber;
	NVariant	rawValue;
	Float32		theValue;



	// Get the value
	theValue = 0.0f;
	
	if (GetValue(theKey, rawValue))
		{
		if (!theNumber.SetValue(rawValue) || !theNumber.GetValueFloat32(theValue))
			NN_LOG("Unable to convert [%s] to Float32", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NDictionary::GetValueFloat64(const NString &theKey) const
{	NNumber		theNumber;
	NVariant	rawValue;
	Float64		theValue;



	// Get the value
	theValue = 0.0;
	
	if (GetValue(theKey, rawValue))
		{
		if (!theNumber.SetValue(rawValue) || !theNumber.GetValueFloat64(theValue))
			NN_LOG("Unable to convert [%s] to Float64", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NDictionary::GetValueString(const NString &theKey) const
{	NString		theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NString", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
NPoint NDictionary::GetValuePoint(const NString &theKey) const
{	NPoint		theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NPoint", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
NSize NDictionary::GetValueSize(const NString &theKey) const
{	NSize		theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NSize", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueRectangle : Get a rectangle value.
//----------------------------------------------------------------------------
NRectangle NDictionary::GetValueRectangle(const NString &theKey) const
{	NRectangle	theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NRectangle", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NDictionary::GetValueColor(const NString &theKey) const
{	NColor		theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NColor", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NDate NDictionary::GetValueDate(const NString &theKey) const
{	NDate		theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NDate", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NDictionary::GetValueArray(const NString &theKey) const
{	NArray		theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NArray", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NDictionary::GetValueDictionary(const NString &theKey) const
{	NDictionary		theValue;
	NVariant		rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NDictionary", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NDictionary::GetValueData(const NString &theKey) const
{	NData		theValue;
	NVariant	rawValue;



	// Get the value
	if (GetValue(theKey, rawValue))
		{
		if (!rawValue.GetValue(theValue))
			NN_LOG("Unable to convert [%s] to NData", theKey.GetUTF8());
		}

	return(theValue);
}





//============================================================================
//		NDictionary::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
const NDictionaryValue *NDictionary::GetNullValue(void) const
{	static NDictionaryValue		sNullValue;



	// Get the value
	return(&sNullValue);
}



//============================================================================
//		NDictionary::GetValue : Get a value value.
//----------------------------------------------------------------------------
bool NDictionary::GetValue(const NString &theKey, NVariant &theValue) const
{


	// Get the value
	theValue = GetValue(theKey);
	
	return(theValue.IsValid());
}

