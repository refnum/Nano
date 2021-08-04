/*	NAME:
		NListener.cpp

	DESCRIPTION:
		Message listener.

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
#include "NListener.h"

// Nano
#include "NScopedLock.h"
#include "NStdAlgorithm.h"
#include "NString.h"


// System
#include <unordered_set>





//=============================================================================
//		NListener::~NListener : Destructor.
//-----------------------------------------------------------------------------
NListener::~NListener()
{


	// Clean up
	StopListening();
}





//=============================================================================
//		NListener::StartListening : Start listening for a message.
//-----------------------------------------------------------------------------
void NListener::StartListening(NBroadcaster*            theBroadcaster,
							   const NString&           theMsg,
							   const NFunctionListenID& theFunction)
{


	// Start listening for the message
	NScopedLock acquireLock(mLock);

	theBroadcaster->AddListener(this, theMsg, theFunction);
	mBroadcasters.push_back(theBroadcaster);
}





//=============================================================================
//		NListener::StartListening : Start listening for a message.
//-----------------------------------------------------------------------------
void NListener::StartListening(NBroadcaster*    theBroadcaster,
							   const NString&   theMsg,
							   const NFunction& theFunction)
{


	// Start listening for the message
	StartListening(theBroadcaster,
				   theMsg,
				   [=](const NString&)
				   {
					   theFunction();
				   });
}





//=============================================================================
//		NListener::StopListening : Stop listening for a message.
//-----------------------------------------------------------------------------
void NListener::StopListening(NBroadcaster* theBroadcaster, const NString& theMsg)
{


	// Stop listening for the message
	NScopedLock acquireLock(mLock);

	theBroadcaster->RemoveListener(this, theMsg);



	// Remove one broadcaster from our list
	//
	// We may be listening to the same broadcaster multiple times by listening
	// for different messages.
	//
	// However, as each Start / Stop must be balanced, we know we can never be
	// asked to stop listening to someone we're not listening to.
	NN_REQUIRE(nstd::contains(mBroadcasters, theBroadcaster));

	nstd::erase(mBroadcasters, theBroadcaster);
}





#pragma mark private
//=============================================================================
//		NListener::StopListening : Stop listening to all messages.
//-----------------------------------------------------------------------------
void NListener::StopListening()
{


	// Stop listening to messages
	//
	// We may listen to a broadcaster multiple items, however when we remove
	// ourselves due to destruction we only need to inform them once.
	NScopedLock acquireLock(mLock);

	NVectorBroadcaster theBroadcasters(mBroadcasters);
	nstd::uniquify(theBroadcasters);

	for (auto& theBroadcaster : theBroadcasters)
	{
		theBroadcaster->RemoveListener(this, "");
	}
}
