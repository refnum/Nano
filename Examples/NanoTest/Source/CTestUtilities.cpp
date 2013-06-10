/*	NAME:
		CTestUtilities.cpp

	DESCRIPTION:
		Test utilities.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadPool.h"

#include "CTestUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kThreadCount									= 10;
static const NIndex kLockCount										= 100;

static const UInt8  kDataActive										= 0x23;
static const UInt8  kDataInactive									= 0x42;
static const NIndex kDataSize										= 5000;





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NString CTestUtilities::mDebugOutput;





//============================================================================
//		CTestUtilities::SetDebugCapture : Enable/disable debug capture.
//----------------------------------------------------------------------------
NString CTestUtilities::SetDebugCapture(bool captureDebug)
{


	// Enable/disable debug capture
	if (captureDebug)
		{
		mDebugOutput.Clear();
		NDebug::Get()->SetDebugOutput(CTestUtilities::DebugOutputHook);
		}
	else
		NDebug::Get()->SetDebugOutput(NULL);
	
	return(mDebugOutput);
}





//============================================================================
//		CTestUtilities::ExecuteRunloop : Execute the runloop.
//----------------------------------------------------------------------------
void CTestUtilities::ExecuteRunloop(NTime waitFor)
{


#if NN_TARGET_MAC || NN_TARGET_IOS
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, waitFor, false);


#elif NN_TARGET_WINDOWS
	NTime	endTime = NTimeUtilities::GetTime() + waitFor;
	MSG		theMsg;

	while (GetMessage(&theMsg, NULL, 0, 0) && NTimeUtilities::GetTime() < endTime)
		{
		TranslateMessage(&theMsg);
		DispatchMessage( &theMsg);
		}

	NThread::Sleep();

#elif NN_TARGET_LINUX
	NTime	endTime = NTimeUtilities::GetTime() + waitFor;

	while (NTimeUtilities::GetTime() < endTime)
		NThread::Sleep();


#else
	UNKNOWN RUNTIME
#endif
}





//============================================================================
//		CTestUtilities::TestLock : Test a lock.
//----------------------------------------------------------------------------
bool CTestUtilities::TestLock(NLock *theLock)
{	NThreadPool		thePool;
	NData			theData;
	bool			wasOK;
	NIndex			n;



	// Get the state we need
	theData.SetSize(kDataSize);
	wasOK = true;



	// Test the lock
	for (n = 0; n < kThreadCount; n++)
		thePool.AddTask(new NThreadTaskFunctor(BindFunction(CTestUtilities::LockUnlock, theLock, &theData, &wasOK)));
	
	thePool.WaitForTasks();
	
	return(wasOK);
}





#pragma mark private
//============================================================================
//		CTestUtilities::DebugOutputHook : Debug output hook.
//----------------------------------------------------------------------------
void CTestUtilities::DebugOutputHook(const char *theMsg)
{


	// Capture the output
	mDebugOutput += NString(theMsg, kNStringLength);
}





//============================================================================
//		CTestUtilities::LockUnlock : Lock and unlock a lock.
//----------------------------------------------------------------------------
void CTestUtilities::LockUnlock(NLock *theLock, NData *theData, bool *wasOK)
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
		memset(dataPtr, kDataActive, (size_t) dataSize);
		
		for (m = 0; m < dataSize; m++)
			{
			if (dataPtr[m] != kDataActive)
				*wasOK = false;
			}
		
		memset(dataPtr, kDataInactive, (size_t) dataSize);
		}
}


