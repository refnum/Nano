/*	NAME:
		TBroadcaster.cpp

	DESCRIPTION:
		NBroadcaster tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NListener.h"

#include "CTestListener.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NBROADCASTER(...)										TEST_NANO(TBroadcaster, ##__VA_ARGS__)

FIXTURE_NANO(TBroadcaster)
{
	NBroadcaster	theBroadcaster;
	CTestListener	theListener;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("Default")
{


	// Perform the test
	REQUIRE( theBroadcaster.IsBroadcasting());
	REQUIRE(!theBroadcaster.HasListeners());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("Single")
{


	// Perform the test
	theBroadcaster.AddListener(&theListener);
	REQUIRE(theBroadcaster.HasListeners());

	theBroadcaster.RemoveListener(&theListener);
	REQUIRE(!theBroadcaster.HasListeners());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("Multiple")
{	CTestListener	theListener2;



	// Perform the test
	theBroadcaster.AddListener(&theListener);
	theBroadcaster.AddListener(&theListener2);
	REQUIRE(theBroadcaster.HasListeners());

	theBroadcaster.RemoveListeners();
	REQUIRE(!theBroadcaster.HasListeners());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("Active")
{	uint32_t		theValue;



	// Perform the test
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.AddListener(&theListener);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	REQUIRE(theValue == kTestMsgSetUInt32);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("Inactive")
{	uint32_t		theValue;



	// Perform the test
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.SetBroadcasting(false);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);
	theBroadcaster.SetBroadcasting(true);

	REQUIRE(theValue == kTestMsgNone);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("Ignored")
{	uint32_t		theValue;



	// Perform the test
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	REQUIRE(theValue == kTestMsgNone);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("Copy")
{	NBroadcaster	theBroadcaster2;
	uint32_t		theValue;



	// Perform the test
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.AddListener(&theListener);
	
	theBroadcaster2 = theBroadcaster;
	theBroadcaster.RemoveListeners();
	
	theBroadcaster2.BroadcastMessage(kTestMsgSetUInt32);

	REQUIRE(theValue == kTestMsgSetUInt32);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("RemoveActive")
{


	// Perform the test
	theListener.SetTargetListener(&theListener);

	theBroadcaster.AddListener(&theListener);
	theBroadcaster.BroadcastMessage(kTestMsgRemoveListener);
	
	REQUIRE(!theBroadcaster.HasListeners());
	REQUIRE(!theListener.IsListeningTo());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBROADCASTER("RemoveDifferent")
{	CTestListener	theListener2;



	// Perform the test
	//
	// We can't control which order the broadcaster visits its listeners in, but
	// since each listener removes the other we should be left with a broadcaster
	// that listens to one listener rather than both.
	//
	// The first one visited will remove the other, verifying that the broadcaster
	// updated its list of listeners even if broadcasting caused a listener to be
	// removed from that list.
	//
	//
	// Before this bug was fixed that update did not occur, which meant that the
	// broadcaster could visit listeners that had been removed from it by a
	// previously visited listener.
	//
	// If that listener removal was due to destruction, the broadcaster would use
	// a stale pointer and could crash.
	theListener.SetTargetListener( &theListener2);
	theListener2.SetTargetListener(&theListener);

	theBroadcaster.AddListener(&theListener);
	theBroadcaster.AddListener(&theListener2);
	theBroadcaster.BroadcastMessage(kTestMsgRemoveListener);

	REQUIRE(theBroadcaster.HasListeners());
	REQUIRE((theListener.IsListeningTo() || theListener2.IsListeningTo()));
}







