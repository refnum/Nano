/*	NAME:
		TListener.cpp

	DESCRIPTION:
		NListener tests.

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
#define TEST_NLISTENER(...)											TEST_NANO(TListener, ##__VA_ARGS__)

FIXTURE_NANO(TListener)
{
	NBroadcaster	theBroadcaster;
	CTestListener	theListener;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLISTENER("Default")
{


	// Perform the test
	REQUIRE( theListener.IsListening());
	REQUIRE(!theListener.IsListeningTo());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLISTENER("Single")
{


	// Perform the test
	theBroadcaster.AddListener(&theListener);
	REQUIRE(theListener.IsListeningTo());
	REQUIRE(theListener.IsListeningTo(&theBroadcaster));
	
	theBroadcaster.RemoveListener(&theListener);
	REQUIRE(!theListener.IsListeningTo());
	REQUIRE(!theListener.IsListeningTo(&theBroadcaster));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLISTENER("Multiple")
{	NBroadcaster	theBroadcaster2;



	// Perform the test
	theBroadcaster.AddListener( &theListener);
	theBroadcaster2.AddListener(&theListener);

	REQUIRE(theListener.IsListeningTo());
	REQUIRE(theListener.IsListeningTo(&theBroadcaster));
	REQUIRE(theListener.IsListeningTo(&theBroadcaster2));
	
	theListener.RemoveFromBroadcasters();
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLISTENER("Active")
{	UInt32	theValue;



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
TEST_NLISTENER("Inactive")
{	UInt32	theValue;



	// Perform the test
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theListener.SetListening(false);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);
	theListener.SetListening(true);

	REQUIRE(theValue == kTestMsgNone);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLISTENER("Copy")
{	CTestListener	theListener2;
	UInt32			theValue;



	// Perform the test
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.AddListener(&theListener);
	
	theListener2 = theListener;
	theListener.RemoveFromBroadcasters();
	
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	REQUIRE(theValue == kTestMsgSetUInt32);
}




