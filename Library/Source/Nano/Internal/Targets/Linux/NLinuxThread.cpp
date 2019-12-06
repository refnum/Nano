/*	NAME:
		NLinuxThread.cpp

	DESCRIPTION:
		Linux thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <semaphore.h>
#include <syscall.h>
#include <unistd.h>
#include <errno.h>

#include "NLinuxTarget.h"
#include "NTargetThread.h"





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
// Main thread
//
// This assumes that the main thread performs static initialisation.
static pthread_t			gMainThreadID = pthread_self();
static ThreadFunctorList	gMainThreadFunctors;





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
{	NFunctor	*theFunctor;



	// Get the state we need
	theFunctor = (NFunctor *) userData;



	// Invoke the thread
	(*theFunctor)();
	delete theFunctor;

	return(NULL);
}





#pragma mark NTargetThread
//============================================================================
//		Public functions
//----------------------------------------------------------------------------
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
NIndex NTargetThread::GetCPUCount(void)
{	NIndex		theResult;



	// Get the CPU count
	theResult = (NIndex) sysconf(_SC_NPROCESSORS_ONLN);
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
	return(pthread_equal(pthread_self(), gMainThreadID));
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
	sysErr = pthread_setname_np(pthread_self(), theName.GetUTF8());
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
	if (ThreadIsMain())
		InvokeMainThreadFunctors();
}





//============================================================================
//		NTargetThread::ThreadCreate : Create a thread.
//----------------------------------------------------------------------------
NStatus NTargetThread::ThreadCreate(const NFunctor &theFunctor)
{	NFunctor		*tmpFunctor;
	pthread_t		threadID;
	NStatus			theErr;
	int				sysErr;



	// Get the state we need
	tmpFunctor = new NFunctor(theFunctor);



	// Create the thread
	theErr = kNoErr;
	sysErr = pthread_create(&threadID, NULL, ThreadEntry, tmpFunctor);
	NN_ASSERT_NOERR(sysErr);

	if (sysErr != 0)
		theErr = NLinuxTarget::ConvertSysErr(errno);
	
	return(theErr);
}





//============================================================================
//		NTargetThread::ThreadInvokeMain : Invoke the main thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadInvokeMain(const NFunctor &theFunctor)
{	NFunctor				invokeFunctor;
	NSemaphore				theSemaphore;
// dair, to do
//	CFAbsoluteTime			fireTime;
//	CFRunLoopTimerRef		cfTimer;
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


// dair, to do
NN_LOG("NTargetThread::ThreadInvokeMain skipping runloop!");
/*
		// Schedule the timer
		fireTime = CFRunLoopGetNextTimerFireDate(CFRunLoopGetMain(), kCFRunLoopCommonModes);
		cfTimer  = CFRunLoopTimerCreate(kCFAllocatorNano, fireTime, 0.0f, 0, 0, MainThreadFunctorsTimer, NULL);

		CFRunLoopAddTimer(CFRunLoopGetMain(), cfTimer, kCFRunLoopCommonModes);
		CFSafeRelease(cfTimer);
*/


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
//      NTargetThread::SpinLock : Acquire the a spin lock.
//----------------------------------------------------------------------------
bool NTargetThread::SpinLock(int32_t &theLock, bool /*canBlock*/)
{	bool	gotLock;



	// Acquire the lock
	//
	// We don't have a blocking spinlock API, so NSpinlock must loop for us.
	gotLock = __sync_bool_compare_and_swap(&theLock, 0, 1);

	return(gotLock);
}





//============================================================================
//      NTargetThread::SpinUnlock : Release a spin lock.
//----------------------------------------------------------------------------
void NTargetThread::SpinUnlock(int32_t &theLock)
{


	// Release the lock
	__sync_add_and_fetch(&theLock, -1);
}



