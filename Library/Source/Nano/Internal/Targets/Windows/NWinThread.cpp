/*	NAME:
		NWinThread.cpp

	DESCRIPTION:
		Windows thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <intrin.h>

#include "NWindows.h"
#include "NWinTarget.h"
#include "NTargetThread.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// Thread info
typedef struct {
	NFunctor			theFunctor;
} ThreadInfo;


// R/W lock info
//
// Based on Stone Steps BSD R/W lock:
//
//	http://forums.stonesteps.ca/thread.asp?t=105
typedef struct {
	CRITICAL_SECTION	theLock;
	HANDLE				eventRead;
	HANDLE				eventWrite;

	NIndex				readersActive;
	NIndex				readersWaiting;

	NIndex				writersActive;
	NIndex				writersWaiting;
} RWLockInfo;





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
// Main thread
//
// This assumes that the main thread performs static initialisation.
static DWORD gMainThreadID = GetCurrentThreadId();





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		ThreadCallback : Thread callback.
//----------------------------------------------------------------------------
static DWORD WINAPI ThreadCallback(void *userData)
{	ThreadInfo			*threadInfo = (ThreadInfo *) userData;



	// Invoke the thread
	threadInfo->theFunctor();
	delete threadInfo;
	
	return(0);
}





//============================================================================
//		RWReadLock : Acquire a read lock on a RW lock.
//----------------------------------------------------------------------------
static NStatus RWReadLock(RWLockInfo *lockInfo, NTime waitFor)
{	bool		mustWait;
	DWORD		timeMS;



	// Get the state we need
	timeMS   = (DWORD) (waitFor / kNTimeMillisecond);
	mustWait = false;



	// Acquire the lock
	do
		{
		// Try and acquire the lock
		EnterCriticalSection(&lockInfo->theLock);

		if (lockInfo->writersActive == 0 && lockInfo->writersWaiting == 0)
			{
			if (mustWait)
				{
				lockInfo->readersWaiting--;
				mustWait = false;
				}

			lockInfo->readersActive++;
			}

		else
			{
			if (!mustWait)
				{
				lockInfo->readersWaiting++;
				mustWait = true;
				}
			
			ResetEvent(lockInfo->eventRead);
			}

		LeaveCriticalSection(&lockInfo->theLock);



		// Wait for the lock
		if (mustWait)
			{
			if (WaitForSingleObject(lockInfo->eventRead, timeMS) != WAIT_OBJECT_0)
				{
				EnterCriticalSection(&lockInfo->theLock);

				lockInfo->readersWaiting--;
				SetEvent(lockInfo->eventRead);
				SetEvent(lockInfo->eventWrite);

				LeaveCriticalSection(&lockInfo->theLock);
				return(kNErrTimeout);
				}
			}
		}
	while (mustWait);
	
	return(kNoErr);
}





//============================================================================
//		RWReadUnlock : Release a read lock on a RW lock.
//----------------------------------------------------------------------------
static void RWReadUnlock(RWLockInfo *lockInfo)
{


	// Release the lock
	EnterCriticalSection(&lockInfo->theLock);

	lockInfo->readersActive--;

	if (lockInfo->writersWaiting != 0)
		SetEvent(lockInfo->eventWrite);

	else if (lockInfo->readersWaiting != 0)
		SetEvent(lockInfo->eventRead);

	LeaveCriticalSection(&lockInfo->theLock);
}





//============================================================================
//		RWWriteLock : Acquire a write lock on a RW lock.
//----------------------------------------------------------------------------
static NStatus RWWriteLock(RWLockInfo *lockInfo, NTime waitFor)
{	bool		mustWait;
	DWORD		timeMS;



	// Get the state we need
	timeMS   = (DWORD) (waitFor / kNTimeMillisecond);
	mustWait = false;



	// Acquire the lock
	do
		{
		// Try and acquire the lock
		EnterCriticalSection(&lockInfo->theLock);

		if (lockInfo->readersActive == 0 && lockInfo->writersActive == 0)
			{
			if (mustWait)
				{
				lockInfo->writersWaiting--;
				mustWait = false;
				}

			lockInfo->writersActive++;
			}
		else
			{
			if (!mustWait)
				{
				lockInfo->writersWaiting++;
				mustWait = true;
				}

			ResetEvent(lockInfo->eventWrite);
			}

		LeaveCriticalSection(&lockInfo->theLock);



		// Wait for the lock
		if (mustWait)
			{
			if (WaitForSingleObject(lockInfo->eventWrite, timeMS) != WAIT_OBJECT_0)
				{
				EnterCriticalSection(&lockInfo->theLock);
				
				lockInfo->writersWaiting--;
				SetEvent(lockInfo->eventRead);
				SetEvent(lockInfo->eventWrite);
				
				LeaveCriticalSection(&lockInfo->theLock);
				return(kNErrTimeout);
				}
			}
		}
	while (mustWait);

	return(kNoErr);
}





//============================================================================
//		RWWriteUnlock : Release a write lock on a RW lock.
//----------------------------------------------------------------------------
static void RWWriteUnlock(RWLockInfo *lockInfo)
{


	// Release the lock
	EnterCriticalSection(&lockInfo->theLock);

	lockInfo->writersActive--;

	if (lockInfo->writersWaiting != 0)
		SetEvent(lockInfo->eventWrite);

	else if (lockInfo->readersWaiting != 0)
		SetEvent(lockInfo->eventRead);

	LeaveCriticalSection(&lockInfo->theLock);
}





//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
#pragma mark -
UInt32 NTargetThread::GetCPUCount(void)
{	SYSTEM_INFO		sysInfo;



	// Get the CPU count
	GetSystemInfo(&sysInfo);
	
	return(sysInfo.dwNumberOfProcessors);
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// Compare and swap
	return(InterlockedCompareExchange((LONG *) &theValue, newValue, oldValue) == (LONG) oldValue);
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// Add the value
	InterlockedExchangeAdd((LONG *) &theValue, theDelta);
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// AND the value
	_InterlockedAnd((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// XOR the value
	_InterlockedXor((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters and state
	NN_ASSERT(NN_ALIGNED_TO(theValue, sizeof(UInt32)));
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// OR the value
	_InterlockedOr((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::ThreadIsMain : Is this the main thread?
//----------------------------------------------------------------------------
bool NTargetThread::ThreadIsMain(void)
{


	// Check our state
	return(GetCurrentThreadId() == gMainThreadID);
}





//============================================================================
//		NTargetThread::ThreadSleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSleep(NTime theTime)
{


	// Sleep the thread
	Sleep((DWORD) (theTime / kNTimeMillisecond));
}





//============================================================================
//		NTargetThread::ThreadCreate : Create a thread.
//----------------------------------------------------------------------------
NStatus NTargetThread::ThreadCreate(const NFunctor &theFunctor)
{	ThreadInfo		*threadInfo;
	NStatus			theErr;



	// Get the state we need
	threadInfo             = new ThreadInfo;
	threadInfo->theFunctor = theFunctor;



	// Create the thread
	theErr = kNoErr;
	
	if (CreateThread(NULL, 0, ThreadCallback, threadInfo, 0, NULL) == NULL)
		theErr = NWinTarget::GetLastError();
	
	return(theErr);
}





//============================================================================
//		NTargetThread::SemaphoreCreate : Create a semaphore.
//----------------------------------------------------------------------------
NSemaphoreRef NTargetThread::SemaphoreCreate(NIndex theValue)
{	HANDLE		semHnd;



	// Create the semaphore
	semHnd = CreateSemaphore(NULL, theValue, kNIndexMax, NULL);
	NN_ASSERT(semHnd != NULL);
	
	return((NSemaphoreRef) semHnd);
}





//============================================================================
//		NTargetThread::SemaphoreDestroy : Destroy a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{	HANDLE		semHnd = (HANDLE) theSemaphore;



	// Destroy the semaphore
	CloseHandle(semHnd);
}





//============================================================================
//		NTargetThread::SemaphoreSignal : Signal a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreSignal(NSemaphoreRef theSemaphore)
{	HANDLE		semHnd = (HANDLE) theSemaphore;
	BOOL		wasOK;



	// Signal the semaphore
	wasOK = ReleaseSemaphore(semHnd, 1, NULL);
	NN_ASSERT(wasOK);
}





//============================================================================
//		NTargetThread::SemaphoreWait : Wait for a semaphore.
//----------------------------------------------------------------------------
bool NTargetThread::SemaphoreWait(NSemaphoreRef theSemaphore, NTime waitFor)
{	HANDLE		semHnd = (HANDLE) theSemaphore;
	DWORD		theResult;



	// Wait for the semaphore
	theResult = WaitForSingleObject(semHnd, (DWORD) (waitFor / kNTimeMillisecond));
	NN_ASSERT(theResult == WAIT_OBJECT_0 || theResult == WAIT_TIMEOUT);

	return(theResult == WAIT_OBJECT_0);
}





//============================================================================
//      NTargetThread::MutexCreate : Create a mutex lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::MutexCreate(void)
{	HANDLE		lockHnd;



	// Create the lock
	lockHnd = CreateMutex(NULL, false, NULL);
	
	return((NLockRef) lockHnd);
}





//============================================================================
//      NTargetThread::MutexDestroy : Destroy a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexDestroy(NLockRef theLock)
{	HANDLE		lockHnd = (HANDLE) theLock;



	// Destroy the lock
	CloseHandle(lockHnd);
}





//============================================================================
//      NTargetThread::MutexLock : Lock a mutex lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::MutexLock(NLockRef theLock, NTime waitFor)
{	HANDLE		lockHnd = (HANDLE) theLock;
	DWORD		theResult;



	// Wait for the semaphore
	theResult = WaitForSingleObject(lockHnd, (DWORD) (waitFor / kNTimeMillisecond));
	NN_ASSERT(theResult == WAIT_OBJECT_0 || theResult == WAIT_TIMEOUT);

	return(theResult == WAIT_OBJECT_0);
}





//============================================================================
//      NTargetThread::MutexUnlock : Unlock a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexUnlock(NLockRef theLock)
{	HANDLE		lockHnd = (HANDLE) theLock;
	BOOL		wasOK;



	// Release the lock
	wasOK = ReleaseMutex(lockHnd);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetThread::ReadWriteCreate : Create a read-write lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::ReadWriteCreate(void)
{	RWLockInfo		*lockInfo;



	// Create the lock
	lockInfo = new RWLockInfo;
	if (lockInfo == NULL)
		return(kNLockRefNone);



	// Initialize the lock
	InitializeCriticalSection(&lockInfo->theLock);
	
	lockInfo->eventRead  = CreateEvent(NULL, false, true, NULL);
	lockInfo->eventWrite = CreateEvent(NULL, false, true, NULL);
	
	lockInfo->readersActive  = 0;
	lockInfo->readersWaiting = 0;

	lockInfo->writersActive  = 0;
	lockInfo->writersWaiting = 0;

	return((NLockRef) lockInfo);
}





//============================================================================
//      NTargetThread::ReadWriteDestroy : Destroy a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteDestroy(NLockRef theLock)
{	RWLockInfo		*lockInfo = (RWLockInfo *) theLock;



	// Validate our parameters
	NN_ASSERT(lockInfo->readersActive  == 0);
	NN_ASSERT(lockInfo->readersWaiting == 0);

	NN_ASSERT(lockInfo->writersActive  == 0);
	NN_ASSERT(lockInfo->writersWaiting == 0);



	// Destroy the lock
	WNSafeCloseHandle(lockInfo->eventRead);
	WNSafeCloseHandle(lockInfo->eventWrite);
	DeleteCriticalSection(&lockInfo->theLock);
	
	delete lockInfo;
}





//============================================================================
//      NTargetThread::ReadWriteLock : Lock a read-write lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::ReadWriteLock(NLockRef theLock, bool forRead, NTime waitFor)
{	RWLockInfo		*lockInfo = (RWLockInfo *) theLock;
	NStatus			theErr;



	// Acquire the lock
	if (forRead)
		theErr = RWReadLock( lockInfo, waitFor);
	else
		theErr = RWWriteLock(lockInfo, waitFor);
	
	return(theErr);
}





//============================================================================
//      NTargetThread::ReadWriteUnlock : Unlock a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteUnlock(NLockRef theLock, bool forRead)
{	RWLockInfo		*lockInfo = (RWLockInfo *) theLock;



	// Release the lock
	if (forRead)
		RWReadUnlock( lockInfo);
	else
		RWWriteUnlock(lockInfo);
}




 