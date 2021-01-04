/*	NAME:
		NClock.cpp

	DESCRIPTION:
		Clock object.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
