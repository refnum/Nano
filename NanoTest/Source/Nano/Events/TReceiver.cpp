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
static constexpr const char* kNTestMessage                  = "TestMessage";
static constexpr uint32_t    kNTestValue                    = 123;





//=============================================================================
//		Internal class declaration
//-----------------------------------------------------------------------------
class TestReceiverIncrement : public NReceiver
{
public:
	TestReceiverIncrement()
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
	TestReceiverIncrement               theReceiver;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StartReceiving/MessageValue")
{
	// Perform the test
	theReceiver.StartReceiving(kNTestMessage,
							   [&](const NBroadcast& theBroadcast)
							   {
								   REQUIRE(theBroadcast.GetMessage() == kNTestMessage);
								   REQUIRE(theBroadcast.GetValue().IsUInt32());
								   REQUIRE(theBroadcast.GetValue().GetUInt32() == kNTestValue);
								   theReceiver.Increment();
								   theReceiver.Increment();
							   });

										REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestMessage, kNTestValue);
										REQUIRE(theReceiver.GetValue() == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StartReceiving/Value")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestMessage,
	[&](const NAny& theValue)
	{
		REQUIRE(theValue.IsUInt32());
		REQUIRE(theValue.GetUInt32() == kNTestValue);
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StartReceiving")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestMessage,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StopReceiving/Message")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestMessage,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StopReceiving(kNTestMessage);

	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StopReceiving/All")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestMessage,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StopReceiving();

	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "Destructor")
{


	// Perform the test
	NBroadcaster::Send(kNTestMessage, kNTestValue);

	if constexpr (true)
	{
		TestReceiverIncrement scopedReceiver;

		scopedReceiver.StartReceiving(kNTestMessage,
		[&]()
		{
			scopedReceiver.Increment();
		});

		REQUIRE(scopedReceiver.GetValue() == 0);
		NBroadcaster::Send(kNTestMessage, kNTestValue);
		REQUIRE(scopedReceiver.GetValue() == 1);
	}

	NBroadcaster::Send(kNTestMessage, kNTestValue);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TReceiver, "StartStopStartReceiving")
{


	// Perform the test
	theReceiver.StartReceiving(kNTestMessage,
	[&]()
	{
		theReceiver.Increment();
	});

	REQUIRE(theReceiver.GetValue() == 0);
	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StopReceiving(kNTestMessage);

	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 1);

	theReceiver.StartReceiving(kNTestMessage,
	[&]()
	{
		theReceiver.Increment();
	});

	NBroadcaster::Send(kNTestMessage, kNTestValue);
	REQUIRE(theReceiver.GetValue() == 2);
}
