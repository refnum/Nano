/*	NAME:
		NBroadcaster.cpp

	DESCRIPTION:
		Message broadcaster.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NBroadcaster.h"

// Nano
#include "NListener.h"
#include "NScopedLock.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		NBroadcaster::~NBroadcaster : Destructor.
//-----------------------------------------------------------------------------
NBroadcaster::~NBroadcaster()
{


	// Clean up
	RemoveListeners();
}





//=============================================================================
//		NBroadcaster::Broadcast : Broadcast a message.
//-----------------------------------------------------------------------------
void NBroadcaster::Broadcast(const NString& theMsg)
{


	// Validate our parameters
	NN_REQUIRE(!theMsg.IsEmpty());


	// Broadcast the message
	NScopedLock acquireLock(mLock);

	auto theRecipients = mRecipients.find(theMsg);
	if (theRecipients != mRecipients.end())
	{
		for (const auto& theRecipient : theRecipients->second)
		{
			theRecipient.second(theMsg);
		}
	}
}





#pragma mark protected
//=============================================================================
//		NBroadcaster::AddListener : Add a listener.
//-----------------------------------------------------------------------------
void NBroadcaster::AddListener(NListener*               theListener,
							   const NString&           theMsg,
							   const NFunctionListenID& theFunctor)
{


	// Validate our parameters
	NN_REQUIRE(theListener != nullptr);
	NN_REQUIRE(!theMsg.IsEmpty());
	NN_REQUIRE(theFunctor);



	// Add the listener
	NScopedLock acquireLock(mLock);

	auto& theRecipients = mRecipients[theMsg];

	NN_REQUIRE(!nstd::contains(theRecipients, theListener));
	theRecipients[theListener] = theFunctor;
}





//=============================================================================
//		NBroadcaster::RemoveListener : Remove a listener.
//-----------------------------------------------------------------------------
void NBroadcaster::RemoveListener(NListener* theListener, const NString& theMsg)
{


	// Validate our parameters
	NN_REQUIRE(theListener != nullptr);



	// Remove the listener
	NScopedLock acquireLock(mLock);

	if (theMsg.IsEmpty())
	{
		// Remove for all events
		for (auto theIter = mRecipients.begin(); theIter != mRecipients.end();)
		{
			theIter->second.erase(theListener);
			if (theIter->second.empty())
			{
				theIter = mRecipients.erase(theIter);
			}
			else
			{
				theIter++;
			}
		}
	}

	else
	{
		// Remove for the specified event
		const auto& theIter = mRecipients.find(theMsg);
		NN_REQUIRE(theIter != mRecipients.end());

		theIter->second.erase(theListener);
		if (theIter->second.empty())
		{
			mRecipients.erase(theIter);
		}
	}
}





#pragma mark private
//=============================================================================
//		NBroadcaster::RemoveListeners : Remove all of our listeners.
//-----------------------------------------------------------------------------
void NBroadcaster::RemoveListeners()
{


	// Remove the listeners
	NScopedLock acquireLock(mLock);

	for (const auto& theIter : mRecipients)
	{
		for (auto& theRecipient : theIter.second)
		{
			theRecipient.first->StopListening(this, theIter.first);
		}
	}

	mRecipients.clear();
}
