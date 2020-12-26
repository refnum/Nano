/*	NAME:
		NThread.h

	DESCRIPTION:
		Thread object.

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
#ifndef NTHREAD_H
#define NTHREAD_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTypes.h"

// System
#include <atomic>
#include <mutex>
#include <thread>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NThread
{
public:
	template<class Function, class... Args>
	explicit                            NThread(Function&& theFunction, Args&&... theArgs);

										NThread() = delete;
									   ~NThread();

										NThread(  const NThread& otherThread) = delete;
	NThread&                            operator=(const NThread& otherThread) = delete;

										NThread(  NThread&& otherThread);
	NThread&                            operator=(NThread&& otherThread);


	// Test / wait for completion
	//
	// A thread is complete when it has finished executing.
	//
	// Destroying an NThread will wait for the associated thread to complete.
	bool                                IsComplete() const;
	void                                WaitForCompletion();


	// Request / test for stopping
	//
	// A running thread may be asked to stop early.
	//
	// A thread must poll the stopping state periodically if it can support
	// stopping early.
	void                                RequestStop();
	static bool                         ShouldStop();


	// Switch the current thread
	//
	// Allows the OS to schedule another thread.
	static void                         Switch();


	// Pause the  current thread
	//
	// Allows the CPU to adapt to a busy-wait loop.
	static void                         Pause();


	// Sleep the current thread
	static void                         Sleep(NInterval sleepFor);


private:
	static NThread*&                    GetThread();


private:
	std::mutex                          mLock;
	std::thread                         mThread;
	std::atomic_bool                    mIsComplete;
	std::atomic_bool                    mShouldStop;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NThread.inl"



#endif // NTHREAD_H
