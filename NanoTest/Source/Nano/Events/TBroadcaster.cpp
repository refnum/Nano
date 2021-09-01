/*	NAME:
		TBroadcaster.cpp

	DESCRIPTION:
		NBroadcaster tests.

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
#include "NBroadcast.h"
#include "NBroadcaster.h"
#include "NReceiver.h"
#include "NTestFixture.h"
#include "NThread.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr const char* kNTestName                     = "TestBroadcast";
static constexpr uint32_t    kNTestValue                    = 123;
static const NBroadcast      kNTestBroadcast(kNTestName, kNTestValue);





//=============================================================================
//		Internal class declaration
//-----------------------------------------------------------------------------
class TestReceiverBroadcaster : public NReceiver
{
public:
	TestReceiverBroadcaster()
		: mValue(0)
	{
		StartReceiving(kNTestName,
					   [&](const NBroadcast& theBroadcast)
					   {
						   if (theBroadcast.GetValue().IsUInt32())
						   {
							   mValue += theBroadcast.GetValue().GetUInt32();
						   }
						   else
						   {
							   mValue++;
						   }
					   });
	}


	// Get the value
	uint32_t GetValue() const
	{
		return mValue;
	}

private:
	uint32_t                            mValue;
};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TBroadcaster)
{
	TestReceiverBroadcaster             theReceiver;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBroadcaster, "SendBroadcast")
{
	// Perform the test
										REQUIRE(theReceiver.GetValue() == 0);

	NBroadcaster::Send(kNTestName);
										REQUIRE(theReceiver.GetValue() == 1);

	NBroadcaster::Send(kNTestBroadcast);
										REQUIRE(theReceiver.GetValue() == 1 + kNTestValue);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBroadcaster, "SendBroadcastValue")
{


	// Perform the test
	REQUIRE(theReceiver.GetValue() == 0);

	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == kNTestValue);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBroadcaster, "SendAsyncBroadcast")
{


	// Perform the test
	REQUIRE(theReceiver.GetValue() == 0);

	NBroadcaster::SendAsync(kNTestName);
	NThread::Sleep(0.050);
	REQUIRE(theReceiver.GetValue() == 1);

	NBroadcaster::SendAsync(kNTestBroadcast);
	NThread::Sleep(0.050);
	REQUIRE(theReceiver.GetValue() == 1 + kNTestValue);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBroadcaster, "SendAsyncBroadcastValue")
{


	// Perform the test
	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::SendAsync(kNTestName, kNTestValue);
	NThread::Sleep(0.050);
	REQUIRE(theReceiver.GetValue() == kNTestValue);
}
