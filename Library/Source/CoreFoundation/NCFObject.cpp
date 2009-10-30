/*	NAME:
		NCFObject.cpp

	DESCRIPTION:
		CoreFoundation object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
NCFObject::NCFObject(const NCFObject &theObject)
{


	// Initialize ourselves
	InitializeSelf();
	
	
	
	// Assign the object
	if (theObject.IsValid())
		Set(theObject.mObject, false);
}





//============================================================================
//		NCFObject::NCFObject : Constructor.
//----------------------------------------------------------------------------
NCFObject::NCFObject(void)
{


	// Initialize ourselves
	InitializeSelf();
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
//		NCFObject::Clear : Clear the object.
//----------------------------------------------------------------------------
void NCFObject::Clear(void)
{


	// Clear the object
	Set(NULL, false);
}





//============================================================================
//		NCFObject::GetHash : Get the object hash.
//----------------------------------------------------------------------------
CFHashCode NCFObject::GetHash(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the hash
	return(CFHash(mObject));
}





//============================================================================
//		NCFObject::GetType : Get the object type.
//----------------------------------------------------------------------------
CFTypeID NCFObject::GetType(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the type
	return(CFGetTypeID(mObject));
}





//============================================================================
//		NCFObject::Set : Set the object.
//----------------------------------------------------------------------------
bool NCFObject::Set(CFTypeRef cfObject, bool takeOwnership)
{	bool	wasValid;



	// Validate our parameters
	//
	// Assigning the same object to ourselves is OK, provided there are at least
	// two references to it (ensuring that when we release mObject, cfObject is
	// still a valid object that we can retain).
	if (cfObject != NULL && cfObject == mObject)
		NN_ASSERT(CFGetRetainCount(cfObject) >= 2);



	// Check for NULL
	//
	// If a NULL object is assigned, sub-classes are allowed to provide their own
	// immutable replacement to ensure they always have a valid value.
	wasValid = (cfObject != NULL);

	if (!wasValid)
		{
		cfObject	  = GetNullValue();
		takeOwnership = false;
		}



	// Release the old object
	DoObjectChanging();

	CFSafeRelease(mObject);



	// Copy the object
	//
	// Unless we are taking ownership, we store a new immutable reference to the object.
	//
	// CreateCopy can either acquire a new reference if the object is immutable, or duplicate the
	// object if it is currently mutable (ensuring that changes to the original do not affect us).
	//
	//
	// Unfortunately many of the CFxxxCreateCopy APIs will actually create duplicates even when
	// passed an immutable object (rdar://5006274), so we also check for the case where we are
	// using the replacement NULL value.
	//
	// In this case we know the object must be immutable, and obtain a significant performance
	// benefit from not making a duplicate copy (since we might never be mutated).
	if (cfObject != NULL && !takeOwnership)
		{
		if (wasValid)
			cfObject = NCoreFoundationUtilities::CreateCopy(cfObject);
		else
			CFSafeRetain(cfObject);
		}



	// Assign the new object
	mIsMutable = false;
	mObject	   = cfObject;

	DoObjectChanged();

	return(wasValid);
}





//============================================================================
//		NCFObject::= : Assignment operator.
//----------------------------------------------------------------------------
const NCFObject& NCFObject::operator = (const NCFObject &theObject)
{


	// Assign the object
	if (this != &theObject)
		Set(theObject.mObject, false);

	return(*this);
}





//============================================================================
//		NCFObject::MakeMutable : Make the object mutable.
//----------------------------------------------------------------------------
#pragma mark -
bool NCFObject::MakeMutable(void)
{


	// Make the object mutable
	//
	// We provide a copy-on-write mechanism for mutable objects, assigning
	// them in their immutable form and then making them mutable on demand.
	if (!mIsMutable)
		{
		mIsMutable = Set(NCoreFoundationUtilities::CreateMutableCopy(mObject), true);
		NN_ASSERT(mIsMutable);
		}
	
	return(mIsMutable);
}





//============================================================================
//		NCFObject::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
CFTypeRef NCFObject::GetNullValue(void)
{


	// Get the null value
	//
	// Sub-classes may wish to override to provide their own "null value".
	return(NULL);
}





//============================================================================
//		NCFObject::DoObjectChanging : Handle an object change.
//----------------------------------------------------------------------------
void NCFObject::DoObjectChanging(void)
{
}





//============================================================================
//		NCFObject::DoObjectChanged : Handle an object change.
//----------------------------------------------------------------------------
void NCFObject::DoObjectChanged(void)
{
}





//============================================================================
//		NCFObject::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCFObject::InitializeSelf(void)
{


	// Initialize ourselves
	mIsMutable = false;
	mObject	   = NULL;
}
