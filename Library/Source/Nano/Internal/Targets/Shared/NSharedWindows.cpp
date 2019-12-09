/*	NAME:
		NSharedWindows.cpp

	DESCRIPTION:
		Windows support.

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
#include "NSharedWindows.h"

// Nano
#include "NTime.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		ToNN : Convert two DWORDs to a uint64_t.
//-----------------------------------------------------------------------------
uint64_t ToNN(DWORD valueHigh, DWORD valueLow)
{
	return (uint64_t(valueHigh) << 32) | uint64_t(valueLow);
}





//=============================================================================
//		NSharedWindows::ToInterval : Convert to an NInterval.
//-----------------------------------------------------------------------------
NInterval NSharedWindows::ToInterval(const FILETIME& fileTime)
{


	// Get the time
	uint64_t hectoNanoSecs = ToNN(fileTime.dwHighDateTime, fileTime.dwLowDateTime);

	NInterval theTime = NInterval(hectoNanoSecs) * (100.0 * kNTimeNanosecond);
	theTime           = kNanoEpochFrom1601;

	return theTime;
}
