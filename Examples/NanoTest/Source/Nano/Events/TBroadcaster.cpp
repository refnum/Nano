/*	NAME:
		TBroadcaster.cpp

	DESCRIPTION:
		NBroadcaster tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NListener.h"

#include "CTestListener.h"
#include "TBroadcaster.h"





//============================================================================
//		TBroadcaster::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TBroadcaster::Execute(void)
{	NBroadcaster		theBroadcaster, theBroadcaster2;
	CTestListener		theListener, theListener2;
	BroadcastMsg		theMsg;



	// Initialise ourselves
	theMsg = kTestMsgNone;
	theListener.SetTarget(&theMsg);



	// State
	NN_ASSERT( theBroadcaster.IsBroadcasting());
	NN_ASSERT(!theBroadcaster.HasListeners());



	// Add/remove listener
	theBroadcaster.AddListener(&theListener);
	NN_ASSERT(theBroadcaster.HasListeners());

	theBroadcaster.RemoveListener(&theListener);
	NN_ASSERT(!theBroadcaster.HasListeners());



	// Remove all listeners
	theBroadcaster.AddListener(&theListener);
	theBroadcaster.AddListener(&theListener2);
	NN_ASSERT(theBroadcaster.HasListeners());

	theBroadcaster.RemoveListeners();
	NN_ASSERT(!theBroadcaster.HasListeners());



	// Broadcast
	theBroadcaster.AddListener(&theListener);
	theBroadcaster.BroadcastMessage(kTestMsgValue);

	NN_ASSERT(theMsg == kTestMsgValue);



	// No broadcast
	theMsg = kTestMsgNone;

	theBroadcaster.SetBroadcasting(false);
	theBroadcaster.BroadcastMessage(kTestMsgValue);
	theBroadcaster.SetBroadcasting(true);

	NN_ASSERT(theMsg == kTestMsgNone);



	// No listeners
	theMsg = kTestMsgNone;

	theBroadcaster.RemoveListeners();
	theBroadcaster.BroadcastMessage(kTestMsgValue);

	NN_ASSERT(theMsg == kTestMsgNone);



	// Copy listeners
	theMsg = kTestMsgNone;
	theBroadcaster.AddListener(&theListener);
	
	theBroadcaster2 = theBroadcaster;
	theBroadcaster.RemoveListeners();
	
	theBroadcaster2.BroadcastMessage(kTestMsgValue);

	NN_ASSERT(theMsg == kTestMsgValue);
}





