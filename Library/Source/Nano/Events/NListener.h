/*	NAME:
		NListener.h

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
#ifndef NLISTENER_H
#define NLISTENER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NBroadcaster.h"
#include "NMutex.h"

// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaratiosn
class NString;

// Containers
using NVectorBroadcaster = std::vector<NBroadcaster*>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NListener
{
public:
										NListener() = default;
	virtual                            ~NListener();

										NListener(const NListener& otherListener) = delete;
	NListener&                          operator=(const NListener& otherListener) = delete;

										NListener(NListener&& otherListener) = delete;
	NListener&                          operator=(NListener&& otherListener) = delete;


	// Start / stop listening for a message
	//
	// Each listener may register one function with a broadcaster for a given message.
	void                                StartListening(NBroadcaster*            theBroadcaster,
													   const NString&           theMsg,
													   const NFunctionListenID& theFunction);

	void                                StartListening(NBroadcaster*    theBroadcaster,
													   const NString&   theMsg,
													   const NFunction& theFunction);

	void                                StopListening(NBroadcaster* theBroadcaster, const NString& theMsg);


private:
	void                                StopListening();


private:
	NMutex                              mLock;
	NVectorBroadcaster                  mBroadcasters;
};



#endif // NLISTENER_H
