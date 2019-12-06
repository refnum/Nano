/*	NAME:
		NTimeUtils.h

	DESCRIPTION:
		Time object.

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
#ifndef NTIME_UTILS_H
#define NTIME_UTILS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTypes.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Epochs
//
// Epochs are converted using addition:
//
//      unixTime = nanoTime + kNanoEpochTo1970;
//      nanoTime = unixTime + kNanoEpochFrom1970;
//
// The Nano epoch starts at 00:00:00 on 2001/01/01 in UTC.
static constexpr NInterval kNanoEpochTo2001    = 0.0;              // Darwin
static constexpr NInterval kNanoEpochTo1970    = 978307200.0;      // Unix
static constexpr NInterval kNanoEpochTo1904    = 3061152000.0;     // Mac OS 9
static constexpr NInterval kNanoEpochTo1601    = 12622780800.0;    // Windows

static constexpr NInterval kNanoEpochFrom2001  = -kNanoEpochTo2001;    // Darwin
static constexpr NInterval kNanoEpochFrom1970  = -kNanoEpochTo1970;    // Unix
static constexpr NInterval kNanoEpochFrom1904  = -kNanoEpochTo1904;    // Mac OS 9
static constexpr NInterval kNanoEpochFrom1601  = -kNanoEpochTo1601;    // Windows





#endif // NTIME_UTILS_H
