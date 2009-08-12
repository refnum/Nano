/*	NAME:
		NMacThread.cpp

	DESCRIPTION:
		Mac thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <unistd.h>
#include <semaphore.h>
#include <sys/sysctl.h>

#include <libKern/OSAtomic.h>

#include "NThreadUtilities.h"
#include "NTimeUtilities.h"
#include "NMathUtilities.h"
#include "NMacTarget.h"
#include "NTargetThread.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef struct {
	NFunctor			theFunctor;
} ThreadInfo;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		ThreadCallback : Thread callback.
//----------------------------------------------------------------------------
static void *ThreadCallback(void *userData)
{	ThreadInfo			*threadInfo = (ThreadInfo *) userData;



	// Invoke the thread
	threadInfo->theFunctor();
	delete threadInfo;
	
	return(NULL);
}





//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
UInt32 NTargetThread::GetCPUCount(void)
{	int			mibNames[2] = { CTL_HW, HW_NCPU }; 
	int			numCPUs, sysErr;
	size_t		theSize;



	// Get the CPU count
	theSize = sizeof(numCPUs);
	sysErr  = sysctl(mibNames, 2, &numCPUs, &theSize, NULL, 0);
	NN_ASSERT_NOERR(sysErr);
	
	if (sysErr != 0)
		numCPUs = 1;
	
	return(numCPUs);
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// Compare and swap
	return(OSAtomicCompareAndSwap32Barrier(oldValue, newValue, (int32_t *) &theValue));
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Add the value
	OSAtomicAdd32Barrier(theDelta, (int32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	OSAtomicAnd32Barrier(theMask, (uint32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	OSAtomicXor32Barrier(theMask, (uint32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Update the value
	OSAtomicOr32Barrier(theMask, (uint32_t *) &theValue);
}





//============================================================================
//		NTargetThread::SemaphoreCreate : Create a semaphore.
//----------------------------------------------------------------------------
NSemaphoreRef NTargetThread::SemaphoreCreate(NIndex theValue)
{	sem_t		semRef;
	int			sysErr;



	// Create the semaphore
	semRef = 0;
	sysErr = sem_init(&semRef, 0, theValue);
	NN_ASSERT_NOERR(sysErr);

	return((NSemaphoreRef) semRef);
}





//============================================================================
//		NTargetThread::SemaphoreDestroy : Destroy a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreDestroy(NSemaphoreRef &theSemaphore)
{	sem_t		semRef = (sem_t) theSemaphore;
	int			sysErr;



	// Destroy the semaphore
	sysErr = sem_destroy(&semRef);
	NN_ASSERT_NOERR(sysErr);

	theSemaphore = (NSemaphoreRef) NULL;
}





//============================================================================
//		NTargetThread::SemaphoreSignal : Signal a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreSignal(NSemaphoreRef theSemaphore)
{	sem_t		semRef = (sem_t) theSemaphore;
	int			sysErr;



	// Signal the semaphore
    sysErr = sem_post(&semRef);
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//		NTargetThread::SemaphoreWait : Wait for a semaphore.
//----------------------------------------------------------------------------
bool NTargetThread::SemaphoreWait(NSemaphoreRef theSemaphore, NTime waitFor)
{	sem_t		semRef = (sem_t) theSemaphore;
	NTime		stopTime;
	int			sysErr;



	// Wait with timeout
	if (!NMathUtilities::AreEqual(waitFor, kNTimeForever))
		{
		stopTime = NTimeUtilities::GetTime() + waitFor;
		do
			{
			sysErr = sem_trywait(&semRef);
			if (sysErr != kNoErr && NTimeUtilities::GetTime() < stopTime)
				{
				NThread::Sleep(kNThreadSpinTime);
				sysErr = kNoErr;
				}
			}
		while (sysErr != kNoErr);
		}


	// Wait for the semaphore
	else
		sysErr = sem_wait(&semRef);

	return(sysErr == kNoErr);
}





//============================================================================
//		NTargetThread::ThreadIsMain : Is this the main thread?
//----------------------------------------------------------------------------
bool NTargetThread::ThreadIsMain(void)
{


	// Check our state
	return(pthread_main_np() != 0);
}





//============================================================================
//		NTargetThread::ThreadSleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSleep(NTime theTime)
{


	// Sleep the thread
	usleep((useconds_t) (theTime / kNTimeMicrosecond));
}





//============================================================================
//		NTargetThread::ThreadCreate : Create a thread.
//----------------------------------------------------------------------------
NStatus NTargetThread::ThreadCreate(const NFunctor &theFunctor)
{	ThreadInfo		*threadInfo;
	pthread_t		threadID;
	int				sysErr;



	// Get the state we need
	threadInfo             = new ThreadInfo;
	threadInfo->theFunctor = theFunctor;



	// Create the thread
	sysErr = pthread_create(&threadID, NULL, ThreadCallback, threadInfo);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}









