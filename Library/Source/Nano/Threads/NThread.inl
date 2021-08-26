/*	NAME:
		NThread.inl

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTargets.h"


// System
#if NN_ARCH_X86
	#include <immintrin.h>
#endif // NN_ARCH_X86





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline constexpr NThreadHandle kNThreadNone                 = 0;





//=============================================================================
//		NThread::NThread : Constructor.
//-----------------------------------------------------------------------------
template<typename Function, typename... Args, typename Enabled>
NUniqueThread NThread::Create(const NString& theName, Function&& theFunction, Args&&... theArgs)
{


	// Create the thread
	return NUniqueThread(new NThread(theName, theFunction, theArgs...));
}





//=============================================================================
//		NThread::NThread : Constructor.
//-----------------------------------------------------------------------------
template<typename Function, typename... Args, typename Enabled>
NUniqueThread NThread::Create(const NString& theName,
							  size_t         stackSize,
							  Function&&     theFunction,
							  Args&&... theArgs)
{


	// Create the thread
	return NUniqueThread(new NThread(theName, stackSize, theFunction, theArgs...));
}





//=============================================================================
//		NThread::~NThread : Destructor.
//-----------------------------------------------------------------------------
inline NThread::~NThread()
{


	// Wait for the thread
	WaitForCompletion();
}





//=============================================================================
//		NThread::GetID : Get the thread ID.
//-----------------------------------------------------------------------------
inline NThreadID NThread::GetID() const
{


	// Get the ID
	return mID;
}





//=============================================================================
//		NThread::GetRunLoop : Get the runloop.
//-----------------------------------------------------------------------------
inline NSharedRunLoop NThread::GetRunLoop() const
{


	// Get our state
	return mRunLoop;
}





//=============================================================================
//		NThread::IsComplete : Is the thread complete?
//-----------------------------------------------------------------------------
inline bool NThread::IsComplete() const
{


	// Validate our state
	NN_REQUIRE(NThreadID::Get() != mID, "The running thread is never complete!");



	// Get our state
	return mIsComplete;
}





//=============================================================================
//		NThread::RequestStop : Request that the thread stop.
//-----------------------------------------------------------------------------
inline void NThread::RequestStop()
{


	// Update our state
	mShouldStop = true;
}





//=============================================================================
//		NThread::ShouldStop : Should a thread stop?
//-----------------------------------------------------------------------------
inline bool NThread::ShouldStop()
{


	// Get our state
	return mShouldStop;
}





//=============================================================================
//		NThread::GetCurrent : Get the current thread.
//-----------------------------------------------------------------------------
inline NThread* NThread::GetCurrent()
{


	// Get the thread
	return mCurrentThread;
}





//=============================================================================
//		NThread::Switch : Switch the current thread.
//-----------------------------------------------------------------------------
inline void NThread::Switch()
{


	// Switch the thread
	std::this_thread::yield();
}





//=============================================================================
//		NThread::Pause : Pause the current thread.
//-----------------------------------------------------------------------------
inline void NThread::Pause()
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



#pragma mark private





//=============================================================================
//		NThread::NThread : Constructor.
//-----------------------------------------------------------------------------
template<typename Function, typename... Args, typename Enabled>
NThread::NThread(const NString& theName, Function&& theFunction, Args&&... theArgs)
	: mLock()
	, mID()
	, mThread(kNThreadNone)
	, mRunLoop(nullptr)
	, mIsComplete(false)
	, mShouldStop(false)
{


	// Create the thread
	CreateThread(theName, 0, theFunction, theArgs...);
}





//=============================================================================
//		NThread::NThread : Constructor.
//-----------------------------------------------------------------------------
template<typename Function, typename... Args, typename Enabled>
NThread::NThread(const NString& theName,
				 size_t         stackSize,
				 Function&&     theFunction,
				 Args&&... theArgs)
	: mLock()
	, mID()
	, mThread(kNThreadNone)
	, mRunLoop(nullptr)
	, mIsComplete(false)
	, mShouldStop(false)
{


	// Create the thread
	CreateThread(theName, stackSize, theFunction, theArgs...);
}





//=============================================================================
//		NThread::NThread : Constructor.
//-----------------------------------------------------------------------------
inline NThread::NThread(const NString& theName, size_t stackSize)
	: mLock()
	, mID()
	, mThread(kNThreadNone)
	, mRunLoop(nullptr)
	, mIsComplete(false)
	, mShouldStop(false)
{


	// Create the thread
	CreateThread(theName,
				 stackSize,
	[&]()
	{
		mRunLoop->Run();
	});
}





//=============================================================================
//		CreateThread : Create a thread.
//-----------------------------------------------------------------------------
template<typename Function, typename... Args>
void NThread::CreateThread(const NString& theName,
						   size_t         stackSize,
						   Function&&     theFunction,
						   Args&&... theArgs)
{


	// Validate our state
	NN_REQUIRE(mThread == kNThreadNone);


	// Create the context
	NThreadContext* theContext = new NThreadContext{};
	NSemaphore      isRunning;

	theContext->stackSize   = stackSize;
	theContext->threadEntry = [=, &isRunning]()
	{
		// Prepare the thread
		mCurrentThread = this;

		mID      = NThreadID::Get();
		mRunLoop = NRunLoop::GetCurrent();

		if (!theName.IsEmpty())
		{
			NThread::SetName(theName);
		}



		// Execute the thread
		isRunning.Signal();
		theFunction(theArgs...);

		mIsComplete = true;
	};



	// Create the thread
	mThread = ThreadCreate(theContext);
	NN_REQUIRE(mThread != kNThreadNone);

	isRunning.Wait();
}
