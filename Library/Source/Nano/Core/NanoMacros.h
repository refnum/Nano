/*	NAME:
		NanoMacros.h

	DESCRIPTION:
		Nano macros.

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
#ifndef NANO_MACROS_H
#define NANO_MACROS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTarget.h"





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Mark unused parameters
#define NN_UNUSED(_x)                                       (void) _x


// Get the size of an array
#define NN_ARRAY_SIZE(_a)                                   ((NIndex)(sizeof((_a)) / sizeof((_a)[0])))


// Get a clamped value
#define NN_CLAMP_VALUE(_v, _min, _max)                      std::min((_max), std::max((_min), (_v)))


// Interpolate between values
#define NN_LERP_VALUE(_start, _end, _progress)              \
	(((_start) * (1.0f - (_progress))) + ((_end) * (_progress)))


// Is an address aligned?
#define NN_ALIGNED_TO(_value, _size)                        ((((uintptr_t) _value) % _size) == 0)


// Is a bit set?
#define NN_TEST_BIT(_value, _mask)                          (((_value) & (_mask)) == (_mask))


// Declare a binary constant
//
//		NN_B8( 01010101)									= 85
//		NN_B16(10101010, 01010101)							= 43605
//		NN_B32(10000000, 11111111, 10101010, 01010101)		= 2164238933
//
// Based on a PD implementation by Tom Torfs ("Binary constant macros").
#define _nn_hex(_n)                                         0x##_n##LU

#define _nn_b8(_x)                                                              \
	(uint8_t)(((_x & 0x0000000FLU) ? 1 : 0) | ((_x & 0x000000F0LU) ? 2 : 0) |   \
			  ((_x & 0x00000F00LU) ? 4 : 0) | ((_x & 0x0000F000LU) ? 8 : 0) |   \
			  ((_x & 0x000F0000LU) ? 16 : 0) | ((_x & 0x00F00000LU) ? 32 : 0) | \
			  ((_x & 0x0F000000LU) ? 64 : 0) | ((_x & 0xF0000000LU) ? 128 : 0))

#define NN_B8(_byte1)                                       ((uint8_t) _nn_b8(_nn_hex(_byte1)))

#define NN_B16(_byte1, _byte2)                              \
	((((uint16_t) NN_B8(_byte1)) << 8) | (((uint16_t) NN_B8(_byte2)) << 0))

#define NN_B32(_byte1, _byte2, _byte3, _byte4)                                  \
	((((uint32_t) NN_B8(_byte1)) << 24) | (((uint32_t) NN_B8(_byte2)) << 16) |  \
	 (((uint32_t) NN_B8(_byte3)) << 8) | (((uint32_t) NN_B8(_byte4)) << 0))


// Format validation
//
// Example:
//
//		NN_VALIDATE_PRINTF(1, 2)
//		void Print(const char* theMsg, ...);
//
#if NN_COMPILER_CLANG || NN_COMPILAER_GCC
	#define NN_VALIDATE_PRINTF(_formatIndex, _firstParamIndex)  \
		__attribute__((format(printf, _formatIndex, _firstParamIndex)))
#else
	#define NN_VALIDATE_PRINTF(_formatIndex, _firstParamIndex)
#endif


// Indicate an expression is likely
//
// Example:
//
//		if (NN_EXPECT_LIKELY(theFlag == true))
//		{
//			DoSomething();
//		}
//
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define NN_EXPECT_LIKELY(_condition)                    __builtin_expect((__condition), 1)
#else
	#define NN_EXPECT_LIKELY(_condition)                    __condition
#endif


// Indicate an expression is unlikely
//
// Example:
//
//		if (NN_EXPECT_UNLIKELY(theFlag == true))
//		{
//			DoSomething();
//		}
//
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define NN_EXPECT_UNLIKELY(_condition)                  __builtin_expect((__condition), 0)
#else
	#define NN_EXPECT_UNLIKELY(_condition)                  __condition
#endif



#endif // NANO_MACROS_H
