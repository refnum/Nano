/*	NAME:
		NCFDictionary.cpp

	DESCRIPTION:
		CFDictionaryRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMacTarget.h"
#include "NCFString.h"
#include "NCFDictionary.h"





//============================================================================
//		NCFDictionary::NCFDictionary : Constructor.
//----------------------------------------------------------------------------
NCFDictionary::NCFDictionary(const NDictionary &theDictionary)
			: NDictionary(theDictionary)
{
}





//============================================================================
//		NCFDictionary::NCFDictionary : Constructor.
//----------------------------------------------------------------------------
NCFDictionary::NCFDictionary(CFDictionaryRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFDictionary::NCFDictionary : Constructor.
//----------------------------------------------------------------------------
NCFDictionary::NCFDictionary(void)
{
}





//============================================================================
//		NCFDictionary::~NCFDictionary : Destructor.
//----------------------------------------------------------------------------
NCFDictionary::~NCFDictionary(void)
{
}





//============================================================================
//		NCFDictionary::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFDictionary::GetObject(void) const
{	NCFObject						theObject, keyObject, theValue;
	CFTypeRef						cfKey, cfValue;
	NStringList						theKeys;
	NStringListConstIterator		theIter;
	NCFString						theKey;



	// Get the state we need
	theKeys = GetKeys();
	
	if (!theObject.SetObject(CFDictionaryCreateMutable(
										kCFAllocatorNano,
										(CFIndex) theKeys.size(),
										&kCFTypeDictionaryKeyCallBacks,
										&kCFTypeDictionaryValueCallBacks)))
		return(theObject);



	// Get the object
	for (theIter = theKeys.begin(); theIter != theKeys.end(); theIter++)
		{
		theKey   = *theIter;
		theValue = NMacTarget::ConvertObjectToCF(GetValue(theKey));
		if (theValue.IsValid())
			{
			keyObject = theKey.GetObject();
			cfKey     = (CFTypeRef) keyObject;
			cfValue   = (CFTypeRef) theValue;
			
			CFDictionarySetValue(theObject, cfKey, cfValue);
			}
		}

	return(theObject);
}





//============================================================================
//		NCFDictionary::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFDictionary::SetObject(CFDictionaryRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	Clear();



	// Set the object
	if (isValid)
		CFDictionaryApplyFunction(cfObject, InsertValue, this);

	return(isValid);
}





#pragma mark private
//============================================================================
//		NCFDictionary::InsertValue : Insert a value.
//----------------------------------------------------------------------------
void NCFDictionary::InsertValue(const void *keyPtr, const void *valuePtr, void *userData)
{	NCFDictionary		*thisPtr = (NCFDictionary *) userData;
	CFTypeRef			cfValue  = (CFTypeRef)       valuePtr;
	CFStringRef			cfKey    = (CFStringRef)     keyPtr;
	NVariant			theValue;
	NCFString			theKey;



	// Insert the value
	theKey   = NCFString(cfKey, false);
	theValue = NMacTarget::ConvertCFToObject(NCFObject(cfValue, false));

	thisPtr->SetValue(theKey, theValue);
}



