/*	NAME:
		NCoreFoundationUtilities.cpp

	DESCRIPTION:
		CoreFoundation utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDictionary.h"
#include "NMathUtilities.h"
#include "NCoreFoundationUtilities.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
CFTypeID NCoreFoundationUtilities::mCFTypeArray;
CFTypeID NCoreFoundationUtilities::mCFTypeAttributedString;
CFTypeID NCoreFoundationUtilities::mCFTypeBag;
CFTypeID NCoreFoundationUtilities::mCFTypeBitVector;
CFTypeID NCoreFoundationUtilities::mCFTypeBoolean;
CFTypeID NCoreFoundationUtilities::mCFTypeCharacterSet;
CFTypeID NCoreFoundationUtilities::mCFTypeData;
CFTypeID NCoreFoundationUtilities::mCFTypeDictionary;
CFTypeID NCoreFoundationUtilities::mCFTypeNumber;
CFTypeID NCoreFoundationUtilities::mCFTypeSet;
CFTypeID NCoreFoundationUtilities::mCFTypeString;





//============================================================================
//		NCoreFoundationUtilities::CreateCopy : Create an immutable copy.
//----------------------------------------------------------------------------
CFTypeRef NCoreFoundationUtilities::CreateCopy(CFTypeRef cfObject)
{	CFTypeID		theType;
	CFTypeRef		cfCopy;



	// Validate our parameters
	NN_ASSERT(cfObject != NULL);



	// Update our state
	InitializeTypeIDs();
	


	// Create the copy
	//
	// Unfortunately CF does not provide a general "copy object" API (rdar://4523716),
	// other than CFPropertyListCreateDeepCopy.
	//
	// Since CFPropertyListCreateDeepCopy does not handle all types, we have to assume
	// that all CF types are immutable (and so can be "copied" with a retain) except
	// for the sub-set which also come in mutable flavors.
	//
	// These types can be copied with their CFXXXCreateCopy API, which may "copy" an
	// immutable object with a retain and create a new copy if passed a mutable object.
	//
	//
	// Unfortunately since there is no "is mutable" API (rdar://4993877), we can not
	// assert anything about the final case (running the risk of our hard-coded list
	// failing if a new mutable type is introduced).
	//
	// Since CF uses the same type ID for mutable as immutable objects, we can not even
	// hard-code a list of known CF types and assert if we see a new type - as one of
	// the existing types (say CFNumber) could be extended to support a mutable variant.
	theType = CFGetTypeID(cfObject);

	if (theType == mCFTypeArray)
		cfCopy = CFArrayCreateCopy(kCFAllocatorNano, (CFArrayRef) cfObject);
	
    else if (theType == mCFTypeAttributedString && CFAttributedStringCreateCopy != NULL)
		cfCopy = CFAttributedStringCreateCopy(kCFAllocatorNano, (CFAttributedStringRef) cfObject);
	
	else if (theType == mCFTypeBag)
		cfCopy = CFBagCreateCopy(kCFAllocatorNano, (CFBagRef) cfObject);
	
	else if (theType == mCFTypeBitVector)
		cfCopy = CFBitVectorCreateCopy(kCFAllocatorNano, (CFBitVectorRef) cfObject);

	else if (theType == mCFTypeCharacterSet)
		cfCopy = CFCharacterSetCreateCopy(kCFAllocatorNano, (CFCharacterSetRef) cfObject);
	
	else if (theType == mCFTypeData)
		cfCopy = CFDataCreateCopy(kCFAllocatorNano, (CFDataRef) cfObject);

	else if (theType == mCFTypeDictionary)
		cfCopy = CFDictionaryCreateCopy(kCFAllocatorNano, (CFDictionaryRef) cfObject);

	else if (theType == mCFTypeSet)
		cfCopy = CFSetCreateCopy(kCFAllocatorNano, (CFSetRef) cfObject);

	else if (theType == mCFTypeString)
		cfCopy = CFStringCreateCopy(kCFAllocatorNano, (CFStringRef) cfObject);
		
	else
		cfCopy = CFRetain(cfObject);

	return(cfCopy);
}





//============================================================================
//		NCoreFoundationUtilities::CreateMutableCopy : Create a mutable copy.
//----------------------------------------------------------------------------
CFTypeRef NCoreFoundationUtilities::CreateMutableCopy(CFTypeRef cfObject)
{	CFTypeID		theType;
	CFTypeRef		cfCopy;



	// Validate our parameters
	NN_ASSERT(cfObject != NULL);



	// Update our state
	InitializeTypeIDs();
	


	// Create the copy
	//
	// As with immutable copies, the lack of a general API means we have
	// to hard-code a list of specific types that need extending over time.
	theType = CFGetTypeID(cfObject);

	if (theType == mCFTypeArray)
		cfCopy = CFArrayCreateMutableCopy(kCFAllocatorNano, 0, (CFArrayRef) cfObject);
	
    else if (theType == mCFTypeAttributedString && CFAttributedStringCreateMutableCopy != NULL)
		cfCopy = CFAttributedStringCreateMutableCopy(kCFAllocatorNano, 0, (CFAttributedStringRef) cfObject);
	
	else if (theType == mCFTypeBag)
		cfCopy = CFBagCreateMutableCopy(kCFAllocatorNano, 0, (CFBagRef) cfObject);
	
	else if (theType == mCFTypeBitVector)
		cfCopy = CFBitVectorCreateMutableCopy(kCFAllocatorNano, 0, (CFBitVectorRef) cfObject);

	else if (theType == mCFTypeCharacterSet)
		cfCopy = CFCharacterSetCreateMutableCopy(kCFAllocatorNano, (CFCharacterSetRef) cfObject);
	
	else if (theType == mCFTypeData)
		cfCopy = CFDataCreateMutableCopy(kCFAllocatorNano, 0, (CFDataRef) cfObject);

	else if (theType == mCFTypeDictionary)
		cfCopy = CFDictionaryCreateMutableCopy(kCFAllocatorNano, 0, (CFDictionaryRef) cfObject);

	else if (theType == mCFTypeSet)
		cfCopy = CFSetCreateMutableCopy(kCFAllocatorNano, 0, (CFSetRef) cfObject);

	else if (theType == mCFTypeString)
		cfCopy = CFStringCreateMutableCopy(kCFAllocatorNano, 0, (CFStringRef) cfObject);
		
	else
		{
		NN_LOG("Unable to create mutable copy!");
		cfCopy = NULL;
		}

	return(cfCopy);
}





//============================================================================
//		NCoreFoundationUtilities::PurgeValues : Purge a dictionary's values.
//----------------------------------------------------------------------------
void NCoreFoundationUtilities::PurgeValues(CFMutableDictionaryRef cfDictionary, bool canRecurse)
{	Float64						valueFloat64;
	SInt64						valueSInt64;
	bool						purgeValue;
	UInt32						n, numKeys;
	CFStringRef					*keyPtrs;
	CFTypeRef					cfValue;
	NStringList					theKeys;
	NStringListConstIterator	theIter;
	NData						theData;
	CFTypeID					cfType;
	CFStringRef					cfKey;



	// Validate our parameters
	NN_ASSERT(CFGetTypeID(cfDictionary) == CFDictionaryGetTypeID());



	// Update our state
	InitializeTypeIDs();



	// Get the state we need
	numKeys = CFDictionaryGetCount(cfDictionary);
	keyPtrs = (CFStringRef *) theData.AppendData(numKeys * sizeof(CFStringRef));

	if (keyPtrs == NULL)
		return;

	CFDictionaryGetKeysAndValues(cfDictionary, (const void **) keyPtrs, NULL);



	// Purge the values
	for (n = 0; n < numKeys; n++)
		{
		// Get the state we need
		cfKey   = keyPtrs[n];
		cfValue = CFDictionaryGetValue(cfDictionary, cfKey);
		cfType  = CFGetTypeID(cfValue);
		


		// Examine the value
		purgeValue = false;

		if (cfType == mCFTypeBoolean)
			purgeValue = (CFBooleanGetValue((CFBooleanRef) cfValue) == false);
		
		else if (cfType == mCFTypeString)
			purgeValue = (CFStringGetLength((CFStringRef) cfValue) == 0);

		else if (cfType == mCFTypeNumber)
			{
			switch (CFNumberGetType((CFNumberRef) cfValue)) {
				case kCFNumberSInt8Type:
				case kCFNumberSInt16Type:
				case kCFNumberSInt32Type:
				case kCFNumberSInt64Type:
				case kCFNumberCharType:
				case kCFNumberShortType:
				case kCFNumberIntType:
				case kCFNumberLongType:
				case kCFNumberLongLongType:
				case kCFNumberCFIndexType:
					if (CFNumberGetValue((CFNumberRef) cfValue, kCFNumberSInt64Type, &valueSInt64))
						purgeValue = (valueSInt64 == 0);
					break;
				
				case kCFNumberFloat32Type:
				case kCFNumberFloat64Type:
				case kCFNumberFloatType:
				case kCFNumberDoubleType:
					if (CFNumberGetValue((CFNumberRef) cfValue, kCFNumberFloat64Type, &valueFloat64))
						purgeValue = NMathUtilities::IsZero(valueFloat64);
					break;
				
				default:
					break;
				}
			}
		
		else if (cfType == mCFTypeArray)
			{
			if (canRecurse)
				PurgeArray((CFArrayRef) cfValue, NULL);

			purgeValue = (CFArrayGetCount((CFArrayRef) cfValue) == 0);
			}

		else if (cfType == mCFTypeDictionary)
			{
			if (canRecurse)
				PurgeValues((CFMutableDictionaryRef) cfValue, canRecurse);

			purgeValue = (CFDictionaryGetCount((CFDictionaryRef) cfValue) == 0);
			}

		else if (cfType == mCFTypeData)
			purgeValue = (CFDataGetLength((CFDataRef) cfValue) == 0);



		// Purge empty values
		if (purgeValue)
			CFDictionaryRemoveValue(cfDictionary, cfKey);
		}
}





//============================================================================
//		NCoreFoundationUtilities::InitializeTypeIDs : Initialize the CF types.
//----------------------------------------------------------------------------
#pragma mark -
void NCoreFoundationUtilities::InitializeTypeIDs(void)
{	static bool		sInitializedTypes = false;



	// Initialize the type IDs
	//
	// We maintain a cache of CF type IDs, to avoid having to make up to a dozen
	// function calls when copying an object (unfortunately the CF type IDs are
	// fetched through functions, rather than named constants, so we still need
	// to use if- statements rather than a switch).
	//
	// Since we may need to initialize these type IDs during static initialization,
	// we maintain a local static to track our current status.
	if (!sInitializedTypes)
		{
		mCFTypeArray				= CFArrayGetTypeID();
        mCFTypeAttributedString		= (CFAttributedStringGetTypeID == NULL) ? CFNullGetTypeID() : CFAttributedStringGetTypeID();
		mCFTypeBag					= CFBagGetTypeID();
		mCFTypeBitVector			= CFBitVectorGetTypeID();
		mCFTypeBoolean				= CFBooleanGetTypeID();
		mCFTypeCharacterSet			= CFCharacterSetGetTypeID();
		mCFTypeData					= CFDataGetTypeID();
		mCFTypeDictionary			= CFDictionaryGetTypeID();
		mCFTypeNumber				= CFNumberGetTypeID();
		mCFTypeSet					= CFSetGetTypeID();
		mCFTypeString				= CFStringGetTypeID();
		
		sInitializedTypes = true;
		}
}





//============================================================================
//		NCoreFoundationUtilities::PurgeArray : Purge an array's dictionaries.
//----------------------------------------------------------------------------
void NCoreFoundationUtilities::PurgeArray(const void *theValue, void */*userData*/)
{	CFRange		cfRange;
	CFTypeID	cfType;



	// Get the state we need
	cfType = CFGetTypeID((CFTypeRef) theValue);



	// Purge containers recursively
	//
	// Although the containers themselves can't be removed from the array,
	// we recurse into them to ensure we purge any contained dictionaries.
	if (cfType == mCFTypeDictionary)
		PurgeValues((CFMutableDictionaryRef) theValue, true);
	
	else if (cfType == mCFTypeArray)
		{
		cfRange = CFRangeMake(0, CFArrayGetCount((CFArrayRef) theValue));

		CFArrayApplyFunction((CFArrayRef) theValue, cfRange, PurgeArray, NULL);
		}
}
