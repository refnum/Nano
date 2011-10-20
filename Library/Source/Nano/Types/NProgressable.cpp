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
	mIsActive = false;
	
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
//		NProgressable::IsProgressActive : Is a progress operation active?
//----------------------------------------------------------------------------
bool NProgressable::IsProgressActive(void) const
{


	// Get the state
	return(mIsActive);
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
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(!mIsActive);



	// Begin the progress
	mIsActive = true;
	theErr    = UpdateProgress(kNProgressBegin, theValue);
	
	return(theErr);
}





//============================================================================
//		NProgressable::EndProgress : End the progress.
//----------------------------------------------------------------------------
NStatus NProgressable::EndProgress(float theValue)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(mIsActive);



	// End the progress
	theErr    = UpdateProgress(kNProgressEnd, theValue);
	mIsActive = false;
	
	return(theErr);
}





//============================================================================
//		NProgressable::ContinueProgress : Continue the progress.
//----------------------------------------------------------------------------
NStatus NProgressable::ContinueProgress(float theValue)
{


	// Continue the progress
	return(UpdateProgress(kNProgressContinue, theValue));
}





//============================================================================
//		NProgressable::ContinueProgress : Continue the progress.
//----------------------------------------------------------------------------
NStatus NProgressable::ContinueProgress(NIndex theValue, NIndex maxValue)
{


	// Continue the progress
	return(UpdateProgress(kNProgressContinue, ((float) theValue) / ((float) maxValue) ));
}





//============================================================================
//		NProgressable::UpdateProgress : Update the progress.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NProgressable::UpdateProgress(NProgressState theState, float theValue)
{	bool		isSentinel, didExpire, didChange;
	NTime		timeNow;
	NStatus		theErr;



	// Validate our state
	NN_ASSERT(mIsActive);



	// Get the state we need
	timeNow = NTimeUtilities::GetTime();
	theErr  = kNoErr;
	
	isSentinel = (theState != kNProgressContinue);
	didExpire  = ((timeNow - mLastTime) >= kProgressUpdateTime);
	didChange  = (mLastValue <  0.0f && theValue >= 0.0f) ||
				 (mLastValue >= 0.0f && theValue <  0.0f);



	// Update the progress
	//
	// We throttle progress to avoid over-updating any UI attached to the progress,
	// but must force an update if we've changed between determinate/indeterminate.
	//
	// Similarly, the begin/end points are always dispatched.
	if (isSentinel || didExpire || didChange)
		{
		// Update our state
		mLastTime  = timeNow;
		mLastValue = theValue;



		// Update the progress
		//
		// Indeterminate progress doesn't need to be offset and scaled.
		if (mProgress != NULL)
			{
			if (theValue >= 0.0f)
				theValue = mRangeOffset + (mRangeScale * theValue);
			
			theErr = mProgress(theState, theValue);
			}
		}
	
	return(theErr);
}




