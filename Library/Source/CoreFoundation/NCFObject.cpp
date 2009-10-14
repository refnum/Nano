/*	NAME:
		NCFObject.cpp

	DESCRIPTION:
		CoreFoundation object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
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
	InitializeSelf(theObject.GetMode(), theObject);
}





//============================================================================
//		NCFObject::NCFObject : Constructor.
//----------------------------------------------------------------------------
NCFObject::NCFObject(void)
{


	// Initialize ourselves
	InitializeSelf(kCFRetain, NULL);
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
//		NCFObject::GetMode : Get the reference mode.
//----------------------------------------------------------------------------
CFReferenceMode NCFObject::GetMode(void) const
{


	// Get the mode
	return(mMode);
}





//============================================================================
//		NCFObject::SetMode : Set the reference mode.
//----------------------------------------------------------------------------
void NCFObject::SetMode(CFReferenceMode theMode)
{


	// Validate our state
	NN_ASSERT(mObject == NULL);



	// Set the mode
	mMode = theMode;
}





//============================================================================
//		NCFObject::Set : Set the object.
//----------------------------------------------------------------------------
bool NCFObject::Set(CFTypeRef cfObject, bool andRelease)
{	CFTypeRef	cfNewValue;
	bool		wasValid;



	// Validate our parameters
	if (cfObject != NULL)
		NN_ASSERT(mObject != cfObject);



	// Get the state we need
	//
	// If the assigned object is NULL, we allow sub-classes to provide a replacement
	// value (which defaults to NULL) so that they can ensure they always contain
	// a valid value.
	cfNewValue = cfObject;
	wasValid   = (cfObject != NULL);
	
	if (!wasValid)
		cfNewValue = GetNullValue();



	// Assign the object
	CFSafeRelease(mObject);

	switch (mMode) {
		case kCFRetain:
			mObject = cfNewValue;
			CFSafeRetain(mObject);
			break;

		case kCFDuplicate:
			mObject = CreateDuplicate(cfNewValue);
			break;

		default:
			NN_LOG("Unknown CF mode: %d", mMode);
			break;
		}

	DoObjectChanged();



	// Clean up
	if (andRelease)
		CFSafeRelease(cfObject);

	return(wasValid);
}





//============================================================================
//		NCFObject::= : Assignment operator.
//----------------------------------------------------------------------------
const NCFObject& NCFObject::operator = (const NCFObject &theObject)
{


	// Assign the object
	Set(theObject, false);
	
	return(*this);
}





//============================================================================
//		NCFObject::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NCFObject::GetNullValue(void)
{


	// Get the null value
	return(NULL);
}





//============================================================================
//		NCFObject::DoObjectChanged : Handle an object change.
//----------------------------------------------------------------------------
//		Note : Override to flush state when the object is changed.
//----------------------------------------------------------------------------
void NCFObject::DoObjectChanged(void)
{
}





//============================================================================
//		NCFObject::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCFObject::InitializeSelf(CFReferenceMode theMode, CFTypeRef cfObject)
{


	// Initialize ourselves
	mMode   = theMode;
	mObject = NULL;



	// Assign the object
	Set(cfObject, false);
}





//============================================================================
//		NCFObject::CreateDuplicate : Duplicate an object.
//----------------------------------------------------------------------------
CFTypeRef NCFObject::CreateDuplicate(CFTypeRef cfObject)
{
#if NN_DEBUG
	static CFTypeID kCFTypeArray		= CFArrayGetTypeID();
	static CFTypeID kCFTypeBoolean		= CFBooleanGetTypeID();
	static CFTypeID kCFTypeData			= CFDataGetTypeID();
	static CFTypeID kCFTypeDate			= CFDateGetTypeID();
	static CFTypeID kCFTypeDictionary	= CFDictionaryGetTypeID();
	static CFTypeID kCFTypeNumber		= CFNumberGetTypeID();
	static CFTypeID kCFTypeString		= CFStringGetTypeID();
#endif
	static CFTypeID kCFTypeURL			= CFURLGetTypeID();

	CFTypeID	theType;
	CFTypeRef	cfCopy;



	// Duplicate the object
	//
	// CF does not provide a "duplicate object" API (rdar://4523716), and so
	// duplicating an object requires that we check the type and use the most
	// appropriate API.
	//
	// CFPropertyListCreateDeepCopy can only duplicate property list objects,
	// and all other objects need to be copied with a type-specific API.
	theType = CFGetTypeID(cfObject);
	
	if (theType == kCFTypeURL)
		cfCopy = (CFTypeRef) CFURLCreateWithString(NULL, CFURLGetString((CFURLRef) cfObject), NULL);
	
	else
		{
		cfCopy = CFPropertyListCreateDeepCopy(NULL, cfObject, kCFPropertyListMutableContainersAndLeaves);
		
		NN_ASSERT(theType == kCFTypeArray      ||
				  theType == kCFTypeBoolean    ||
				  theType == kCFTypeData       ||
				  theType == kCFTypeDate       ||
				  theType == kCFTypeDictionary ||
				  theType == kCFTypeNumber     ||
				  theType == kCFTypeString);
		}



	// Validate our state
	//
	// Since some objects can not be duplicated at all (e.g., CFBundleRef), or are unlikely
	// to want to be dupliated (e.g., CGImageRef), we do not try and support every option.
	//
	// If this assertion fails, it will probably be because CFPropertyListCreateDeepCopy
	// was passed a non-property list type.
	//
	// Assuming duplication makes sense, that object type should be handled with a new
	// type-specific CreateCopy API as per CFURL.
	NN_ASSERT(cfCopy != NULL);

	return(cfCopy);
}


