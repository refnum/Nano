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
	BroadcastMsg		theMsg;



	// Initialise ourselves
	theMsg = kTestMsgNone;
	theListener.SetTarget(&theMsg);



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
	theBroadcaster.AddListener(&theListener);
	theBroadcaster.BroadcastMessage(kTestMsgValue);

	NN_ASSERT(theMsg == kTestMsgValue);



	// No listen
	theMsg = kTestMsgNone;

	theListener.SetListening(false);
	theBroadcaster.BroadcastMessage(kTestMsgValue);
	theListener.SetListening(true);

	NN_ASSERT(theMsg == kTestMsgNone);



	// Copy broadcasters
	theMsg = kTestMsgNone;

	theListener.RemoveFromBroadcasters();
	theBroadcaster.AddListener(&theListener);
	
	theListener2 = theListener;
	theListener.RemoveFromBroadcasters();
	
	theBroadcaster.BroadcastMessage(kTestMsgValue);

	NN_ASSERT(theMsg == kTestMsgValue);
}




