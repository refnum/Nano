/*	NAME:
		NThreadPool.h

	DESCRIPTION:
		Thread pool.

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
#ifndef NTHREAD_POOL_H
#define NTHREAD_POOL_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDeque.h"
#include "NFunction.h"
#include "NMutex.h"
#include "NRunLoopTask.h"
#include "NSemaphore.h"
#include "NString.h"
#include "NThread.h"
#include "NanoConstants.h"

// System
#include <atomic>
#include <memory>
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NThreadTask;


// Worker thread
struct NThreadPoolWorker
{
	NUniqueThread    theThread;
	std::atomic_bool isIdle;
};


// Thread pool
using NSharedThreadTask       = std::shared_ptr<NThreadTask>;
using NUniqueThreadPoolWorker = std::unique_ptr<NThreadPoolWorker>;
using NVectorThreadPoolWorker = std::vector<NUniqueThreadPoolWorker>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NThreadPool
{
public:
										NThreadPool(const NString& theName,
													size_t         minThreads = 1,
													size_t         maxThreads = kNSizeMax,
													size_t         stackSize  = 0);

									   ~NThreadPool();


	// Get the thread count
	//
	// The thread pool will allocate up to maxThreads threads to perform work.
	//
	// An idle pool will gradually reduce the number of active threads down
	// to minThreads.
	size_t                              GetThreads()    const;
	size_t                              GetMinThreads() const;
	size_t                              GetMaxThreads() const;


	// Is the pool paused?
	bool                                IsPaused() const;


	// Pause / resume the pool
	//
	// No further tasks are executed while the pool is paused.
	void                                Pause();
	void                                Resume();


	// Add a task to the pool
	void                                Add(const NFunction& theFunction);
	void                                Add(const NSharedThreadTask& theTask);


	// Get the main thread pool
	static NThreadPool*                 GetMain();


private:
	void                                StartWorkers();
	void                                StopWorkers();
	void                                ExecuteTasks(NThreadPoolWorker* theWorker);

	void                                CreateWorker();
	void                                StartWorker(NThreadPoolWorker* theWorker);

	void                                StartCulling();
	void                                StopCulling();
	void                                CullWorkers();


private:
	NString                             mName;
	size_t                              mMinThreads;
	size_t                              mMaxThreads;
	size_t                              mStackSize;

	mutable NMutex                      mLock;
	NRunLoopTask                        mCullTask;
	std::atomic_bool                    mIsPaused;
	std::atomic<NTime>                  mLastWork;
	NVectorThreadPoolWorker             mWorkers;
	NDeque<NSharedThreadTask>           mTasks;
};



#endif // NTHREAD_POOL_H
