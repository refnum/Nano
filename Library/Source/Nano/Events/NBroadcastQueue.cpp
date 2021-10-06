/*	NAME:
		NBroadcastQueue.cpp

	DESCRIPTION:
		FIFO broadcast message queue.

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
#include "NBroadcastQueue.h"

// Nano
#include "NAsync.h"
#include "NBroadcaster.h"
#include "NScopedLock.h"





//=============================================================================
//		NBroadcastQueue::Add : Add a broadcast.
//-----------------------------------------------------------------------------
void NBroadcastQueue::Add(const NBroadcast& theBroadcast)
{


	// Add the broadcast
	//
	// Broadcasts are send asynchronously, in FIFO order.
	NScopedLock acquireLock(mLock);

	mBroadcasts.emplace_back(theBroadcast);

	NAsync(
	[=]()
	{
		SendBroadcasts();
	});
}





#pragma mark private
//=============================================================================
//		NBroadcastQueue::SendBroadcasts : Send the broadcasts.
//-----------------------------------------------------------------------------
void NBroadcastQueue::SendBroadcasts()
{


	// Send the broadcasts
	NScopedLock acquireLock(mLock);

	for (const auto& theBroadcast : mBroadcasts)
	{
		NBroadcaster::Send(theBroadcast);
	}

	mBroadcasts.clear();
}
