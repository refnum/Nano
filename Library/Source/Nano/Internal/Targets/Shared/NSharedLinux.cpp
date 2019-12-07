/*	NAME:
		NSharedLinux.cpp

	DESCRIPTION:
		Linux support.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
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
#include "NSharedLinux.h"

// Nano
#include "NDebug.h"
#include "NTimeUtils.h"

// System
#include <limits.h>
#include <semaphore.h>
#include <stdlib.h>





//=============================================================================
//		NSharedLinux::SemaphoreCreate : Create a semaphore.
//-----------------------------------------------------------------------------
NSemaphoreRef NSharedLinux::SemaphoreCreate(size_t theValue)
{


	// Validate our parameters and state
	NN_REQUIRE(theValue <= size_t(UINT_MAX));

	static_assert(sizeof(NSemaphoreRef) >= sizeof(sem_t*));



	// Create the semaphore
	sem_t* semRef = (sem_t*) malloc(sizeof(sem_t));
	NN_REQUIRE_NOT_NULL(semRef);

	int sysErr = sem_init(semRef, 0, static_cast<unsigned int>(theValue));
	NN_REQUIRE_NOT_ERR(sysErr);

	return NSemaphoreRef(semRef);
}





//=============================================================================
//		NSharedLinux::SemaphoreDestroy : Destroy a semaphore.
//-----------------------------------------------------------------------------
void NSharedLinux::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{


	// Destroy the semaphore
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = sem_destroy(semRef);

	NN_REQUIRE_NOT_ERR(sysErr);

	free(semRef);
}





//=============================================================================
//		NSharedLinux::SemaphoreWait : Wait for a semaphore.
//-----------------------------------------------------------------------------
bool NSharedLinux::SemaphoreWait(NSemaphoreRef theSemaphore, NInterval waitFor)
{


	// Get the state we need
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = 0;



	// Wait for the semaphore
	if (waitFor == kNTimeForever)
	{
		sysErr = sem_wait(semRef);
	}
	else
	{
		struct timespec semWait = NTimeUtils::ToTimespec(waitFor);
		sysErr                  = sem_timedwait(semRef, &semWait);
	}

	return sysErr == 0;
}





//=============================================================================
//		NSharedLinux::SemaphoreSignal : Signal a semaphore.
//-----------------------------------------------------------------------------
void NSharedLinux::SemaphoreSignal(NSemaphoreRef theSemaphore)
{


	// Signal the semaphore
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = sem_post(semRef);

	NN_EXPECT_NOT_ERR(sysErr);
}
