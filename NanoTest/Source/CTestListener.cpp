/*	NAME:
		CTestListener.cpp

	DESCRIPTION:
		Test listener.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
	mTargetUInt32   = NULL;
	mTargetListener = NULL;
}





//============================================================================
//		CTestListener::~CTestListener : Destructor.
//----------------------------------------------------------------------------
CTestListener::~CTestListener(void)
{
}





//============================================================================
//		CTestListener::SetTargetUInt32 : Set the uint32_t target.
//----------------------------------------------------------------------------
void CTestListener::SetTargetUInt32(uint32_t *theTarget)
{


	// Set the target
	mTargetUInt32   = theTarget;
	mTargetListener = NULL;
}





//============================================================================
//		CTestListener::SetTargetListener : Set the NListener target.
//----------------------------------------------------------------------------
void CTestListener::SetTargetListener(NListener *theTarget)
{


	// Set the target
	mTargetUInt32   = NULL;
	mTargetListener = theTarget;
}





//============================================================================
//		CTestListener::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CTestListener::DoMessage(NBroadcastMsg theMsg, const void * /*msgData*/)
{


	// Handle the message
	switch (theMsg) {
		case kTestMsgSetUInt32:
			if (mTargetUInt32 != NULL)
				*mTargetUInt32 = kTestMsgSetUInt32;
			break;
		
		case kTestMsgRemoveListener:
			if (mTargetListener != NULL)
				mTargetListener->RemoveFromBroadcasters();
			break;
		
		case kMsgNBroadcasterDestroyed:
			break;

		default:
			NN_LOG("Unknown messge: %d", theMsg);
			break;
		}
}


