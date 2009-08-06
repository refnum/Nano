/*	NAME:
		NCFObject.cpp

	DESCRIPTION:
		CFTypeRef wrapper.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"





//============================================================================
//		NCFObject::NCFObject : Constructor.
//----------------------------------------------------------------------------
NCFObject::NCFObject(CFTypeRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	InitializeSelf(cfObject, takeOwnership);
}





//============================================================================
//		NCFObject::NCFObject : Constructor.
//----------------------------------------------------------------------------
NCFObject::NCFObject(const NCFObject &otherObject)
{


	// Initialize ourselves
	InitializeSelf(otherObject.mObject, false);
}





//============================================================================
//		NCFObject::NCFObject : Constructor.
//----------------------------------------------------------------------------
NCFObject::NCFObject(void)
{


	// Initialize ourselves
	InitializeSelf(NULL, false);
}





//============================================================================
//		NCFObject::~NCFObject : Destructor.
//----------------------------------------------------------------------------
NCFObject::~NCFObject(void)
{


	// Clean up
	CFSafeRelease(mObject);
}





//============================================================================
//		NCFObject::IsValid : Is the object valid?
//----------------------------------------------------------------------------
bool NCFObject::IsValid(void) const
{


	// Check our state
	return(mObject != NULL);
}





//============================================================================
//		NCFObject::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCFObject::SetObject(CFTypeRef cfObject, bool takeOwnership)
{


	// Retain the object
	CFSafeRetain(cfObject);



	// Assign the object
	CFSafeRelease(mObject);
	
	mObject = cfObject;



	// Update the ref counts
	//
	// If we're to take ownership then our initial retain needs to be balanced
	// with a release (we always retain before assigning, to ensure that we can
	// be passed the existing object even if we hold the last ref).
	//
	// If we're not to take ownership then we needed to retain the object anyway,
	// which we've already done.
	if (takeOwnership)
		CFSafeRelease(cfObject);

	return(mObject != NULL);
}





//============================================================================
//		NCFObject::= : Assignment operator.
//----------------------------------------------------------------------------
const NCFObject& NCFObject::operator = (const NCFObject &theObject)
{


	// Assign the object
	if (this != &theObject)
		SetObject(theObject.mObject, false);

	return(*this);
}





//============================================================================
//		NCFObject::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCFObject::InitializeSelf(CFTypeRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	mObject = cfObject;
	
	if (!takeOwnership)
		CFSafeRetain(mObject);
}




