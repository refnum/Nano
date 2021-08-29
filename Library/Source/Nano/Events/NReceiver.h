/*	NAME:
		NReceiver.h

	DESCRIPTION:
		Broadcast receiver.

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
#ifndef NRECEIVER_H
#define NRECEIVER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFunction.h"
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NBroadcast;

// Receivers
using NFunctionReceiver = std::function<void(const NBroadcast& theBroadcast)>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NReceiver
{
public:
										NReceiver() = default;
	virtual                            ~NReceiver();

										NReceiver(const NReceiver& otherReceiver) = delete;
	NReceiver&                          operator=(const NReceiver& otherReceiver) = delete;

										NReceiver(NReceiver&& otherReceiver) = delete;
	NReceiver&                          operator=(NReceiver&& otherReceiver) = delete;


	// Get the messages being received
	NVectorString                       GetMessages() const;


	// Start receiving a message
	//
	// A receiver may only register one function per message.
	void                                StartReceiving(const NString& theMessage, const NFunctionReceiver& theFunction);
	void                                StartReceiving(const NString& theMessage, const NFunction&         theFunction);


	// Stop receiving a message
	//
	// The receiver must be registered to receive the message.
	void                                StopReceiving(const NString& theMessage);


	// Stop receiving any messages
	void                                StopReceiving();
};



#endif // NRECEIVER_H
