/*	NAME:
		NThread.h

	DESCRIPTION:
		Thread object.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREAD_HDR
#define NTHREAD_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"
#include "NSemaphore.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NTime kNThreadSleepTime								= 10.0 * kNTimeMillisecond;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Thread local value
typedef UIntPtr NThreadLocalRef;
typedef UIntPtr NThreadID;

static const NThreadLocalRef kNThreadLocalRefNone					= (NThreadLocalRef) -1;
static const NThreadID       kNThreadIDNone							= (NThreadID) NULL;


// Lists
class NThread;

typedef std::vector<NThread*>										NThreadList;
typedef NThreadList::iterator										NThreadListIterator;
typedef NThreadList::const_iterator									NThreadListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThread {
public:
										NThread(void);
	virtual							   ~NThread(void);


	// Is this the main thread?
	static bool							IsMain(void);

	// Is the thread running?
	bool								IsRunning(void) const;


	// Get the current thread
	static NThreadID					GetID(void);


	// Are two thread IDs equal?
	static bool							AreEqual(NThreadID thread1, NThreadID thread2);


	// Get/set the auto-delete state
	//
	// Auto-delete threads will delete themselves once Run returns.
	bool								IsAutoDelete(void) const;
	void								SetAutoDelete(bool autoDelete);


	// Start/stop the thread
	void								Start(void);
	void								Stop(bool shouldWait=true);


	// Sleep the current thread
	//
	// A sleep time of kNTimeNone indicates a request to yield the processor
	// if desireable, rather than actually sleeping the thread.
	static void							Sleep(NTime theTime=kNThreadSleepTime);


	// Create/destroy a thread-local value
	static NThreadLocalRef				CreateLocal(void);
	static void							DestroyLocal(NThreadLocalRef theKey);


	// Get/set a thread-local value
	static void						   *GetLocalValue(NThreadLocalRef theKey);
	static void							SetLocalValue(NThreadLocalRef theKey, void *theValue);


	// Invoke a functor on the main thread
	//
	// The thread will block until the functor has executed.
	static void							InvokeMain(const NFunctor &theFunctor);


protected:
	// Should the thread stop?
	bool								ShouldStop(void) const;


	// Run the thread
	virtual void						Run(void) = 0;


private:
	void								InvokeRun(void);


private:
	bool								mIsRunning;
	bool								mAutoDelete;
	bool								mShouldStop;
};



#endif // NTHREAD_HDR


