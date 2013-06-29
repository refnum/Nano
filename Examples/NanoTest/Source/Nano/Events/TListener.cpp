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
#define TEST_NLISTENER(_name, _desc)								TEST_NANO(TListener, _name, _desc)

FIXTURE_NANO(TListener)
{
	NBroadcaster	theBroadcaster;
	CTestListener	theListener;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLISTENER("Default", "Default state")
{


	// Perform the test
	REQUIRE( theListener.IsListening());
	REQUIRE(!theListener.IsListeningTo());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLISTENER("Single", "Single broadcaster")
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
TEST_NLISTENER("Multiple", "Multiple broadcasters")
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
TEST_NLISTENER("Active", "Active listener")
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
TEST_NLISTENER("Inactive", "Inactive listener")
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
TEST_NLISTENER("Copy", "Copy broadcasters")
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




