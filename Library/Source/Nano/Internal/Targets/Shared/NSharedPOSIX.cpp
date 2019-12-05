/*	NAME:
		NSharedPOSIX.cpp

	DESCRIPTION:
		POSIX support.

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
#include "NSharedPOSIX.h"

// Nano
#include "NanoConstants.h"

// System
#include <math.h>
#include <sys/time.h>





//=============================================================================
//		NSharedPOSIX::FromTimeval : Convert a timeval to an NInterval.
//-----------------------------------------------------------------------------
NInterval NSharedPOSIX::FromTimeval(const struct timeval& theTime)
{


	// Get the time
	NInterval timeSecs = NInterval(theTime.tv_sec);
	NInterval timeFrac = NInterval(theTime.tv_usec) * kNTimeMicrosecond;

	return timeSecs + timeFrac;
}





//=============================================================================
//		NSharedPOSIX::ToTimeval : Convert am NInterval to a timeval.
//-----------------------------------------------------------------------------
struct timeval NSharedPOSIX::ToTimeval(NInterval theInterval)
{
	// Get the state we need
	NInterval timeSecs = floor(theInterval);
	NInterval timeFrac = theInterval - timeSecs;



	// Get the time
	struct timeval theTime;

	theTime.tv_sec  = time_t(timeSecs);
	theTime.tv_usec = suseconds_t(timeFrac / kNTimeMicrosecond);

	return theTime;
}
