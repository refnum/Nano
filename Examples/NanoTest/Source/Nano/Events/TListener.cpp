/*	NAME:
		TListener.cpp

	DESCRIPTION:
		NListener tests.

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
#include "TListener.h"





//============================================================================
//		TListener::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TListener::Execute(void)
{	NBroadcaster		theBroadcaster, theBroadcaster2;
	CTestListener		theListener, theListener2;
	UInt32				theValue;



	// State
	NN_ASSERT( theListener.IsListening());
	NN_ASSERT(!theListener.IsListeningTo());



	// Add/remove listener
	theBroadcaster.AddListener(&theListener);
	NN_ASSERT(theListener.IsListeningTo());
	NN_ASSERT(theListener.IsListeningTo(&theBroadcaster));
	
	theBroadcaster.RemoveListener(&theListener);
	NN_ASSERT(!theListener.IsListeningTo());
	NN_ASSERT(!theListener.IsListeningTo(&theBroadcaster));
	NN_ASSERT(!theListener.IsListeningTo(&theBroadcaster2));



	// Removal
	theBroadcaster.AddListener( &theListener);
	theBroadcaster2.AddListener(&theListener);

	NN_ASSERT(theListener.IsListeningTo());
	NN_ASSERT(theListener.IsListeningTo(&theBroadcaster));
	NN_ASSERT(theListener.IsListeningTo(&theBroadcaster2));
	
	theListener.RemoveFromBroadcasters();



	// Listen
	theValue = kTestMsgNone;
	theListener.SetTarget(&theValue);

	theBroadcaster.AddListener(&theListener);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == kTestMsgSetUInt32);



	// No listen
	theValue = kTestMsgNone;
	theListener.SetTarget(&theValue);

	theListener.SetListening(false);
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);
	theListener.SetListening(true);

	NN_ASSERT(theValue == kTestMsgNone);



	// Copy broadcasters
	theValue = kTestMsgNone;
	theListener.SetTarget(&theValue);

	theListener.RemoveFromBroadcasters();
	theBroadcaster.AddListener(&theListener);
	
	theListener2 = theListener;
	theListener.RemoveFromBroadcasters();
	
	theBroadcaster.BroadcastMessage(kTestMsgSetUInt32);

	NN_ASSERT(theValue == kTestMsgSetUInt32);
}




