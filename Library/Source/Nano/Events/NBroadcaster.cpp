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
#include "NAny.h"
#include "NBroadcast.h"
#include "NExecute.h"
#include "NScopedLock.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		NBroadcaster::Send : Send a broadcast synchronously.
//-----------------------------------------------------------------------------
void NBroadcaster::Send(const NString& theName, const NAny& theValue)
{


	// Validate our paramaeters
	NN_REQUIRE(!theName.IsEmpty());



	// Send the broadcast
	Send({theName, theValue});
}





//=============================================================================
//		NBroadcaster::Send : Send a broadcast synchronously.
//-----------------------------------------------------------------------------
void NBroadcaster::Send(const NBroadcast& theBroadcast)
{


	// Send the broadcast
	Get().SendBroadcast(theBroadcast);
}





//=============================================================================
//		NBroadcaster::SendAsync : Send a broadcast asynchronously.
//-----------------------------------------------------------------------------
void NBroadcaster::SendAsync(const NString& theName, const NAny& theValue)
{


	// Validate our paramaeters
	NN_REQUIRE(!theName.IsEmpty());



	// Send the broadcast
	SendAsync({theName, theValue});
}





//=============================================================================
//		NBroadcaster::SendAsync : Send a broadcast asynchronously.
//-----------------------------------------------------------------------------
void NBroadcaster::SendAsync(const NBroadcast& theBroadcast)
{


	// Send the broadcast
	NExecute(
	[=]()
	{
		Send(theBroadcast);
	});
}





#pragma mark protected
//=============================================================================
//		NBroadcaster::GetBroadcasts : Get the broadcasts for a receiver.
//-----------------------------------------------------------------------------
NVectorString NBroadcaster::GetBroadcasts(const NReceiver* theReceiver)
{


	// Validate our parameters
	NN_REQUIRE(theReceiver != nullptr);



	// Get the broadcasts
	return Get().GetReceiverBroadcasts(theReceiver);
}





//=============================================================================
//		NBroadcaster::StartReceiving : Start receiving a broadcast.
//-----------------------------------------------------------------------------
void NBroadcaster::StartReceiving(const NReceiver*         theReceiver,
								  const NString&           theName,
								  const NFunctionReceiver& theFunction)
{


	// Validate our paramaeters
	NN_REQUIRE(theReceiver != nullptr);
	NN_REQUIRE(!theName.IsEmpty());
	NN_REQUIRE(theFunction != nullptr);



	// Add the receiver
	Get().AddReceiver(theReceiver, theName, theFunction);
}





//=============================================================================
//		NBroadcaster::StopReceiving : Stop receiving a broadcast.
//-----------------------------------------------------------------------------
void NBroadcaster::StopReceiving(const NReceiver* theReceiver, const NString& theName)
{


	// Validate our paramaeters
	NN_REQUIRE(theReceiver != nullptr);
	NN_REQUIRE(!theName.IsEmpty());



	// Remove the receiver
	Get().RemoveReceiver(theReceiver, theName);
}





//=============================================================================
//		NBroadcaster::StopReceiving : Stop receiving broadcasts.
//-----------------------------------------------------------------------------
void NBroadcaster::StopReceiving(const NReceiver* theReceiver)
{


	// Validate our paramaeters
	NN_REQUIRE(theReceiver != nullptr);



	// Remove the receiver
	Get().RemoveReceiver(theReceiver);
}





//=============================================================================
//		NBroadcaster::DestroyedReceiver : A receiver has been detroyed.
//-----------------------------------------------------------------------------
void NBroadcaster::DestroyedReceiver(const NReceiver* theReceiver)
{


	// Validate our paramaeters
	NN_REQUIRE(theReceiver != nullptr);



	// Mark the receiver as destroyed
	Get().DestroyReceiver(theReceiver);
}





#pragma mark private
//=============================================================================
//		NBroadcaster::SendBroadcast : Send a broadcast.
//-----------------------------------------------------------------------------
void NBroadcaster::SendBroadcast(const NBroadcast& theBroadcast)
{


	// Prepare to send
	NScopedLock acquireLock(mLock);

	NN_REQUIRE(mCurrentBroadcast.IsEmpty());
	NN_REQUIRE(mCurrentDestroyed.empty());



	// Send the broadcast
	//
	// Sending a broadcast may cause receivers to be added or removed from
	// the recipient list for the current broadcast.
	//
	// As broadcasts are sent in an undefined order we defer these changes
	// until after a broadcast has concluded.
	//
	// This means that:
	//
	//	o A receiver destroyed during a broadcast will not receive the broadcast
	//	o A receiver added     during a broadcast will not receive the broadcast
	//	o A receiver removed   during a broadcast will     receive the broadcast
	mCurrentBroadcast = theBroadcast.GetName();

	for (const auto& theRecipient : GetBroadcastRecipients(theBroadcast.GetName()))
	{
		if (!nstd::contains(mCurrentDestroyed, theRecipient.theReceiver))
		{
			theRecipient.theFunction(theBroadcast);
		}
	}



	// Reset our state
	mCurrentBroadcast.Clear();
	mCurrentDestroyed.clear();
}





