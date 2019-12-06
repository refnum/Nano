/*	NAME:
		NWinThread.cpp

	DESCRIPTION:
		Windows thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
//		Internal constants
//----------------------------------------------------------------------------
static const UINT	WM_INVOKEMAINTHREADFUNCTORS						= WM_USER + 1;

static const DWORD	MSVC_EXCEPTION									= 0x406D1388;
static const DWORD	MSVC_SET_THREADNAME								= 0x1000;





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// R/W lock reference
//
// Based on Stone Steps BSD R/W lock:
//
//	http://forums.stonesteps.ca/thread.asp?t=105
typedef struct {
	CRITICAL_SECTION	theLock;
	NIndex				numReaders;
	NIndex				numWriters;
} RWLockRef;


// MSVC thread info
//
// Documented at:
//
//	http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
#pragma pack(push,8)

typedef struct {
	DWORD				dwType;
	LPCSTR				szName;
	DWORD				dwThreadID;
	DWORD				dwFlags;
} THREADNAME_INFO;

#pragma pack(pop)


// Maps
typedef std::map<NThreadID, NString>								ThreadNameMap;
typedef ThreadNameMap::iterator										ThreadNameMapIterator;
typedef ThreadNameMap::const_iterator								ThreadNameMapConstIterator;





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NFunctorInvoker {
public:
										NFunctorInvoker(const NFunctor &theFunctor, NSemaphore *theSemaphore=NULL);
	virtual							   ~NFunctorInvoker(void) { }


	// Invoke the functor
	void								Invoke(void);


private:
	NFunctor							mFunctor;
	NSemaphore						   *mSemaphore;
};





//============================================================================
//		Function prototypes
//----------------------------------------------------------------------------
static HWND CreateMainThreadWindow(void);





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
// Main thread
//
// This assumes that the main thread performs static initialisation.
static DWORD				gMainThreadID     = GetCurrentThreadId();
static HWND					gMainThreadWindow = CreateMainThreadWindow();
static ThreadFunctorList	gMainThreadFunctors;

static NSpinLock			gThreadNameLock;
static ThreadNameMap		gThreadNames;





//============================================================================
//		NFunctorInvoker::NFunctorInvoker : Constructor
//----------------------------------------------------------------------------
NFunctorInvoker::NFunctorInvoker(const NFunctor &theFunctor, NSemaphore *theSemaphore)
{


	// Initialise ourselves
	mFunctor   = theFunctor;
	mSemaphore = theSemaphore;
}





//============================================================================
//		NFunctorInvoker::Invoke : Invoke the functor.
//----------------------------------------------------------------------------
void NFunctorInvoker::Invoke(void)
{


	// Invoke the functor
	mFunctor();
	
	if (mSemaphore != NULL)
		mSemaphore->Signal();

	delete this;
}





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		ThreadEntry : Thread entry point.
//----------------------------------------------------------------------------
static DWORD WINAPI ThreadEntry(void *userData)
{	NFunctorInvoker		*theInvoker;



	// Invoke the thread
	theInvoker = (NFunctorInvoker *) userData;
	theInvoker->Invoke();
	
	return(0);
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
//		HelperWindowProc : Helper window proc.
//----------------------------------------------------------------------------
static LRESULT CALLBACK HelperWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	LRESULT		theResult;



	// Handle the message
	theResult = 0;

	switch (uMsg) {
		case WM_INVOKEMAINTHREADFUNCTORS:
			InvokeMainThreadFunctors();
			break;

		default:
			theResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}

	return(theResult);
}





//============================================================================
//		CreateMainThreadWindow : Create a window in the main thread.
//----------------------------------------------------------------------------
static HWND CreateMainThreadWindow(void)
{	WNDCLASSEX		classInfo;
	HWND			theWindow;
	ATOM			theClass;



	// Register the class
	memset(&classInfo, 0x00, sizeof(classInfo));

	classInfo.cbSize		= sizeof(WNDCLASSEX);
	classInfo.style			= CS_GLOBALCLASS;
	classInfo.lpfnWndProc	= HelperWindowProc;
	classInfo.cbClsExtra	= 0;
	classInfo.cbWndExtra	= 0;
	classInfo.hInstance		= NULL;
	classInfo.hIcon			= NULL;
	classInfo.hCursor		= NULL;
	classInfo.hbrBackground	= NULL;
	classInfo.lpszMenuName	= NULL;
	classInfo.lpszClassName	= L"Nano::NWinThread";
	classInfo.hIconSm		= NULL;

	theClass = RegisterClassEx(&classInfo);
	NN_ASSERT(theClass != NULL);



	// Create the window
	theWindow = CreateWindow((LPCTSTR) theClass, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
	NN_ASSERT(theWindow != NULL);

	return(theWindow);
}





#pragma mark NTargetThread
//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
NIndex NTargetThread::GetCPUCount(void)
{	SYSTEM_INFO		sysInfo;



	// Get the CPU count
	GetSystemInfo(&sysInfo);
	
	return((NIndex) sysInfo.dwNumberOfProcessors);
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwap32(int32_t &theValue, int32_t oldValue, int32_t newValue)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(int32_t) == sizeof(LONG));
	


	// Compare and swap
	return(_InterlockedCompareExchange((LONG *) &theValue, newValue, oldValue) == (LONG) oldValue);
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwapPtr : Atomic pointer compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwapPtr(void *&theValue, void *oldValue, void *newValue)
{


	// Validate our parameters
	NN_ASSERT_ALIGNED_4(&theValue);



	// Compare and swap
	return(InterlockedCompareExchangePointer(&theValue, oldValue, newValue) == oldValue);
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
int32_t NTargetThread::AtomicAdd32(int32_t &theValue, int32_t theDelta)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(theValue) == sizeof(LONG));
	


	// Add the value
	//
	// InterlockedAdd produces the return value we want, namely the new value
	// of 'theValue' as a result of the add (without any further reads from
	// memory).
	//
	// However InterlockedAdd is only available on Windows 8, so we need to use
	// the older InterlockedExchangeAdd.
	//
	// This returns the original value before the add, and so we need to apply
	// the delta obtain the correct result.
	//
	// This gives us the new value of 'theValue' after the atomic add, without
	// performing an extra read of 'theValue' (as this may have been changed by
	// another thread in the mean time).
	return(_InterlockedExchangeAdd((LONG *) &theValue, theDelta) + theDelta);
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(uint32_t &theValue, uint32_t theMask)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(theValue) == sizeof(LONG));
	


	// AND the value
	_InterlockedAnd((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(uint32_t &theValue, uint32_t theMask)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(theValue) == sizeof(LONG));



	// OR the value
	_InterlockedOr((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(uint32_t &theValue, uint32_t theMask)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(theValue) == sizeof(LONG));
	


	// XOR the value
	_InterlockedXor((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::MemoryBarrier : Insert a memory barrier.
//----------------------------------------------------------------------------
void NTargetThread::MemoryBarrier(void)
{


	// Insert the barrier
	::MemoryBarrier();
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
//		NTargetThread::ThreadGetID : Get the current thread ID.
//----------------------------------------------------------------------------
NThreadID NTargetThread::ThreadGetID(void)
{


	// Validate our state
	NN_ASSERT(sizeof(NThreadID) >= sizeof(DWORD));



	// Get the thread ID
	return((NThreadID) GetCurrentThreadId());
}





//============================================================================
//		NTargetThread::ThreadGetName : Get the current thread name.
//----------------------------------------------------------------------------
NString NTargetThread::ThreadGetName(void)
{	StLock							acquireLock(gThreadNameLock);
	ThreadNameMapConstIterator		theIter;
	NString							theName;



	// Get the name
	theIter = gThreadNames.find(ThreadGetID());
	if (theIter != gThreadNames.end())
		theName = theIter->second;
	
	return(theName);
}





//============================================================================
//		NTargetThread::ThreadSetName : Set the current thread name.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSetName(const NString &theName)
{	THREADNAME_INFO		theInfo;
	NThreadID			theID;



	// Get the state we need
	theID = ThreadGetID();
	
	theInfo.dwType     = MSVC_SET_THREADNAME;
	theInfo.szName     = theName.GetUTF8();
	theInfo.dwThreadID = (DWORD) theID;
	theInfo.dwFlags    = 0;



	// Set the name
	//
	// We can't use a StLock in a function that uses SEH, so we need
	// to lock/unlock manually.
	gThreadNameLock.Lock();
	gThreadNames[theID] = theName;

	__try
		{
		RaiseException(MSVC_EXCEPTION, 0, sizeof(theInfo)/sizeof(ULONG_PTR), (ULONG_PTR *) &theInfo);
		}
	__except(EXCEPTION_EXECUTE_HANDLER)
		{
		}

	gThreadNameLock.Unlock();
}





//============================================================================
//		NTargetThread::ThreadAreEqual : Are two thread IDs equal?
//----------------------------------------------------------------------------
bool NTargetThread::ThreadAreEqual(NThreadID thread1, NThreadID thread2)
{


	// Validate our state
	NN_ASSERT(sizeof(NThreadID) >= sizeof(DWORD));



	// Compare the IDs
	return(thread1 == thread2);
}





//============================================================================
//		NTargetThread::ThreadSleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSleep(NTime theTime)
{	DWORD		sleepTime;



	// Sleep the thread
	//
	// A sleep of 0 is a yield.
	sleepTime = NWinTarget::ConvertTimeMS(theTime);
	
	if (sleepTime == 0)
		YieldProcessor();
	else
		Sleep(sleepTime);



	// Invoke the functors
	//
	// Sleeping the main thread will prevent functors due to be executed on the
	// main thread from firing.
	//
	// To avoid deadlocks where the main thread is waiting for a thread to exit
	// and that thread is waiting inside InvokeMain for a functor to complete,
	// sleeping the main thread will also invoke any queued functors.
	if (ThreadIsMain())
		InvokeMainThreadFunctors();
}





//============================================================================
//		NTargetThread::ThreadCreate : Create a thread.
//----------------------------------------------------------------------------
NStatus NTargetThread::ThreadCreate(const NFunctor &theFunctor)
{	NFunctorInvoker		*theInvoker;
	NStatus				theErr;



	// Get the state we need
	theInvoker = new NFunctorInvoker(theFunctor);
	theErr     = kNoErr;



	// Create the thread
	if (CreateThread(NULL, 0, ThreadEntry, theInvoker, 0, NULL) == NULL)
		theErr = NWinTarget::GetLastError();
	
	return(theErr);
}





//============================================================================
//		NTargetThread::ThreadInvokeMain : Invoke the main thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadInvokeMain(const NFunctor &theFunctor)
{	BOOL				wasOK, wasDone;
	NSemaphore			theSemaphore;
	NFunctorInvoker		*theInvoker;



	// Invoke the functor
	if (ThreadIsMain())
		theFunctor();
	
	
	// Pass it to the main thread
	//
	// An invoker is used to invoke the functor then set our semaphore.
	//
	// The invoker is executed by the main thread, via gMainThreadFunctors,
	// either due to the main thread being blocked in ThreadSleep or due
	// to the event loop running as normal and processing the message.
	else
		{
		theInvoker = new NFunctorInvoker(theFunctor, &theSemaphore);
		gMainThreadFunctors.PushBack(BindMethod(theInvoker, NFunctorInvoker::Invoke));
		
		wasOK = PostMessage(gMainThreadWindow, WM_INVOKEMAINTHREADFUNCTORS, 0, 0);
		NN_ASSERT(wasOK);

		wasDone = theSemaphore.Wait();
		NN_ASSERT(wasDone);
		}
}





//============================================================================
//		NTargetThread::LocalCreate : Create a thread-local value.
//----------------------------------------------------------------------------
NThreadLocalRef NTargetThread::LocalCreate(void)
{	DWORD	keyRef;



	// Validate our state
	NN_ASSERT(sizeof(DWORD) <= sizeof(NThreadLocalRef));



	// Create the key
	keyRef = TlsAlloc();
	
	return((NThreadLocalRef) keyRef);
}





//============================================================================
//		NTargetThread::LocalDestroy : Destroy a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalDestroy(NThreadLocalRef theKey)
{	DWORD		keyRef = (DWORD) theKey;
	BOOL		wasOK;



	// Destroy the key
	wasOK = TlsFree(keyRef);
	NN_ASSERT(wasOK);
}





//============================================================================
//		NTargetThread::LocalGetValue : Get a thread-local value.
//----------------------------------------------------------------------------
void *NTargetThread::LocalGetValue(NThreadLocalRef theKey)
{	DWORD		keyRef = (DWORD) theKey;
	void		*theValue;



	// Get the value
	theValue = TlsGetValue(keyRef);

	return(theValue);
}





//============================================================================
//		NTargetThread::LocalSetValue : Set a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalSetValue(NThreadLocalRef theKey, void *theValue)
{	DWORD		keyRef = (DWORD) theKey;
	BOOL		wasOK;



	// Set the value
	wasOK = TlsSetValue(keyRef, theValue);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetThread::SpinLock : Acquire a spin lock.
//----------------------------------------------------------------------------
bool NTargetThread::SpinLock(int32_t &theLock, bool /*canBlock*/)
{	bool	gotLock;



	// Acquire the lock
	//
	// We don't have a blocking spinlock API, so NSpinlock must loop for us.
	gotLock = (_InterlockedCompareExchange((LONG *) &theLock, 1, 0) == 0);

	return(gotLock);
}





//============================================================================
//      NTargetThread::SpinUnlock : Release a spin lock.
//----------------------------------------------------------------------------
void NTargetThread::SpinUnlock(int32_t &theLock)
{


	// Release the lock
	_InterlockedDecrement((LONG *) &theLock);
}





