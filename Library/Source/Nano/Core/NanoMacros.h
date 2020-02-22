/*	NAME:
		NanoMacros.h

	DESCRIPTION:
		Nano macros.

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
#ifndef NANO_MACROS_H
#define NANO_MACROS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTargets.h"





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


// Stringify a token
//
// Example:
//
//		NN_STRINGIFY(test)
//
#define NN_STRINGIFY(_token)                                #_token


// Expand a token
//
// Allow MSVC to expand __VA_ARGS__ to multiple tokens when passed
// between macros, rather than passing it on as a single token.
//
//		https://devblogs.microsoft.com/cppblog/msvc-preprocessor-progress-towards-conformance/
//
// Example:
//
//		NN_EXPAND(OTHER_MACRO(__VA_ARGS))
//
#define NN_EXPAND(_token)                                   _token


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
#if NN_COMPILER_MSVC
	#define NN_ALWAYS_INLINE                                __forceinline
#else
	#define NN_ALWAYS_INLINE                                __attribute__((always_inline))
#endif


// Request EBO
//
// MSVC only performs Empty Base Optimization for classs that derive
// from a single empty base class.
//
// This macro enables EBO for multiple base classes, as per:
//
//		https://devblogs.microsoft.com/cppblog/optimizing-the-layout-of-empty-base-classes-in-vs2015-update-2-3/
//
// Example:
//
//		class NN_EMPTY_BASE DerivedClass
//			: public BaseClass1
//			, public BaseClass1
//		{
//			...
//
#if NN_COMPILER_MSVC
	#define NN_EMPTY_BASE                                   __declspec(empty_bases)
#else
	#define NN_EMPTY_BASE
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


// Break to the debugger
//
// Terminates the process if no debugger is present.
//
// Example:
//
//		void Function(int x)
//		{
//			if (x == 0)
//			{
//				NN_DEBUG_BREAK();
//			}
//		}
//
#if NN_COMPILER_MSVC
	#define NN_DEBUG_BREAK()                                __debugbreak()

#else
	#if NN_ARCH_X86
		#define NN_DEBUG_BREAK()                            __asm__ volatile("int $0x03")

	#elif NN_ARCH_ARM
		#if NN_ARCH_64
			#define NN_DEBUG_BREAK()                        __asm__ volatile(".inst 0xd4200000")
		#else
			#define NN_DEBUG_BREAK()                        __asm__ volatile(".inst 0xe7f001f0")
		#endif
	#endif
#endif


// Mark as unused
//
// Marks a variable, or expression, as unused without evaluation.
//
// Example:
//
//		void Function(int x)
//		{
//			NN_UNUSED(x);
//		}
//
// sizeof() cannot be used with a bitfield or function type, or lambdas
// in an unevaluated context until C++20. As such we have two approaches.
//
// For C++ we can use a constexpr if where we place the expression in
// the discarded branch.
//
// For C we fall back to sizeof(), even though this cannot be used with
// certain types as above.
//
#if defined(__cplusplus)

	#define NN_UNUSED(_expression)                          \
		do                                                  \
		{                                                   \
			if constexpr (true)                             \
			{                                               \
				/* Do nothing */                            \
			}                                               \
			else                                            \
			{                                               \
				(_expression);                              \
			}                                               \
		} while (false)

#else

	#define NN_UNUSED(_expression)                          \
		do                                                  \
		{                                                   \
			(void) sizeof((_expression));                   \
		} while (false)

#endif


// Format validation
//
// Example:
//
//		NN_VALIDATE_PRINTF(1, 2)
//		void Print(const char* theMsg, ...);
//
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define NN_VALIDATE_PRINTF(_formatIndex, _firstParamIndex)  \
		__attribute__((format(printf, _formatIndex, _firstParamIndex)))
#else
	#define NN_VALIDATE_PRINTF(_formatIndex, _firstParamIndex)
#endif


