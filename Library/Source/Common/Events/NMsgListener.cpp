/*	NAME:
		NMsgListener.cpp

	DESCRIPTION:
		Message listener.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgListener.h"





//============================================================================
//		NMsgListener::NMsgListener : Constructor.
//----------------------------------------------------------------------------
NMsgListener::NMsgListener(const NMsgListener &theListener)
{


	// Initialize ourselves
	Clone(theListener);
}





//============================================================================
//		NMsgListener::NMsgListener : Constructor.
//----------------------------------------------------------------------------
NMsgListener::NMsgListener(void)
{


	// Initialize ourselves
	mIsListening = true;
}





//============================================================================
//		NMsgListener::~NMsgListener : Destructor.
//----------------------------------------------------------------------------
NMsgListener::~NMsgListener(void)
{


	// Clean up
	RemoveFromBroadcasters();
}





//============================================================================
//		NMsgListener::IsListening : Are we listening to messages?
//----------------------------------------------------------------------------
bool NMsgListener::IsListening(void) const
{


	// Get our state
	return(mIsListening);
}





//============================================================================
//		NMsgListener::SetListening : Set our listening state.
//----------------------------------------------------------------------------
void NMsgListener::SetListening(bool isListening)
{


	// Set our state
	mIsListening = isListening;
}





//============================================================================
//		NMsgListener::IsListeningTo : Are we listening to a broadcaster?
//----------------------------------------------------------------------------
bool NMsgListener::IsListeningTo(const NMsgBroadcaster *theBroadcaster) const
{	bool									isListening;
	NMsgBroadcasterListConstIterator		theIter;



	// Get the state we need
	if (theBroadcaster == kBroadcasterAny)
		isListening = !mBroadcasters.empty();
	else
		{
		theIter     = mBroadcasters.find((NMsgBroadcaster *) theBroadcaster);
		isListening = (theIter != mBroadcasters.end());
		}
	
	return(isListening);
}





//============================================================================
//		NMsgListener::RemoveFromBroadcasters : Remove all broadcasters.
//----------------------------------------------------------------------------
#pragma mark -
void NMsgListener::RemoveFromBroadcasters(void)
{	NMsgBroadcaster			*theBroadcaster;
	NMsgBroadcasterList		theList;



	// Stop listening to our broadcasters
	while (!mBroadcasters.empty())
		{
		theBroadcaster = mBroadcasters.begin()->first;
		theBroadcaster->RemoveListener(this);
		}
}





//=============================================================================
//		NMsgListener::operator = : Assignment operator.
//-----------------------------------------------------------------------------
const NMsgListener &NMsgListener::operator = (const NMsgListener &theListener)
{


	// Assign the object
	if (this != &theListener)
		Clone(theListener);

	return(*this);
}





//============================================================================
//		NMsgListener::AddBroadcaster : Add a broadcaster.
//----------------------------------------------------------------------------
#pragma mark -
void NMsgListener::AddBroadcaster(NMsgBroadcaster *theBroadcaster)
{


	// Validate our parameters
	NN_ASSERT(theBroadcaster != NULL);
	NN_ASSERT(!IsListeningTo(theBroadcaster));



	// Add the broadcaster
	mBroadcasters[theBroadcaster] = 1;
}





//============================================================================
//		NMsgListener::RemoveBroadcaster : Remove a broadcaster.
//----------------------------------------------------------------------------
void NMsgListener::RemoveBroadcaster(NMsgBroadcaster *theBroadcaster)
{


	// Validate our parameters
	NN_ASSERT(theBroadcaster != NULL);
	NN_ASSERT(IsListeningTo(theBroadcaster));



	// Remove the broadcaster
	mBroadcasters.erase(theBroadcaster);
}





//=============================================================================
//		NMsgListener::Clone : Clone an object.
//-----------------------------------------------------------------------------
void NMsgListener::Clone(const NMsgListener &theListener)
{	NMsgBroadcasterListConstIterator	theIter;



	// Clone the object
	//
	// An explicit clone is necessary to ensure the pointers between listener and
	// broadcaster are correctly established (the default copy constructor would
	// copy the pointers in this object, but wouldn't update the other).
	mIsListening = theListener.mIsListening;
	
	for (theIter = theListener.mBroadcasters.begin(); theIter != theListener.mBroadcasters.end(); theIter++)
		theIter->first->AddListener(this);
}




