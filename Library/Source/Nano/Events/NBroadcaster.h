/*	NAME:
		NBroadcaster.h

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
#ifndef NBROADCASTER_H
#define NBROADCASTER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NAny.h"
#include "NMap.h"
#include "NMutex.h"
#include "NReceiver.h"
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Recipients
struct NBroadcastRecipient
{
	const NReceiver*  theReceiver;
	NFunctionReceiver theFunction;
};

using NVectorBroadcastRecipients = std::vector<NBroadcastRecipient>;
using NVectorBroadcastReceivers  = std::vector<const NReceiver*>;

using NBroadcastMessageRecipients = NMap<NString, NVectorBroadcastRecipients>;
using NBroadcastReceiverMessages  = NMap<const NReceiver*, NVectorString>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NBroadcaster
{
	friend class                        NReceiver;

public:
	// Broadcast a message synchronously.
	//
	// The message will be delivered on the calling thread.
	static void                         Send(const NString& theMessage, const NAny& theValue = {});


	// Broadcast a message asynchronously
	//
	// The message will be delivered on a background thread.
	static void                         SendAsync(const NString& theMessage, const NAny& theValue = {});


protected:
	// Start receiving a message
	static void                         StartReceiving(const NReceiver*         theReceiver,
													   const NString&           theMessage,
													   const NFunctionReceiver& theFunction);


	// Stop receiving a message
	static void                         StopReceiving(const NReceiver* theReceiver, const NString& theMessage);


	// Stop receiving any messages
	static void                         StopReceiving(const NReceiver* theReceiver);


	// A receiver has been destroyed
	static void                         DestroyedReceiver(const NReceiver* theReceiver);


private:
	void                                SendBroadcast(const NString& theMessage, const NAny& theValue);

	bool                                IsReceiving(const NReceiver* theReceiver, const NString& theMessage);

	bool                                HasReceiver(const NVectorBroadcastRecipients& theRecipients, const NReceiver* theReceiver);

	void                                AddReceiver(const NReceiver*         theReceiver,
													const NString&           theMessage,
													const NFunctionReceiver& theFunction);

	void                                RemoveReceiver(const NReceiver* theReceiver, const NString& theMessage);
	void                                RemoveReceiver(const NReceiver* theReceiver);

	void                                DestroyReceiver(const NReceiver* theReceiver);

	NVectorBroadcastRecipients          GetRecipients(const NString& theMessage);
	NVectorString                       GetMessages(  const NReceiver* theReceiver);

	static NBroadcaster&                Get();



private:
	NMutex                              mLock;
	NBroadcastMessageRecipients         mMessages;
	NBroadcastReceiverMessages          mReceivers;

	NString                             mCurrentMessage;
	NVectorBroadcastReceivers           mCurrentDestroyed;
};



#endif // NBROADCASTER_H
