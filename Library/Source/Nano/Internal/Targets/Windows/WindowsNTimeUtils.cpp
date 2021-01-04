/*	NAME:
		WindowsNTimeUtils.cpp

	DESCRIPTION:
		Windows time utilities.

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
#include "NTimeUtils.h"

// Nano
#include "NSharedWindows.h"
#include "NanoConstants.h"





//=============================================================================
//		NTimeUtils::GetTime : Get the time.
//-----------------------------------------------------------------------------
NTime NTimeUtils::GetTime()
{


	// Get the time
	FILETIME fileTime{};
	GetSystemTimeAsFileTime(&fileTime);

	return NTime(NSharedWindows::ToInterval(fileTime), kNanoEpochFrom1601);
}





//=============================================================================
//		NTimeUtils::GetUpTime : Get the time since boot.
//-----------------------------------------------------------------------------
NInterval NTimeUtils::GetUpTime()
{


	// Get the time since boot
	uint64_t tickCount = GetTickCount64();

	return NInterval(tickCount) * kNTimeMillisecond;
}





//=============================================================================
//		NTimeUtils::GetClockTicks : Get the clock ticks.
//-----------------------------------------------------------------------------
uint64_t NTimeUtils::GetClockTicks()
{


	// Get the clock ticks
	LARGE_INTEGER clockTicks = NSharedWindows::ToLargeInteger(0);

	BOOL wasOK = QueryPerformanceCounter(&clockTicks);
	NN_EXPECT(wasOK);

	int64_t theTicks = NSharedWindows::ToInt64(clockTicks);
	NN_REQUIRE(theTicks >= 0);

	if (!wasOK)
	{
		theTicks = 0;
	}

	return uint64_t(theTicks);
}





//=============================================================================
//		NTimeUtils::GetClockFrequency : Get the clock frequency.
//-----------------------------------------------------------------------------
uint64_t NTimeUtils::GetClockFrequency()
{


	// Get the clock frequency
	LARGE_INTEGER clockFreq = NSharedWindows::ToLargeInteger(0);

	BOOL wasOK = QueryPerformanceFrequency(&clockFreq);
	NN_EXPECT(wasOK);

	int64_t theFreq = NSharedWindows::ToInt64(clockFreq);
	NN_REQUIRE(theFreq >= 0);

	if (!wasOK)
	{
		theFreq = 0;
	}

	return uint64_t(theFreq);
}
