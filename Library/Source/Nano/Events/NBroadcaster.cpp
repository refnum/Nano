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
//		NBroadcaster::Send : Send a message synchronously.
//-----------------------------------------------------------------------------
void NBroadcaster::Send(const NString& theMessage, const NAny& theValue)
{


	// Validate our paramaeters
	NN_REQUIRE(!theMessage.IsEmpty());



	// Send the message
	Get().SendBroadcast(theMessage, theValue);
}





//=============================================================================
//		NBroadcaster::SendAsync : Send a message asynchronously.
//-----------------------------------------------------------------------------
void NBroadcaster::SendAsync(const NString& theMessage, const NAny& theValue)
{


	// Validate our paramaeters
	NN_REQUIRE(!theMessage.IsEmpty());



	// Send the message
	NExecute(
		[=]()
		{
			Send(theMessage, theValue);
		});
}





#pragma mark protect
//=============================================================================
//		NBroadcaster::StartReceiving : Start receiving a message.
//-----------------------------------------------------------------------------
void NBroadcaster::StartReceiving(const NReceiver*         theReceiver,
								  const NString&           theMessage,
								  const NFunctionReceiver& theFunction)
{


	// Validate our paramaeters
	NN_REQUIRE(theReceiver != nullptr);
	NN_REQUIRE(!theMessage.IsEmpty());
	NN_REQUIRE(theFunction != nullptr);



	// Add the receiver
	Get().AddReceiver(theReceiver, theMessage, theFunction);
}





//=============================================================================
//		NBroadcaster::StopReceiving : Stop receiving to a message.
//-----------------------------------------------------------------------------
void NBroadcaster::StopReceiving(const NReceiver* theReceiver, const NString& theMessage)
{


	// Validate our paramaeters
	NN_REQUIRE(theReceiver != nullptr);
	NN_REQUIRE(!theMessage.IsEmpty());



	// Remove the receiver
	Get().RemoveReceiver(theReceiver, theMessage);
}





//=============================================================================
//		NBroadcaster::StopReceiving : Stop receiving messages.
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
void NBroadcaster::SendBroadcast(const NString& theMessage, const NAny& theValue)
{


	// Validate our parameters
	NN_REQUIRE(!theMessage.IsEmpty());



	// Prepare to send
	NScopedLock acquireLock(mLock);

	NN_REQUIRE(mCurrentMessage.IsEmpty());
	NN_REQUIRE(mCurrentDestroyed.empty());



	// Send the broadcast
	//
	// Sending a broadcast may cause receivers to be added or removed from
	// the recipient list for the current message.
	//
	// As broadcasts are sent in an undefined order we defer these changes
	// until after a broadcast has concluded.
	//
	// This means that:
	//
	//	o A receiver destroyed during a broadcast will not receive the message
	//	o A receiver added     during a broadcast will not receive the message
	//	o A receiver removed   during a broadcast will     receive the message
	mCurrentMessage = theMessage;

	for (const auto& theRecipient : GetRecipients(theMessage))
	{
		if (!nstd::contains(mCurrentDestroyed, theRecipient.theReceiver))
		{
			theRecipient.theFunction({theMessage, theValue});
		}
	}



	// Reset our state
	mCurrentMessage.Clear();
	mCurrentDestroyed.clear();
}





//=============================================================================
//		NBroadcaster::IsReceiving : Is a receiver receiving a message?
//-----------------------------------------------------------------------------
bool NBroadcaster::IsReceiving(const NReceiver* theReceiver, const NString& theMessage)
{


	// Validate our parameters
	NN_REQUIRE(theReceiver != nullptr);
	NN_REQUIRE(!theMessage.IsEmpty());



	// Check the receivers
	//
	// If we have the receiver and the receiver has the message then we know
	// that we must have the receiver listed as a recipient of the message.
	auto iterReceiver = mReceivers.find(theReceiver);
	if (iterReceiver != mReceivers.end())
	{
		if (nstd::contains(iterReceiver->second, theMessage))
		{
			NN_REQUIRE(HasReceiver(GetRecipients(theMessage), theReceiver));
			return true;
		}
	}



	// Validate our state
	//
	// If we don't have the receiver, or the receiver doesn't have the message,
	// we shouldn't find the reciever in the recipient list for the message.
	NN_REQUIRE(!nstd::contains(mMessages, theMessage) ||
			   !HasReceiver(GetRecipients(theMessage), theReceiver));

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
							   const NString&           theMessage,
							   const NFunctionReceiver& theFunction)
{


	// Validate our parameters
	NN_REQUIRE(!IsReceiving(theReceiver, theMessage));
	NN_REQUIRE(!theMessage.IsEmpty());
	NN_REQUIRE(theFunction != nullptr);



	// Add the receiver
	NScopedLock acquireLock(mLock);

	mMessages[theMessage].push_back({theReceiver, theFunction});
	mReceivers[theReceiver].push_back(theMessage);
}





//=============================================================================
//		NBroadcaster::RemoveReceiver : Remove a receiver.
//-----------------------------------------------------------------------------
void NBroadcaster::RemoveReceiver(const NReceiver* theReceiver, const NString& theMessage)
{


	// Validate our parameters
	NN_REQUIRE(IsReceiving(theReceiver, theMessage));
	NN_REQUIRE(!theMessage.IsEmpty());



	// Remove the receiver
	NScopedLock acquireLock(mLock);

	nstd::erase_if(mMessages.find(theMessage).value(),
				   [=](const NBroadcastRecipient& theRecipient)
				   {
					   return theRecipient.theReceiver == theReceiver;
				   });

	nstd::erase(mReceivers.find(theReceiver).value(), theMessage);



	// Clean up
	//
	// The maps can be shrunk when an entry becomes empty.
	auto iterMessage = mMessages.find(theMessage);
	if (iterMessage->second.empty())
	{
		mMessages.erase(theMessage);
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

	for (const auto& theMessage : GetMessages(theReceiver))
	{
		RemoveReceiver(theReceiver, theMessage);
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

	if (!mCurrentMessage.IsEmpty())
	{
		mCurrentDestroyed.push_back(theReceiver);
	}

	RemoveReceiver(theReceiver);
}





//=============================================================================
//		NBroadcaster::GetRecipients : Get the recipients of a message.
//-----------------------------------------------------------------------------
NVectorBroadcastRecipients NBroadcaster::GetRecipients(const NString& theMessage)
{


	// Validate our parameters
	NN_REQUIRE(!theMessage.IsEmpty());



	// Get the recipients
	auto iterMessage = mMessages.find(theMessage);
	if (iterMessage != mMessages.end())
	{
		return iterMessage->second;
	}

	return {};
}





//=============================================================================
//		NBroadcaster::GetMessages : Get the messages for a receiver.
//-----------------------------------------------------------------------------
NVectorString NBroadcaster::GetMessages(const NReceiver* theReceiver)
{


	// Validate our parameters
	NN_REQUIRE(theReceiver != nullptr);



	// Get the messages
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
