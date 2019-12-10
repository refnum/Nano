/*	NAME:
		NThread.cpp

	DESCRIPTION:
		Thread object.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NThread.h"



// Nano 3.x
#if 0
	#include "NSystemUtilities.h"
	#include "NTargetThread.h"
	#include "NThread.h"





//=============================================================================
//		NThread::NThread : Constructor.
//-----------------------------------------------------------------------------
NThread::NThread()
{


  // Initialise ourselves
  mIsRunning  = false;
  mAutoDelete = false;
  mShouldStop = false;
}





//=============================================================================
//		NThread::~NThread : Destructor.
//-----------------------------------------------------------------------------
NThread::~NThread()
{


  // Validate our state
  //
  // Sub-classes must call Stop() in their destructor; if we call it here
  // then the sub-class has already been destroyed, even though its Run()
  // method may still be executing.
  NN_ASSERT(!IsRunning());
}





//=============================================================================
//		NThread::IsMain : Is this the main thread?
//-----------------------------------------------------------------------------
bool NThread::IsMain()
{


  // Is this the main thread
  return NTargetThread::ThreadIsMain();
}





//=============================================================================
//		NThread::IsRunning : Is the thread running?
//-----------------------------------------------------------------------------
bool NThread::IsRunning() const
{


  // Get our state
  return mIsRunning;
}





//=============================================================================
//		NThread::GetName : Get the current thread name.
//-----------------------------------------------------------------------------
NString NThread::GetName()
{


  // Get the name
  return NTargetThread::ThreadGetName();
}





//=============================================================================
//		NThread::SetName : Set the current thread name.
//-----------------------------------------------------------------------------
void NThread::SetName(const NString &theName)
{


  // Validate our parameters
  NN_ASSERT(!theName.IsEmpty());



  // Set the name
  NTargetThread::ThreadSetName(theName);
}





//=============================================================================
//		NThread::AreEqual : Are two thread IDs equal?
//-----------------------------------------------------------------------------
bool NThread::AreEqual(NThreadID thread1, NThreadID thread2)
{
  bool areEqual;



  // Check the IDs
  //
  // An invalid ID can only be compared to another invalid ID.
  if (thread1 == kNThreadIDNone || thread2 == kNThreadIDNone) {
	areEqual = (thread1 == thread2);
  }
  else{
	areEqual = NTargetThread::ThreadAreEqual(thread1, thread2);
  }

  return areEqual;
}





//=============================================================================
//		NThread::IsAutoDelete : Is this an auto-delete thread?
//-----------------------------------------------------------------------------
bool NThread::IsAutoDelete() const
{


  // Get our state
  return mAutoDelete;
}





//=============================================================================
//		NThread::SetAutoDelete : Set the auto-delete state.
//-----------------------------------------------------------------------------
void NThread::SetAutoDelete(bool autoDelete)
{


  // Set our state
  mAutoDelete = autoDelete;
}





//=============================================================================
//		NThread::Start : Start the thread.
//-----------------------------------------------------------------------------
void NThread::Start()
{


  // Validate our state
  NN_ASSERT(!IsRunning());



  // Create the thread
  NTargetThread::ThreadCreate(BindSelf(NThread::InvokeRun));
}





//=============================================================================
//		NThread::Stop : Stop the thread.
//-----------------------------------------------------------------------------
void NThread::Stop(bool shouldWait)
{
  bool autoDelete;



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

	while (IsRunning()) {
	  Sleep();
	}

	if (autoDelete) {
	  delete this;
	}
  }
}





//=============================================================================
//		NThread::Sleep : Sleep the current thread.
//-----------------------------------------------------------------------------
void NThread::Sleep(NTime theTime)
{


  // Validate our parameters
  NN_ASSERT(theTime >= 0);



  // Sleep the thread
  NTargetThread::ThreadSleep(theTime);
}





//=============================================================================
//		NThread::CreateLocal : Create a thread-local value.
//-----------------------------------------------------------------------------
NThreadLocalRef NThread::CreateLocal()
{


  // Create the value
  return NTargetThread::LocalCreate();
}





//=============================================================================
//		NThread::DestroyLocal : Destroy a thread-local value.
//-----------------------------------------------------------------------------
void NThread::DestroyLocal(NThreadLocalRef theKey)
{


  // Destroy the value
  NTargetThread::LocalDestroy(theKey);
}





//=============================================================================
//		NThread::GetLocalValue : Get a thread-local value.
//-----------------------------------------------------------------------------
void *NThread::GetLocalValue(NThreadLocalRef theKey)
{


  // Get the value
  return NTargetThread::LocalGetValue(theKey);
}





//=============================================================================
//		NThread::SetLocalValue : Set a thread-local value.
//-----------------------------------------------------------------------------
void NThread::SetLocalValue(NThreadLocalRef theKey, void *theValue)
{


  // Set the value
  NTargetThread::LocalSetValue(theKey, theValue);
}





//=============================================================================
//		NThread::InvokeMain : Perform a functor on the main thread.
//-----------------------------------------------------------------------------
void NThread::InvokeMain(const NFunctor &theFunctor)
{


  // Invoke the functor
  NTargetThread::ThreadInvokeMain(theFunctor);
}



	#pragma mark protected
//=============================================================================
//		NThread::ShouldStop : Should the thread stop?
//-----------------------------------------------------------------------------
bool NThread::ShouldStop() const
{


  // Get our state
  return mShouldStop;
}



	#pragma mark private
//=============================================================================
//		NThread::InvokeRun : Invoke a thread.
//-----------------------------------------------------------------------------
void NThread::InvokeRun()
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
  if (mAutoDelete) {
	delete this;
  }
}


#endif
