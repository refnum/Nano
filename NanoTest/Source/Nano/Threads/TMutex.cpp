/*	NAME:
		TMutex.cpp

	DESCRIPTION:
		NMutex tests.

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
#include "NClock.h"
#include "NMutex.h"
#include "NScopedLock.h"
#include "NTestFixture.h"
#include "NThread.h"





//=============================================================================
//		Defines
//-----------------------------------------------------------------------------
#define BENCHMARK_NMUTEX                                    0





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
template<typename MutexType, size_t kNumIters, size_t kNumThreads>
NInterval BenchmarkMutex(std::function<void(MutexType&)> lockFunc,
						 std::function<void(MutexType&)> unlockFunc)
{


	// Create the threads
	std::atomic_bool     startFlag = false;
	std::vector<NThread> theThreads;
	MutexType            theMutexes[kNumThreads];

	for (size_t t = 0; t < kNumThreads; t++)
	{
		theThreads.emplace_back(
			NThread("BenchmarkMutex",
			[&lockFunc, &unlockFunc, &startFlag, &theMutex = theMutexes[t]]()
			{
				// Wait for the flag
				std::mt19937 randomWork{std::random_device{}()};

				while (!startFlag)
				{
					randomWork();
				}


				// Do the work
				for (size_t n = 0; n < kNumIters; n++)
				{
					lockFunc(theMutex);

					while (randomWork() % 4)
					{
					}

					unlockFunc(theMutex);
				}
			}));
	}



	// Time the work
	NThread::Sleep(0.5);

	NClock theClock;
	startFlag = true;

	for (auto& theThread : theThreads)
	{
		theThread.WaitForCompletion();
	}

	return theClock.Stop();
}





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMutex)
{
	NMutex theLock;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMutex, "Default")
{


	// Perform the test
	REQUIRE(!theLock.IsLocked());
	theLock.Lock();

	REQUIRE(theLock.IsLocked());

	theLock.Unlock();
	REQUIRE(!theLock.IsLocked());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMutex, "Scoped")
{


	// Perform the test
	REQUIRE(!theLock.IsLocked());

	if (true)
	{
		NScopedLock acquireLock(theLock);
		REQUIRE(theLock.IsLocked());
	}

	REQUIRE(!theLock.IsLocked());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMutex, "Recursion")
{


	// Perform the test
	REQUIRE(!theLock.IsLocked());
	theLock.Lock();

	REQUIRE(theLock.IsLocked());
	theLock.Lock();

	theLock.Unlock();
	REQUIRE(theLock.IsLocked());

	theLock.Unlock();
	REQUIRE(!theLock.IsLocked());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMutex, "Benchmark/Uncontended")
{
#if BENCHMARK_NMUTEX


	// Perform the test
	constexpr size_t kNumIters   = 100000000;
	constexpr size_t kNumThreads = 1;

	printf("TMutex/Benchmark/Contended\n");
	printf("%zu threads, %zu Iterations\n", kNumThreads, kNumIters);


	NInterval timeNMutex = BenchmarkMutex<NMutex, kNumIters, kNumThreads>(
	[](NMutex& theLock)
	{
		theLock.Lock();
	},
		[](NMutex& theLock)
		{
			theLock.Unlock();
		});

	printf("  NMutex     = %.6f\n", timeNMutex);


	NInterval timeStdMutex = BenchmarkMutex<std::mutex, kNumIters, kNumThreads>(
	[](std::mutex& theLock)
	{
		theLock.lock();
	},
		[](std::mutex& theLock)
		{
			theLock.unlock();
		});

	printf("  std::mutex = %.6f\n", timeStdMutex);

#endif
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMutex, "Benchmark/Contended")
{
#if BENCHMARK_NMUTEX


	// Perform the test
	constexpr size_t kNumIters   = 10000000;
	constexpr size_t kNumThreads = 4;

	printf("TMutex/Benchmark/Contended\n");
	printf("%zu threads, %zu Iterations\n", kNumThreads, kNumIters);


	NInterval timeNMutex = BenchmarkMutex<NMutex, kNumIters, kNumThreads>(
	[](NMutex& theLock)
	{
		theLock.Lock();
	},
		[](NMutex& theLock)
		{
			theLock.Unlock();
		});

	printf("  NMutex     = %.6f\n", timeNMutex);


	NInterval timeStdMutex = BenchmarkMutex<std::mutex, kNumIters, kNumThreads>(
	[](std::mutex& theLock)
	{
		theLock.lock();
	},
		[](std::mutex& theLock)
		{
			theLock.unlock();
		});

	printf("  std::mutex = %.6f\n", timeStdMutex);

#endif
}
