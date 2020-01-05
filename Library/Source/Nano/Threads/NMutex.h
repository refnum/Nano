/*	NAME:
		NMutex.h

	DESCRIPTION:
		Mutex lock.

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
#ifndef NMUTEX_H
#define NMUTEX_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NSemaphore.h"
#include "NThread.h"


// System
#include <atomic>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NMutex
{
public:
										NMutex();
									   ~NMutex();

										NMutex(   const NMutex&) = delete;
	NMutex&                             operator=(const NMutex&) = delete;

										NMutex(   const NMutex&&) = delete;
	NMutex&                             operator=(const NMutex&&) = delete;


	// Is the lock locked?
	bool                                IsLocked() const;


	// Acquire/release the lock
	//
	// A mutex can be locked multiple times by the same thread.
	inline bool                         Lock(NInterval waitFor = kNTimeForever);
	inline void                         Unlock();


private:
	bool                                WaitForLock(     NInterval waitFor);
	bool                                WaitForSemaphore(NInterval waitFor);

	inline bool                         AcquireCount();
	inline bool                         ReleaseCount();

	inline uint32_t                     IncrementCount();
	inline uint32_t                     DecrementCount();


private:
	std::atomic<NSemaphoreRef>          mSemaphore;
	std::atomic<NThreadID>              mOwner;

	std::atomic_uint32_t                mLockCount;
	size_t                              mRecursion;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMutex.inl"



#endif // NMUTEX_H
