/*	NAME:
		NThread.inl

	DESCRIPTION:
		Thread object.

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
#include "NanoTargets.h"

#if NN_TARGET_WINDOWS
	#include <Windows.h>
#endif // NN_TARGET_WINDOWS

#if NN_ARCH_X86
	#include <immintrin.h>
#endif // NN_ARCH_X86





//=============================================================================
//		NThread::GetID : Get the thread ID.
//-----------------------------------------------------------------------------
NThreadID NThread::GetID()
{


	// Get the ID
	//
	// A std::thread::id is deliberately opaque, and has to be pushed
	// through a std::hash to obtain an integer representation.
	//
	// As a thread ID is always numeric we can use a platform-specific
	// method to avoid this hash overhead.
#if NN_TARGET_WINDOWS
	static_assert(sizeof(NThreadID) >= sizeof(DWORD));
	return NThreadID(GetCurrentThreadId());

#else
	static_assert(sizeof(NThreadID) >= sizeof(pthread_t));
	return NThreadID(pthread_self());
#endif
}





//=============================================================================
//		NThread::Switch : Switch the current thread.
//-----------------------------------------------------------------------------
void NThread::Switch()
{


	// Switch the thread
	std::this_thread::yield();
}





//=============================================================================
//		NThread::Pause : Pause the current thread.
//-----------------------------------------------------------------------------
void NThread::Pause()
{


	// Pause the thread.
	//
	// Inserts an architecture-specific instruction that informs the CPU we
	// are in a busy-wait loop, allowing it to adjust scheduling accordingly:
	//
	//	https://software.intel.com/en-us/articles/benefitting-power-and-performance-sleep-loops
	//
#if NN_ARCH_X86
	_mm_pause();

#elif NN_ARCH_ARM
	__asm__ __volatile__("yield");
#endif
}
