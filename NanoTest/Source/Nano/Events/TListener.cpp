/*	NAME:
		TListener.cpp

	DESCRIPTION:
		NListener tests.

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
#include "NListener.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TListener)
{
	NBroadcaster theBroadcaster;
	NListener    theListener;

	NFunction doEvent;
	uint32_t  numEvents;

	SETUP
	{
		numEvents = 0;
		doEvent   = std::bind(&Fixture_TListener::IncrementEvent, this);
	}

	void IncrementEvent()
	{
		numEvents++;
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TListener, "Default")
{


	// Perform the test
	theListener.StartListening(&theBroadcaster,
							   "apple",
							   []()
							   {
							   });

	theBroadcaster.Broadcast("apple");
	theBroadcaster.Broadcast("banana");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TListener, "Member")
{


	// Perform the test
	theListener.StartListening(&theBroadcaster, "apple", doEvent);
	REQUIRE(numEvents == 0);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TListener, "LambdaID")
{


	// Perform the test
	theListener.StartListening(&theBroadcaster,
							   "apple",
							   [&](const NString&)
							   {
								   IncrementEvent();
							   });
	REQUIRE(numEvents == 0);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TListener, "LambdaVoid")
{


	// Perform the test
	theListener.StartListening(&theBroadcaster,
							   "apple",
							   [&]()
							   {
								   IncrementEvent();
							   });
	REQUIRE(numEvents == 0);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TListener, "IgnoredEvent")
{


	// Perform the test
	theListener.StartListening(&theBroadcaster, "apple", doEvent);
	REQUIRE(numEvents == 0);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 1);

	theBroadcaster.Broadcast("banana");
	REQUIRE(numEvents == 1);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TListener, "MultipleEvents")
{


	// Perform the test
	theListener.StartListening(&theBroadcaster, "apple", doEvent);
	theListener.StartListening(&theBroadcaster, "banana", doEvent);
	REQUIRE(numEvents == 0);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 1);

	theBroadcaster.Broadcast("banana");
	REQUIRE(numEvents == 2);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TListener, "StopListening")
{


	// Perform the test
	theListener.StartListening(&theBroadcaster, "apple", doEvent);
	REQUIRE(numEvents == 0);

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 1);

	theListener.StopListening(&theBroadcaster, "apple");

	theBroadcaster.Broadcast("apple");
	REQUIRE(numEvents == 1);
}
