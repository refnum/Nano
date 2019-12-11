/*	NAME:
		NThread.h

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
#ifndef NTHREAD_H
#define NTHREAD_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// System
#include <thread>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
using NThreadID                                             = uintptr_t;





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static constexpr NThreadID kNThreadIDNone                   = 0;



// Nano 3.x
#if 0
	#include "NFunctor.h"
	#include "NSemaphore.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Thread local value
typedef uintptr_t NThreadLocalRef;

static const NThreadLocalRef kNThreadLocalRefNone           = (NThreadLocalRef) -1;


// Lists
class NThread;

typedef std::vector<NThread*>                                       NThreadList;
typedef NThreadList::iterator NThreadListIterator;
typedef NThreadList::const_iterator NThreadListConstIterator;

#endif





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NThread
{
public:
	// Get the thread ID
	static inline NThreadID             GetID();


	// Switch the current thread
	//
	// Allows the OS to schedule another thread.
	inline static void                  Switch();


	// Pause the  current thread
	//
	// Allows the CPU to adapt to a busy-wait loop.
	inline static void                  Pause();


// Nano 3.x
#if 0
NThread();
	virtual                            ~NThread();


// Is this the main thread?
static bool                         IsMain();


// Is the thread running?
bool                                IsRunning() const;


// Get/set the current thread name
static NString                      GetName();
static void                         SetName(const NString &theName);


// Are two thread IDs equal?
static bool                         AreEqual(NThreadID thread1, NThreadID thread2);


// Get/set the auto-delete state
//
// Auto-delete threads will delete themselves once Run returns.
bool                                IsAutoDelete() const;
void                                SetAutoDelete(bool autoDelete);


// Start/stop the thread
void                                Start();
void                                Stop(bool shouldWait=true);


// Sleep the current thread
//
// A sleep time of kNTimeNone indicates a request to yield the processor
// if desireable, rather than actually sleeping the thread.
static void                         Sleep(NTime theTime=kNThreadSleepTime);


// Create/destroy a thread-local value
static NThreadLocalRef              CreateLocal();
static void                         DestroyLocal(NThreadLocalRef theKey);


// Get/set a thread-local value
static void                        *GetLocalValue(NThreadLocalRef theKey);
static void                         SetLocalValue(NThreadLocalRef theKey, void *theValue);


// Invoke a functor on the main thread
//
// The thread will block until the functor has executed.
static void                         InvokeMain(const NFunctor &theFunctor);


protected:
// Should the thread stop?
bool                                ShouldStop() const;


// Run the thread
virtual void                        Run() = 0;


private:
void                                InvokeRun();


private:
bool mIsRunning;
bool mAutoDelete;
bool mShouldStop;
#endif
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NThread.inl"



#endif // NTHREAD_H
