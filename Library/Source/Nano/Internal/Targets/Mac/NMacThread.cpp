/*	NAME:
		NMacThread.cpp

	DESCRIPTION:
		Mac thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <unistd.h>
#include <mach/task.h>
#include <mach/mach_init.h>
#include <mach/semaphore.h>
#include <libKern/OSAtomic.h>

#include "NNSAutoReleasePool.h"
#include "NCoreFoundation.h"
#include "NThreadUtilities.h"
#include "NTimeUtilities.h"
#include "NMathUtilities.h"
#include "NUncopyable.h"
#include "NMacTarget.h"
#include "NTargetThread.h"





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
static ThreadFunctorList gMainThreadFunctors;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		InvokeMainThreadFunctor : Invoke a main-thread functor.
//----------------------------------------------------------------------------
static void InvokeMainThreadFunctor(const NFunctor &theFunctor, NSemaphore *theSemaphore)
{


	// Validate our state
	NN_ASSERT(NTargetThread::ThreadIsMain());



	// Invoke the functor
	theFunctor();
	theSemaphore->Signal();
}





//============================================================================
//		InvokeMainThreadFunctors : Invoke the main-thread functors.
//----------------------------------------------------------------------------
static void InvokeMainThreadFunctors(void)
{	static bool			sIsInvoking = false;

	NFunctor			theFunctor;



	// Validate our state
	NN_ASSERT(NTargetThread::ThreadIsMain());



	// Invoke the functors
	//
	// If a main thread functor invokes NThread::Sleep (perhaps by waiting for
	// a lock) then we can be called recursively.
	//
	// This can lead to deadlocks, as we can try and call a functor which needs
	// to acquire a lock while executing another functor which also needs to
	// acquire the same lock.
	//
	// If the lock is meant to synchronise the main thread with a worker thread,
	// this can mean the main thread deadlocks itself.
	if (!sIsInvoking)
		{
		sIsInvoking = true;
		
		while (gMainThreadFunctors.PopFront(theFunctor))
			theFunctor();
		
		sIsInvoking = false;
		}
}





//============================================================================
//		ThreadEntry : Thread entry point.
//----------------------------------------------------------------------------
static void *ThreadEntry(void *userData)
{	NFunctor			*theFunctor;
	StAutoReleasePool	thePool;



	// Get the state we need
	theFunctor = (NFunctor *) userData;



	// Invoke the thread
	(*theFunctor)();
	delete theFunctor;

	return(NULL);
}





//============================================================================
//		MainThreadFunctorsTimer : CFTimer callback.
//----------------------------------------------------------------------------
static void MainThreadFunctorsTimer(CFRunLoopTimerRef /*cfTimer*/, void */*userData*/)
{


	// Invoke the functors
	InvokeMainThreadFunctors();
}





#pragma mark NTargetThread
//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
UInt32 NTargetThread::GetCPUCount(void)
{	UInt32		theResult;



	// Get the CPU count
	theResult = NMacTarget::GetSysctl<UInt32>(CTL_HW, HW_NCPU);
	NN_ASSERT(theResult > 0);

	if (theResult == 0)
		theResult = 1;

	return(theResult);
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
SInt32 NTargetThread::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// Add the value
	return(OSAtomicAdd32Barrier(theDelta, (int32_t *) &theValue));
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



	// Invoke the functors
	//
	// Sleeping the main thread will prevent functors due to be executed on the main
	// thread from firing.
	//
	// To avoid deadlocks where the main thread is waiting for a thread to exit and
	// that thread is waiting inside InvokeMain for a functor to complete, sleeping
	// the main thread will also invoke any queued functors.
	//
	//
	// We also run maintain an internal Nano run-loop for tasks which must be driven
	// by the main run loop (e.g., NSURLConnection) but where we may not want to
	// run the run loop when the main thread is sleeping.
	//
	// E.g., the main thread may need to sleep to acquire a lock, which would block
	// tasks that are scheduled on the main run loop. We can't run the main run loop
	// at this point, as this may cause unwanted recursion (e.g., if called while
	// handling drawRect, AppKit will send drawRect recursively).
	//
	// Scheduling these tasks on our private Nano mode means they can be run even
	// if the main thread has had to block. Tasks that use this special mode will
	// probably also need to arrange for the run loop to be dispatched regularly
	// from the main thread, typically by using a repeating timer.
	if (ThreadIsMain())
		{
		InvokeMainThreadFunctors();
		CFRunLoopRunInMode(kNanoRunLoopMode, 0.0, true);
		}
}





