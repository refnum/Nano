/*	NAME:
		NMsgListener.cpp

	DESCRIPTION:
		Message listener.
	
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
	RemoveListener();
}





//============================================================================
//		NMsgListener::IsListening : Are we listening to messages?
//----------------------------------------------------------------------------
bool NMsgListener::IsListening(void)
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
bool NMsgListener::IsListeningTo(NMsgBroadcaster *theBroadcaster)
{	NMsgBroadcasterListConstIterator		theIter;



	// Find the broadcaster
	theIter = find(mBroadcasters.begin(), mBroadcasters.end(), theBroadcaster);
	
	return(theIter != mBroadcasters.end());
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
	mBroadcasters.push_back(theBroadcaster);
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
	mBroadcasters.erase(std::remove(mBroadcasters.begin(), mBroadcasters.end(), theBroadcaster), mBroadcasters.end());
}





//============================================================================
//		NMsgListener::RemoveListener : Remove ourselves from our broadcasters.
//----------------------------------------------------------------------------
#pragma mark -
void NMsgListener::RemoveListener(void)
{	NMsgBroadcaster			*theBroadcaster;
	NMsgBroadcasterList		theList;



	// Remove ourselves from our broadcasters
	while (!mBroadcasters.empty())
		{
		theBroadcaster = mBroadcasters.front();
		theBroadcaster->RemoveListener(this);
		}
}





