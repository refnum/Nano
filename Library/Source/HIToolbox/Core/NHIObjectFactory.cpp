/*	NAME:
		NHIObjectFactory.cpp

	DESCRIPTION:
		HIObject factory.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NHIObjectFactory.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kHIControlAdapterClassID			= "com.apple.hitoolbox.view.cdefadapter";





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NHIObject *NHIObjectFactory::mNextInstance;





//============================================================================
//		NHIObjectFactory::CreateObject : Create an object.
//----------------------------------------------------------------------------
NHIObject *NHIObjectFactory::CreateObject(HIObjectRef hiObject)
{	NHIObject						*theObject;
	HIFactoryInfoMapConstIterator	theIter;
	NString							classID;
	ControlKind						theKind;
	OSStatus						theErr;



	// Validate our parameters
	NN_ASSERT(hiObject != NULL);



	// Get the state we need
	classID.Set(HIObjectCopyClassID(hiObject));



	// Handle legacy controls
	//
	// Prior to 10.4, some controls were registered under a shared "adapter" class.
	//
	// These controls can be created by InterfaceBuilder, but since they share a
	// common class ID we can not register a factory against them.
	//
	// Our solution is to identify these controls from the control kind, and
	// redirect them to the class IDs used on Mac OS X 10.4 and onwards.
	if (classID.EqualTo(kHIControlAdapterClassID))
		{
		theErr = GetControlKind((ControlRef) hiObject, &theKind);
		if (theErr == noErr && theKind.signature == kControlKindSignatureApple)
			{
			switch (theKind.kind) {
				case kControlKindDataBrowser:
					classID = kHIDataBrowserClassID;
					break;
				
				default:
					NN_LOG("Unknown class: %d", theKind.kind);
					break;
				}
			}
		}



	// Create the object
	for (theIter = GetFactories().begin(); theIter != GetFactories().end(); theIter++)
		{
		if (theIter->first.EqualTo(classID))
			{
			theObject = theIter->second.theFactory();
			if (theObject != NULL)
				theObject->SetHIObject(hiObject);
				
			return(theObject);
			}
		}



	// Create a default if no factory is found
	//
	// The most common HIObject type is an HIView; to allow unknown HIView objects to
	// be wrapped in an NHIView object, we special-case this object type to allow us
	// to create a suitable wrapper even if no factory could be found.
	if (HIObjectIsOfClass(hiObject, kHIViewClassID))
		{
		theObject = new NHIView;
		if (theObject != NULL)
			theObject->SetHIObject(hiObject);
		
		return(theObject);
		}



	// Handle failure
	NN_LOG("Can't create '%s', no factory found", classID.GetUTF8());
	
	return(NULL);
}





//============================================================================
//		NHIObjectFactory::AddFactory : Add a factory.
//----------------------------------------------------------------------------
OSStatus NHIObjectFactory::AddFactory(HIObjectFactory theFactory, const NString &classID, const NString &baseID)
{	static const EventTypeSpec		sEvents[] = { { kEventClassHIObject, kEventHIObjectConstruct  },
												  { kEventClassHIObject, kEventHIObjectInitialize },
												  { kEventClassHIObject, kEventHIObjectDestruct   } };
	static const EventHandlerUPP	sEventHandler = NewEventHandlerUPP(EventHandler);

	HIFactoryInfo			theInfo;
	OSStatus				theErr;



	// Prepare the factory info
	theInfo.baseID     = baseID;
	theInfo.theClass   = NULL;
	theInfo.theFactory = theFactory;



	// Register a sub-class
	//
	// If this factory's class is a sub-class of a system class, we must register
	// with the system so that the appropriate HIObject will be constructed when
	// that class is instantiated.
	//
	// Typically, custom HIViews will be sub-classes of HIView - while wrappers
	// around system controls will just be a wrapper rather than a true sub-class.
	theErr = noErr;
	
	if (baseID.IsNotEmpty())
		{
		theErr = HIObjectRegisterSubclass(classID, baseID, 0, sEventHandler,
										  GetEventTypeCount(sEvents), sEvents,
										  (void *) theFactory, &theInfo.theClass);
		NN_ASSERT_NOERR(theErr);
	
		if (theErr != noErr)
			return(theErr);
		}



	// Add the factory
	GetFactories()[classID] = theInfo;
	
	return(theErr);
}





//============================================================================
//		NHIObjectFactory::RemoveFactories : Remove the factories.
//----------------------------------------------------------------------------
void NHIObjectFactory::RemoveFactories(void)
{	HIFactoryInfoMapConstIterator	theIter;
	HIFactoryInfo					theInfo;
	OSStatus						theErr;



	// Remove the factories
	//
	// If HIObjectUnregisterClass fails, it is typically because an instance
	// of the class still exists.
	//
	// These instances must be destroyed before the class can be unregistered.
	for (theIter = GetFactories().begin(); theIter != GetFactories().end(); theIter++)
		{
		theInfo = theIter->second;
		if (theInfo.theClass != NULL)
			{
			theErr = HIObjectUnregisterClass(theInfo.theClass);
			NN_ASSERT_NOERR(theErr);
			}
		}
	
	GetFactories().clear();
}





//============================================================================
//		NHIObjectFactory::SetNextInstance : Set the next instance.
//----------------------------------------------------------------------------
void NHIObjectFactory::SetNextInstance(NHIObject *theObject)
{


	// Validate our parameters and state
	NN_ASSERT(theObject     != NULL);
	NN_ASSERT(mNextInstance == NULL);



	// Set the instance
	mNextInstance = theObject;
}





//============================================================================
//		NHIObjectFactory::GetFactories : Get the factories.
//----------------------------------------------------------------------------
#pragma mark -
HIFactoryInfoMap &NHIObjectFactory::GetFactories(void)
{	static HIFactoryInfoMap		sFactories;



	// Get the factories
	//
	// The factory list is stored as a private static variable, so that
	// it can be accessed deterministically during static initialisation.
	return(sFactories);
}





//============================================================================
//		NHIObjectFactory::EventHandler : Carbon Event handler.
//----------------------------------------------------------------------------
OSStatus NHIObjectFactory::EventHandler(EventHandlerCallRef callRef, EventRef eventRef, void *userData)
{	NHIObject			*thisPtr = (NHIObject *) userData;
	NCarbonEvent		theEvent(eventRef);
	bool				isConstruct;
	HIObjectFactory		theFactory;
	OSStatus			theErr;



	// Construct the object
	//
	// For the initial kEventHIObjectConstruct event, the userData parameter carries
	// the value passed to HIObjectRegisterSubclass.
	//
	// On input the event's kEventParamHIObjectInstance parameter holds the HIObjectRef,
	// and on output is expected to contain the userData value for future HIObject events.
	//
	//
	// Since we passed the factory callback for the class to HIObjectRegisterSubclass,
	// we can use this to instantiate a C++ object for the object.
	//
	// However, if this HIOBjectRef was created by a C++ object coming into creation,
	// that C++ object can use SetNextInstance to set itself as the instance variable
	// for the HIObjectRef.
	isConstruct = theEvent.IsClass(kEventClassHIObject) && theEvent.IsKind(kEventHIObjectConstruct);
	
	if (isConstruct)
		{
		// Use the existing object
		if (mNextInstance != NULL)
			{
			thisPtr       = mNextInstance;
			mNextInstance = NULL;
			}
		
		
		// Or create a new one
		else
			{
			theFactory = (HIObjectFactory) userData;
			thisPtr    = theFactory();

			if (thisPtr == NULL)
				return(memFullErr);
			}
		}



	// Dispatch the event
	theErr = thisPtr->DispatchEvent(theEvent, callRef);



	// Update the instance
	//
	// Once NHIObject has processed kEventHIObjectConstruct, we must update the event
	// to reflect the value we wish future events to carry in their userData parameter.
	if (theErr == noErr && isConstruct)
		theEvent.SetParameterPointer(kEventParamHIObjectInstance, thisPtr);
	
	return(theErr);
}



