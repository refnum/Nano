/*	NAME:
		WindowsNThread.cpp

	DESCRIPTION:
		Windows thread.

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
#include "NThread.h"

// System
#include <Windows.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// MSVC thread support
static const DWORD MSVC_DEBUGGER                            = 0x406D1388;
static const DWORD MSVC_SET_THREADNAME                      = 0x1000;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// MSVC thread support
//
// Documented at:
//
//	http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
//
NN_STRUCT_PACK_8(THREADNAME_INFO {
	DWORD  dwType;
	LPCSTR szName;
	DWORD  dwThreadID;
	DWORD  dwFlags;
});





//=============================================================================
//		Global variables
//-----------------------------------------------------------------------------
// Thread
//
// The main thread performs static initialisation.
static DWORD                gMainThreadID                   = GetCurrentThreadId();
static thread_local NString gThreadName;





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		NThreadEntry : Thread entry point.
//-----------------------------------------------------------------------------
static DWORD WINAPI NThreadEntry(void* theParam)
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
#if NN_ARCH_64
	NT_TIB64* threadInfo = (NT_TIB64*) __readgsqword(0x30);
#else
	NT_TIB32* threadInfo = (NT_TIB32*) __readfsdword(0x18);
#endif


	// Get the size
	return uintptr_t(threadInfo->StackBase) - uintptr_t(threadInfo->StackLimit);
}





//=============================================================================
//		NThread::GetName : Get the current thread's name.
//-----------------------------------------------------------------------------
NString NThread::GetName()
{


	// Get the name
	return gThreadName;
}





//=============================================================================
//		NThread::SetName : Set the current thread's name.
//-----------------------------------------------------------------------------
void NThread::SetName(const NString& theName)
{


	// Get the state we need
	THREADNAME_INFO theInfo{};

	theInfo.dwType     = MSVC_SET_THREADNAME;
	theInfo.szName     = theName.GetUTF8();
	theInfo.dwThreadID = GetCurrentThreadId();
	theInfo.dwFlags    = 0;



	// Set the name
	__try
	{
		RaiseException(MSVC_DEBUGGER,
					   0,
					   sizeof(theInfo) / sizeof(ULONG_PTR),
					   (ULONG_PTR*) &theInfo);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	gThreadName = theName;
}





//=============================================================================
//		NThread::GetPriority : Get the current thread's priority.
//-----------------------------------------------------------------------------
float NThread::GetPriority()
{


	// Get the priority
	int winPriority = GetThreadPriority(GetCurrentThread());
	NN_EXPECT(winPriority >= THREAD_PRIORITY_LOWEST);
	NN_EXPECT(winPriority <= THREAD_PRIORITY_HIGHEST);

	float thePriority = float(winPriority - THREAD_PRIORITY_LOWEST) /
						float(THREAD_PRIORITY_HIGHEST - THREAD_PRIORITY_LOWEST);
	NN_REQUIRE(thePriority >= 0.0f && thePriority <= 1.0f);

	return thePriority;
}





//=============================================================================
//		NThread::SetPriority : Set the current thread's priority.
//-----------------------------------------------------------------------------
void NThread::SetPriority(float thePriority)
{


	// Validate our parameters
	NN_REQUIRE(thePriority >= 0.0f && thePriority <= 1.0f);



	// Set the priority
	int winPriority = THREAD_PRIORITY_LOWEST +
					  int(thePriority * (THREAD_PRIORITY_HIGHEST - THREAD_PRIORITY_LOWEST));
	NN_EXPECT(winPriority >= THREAD_PRIORITY_LOWEST);
	NN_EXPECT(winPriority <= THREAD_PRIORITY_HIGHEST);

	BOOL wasOK = SetThreadPriority(GetCurrentThread(), winPriority);
	NN_EXPECT(wasOK);
}





//=============================================================================
//		NThread::GetCores : Get the current thread's preferred cores.
//-----------------------------------------------------------------------------
NVectorUInt8 NThread::GetCores()
{


	// Get the thread affinity
	//
	// The current thread affinity is queried by setting some other affinity,
	// then restoring the previous value afterwards.
	DWORD_PTR threadAffinity = SetThreadAffinityMask(GetCurrentThread(), 1 << 0);
	NN_EXPECT_NOT_ERR(threadAffinity);

	(void) SetThreadAffinityMask(GetCurrentThread(), threadAffinity);



	// Convert to cores
	NVectorUInt8 theCores;

	for (uint8_t n = 0; n < kNUInt8Max; n++)
	{
		if (threadAffinity & (DWORD_PTR(1) << n))
		{
			theCores.push_back(n);
		}
	}

	return theCores;
}





//=============================================================================
//		NThread::SetCores : Set the current thread's preferred cores.
//-----------------------------------------------------------------------------
void NThread::SetCores(const NVectorUInt8& theCores)
{


	// Get the state we need
	DWORD_PTR threadAffinity = 0;

	for (auto n : theCores)
	{
		threadAffinity |= (DWORD_PTR(1) << n);
	}



	// Set the thread affinity
	DWORD_PTR oldAffinity = SetThreadAffinityMask(GetCurrentThread(), threadAffinity);
	NN_EXPECT_NOT_ERR(oldAffinity);
}





#pragma mark private
//=============================================================================
//		NThread::ThreadCreate : Create a native thread.
//-----------------------------------------------------------------------------
NThreadHandle NThread::ThreadCreate(NThreadContext* theContext)
{


	// Validate our state
	static_assert(sizeof(NThreadHandle) >= sizeof(HANDLE));



	// Create the thread
	HANDLE threadHnd =
		::CreateThread(nullptr, theContext->stackSize, NThreadEntry, theContext, 0, nullptr);
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
