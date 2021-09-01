/*	NAME:
		NBroadcast.h

	DESCRIPTION:
		Broadcast message.

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
#ifndef NBROADCAST_H
#define NBROADCAST_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NAny.h"
#include "NString.h"

// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NBroadcast;


// Containers
using NVectorBroadcast = std::vector<NBroadcast>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NBroadcast
{
public:
										NBroadcast(const NString& theName);
										NBroadcast(const NString& theName, const NAny& theValue);


	// Get the name
	const NString&                      GetName() const;


	// Get the value
	const NAny&                         GetValue() const;


private:
	NString                             mName;
	NAny                                mValue;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NBroadcast.inl"


#endif // NBROADCAST_RECEIVER_H
