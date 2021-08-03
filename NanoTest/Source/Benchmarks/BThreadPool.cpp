/*	NAME:
		BThreadPool.cpp

	DESCRIPTION:
		NThreadPool benchmark.

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
#include "NTestFixture.h"
#include "NThreadPool.h"

// System
#include <future>

#if NN_PLATFORM_DARWIN
	#include <dispatch/dispatch.h>
#endif





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define NN_BENCHMARK_THREADPOOL                             0





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kTasksSmall                         = 1000;
static constexpr size_t kTasksLarge                         = kTasksSmall * 20;

static constexpr size_t kWorkSmall                          = 5000;
static constexpr size_t kWorkLarge                          = kWorkSmall * 10;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
struct alignas(64) WorkState
{
	WorkState(size_t n)
		: numIters(n)
		, theValue(1.0)
	{
	}

	size_t     numIters;
	float64_t  theValue;
	NSemaphore isDone;
};





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		ExecuteWork : Execute the work on a thread.
//-----------------------------------------------------------------------------
static void ExecuteWork(WorkState* theState)
{
	// Calculate Pi
	for (size_t n = 1; n < theState->numIters; n++)
	{
		if ((n % 2) == 1)
		{
			theState->theValue -= (1.0 / ((2.0 * n) + 1));
		}
		else
		{
			theState->theValue += (1.0 / ((2.0 * n) + 1));
		}
	}

	theState->theValue *= 4.0;
	theState->isDone.Signal();
}





//=============================================================================
//		CreateWork : Create and execute some volume of work.
//-----------------------------------------------------------------------------
template<typename F>
size_t CreateWork(size_t numTasks, size_t numIters, const F& theFunction)
{


	// Create the work
	std::vector<WorkState> workState;

	for (size_t n = 0; n < numTasks; n++)
	{
		workState.emplace_back(WorkState(numIters));
	}



	// Execute it
	for (auto& theWork : workState)
	{
		theFunction(&theWork);
	}



	// Wait for completion
	for (auto& theWork : workState)
	{
		theWork.isDone.Wait();
	}

	return numTasks;
}





//=============================================================================
//		PerformBenchmark : Perform a benchmark.
//-----------------------------------------------------------------------------
static void PerformBenchmark(size_t numTasks, size_t numIters)
{


	// Perform the benchmark
	BENCHMARK("NThreadPool")
	{
		return CreateWork(numTasks,
						  numIters,
						  [&](WorkState* theWork)
						  {
							  NThreadPool::GetMain()->Add(
								  [=]()
								  {
									  ExecuteWork(theWork);
								  });
						  });
	};


#if NN_TARGET_WINDOWS
	BENCHMARK("std::async")
	{
		return CreateWork(numTasks,
						  numIters,
						  [=](WorkState* theWork)
						  {
							  std::async(
								  [=]()
								  {
									  ExecuteWork(theWork);
								  });
						  });
	};
#endif // NN_TARGET_WINDOWS


#if NN_PLATFORM_DARWIN
	BENCHMARK("GCD")
	{
		auto theQueue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);

		return CreateWork(numTasks,
						  numIters,
						  [=](WorkState* theWork)
						  {
							  dispatch_async_f(
								  theQueue,
								  theWork,
								  [](void* userData)
								  {
									  ExecuteWork(reinterpret_cast<WorkState*>(userData));
								  });
						  });
	};
#endif // NN_PLATFORM_DARWIN
}





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(BThreadPool){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BThreadPool, "SmallTasksSmallWork")
{


	// Perform the test
	if (NN_BENCHMARK_THREADPOOL)
	{
		PerformBenchmark(kTasksSmall, kWorkSmall);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BThreadPool, "SmalTasksLargeWork")
{


	// Perform the test
	if (NN_BENCHMARK_THREADPOOL)
	{
		PerformBenchmark(kTasksSmall, kWorkLarge);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BThreadPool, "LargeTasksSmallWork")
{


	// Perform the test
	if (NN_BENCHMARK_THREADPOOL)
	{
		PerformBenchmark(kTasksLarge, kWorkSmall);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BThreadPool, "LargeTasksLargeWork")
{


	// Perform the test
	if (NN_BENCHMARK_THREADPOOL)
	{
		PerformBenchmark(kTasksLarge, kWorkLarge);
	}
}
