/*	NAME:
		NMessageQueue.cpp

	DESCRIPTION:
		Message queue.
	
	COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NMessageQueue.h"





//============================================================================
//		NMessageQueue::NMessageQueue : Constructor.
//----------------------------------------------------------------------------
NMessageQueue::NMessageQueue(void)
{	OSStatus	theErr;



	// Initialise ourselves
	theErr = MPCreateQueue(&mQueue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMessageQueue::~NMessageQueue : Destructor.
//----------------------------------------------------------------------------
NMessageQueue::~NMessageQueue(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mQueue != NULL);



	// Clean up
	theErr = MPDeleteQueue(mQueue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMessageQueue::PostMessage : Post a message to the queue.
//----------------------------------------------------------------------------
void NMessageQueue::PostMessage(NThreadMsg theMsg)
{


	// Validate our state
	NN_ASSERT(sizeof(NThreadMsg) == sizeof(void*));



	// Post the message
	PostMessage((void *) theMsg, NULL, NULL);
}





//============================================================================
//		NMessageQueue::PostMessage : Post a message to the queue.
//----------------------------------------------------------------------------
void NMessageQueue::PostMessage(void *param1, void *param2, void *param3)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mQueue != NULL);



	// Post the message
	theErr = MPNotifyQueue(mQueue, param1, param2, param3);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMessageQueue::WaitForMessage : Wait for a message on the queue.
//----------------------------------------------------------------------------
NThreadMsg NMessageQueue::WaitForMessage(EventTime theTime)
{	NThreadMsg	theMsg;



	// Validate our state
	NN_ASSERT(sizeof(NThreadMsg) == sizeof(void*));



	// Wait for a message
	if (!WaitForMessage((void **) &theMsg, NULL, NULL, theTime))
		theMsg = kThreadMsgNone;
	
	return(theMsg);
}





//============================================================================
//		NMessageQueue::WaitForMessage : Wait for a message on the queue.
//----------------------------------------------------------------------------
bool NMessageQueue::WaitForMessage(void **param1, void **param2, void **param3, EventTime theTime)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mQueue != NULL);



	// Wait for a message
	theErr = MPWaitOnQueue(mQueue, param1, param2, param3, NSystemUtilities::EventTimeToDuration(theTime));
	NN_ASSERT(theErr == noErr || theErr == kMPTimeoutErr);
	
	return(theErr == noErr);
}
