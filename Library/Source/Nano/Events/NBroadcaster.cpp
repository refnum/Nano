/*	NAME:
		NBroadcaster.cpp

	DESCRIPTION:
		Message broadcaster.
	
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
#include "NBroadcaster.h"





//============================================================================
//		NBroadcaster::NBroadcaster : Constructor.
//----------------------------------------------------------------------------
NBroadcaster::NBroadcaster(const NBroadcaster &theBroadcaster)
{


	// Initialize ourselves
	CloneBroadcaster(theBroadcaster);
}





//============================================================================
//		NBroadcaster::NBroadcaster : Constructor.
//----------------------------------------------------------------------------
NBroadcaster::NBroadcaster(void)
{


	// Initialize ourselves
	mIsBroadcasting = true;
	mIsDead         = NULL;
}





//============================================================================
//		NBroadcaster::~NBroadcaster : Destructor.
//----------------------------------------------------------------------------
NBroadcaster::~NBroadcaster(void)
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
//		NBroadcaster::IsBroadcasting : Are we broadcasting messages?
//----------------------------------------------------------------------------
bool NBroadcaster::IsBroadcasting(void) const
{


	// Get our state
	return(mIsBroadcasting);
}





//============================================================================
//		NBroadcaster::SetBroadcasting : Set our broadcasting state.
//----------------------------------------------------------------------------
void NBroadcaster::SetBroadcasting(bool isBroadcasting)
{


	// Set our state
	mIsBroadcasting = isBroadcasting;
}





//============================================================================
//		NBroadcaster::HasListeners : Do we have any listeners?
//----------------------------------------------------------------------------
bool NBroadcaster::HasListeners(void) const
{


	// Check our state
	return(!mListeners.empty());
}





//============================================================================
//		NBroadcaster::AddListener : Add a listener.
//----------------------------------------------------------------------------
#pragma mark -
void NBroadcaster::AddListener(NListener *theListener)
{


	// Validate our parameters
	NN_ASSERT(theListener != NULL);
	
	if (NN_DEBUG && theListener->IsListeningTo(this))
		NN_LOG("NBroadcaster already being listened to by %p", theListener);



	// Add the listener
	mListeners[theListener] = 1;
	
	theListener->AddBroadcaster(this);
}





//============================================================================
//		NBroadcaster::RemoveListener : Remove a listener.
//----------------------------------------------------------------------------
void NBroadcaster::RemoveListener(NListener *theListener)
{


	// Validate our parameters
	NN_ASSERT(theListener != NULL);

	if (NN_DEBUG && !theListener->IsListeningTo(this))
		NN_LOG("NBroadcaster wasn't being listened to by %p", theListener);



	// Remove the listener
	mListeners.erase(theListener);
	
	theListener->RemoveBroadcaster(this);
}





//============================================================================
//		NBroadcaster::RemoveListeners : Remove all listeners.
//----------------------------------------------------------------------------
void NBroadcaster::RemoveListeners(void)
{	NListener		*theListener;
	NListenerList	theList;



	// Stop broadcasting to our listeners
	while (!mListeners.empty())
		{
		theListener = mListeners.begin()->first;
		RemoveListener(theListener);
		}
}





//============================================================================
//		NBroadcaster::BroadcastMessage : Broadcast a message.
//----------------------------------------------------------------------------
void NBroadcaster::BroadcastMessage(BroadcastMsg theMsg, const void *msgData)
{	NListener						*theListener;
	bool							*oldFlag;
	NListenerList					theList;
	NListenerListConstIterator		theIter;
	bool							isDead;



	// Check our state
	if (!mIsBroadcasting)
		return;



	// Set our flag
	//
	// To allow broadcasting a message to trigger the destruction of the NBroadcaster
	// that is broadcsting the message, we need to watch for our destructor being called
	// while we are broadcasting.
	//
	// We do this with a member that points to data on our stack. If we see the flag
	// being set, we know the message has triggered our destruction.
	//
	// To make ourselves re-entrant, we must save the previous flag before sending the
	// message then restore it (subject to us still being alive) after.
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
//		NBroadcaster::operator = : Assignment operator.
//-----------------------------------------------------------------------------
const NBroadcaster &NBroadcaster::operator = (const NBroadcaster &theBroadcaster)
{


	// Assign the object
	if (this != &theBroadcaster)
		CloneBroadcaster(theBroadcaster);

	return(*this);
}





//=============================================================================
//		NBroadcaster::CloneBroadcaster : Clone a broadcaster.
//-----------------------------------------------------------------------------
#pragma mark -
void NBroadcaster::CloneBroadcaster(const NBroadcaster &theBroadcaster)
{	NListenerListConstIterator	theIter;



	// Clone the broadcaster
	//
	// An explicit clone is necessary to ensure the pointers between listener and
	// broadcaster are correctly established (the default copy constructor would
	// copy the pointers in this object, but wouldn't update the other).
	mIsBroadcasting = theBroadcaster.mIsBroadcasting;
	mIsDead         = NULL;
	
	for (theIter = theBroadcaster.mListeners.begin(); theIter != theBroadcaster.mListeners.end(); theIter++)
		AddListener(theIter->first);
}




