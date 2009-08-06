/*	NAME:
		NCFString.cpp

	DESCRIPTION:
		CFStringRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFData.h"
#include "NCFString.h"





//============================================================================
//		NCFString::NCFString : Constructor.
//----------------------------------------------------------------------------
NCFString::NCFString(const NString &theString)
			: NString(theString)
{
}





//============================================================================
//		NCFString::NCFString : Constructor.
//----------------------------------------------------------------------------
NCFString::NCFString(CFStringRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCFString::NCFString : Constructor.
//----------------------------------------------------------------------------
NCFString::NCFString(void)
{
}





//============================================================================
//		NCFString::~NCFString : Destructor.
//----------------------------------------------------------------------------
NCFString::~NCFString(void)
{
}





//============================================================================
//		NCFString::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCFString::GetObject(void) const
{	NCFObject		theObject;



	// Get the object
	theObject.SetObject(CFStringCreateWithCString(kCFAllocatorNano, GetUTF8(), kCFStringEncodingUTF8));

	return(theObject);
}





//============================================================================
//		NCFString::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFString::SetObject(CFStringRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	NCFData			theData;
	bool			isValid;



	// Get the state we need
	isValid = (cfObject != NULL);
	Clear();



	// Set the object
	if (isValid)
		{
		if (theData.SetObject(CFStringCreateExternalRepresentation(kCFAllocatorNano, cfObject, kCFStringEncodingUTF8, 0)))
			SetData(theData, kNStringEncodingUTF8);
		}

	return(isValid);
}

