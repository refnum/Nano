/*	NAME:
		NThread.h

	DESCRIPTION:
		Thread object.
	
	COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREAD_HDR
#define NTHREAD_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimer.h"
#include "NFunctor.h"
#include "NSemaphore.h"
#include "NAtomicList.h"

#include <vector>





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt32    kThreadStack								= 512 * 1024;
static const EventTime kThreadSleep								= 10 * kEventDurationMillisecond;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Deferred functors
typedef struct {
	NFunctor		theFunctor;
	NSemaphore		*isDone;
} ThreadFunctor;


// Lists
class NThread;

typedef std::vector<NThread*>									NThreadList;
typedef NThreadList::iterator									NThreadListIterator;
typedef NThreadList::const_iterator								NThreadListConstIterator;

typedef NAtomicList<ThreadFunctor>								ThreadFunctorList;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThread {
public:
										NThread(UInt32 stackSize=kThreadStack);
	virtual								~NThread(void);


	// Is the thread running?
	bool								IsRunning(void) const;


	// Get/set the auto-delete state
	//
	// Auto-delete threads will delete themselves after Run has returned.
	bool								IsAutoDelete(void) const;
	void								SetAutoDelete(bool autoDelete);


	// Start/stop the thread
	void								Start(void);
	void								Stop(bool shouldWait=true);


	// Sleep the current thread
	static void							Sleep(EventTime theTime=kThreadSleep);


	// Invoke a functor on the main thread
	static void							InvokeMain(const NFunctor &theFunctor);


protected:
	// Should the thread stop?
	bool								ShouldStop(void) const;


	// Run the thread
	virtual void						Run(void) = 0;


private:
	static OSStatus						ThreadEntry(void *userData);
	static void							InvokeFunctors(void);


private:
	static ThreadFunctorList			mFunctors;

	bool								mIsRunning;
	bool								mAutoDelete;
	bool								mShouldStop;
	UInt32								mStackSize;
};



#endif // NTHREAD_HDR


