/*	NAME:
		WindowsNThread.cpp

	DESCRIPTION:
		Windows thread.

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
#include "NThread.h"

// System
#include <Windows.h>





//=============================================================================
//		Global variables
//-----------------------------------------------------------------------------
// Main thread
//
// The main thread performs static initialisation.
static DWORD gMainThreadID                                  = GetCurrentThreadId();





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		NThreadEntry : Thread entry point.
//-----------------------------------------------------------------------------
static DWORD WINAPI NThreadEntry(void* userData)
{


	// Get the state we need
	NThreadContext* theContext = reinterpret_cast<NThreadContext*>(theParam);



	// Invoke the thread
	theContext->threadEntry();

	delete theContext;
	return 0;
}





#pragma mark NThread
//=============================================================================
//		NThread::GetStackSize : Get the current thread's stack size.
//-----------------------------------------------------------------------------
size_t NThread::GetStackSize()
{


	// Get the Thread Information Block
#if NN_TARGET_ARCH_32
	NT_TIB32* threadInfo = (NT_TIB32*) __readfsdword(0x18);
#else
	NT_TIB64* threadInfo = (NT_TIB64*) __readgsqword(0x30);
#endif


	// Get the size
	return uintptr_t(threadInfo->StackBase) - uintptr_t(threadInfo->StackLimit);
}





//=============================================================================
//		NThread::ThreadCreate : Create a native thread.
//-----------------------------------------------------------------------------
NThreadHandle NThread::ThreadCreate(NThreadContext* theContext)
{


	// Validate our state
	static_assert(sizeof(NThreadHandle) >= sizeof(HANDLE));



	// Create the thread
	HANDLE threadHnd = CreateThread(nullptr, 0, NThreadEntry, theContext, 0, nullptr);
	NN_EXPECT(threadHnd != nullptr);

	return NThreadHandle(threadHnd);
}





//=============================================================================
//		NThread::ThreadJoin : Join a native thread.
//-----------------------------------------------------------------------------
void NThread::ThreadJoin(NThreadHandle theThread)
{


	// Join the thread
	HANDLE threadHnd = HANDLE(theThread);

	DWORD winErr = WaitForSingleObject(threadHnd, INFINITE);
	NN_EXPECT(winErr == WAIT_OBJECT_0);

	CloseHandle(threadHnd);
}





//=============================================================================
//		NThread::ThreadIsMain : Is this the main thread?
//-----------------------------------------------------------------------------
bool NThread::ThreadIsMain()
{


	// Check the thread
	bool isMain = (gMainThreadID == GetCurrentThreadId());

	return isMain;
}
