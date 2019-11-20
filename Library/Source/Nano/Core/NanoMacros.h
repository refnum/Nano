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
// Get a clamped value
#define NN_CLAMP_VALUE(_v, _min, _max)                      std::min((_max), std::max((_min), (_v)))


// Interpolate between values
#define NN_LERP_VALUE(_start, _end, _progress)              \
	(((_start) * (1.0f - (_progress))) + ((_end) * (_progress)))


// Is an address aligned?
#define NN_ALIGNED_TO(_value, _size)                        ((((uintptr_t) _value) % _size) == 0)


// Is a bit set?
#define NN_TEST_BIT(_value, _mask)                          (((_value) & (_mask)) == (_mask))



// Force inlining
//
// Example:
//
//		NN_ALWAYS_INLINE
//		int Function()
//		{
//			return 1;
//		}
//
#if NN_COMPILER_MSC
	#define NN_ALWAYS_INLINE                                __forceinline
#else
	#define NN_ALWAYS_INLINE                                __attribute__((always_inline))
#endif


// Indicate an expression is likely
//
// Example:
//
//		if (NN_EXPECT_LIKELY(theFlag == true))
//		{
//			Function();
//		}
//
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define NN_EXPECT_LIKELY(_condition)                    __builtin_expect((_condition), 1)
#else
	#define NN_EXPECT_LIKELY(_condition)                    _condition
#endif


// Indicate an expression is unlikely
//
// Example:
//
//		if (NN_EXPECT_UNLIKELY(theFlag == true))
//		{
//			Function();
//		}
//
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define NN_EXPECT_UNLIKELY(_condition)                  __builtin_expect((_condition), 0)
#else
	#define NN_EXPECT_UNLIKELY(_condition)                  _condition
#endif


// Mark as unused
//
// Example:
//
//		void Function(int x)
//		{
//			NN_UNUSED(x);
//		}
//
#define NN_UNUSED(_x)                                       ((void) (_x))


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



#endif // NANO_MACROS_H
