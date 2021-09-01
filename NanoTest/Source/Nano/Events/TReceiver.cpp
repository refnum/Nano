/*	NAME:
		TReceiver.cpp

	DESCRIPTION:
		NReceiver tests.

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





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr const char* kNTestName                     = "TestBroadcast";
static constexpr uint32_t    kNTestValue                    = 123;





//=============================================================================
//		Internal class declaration
//-----------------------------------------------------------------------------
class TestReceiverReceiver : public NReceiver
{
public:
	TestReceiverReceiver()
		: mValue(0)
	{
	}


	// Increment the value
	void Increment()
	{
		mValue++;
	}


	// Get the value
	uint32_t GetValue()
	{
		return mValue;
	}


private:
	uint32_t                            mValue;
};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TReceiver)
{
	TestReceiverReceiver                theReceiver;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "GetBroadcasts")
{
	// Perform the test
										REQUIRE(theReceiver.GetBroadcasts().empty());

	theReceiver.StartReceiving("test.1",
							   [&]()
							   {
							   });
										REQUIRE(theReceiver.GetBroadcasts().size() == 1);

	theReceiver.StartReceiving("test.2",
							   [&]()
							   {
							   });
										REQUIRE(theReceiver.GetBroadcasts().size() == 2);

	theReceiver.StopReceiving();
										REQUIRE(theReceiver.GetBroadcasts().empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StartReceiving/Broadcast")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestName,
	[&](const NBroadcast& theBroadcast)
	{
		REQUIRE(theBroadcast.GetName() == kNTestName);
		REQUIRE(theBroadcast.GetValue().IsUInt32());
		REQUIRE(theBroadcast.GetValue().GetUInt32() == kNTestValue);
		theReceiver.Increment();
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StartReceiving/Void")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestName,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StopReceiving/One")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestName,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StopReceiving(kNTestName);

	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StopReceiving/All")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestName,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StopReceiving();

	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "Destructor")
{


	// Perform the test
	NBroadcaster::Send(kNTestName, kNTestValue);

	if constexpr (true)
	{
		TestReceiverReceiver scopedReceiver;

		scopedReceiver.StartReceiving(kNTestName,
		[&]()
		{
			scopedReceiver.Increment();
		});

		REQUIRE(scopedReceiver.GetValue() == 0);
		NBroadcaster::Send(kNTestName, kNTestValue);
		REQUIRE(scopedReceiver.GetValue() == 1);
	}

	NBroadcaster::Send(kNTestName, kNTestValue);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StartStopStartReceiving")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestName,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StopReceiving(kNTestName);

	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StartReceiving(kNTestName,
	[&]()
	{
		theReceiver.Increment();
	});

	NBroadcaster::Send(kNTestName, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 2);
}
