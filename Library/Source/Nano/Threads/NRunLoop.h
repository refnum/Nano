/*	NAME:
		NRunLoop.h

	DESCRIPTION:
		Thread runloop.

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
#ifndef NRUN_LOOP_H
#define NRUN_LOOP_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFunction.h"
#include "NMutex.h"
#include "NThreadID.h"
#include "NTime.h"

// System
#include <deque>
#include <memory>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// RunLoop work
using NRunLoopWorkID                                        = uint32_t;

inline constexpr NRunLoopWorkID NRunLoopWorkNone            = 0;





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NSemaphore;
class NRunLoop;


// RunLoop
using NSharedRunLoop = std::shared_ptr<NRunLoop>;
using NRunLoopHandle = uintptr_t;


// RunLoop work
struct NRunLoopWork
{
	NRunLoopWorkID              theID;
	NTime                       executeAt;
	NInterval                   executeEvery;
	NFunction                   theFunction;
	std::shared_ptr<NSemaphore> theSemaphore;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NRunLoop
{
public:
										NRunLoop(bool isMain);
									   ~NRunLoop();

										NRunLoop( const NRunLoop& otherRunLoop) = delete;
	NRunLoop&                           operator=(const NRunLoop& otherRunLoop) = delete;

										NRunLoop( NRunLoop&& otherRunLoop) = delete;
	NRunLoop&                           operator=(NRunLoop&& otherRunLoop) = delete;


	// Run the runloop
	//
	// Executes work until the timeout expires or the runloop is stopped.
	//
	// A timeout of kNTimeNone will execute at most one function.
	//
	// A runloop may be run recursively.
	void                                Run(NInterval runFor = kNTimeForever);


	// Stop the runloop
	//
	// Stop the current call to Run on the runloop.
	//
	// A runloop may be stopped from any thread.
	void                                Stop();


	// Add a function
	//
	// The function will be executed after the specified delay, and may be
	// repeated indefinitely with a specified interval.
	//
	// An optional semaphore can be supplied which will be signalled whenever
	// the function has been executed.
	NRunLoopWorkID                      Add(const NFunction&             theFunctor,
											NInterval                    executeAfter = kNTimeNone,
											NInterval                    executeEvery = kNTimeNone,
											std::shared_ptr<NSemaphore>* theSemaphore = nullptr);


	// Remove a function
	//
	// A function may be removed from any thread.
	void                                Remove(NRunLoopWorkID theID);


	// Get / set work execution time
	NTime                               GetWorkTime(NRunLoopWorkID theID) const;
	void                                SetWorkTime(NRunLoopWorkID theID, NTime theTime);


	// Get / set work execution interval
	NInterval                           GetWorkInterval(NRunLoopWorkID theID) const;
	void                                SetWorkInterval(NRunLoopWorkID theID, NInterval theInterval);


	// Get the main thread's runloop
	static NSharedRunLoop               GetMain();


	// Get the current thread's runloop
	static NSharedRunLoop               GetCurrent();


private:
	void                                PerformWork(bool onlyOne);
	bool                                FinishedSleep(NTime endTime);
	bool                                FetchNextWork(NRunLoopWork& theWork);
	NRunLoopWork*                       GetNextWork();

	static NRunLoopHandle               RunLoopCreate(bool isMain);
	static void                         RunLoopDestroy(NRunLoopHandle runLoop);
	static void                         RunLoopSleep(  NRunLoopHandle runLoop, NInterval sleepFor);
	static void                         RunLoopWake(   NRunLoopHandle runLoop);


private:
	mutable NMutex                      mLock;
	NRunLoopHandle                      mRunLoop;
	NThreadID                           mOwnerID;

	bool*                               mStopWork;
	bool                                mSortWork;
	NRunLoopWorkID                      mNextID;
	std::deque<NRunLoopWork>            mWork;
};



#endif // NRUN_LOOP_H
