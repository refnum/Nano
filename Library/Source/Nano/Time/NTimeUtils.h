/*	NAME:
		NTimeUtils.h

	DESCRIPTION:
		Time utilities.

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
#ifndef NTIME_UTILS_H
#define NTIME_UTILS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NTime.h"

// System
#include <time.h>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NTimeUtils
{
public:
	// Get the current time
	static NTime                        GetTime();


	// Get the time since boot
	static NInterval                    GetUpTime();


	// Get an absolute time
	//
	// Returns a time relative to the current time.
	static NTime                        GetAbsolute(NInterval theInterval);


	// Check the current time
	//
	// Returns as the current time is after the specified time.
	static bool                         IsAfter(NTime theTime);


	// Get the clock ticks
	//
	// Returns the ticks since boot in the sytem clock.
	static uint64_t                     GetClockTicks();


	// Get the clock frequency
	//
	// Returns the frequency, in ticks per second, of the system clock.
	static uint64_t                     GetClockFrequency();


	// Convert between time formats
	//
	// No epoch conversion is performed on intervals.
	static struct timespec              ToTimespec(NInterval theInterval);
	static struct tm                    ToTmUTC(  NTime theTime);
	static struct tm                    ToTmLocal(NTime theTime);

	static NInterval                    ToInterval(const struct timespec& timeSpec);
	static NTime                        ToTime(    const struct tm& timeUTC);
};



#endif // NTIME_UTILS_H
