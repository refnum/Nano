/*	NAME:
		NSemaphore.h

	DESCRIPTION:
		Thread semaphore.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSEMAPHORE_HDR
#define NSEMAPHORE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef UIntPtr NSemaphoreRef;

static const NSemaphoreRef kNSemaphoreRefNone						= 0;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSemaphore {
public:
										NSemaphore(NIndex theValue=0);
	virtual							   ~NSemaphore(void);


	// Wait for the semaphore
	//
	// If the value of the semaphore is greater than zero, decrements the value
	// and returns true.
	//
	// Otherwise, the calling thread is blocked until the timeout occurs or the
	// semaphore is finally set.
	bool								Wait(NTime waitFor=kNTimeForever);


	// Signal the semaphore
	//
	// Wakes the thread which has been waiting the longest for the semaphore
	// or, if no threads are waiting, increments the value of the semaphore.
	void								Signal(void);


private:
	NSemaphoreRef						mSemaphore;
};





#endif // NSEMAPHORE_HDR


