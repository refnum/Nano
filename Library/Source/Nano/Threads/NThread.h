/*	NAME:
		NThread.h

	DESCRIPTION:
		Thread object.

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
#ifndef NTHREAD_H
#define NTHREAD_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFunction.h"
#include "NMutex.h"
#include "NRunLoop.h"
#include "NString.h"
#include "NanoTypes.h"

// System
#include <atomic>
#include <memory>
#include <thread>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NThread;


// Thread
using NUniqueThread = std::unique_ptr<NThread>;
using NThreadHandle = uintptr_t;


// Thread context
struct NThreadContext
{
	size_t    stackSize;
	NFunction threadEntry;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NThread
{
public:
	// Create a thread
	//
	// Every thread has a name and a runloop.
	//
	//
	// A thread may also be created with a custom stack size, where
	// a stack size of 0 is a request for the platform default.
	//
	// A thread may also be created with an invocable object and its
	// arguments, such as a lambda or function.
	//
	//
	// A thread created with an invocable will execute the invocable.
	//
	// A thread created without an invocable will enter its runloop
	// and wait for work.
	template<typename Function,
			 typename... Args,
			 typename = std::enable_if_t<std::is_invocable_v<Function&, Args...>>>
	static NUniqueThread                Create(const NString& theName, Function&& theFunction, Args&&... theArgs);

	template<typename Function,
			 typename... Args,
			 typename = std::enable_if_t<std::is_invocable_v<Function&, Args...>>>
	static NUniqueThread                Create(const NString& theName,
											   size_t         stackSize,
											   Function&&     theFunction,
											   Args&&... theArgs);

	static NUniqueThread                Create(const NString& theName, size_t stackSize = 0);



									   ~NThread();

										NThread(  const NThread& otherThread) = delete;
	NThread&                            operator=(const NThread& otherThread) = delete;

										NThread(  NThread&& otherThread) = delete;
	NThread&                            operator=(NThread&& otherThread) = delete;


	// Get the thread ID
	NThreadID                           GetID() const;


	// Get the runloop
	NSharedRunLoop                      GetRunLoop() const;


	// Test / wait for completion
	//
	// A thread is complete when it has finished executing.
	//
	// Destroying an NThread will wait for the associated thread to complete.
	bool                                IsComplete() const;
	void                                WaitForCompletion();


	// Request / test for stopping
	//
	// A thread that supports stopping must poll the stopping state.
	void                                RequestStop();
	bool                                ShouldStop();


	// Is the current thred the main thread?
	static bool                         IsMain();


	// Get the current thread
	//
	// Only available to NThread threads.
	static NThread*                     GetCurrent();


	// Get the current thread's stack size
	static size_t                       GetStackSize();


	// Get / set the current thread's name
	static NString                      GetName();
	static void                         SetName(const NString& theName);


	// Get / set the current thread's priority
	//
	// Thread priorities range from 0.0 (lowest) to 1.0 (highest).
	static float                        GetPriority();
	static void                         SetPriority(float thePriority);


	// Get / set the current thread's preferred cores
	//
	// Cores are indexed from 0 to the number of logical cores.
	//
	// An empty list indicates no preference.
	static NVectorUInt8                 GetCores();
	static void                         SetCores(const NVectorUInt8& theCores);


	// Sleep the current thread
	static void                         Sleep(NInterval sleepFor);


	// Switch the current thread
	//
	// Allows the OS to schedule another thread.
	static void                         Switch();


	// Pause the  current thread
	//
	// Allows the CPU to adapt to a busy-wait loop.
	static void                         Pause();


private:
	template<typename Function,
			 typename... Args,
			 typename = std::enable_if_t<std::is_invocable_v<Function&, Args...>>>
	explicit                            NThread(const NString& theName, Function&& theFunction, Args&&... theArgs);

	template<typename Function,
			 typename... Args,
			 typename = std::enable_if_t<std::is_invocable_v<Function&, Args...>>>
	explicit                            NThread(const NString& theName,
												size_t         stackSize,
												Function&&     theFunction,
												Args&&... theArgs);

										NThread(const NString& theName, size_t stackSize = 0);

	template<typename Function, typename... Args>
	void                                CreateThread(const NString& theName,
													 size_t         stackSize,
													 Function&&     theFunction,
													 Args&&... theArgs);

	void                                Clear();

	static NThreadHandle                ThreadCreate(NThreadContext* theContext);
	static void                         ThreadJoin(NThreadHandle theThread);
	static bool                         ThreadIsMain();


private:
	NMutex                              mLock;

	NThreadID                           mID;
	NThreadHandle                       mThread;
	NSharedRunLoop                      mRunLoop;
	std::atomic_bool                    mIsComplete;
	std::atomic_bool                    mShouldStop;

	static thread_local NThread*        mCurrentThread;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NThread.inl"



#endif // NTHREAD_H
