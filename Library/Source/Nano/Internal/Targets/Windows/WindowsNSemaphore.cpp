/*	NAME:
		WindowsNSemaphore.cpp

	DESCRIPTION:
		macOS semaphore.

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
#include "NSemaphore.h"

// Nano
#include "NDebug.h"
#include "NSharedWindows.h"

// System
#include <limits.h>





//=============================================================================
//		NSemaphore::Create : Create the semaphore.
//-----------------------------------------------------------------------------
NSemaphoreRef NSemaphore::Create(size_t theValue)
{


	// Validate our parameters and state
	NN_REQUIRE(theValue <= size_t(LONG_MAX));

	static_assert(sizeof(NSemaphoreRef) >= sizeof(HANDLE));


	// Create the semaphore
	HANDLE semHnd = CreateSemaphore(nullptr, LONG(theValue), LONG_MAX, nullptr);
	NN_REQUIRE_NOT_NULL(semHnd);

	return NSemaphoreRef(semHnd);
}





//=============================================================================
//		NSemaphore::Destroy : Destroy the semaphore.
//-----------------------------------------------------------------------------
void NSemaphore::Destroy(NSemaphoreRef theSemaphore)
{


	// Destroy the semaphore
	HANDLE semHnd = HANDLE(theSemaphore);
	BOOL   wasOK  = CloseHandle(semHnd);

	NN_EXPECT(wasOK);
}





//=============================================================================
//		NSemaphore::Wait : Wait for the semaphore.
//-----------------------------------------------------------------------------
bool NSemaphore::Wait(NSemaphoreRef theSemaphore, NInterval waitFor)
{


	// Get the state we need
	HANDLE semHnd = HANDLE(theSemaphore);
	DWORD  waitMS = INFINITE;

	if (waitFor != kNTimeForever)
	{
		waitMS = DWORD(waitFor / kNTimeMillisecond);
	}



	// Wait for the semaphore
	DWORD theResult = WaitForSingleObject(semHnd, waitMS);
	NN_EXPECT(theResult == WAIT_OBJECT_0 || theResult == WAIT_TIMEOUT);

	return theResult == WAIT_OBJECT_0;
}





//=============================================================================
//		NSemaphore::Signal : Signal the semaphore.
//-----------------------------------------------------------------------------
void NSemaphore::Signal(NSemaphoreRef theSemaphore)
{


	// Signal the semaphore
	HANDLE semHnd = HANDLE(theSemaphore);
	BOOL   wasOK  = ReleaseSemaphore(semHnd, 1, nullptr);

	NN_EXPECT(wasOK);
}
