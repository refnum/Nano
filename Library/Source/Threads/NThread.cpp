/*	NAME:
		NThread.cpp

	DESCRIPTION:
		Thread object.
	
	COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NThreadUtilities.h"
#include "NThread.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
ThreadFunctorList NThread::mFunctors;





//============================================================================
//		NThread::NThread : Constructor.
//----------------------------------------------------------------------------
NThread::NThread(UInt32 stackSize)
{


	// Initialise ourselves
	mIsRunning  = false;
	mAutoDelete = false;
	mShouldStop = false;
	mStackSize  = stackSize;
}





//============================================================================
//		NThread::~NThread : Destructor.
//----------------------------------------------------------------------------
NThread::~NThread(void)
{


	// Clean up
	Stop();
}





//============================================================================
//		NThread::IsRunning : Is the thread running?
//----------------------------------------------------------------------------
bool NThread::IsRunning(void) const
{


	// Get our state
	return(mIsRunning);
}





//============================================================================
//		NThread::IsAutoDelete : Is this an auto-delete thread?
//----------------------------------------------------------------------------
bool NThread::IsAutoDelete(void) const
{


	// Get our state
	return(mAutoDelete);
}





//============================================================================
//		NThread::SetAutoDelete : Set the auto-delete state.
//----------------------------------------------------------------------------
void NThread::SetAutoDelete(bool autoDelete)
{


	// Set our state
	mAutoDelete = autoDelete;
}





//============================================================================
//		NThread::Start : Start the thread.
//----------------------------------------------------------------------------
void NThread::Start(void)
{	MPTaskID	taskID;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(!IsRunning());



	// Create the thread
	theErr = MPCreateTask(ThreadEntry, this, mStackSize, NULL, NULL, NULL, 0, &taskID);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NThread::Stop : Stop the thread.
//----------------------------------------------------------------------------
void NThread::Stop(bool shouldWait)
{


	// Stop the thread
	mShouldStop = true;

	if (shouldWait)
		{
		while (IsRunning())
			Sleep();
		}
}





//============================================================================
//		NThread::Sleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NThread::Sleep(EventTime theTime)
{	useconds_t	timeUS;



	// Sleep the thread
	timeUS = (useconds_t) (theTime / kEventDurationMicrosecond);
	usleep(timeUS);



	// Invoke the functors
	//
	// By using usleep to block the thread, we can avoid running the event loop
	// in situations where the main thread may not be in a position to do so (it
	// may be in the middle of destroying an object which has had to block for
	// its thread to exit).
	//
	// However, not running the event loop means that the functor timers never fire
	// so we also invoke these functors here to ensure they are eventually completed.
	//
	// This allows us to avoid a deadlock common to APIs such as MPRemote, where
	// the main thread is waiting for a thread to exit and that thread is waiting
	// inside InvokeMain for a functor to complete.
	if (NThreadUtilities::IsMainThread())
		InvokeFunctors();
}





//============================================================================
//		NThread::InvokeMain : Perform a functor on the main thread.
//----------------------------------------------------------------------------
void NThread::InvokeMain(const NFunctor &theFunctor)
{	NSemaphore			theSemaphore;
	ThreadFunctor		theInfo;
	bool				wasDone;



	// Invoke the functor directly if we can
	if (NThreadUtilities::IsMainThread())
		{
		theFunctor();
		return;
		}



	// Queue up the request
	theInfo.theFunctor = theFunctor;
	theInfo.isDone     = &theSemaphore;
	
	mFunctors.PushBack(theInfo);



	// Wait for the result
	//
	// We install a one-shot timer to execute the functor on the main event loop,
	// however this timer may never be used (if the main thread is busy sleeping,
	// it will invoke the functor directly since the event loop isn't running).
	NSystemUtilities::DelayFunctor(BindFunction(NThread::InvokeFunctors), 0.0);

	wasDone = theSemaphore.Wait();
	NN_ASSERT(wasDone);
}





//============================================================================
//		NThread::ShouldStop : Should the thread stop?
//----------------------------------------------------------------------------
#pragma mark -
bool NThread::ShouldStop(void) const
{


	// Get our state
	return(mShouldStop);
}





//============================================================================
//		NThread::ThreadEntry : Thread entry point.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NThread::ThreadEntry(void *userData)
{	NThread		*thisPtr = (NThread *) userData;



	// Validate our state
	NN_ASSERT(!thisPtr->mIsRunning);



	// Invoke the thread
	thisPtr->Run();
	thisPtr->mIsRunning = false;



	// Clean up
	if (thisPtr->mAutoDelete)
		delete thisPtr;

	return(noErr);
}





//============================================================================
//		NThread::InvokeFunctors : Invoke the functors.
//----------------------------------------------------------------------------
void NThread::InvokeFunctors(void)
{	ThreadFunctor	theInfo;



	// Validate our state
	NN_ASSERT(NThreadUtilities::IsMainThread());



	// Invoke the functors
	while (mFunctors.PopFront(theInfo))
		{
		theInfo.theFunctor();
		theInfo.isDone->Signal();
		}
}

