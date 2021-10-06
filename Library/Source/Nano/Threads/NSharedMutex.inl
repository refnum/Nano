/*	NAME:
		NSharedMutex.inl

	DESCRIPTION:
		Shared mutex.

		Selects the most performant implementation for the current system.

		BRAVO locking is based on:

			BRAVO - Biased Locking for Reader-Writer Locks
			https://www.usenix.org/system/files/atc19-dice.pdf.

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
#include "NDebug.h"
#include "NThread.h"
#include "NThreadID.h"
#include "NTimeUtils.h"

// System
#include <atomic>
#include <shared_mutex>
#include <vector>

#if __has_include(<pthread.h>)
	#include <pthread.h>
#endif





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
#define NN_NSHAREDMUTEX_PTHREAD                             __has_include(<pthread.h>)





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NSharedMutexBravo;

// Containers
using NVectorSharedMutexBravo = std::vector<std::atomic<NSharedMutexBravo*>>;





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline constexpr size_t    kNSharedReaders      = 4096;
inline constexpr float64_t kNSharedInhibitScale = 9.0;

inline constexpr NInterval kNSharedMaxSleep = 2.0;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSharedMutexBravo
{
public:
										NSharedMutexBravo();
									   ~NSharedMutexBravo() = default;

										NSharedMutexBravo(const NSharedMutexBravo& otherMutex) = delete;
	NSharedMutexBravo&                  operator=(        const NSharedMutexBravo& otherMutex) = delete;

										NSharedMutexBravo(const NSharedMutexBravo&& otherMutex) = delete;
	NSharedMutexBravo&                  operator=(        const NSharedMutexBravo&& otherMutex) = delete;


	// Acquire/release an exclusive lock
	//
	// A single thread may acquire the exclusive lock.
	bool                                Lock(NInterval waitFor = kNTimeForever);
	void                                Unlock();


	// Acquire/release a shared lock
	//
	// Multiple threads may acquire the shared lock.
	bool                                LockShared(NInterval waitFor = kNTimeForever);
	void                                UnlockShared();


private:
	size_t                              GetMutexIndex();
	size_t&                             GetReaderIndex();
	NVectorSharedMutexBravo&            GetVisibleReaders();



private:
	NTime                               mInhibitTime;
	std::atomic_bool                    mFastReads;
	std::shared_timed_mutex             mLock;
};



#if NN_NSHAREDMUTEX_PTHREAD
//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSharedMutexPThread
{
public:
										NSharedMutexPThread();
									   ~NSharedMutexPThread();

										NSharedMutexPThread(const NSharedMutexPThread& otherMutex) = delete;
	NSharedMutexPThread&                operator=(          const NSharedMutexPThread& otherMutex) = delete;

										NSharedMutexPThread(const NSharedMutexPThread&& otherMutex) = delete;
	NSharedMutexPThread&                operator=(          const NSharedMutexPThread&& otherMutex) = delete;


	// Acquire/release an exclusive lock
	//
	// A single thread may acquire the exclusive lock.
	bool                                Lock(NInterval waitFor = kNTimeForever);
	void                                Unlock();


	// Acquire/release a shared lock
	//
	// Multiple threads may acquire the shared lock.
	bool                                LockShared(NInterval waitFor = kNTimeForever);
	void                                UnlockShared();



private:
	pthread_rwlock_t                    mLock;
};
#endif // NN_NSHAREDMUTEX_PTHREAD





#pragma mark NSharedMutexBravo
//=============================================================================
//		NSharedMutexBravo::NSharedMutexBravo : Constructor.
//-----------------------------------------------------------------------------
inline NSharedMutexBravo::NSharedMutexBravo()
	: mInhibitTime(0)
	, mFastReads(false)
	, mLock()
{
}





//=============================================================================
//		NSharedMutexBravo::Lock : Acquire the exclusive lock.
//-----------------------------------------------------------------------------
inline bool NSharedMutexBravo::Lock(NInterval waitFor)
{


	// Acquire the lock
	bool didLock = false;

	if (waitFor == kNTimeForever)
	{
		mLock.lock();
		didLock = true;
	}
	else
	{
		auto theTimeout = std::chrono::nanoseconds(NTime::ToNS(waitFor));
		didLock         = mLock.try_lock_for(theTimeout);
	}



	// Inhibit future readers
	if (didLock && mFastReads)
	{
		mFastReads      = false;
		NTime timeStart = NTimeUtils::GetTime();

		for (auto& theReader : GetVisibleReaders())
		{
			while (theReader == this)
			{
				NThread::Pause();
			}
		}

		NTime timeEnd = NTimeUtils::GetTime();
		mInhibitTime  = timeEnd + ((timeEnd - timeStart) * kNSharedInhibitScale);
	}

	return didLock;
}





//=============================================================================
//		NSharedMutexBravo::Unlock : Release the exclusive lock.
//-----------------------------------------------------------------------------
inline void NSharedMutexBravo::Unlock()
{


	// Release the lock
	mLock.unlock();
}





//=============================================================================
//		NSharedMutexBravo::LockShared : Acquire the shared lock.
//-----------------------------------------------------------------------------
inline bool NSharedMutexBravo::LockShared(NInterval waitFor)
{


	// Fast path
	if (mFastReads)
	{
		size_t             theIndex  = GetMutexIndex();
		auto&              theReader = GetVisibleReaders().at(theIndex);
		NSharedMutexBravo* nullMutex = nullptr;

		if (theReader.compare_exchange_strong(nullMutex, this))
		{
			if (mFastReads)
			{
				GetReaderIndex() = theIndex;
				return true;
			}

			theReader = nullptr;
		}
	}



	// Slow path
	bool didLock = false;

	if (waitFor == kNTimeForever)
	{
		mLock.lock_shared();
		didLock = true;
	}
	else
	{
		auto theTimeout = std::chrono::nanoseconds(NTime::ToNS(waitFor));
		didLock         = mLock.try_lock_shared_for(theTimeout);
	}



	// Restore fast reads
	if (didLock && !mFastReads && NTimeUtils::IsAfter(mInhibitTime))
	{
		mFastReads = true;
	}

	return didLock;
}





//=============================================================================
//		NSharedMutexBravo::UnlockShared : Release the shared lock.
//-----------------------------------------------------------------------------
inline void NSharedMutexBravo::UnlockShared()
{


	// Fast path
	size_t& theIndex = GetReaderIndex();
	if (theIndex != kNSizeMax)
	{
		auto& theReader = GetVisibleReaders().at(theIndex);
		NN_REQUIRE(theReader == this);

		theReader = nullptr;
		theIndex  = kNSizeMax;
	}


	// Slow path
	else
	{
		mLock.unlock_shared();
	}
}





//=============================================================================
//		NSharedMutexBravo::GetMutexIndex : Get the mutex index.
//-----------------------------------------------------------------------------
inline size_t NSharedMutexBravo::GetMutexIndex()
{


	// Get the mutex index
	uintptr_t a = NThreadID::Get().GetValue();
	uintptr_t b = a + uintptr_t(this);
	uintptr_t c = (a << 16) + (b >> 16);

	return size_t(c % kNSharedReaders);
}





//=============================================================================
//		NSharedMutexBravo::GetReaderIndex : Get the reader index.
//-----------------------------------------------------------------------------
inline size_t& NSharedMutexBravo::GetReaderIndex()
{


	// Get the reader index
	static thread_local size_t sIndex = kNSizeMax;

	return sIndex;
}





//=============================================================================
//		NSharedMutexBravo::GetVisibleReaders : Get the mutex table.
//-----------------------------------------------------------------------------
inline NVectorSharedMutexBravo& NSharedMutexBravo::GetVisibleReaders()
{


	// Get the readers
	static NVectorSharedMutexBravo sVisibleReaders(kNSharedReaders);

	return sVisibleReaders;
}



#pragma mark NSharedMutexPThread
#if NN_NSHAREDMUTEX_PTHREAD
//=============================================================================
//		NSharedMutexPThread::NSharedMutexPThread : Constructor.
//-----------------------------------------------------------------------------
inline NSharedMutexPThread::NSharedMutexPThread()
	: mLock()
{


	// Initliase ourselves
	int sysErr = pthread_rwlock_init(&mLock, nullptr);
	NN_REQUIRE_NOT_ERR(sysErr);
}





//=============================================================================
//		NSharedMutexPThread::~NSharedMutexPThread : Destructor.
//-----------------------------------------------------------------------------
inline NSharedMutexPThread::~NSharedMutexPThread()
{


	// Clean up
	int sysErr = pthread_rwlock_destroy(&mLock);
	NN_EXPECT_NOT_ERR(sysErr);
}





//=============================================================================
//		NSharedMutexPThread::Lock : Acquire the exclusive lock.
//-----------------------------------------------------------------------------
inline bool NSharedMutexPThread::Lock(NInterval waitFor)
{


	// Acquire the lock
	bool didLock = false;

	if (waitFor == kNTimeForever)
	{
		int sysErr = pthread_rwlock_wrlock(&mLock);
		NN_EXPECT_NOT_ERR(sysErr);

		didLock = (sysErr == 0);
	}
	else
	{
	#if NN_TARGET_ANDROID || NN_TARGET_LINUX
		struct timespec theTimeout = NTimeUtils::ToTimespec(waitFor);

		int sysErr = pthread_rwlock_timedwrlock(&mLock, &theTimeout);
		NN_EXPECT_NOT_ERR(sysErr);

		didLock = (sysErr == 0);
	#else
		NTime     endTime   = NTimeUtils::GetAbsolute(waitFor);
		NInterval sleepTime = kNTimeNanosecond;

		while (!didLock && !NTimeUtils::IsAfter(endTime))
		{
			int sysErr = pthread_rwlock_trywrlock(&mLock);
			didLock    = (sysErr == 0);

			if (!didLock)
			{
				NThread::Sleep(sleepTime);
				sleepTime = std::min(sleepTime * 1.25, kNSharedMaxSleep);
			}
		}
	#endif
	}

	return didLock;
}





//=============================================================================
//		NSharedMutexPThread::Unlock : Release the exclusive lock.
//-----------------------------------------------------------------------------
inline void NSharedMutexPThread::Unlock()
{


	// Release the lock
	int sysErr = pthread_rwlock_unlock(&mLock);
	NN_EXPECT_NOT_ERR(sysErr);
}





//=============================================================================
//		NSharedMutexPThread::LockShared : Acquire the shared lock.
//-----------------------------------------------------------------------------
inline bool NSharedMutexPThread::LockShared(NInterval waitFor)
{


	// Acquire the lock
	bool didLock = false;

	if (waitFor == kNTimeForever)
	{
		int sysErr = pthread_rwlock_rdlock(&mLock);
		NN_EXPECT_NOT_ERR(sysErr);

		didLock = (sysErr == 0);
	}
	else
	{
	#if NN_TARGET_ANDROID || NN_TARGET_LINUX
		struct timespec theTimeout = NTimeUtils::ToTimespec(waitFor);

		int sysErr = pthread_rwlock_timedrdlock(&mLock, &theTimeout);
		NN_EXPECT_NOT_ERR(sysErr);

		didLock = (sysErr == 0);
	#else
		NTime     endTime   = NTimeUtils::GetAbsolute(waitFor);
		NInterval sleepTime = kNTimeNanosecond;

		while (!didLock && !NTimeUtils::IsAfter(endTime))
		{
			int sysErr = pthread_rwlock_tryrdlock(&mLock);
			didLock    = (sysErr == 0);

			if (!didLock)
			{
				NThread::Sleep(sleepTime);
				sleepTime = std::min(sleepTime * 1.25, kNSharedMaxSleep);
			}
		}
	#endif
	}

	return didLock;
}





//=============================================================================
//		NSharedMutexPThread::UnlockShared : Release the shared lock.
//-----------------------------------------------------------------------------
inline void NSharedMutexPThread::UnlockShared()
{


	// Release the lock
	int sysErr = pthread_rwlock_unlock(&mLock);
	NN_REQUIRE_NOT_ERR(sysErr);
}
#endif // NN_NSHAREDMUTEX_PTHREAD





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
#if NN_TARGET_WINDOWS
using NSharedMutex                                          = NSharedMutexBravo;
#else
using NSharedMutex                                          = NSharedMutexPThread;
#endif // NN_TARGET_WINDOWS
