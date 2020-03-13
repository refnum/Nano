/*	NAME:
		NSharedDarwin.cpp

	DESCRIPTION:
		Darwin support.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#include "NSharedDarwin.h"

// Nano
#include "NDebug.h"
#include "NSharedPOSIX.h"
#include "NanoConstants.h"

// System
#include <CoreFoundation/CoreFoundation.h>
#include <dispatch/semaphore.h>
#include <sys/sysctl.h>





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//      GetBootTime : Get the boot time.
//-----------------------------------------------------------------------------
static NInterval GetBootTime()
{


	// Get the boot time
	struct timeval timeVal     = {};
	size_t         theSize     = sizeof(timeVal);
	int            theNames[2] = {CTL_KERN, KERN_BOOTTIME};

	int sysErr = sysctl(theNames, 2, &timeVal, &theSize, nullptr, 0);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeVal, 0x00, sizeof(timeVal));
	}



	// Convert the time
	return NSharedPOSIX::ToInterval(timeVal);
}





#pragma mark NSharedDarwin
//=============================================================================
//		NSharedDarwin::GetTime : Get the time.
//-----------------------------------------------------------------------------
NTime NSharedDarwin::GetTime()
{


	// Get the time
	return NTime(CFAbsoluteTimeGetCurrent(), kNanoEpochFrom2001);
}





//=============================================================================
//		NSharedDarwin::GetUpTime : Get the time since boot.
//-----------------------------------------------------------------------------
NInterval NSharedDarwin::GetUpTime()
{


	// Get the time since boot
	static NInterval sBootTime = GetBootTime();

	return GetTime() - sBootTime;
}





//=============================================================================
//		NSharedDarwin::SemaphoreCreate : Create a semaphore.
//-----------------------------------------------------------------------------
NSemaphoreRef NSharedDarwin::SemaphoreCreate(size_t theValue)
{


	// Validate our parameters and state
	NN_REQUIRE(theValue <= size_t(LONG_MAX));

	static_assert(sizeof(NSemaphoreRef) >= sizeof(dispatch_semaphore_t));



	// Create the semaphore
	//
	// A dispatch semaphore can avoid a trip to the kernel and so
	// performs substantially better than a Mach semaphore.
	//
	//	A signal/wait loop that runs 10,000,000 times takes:
	//
	//		Mach semaphore:			4.7s
	//		Dispatch semaphore:		0.22s
	//
	// Timings from a 2.2Ghz Core i7.
	dispatch_semaphore_t dispatchSem = dispatch_semaphore_create(long(theValue));
	NN_REQUIRE_NOT_NULL(dispatchSem);

	return NSemaphoreRef(dispatchSem);
}





//=============================================================================
//		NSharedDarwin::SemaphoreDestroy : Destroy a semaphore.
//-----------------------------------------------------------------------------
void NSharedDarwin::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{


	// Destroy the semaphore
	dispatch_semaphore_t dispatchSem = dispatch_semaphore_t(theSemaphore);

	dispatch_release(dispatchSem);
}





//=============================================================================
//		NSharedDarwin::SemaphoreWait : Wait for a semaphore.
//-----------------------------------------------------------------------------
bool NSharedDarwin::SemaphoreWait(NSemaphoreRef theSemaphore, NInterval waitFor)
{


	// Get the state we need
	dispatch_semaphore_t dispatchSem  = dispatch_semaphore_t(theSemaphore);
	dispatch_time_t      dispatchWait = DISPATCH_TIME_FOREVER;

	if (waitFor != kNTimeForever)
	{
		dispatchWait = dispatch_time(DISPATCH_TIME_NOW, int64_t(waitFor / kNTimeNanosecond));
	}



	// Wait for the semaphore
	long sysErr = dispatch_semaphore_wait(dispatchSem, dispatchWait);
	NN_EXPECT_NOT_ERR(sysErr);

	return sysErr == 0;
}





//=============================================================================
//		NSharedDarwin::SemaphoreSignal : Signal a semaphore.
//-----------------------------------------------------------------------------
void NSharedDarwin::SemaphoreSignal(NSemaphoreRef theSemaphore)
{


	// Signal the semaphore
	dispatch_semaphore_t dispatchSem = dispatch_semaphore_t(theSemaphore);

	(void) dispatch_semaphore_signal(dispatchSem);
}
