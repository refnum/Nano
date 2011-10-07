/*	NAME:
		NProgressable.cpp

	DESCRIPTION:
		Mix-in class for objects that emit progress.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"
#include "NProgressable.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const float kProgressUpdateTime								= 0.2f;





//============================================================================
//		NProgressable::NProgressable : Constructor.
//----------------------------------------------------------------------------
NProgressable::NProgressable(void)
{


	// Initialise ourselves
	mLastTime  = 0.0;
	mLastValue = 0.0;
	
	mRangeOffset = 0.0f;
	mRangeScale  = 1.0f;
}





//============================================================================
//		NProgressable::~NProgressable : Destructor.
//----------------------------------------------------------------------------
NProgressable::~NProgressable(void)
{
}





//============================================================================
//		NProgressable::GetProgress : Get the progress functor.
//----------------------------------------------------------------------------
NProgressFunctor NProgressable::GetProgress(void) const
{


	// Get the functor
	return(mProgress);
}





//============================================================================
//		NProgressable::SetProgress : Set the progress functor.
//----------------------------------------------------------------------------
void NProgressable::SetProgress(const NProgressFunctor &theFunctor)
{


	// Set the functor
	mProgress = theFunctor;
}





//============================================================================
//		NProgressable::GetProgressRange : Get the progress range.
//----------------------------------------------------------------------------
void NProgressable::GetProgressRange(float &theOffset, float &theScale) const
{


	// Get the range
	theOffset = mRangeOffset;
	theScale  = mRangeScale;
}





//============================================================================
//		NProgressable::SetProgressRange : Set the progress range.
//----------------------------------------------------------------------------
void NProgressable::SetProgressRange(float theOffset, float theScale)
{


	// Set the range
	mRangeOffset = theOffset;
	mRangeScale  = theScale;
}





//============================================================================
//		NProgressable::BeginProgress : Begin the progress.
//----------------------------------------------------------------------------
NStatus NProgressable::BeginProgress(float theValue)
{


	// Begin the progress
	//
	// Resetting the time ensures we always perform the update.
	mLastTime = 0.0;
	
	return(UpdateProgress(theValue));
}





//============================================================================
//		NProgressable::EndProgress : End the progress.
//----------------------------------------------------------------------------
void NProgressable::EndProgress(float theValue)
{


	// Begin the progress
	//
	// Resetting the time ensures we always perform the update.
	mLastTime = 0.0;
	
	UpdateProgress(theValue);
}





//============================================================================
//		NProgressable::UpdateProgress : Update the progress.
//----------------------------------------------------------------------------
NStatus NProgressable::UpdateProgress(float theValue)
{	bool		didExpire, didChange;
	NTime		timeNow;
	NStatus		theErr;



	// Get the state we need
	timeNow = NTimeUtilities::GetTime();
	theErr  = kNoErr;
	
	didExpire = ((timeNow - mLastTime) >= kProgressUpdateTime);
	didChange = (mLastValue <  0.0f && theValue >= 0.0f) ||
				(mLastValue >= 0.0f && theValue <  0.0f);



	// Update the progress
	//
	// We throttle progress to avoid over-updating any UI attached to the progress,
	// but must force an update if we've changed between determinate/indeterminate.
	if (didExpire || didChange)
		{
		// Update our state
		mLastTime  = timeNow;
		mLastValue = theValue;



		// Update the progress
		if (mProgress != NULL)
			{
			if (theValue >= 0.0f)
				theValue = mRangeOffset + (mRangeScale * theValue);
			
			theErr = mProgress(theValue);
			}
		}
	
	return(theErr);
}





//============================================================================
//		NProgressable::UpdateProgress : Update the progress.
//----------------------------------------------------------------------------
NStatus NProgressable::UpdateProgress(NIndex theValue, NIndex maxValue)
{


	// Update the progress
	return(UpdateProgress( ((float) theValue) / ((float) maxValue) ));
}


