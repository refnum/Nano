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
	, mThread(std::move(otherThread.mThread))
	, mIsComplete(otherThread.mIsComplete.load())
	, mShouldStop(otherThread.mShouldStop.load())
{


	// Reset the other thread
	NN_REQUIRE(otherThread.mThread.get_id() == std::thread::id());

	otherThread.mIsComplete = true;
	otherThread.mShouldStop = false;
}





//=============================================================================
//		NThread::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NThread& NThread::operator=(NThread&& otherThread)
{


	// Validate our state
	NN_REQUIRE(mThread.get_id() == std::thread::id());
	NN_REQUIRE(mIsComplete);



	// Move the thread
	if (this != &otherThread)
	{
		std::swap(mThread, otherThread.mThread);
		mIsComplete = otherThread.mIsComplete.load();
		mShouldStop = otherThread.mShouldStop.load();

		otherThread.mIsComplete = true;
		otherThread.mShouldStop = false;
	}

	return *this;
}





//=============================================================================
//		NThread::IsComplete : Is the thread complete?
//-----------------------------------------------------------------------------
bool NThread::IsComplete() const
{


	// Validate our state
	NN_REQUIRE(mThread.get_id() != std::this_thread::get_id());



	// Get our state
	return mIsComplete;
}





//=============================================================================
//		NThread::WaitForCompletion : Wait for the thread.
//-----------------------------------------------------------------------------
void NThread::WaitForCompletion()
{


	// Validate our state
	NN_REQUIRE(mThread.get_id() != std::this_thread::get_id());


	// Wait for the thread
	if (mThread.joinable())
	{
		NScopedLock acquireLock(mLock);
		mThread.join();
	}
}





//=============================================================================
//		NThread::RequestStop : Request that the thread stop.
//-----------------------------------------------------------------------------
void NThread::RequestStop()
{


	// Validate our state
	NN_REQUIRE(mThread.get_id() != std::this_thread::get_id());


	// Update our state
	mShouldStop = true;
}





//=============================================================================
//		NThread::ShouldStop : Should a thread stop?
//-----------------------------------------------------------------------------
bool NThread::ShouldStop()
{


	// Get our state
	return GetThread()->mShouldStop;
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
//		NThread::GetThread : Get the current thread.
//-----------------------------------------------------------------------------
NThread*& NThread::GetThread()
{


	// Get the thread
	static thread_local NThread* sThread;

	return sThread;
}
