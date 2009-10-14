/*	NAME:
		NHIObject.cpp

	DESCRIPTION:
		HIObjectRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NIBUtilities.h"
#include "NHIObject.h"





//============================================================================
//		NHIObject::NHIObject : Constructor.
//----------------------------------------------------------------------------
NHIObject::NHIObject(void)
{


	// Initialize ourselves
	mObject = NULL;
}





//============================================================================
//		NHIObject::~NHIObject : Destructor.
//----------------------------------------------------------------------------
NHIObject::~NHIObject(void)
{	HIObjectRef		hiObject;



	// Clean up
	//
	// We can be destroyed in two ways - by destroying the C++ object with
	// delete, or receiving a kEventHIObjectDestruct after our HIObjectRef
	// has been released.
	//
	// When destroyed with delete we need to release our object, and when
	// destroyed via kEventHIObjectDestruct we need to delete the C++ object.
	//
	// We reset mObject before releasing, to ensure that kEventHIObjectDestruct
	// will not delete the C++ object a second time.
	if (mObject != NULL)
		ForgetObject();
	
	hiObject = mObject;
	mObject  = NULL;
	
	CFSafeRelease(hiObject);
}





//============================================================================
//      NHIObject::IsValid : Is the object valid?
//----------------------------------------------------------------------------
bool NHIObject::IsValid(void) const
{


	// Check our state
	return(mObject != NULL);
}





//============================================================================
//      NHIObject::IsOfClass : Is this object of a certain class?
//----------------------------------------------------------------------------
bool NHIObject::IsOfClass(const NString &classID) const
{


    // Validate our parameters
    NN_ASSERT(classID.IsNotEmpty());



    // Check the class ID
    return(HIObjectIsOfClass(mObject, classID));
}





//============================================================================
//      NHIObject::GetHIObject : Get the HIObjectRef.
//----------------------------------------------------------------------------
HIObjectRef NHIObject::GetHIObject(void) const
{


	// Get the object
	return(mObject);
}





//============================================================================
//		NHIObject::GetClassID : Get the class ID.
//----------------------------------------------------------------------------
NString NHIObject::GetClassID(void) const
{	NString		classID;



	// Get the class ID
	if (mObject != NULL)
		classID.Set(HIObjectCopyClassID(mObject));

	return(classID);
}





//============================================================================
//		NHIObject::GetNHIObject : Get the NHIObject for an HIObjectRef.
//----------------------------------------------------------------------------
NHIObject *NHIObject::GetNHIObject(HIObjectRef hiObject)
{	NHIObjectMapConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(hiObject != NULL);



	// Find the object
	theIter = GetObjects().find(hiObject);
	if (theIter != GetObjects().end())
		return(theIter->second);
	
	return(NULL);
}





//============================================================================
//		NHIObject::DoEventHIObjectConstruct : Handle kEventHIObjectConstruct.
//----------------------------------------------------------------------------
//		Note : Invoked for objects that are constructed with HIObjectCreate.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NHIObject::DoEventHIObjectConstruct(NCarbonEvent &theEvent)
{


	// Initialize ourselves
	mObject = theEvent.GetParameterHIObject(kEventParamHIObjectInstance);
	NN_ASSERT(mObject != NULL);



	// Save the object
	SaveObject();
	
	return(noErr);
}





//============================================================================
//		NHIObject::DoEventHIObjectDestruct : Handle kEventHIObjectDestruct.
//----------------------------------------------------------------------------
OSStatus NHIObject::DoEventHIObjectDestruct(NCarbonEvent &/*theEvent*/)
{	bool	deleteThis;



	// Clean up
	//
	// We can be destroyed in two ways - by destroying the C++ object with
	// delete, or receiving a kEventHIObjectDestruct after our HIObjectRef
	// has been released.
	//
	// When destroyed with delete we need to release our object, and when
	// destroyed via kEventHIObjectDestruct we need to delete the C++ object.
	//
	// We test mObject before deleting, to ensure that our destructor will
	// not release the HIObjectRef a second time.
	if (mObject != NULL)
		ForgetObject();
	
	deleteThis = (mObject != NULL);
	mObject    = NULL;

	if (deleteThis)
		delete this;
	
	return(noErr);
}





//============================================================================
//		NHIObject::SetHIObject : Set the HIObjectRef.
//----------------------------------------------------------------------------
//		Note : Invoked for objects that are constructed with NHIObjectFactory.
//----------------------------------------------------------------------------
void NHIObject::SetHIObject(HIObjectRef hiObject)
{	EventTypeSpec		theEvents[] = {	{ kEventClassHIObject,	kEventHIObjectDestruct },
										{ kEventClassNone,		kEventKindNone         } };
	OSStatus			theErr;



	// Validate our parameters and state
	NN_ASSERT(hiObject != NULL);
	NN_ASSERT( mObject == NULL);



	// Set the object
	mObject = hiObject;

	SaveObject();



	// Register for events
	theErr = InstallHandler(mObject, theEvents);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIObject::SaveObject : Save an object reference.
//----------------------------------------------------------------------------
void NHIObject::SaveObject(void)
{


	// Validate our state
	NN_ASSERT(mObject != NULL);



	// Save this object
	NN_ASSERT(GetNHIObject(mObject) == NULL);
	GetObjects()[mObject] = this;
}





//============================================================================
//		NHIObject::ForgetObject : Forget an object reference.
//----------------------------------------------------------------------------
void NHIObject::ForgetObject(void)
{


	// Validate our state
	NN_ASSERT(mObject != NULL);



	// Remove the handler
	//
	// Since the object is going away, our event handler can be removed since
	// we no longer need/wish to receive events for this object (typically
	// because it is being destroyed).
	RemoveHandler();



	// Forget this object
	NN_ASSERT(GetNHIObject(mObject) != NULL);
	GetObjects().erase(mObject);
}





//============================================================================
//		NHIObject::GetObjects : Get the object list.
//----------------------------------------------------------------------------
#pragma mark -
NHIObjectMap &NHIObject::GetObjects(void)
{	static NHIObjectMap		sObjectMap;



	// Get the objects
	//
	// Unfortunately HIObject does not provide a way to tag objects with custom
	// data (rdar://4507564), and so we need to maintain a map of all NHIObjects
	// that have been created in order to do a reverse lookup.
	//
	// Although some HIObject sub-classes (e.g., HIView) have a property tagging
	// system that could be used to store a back pointer, some (HIToolbar and
	// HIToolbarItem) do not.
	//
	// To provide a generic mechanism for any HIObject, we implement it at the
	// base class.
	//
	// This map is updated as objects are constructed/destroyed, and is a local
	// static to ensure deterministic access during static initialisation.
	return(sObjectMap);
}