//============================================================================
//		NTargetThread::ThreadCreate : Create a thread.
//----------------------------------------------------------------------------
NStatus NTargetThread::ThreadCreate(const NFunctor &theFunctor)
{	NFunctor		*tmpFunctor;
	pthread_t		threadID;
	int				sysErr;



	// Get the state we need
	tmpFunctor = new NFunctor(theFunctor);



	// Create the thread
	sysErr = pthread_create(&threadID, NULL, ThreadEntry, tmpFunctor);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//		NTargetThread::ThreadInvokeMain : Invoke the main thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadInvokeMain(const NFunctor &theFunctor)
{	NFunctor				invokeFunctor;
	NSemaphore				theSemaphore;
	CFAbsoluteTime			fireTime;
	CFRunLoopTimerRef		cfTimer;
	bool					wasDone;



	// Invoke the functor
	if (ThreadIsMain())
		theFunctor();


	// Pass it to the main thread
	//
	// An invoker is used to invoke the functor then set our semaphore.
	//
	// The invoker is executed by the main thread, via InvokeMainThreadFunctors, either
	// due to the main thread being blocked in ThreadSleep or due to the event loop
	// running as normal and executing our timer.
	else
		{
		// Save the functor
		gMainThreadFunctors.PushBack(BindFunction(InvokeMainThreadFunctor, theFunctor, &theSemaphore));


		// Schedule the timer
		fireTime = CFRunLoopGetNextTimerFireDate(CFRunLoopGetMain(), kCFRunLoopCommonModes);
		cfTimer  = CFRunLoopTimerCreate(kCFAllocatorNano, fireTime, 0.0f, 0, 0, MainThreadFunctorsTimer, NULL);

		CFRunLoopAddTimer(CFRunLoopGetMain(), cfTimer, kCFRunLoopCommonModes);
		CFSafeRelease(cfTimer);


		// Wait for the functor to be processed
		wasDone = theSemaphore.Wait();
		NN_ASSERT(wasDone);
		}
}





//============================================================================
//		NTargetThread::LocalCreate : Create a thread-local value.
//----------------------------------------------------------------------------
NThreadLocalRef NTargetThread::LocalCreate(void)
{	pthread_key_t	keyRef;
	int				sysErr;



	// Validate our state
	NN_ASSERT(sizeof(pthread_key_t) <= sizeof(NThreadLocalRef));



	// Create the key
	keyRef = 0;
	sysErr = pthread_key_create(&keyRef, NULL);
	NN_ASSERT_NOERR(sysErr);

	return((NThreadLocalRef) keyRef);
}





//============================================================================
//		NTargetThread::LocalDestroy : Destroy a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalDestroy(NThreadLocalRef theKey)
{	pthread_key_t	keyRef = (pthread_key_t) theKey;
	int				sysErr;



	// Destroy the key
	sysErr = pthread_key_delete(keyRef);
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//		NTargetThread::LocalGetValue : Get a thread-local value.
//----------------------------------------------------------------------------
void *NTargetThread::LocalGetValue(NThreadLocalRef theKey)
{	pthread_key_t	keyRef = (pthread_key_t) theKey;
	void			*theValue;



	// Get the value
	theValue = pthread_getspecific(keyRef);

	return(theValue);
}





//============================================================================
//		NTargetThread::LocalSetValue : Set a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalSetValue(NThreadLocalRef theKey, void *theValue)
{	pthread_key_t	keyRef = (pthread_key_t) theKey;
	int				sysErr;



	// Set the value
	sysErr = pthread_setspecific(keyRef, theValue);
	NN_ASSERT_NOERR(sysErr);
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
NStatus NTargetThread::MutexLock(NLockRef theLock, bool canBlock)
{	pthread_mutex_t		*lockRef = (pthread_mutex_t *) theLock;
	int					sysErr;
	NStatus				theErr;



	// Acquire the lock
	if (canBlock)
		sysErr = pthread_mutex_lock(   lockRef);
	else
		sysErr = pthread_mutex_trylock(lockRef);
	
	theErr = (sysErr == 0) ? kNoErr : kNErrTimeout;

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
	//
	// We use calloc rather than malloc to avoid an uninitialised read
	// warning from valgrind against Darwin's pthread implementation.
	lockRef = (pthread_rwlock_t *) calloc(1, sizeof(pthread_rwlock_t));
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
NStatus NTargetThread::ReadWriteLock(NLockRef theLock, bool forRead, bool canBlock)
{	pthread_rwlock_t	*lockRef = (pthread_rwlock_t *) theLock;
	int					sysErr;
	NStatus				theErr;



	// Acquire the lock
	if (canBlock)
		{
		if (forRead)
			sysErr = pthread_rwlock_rdlock(lockRef);
		else
			sysErr = pthread_rwlock_wrlock(lockRef);
		}
	else
		{
		if (forRead)
			sysErr = pthread_rwlock_tryrdlock(lockRef);
		else
			sysErr = pthread_rwlock_trywrlock(lockRef);
		}
	
	theErr = (sysErr == 0) ? kNoErr : kNErrTimeout;

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





//============================================================================
//      NTargetThread::SpinCreate : Create a spin lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::SpinCreate(void)
{	OSSpinLock		*lockRef;



	// Validate our state
	NN_ASSERT(sizeof(lockRef) == sizeof(NLockRef));



	// Create the lock
	lockRef = (OSSpinLock *) malloc(sizeof(OSSpinLock));
	if (lockRef != NULL)
		*lockRef = OS_SPINLOCK_INIT;

	return((NLockRef) lockRef);
}





//============================================================================
//      NTargetThread::SpinDestroy : Destroy a spin lock.
//----------------------------------------------------------------------------
void NTargetThread::SpinDestroy(NLockRef theLock)
{	OSSpinLock		*lockRef = (OSSpinLock *) theLock;



	// Destroy the lock
	free(lockRef);
}





//============================================================================
//      NTargetThread::SpinLock : Lock a spin lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::SpinLock(NLockRef theLock, bool canBlock)
{	OSSpinLock		*lockRef = (OSSpinLock *) theLock;
	NStatus			theErr;



	// Acquire the lock
	theErr = kNoErr;
	
	if (canBlock)
		OSSpinLockLock(lockRef);
	else
		{
		if (!OSSpinLockTry(lockRef))
			theErr = kNErrTimeout;
		}

	return(theErr);
}





//============================================================================
//      NTargetThread::SpinUnlock : Unlock a spin lock.
//----------------------------------------------------------------------------
void NTargetThread::SpinUnlock(NLockRef theLock)
{	OSSpinLock		*lockRef = (OSSpinLock *) theLock;



	// Release the lock
	OSSpinLockUnlock(lockRef);
}


