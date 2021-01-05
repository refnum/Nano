/*	NAME:
		WindowsNRunLoop.cpp

	DESCRIPTION:
		Windows thread runloop.

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
#include "NRunLoop.h"

// Nano
#include "NSemaphore.h"





//=============================================================================
//		NRunLoop::RunLoopCreate : Create a runloop.
//-----------------------------------------------------------------------------
NRunLoopHandle NRunLoop::RunLoopCreate(bool /*isMain*/)
{


	// Validate our state
	static_assert(sizeof(NRunLoopHandle) >= sizeof(NSemaphore*));



	// Create the runloop
	NSemaphore* windowsRunLoop = new NSemaphore;

	return NRunLoopHandle(windowsRunLoop);
}





//=============================================================================
//		NRunLoop::RunLoopDestroy : Destroy a runloop.
//-----------------------------------------------------------------------------
void NRunLoop::RunLoopDestroy(NRunLoopHandle runLoop)
{


	// Get the state we need
	NSemaphore* windowsRunLoop = reinterpret_cast<NSemaphore*>(windowsRunLoop);



	// Destroy the runloop
	delete windowsRunLoop;
}





//=============================================================================
//		NRunLoop::RunLoopSleep : Sleep a runloop.
//-----------------------------------------------------------------------------
void NRunLoop::RunLoopSleep(NRunLoopHandle runLoop, NInterval sleepFor)
{


	// Get the state we need
	NSemaphore* windowsRunLoop = reinterpret_cast<NSemaphore*>(windowsRunLoop);



	// Run the system message queue
	//
	// We can use some of our time processing system messages.
	NInterval timeLeft = (sleepFor == kNTimeForever) ? kNTimeDistantFuture : sleepFor;
	NClock    theClock;
	MSG       winMsg;

	while (timeLeft > 0 && PeekMessage(&winMsg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&winMsg);
		DispatchMessage(&winMsg);

		timeLeft -= theClock.GetTime();
		theClock.Start();
	}

	timeLeft -= theClock.GetTime();



	// Sleep the runloop
	//
	// We spend the remaining time waiting for us to awake.
	(void) windowsRunLoop->Wait(sleepFor == kNTimeForever ? kNTimeForever : timeLeft);
}





//=============================================================================
//		NRunLoop::RunLoopWake : Wake a runloop.
//-----------------------------------------------------------------------------
void NRunLoop::RunLoopWake(NRunLoopHandle runLoop)
{


	// Get the state we need
	NSemaphore* windowsRunLoop = reinterpret_cast<NSemaphore*>(windowsRunLoop);


	// Wake the runloop
	windowsRunLoop->Signal();
}
