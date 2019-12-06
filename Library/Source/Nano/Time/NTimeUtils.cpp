/*	NAME:
		NTimeUtils.cpp

	DESCRIPTION:
		Time utilities.

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
#include "NTimeUtils.h"

// Nano
#include "NanoConstants.h"

// System
#include <math.h>





//=============================================================================
//		NTimeUtils::ToInterval : Convert to an NInterval.
//-----------------------------------------------------------------------------
NInterval NTimeUtils::ToInterval(const struct timespec& timeSpec)
{


	// Get the time
	NInterval timeSecs = NInterval(timeSpec.tv_sec);
	NInterval timeFrac = NInterval(timeSpec.tv_nsec) * kNTimeNanosecond;

	return timeSecs + timeFrac;
}





//=============================================================================
//		NTimeUtils::ToTimespec : Convert to a timespec.
//-----------------------------------------------------------------------------
struct timespec NTimeUtils::ToTimespec(NInterval theInterval)
{
	// Get the state we need
	NInterval timeSecs = floor(theInterval);
	NInterval timeFrac = theInterval - timeSecs;



	// Get the time
	struct timespec timeSpec;

	timeSpec.tv_sec  = time_t(timeSecs);
	timeSpec.tv_nsec = long(timeFrac / kNTimeNanosecond);

	return timeSpec;
}
