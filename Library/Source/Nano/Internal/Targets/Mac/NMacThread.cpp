/*	NAME:
		NMacThread.cpp

	DESCRIPTION:
		Mac thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <unistd.h>
#include <sys/sysctl.h>
#include <mach/task.h>
#include <mach/mach_init.h>
#include <mach/semaphore.h>
#include <libKern/OSAtomic.h>

#include "NThreadUtilities.h"
#include "NTimeUtilities.h"
#include "NMathUtilities.h"
#include "NMacTarget.h"
#include "NTargetThread.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// Functor info
typedef struct {
	NFunctor			theFunctor;
	NTimer				theTimer;
} FunctorInfo;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		InvokeFunctor : Invoke a functor.
//----------------------------------------------------------------------------
static void InvokeFunctor(FunctorInfo *theInfo)
{


	// Invoke the functor
	theInfo->theFunctor();
	delete theInfo;
}





//============================================================================
//		ThreadEntry : Thread entry point.
//----------------------------------------------------------------------------
static void *ThreadEntry(void *userData)
{


	// Invoke the thread
	InvokeFunctor((FunctorInfo *) userData);
	
	return(NULL);
}





//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
#pragma mark -
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


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// Compare and swap
	return(OSAtomicCompareAndSwap32Barrier(oldValue, newValue, (int32_t *) &theValue));
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// Add the value
	OSAtomicAdd32Barrier(theDelta, (int32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// AND the value
	OSAtomicAnd32Barrier(theMask, (uint32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// XOR the value
	OSAtomicXor32Barrier(theMask, (uint32_t *) &theValue);
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// OR the value
	OSAtomicOr32Barrier(theMask, (uint32_t *) &theValue);
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
{	FunctorInfo		*theInfo;
	pthread_t		threadID;
	int				sysErr;



	// Get the state we need
	theInfo             = new FunctorInfo;
	theInfo->theFunctor = theFunctor;



	// Create the thread
	sysErr = pthread_create(&threadID, NULL, ThreadEntry, theInfo);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//		NTargetThread::ThreadInvokeMain : Invoke the main thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadInvokeMain(const NFunctor &theFunctor)
{	FunctorInfo		*theInfo;



	// Invoke the functor
	if (ThreadIsMain())
		theFunctor();


	// Defer it to the main thread
	else
		{
		theInfo             = new FunctorInfo;
		theInfo->theFunctor = theFunctor;
		theInfo->theTimer.AddTimer(BindFunction(InvokeFunctor, theInfo), 0.0);
		}
}





//============================================================================
//		NTargetThread::SemaphoreCreate : Create a semaphore.
//----------------------------------------------------------------------------
NSemaphoreRef NTargetThread::SemaphoreCreate(NIndex theValue)
{	semaphore_t		semRef;
	kern_return_t	sysErr;



	// Create the semaphore
	semRef = 0;
	sysErr = semaphore_create(mach_task_self(), &semRef, SYNC_POLICY_FIFO, theValue);
	NN_ASSERT_NOERR(sysErr);

	return((NSemaphoreRef) semRef);
}





//============================================================================
//		NTargetThread::SemaphoreDestroy : Destroy a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{	semaphore_t		semRef = (semaphore_t) theSemaphore;
	kern_return_t	sysErr;



	// Destroy the semaphore
	sysErr = semaphore_destroy(mach_task_self(), semRef);
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//		NTargetThread::SemaphoreSignal : Signal a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreSignal(NSemaphoreRef theSemaphore)
{	semaphore_t		semRef = (semaphore_t) theSemaphore;
	kern_return_t	sysErr;



	// Signal the semaphore
    sysErr = semaphore_signal(semRef);
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//		NTargetThread::SemaphoreWait : Wait for a semaphore.
//----------------------------------------------------------------------------
NStatus NTargetThread::SemaphoreWait(NSemaphoreRef theSemaphore, NTime waitFor)
{	semaphore_t			semRef = (semaphore_t) theSemaphore;
	NTime				timeSecs, timeFrac;
	mach_timespec_t		waitTime;
	kern_return_t		sysErr;



	// Wait with timeout
	if (!NMathUtilities::AreEqual(waitFor, kNTimeForever))
		{
		timeSecs = floor(waitFor);
		timeFrac = waitFor - timeSecs;
		
		waitTime.tv_sec  = (unsigned int) timeSecs;
		waitTime.tv_nsec = (clock_res_t) (timeFrac / kNTimeNanosecond);
		
		sysErr = semaphore_timedwait(semRef, waitTime);
		}


	// Wait for the semaphore
	else
		sysErr = semaphore_wait(semRef);

	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetThread::MutexCreate : Create a mutex lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::MutexCreate(void)
{	pthread_mutexattr_t		lockAttr;
	pthread_mutex_t			*lockRef;
	NStatus					theErr;



	// Validate our state
	NN_ASSERT(sizeof(lockRef) == sizeof(NLockRef));
	
	
	
	// Get the state we need
	pthread_mutexattr_init(   &lockAttr);
	pthread_mutexattr_settype(&lockAttr, PTHREAD_MUTEX_RECURSIVE);



	// Create the lock
	lockRef = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
	if (lockRef != NULL)
		{
		theErr = pthread_mutex_init(lockRef, &lockAttr);
		NN_ASSERT_NOERR(theErr);
		}

	return((NLockRef) lockRef);
}





//============================================================================
//      NTargetThread::MutexDestroy : Destroy a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexDestroy(NLockRef theLock)
{	pthread_mutex_t		*lockRef = (pthread_mutex_t *) theLock;
	NStatus				theErr;



	// Destroy the lock
	theErr = pthread_mutex_destroy(lockRef);
	NN_ASSERT_NOERR(theErr);
	
	free(lockRef);
}





//============================================================================
//      NTargetThread::MutexLock : Lock a mutex lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::MutexLock(NLockRef theLock, NTime waitFor)
{	pthread_mutex_t		*lockRef = (pthread_mutex_t *) theLock;
	NTime				stopTime;
	NStatus				theErr;



	// Acquire with timeout
	if (!NMathUtilities::AreEqual(waitFor, kNTimeForever))
		{
		stopTime = NTimeUtilities::GetTime() + waitFor;
		do
			{
			// Acquire the lock
			theErr = pthread_mutex_trylock(lockRef);


			// Handle failure
			if (theErr != kNoErr && NTimeUtilities::GetTime() < stopTime)
				{
				NThread::Sleep(kNThreadSleepTime);
				theErr = kNoErr;
				}
			}
		while (theErr != kNoErr);
		}



	// Acquire the lock
	else
		theErr = pthread_mutex_lock(lockRef);



	// Convert the result
	if (theErr != kNoErr)
		theErr = kNErrTimeout;

	return(theErr);
}





//============================================================================
//      NTargetThread::MutexUnlock : Unlock a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexUnlock(NLockRef theLock)
{	pthread_mutex_t		*lockRef = (pthread_mutex_t *) theLock;
	NStatus				theErr;



	// Release the lock
	theErr = pthread_mutex_unlock(lockRef);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//      NTargetThread::ReadWriteCreate : Create a read-write lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::ReadWriteCreate(void)
{	pthread_rwlock_t		*lockRef;
	NStatus					theErr;



	// Validate our state
	NN_ASSERT(sizeof(lockRef) == sizeof(NLockRef));
	


	// Create the lock
	lockRef = (pthread_rwlock_t *) malloc(sizeof(pthread_rwlock_t));
	if (lockRef != NULL)
		{
		theErr = pthread_rwlock_init(lockRef, NULL);
		NN_ASSERT_NOERR(theErr);
		}

	return((NLockRef) lockRef);
}





//============================================================================
//      NTargetThread::ReadWriteDestroy : Destroy a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteDestroy(NLockRef theLock)
{	pthread_rwlock_t		*lockRef = (pthread_rwlock_t *) theLock;
	NStatus					theErr;



	// Destroy the lock
	theErr = pthread_rwlock_destroy(lockRef);
	NN_ASSERT_NOERR(theErr);
	
	free(lockRef);
}





//============================================================================
//      NTargetThread::ReadWriteLock : Lock a read-write lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::ReadWriteLock(NLockRef theLock, bool forRead, NTime waitFor)
{	pthread_rwlock_t		*lockRef = (pthread_rwlock_t *) theLock;
	NTime					stopTime;
	NStatus					theErr;



	// Acquire with timeout
	if (!NMathUtilities::AreEqual(waitFor, kNTimeForever))
		{
		stopTime = NTimeUtilities::GetTime() + waitFor;
		do
			{
			// Acquire the lock
			if (forRead)
				theErr = pthread_rwlock_tryrdlock(lockRef);
			else
				theErr = pthread_rwlock_trywrlock(lockRef);


			// Handle failure
			if (theErr != kNoErr && NTimeUtilities::GetTime() < stopTime)
				{
				NThread::Sleep(kNThreadSleepTime);
				theErr = kNoErr;
				}
			}
		while (theErr != kNoErr);
		}



	// Acquire the lock
	else
		{
		if (forRead)
			theErr = pthread_rwlock_rdlock(lockRef);
		else
			theErr = pthread_rwlock_wrlock(lockRef);
		}



	// Convert the result
	if (theErr != kNoErr)
		theErr = kNErrTimeout;

	return(theErr);
}





//============================================================================
//      NTargetThread::ReadWriteUnlock : Unlock a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteUnlock(NLockRef theLock, bool /*forRead*/)
{	pthread_rwlock_t		*lockRef = (pthread_rwlock_t *) theLock;
	NStatus					theErr;



	// Release the lock
	theErr = pthread_rwlock_unlock(lockRef);
	NN_ASSERT_NOERR(theErr);
}








