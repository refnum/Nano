/*	NAME:
		CWindowNanoViews.cpp

	DESCRIPTION:
		NanoDemo custom views window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NanoDemo Build.h"
#include "CWindowNanoViews.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const EventTime kDelayUpdate									= 0.025f;





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(CWindowNanoViews, kAppNibFile, "Nano Views");





//============================================================================
//		CWindowNanoViews::CWindowNanoViews : Constructor.
//----------------------------------------------------------------------------
CWindowNanoViews::CWindowNanoViews(void)
{
}





//============================================================================
//		CWindowNanoViews::~CWindowNanoViews : Destructor.
//----------------------------------------------------------------------------
CWindowNanoViews::~CWindowNanoViews(void)
{
}





//============================================================================
//		CWindowNanoViews::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowNanoViews::InitializeSelf(void)
{


	// Initialize ourselves
	mProgress = 0;

	mPDF->SetImageOrigin(CGPointMake(144, 106));



	// Start getting time
	AddTimer(kDelayUpdate, kDelayUpdate);

	return(noErr);
}





//============================================================================
//		CWindowNanoViews::DoTimer : Handle a timer.
//----------------------------------------------------------------------------
void CWindowNanoViews::DoTimer(NTimerID theTimer, TimerMsg theMsg)
{


	// Handle the timer
	switch (theTimer) {
		case kTimerDefault:
			UpdateViews();
			break;
		
		default:
			NTimerTask::DoTimer(theTimer, theMsg);
			break;
		}
}





//============================================================================
//		CWindowNanoViews::UpdateViews : Update the views.
//----------------------------------------------------------------------------
#pragma mark -
void CWindowNanoViews::UpdateViews(void)
{	float	theValue;



	// Update the progress
	mProgress += 1;

	if (mProgress > 100)
		mProgress = 0;
	
	theValue = (float) mProgress / 100.0f;



	// Update the views
	mProgressPie->SetPercentValue(    theValue);
	mLevelContinuous->SetPercentValue(theValue);
	mLevelDiscrete->SetPercentValue(  theValue);
}





