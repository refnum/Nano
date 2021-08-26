/*	NAME:
		TProgressable.cpp

	DESCRIPTION:
		NProgressable tests.

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
#include "NProgressable.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TProgressable)
{
	NProgressable theProgress;

	SETUP
	{
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TProgressable, "Default")
{


	// Perform the test
	size_t taskIndex = 99999;
	size_t numTasks  = 99999;

	REQUIRE(theProgress.GetProgressFunction() == nullptr);
	REQUIRE(theProgress.GetProgressInterval() > 0.0);
	REQUIRE(theProgress.GetProgressInterval() < 1.0);

	theProgress.GetProgressTasks(taskIndex, numTasks);
	REQUIRE(taskIndex == 0);
	REQUIRE(numTasks == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TProgressable, "GetProgressFunction")
{


	// Perform the test
	theProgress.SetProgressFunction(
	[](NProgress, float)
	{
		return NStatus::OK;
	});

	REQUIRE(theProgress.GetProgressFunction() != nullptr);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TProgressable, "GetProgressInterval")
{


	// Perform the test
	theProgress.SetProgressInterval(100.0);
	REQUIRE(theProgress.GetProgressInterval() == 100.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TProgressable, "GetProgressTasks")
{


	// Perform the test
	constexpr size_t kNumTasks = 20;

	for (size_t n = 0; n < kNumTasks; n++)
	{
		size_t taskIndex = 9999;
		size_t numTasks  = 9999;

		theProgress.SetProgressTasks(n, kNumTasks);
		theProgress.GetProgressTasks(taskIndex, numTasks);

		REQUIRE(taskIndex == n);
		REQUIRE(numTasks == kNumTasks);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TProgressable, "UpdateProgress")
{


	// Get the state we need
	size_t numBegin  = 0;
	size_t numUpdate = 0;
	size_t numEnd    = 0;

	theProgress.SetProgressFunction(
	[&](NProgress theState, float)
	{
		switch (theState)
		{
			case NProgress::Begin:
				numBegin++;
				break;
			case NProgress::Update:
				numUpdate++;
				break;
			case NProgress::End:
				numEnd++;
				break;
		}

		return NStatus::OK;
	});



	// Perform the test
	theProgress.SetProgressInterval(0.0);
	theProgress.BeginProgress();

	for (float n = 0.0f; n < 0.5f; n += 0.1f)
	{
		theProgress.UpdateProgress(n);
	}

	for (size_t n = 0; n < 5; n++)
	{
		theProgress.UpdateProgress(n, 5);
	}

	theProgress.EndProgress();

	REQUIRE(numBegin == 1);
	REQUIRE(numUpdate == 10);
	REQUIRE(numEnd == 1);
}