//=============================================================================
//		NBroadcaster::IsReceiving : Is a receiver receiving a broadcast?
//-----------------------------------------------------------------------------
bool NBroadcaster::IsReceiving(const NReceiver* theReceiver, const NString& theName)
{


	// Validate our parameters
	NN_REQUIRE(theReceiver != nullptr);
	NN_REQUIRE(!theName.IsEmpty());



	// Check the receivers
	//
	// If we have the receiver and the receiver has the broadcast then we know
	// that we must have the receiver listed as a recipient of the broadcast.
	auto iterReceiver = mReceivers.find(theReceiver);
	if (iterReceiver != mReceivers.end())
	{
		if (nstd::contains(iterReceiver->second, theName))
		{
			NN_REQUIRE(HasReceiver(GetBroadcastRecipients(theName), theReceiver));
			return true;
		}
	}



	// Validate our state
	//
	// If we don't have the receiver, or the receiver doesn't have the broadcast,
	// we shouldn't find the reciever in the recipient list for the broadcast.
	NN_REQUIRE(!nstd::contains(mBroadcasts, theName) ||
			   !HasReceiver(GetBroadcastRecipients(theName), theReceiver));

	return false;
}





//=============================================================================
//		NBroadcaster::HasReceiver : Does a recipient list contain a receiver?
//-----------------------------------------------------------------------------
bool NBroadcaster::HasReceiver(const NVectorBroadcastRecipients& theRecipients,
							   const NReceiver*                  theReceiver)
{


	// Validate our parameters
	NN_REQUIRE(theReceiver != nullptr);


	// Check the recipients
	for (const auto& theRecipient : theRecipients)
	{
		if (theRecipient.theReceiver == theReceiver)
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NBroadcaster::AddReceiver : Add a receiver.
//-----------------------------------------------------------------------------
void NBroadcaster::AddReceiver(const NReceiver*         theReceiver,
							   const NString&           theName,
							   const NFunctionReceiver& theFunction)
{


	// Validate our parameters
	NN_REQUIRE(!theName.IsEmpty());
	NN_REQUIRE(theFunction != nullptr);



	// Add the receiver
	NScopedLock acquireLock(mLock);

	NN_REQUIRE(!IsReceiving(theReceiver, theName));

	mBroadcasts[theName].push_back({theReceiver, theFunction});
	mReceivers[theReceiver].push_back(theName);
}





//=============================================================================
//		NBroadcaster::RemoveReceiver : Remove a receiver.
//-----------------------------------------------------------------------------
void NBroadcaster::RemoveReceiver(const NReceiver* theReceiver, const NString& theName)
{


	// Validate our parameters
	NN_REQUIRE(!theName.IsEmpty());



	// Remove the receiver
	NScopedLock acquireLock(mLock);

	NN_REQUIRE(IsReceiving(theReceiver, theName));

	nstd::erase_if(mBroadcasts.find(theName).value(),
	[=](const NBroadcastRecipient& theRecipient)
	{
		return theRecipient.theReceiver == theReceiver;
	});

	nstd::erase(mReceivers.find(theReceiver).value(), theName);



	// Clean up
	//
	// The maps can be shrunk when an entry becomes empty.
	auto iterBroadcast = mBroadcasts.find(theName);
	if (iterBroadcast->second.empty())
	{
		mBroadcasts.erase(theName);
	}

	auto iterReceiver = mReceivers.find(theReceiver);
	if (iterReceiver->second.empty())
	{
		mReceivers.erase(iterReceiver);
	}
}





//=============================================================================
//		NBroadcaster::RemoveReceiver : Remove a receiver.
//-----------------------------------------------------------------------------
void NBroadcaster::RemoveReceiver(const NReceiver* theReceiver)
{


	// Validate our parameters
	NN_REQUIRE(theReceiver != nullptr);



	// Remove the receiver
	NScopedLock acquireLock(mLock);

	for (const auto& theName : GetReceiverBroadcasts(theReceiver))
	{
		RemoveReceiver(theReceiver, theName);
	}
}





//=============================================================================
//		NBroadcaster::DestroyReceiver : A receiver has been detroyed.
//-----------------------------------------------------------------------------
void NBroadcaster::DestroyReceiver(const NReceiver* theReceiver)
{


	// Validate our paramaeters
	NN_REQUIRE(theReceiver != nullptr);



	// Mark the receiver as destroyed
	NScopedLock acquireLock(mLock);

	if (!mCurrentBroadcast.IsEmpty())
	{
		mCurrentDestroyed.push_back(theReceiver);
	}

	RemoveReceiver(theReceiver);
}





//=============================================================================
//		NBroadcaster::GetBroadcastRecipients : Get the recipients of a broadcast.
//-----------------------------------------------------------------------------
NVectorBroadcastRecipients NBroadcaster::GetBroadcastRecipients(const NString& theName)
{


	// Validate our parameters and state
	NN_REQUIRE(!theName.IsEmpty());
	NN_REQUIRE(mLock.IsLocked());



	// Get the recipients
	auto iterBroadcast = mBroadcasts.find(theName);
	if (iterBroadcast != mBroadcasts.end())
	{
		return iterBroadcast->second;
	}

	return {};
}





//=============================================================================
//		NBroadcaster::GetReceiverBroadcasts : Get the broadcasts for a receiver.
//-----------------------------------------------------------------------------
NVectorString NBroadcaster::GetReceiverBroadcasts(const NReceiver* theReceiver)
{


	// Validate our parameters
	NN_REQUIRE(theReceiver != nullptr);



	// Get the broadcasts
	NScopedLock acquireLock(mLock);

	auto iterReceiver = mReceivers.find(theReceiver);
	if (iterReceiver != mReceivers.end())
	{
		return iterReceiver->second;
	}

	return {};
}





//=============================================================================
//		NBroadcaster::Get : Get the instance.
//-----------------------------------------------------------------------------
NBroadcaster& NBroadcaster::Get()
{


	// Get the instance
	static NBroadcaster sInstance;

	return sInstance;
}
