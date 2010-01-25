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
void CTestListener::SetTarget(BroadcastMsg *theTarget)
{


	// Set the target
	mTarget = theTarget;
}





//============================================================================
//		CTestListener::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CTestListener::DoMessage(BroadcastMsg theMsg, const void * /*msgData*/)
{


	// Update the target
	if (mTarget != NULL)
		*mTarget = theMsg;
}