// Compiler diagnostics
//
// Example:
//
//		NN_DIAGNOSTIC_PUSH();
//		NN_DIAGNOSTIC_IGNORE_CLANG("-Wold-style-cast");
//
//		uint8_t* thePtr = (uint8_t*) otherPtr;
//
//		NN_DIAGNOSTIC_POP();
//
#define NN_DIAGNOSTIC_IGNORE_CLANG(_warning)
#define NN_DIAGNOSTIC_IGNORE_GCC(_warning)
#define NN_DIAGNOSTIC_IGNORE_MSVC(_warning)

#if NN_COMPILER_CLANG
	#define NN_DIAGNOSTIC_IGNORE(_warning)                  _Pragma(NN_STRINGIFY(clang diagnostic ignored _warning))
	#define NN_DIAGNOSTIC_PUSH()                            _Pragma(NN_STRINGIFY(clang diagnostic push))
	#define NN_DIAGNOSTIC_POP()                             _Pragma(NN_STRINGIFY(clang diagnostic pop))

	#undef  NN_DIAGNOSTIC_IGNORE_CLANG
	#define NN_DIAGNOSTIC_IGNORE_CLANG(_warning)            NN_DIAGNOSTIC_IGNORE(_warning)

#elif NN_COMPILER_GCC
	#define NN_DIAGNOSTIC_IGNORE(_warning)                  _Pragma(NN_STRINGIFY(GCC diagnostic ignored _warning))
	#define NN_DIAGNOSTIC_PUSH()                            _Pragma(NN_STRINGIFY(GCC diagnostic push))
	#define NN_DIAGNOSTIC_POP()                             _Pragma(NN_STRINGIFY(GCC diagnostic pop))

	#undef  NN_DIAGNOSTIC_IGNORE_GCC
	#define NN_DIAGNOSTIC_IGNORE_GCC(_warning)              NN_DIAGNOSTIC_IGNORE(_warning)

#elif NN_COMPILER_MSVC
	#define NN_DIAGNOSTIC_IGNORE(_warning)                  __pragma(warning(disable : _warning))
	#define NN_DIAGNOSTIC_PUSH()                            __pragma(warning(push))
	#define NN_DIAGNOSTIC_POP()                             __pragma(warning(pop))

	#undef  NN_DIAGNOSTIC_IGNORE_MSVC
	#define NN_DIAGNOSTIC_IGNORE_MSVC(_warning)             NN_DIAGNOSTIC_IGNORE(_warning)

#else
	#define NN_DIAGNOSTIC_IGNORE(_warning)
	#define NN_DIAGNOSTIC_PUSH()
	#define NN_DIAGNOSTIC_POP()
#endif



// Packed structure
//
// Set the packing applied to structure fields.
//
// Example:
//
//		NN_STRUCT_PACK_1(MyStructure
//		{
//			uint8_t		value1;
//			uint16_t	value2;
//		});
//
// Packing affects the internal layout of a structure.
//
// The alignas() specifier can be used to set the alignment
// of the structure itself:
//
//		NN_STRUCT_PACK_1(alignas(16) MyStructure
//		{
//			uint8_t		value1;
//			uint16_t	value2;
//		});
//
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define _nn_struct_pack(_packing, ...)                              \
		_Pragma(NN_STRINGIFY(pack(push, _packing))) struct __VA_ARGS__; \
		_Pragma(NN_STRINGIFY(pack(pop)))

#elif NN_COMPILER_MSVC
	#define _nn_struct_pack(_packing, ...)                  \
		__pragma(pack(push, _packing)) struct __VA_ARGS__ __pragma(pack(pop))

#else
	#error "Unable to implement NN_STRUCT_PACK_x macros!"
#endif

#define NN_STRUCT_PACK_1(...)                               _nn_struct_pack(1, __VA_ARGS__)
#define NN_STRUCT_PACK_2(...)                               _nn_struct_pack(2, __VA_ARGS__)
#define NN_STRUCT_PACK_4(...)                               _nn_struct_pack(4, __VA_ARGS__)
#define NN_STRUCT_PACK_8(...)                               _nn_struct_pack(8, __VA_ARGS__)
#define NN_STRUCT_PACK_16(...)                              _nn_struct_pack(16, __VA_ARGS__)



#endif // NANO_MACROS_H
