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
#include "NanoTargets.h"
#include "NanoTypes.h"

// System
#include <limits>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Indices
inline constexpr size_t kNNotFound                          = size_t(-1);


// Limits
inline constexpr uint8_t  kNUInt8Min                        = std::numeric_limits<uint8_t>::lowest();
inline constexpr uint16_t kNUInt16Min                       = std::numeric_limits<uint16_t>::lowest();
inline constexpr uint32_t kNUInt32Min                       = std::numeric_limits<uint32_t>::lowest();
inline constexpr uint64_t kNUInt64Min                       = std::numeric_limits<uint64_t>::lowest();

inline constexpr int8_t  kNInt8Min                          = std::numeric_limits<int8_t>::lowest();
inline constexpr int16_t kNInt16Min                         = std::numeric_limits<int16_t>::lowest();
inline constexpr int32_t kNInt32Min                         = std::numeric_limits<int32_t>::lowest();
inline constexpr int64_t kNInt64Min                         = std::numeric_limits<int64_t>::lowest();

inline constexpr float32_t kNFloat32Min                     = std::numeric_limits<float32_t>::lowest();
inline constexpr float64_t kNFloat64Min                     = std::numeric_limits<float64_t>::lowest();

inline constexpr uint8_t  kNUInt8Max                        = std::numeric_limits<uint8_t>::max();
inline constexpr uint16_t kNUInt16Max                       = std::numeric_limits<uint16_t>::max();
inline constexpr uint32_t kNUInt32Max                       = std::numeric_limits<uint32_t>::max();
inline constexpr uint64_t kNUInt64Max                       = std::numeric_limits<uint64_t>::max();

inline constexpr int8_t  kNInt8Max                          = std::numeric_limits<int8_t>::max();
inline constexpr int16_t kNInt16Max                         = std::numeric_limits<int16_t>::max();
inline constexpr int32_t kNInt32Max                         = std::numeric_limits<int32_t>::max();
inline constexpr int64_t kNInt64Max                         = std::numeric_limits<int64_t>::max();

inline constexpr float32_t kNFloat32Max                     = std::numeric_limits<float32_t>::max();
inline constexpr float64_t kNFloat64Max                     = std::numeric_limits<float64_t>::max();

inline constexpr size_t kNSizeMax                           = std::numeric_limits<size_t>::max();


// Memory
inline constexpr uint64_t kNByte                            = 1;

inline constexpr uint64_t kNKilobyte                        = 1000 * kNByte;
inline constexpr uint64_t kNMegabyte                        = 1000 * kNKilobyte;
inline constexpr uint64_t kNGigabyte                        = 1000 * kNMegabyte;
inline constexpr uint64_t kNTerabyte                        = 1000 * kNGigabyte;

inline constexpr uint64_t kNKibibyte                        = 1024 * kNByte;
inline constexpr uint64_t kNMebibyte                        = 1024 * kNKibibyte;
inline constexpr uint64_t kNGibibyte                        = 1024 * kNMebibyte;
inline constexpr uint64_t kNTebibyte                        = 1024 * kNGibibyte;


// Frequency
inline constexpr uint64_t kNHertz                           = 1;
inline constexpr uint64_t kNKilohertz                       = 1000;
inline constexpr uint64_t kNMegahertz                       = 1000 * 1000;
inline constexpr uint64_t kNGigahertz                       = 1000 * 1000 * 1000;


// Intervals
inline constexpr NInterval kNTimeNanosecond                 = 1.0 / 1000000000.0;
inline constexpr NInterval kNTimeMicrosecond                = 1.0 / 1000000.0;
inline constexpr NInterval kNTimeMillisecond                = 1.0 / 1000.0;
inline constexpr NInterval kNTimeSecond                     = 1.0;
inline constexpr NInterval kNTimeMinute                     = 60.0;
inline constexpr NInterval kNTimeHour                       = 60.0 * 60.0;
inline constexpr NInterval kNTimeDay                        = 60.0 * 60.0 * 24.0;
inline constexpr NInterval kNTimeWeek                       = 60.0 * 60.0 * 24.0 * 7.0;
inline constexpr NInterval kNTimeMonthish                   = 60.0 * 60.0 * 24.0 * 7.0 * 30.0;
inline constexpr NInterval kNTimeYearish                    = 60.0 * 60.0 * 24.0 * 7.0 * 30.0 * 365.0;

inline constexpr NInterval kNTimeNone                       = 0.0;
inline constexpr NInterval kNTimeForever                    = -1.0;
inline constexpr NInterval kNTimeDistantFuture              = kNTimeYearish * 4000000000.0;


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


// MSVC warnings
#if NN_COMPILER_MSVC
	#define C4127_conditional_expression_is_constant            4127
	#define C4146_unary_minus_applied_to_unsigned               4146
	#define C4355_this_used_in_base_member_initializer          4355
	#define C4365_signed_unsigned_mismatch                      4365
	#define C4388_signed_unsigned_mismatch                      4388
	#define C4464_relative_include_path_contains_dotdot         4464
	#define C4474_too_many_arguments_for_format_string          4474
	#define C4476_unknown_type_field_character_in_format        4476
	#define C4582_constructor_not_implicitly_called             4582
	#define C4583_destructor_is_not_implicitly_called           4583
	#define C4619_unknown_warning                               4619
	#define C4668_undefined_macro_treated_as_zero               4668
	#define C4840_non_pod_argument_passed_to_variadic_function  4840
	#define C4868_left_to_right_evaluation_order_undefined      4868
	#define C4996_deprecated_function                           4996
	#define C5204_destructor_is_not_virtual                     5204
	#define C5219_implicit_conversion                           5219
#endif // NN_COMPILER_MSVC



#endif // NANO_CONSTANTS_H
