/*	NAME:
		NThreadTask.cpp

	DESCRIPTION:
		Thread pool task.
	
	COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"
#include "NThreadTask.h"





//============================================================================
//		NThreadTask::NThreadTask : Constructor.
//----------------------------------------------------------------------------
NThreadTask::NThreadTask(void)
{


	// Initialize ourselves
	mName.Format("0x%8X", (UInt32) this);

	mStopped   = false;
	mPriority  = 0.0f;
	mTimeStamp = NTimeUtilities::GetTime();
}





//============================================================================
//		NThreadTask::~NThreadTask : Destructor.
//----------------------------------------------------------------------------
NThreadTask::~NThreadTask(void)
{
}





//============================================================================
//		NThreadTask::GetName : Get the name.
//----------------------------------------------------------------------------
NString NThreadTask::GetName(void) const
{


	// Get our state
	return(mName);
}





//============================================================================
//		NThreadTask::SetName : Set the name.
//----------------------------------------------------------------------------
void NThreadTask::SetName(const NString &theName)
{


	// Set our state
	mName = theName;
}





//============================================================================
//		NThreadTask::IsStopped : Is the task stopped?
//----------------------------------------------------------------------------
bool NThreadTask::IsStopped(void) const
{


	// Get our state
	return(mStopped);
}





//============================================================================
//		NThreadTask::SetStopped : Set the stopped state.
//----------------------------------------------------------------------------
void NThreadTask::Stop(void)
{


	// Set our state
	mStopped = true;
}





//============================================================================
//		NThreadTask::GetPriority : Get the priority.
//----------------------------------------------------------------------------
float NThreadTask::GetPriority(void) const
{


	// Get our state
	return(mPriority);
}





//============================================================================
//		NThreadTask::SetPriority : Set the priority.
//----------------------------------------------------------------------------
void NThreadTask::SetPriority(float theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);
	


	// Set our state
	mPriority = theValue;
}





//============================================================================
//		NThreadTask::GetTimeStamp : Get the time stamp.
//----------------------------------------------------------------------------
NTime NThreadTask::GetTimeStamp(void) const
{


	// Get our state
	return(mTimeStamp);
}





//============================================================================
//		NThreadTask::SetTimeStamp : Set the time stamp.
//----------------------------------------------------------------------------
void NThreadTask::SetTimeStamp(NTime theValue)
{


	// Set our state
	mTimeStamp = theValue;
}


