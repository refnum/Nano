/*	NAME:
		CTestListener.cpp

	DESCRIPTION:
		Test listener.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "CTestListener.h"





//============================================================================
//		CTestListener::CTestListener : Constructor.
//----------------------------------------------------------------------------
CTestListener::CTestListener(void)
{


	// Initialise ourselves
	mTarget = NULL;
}





//============================================================================
//		CTestListener::~CTestListener : Destructor.
//----------------------------------------------------------------------------
CTestListener::~CTestListener(void)
{
}





//============================================================================
//		CTestListener::SetTarget : Set the target.
//----------------------------------------------------------------------------
void CTestListener::SetTarget(void *theTarget)
{


	// Set the target
	mTarget = theTarget;
}





//============================================================================
//		CTestListener::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CTestListener::DoMessage(BroadcastMsg theMsg, const void * /*msgData*/)
{


	// Handle the message
	switch (theMsg) {
		case kTestMsgSetUInt32:
			*((UInt32 *) mTarget) = kTestMsgSetUInt32;
			break;

		default:
			NN_LOG("Unknown messge: %d", theMsg);
			break;
		}
}


