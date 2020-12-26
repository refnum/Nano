/*	NAME:
		NThread.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NThread.h"

// Nano
#include "NDebug.h"
#include "NScopedLock.h"





//=============================================================================
//		Static variables
//-----------------------------------------------------------------------------
thread_local NThread* NThread::mThisThread;





//=============================================================================
//		NThread::~NThread : Destructor.
//-----------------------------------------------------------------------------
NThread::~NThread()
{


	// Wait for the thread
	WaitForCompletion();
}





//=============================================================================
//		NThread::NThread : Constructor.
//-----------------------------------------------------------------------------
NThread::NThread(NThread&& otherThread)
	: mLock()
	, mID(otherThread.mID)
	, mThread(otherThread.mThread)
	, mIsComplete(otherThread.mIsComplete.load())
	, mShouldStop(otherThread.mShouldStop.load())
{


	// Reset the other thread
	otherThread.mID.Clear();
	otherThread.mThread     = kNThreadNone;
	otherThread.mIsComplete = true;
	otherThread.mShouldStop = false;
}





//=============================================================================
//		NThread::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NThread& NThread::operator=(NThread&& otherThread)
{


	// Validate our state
	NN_REQUIRE(!mID.IsValid());
	NN_REQUIRE(mIsComplete);



	// Move the thread
	if (this != &otherThread)
	{
		// Move the thread
		mID         = otherThread.mID;
		mThread     = otherThread.mThread;
		mIsComplete = otherThread.mIsComplete.load();
		mShouldStop = otherThread.mShouldStop.load();


		// Reset the other thread
		otherThread.mID.Clear();
		otherThread.mThread     = kNThreadNone;
		otherThread.mIsComplete = true;
		otherThread.mShouldStop = false;
	}

	return *this;
}





//=============================================================================
//		NThread::GetID : Get the thread ID.
//-----------------------------------------------------------------------------
NThreadID NThread::GetID() const
{


	// Get the ID
	return mID;
}





//=============================================================================
//		NThread::IsComplete : Is the thread complete?
//-----------------------------------------------------------------------------
bool NThread::IsComplete() const
{


	// Validate our state
	NN_REQUIRE(NThreadID::Get() != mID, "The running thread is never complete!");



	// Get our state
	return mIsComplete;
}





//=============================================================================
//		NThread::WaitForCompletion : Wait for the thread.
//-----------------------------------------------------------------------------
void NThread::WaitForCompletion()
{


	// Validate our state
	NN_REQUIRE(NThreadID::Get() != mID, "The running thread cannot wait for itself!");



	// Wait for the thread
	NScopedLock acquireLock(mLock);

	ThreadJoin(mThread);
	mThread = kNThreadNone;
}





//=============================================================================
//		NThread::RequestStop : Request that the thread stop.
//-----------------------------------------------------------------------------
void NThread::RequestStop()
{


	// Update our state
	mShouldStop = true;
}





//=============================================================================
//		NThread::ShouldStop : Should a thread stop?
//-----------------------------------------------------------------------------
bool NThread::ShouldStop()
{


	// Get our state
	return mThisThread->mShouldStop;
}





//=============================================================================
//		NThread::Sleep : Sleep the current thread.
//-----------------------------------------------------------------------------
void NThread::Sleep(NInterval sleepFor)
{


	// Sleep the thread
	int64_t timeNS = int64_t(sleepFor / kNTimeNanosecond);
	std::this_thread::sleep_for(std::chrono::nanoseconds(timeNS));
}





//=============================================================================
//		NThread::IsMain : Is the current thread the main thread?
//-----------------------------------------------------------------------------
bool NThread::IsMain()
{


	// Check the thread
	//
	// Checking the main thread can be expensive on some platforms
	// so we cache the result.
	static thread_local bool sIsMain = ThreadIsMain();

	return sIsMain;
}
