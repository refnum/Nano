/*	NAME:
		TThread.cpp

	DESCRIPTION:
		NThread tests.

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
#include "NThread.h"
#include "NTimeUtils.h"





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TThread){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "GetID")
{


	// Perform the test
	NThread theThread("TThread_GetID");

	REQUIRE(theThread.GetID().IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "GetRunLoop")
{


	// Perform the test
	NThread theThread2("TThread_GetRunLoop", []() {
		NThread* currentThread = NThread::GetCurrent();

		REQUIRE(currentThread->GetRunLoop() != nullptr);
		REQUIRE(currentThread->GetRunLoop() != NRunLoop::GetMain());
		REQUIRE(currentThread->GetRunLoop() == NRunLoop::GetCurrent());
	});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "WaitForCompletion")
{


	// Perform the test
	NThread theThread("TThread_WaitForCompletion", []() {
		NThread::Sleep(0.050);
	});

	REQUIRE(!theThread.IsComplete());
	theThread.WaitForCompletion();
	REQUIRE(theThread.IsComplete());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "RequestStop")
{


	// Perform the test
	NThread theThread("TThread_RequestStop", []() {
		NThread* currentThread = NThread::GetCurrent();

		while (!currentThread->ShouldStop())
		{
			NThread::Sleep(0.001);
		}
	});

	REQUIRE(!theThread.IsComplete());
	theThread.RequestStop();
	NThread::Sleep(0.100);
	REQUIRE(theThread.IsComplete());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "IsMain")
{


	// Perform the test
	REQUIRE(NThread::IsMain());

	NThread theThread("TThread_IsMain", []() {
		REQUIRE(!NThread::IsMain());
	});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "GetCurrent")
{


	// Perform the test
	REQUIRE(NThread::GetCurrent() == nullptr);

	NThread theThread2("TThread_GetCurrent", []() {
		REQUIRE(NThread::GetCurrent() != nullptr);
	});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "StackSize")
{


	// Perform the test
	size_t theSize = NThread::GetStackSize();
	REQUIRE(theSize != 0);

	NThread theThread("TThread_StackSize", size_t(300 * kNMebibyte), []() {
		size_t customSize = NThread::GetStackSize();
		REQUIRE(customSize >= 295 * kNMebibyte);
	});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "Name")
{


	// Perform the test
	REQUIRE((NThread::GetName().IsEmpty() || NN_TARGET_LINUX));

	NThread::SetName("Test");
	REQUIRE(NThread::GetName() == "Test");

	NThread::SetName("");
	REQUIRE(NThread::GetName().IsEmpty());

	NThread theThread("TestName", []() {
		REQUIRE(NThread::GetName() == "TestName");
	});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "Priority")
{


	// Perform the test
	//
	// Linux requires additional privileges to change thread priority.
	float thePriority = NThread::GetPriority();
	REQUIRE(thePriority == 0.5f);

	if (!NN_TARGET_LINUX)
	{
		NThread::SetPriority(0.0f);
		thePriority = NThread::GetPriority();
		REQUIRE(thePriority == 0.0f);

		NThread::SetPriority(1.0f);
		thePriority = NThread::GetPriority();
		REQUIRE(thePriority == 1.0f);

		NThread::SetPriority(0.5f);
		thePriority = NThread::GetPriority();
		REQUIRE(thePriority == 0.5f);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "Cores")
{


	// Perform the test
	NVectorUInt8 theCores = NThread::GetCores();
	REQUIRE(theCores.empty());

	NThread::SetCores({0});
	theCores = NThread::GetCores();
	REQUIRE(!theCores.empty());

	NThread::SetCores({});
	theCores = NThread::GetCores();
	REQUIRE(theCores.empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "Sleep")
{


	// Perform the test
	NTime timeBefore = NTimeUtils::GetTime();
	NThread::Sleep(0.100);
	NTime timeAfter = NTimeUtils::GetTime();

	REQUIRE(timeAfter >= (timeBefore + 0.100));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "Switch")
{


	// Perform the test
	NThread::Switch();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "Pause")
{


	// Perform the test
	NThread::Pause();
}
