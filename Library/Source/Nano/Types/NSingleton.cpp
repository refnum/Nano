/*	NAME:
		NSingleton.cpp

	DESCRIPTION:
		Singleton object.
		
		Provides a base class for singleton objects, and ensures that they
		are destroyed in the reverse order of construction.

		Applications that use singletons should invoke NSingleton::DestroyObjects
		prior to exiting main.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSingleton.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NMutexLock NSingleton::mLock;





//============================================================================
//		NSingleton::NSingleton : Constructor.
//----------------------------------------------------------------------------
NSingleton::NSingleton(void)
{


	// Initialise ourselves
	mInstance = NULL;
	


	// Register the object
	GetObjects()->PushBack(this);
}





//============================================================================
//		NSingleton::~NSingleton : Destructor.
//----------------------------------------------------------------------------
NSingleton::~NSingleton(void)
{


	// Validate our state
	//
	// Singletons must be stored in an instance pointer, which should point to us.
	NN_ASSERT( mInstance != NULL);
	NN_ASSERT(*mInstance == this);



	// Validate the list state
	//
	// Singletons should only be destroyed via DestroyObjects, so by the time
	// our destructor is reached we should have been removed from the list.
	NN_ASSERT(!IsKnownObject(this));



	// Update the instance pointer
	*mInstance = NULL;
}





//============================================================================
//		NSingleton::DestroyObjects : Destroy the singletons.
//----------------------------------------------------------------------------
void NSingleton::DestroyObjects(void)
{	NSingleton	*theObject;



	// Destroy the singletons
	while (GetObjects()->PopFront(theObject))
		delete theObject;
}





//============================================================================
//		NSingleton::SetInstance : Set the instance pointer.
//----------------------------------------------------------------------------
#pragma mark -
void NSingleton::SetInstance(void **thePtr)
{


	// Update our state
	mInstance = thePtr;
}





//============================================================================
//		NSingleton::IsKnownObject : Is this a known object?
//----------------------------------------------------------------------------
bool NSingleton::IsKnownObject(NSingleton *theObject)
{	UInt32				n, numItems;
	NSingletonList		*theList;
	bool				isKnown;



	// Lock the list
	theList = GetObjects();
	theList->Lock();



	// Examine the contents
	numItems = theList->GetSize();
	isKnown  = false;
	
	for (n = 0; n < numItems && !isKnown; n++)
		isKnown = (theList->GetValue(n) == theObject);



	// Clean up
	theList->Unlock();
	
	return(isKnown);
}





//============================================================================
//		NSingleton::GetObjects : Get the object list.
//----------------------------------------------------------------------------
NSingletonList *NSingleton::GetObjects(void)
{	static NSingletonList		sObjects;



	// Get the objects
	return(&sObjects);
}






