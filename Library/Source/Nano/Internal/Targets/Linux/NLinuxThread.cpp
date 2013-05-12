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
#include "NTargetThread.h"





//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
#pragma mark -
UInt32 NTargetThread::GetCPUCount(void)
{


	// dair, to do
	NN_LOG("NTargetThread::GetCPUCount not implemented!");
	return(0);
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// dair, to do
	NN_LOG("NTargetThread::AtomicCompareAndSwap32 not implemented!");
	return(false);
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
SInt32 NTargetThread::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// dair, to do
	NN_LOG("NTargetThread::AtomicAdd32 not implemented!");
	return(0);
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// dair, to do
	NN_LOG("NTargetThread::AtomicAnd32 not implemented!");
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// dair, to do
	NN_LOG("NTargetThread::AtomicXor32 not implemented!");
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// dair, to do
	NN_LOG("NTargetThread::AtomicOr32 not implemented!");
}





//============================================================================
//		NTargetThread::ThreadIsMain : Is this the main thread?
//----------------------------------------------------------------------------
bool NTargetThread::ThreadIsMain(void)
{


	// dair, to do
	NN_LOG("NTargetThread::ThreadIsMain not implemented!");
	return(false);
}





//============================================================================
//		NTargetThread::ThreadSleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSleep(NTime theTime)
{


	// dair, to do
	NN_LOG("NTargetThread::ThreadSleep not implemented!");
}





//============================================================================
//		NTargetThread::ThreadCreate : Create a thread.
//----------------------------------------------------------------------------
NStatus NTargetThread::ThreadCreate(const NFunctor &theFunctor)
{


	// dair, to do
	NN_LOG("NTargetThread::ThreadCreate not implemented!");
	return(kNErrPermission);
}





//============================================================================
//		NTargetThread::ThreadInvokeMain : Invoke the main thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadInvokeMain(const NFunctor &theFunctor)
{


	// dair, to do
	NN_LOG("NTargetThread::ThreadInvokeMain not implemented!");
}





//============================================================================
//		NTargetThread::LocalCreate : Create a thread-local value.
//----------------------------------------------------------------------------
NThreadLocalRef NTargetThread::LocalCreate(void)
{


	// dair, to do
	NN_LOG("NTargetThread::LocalCreate not implemented!");
	return(0);
}





//============================================================================
//		NTargetThread::LocalDestroy : Destroy a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalDestroy(NThreadLocalRef theKey)
{


	// dair, to do
	NN_LOG("NTargetThread::LocalDestroy not implemented!");
}





//============================================================================
//		NTargetThread::LocalGetValue : Get a thread-local value.
//----------------------------------------------------------------------------
void *NTargetThread::LocalGetValue(NThreadLocalRef theKey)
{


	// dair, to do
	NN_LOG("NTargetThread::LocalGetValue not implemented!");
	return(NULL);
}





//============================================================================
//		NTargetThread::LocalSetValue : Set a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalSetValue(NThreadLocalRef theKey, void *theValue)
{


	// dair, to do
	NN_LOG("NTargetThread::LocalSetValue not implemented!");
}





//============================================================================
//		NTargetThread::SemaphoreCreate : Create a semaphore.
//----------------------------------------------------------------------------
NSemaphoreRef NTargetThread::SemaphoreCreate(NIndex theValue)
{


	// dair, to do
	NN_LOG("NTargetThread::SemaphoreCreate not implemented!");
	return(0);
}





//============================================================================
//		NTargetThread::SemaphoreDestroy : Destroy a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{


	// dair, to do
	NN_LOG("NTargetThread::SemaphoreDestroy not implemented!");
}





//============================================================================
//		NTargetThread::SemaphoreSignal : Signal a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreSignal(NSemaphoreRef theSemaphore)
{


	// dair, to do
	NN_LOG("NTargetThread::SemaphoreSignal not implemented!");
}





//============================================================================
//		NTargetThread::SemaphoreWait : Wait for a semaphore.
//----------------------------------------------------------------------------
NStatus NTargetThread::SemaphoreWait(NSemaphoreRef theSemaphore, NTime waitFor)
{


	// dair, to do
	NN_LOG("NTargetThread::SemaphoreWait not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetThread::MutexCreate : Create a mutex lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::MutexCreate(void)
{


	// dair, to do
	NN_LOG("NTargetThread::MutexCreate not implemented!");
	return(0);
}





//============================================================================
//      NTargetThread::MutexDestroy : Destroy a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexDestroy(NLockRef theLock)
{


	// dair, to do
	NN_LOG("NTargetThread::MutexDestroy not implemented!");
}





//============================================================================
//      NTargetThread::MutexLock : Lock a mutex lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::MutexLock(NLockRef theLock, NTime waitFor)
{


	// dair, to do
	NN_LOG("NTargetThread::MutexLock not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetThread::MutexUnlock : Unlock a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexUnlock(NLockRef theLock)
{


	// dair, to do
	NN_LOG("NTargetThread::MutexUnlock not implemented!");
}





//============================================================================
//      NTargetThread::ReadWriteCreate : Create a read-write lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::ReadWriteCreate(void)
{


	// dair, to do
	NN_LOG("NTargetThread::ReadWriteCreate not implemented!");
	return(0);
}





//============================================================================
//      NTargetThread::ReadWriteDestroy : Destroy a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteDestroy(NLockRef theLock)
{


	// dair, to do
	NN_LOG("NTargetThread::ReadWriteDestroy not implemented!");
}





//============================================================================
//      NTargetThread::ReadWriteLock : Lock a read-write lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::ReadWriteLock(NLockRef theLock, bool forRead, NTime waitFor)
{


	// dair, to do
	NN_LOG("NTargetThread::ReadWriteLock not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetThread::ReadWriteUnlock : Unlock a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteUnlock(NLockRef theLock, bool /*forRead*/)
{


	// dair, to do
	NN_LOG("NTargetThread::ReadWriteUnlock not implemented!");
}








