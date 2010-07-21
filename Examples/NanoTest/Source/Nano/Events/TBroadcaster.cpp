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
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.AddListener(&theListener);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == kTestMsgSetUInt32);



	// No broadcast
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.SetBroadcasting(false);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);
	theBroadcaster.SetBroadcasting(true);

	NN_ASSERT(theValue == kTestMsgNone);



	// No listeners
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);

	theBroadcaster.RemoveListeners();
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == kTestMsgNone);



	// Copy listeners
	theValue = kTestMsgNone;
	theListener.SetTargetUInt32(&theValue);
	theBroadcaster.AddListener(&theListener);
	
	theBroadcaster2 = theBroadcaster;
	theBroadcaster.RemoveListeners();
	
	theBroadcaster2.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == kTestMsgSetUInt32);



	// Remove the listener that's broadcasting, while broadcasting
	theListener.SetTargetListener(&theListener);

	theBroadcaster.RemoveListeners();
	theBroadcaster.AddListener(&theListener);

	theBroadcaster.BroadcastMessage(kTestMsgRemoveListener);
	
	NN_ASSERT(!theBroadcaster.HasListeners());
	NN_ASSERT(!theListener.IsListeningTo());



	// Remove a different listener from the same broadcaster, while broadcasting
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

	theBroadcaster.RemoveListeners();
	theBroadcaster.AddListener(&theListener);
	theBroadcaster.AddListener(&theListener2);

	theBroadcaster.BroadcastMessage(kTestMsgRemoveListener);

	NN_ASSERT(theBroadcaster.HasListeners());
	NN_ASSERT(theListener.IsListeningTo() || theListener2.IsListeningTo());
}







