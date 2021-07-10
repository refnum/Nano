/*	NAME:
		NSemaphore.h

	DESCRIPTION:
		Semaphore object.

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
#ifndef NSEMAPHORE_H
#define NSEMAPHORE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoConstants.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Native semaphore
using NSemaphoreRef                                         = void*;

// Forward declarations
class NMutex;





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Native semaphore
inline constexpr NSemaphoreRef kNSemaphoreNone = nullptr;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSemaphore
{
	friend class                        NMutex;

public:
										NSemaphore(size_t theValue = 0);
									   ~NSemaphore();

										NSemaphore(const NSemaphore& otherSemaphore) = delete;
	NSemaphore&                         operator=( const NSemaphore& otherSemaphore) = delete;

										NSemaphore(NSemaphore&& otherSemaphore);
	NSemaphore&                         operator=( NSemaphore&& otherSemaphore);


	// Wait for the semaphore
	//
	// If the value of the semaphore is greater than zero, decrements the value
	// and returns true.
	//
	// Otherwise, the calling thread is blocked until the timeout occurs or the
	// semaphore is finally set.
	bool                                Wait(NInterval waitFor = kNTimeForever);


	// Signal the semaphore
	//
	// Wakes one of the threads which are waiting on the semaphore or,
	// if no threads are waiting, increments the value of the semaphore.
	void                                Signal(size_t numSignals = 1);


protected:
	// NMutex
	//
	// Must not be used directly.
	static NSemaphoreRef                Create(size_t theValue);
	static void                         Destroy(NSemaphoreRef theSemaphore);
	static bool                         Wait(   NSemaphoreRef theSemaphore, NInterval waitFor);
	static void                         Signal( NSemaphoreRef theSemaphore);


private:
	NSemaphoreRef                       mSemaphore;
};



#endif // NSEMAPHORE_H
