/*	NAME:
		NListener.cpp

	DESCRIPTION:
		Message listener.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NListener.h"





//============================================================================
//		NListener::NListener : Constructor.
//----------------------------------------------------------------------------
NListener::NListener(const NListener &theListener)
{


	// Initialize ourselves
	CloneListener(theListener);
}





//============================================================================
//		NListener::NListener : Constructor.
//----------------------------------------------------------------------------
NListener::NListener(void)
{


	// Initialize ourselves
	mIsListening = true;
}





//============================================================================
//		NListener::~NListener : Destructor.
//----------------------------------------------------------------------------
NListener::~NListener(void)
{


	// Clean up
	RemoveFromBroadcasters();
}





//============================================================================
//		NListener::IsListening : Are we listening to messages?
//----------------------------------------------------------------------------
bool NListener::IsListening(void) const
{


	// Get our state
	return(mIsListening);
}





//============================================================================
//		NListener::SetListening : Set our listening state.
//----------------------------------------------------------------------------
void NListener::SetListening(bool isListening)
{


	// Set our state
	mIsListening = isListening;
}





//============================================================================
//		NListener::IsListeningTo : Are we listening to a broadcaster?
//----------------------------------------------------------------------------
bool NListener::IsListeningTo(const NBroadcaster *theBroadcaster) const
{	bool								isListening;
	NBroadcasterListConstIterator		theIter;



	// Get the state we need
	if (theBroadcaster == kBroadcasterAny)
		isListening = !mBroadcasters.empty();
	else
		{
		theIter     = mBroadcasters.find((NBroadcaster *) theBroadcaster);
		isListening = (theIter != mBroadcasters.end());
		}
	
	return(isListening);
}





//============================================================================
//		NListener::RemoveFromBroadcasters : Remove all broadcasters.
//----------------------------------------------------------------------------
#pragma mark -
void NListener::RemoveFromBroadcasters(void)
{	NBroadcaster			*theBroadcaster;
	NBroadcasterList		theList;



	// Stop listening to our broadcasters
	while (!mBroadcasters.empty())
		{
		theBroadcaster = mBroadcasters.begin()->first;
		theBroadcaster->RemoveListener(this);
		}
}





//=============================================================================
//		NListener::operator = : Assignment operator.
//-----------------------------------------------------------------------------
const NListener &NListener::operator = (const NListener &theListener)
{


	// Assign the object
	if (this != &theListener)
		CloneListener(theListener);

	return(*this);
}





//============================================================================
//		NListener::AddBroadcaster : Add a broadcaster.
//----------------------------------------------------------------------------
#pragma mark -
void NListener::AddBroadcaster(NBroadcaster *theBroadcaster)
{


	// Validate our parameters
	NN_ASSERT(theBroadcaster != NULL);
	NN_ASSERT(!IsListeningTo(theBroadcaster));



	// Add the broadcaster
	mBroadcasters[theBroadcaster] = 1;
}





//============================================================================
//		NListener::RemoveBroadcaster : Remove a broadcaster.
//----------------------------------------------------------------------------
void NListener::RemoveBroadcaster(NBroadcaster *theBroadcaster)
{


	// Validate our parameters
	NN_ASSERT(theBroadcaster != NULL);
	NN_ASSERT(IsListeningTo(theBroadcaster));



	// Remove the broadcaster
	mBroadcasters.erase(theBroadcaster);
}





//=============================================================================
//		NListener::CloneListener : Clone a listener.
//-----------------------------------------------------------------------------
void NListener::CloneListener(const NListener &theListener)
{	NBroadcasterListConstIterator	theIter;



	// Clone the object
	//
	// An explicit clone is necessary to ensure the pointers between listener and
	// broadcaster are correctly established (the default copy constructor would
	// copy the pointers in this object, but wouldn't update the other).
	mIsListening = theListener.mIsListening;
	
	for (theIter = theListener.mBroadcasters.begin(); theIter != theListener.mBroadcasters.end(); theIter++)
		theIter->first->AddListener(this);
}




