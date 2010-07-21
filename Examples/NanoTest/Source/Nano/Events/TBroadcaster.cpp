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
	UInt32				theValue;



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
	theValue = 0;
	theListener.SetTarget(&theValue);

	theBroadcaster.AddListener(&theListener);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == kTestMsgSetUInt32);



	// No broadcast
	theValue = 0;
	theListener.SetTarget(&theValue);

	theBroadcaster.SetBroadcasting(false);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);
	theBroadcaster.SetBroadcasting(true);

	NN_ASSERT(theValue == 0);



	// No listeners
	theValue = 0;
	theListener.SetTarget(&theValue);

	theBroadcaster.RemoveListeners();
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == 0);



	// Copy listeners
	theValue = 0;
	theListener.SetTarget(&theValue);
	theBroadcaster.AddListener(&theListener);
	
	theBroadcaster2 = theBroadcaster;
	theBroadcaster.RemoveListeners();
	
	theBroadcaster2.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == kTestMsgSetUInt32);
}





