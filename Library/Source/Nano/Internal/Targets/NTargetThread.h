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
#include "NThread.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetThread {
public:
	// Get the active CPU count
	static UInt32						GetCPUCount(void);


	// Atomic operations
	static bool							AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue);
	static void							AtomicAdd32(           SInt32 &theValue, SInt32 theDelta);

	static void							AtomicAnd32(UInt32 &theValue, UInt32 theMask);
	static void							AtomicOr32( UInt32 &theValue, UInt32 theMask);
	static void							AtomicXor32(UInt32 &theValue, UInt32 theMask);


	// Threads
	static bool							ThreadIsMain(void);
	static void							ThreadSleep(NTime theTime);
	static NStatus						ThreadCreate(const NFunctor &theFunctor);


	// Semaphores
	static NSemaphoreRef				SemaphoreCreate(NIndex theValue);
	static void							SemaphoreDestroy(NSemaphoreRef theSemaphore);
	static void							SemaphoreSignal( NSemaphoreRef theSemaphore);
	static bool							SemaphoreWait(   NSemaphoreRef theSemaphore, NTime waitFor=kNTimeForever);


	// Mutex lock
	static NLockRef						MutexCreate(void);
	static void							MutexDestroy(NLockRef theLock);
	static NStatus						MutexLock(   NLockRef theLock, NTime waitFor);
	static void							MutexUnlock( NLockRef theLock);


	// Read/write lock
	static NLockRef						ReadWriteCreate(void);
	static void							ReadWriteDestroy(NLockRef theLock);
	static NStatus						ReadWriteLock(   NLockRef theLock, bool forRead, NTime waitFor);
	static void							ReadWriteUnlock( NLockRef theLock, bool forRead);
};





#endif // NTARGET_THREAD_HDR
