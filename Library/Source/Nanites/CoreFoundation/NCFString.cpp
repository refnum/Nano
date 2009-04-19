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
NCFString::NCFString(CFStringRef cfString, bool takeOwnership)
{


	// Initialize ourselves
	Set(cfString, takeOwnership);
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


	// Clean up
	NCFObject::ReleaseObjects(mObjects);
}





//============================================================================
//		NCFString::Set : Assign an object.
//----------------------------------------------------------------------------
bool NCFString::Set(CFStringRef cfString, bool takeOwnership)
{	NData			theData;
	NCFObject		cfData;



	// Assign the string
	if (cfString != NULL)
		{
		if (cfData.Set(CFStringCreateExternalRepresentation(kCFAllocatorNano, cfString, kCFStringEncodingUTF8, 0)))
			{
			theData = NData(CFDataGetLength(cfData), CFDataGetBytePtr(cfData));
			SetData(theData, kNStringEncodingUTF8);
			}

		if (takeOwnership)
			CFSafeRelease(cfString);
		}
	
	return(cfString != NULL);
}





//============================================================================
//		NCFString::CFStringRef : Cast operator.
//----------------------------------------------------------------------------
NCFString::operator CFStringRef(void) const
{	CFStringRef		cfString;



	// Convert the string
	cfString = CFStringCreateWithCString(kCFAllocatorNano, GetUTF8(), kCFStringEncodingUTF8);
	if (cfString != NULL)
		mObjects.push_back(cfString);
		
	return(cfString);
}
