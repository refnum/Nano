/*	NAME:
		NSemaphore.cpp

	DESCRIPTION:
		Thread semaphore.
	
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
#include "NSemaphore.h"





//============================================================================
//		NSemaphore::NSemaphore : Constructor.
//----------------------------------------------------------------------------
NSemaphore::NSemaphore(MPSemaphoreCount valueMax, MPSemaphoreCount valueInitial)
{	OSStatus	theErr;



	// Initialise ourselves
	theErr = MPCreateSemaphore(valueMax, valueInitial, &mSemaphore);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NSemaphore::~NSemaphore : Destructor.
//----------------------------------------------------------------------------
NSemaphore::~NSemaphore(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mSemaphore != NULL);



	// Clean up
	theErr = MPDeleteSemaphore(mSemaphore);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NSemaphore::Signal : SIgnal the semaphore.
//----------------------------------------------------------------------------
void NSemaphore::Signal(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mSemaphore != NULL);



	// Signal the semaphore
	theErr = MPSignalSemaphore(mSemaphore);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NSemaphore::Wait : Wait for the semaphore.
//----------------------------------------------------------------------------
bool NSemaphore::Wait(EventTime theTime)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mSemaphore != NULL);



	// Wait for the semaphore
	theErr = MPWaitOnSemaphore(mSemaphore, NSystemUtilities::EventTimeToDuration(theTime));
	NN_ASSERT(theErr == noErr || theErr == kMPTimeoutErr);
	
	return(theErr == noErr);
}



