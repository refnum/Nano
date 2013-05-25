/*	NAME:
		NSingleton.cpp

	DESCRIPTION:
		Singleton object.
		
		Provides a base class for singleton objects, and ensures that they
		are destroyed in the reverse order of construction.

		Applications that use singletons should invoke NSingleton::DestroyObjects
		prior to exiting main.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
	// Singletons must hold a valid instance pointer.
	NN_ASSERT(mInstance != NULL);



	// Update our state
	GetObjects()->RemoveValue(this);
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





#pragma mark private
//============================================================================
//		NSingleton::SetInstance : Set the instance pointer.
//----------------------------------------------------------------------------
void NSingleton::SetInstance(void **thePtr)
{


	// Update our state
	mInstance = thePtr;
}





//============================================================================
//		NSingleton::GetObjects : Get the object list.
//----------------------------------------------------------------------------
NSingletonList *NSingleton::GetObjects(void)
{	static NSingletonList		sObjects;



	// Get the objects
	return(&sObjects);
}






