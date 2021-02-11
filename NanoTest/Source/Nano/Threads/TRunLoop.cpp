/*	NAME:
		TRunLoop.cpp

	DESCRIPTION:
		NRunLoop tests.

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
#include "NRunLoop.h"
#include "NTestFixture.h"
#include "NThread.h"
#include "NTimeUtils.h"





//=============================================================================
//		InteranlFunctions
//-----------------------------------------------------------------------------
static void TestIncrement(size_t* theValue)
{


	// Update the value
	*theValue = *theValue + 1;
}





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TRunLoop)
{
	std::shared_ptr<NRunLoop> runLoop;
	NRunLoopWorkID            workID;
	size_t                    theValue;

	SETUP
	{
		runLoop  = NRunLoop::GetCurrent();
		workID   = NRunLoopWorkNone;
		theValue = 0;
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRunLoop, "GetMain")
{


	// Perform the teest
	REQUIRE(runLoop == NRunLoop::GetMain());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRunLoop, "GetCurrent")
{


	// Perform the test
	NSemaphore theSemaphore;
	bool       isMain = true;

	NThread theThread("TRunLoop_GetCurrent", [&]() {
		isMain = (NRunLoop::GetCurrent() == NRunLoop::GetMain());
		theSemaphore.Signal();
	});

	theSemaphore.Wait();
	REQUIRE(!isMain);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRunLoop, "Stop")
{


	// Perform the test
	NThread theThread("NRunLoop");

	std::shared_ptr<NRunLoop> threadLoop = theThread.GetRunLoop();
	threadLoop->Add(std::bind(TestIncrement, &theValue), kNTimeMillisecond, kNTimeMillisecond);

	runLoop->Run(kNTimeMillisecond * 100);
	threadLoop->Stop();
	size_t stoppedValue = theValue;

	runLoop->Run(kNTimeMillisecond * 100);
	REQUIRE(theValue == stoppedValue);
	REQUIRE(theValue >= 5);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRunLoop, "Add")
{


	// Perform the test
	workID = runLoop->Add(std::bind(TestIncrement, &theValue));
	runLoop->Run(kNTimeNone);
	REQUIRE(theValue == 1);

	workID = runLoop->Add(std::bind(TestIncrement, &theValue));
	runLoop->Run(kNTimeNanosecond);
	REQUIRE(theValue == 2);

	workID = runLoop->Add(std::bind(TestIncrement, &theValue));
	runLoop->Run(kNTimeMillisecond);
	REQUIRE(theValue == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRunLoop, "Remove")
{


	// Perform the test
	workID = runLoop->Add(std::bind(TestIncrement, &theValue), kNTimeHour);
	REQUIRE(workID != NRunLoopWorkNone);

	runLoop->Remove(workID);
	REQUIRE(theValue == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRunLoop, "GetWorkTime")
{


	// Perform the test
	workID = runLoop->Add(std::bind(TestIncrement, &theValue), kNTimeHour);
	runLoop->SetWorkTime(workID, NTimeUtils::GetTime() + 10.0);

	NTime workTime = runLoop->GetWorkTime(workID);
	REQUIRE(workTime <= (NTimeUtils::GetTime() + 10.0));

	runLoop->Remove(workID);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRunLoop, "GetWorkInterval")
{


	// Perform the test
	workID = runLoop->Add(std::bind(TestIncrement, &theValue), kNTimeHour, kNTimeHour);
	runLoop->SetWorkInterval(workID, 10.0);

	NInterval workInterval = runLoop->GetWorkInterval(workID);
	REQUIRE(workInterval == 10.0);

	runLoop->Remove(workID);
}
