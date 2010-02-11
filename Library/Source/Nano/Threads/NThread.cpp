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
//		Static variables
//----------------------------------------------------------------------------
ThreadFunctorList NThread::mFunctors;





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


	// Clean up
	Stop();
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
void NThread::Sleep(NTime theTime)
{


	// Sleep the thread
	NTargetThread::ThreadSleep(theTime);



	// Invoke the functors
	//
	// Sleeping the main thread will prevent functors due to be executed on the
	// main thread from firing.
	//
	// To avoid deadlocks where the main thread is waiting for a thread to exit
	// and that thread is waiting inside InvokeMain for a functor to complete,
	// sleeping the main thread will also invoke any queued functors.
	if (IsMain())
		InvokeFunctors();
}





//============================================================================
//		NThread::InvokeMain : Perform a functor on the main thread.
//----------------------------------------------------------------------------
void NThread::InvokeMain(const NFunctor &theFunctor)
{


	// Invoke the functor
	//
	// Rather than simply doing ThreadInvokeMain(theFunctor), we push functors
	// onto a list which is then processed in the main thread.
	//
	// This allows us to avoid a deadlock where a thread is blocked waiting for
	// the main thread, and the main thread is blocked in Sleep (since Sleep can
	// call InvokeFunctors to process the list).
	mFunctors.PushBack(theFunctor);

	NTargetThread::ThreadInvokeMain(BindFunction(NThread::InvokeFunctors));
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



	// Invoke the thread
	Run();
	mIsRunning = false;



	// Clean up
	if (mAutoDelete)
		delete this;
}





//============================================================================
//		NThread::InvokeFunctors : Invoke the functors.
//----------------------------------------------------------------------------
void NThread::InvokeFunctors(void)
{	NFunctor	theFunctor;



	// Validate our state
	NN_ASSERT(IsMain());



	// Invoke the functors
	while (mFunctors.PopFront(theFunctor))
		theFunctor();
}


