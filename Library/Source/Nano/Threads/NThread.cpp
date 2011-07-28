/*	NAME:
		NThread.cpp

	DESCRIPTION:
		Thread object.
	
	COPYRIGHT:
        Copyright (c) 2006-2010, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NTargetThread.h"
#include "NThread.h"





//============================================================================
//		NThread::NThread : Constructor.
//----------------------------------------------------------------------------
NThread::NThread(void)
{


	// Initialise ourselves
	mIsRunning  = false;
	mAutoDelete = false;
	mShouldStop = false;
}





//============================================================================
//		NThread::~NThread : Destructor.
//----------------------------------------------------------------------------
NThread::~NThread(void)
{


	// Validate our state
	//
	// Sub-classes must call Stop() in their destructor; if we call it here
	// then the sub-class has already been destroyed, even though its Run()
	// method may still be executing.
	NN_ASSERT(!IsRunning());
}





//============================================================================
//		NThread::IsMain : Is this the main thread?
//----------------------------------------------------------------------------
bool NThread::IsMain(void)
{


	// Is this the main thread
	return(NTargetThread::ThreadIsMain());
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
{


	// Validate our state
	NN_ASSERT(!IsRunning());



	// Create the thread
	NTargetThread::ThreadCreate(BindSelf(NThread::InvokeRun));
}





//============================================================================
//		NThread::Stop : Stop the thread.
//----------------------------------------------------------------------------
void NThread::Stop(bool shouldWait)
{	bool	autoDelete;



	// Stop the thread
	mShouldStop = true;



	// Wait if necessary
	//
	// If we're to wait for the thread to stop, we also need to handle deletion
	// for the thread (otherwise the thread will delete itself when it stops,
	// and we'll call IsRunning on a dead object).
	if (shouldWait)
		{
		autoDelete  = mAutoDelete;
		mAutoDelete = false;

		while (IsRunning())
			Sleep();

		if (autoDelete)
			delete this;
		}
}





//============================================================================
//		NThread::Sleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NThread::Sleep(NTime theTime)
{


	// Sleep the thread
	//
	// Sleeping the main thread will also invoke any InvokeMain functors.
	NTargetThread::ThreadSleep(theTime);
}





//============================================================================
//		NThread::CreateLocal : Create a thread-local value.
//----------------------------------------------------------------------------
NThreadLocalRef NThread::CreateLocal(void)
{


	// Create the value
	return(NTargetThread::LocalCreate());
}





//============================================================================
//		NThread::DestroyLocal : Destroy a thread-local value.
//----------------------------------------------------------------------------
void NThread::DestroyLocal(NThreadLocalRef theKey)
{


	// Destroy the value
	NTargetThread::LocalDestroy(theKey);
}





//============================================================================
//		NThread::GetLocalValue : Get a thread-local value.
//----------------------------------------------------------------------------
void *NThread::GetLocalValue(NThreadLocalRef theKey)
{


	// Get the value
	return(NTargetThread::LocalGetValue(theKey));
}





//============================================================================
//		NThread::SetLocalValue : Set a thread-local value.
//----------------------------------------------------------------------------
void NThread::SetLocalValue(NThreadLocalRef theKey, void *theValue)
{


	// Set the value
	NTargetThread::LocalSetValue(theKey, theValue);
}





//============================================================================
//		NThread::InvokeMain : Perform a functor on the main thread.
//----------------------------------------------------------------------------
void NThread::InvokeMain(const NFunctor &theFunctor)
{


	// Invoke the functor
	NTargetThread::ThreadInvokeMain(theFunctor);
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
//		NThread::InvokeRun : Invoke a thread.
//----------------------------------------------------------------------------
#pragma mark -
void NThread::InvokeRun(void)
{


	// Validate our state
	NN_ASSERT(!mIsRunning);



	// Run the thread
	mIsRunning = true;
	Run();



	// Reset our state
	mIsRunning  = false;
	mShouldStop = false;



	// Clean up
	if (mAutoDelete)
		delete this;
}

