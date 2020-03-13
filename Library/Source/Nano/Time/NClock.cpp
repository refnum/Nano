/*	NAME:
		NClock.cpp

	DESCRIPTION:
		Clock object.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
		All rights reserved.

		This software is distributed under the terms of your licensing
		agreement with refNum Software.
		
		This license grants you permission to use, copy, modify, or
		distribute this sofware only under the terms of that license.
		
		refNum Software retains full ownership of this software.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NClock.h"

// Nanp
#include "NDebug.h"
#include "NTimeUtils.h"





//=============================================================================
//		NClock::NClock : Constructor.
//-----------------------------------------------------------------------------
NClock::NClock()
	: mStart(0)
	, mEnd(0)
{


	// Initialise ourselves
	Start();
}





//=============================================================================
//		NClock::IsRunning : Is the clock running?
//-----------------------------------------------------------------------------
bool NClock::IsRunning() const
{


	// Check our state
	return mEnd == 0;
}





//=============================================================================
//		NClock::Start : Start the clock.
//-----------------------------------------------------------------------------
void NClock::Start()
{


	// Start the clock
	mStart = NTimeUtils::GetClockTicks();
	mEnd   = 0;
}





//=============================================================================
//		NClock::Stop : Stop the clock.
//-----------------------------------------------------------------------------
NInterval NClock::Stop()
{


	// Stop the clock
	mEnd = NTimeUtils::GetClockTicks();

	return GetTime();
}





//=============================================================================
//		NClock::GetTime : Get the elapsed time.
//-----------------------------------------------------------------------------
NInterval NClock::GetTime() const
{


	// Get the elapsed time
	//
	// If the clock is still running we return the currently elapsed
	// time, otherwise we return the time the clock was running for.
	uint64_t endTime = mEnd;

	if (endTime == 0)
	{
		endTime = NTimeUtils::GetClockTicks();
	}

	return GetTime(mStart, endTime);
}





#pragma mark private
//=============================================================================
//		NClock::GetTime : Get the elapsed time.
//-----------------------------------------------------------------------------
NInterval NClock::GetTime(uint64_t startTicks, uint64_t endTicks) const
{


	// Validate our parameters
	NN_REQUIRE(startTicks <= endTicks);



	// Get the time
	NInterval clockTicks = NInterval(endTicks - startTicks);
	NInterval clockFreq  = NInterval(NTimeUtils::GetClockFrequency());

	return clockTicks / clockFreq;
}
