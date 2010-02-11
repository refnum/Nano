/*	NAME:
		TLock.cpp

	DESCRIPTION:
		NLock tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadPool.h"
#include "NLock.h"

#include "TLock.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kThreadCount										= 10;
static const NIndex kLockCount											= 100;
static const NIndex kDataSize											= 5000;

static const UInt8  kDataActive											= 0x23;
static const UInt8  kDataInactive										= 0x42;





//============================================================================
//		TLock::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TLock::Execute(void)
{	NReadWriteLock		lockReadWrite;
	NMutexLock			lockMutex;
	NSpinLock			lockSpin;
	NThreadPool			thePool;
	NData				theData;
	NIndex				n;



	// Get the state we need
	theData.SetSize(kDataSize);



	// Spin lock - contention
	for (n = 0; n < kThreadCount; n++)
		thePool.AddTask(new NThreadTaskFunctor(BindFunction(TLock::LockUnlock, &lockSpin, &theData)));
	
	thePool.WaitForTasks();



	// Mutex - contention
	for (n = 0; n < kThreadCount; n++)
		thePool.AddTask(new NThreadTaskFunctor(BindFunction(TLock::LockUnlock, &lockMutex, &theData)));
	
	thePool.WaitForTasks();



	// Read/Write lock - contention
	for (n = 0; n < kThreadCount; n++)
		thePool.AddTask(new NThreadTaskFunctor(BindFunction(TLock::LockUnlock, &lockReadWrite, &theData)));
	
	thePool.WaitForTasks();



	// Mutex - must be recursive
	NN_ASSERT(!lockMutex.IsLocked());
	lockMutex.Lock();

		NN_ASSERT(lockMutex.IsLocked());
		lockMutex.Lock();
		lockMutex.Unlock();
		NN_ASSERT(lockMutex.IsLocked());

	lockMutex.Unlock();
	NN_ASSERT(!lockMutex.IsLocked());
}





//============================================================================
//		TLock::LockUnlock : Lock and unlock a lock.
//----------------------------------------------------------------------------
void TLock::LockUnlock(NLock *theLock, NData *theData)
{	NIndex		n, m, dataSize;
	UInt8		*dataPtr;



	// Get the state we need
	dataSize = theData->GetSize();
	dataPtr  = theData->GetData();



	// Lock/unlock the lock
	for (n = 0; n < kLockCount; n++)
		{
		// Acquire the lock
		StLock	acquireLock(*theLock);



		// Use the data
		memset(dataPtr, kDataActive, dataSize);
		
		for (m = 0; m < dataSize; m++)
			NN_ASSERT(dataPtr[m] == kDataActive);
		
		memset(dataPtr, kDataInactive, dataSize);
		}
}



