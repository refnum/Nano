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
#include "NFunction.h"
#include "NMutex.h"
#include "NString.h"

// System
#include <unordered_map>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaratiosn
class NListener;


// Functions
using NFunctionListenID = std::function<void(const NString& theMsg)>;


// Containers
using NMapListenerFunctions = std::unordered_map<NListener*, NFunctionListenID>;
using NMapMessageRecipients = std::unordered_map<NString, NMapListenerFunctions>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NBroadcaster
{
	friend class                        NListener;

public:
										NBroadcaster() = default;
	virtual                            ~NBroadcaster();

										NBroadcaster(const NBroadcaster& otherBroadcaster) = delete;
	NBroadcaster&                       operator=(   const NBroadcaster& otherBroadcaster) = delete;

										NBroadcaster(NBroadcaster&& otherBroadcaster) = delete;
	NBroadcaster&                       operator=(   NBroadcaster&& otherBroadcaster) = delete;


	// Broadcast a message
	void                                Broadcast(const NString& theMsg);


protected:
	// Add / remove a listener
	void                                AddListener(NListener*               theListener,
													const NString&           theMsg,
													const NFunctionListenID& theFunction);

	void                                RemoveListener(NListener* theListener, const NString& theMsg);


private:
	void                                RemoveListeners();


private:
	NMutex                              mLock;
	NMapMessageRecipients               mRecipients;
};



#endif // NBROADCASTER_H
