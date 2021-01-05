/*	NAME:
		NanoConstants.h

	DESCRIPTION:
		Nano core constants.

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
#ifndef NANO_CONSTANTS_H
#define NANO_CONSTANTS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTypes.h"

// System
#include <limits>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Indices
static constexpr size_t kNNotFound                          = size_t(-1);


// Limits
static constexpr uint8_t  kNUInt8Min                        = std::numeric_limits<uint8_t>::lowest();
static constexpr uint16_t kNUInt16Min                       = std::numeric_limits<uint16_t>::lowest();
static constexpr uint32_t kNUInt32Min                       = std::numeric_limits<uint32_t>::lowest();
static constexpr uint64_t kNUInt64Min                       = std::numeric_limits<uint64_t>::lowest();

static constexpr int8_t  kNInt8Min                          = std::numeric_limits<int8_t>::lowest();
static constexpr int16_t kNInt16Min                         = std::numeric_limits<int16_t>::lowest();
static constexpr int32_t kNInt32Min                         = std::numeric_limits<int32_t>::lowest();
static constexpr int64_t kNInt64Min                         = std::numeric_limits<int64_t>::lowest();

static constexpr float32_t kNFloat32Min                     = std::numeric_limits<float32_t>::lowest();
static constexpr float64_t kNFloat64Min                     = std::numeric_limits<float64_t>::lowest();

static constexpr uint8_t  kNUInt8Max                        = std::numeric_limits<uint8_t>::max();
static constexpr uint16_t kNUInt16Max                       = std::numeric_limits<uint16_t>::max();
static constexpr uint32_t kNUInt32Max                       = std::numeric_limits<uint32_t>::max();
static constexpr uint64_t kNUInt64Max                       = std::numeric_limits<uint64_t>::max();

static constexpr int8_t  kNInt8Max                          = std::numeric_limits<int8_t>::max();
static constexpr int16_t kNInt16Max                         = std::numeric_limits<int16_t>::max();
static constexpr int32_t kNInt32Max                         = std::numeric_limits<int32_t>::max();
static constexpr int64_t kNInt64Max                         = std::numeric_limits<int64_t>::max();

static constexpr float32_t kNFloat32Max                     = std::numeric_limits<float32_t>::max();
static constexpr float64_t kNFloat64Max                     = std::numeric_limits<float64_t>::max();

static constexpr size_t kNSizeMax                           = std::numeric_limits<size_t>::max();


// Memory
static constexpr uint64_t kNKilobyte                        = 1000;
static constexpr uint64_t kNMegabyte                        = 1000 * 1000;
static constexpr uint64_t kNGigabyte                        = 1000 * 1000 * 1000;

static constexpr uint64_t kNKibibyte                        = 1024;
static constexpr uint64_t kNMebibyte                        = 1024 * 1024;
static constexpr uint64_t kNGibibyte                        = 1024 * 1024 * 1024;


// Frequency
static constexpr uint64_t kNHertz                           = 1;
static constexpr uint64_t kNKilohertz                       = 1000;
static constexpr uint64_t kNMegahertz                       = 1000 * 1000;
static constexpr uint64_t kNGigahertz                       = 1000 * 1000 * 1000;


// Intervals
static constexpr NInterval kNTimeNanosecond                 = 1.0 / 1000000000.0;
static constexpr NInterval kNTimeMicrosecond                = 1.0 / 1000000.0;
static constexpr NInterval kNTimeMillisecond                = 1.0 / 1000.0;
static constexpr NInterval kNTimeSecond                     = 1.0;
static constexpr NInterval kNTimeMinute                     = 60.0;
static constexpr NInterval kNTimeHour                       = 60.0 * 60.0;
static constexpr NInterval kNTimeDay                        = 60.0 * 60.0 * 24.0;
static constexpr NInterval kNTimeWeek                       = 60.0 * 60.0 * 24.0 * 7.0;
static constexpr NInterval kNTimeMonthish                   = 60.0 * 60.0 * 24.0 * 7.0 * 30.0;
static constexpr NInterval kNTimeYearish                    = 60.0 * 60.0 * 24.0 * 7.0 * 30.0 * 365.0;

static constexpr NInterval kNTimeNone                       = 0.0;
static constexpr NInterval kNTimeForever                    = -1.0;
static constexpr NInterval kNTimeDistantFuture              = kNTimeYearish * 4000000000.0;


// NStatus
enum class NStatus
{
	OK,
	Busy,
	Cancelled,
	Checksum,
	DiskFull,
	Duplicate,
	ExhaustedDst,
	ExhaustedSrc,
	Internal,
	Locked,
	Malformed,
	Memory,
	NotFound,
	NotHandled,
	NotSupported,
	Param,
	Permission,
	Timeout,
	TooNew,
	TooOld
};



#endif // NANO_CONSTANTS_H
