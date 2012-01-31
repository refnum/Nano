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
}





//============================================================================
//		NBroadcaster::~NBroadcaster : Destructor.
//----------------------------------------------------------------------------
NBroadcaster::~NBroadcaster(void)
{	NBroadcastStateListIterator		theIter;



	// Let everyone know
	BroadcastMessage(kMsgNBroadcasterDestroyed, this);



	// Clean up
	RemoveListeners();



	// Update our state
	for (theIter = mBroadcasts.begin(); theIter != mBroadcasts.end(); theIter++)
		(*theIter)->isDead = true;
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
{	NBroadcastStateListIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theListener != NULL);

	if (NN_DEBUG && !theListener->IsListeningTo(this))
		NN_LOG("NBroadcaster wasn't being listened to by %p", theListener);



	// Remove the listener
	mListeners.erase(theListener);

	theListener->RemoveBroadcaster(this);



	// Update our state
	for (theIter = mBroadcasts.begin(); theIter != mBroadcasts.end(); theIter++)
		(*theIter)->theRecipients.erase(theListener);
}





//============================================================================
//		NBroadcaster::RemoveListeners : Remove all listeners.
//----------------------------------------------------------------------------
void NBroadcaster::RemoveListeners(void)
{	NListener		*theListener;
	NListenerMap	theList;



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
void NBroadcaster::BroadcastMessage(NBroadcastMsg theMsg, const void *msgData)
{	NListener						*theListener;
	NBroadcastState					theState;
	NListenerMapIterator			theIter;



	// Check our state
	if (!mIsBroadcasting)
		return;



	// Update our state
	//
	// Since a broadcast invokes a function, it can trigger arbitrary events
	// including:
	//
	//		- Destruction of the broadcaster
	//		- Destruction of a listener
	//		- Removal     of a listener
	//		- Broadcast of additional messages (by this object)
	//
	// To allow us to perform a broadcast without interference, we maintain a
	// list of the state for the broadcasts being performed by this object.
	//
	// Each broadcast requires a flag to indicate if the broadcaster has been
	// destroyed, and a list of recipients for the broadcast (the listeners
	// that were present when the broadcast was requested).
	//
	//
	// This state is stored on our stack, and a pointer to it placed in the
	// list of broadcasts being performed by this object.
	//
	// This list is updated if the broadcaster itself is destroyed, or if a
	// listener is removed during the broadcast.
	theState.isDead        = false;
	theState.theRecipients = mListeners;
	
	mBroadcasts.push_back(&theState);



	// Broadcast the message
	//
	// Since recipients might be removed or destroyed while we're broadcasting,
	// we need to prune the list as we go rather than iterate through it.
	//
	// If the broadcaster itself is destroyed, our stack-based state will have
	// its flag set letting us know we need to stop the broadcast.
	
	while (!theState.isDead && !theState.theRecipients.empty())
		{
		// Get the state we need
		theIter     = theState.theRecipients.begin();
		theListener = theIter->first;
		
		theState.theRecipients.erase(theIter);


		// Send the message
		if (theListener->IsListening())
			theListener->DoMessage(theMsg, msgData);
		}



	// Update our state
	if (!theState.isDead)
		erase(mBroadcasts, &theState);
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
{	NListenerMapConstIterator	theIter;



	// Validate our state
	NN_ASSERT(mBroadcasts.empty());



	// Clone the broadcaster
	//
	// An explicit clone is necessary to ensure the pointers between listener and
	// broadcaster are correctly established (the default copy constructor would
	// copy the pointers in this object, but wouldn't update the other).
	//
	// Cloning a broadcaster that's currently broadcasting will clone the listeners
	// so that future broadcasts go to the same place - however we don't clone the
	// list of active broadcasts, since we're not ourselves broadcasting.
	mIsBroadcasting = theBroadcaster.mIsBroadcasting;
	mBroadcasts.clear();

	for (theIter = theBroadcaster.mListeners.begin(); theIter != theBroadcaster.mListeners.end(); theIter++)
		AddListener(theIter->first);
}




