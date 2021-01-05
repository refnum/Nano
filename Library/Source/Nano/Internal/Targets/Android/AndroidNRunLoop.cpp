/*	NAME:
		AndroidNRunLoop.cpp

	DESCRIPTION:
		Android thread runloop.

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


// System
NN_DIAGNOSTIC_IGNORE_CLANG("-Wreserved-id-macro");
NN_DIAGNOSTIC_IGNORE_CLANG("-Wmissing-variable-declarations");

#include <native_app_glue/android_native_app_glue.h>





//=============================================================================
//		Global variables
//-----------------------------------------------------------------------------
// Nano application
//
// Defined in AndroidNThread.cpp.
extern android_app* gNanoAndroidApp;





//=============================================================================
//		NRunLoop::RunLoopCreate : Create a runloop.
//-----------------------------------------------------------------------------
NRunLoopHandle NRunLoop::RunLoopCreate(bool isMain)
{


	// Validate our state
	static_assert(sizeof(NRunLoopHandle) >= sizeof(ALooper*));



	// Create the runloop
	ALooper* androidRunLoop = nullptr;

	if (isMain)
	{
		androidRunLoop = gNanoAndroidApp->looper;
		ALooper_acquire(androidRunLoop);
	}
	else
	{
		androidRunLoop = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	}

	return NRunLoopHandle(androidRunLoop);
}





//=============================================================================
//		NRunLoop::RunLoopDestroy : Destroy a runloop.
//-----------------------------------------------------------------------------
void NRunLoop::RunLoopDestroy(NRunLoopHandle runLoop)
{


	// Get the state we need
	ALooper* androidRunLoop = reinterpret_cast<ALooper*>(runLoop);



	// Destroy the runloop
	ALooper_release(androidRunLoop);
}





//=============================================================================
//		NRunLoop::RunLoopSleep : Sleep a runloop.
//-----------------------------------------------------------------------------
void NRunLoop::RunLoopSleep(NRunLoopHandle runLoop, NInterval sleepFor)
{


	// Get the state we need
	ALooper* androidRunLoop = reinterpret_cast<ALooper*>(runLoop);
	NN_REQUIRE(ALooper_forThread() == androidRunLoop);

	struct android_poll_source* pollSource = nullptr;
	int                         timeMS     = -1;

	if (sleepFor != kNTimeForever)
	{
		NN_REQUIRE(sleepFor >= 0.0);
		timeMS = int(NTime::ToMS(sleepFor));
	}



	// Sleep the runloop
	if (ALooper_pollAll(timeMS, nullptr, nullptr, reinterpret_cast<void**>(&pollSource)) >= 0)
	{
		if (pollSource != nullptr)
		{
			pollSource->process(pollSource->app, pollSource);
		}
	}
}





//=============================================================================
//		NRunLoop::RunLoopWake : Wake a runloop.
//-----------------------------------------------------------------------------
void NRunLoop::RunLoopWake(NRunLoopHandle runLoop)
{


	// Get the state we need
	ALooper* androidRunLoop = reinterpret_cast<ALooper*>(runLoop);


	// Wake the runloop
	ALooper_wake(androidRunLoop);
}
