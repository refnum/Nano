/*	NAME:
		NCFArray.cpp

	DESCRIPTION:
		CFArrayRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMacTarget.h"
#include "NCFArray.h"





//============================================================================
//		NCFArray::NCFArray : Constructor.
//----------------------------------------------------------------------------
NCFArray::NCFArray(const NArray &theArray)
			: NArray(theArray)
{
}





//============================================================================
//		NCFArray::NCFArray : Constructor.
//----------------------------------------------------------------------------
NCFArray::NCFArray(CFArrayRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFArray::NCFArray : Constructor.
//----------------------------------------------------------------------------
NCFArray::NCFArray(void)
{
}





//============================================================================
//		NCFArray::~NCFArray : Destructor.
//----------------------------------------------------------------------------
NCFArray::~NCFArray(void)
{
}





//============================================================================
//		NCFArray::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFArray::GetObject(void) const
{	NCFObject		theObject, theValue;
	NIndex			n, numItems;



	// Get the state we need
	numItems = GetSize();
	
	if (!theObject.SetObject(CFArrayCreateMutable(kCFAllocatorNano, numItems, &kCFTypeArrayCallBacks)))
		return(theObject);



	// Get the object
	for (n = 0; n < numItems; n++)
		{
		theValue = NMacTarget::ConvertObjectToCF(GetValue(n));
		if (theValue.IsValid())
			CFArrayAppendValue(theObject, (CFTypeRef) theValue);
		}

	return(theObject);
}





//============================================================================
//		NCFArray::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFArray::SetObject(CFArrayRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	NIndex			n, numItems;
	NVariant		theValue;
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	Clear();



	// Set the object
	if (isValid)
		{
		numItems = CFArrayGetCount(cfObject);
		for (n = 0; n < numItems; n++)
			{
			theValue = NMacTarget::ConvertCFToObject(CFArrayGetValueAtIndex(cfObject, n));
			AppendValue(theValue);
			}
		}

	return(isValid);
}


