/*	NAME:
		NMsgBroadcaster.cpp

	DESCRIPTION:
		Message broadcaster.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <algorithm>

#include "NMsgListener.h"
#include "NMsgBroadcaster.h"





//============================================================================
//		NMsgBroadcaster::NMsgBroadcaster : Constructor.
//----------------------------------------------------------------------------
NMsgBroadcaster::NMsgBroadcaster(const NMsgBroadcaster &theBroadcaster)
{


	// Initialize ourselves
	Clone(theBroadcaster);
}





//============================================================================
//		NMsgBroadcaster::NMsgBroadcaster : Constructor.
//----------------------------------------------------------------------------
NMsgBroadcaster::NMsgBroadcaster(void)
{


	// Initialize ourselves
	mIsBroadcasting = true;
	mIsDead         = NULL;
}





//============================================================================
//		NMsgBroadcaster::~NMsgBroadcaster : Destructor.
//----------------------------------------------------------------------------
NMsgBroadcaster::~NMsgBroadcaster(void)
{


	// Let everyone know
	BroadcastMessage(kMsgBroadcasterDestroyed, this);



	// Clean up
	RemoveListeners();



	// Set our flag
	if (mIsDead != NULL)
		*mIsDead = true;
}





//============================================================================
//		NMsgBroadcaster::IsBroadcasting : Are we broadcasting messages?
//----------------------------------------------------------------------------
bool NMsgBroadcaster::IsBroadcasting(void) const
{


	// Get our state
	return(mIsBroadcasting);
}





//============================================================================
//		NMsgBroadcaster::SetBroadcasting : Set our broadcasting state.
//----------------------------------------------------------------------------
void NMsgBroadcaster::SetBroadcasting(bool isBroadcasting)
{


	// Set our state
	mIsBroadcasting = isBroadcasting;
}





//============================================================================
//		NMsgBroadcaster::HasListeners : Do we have any listeners?
//----------------------------------------------------------------------------
bool NMsgBroadcaster::HasListeners(void) const
{


	// Check our state
	return(!mListeners.empty());
}





//============================================================================
//		NMsgBroadcaster::AddListener : Add a listener.
//----------------------------------------------------------------------------
#pragma mark -
void NMsgBroadcaster::AddListener(NMsgListener *theListener)
{


	// Validate our parameters
	NN_ASSERT(theListener != NULL);
	
	if (NN_DEBUG && theListener->IsListeningTo(this))
		NN_LOG("NMsgBroadcaster already being listened to by %p", theListener);



	// Add the listener
	mListeners[theListener] = 1;
	
	theListener->AddBroadcaster(this);
}





//============================================================================
//		NMsgBroadcaster::RemoveListener : Remove a listener.
//----------------------------------------------------------------------------
void NMsgBroadcaster::RemoveListener(NMsgListener *theListener)
{


	// Validate our parameters
	NN_ASSERT(theListener != NULL);

	if (NN_DEBUG && !theListener->IsListeningTo(this))
		NN_LOG("NMsgBroadcaster wasn't being listened to by %p", theListener);



	// Remove the listener
	mListeners.erase(theListener);
	
	theListener->RemoveBroadcaster(this);
}





//============================================================================
//		NMsgBroadcaster::RemoveListeners : Remove all listeners.
//----------------------------------------------------------------------------
void NMsgBroadcaster::RemoveListeners(void)
{	NMsgListener		*theListener;
	NMsgListenerList	theList;



	// Stop broadcasting to our listeners
	while (!mListeners.empty())
		{
		theListener = mListeners.begin()->first;
		RemoveListener(theListener);
		}
}





//============================================================================
//		NMsgBroadcaster::BroadcastMessage : Broadcast a message.
//----------------------------------------------------------------------------
void NMsgBroadcaster::BroadcastMessage(BroadcastMsg theMsg, void *msgData)
{	NMsgListener						*theListener;
	bool								*oldFlag;
	NMsgListenerList					theList;
	NMsgListenerListConstIterator		theIter;
	bool								isDead;



	// Check our state
	if (!mIsBroadcasting)
		return;



	// Set our flag
	//
	// To allow broadcasting an event to kill the NMsgBroadcaster who is doing the
	// broadcsting, we need to watch for our destructor being called while sending.
	// We do this with a member that points to data on our stack.
	//
	// If we see the flag being set, we know the message has triggered our own death.
	//
	// To make ourselves re-entrant, we must save the previous flag before doing
	// the dispatch then restore it (subject to us still being alive) after.
	isDead = false;

	oldFlag = mIsDead;
	mIsDead = &isDead;



	// Broadcast the message
	//
	// Since broadcasting a message may cause listeners to remove themselves
	// from our list, we need to take a copy and iterate through that.
	theList = mListeners;
	
	for (theIter = theList.begin(); theIter != theList.end(); theIter++)
		{
		// Send the message
		theListener = theIter->first;

		if (theListener->IsListening())
			theListener->DoMessage(theMsg, msgData);



		// Stop if we die
		//
		// If broadcasting the message killed the broadcaster, we're done
		if (isDead)
			break;
		}



	// Reset our state
	//
	// Note that we only reset if we were not destroyed. If we were destroyed,
	// we can not touch any member variables once we see the flag is set.
	if (!isDead)
		mIsDead = oldFlag;
}





//=============================================================================
//		NMsgBroadcaster::operator = : Assignment operator.
//-----------------------------------------------------------------------------
const NMsgBroadcaster &NMsgBroadcaster::operator = (const NMsgBroadcaster &theBroadcaster)
{


	// Assign the object
	Clone(theBroadcaster);

	return(*this);
}





//=============================================================================
//		NMsgBroadcaster::Clone : Clone an object.
//-----------------------------------------------------------------------------
#pragma mark -
void NMsgBroadcaster::Clone(const NMsgBroadcaster &theBroadcaster)
{	NMsgListenerListConstIterator	theIter;



	// Clone the object
	//
	// An explicit clone is necessary to ensure the pointers between listener and
	// broadcaster are correctly established (the default copy constructor would
	// copy the pointers in this object, but wouldn't update the other).
	mIsBroadcasting = theBroadcaster.mIsBroadcasting;
	mIsDead         = NULL;
	
	for (theIter = theBroadcaster.mListeners.begin(); theIter != theBroadcaster.mListeners.end(); theIter++)
		AddListener(theIter->first);
}




