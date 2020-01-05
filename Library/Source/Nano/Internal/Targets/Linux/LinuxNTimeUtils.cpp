/*	NAME:
		LinuxNTimeUtils.cpp

	DESCRIPTION:
		Linux time utilities.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#include "NDebug.h"
#include "NSharedPOSIX.h"
#include "NTimeUtils.h"

// System
#include <string.h>





//=============================================================================
//		NTimeUtils::GetTime : Get the time.
//-----------------------------------------------------------------------------
NInterval NTimeUtils::GetTime()
{


	// Get the time
	struct timeval timeVal = {};
	gettimeofday(&timeVal, NULL);

	return NSharedPOSIX::ToInterval(timeVal);
}





//=============================================================================
//		NTimeUtils::GetUpTime : Get the time since boot.
//-----------------------------------------------------------------------------
NInterval NTimeUtils::GetUpTime()
{


	// Get the state we need
	struct timespec timeSpec = {};
	int             sysErr   = clock_gettime(CLOCK_MONOTONIC, &timeSpec);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeSpec, 0x00, sizeof(timeSpec));
	}



	// Get the time since boot
	return NTimeUtils::ToInterval(timeSpec);
}
