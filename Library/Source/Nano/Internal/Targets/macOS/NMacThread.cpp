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
#include <dispatch/semaphore.h>
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
NIndex NTargetThread::GetCPUCount(void)
{	NIndex		theResult;



	// Get the CPU count
	theResult = (NIndex) NMacTarget::GetSysctl<int>(CTL_HW, HW_NCPU);
	NN_ASSERT(theResult > 0);

	if (theResult == 0)
		theResult = 1;

	return(theResult);
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwap32(int32_t &theValue, int32_t oldValue, int32_t newValue)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// Compare and swap
	return(__sync_bool_compare_and_swap(&theValue, oldValue, newValue));
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwapPtr : Atomic pointer compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwapPtr(void *&theValue, void *oldValue, void *newValue)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// Compare and swap
	return(__sync_bool_compare_and_swap(&theValue, oldValue, newValue));
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
int32_t NTargetThread::AtomicAdd32(int32_t &theValue, int32_t theDelta)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// Add the value
	return(__sync_add_and_fetch(&theValue, theDelta));
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(uint32_t &theValue, uint32_t theMask)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// AND the value
	__sync_fetch_and_and(&theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(uint32_t &theValue, uint32_t theMask)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// OR the value
	__sync_fetch_and_or(&theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(uint32_t &theValue, uint32_t theMask)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// XOR the value
	__sync_fetch_and_xor(&theValue, theMask);
}





//============================================================================
//		NTargetThread::MemoryBarrier : Insert a memory barrier.
//----------------------------------------------------------------------------
void NTargetThread::MemoryBarrier(void)
{


	// Insert the barrier
	__sync_synchronize();
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
//		NTargetThread::ThreadGetID : Get the current thread ID.
//----------------------------------------------------------------------------
NThreadID NTargetThread::ThreadGetID(void)
{


	// Validate our state
	NN_ASSERT(sizeof(NThreadID) >= sizeof(pthread_t));



	// Get the thread ID
	return((NThreadID) pthread_self());
}





//============================================================================
//		NTargetThread::ThreadGetName : Get the current thread name.
//----------------------------------------------------------------------------
NString NTargetThread::ThreadGetName(void)
{	char		theBuffer[PATH_MAX];
	NString		theName;
	int			sysErr;



	// Get the name
	sysErr = pthread_getname_np(pthread_self(), theBuffer, sizeof(theBuffer));
	NN_ASSERT_NOERR(sysErr);
	
	if (sysErr == 0)
		theName = NString(theBuffer, kNStringLength);
	
	return(theName);
}





//============================================================================
//		NTargetThread::ThreadSetName : Set the current thread name.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSetName(const NString &theName)
{	int		sysErr;



	// Set the name
	sysErr = pthread_setname_np(theName.GetUTF8());
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//		NTargetThread::ThreadAreEqual : Are two thread IDs equal?
//----------------------------------------------------------------------------
bool NTargetThread::ThreadAreEqual(NThreadID thread1, NThreadID thread2)
{


	// Validate our state
	NN_ASSERT(sizeof(NThreadID) >= sizeof(pthread_t));



	// Compare the IDs
	return(pthread_equal((pthread_t) thread1, (pthread_t) thread2));
}





//============================================================================
//		NTargetThread::ThreadSleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSleep(NTime theTime)
{	useconds_t		sleepTime;



	// Sleep the thread
	//
	// A sleep of 0 is a yield.
	sleepTime = (useconds_t) (theTime / kNTimeMicrosecond);
	
	if (sleepTime == 0)
		sched_yield();
	else
		usleep(sleepTime);



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
//		Note :	A dispatch semaphore avoids a trip to the kernel if possible,
//				so performs substantially better than a Mach semaphore.
//
//				A signal/wait loop that runs 10,000,000 times takes:
//
//					Mach semaphore:			4.7s
//					Dispatch semaphore:		0.22s
//
//				Tested on a 2.2Ghz Core i7.
//----------------------------------------------------------------------------
NSemaphoreRef NTargetThread::SemaphoreCreate(NIndex theValue)
{	dispatch_semaphore_t	semRef;



	// Validate our state
	NN_ASSERT(sizeof(NSemaphoreRef) >= sizeof(semRef));



	// Create the semaphore
	semRef = dispatch_semaphore_create(theValue);
	NN_ASSERT(semRef != NULL);

	return((NSemaphoreRef) semRef);
}





//============================================================================
//		NTargetThread::SemaphoreDestroy : Destroy a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{	dispatch_semaphore_t	semRef = (dispatch_semaphore_t) theSemaphore;



	// Destroy the semaphore
	dispatch_release(semRef);
}





//============================================================================
//		NTargetThread::SemaphoreWait : Wait for a semaphore.
//----------------------------------------------------------------------------
bool NTargetThread::SemaphoreWait(NSemaphoreRef theSemaphore, NTime waitFor)
{	dispatch_semaphore_t	semRef = (dispatch_semaphore_t) theSemaphore;
	dispatch_time_t			waitTime;
	long					sysErr;



	// Wait with timeout
	if (NMathUtilities::AreEqual(waitFor, kNTimeForever))
		waitTime = DISPATCH_TIME_FOREVER;
	else
		waitTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t) (waitFor / kNTimeNanosecond));

	sysErr = dispatch_semaphore_wait(semRef, waitTime);

	return(sysErr == 0);
}





//============================================================================
//		NTargetThread::SemaphoreSignal : Signal a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreSignal(NSemaphoreRef theSemaphore)
{	dispatch_semaphore_t	semRef = (dispatch_semaphore_t) theSemaphore;



	// Signal the semaphore
    (void) dispatch_semaphore_signal(semRef);
}





//============================================================================
//      NTargetThread::SpinLock : Acquire a spin lock.
//----------------------------------------------------------------------------
bool NTargetThread::SpinLock(int32_t &theLock, bool canBlock)
{	bool	gotLock;



	// Validate our state
	NN_ASSERT(sizeof(OSSpinLock) == sizeof(int32_t));



	// Acquire the lock
	//
	// Blocking in the OS is preferrable to looping in NSpinLock, so we do it.
	if (canBlock)
		{
		OSSpinLockLock((OSSpinLock *) &theLock);
		gotLock = true;
		}
	else
		gotLock = OSSpinLockTry((OSSpinLock *) &theLock);

	return(gotLock);
}





//============================================================================
//      NTargetThread::SpinUnlock : Release a spin lock.
//----------------------------------------------------------------------------
void NTargetThread::SpinUnlock(int32_t &theLock)
{


	// Validate our state
	NN_ASSERT(sizeof(OSSpinLock) == sizeof(int32_t));



	// Release the lock
	OSSpinLockUnlock((OSSpinLock *) &theLock);
}


