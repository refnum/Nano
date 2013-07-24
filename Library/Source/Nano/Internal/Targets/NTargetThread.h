/*	NAME:
		NTargetThread.h

	DESCRIPTION:
		Thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_THREAD_HDR
#define NTARGET_THREAD_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"
#include "NAtomicList.h"
#include "NSemaphore.h"
#include "NThread.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef NAtomicList<NFunctor>										ThreadFunctorList;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetThread {
public:
	// Get the active CPU count
	static NIndex						GetCPUCount(void);


	// Atomic operations
	static bool							AtomicCompareAndSwap32( int32_t &theValue, int32_t oldValue, int32_t newValue);
	static bool							AtomicCompareAndSwapPtr(void  *&theValue, void  *oldValue, void  *newValue);

	static int32_t						AtomicAdd32(int32_t &theValue, int32_t theDelta);

	static void							AtomicAnd32(uint32_t &theValue, uint32_t theMask);
	static void							AtomicOr32( uint32_t &theValue, uint32_t theMask);
	static void							AtomicXor32(uint32_t &theValue, uint32_t theMask);


	// Barriers
	static void							MemoryBarrier(void);


	// Threads
	static bool							ThreadIsMain(void);
	static NThreadID					ThreadGetID(void);
	static NString						ThreadGetName(void);
	static void							ThreadSetName(const NString &theName);
	static bool							ThreadAreEqual(NThreadID thread1, NThreadID thread2);
	static void							ThreadSleep(NTime theTime);
	static NStatus						ThreadCreate(    const NFunctor &theFunctor);
	static void							ThreadInvokeMain(const NFunctor &theFunctor);


	// Thread-local values
	static NThreadLocalRef				LocalCreate(void);
	static void							LocalDestroy( NThreadLocalRef theKey);
	static void						   *LocalGetValue(NThreadLocalRef theKey);
	static void							LocalSetValue(NThreadLocalRef theKey, void *theValue);


	// Semaphores
	static NSemaphoreRef				SemaphoreCreate(NIndex theValue);
	static void							SemaphoreDestroy(NSemaphoreRef theSemaphore);
	static bool							SemaphoreWait(   NSemaphoreRef theSemaphore, NTime waitFor);
	static void							SemaphoreSignal( NSemaphoreRef theSemaphore);


	// Spin lock
	static bool							SpinLock(  int32_t &theLock, bool canBlock);
	static void							SpinUnlock(int32_t &theLock);
};





#endif // NTARGET_THREAD_HDR
