/*	NAME:
		NSemaphore.cpp

	DESCRIPTION:
		Thread semaphore.
	
	COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetThread.h"
#include "NSemaphore.h"





//============================================================================
//		NSemaphore::NSemaphore : Constructor.
//----------------------------------------------------------------------------
NSemaphore::NSemaphore(NIndex theValue)
{


	// Initialise ourselves
	mSemaphore = NTargetThread::SemaphoreCreate(theValue);
}





//============================================================================
//		NSemaphore::~NSemaphore : Destructor.
//----------------------------------------------------------------------------
NSemaphore::~NSemaphore(void)
{


	// Clean up
	NTargetThread::SemaphoreDestroy(mSemaphore);
}





//============================================================================
//		NSemaphore::Signal : SIgnal the semaphore.
//----------------------------------------------------------------------------
void NSemaphore::Signal(void)
{


	// Signal the semaphore
	NTargetThread::SemaphoreSignal(mSemaphore);
}





//============================================================================
//		NSemaphore::Wait : Wait for the semaphore.
//----------------------------------------------------------------------------
bool NSemaphore::Wait(NTime waitFor)
{	


	// Wait for the semaphore
	return(NTargetThread::SemaphoreWait(mSemaphore, waitFor));
}






