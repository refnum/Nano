/*	NAME:
		BSharedMutex.cpp

	DESCRIPTION:
		NSharedMutex benchmark.

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
#include "NData.h"
#include "NDataCompressor.h"
#include "NFunction.h"
#include "NMachine.h"
#include "NRandom.h"
#include "NSharedMutex.h"
#include "NTestFixture.h"
#include "NThread.h"

// System
#include <shared_mutex>

#if !NN_TARGET_WINDOWS
	#include <pthread.h>
#endif // !NN_TARGET_WINDOWS





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define NN_BENCHMARK_SHAREDMUTEX                            0
#define NN_BENCHMARK_SHAREDMUTEX_ALL                        0

NN_DIAGNOSTIC_IGNORE_CLANG("-Wformat-extra-args");
NN_DIAGNOSTIC_IGNORE_CLANG("-Wunused-template");





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kSizeSeeds                          = 100;
static constexpr size_t kSizeBlock                          = 1 * kNKilobyte;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
template<typename T>
struct WorkState
{
	T                theLock;
	size_t           theLoad;
	NVectorUInt64    theSeeds;
	std::atomic_bool startWork;
	size_t           percentRead;
};





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class Work
{
public:
	Work(WorkState<T>& theState)
		: mData(kSizeBlock, nullptr, NDataSource::None)
		, mRandom(1)
		, mState(theState)
	{
	}

	void Execute()
	{
		while (!mState.startWork)
		{
			NThread::Switch();
		}

		for (size_t n = 0; n < mState.theLoad; n++)
		{
			if (mRandom.NextUInt8(0, 100) <= mState.percentRead)
			{
				ExecuteRead();
			}
			else
			{
				ExecuteWrite();
			}
		}
	}


private:
	void ExecuteRead()
	{
		mState.theLock.LockShared();

		for (const auto theSeed : mState.theSeeds)
		{
			mRandom.SetSeed(theSeed);
			mRandom.NextData(mData.GetSize(), mData.GetMutableData());

			(void) NDataCompressor::Compress(NCompression::ZLib, mData);
		}

		mState.theLock.UnlockShared();
	}

	void ExecuteWrite()
	{
		mState.theLock.Lock();

		for (auto& theSeed : mState.theSeeds)
		{
			theSeed = NRandom::GetUInt64();
		}

		mState.theLock.Unlock();
	}



private:
	NData                               mData;
	NRandom                             mRandom;
	WorkState<T>&                       mState;
};





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class LockNull
{
public:
	static constexpr const char* Name = "null";

	void Lock()
	{
	}

	void Unlock()
	{
	}

	void LockShared()
	{
	}

	void UnlockShared()
	{
	}

private:
};





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class LockNSharedMutex
{
public:
	static constexpr const char* Name = "NSharedMutex";

	void Lock()
	{
		bool didLock = mLock.Lock();
		NN_REQUIRE(didLock);
	}

	void Unlock()
	{
		mLock.Unlock();
	}

	void LockShared()
	{
		bool didLock = mLock.LockShared();
		NN_REQUIRE(didLock);
	}

	void UnlockShared()
	{
		mLock.UnlockShared();
	}

private:
	NSharedMutex                        mLock;
};





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class LockStdMutex
{
public:
	static constexpr const char* Name = "std::mutex";

	void Lock()
	{
		mLock.lock();
	}

	void Unlock()
	{
		mLock.unlock();
	}

	void LockShared()
	{
		mLock.lock();
	}

	void UnlockShared()
	{
		mLock.unlock();
	}

private:
	std::mutex                          mLock;
};





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class LockStdSharedMutex
{
public:
	static constexpr const char* Name = "std::shared_mutex";

	void Lock()
	{
		mLock.lock();
	}

	void Unlock()
	{
		mLock.unlock();
	}

	void LockShared()
	{
		mLock.lock_shared();
	}

	void UnlockShared()
	{
		mLock.unlock_shared();
	}

private:
	std::shared_mutex                   mLock;
};



#if NN_TARGET_WINDOWS
//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class LockSRWLock
{
public:
	static constexpr const char* Name = "SRWLock";

	LockSRWLock()
	{
		InitializeSRWLock(&mLock);
	}

	void Lock()
	{
		AcquireSRWLockExclusive(&mLock);
	}

	void Unlock()
	{
		ReleaseSRWLockExclusive(&mLock);
	}

	void LockShared()
	{
		AcquireSRWLockShared(&mLock);
	}

	void UnlockShared()
	{
		ReleaseSRWLockShared(&mLock);
	}

private:
	SRWLOCK                             mLock;
};
#endif // NN_TARGET_WINDOWS



#if !NN_TARGET_WINDOWS
//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class LockPThread
{
public:
	static constexpr const char* Name = "pthread_rwlock";

	LockPThread()
	{
		int sysErr = pthread_rwlock_init(&mLock, nullptr);
		NN_REQUIRE_NOT_ERR(sysErr);
	}

	~LockPThread()
	{
		int sysErr = pthread_rwlock_destroy(&mLock);
		NN_REQUIRE_NOT_ERR(sysErr);
	}

	void Lock()
	{
		int sysErr = pthread_rwlock_wrlock(&mLock);
		NN_REQUIRE_NOT_ERR(sysErr);
	}

	void Unlock()
	{
		int sysErr = pthread_rwlock_unlock(&mLock);
		NN_REQUIRE_NOT_ERR(sysErr);
	}

	void LockShared()
	{
		int sysErr = pthread_rwlock_rdlock(&mLock);
		NN_REQUIRE_NOT_ERR(sysErr);
	}

	void UnlockShared()
	{
		int sysErr = pthread_rwlock_unlock(&mLock);
		NN_REQUIRE_NOT_ERR(sysErr);
	}

private:
	pthread_rwlock_t                    mLock;
};
#endif // !NN_TARGET_WINDOWS





//=============================================================================
//		ExecuteWork : Execute the work.
//-----------------------------------------------------------------------------
template<typename T>
static void ExecuteWork(size_t numThreads, size_t percentRead)
{
	// Prepare the state
	WorkState<T>                        theState;

	theState.theLoad =                  20;
	theState.theSeeds.resize(kSizeSeeds, 999);
	theState.startWork   =              false;
	theState.percentRead =              percentRead;


	// Perform the work
	BENCHMARK_ADVANCED(T::Name)(Catch::Benchmark::Chronometer theMeter)
	{
		std::vector<NUniqueThread> theThreads;

		for (size_t n = 0; n < numThreads; n++)
		{
			theThreads.push_back(std::move(NThread::Create("BSharedMutex",
														   [&]()
														   {
															   Work<T> theWork(theState);

															   theWork.Execute();
														   })));
		}

		theMeter.measure(
			[&]
			{
				theState.startWork = true;

				for (auto& theThread : theThreads)
				{
					theThread->WaitForCompletion();
				}
			});
	};
}





//=============================================================================
//		PerformBenchmark : Perform a benchmark.
//-----------------------------------------------------------------------------
static void PerformBenchmark(size_t numThreads, size_t percentRead)
{


	// Perform the benchmark
	if constexpr (NN_BENCHMARK_SHAREDMUTEX || NN_BENCHMARK_SHAREDMUTEX_ALL)
	{
		ExecuteWork<LockStdSharedMutex>(numThreads, percentRead);
		ExecuteWork<LockNSharedMutex>(numThreads, percentRead);

		if constexpr (NN_BENCHMARK_SHAREDMUTEX_ALL)
		{
			ExecuteWork<LockNull>(numThreads, percentRead);
			ExecuteWork<LockStdMutex>(numThreads, percentRead);
		}

#if NN_TARGET_WINDOWS
		ExecuteWork<LockSRWLock>(numThreads, percentRead);
#else
		ExecuteWork<LockPThread>(numThreads, percentRead);
#endif // NN_TARGET_WINDOWS
	}
}





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(BSharedMutex){};



#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "2Threads_10PercentRead")
{


	// Perform the test
	PerformBenchmark(2, 10);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL


#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "2Threads_50PercentRead")
{


	// Perform the test
	PerformBenchmark(2, 50);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL


#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "2Threads_90PercentRead")
{


	// Perform the test
	PerformBenchmark(2, 90);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL



#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "8Threads_10PercentRead")
{


	// Perform the test
	PerformBenchmark(8, 10);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "8Threads_50PercentRead")
{


	// Perform the test
	PerformBenchmark(8, 50);
}


#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "8Threads_90PercentRead")
{


	// Perform the test
	PerformBenchmark(8, 90);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL



#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "16Threads_10PercentRead")
{


	// Perform the test
	PerformBenchmark(16, 10);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL


#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "16Threads_50PercentRead")
{


	// Perform the test
	PerformBenchmark(16, 50);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL


#if NN_BENCHMARK_SHAREDMUTEX_ALL
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSharedMutex, "16Threads_90PercentRead")
{


	// Perform the test
	PerformBenchmark(16, 90);
}
#endif // NN_BENCHMARK_SHAREDMUTEX_ALL
