/*	NAME:
		NProgressable.h

	DESCRIPTION:
		Mix-in class for objects that emit progress.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROGRESSABLE_HDR
#define NPROGRESSABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Progress values
static const float kNProgressNone									=  0.0f;
static const float kNProgressDone									=  1.0f;
static const float kNProgressUnknown								= -1.0f;


// Progress state
//
// Progress functors take both a state and a value, to ensure that the
// begin/end points are invoked exactly once for each operation.
//
// The continuation value may be invoked zero or more times, depending
// on the time taken by the operation.
typedef enum {
	kNProgressBegin,
	kNProgressContinue,
	kNProgressEnd
} NProgressState;





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef nfunctor<NStatus (NProgressState theState, float theValue)>	NProgressFunctor;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NProgressable {
public:
										NProgressable(void);
	virtual							   ~NProgressable(void);


	// Is a progress operation active?
	bool								IsProgressActive(void) const;


	// Get/set the progress functor
	//
	// The operations that emit progress are determined by the derived class.
	NProgressFunctor					GetProgress(void) const;
	void								SetProgress(const NProgressFunctor &theFunctor);


	// Get/set the progress range
	//
	// Progress values range from 0.0 to 1.0 by default.
	//
	// To support multiple tasks within a single operation, progress values can be
	// offset and scaled to some range before being passed to the progress functor.
	void								GetProgressRange(float &theOffset, float &theScale) const;
	void								SetProgressRange(float  theOffset, float  theScale);


	// Get/set the update time
	//
	// Progress updates are throttled to occur at most once per update time,
	// and may never be dispatched if the operation completes quickly enough.
	//
	// Begin/end updates, and updates that change to/from indeterminate progress,
	// are always dispatched.
	NTime								GetProgressTime(void) const;
	void								SetProgressTime(NTime theTime);


	// Begin/end a progress operation
	//
	// All progress operations must be bracketed with a begin/end pair.
	NStatus								BeginProgress(float theValue=kNProgressNone);
	NStatus								EndProgress(  float theValue=kNProgressDone);


	// Continue a progress operation
	//
	// Progress updates are throttled to a sensible interval for UI updates,
	// and may never be dispatched if the operation completes quickly enough.
	NStatus								ContinueProgress(float  theValue);
	NStatus								ContinueProgress(NIndex theValue, NIndex maxValue);


private:
	NStatus								UpdateProgress(NProgressState theState, float theValue);


private:
	bool								mIsActive;
	NProgressFunctor					mProgress;
	NTime								mUpdateTime;

	NTime								mLastTime;
	float								mLastValue;
	float								mRangeOffset;
	float								mRangeScale;
};






#endif // NCOMPARABLE_HDR
