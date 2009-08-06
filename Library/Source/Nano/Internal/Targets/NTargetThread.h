/*	NAME:
		NTargetThread.h

	DESCRIPTION:
		Thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_THREAD_HDR
#define NTARGET_THREAD_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSemaphore.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetThread {
public:
	// Is this the main thread?
	static bool							IsMainThread(void);


	// Sleep the current thread
	static void							Sleep(NTime theTime);


	// Get the active CPU count
	static UInt32						GetCPUCount(void);


	// Atomic operations
	static bool							AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue);
	static void							AtomicAdd32(           SInt32 &theValue, SInt32 theDelta);

	static void							AtomicAnd32(UInt32 &theValue, UInt32 theMask);
	static void							AtomicOr32( UInt32 &theValue, UInt32 theMask);
	static void							AtomicXor32(UInt32 &theValue, UInt32 theMask);


	// Semaphores
	static NSemaphoreRef				SemaphoreCreate(NIndex theValue);
	static void							SemaphoreDestroy(NSemaphoreRef &theSemaphore);
	static void							SemaphoreSignal( NSemaphoreRef  theSemaphore);
	static bool							SemaphoreWait(   NSemaphoreRef  theSemaphore, NTime waitFor=kNTimeForever);
};





#endif // NTARGET_THREAD_HDR
