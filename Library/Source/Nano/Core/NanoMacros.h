/*	NAME:
		NanoMacros.h

	DESCRIPTION:
		Nano macros.

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
#ifndef NANO_MACROS_H
#define NANO_MACROS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTargets.h"





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Stringify a token
//
// NN_STRINGIFY_TOKEN replaces a token with a string.
//
// NN_STRINGIFY replaces a token with its expansion as a string.
//
// Example:
//
//		#define kMaximum						1000
//
//		NN_STRINGIFY_TOKEN(kMaximum)			"kMaximum"
//		NN_STRINGIFY(      kMaximum)			"1000"
//
//		NN_STRINGIFY_TOKEN(one kMaximum)		"one kMaximum"
//		NN_STRINGIFY(      one kMaximum)		"one 1000"
//
//		NN_STRINGIFY_TOKEN("Maximum")			"\"Maximum\""
//		NN_STRINGIFY(      "Maximum")			"\"Maximum\""
//
#define NN_STRINGIFY_TOKEN(_token)                          #_token
#define NN_STRINGIFY(_token)                                NN_STRINGIFY_TOKEN(_token)


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


// Consume a trailing semicolon
//
// Allows a function-like macro to consume a trailing semicolon.
//
// Example:
//
//		#if HAVE_FEATURE
//			#define DO_FEATURE()	DoFeature();
//		#else
//			#define DO_FEATURE()	NN_CONSUME_SEMICOLON()
//		#endif
//
//		void Function()
//		{
//			DO_FEATURE();
//		}
//
#define NN_CONSUME_SEMICOLON()                              struct _nano_consume_trailing_semicolon


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
#define NN_DIAGNOSTIC_PUSH()                                NN_CONSUME_SEMICOLON()
#define NN_DIAGNOSTIC_POP()                                 NN_CONSUME_SEMICOLON()
#define NN_DIAGNOSTIC_IGNORE_CLANG(_warning)                NN_CONSUME_SEMICOLON()
#define NN_DIAGNOSTIC_IGNORE_GCC(_warning)                  NN_CONSUME_SEMICOLON()
#define NN_DIAGNOSTIC_IGNORE_MSVC(_warning)                 NN_CONSUME_SEMICOLON()

#if NN_COMPILER_CLANG
	#undef NN_DIAGNOSTIC_PUSH
	#undef NN_DIAGNOSTIC_POP
	#undef NN_DIAGNOSTIC_IGNORE_CLANG

	#define NN_DIAGNOSTIC_PUSH()                            _Pragma("clang diagnostic push") NN_CONSUME_SEMICOLON()
	#define NN_DIAGNOSTIC_POP()                             _Pragma("clang diagnostic pop") NN_CONSUME_SEMICOLON()
	#define NN_DIAGNOSTIC_IGNORE_CLANG(_warning)            \
		_Pragma(NN_STRINGIFY_TOKEN(clang diagnostic ignored _warning)) NN_CONSUME_SEMICOLON()

#elif NN_COMPILER_GCC
	#undef NN_DIAGNOSTIC_PUSH
	#undef NN_DIAGNOSTIC_POP
	#undef NN_DIAGNOSTIC_IGNORE_GCC

	#define NN_DIAGNOSTIC_PUSH()                            _Pragma("GCC diagnostic push") NN_CONSUME_SEMICOLON()
	#define NN_DIAGNOSTIC_POP()                             _Pragma("GCC diagnostic pop") NN_CONSUME_SEMICOLON()
	#define NN_DIAGNOSTIC_IGNORE_GCC(_warning)              \
		_Pragma(NN_STRINGIFY_TOKEN(GCC diagnostic ignored _warning)) NN_CONSUME_SEMICOLON()

#elif NN_COMPILER_MSVC
	#undef NN_DIAGNOSTIC_PUSH
	#undef NN_DIAGNOSTIC_POP
	#undef NN_DIAGNOSTIC_IGNORE_MSVC

	#define NN_DIAGNOSTIC_PUSH()                            __pragma(warning(push))
	#define NN_DIAGNOSTIC_POP()                             __pragma(warning(pop))
	#define NN_DIAGNOSTIC_IGNORE_MSVC(_warning)             __pragma(warning(disable : _warning))
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


// Expand a token
//
// Allow MSVC to expand __VA_ARGS__ to multiple tokens when passed
// between macros, rather than passing it on as a single token.
//
//		https://devblogs.microsoft.com/cppblog/msvc-preprocessor-progress-towards-conformance/
//
// Expansion only appears to be necessary when the outer macro only
// takes ... and uses __VA_ARGS__ to pass on its arguments.
//
// If the outer macro takes a named parameter, and uses ##__VA_ARGS__
// to consume trailing commas, no expansion is necessary.
//
// Example:
//
//		#define OUTER(...)  NN_EXPAND(INNER(__VA_ARGS__))
//
#define NN_EXPAND(_token)                                   _token



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



// Declare a function as a C symbol
//
// Example:
//
//		NN_EXTERN_C void SomeFunction(uint32_t theValue);
//
#if defined(__cplusplus)
	#define NN_EXTERN_C                                     extern "C"
#else
	#define NN_EXTERN_C
#endif



// Function name
//
// The name of the current function.
//
// Example:
//
//		const char* MyFunction()
//		{
//			return NN_FUNCTION;
//		}
//
#if NN_COMPILER_MSVC
	#define NN_FUNCTION                                     __FUNCSIG__
#else
	#define NN_FUNCTION                                     __PRETTY_FUNCTION__
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



// Indicate support for CTAD
//
// Marks a type as opting-in to class template argument deduction.
//
// Example:
//
//		template<typename T>
//		class NClassName
//		{
//			...
//		}
//
//		NN_SUPPORTS_CTAD(NClassName);
//
// Some compilers allow CTAD to be made "opt-in" by issuing a warning for
// types that appear to support CTAD but have no user defined deduction
// guides.
//
// Clang enables this behaviour through the -Wctad-maybe-unsupported warning.
//
#if NN_COMPILER_CLANG
	#define NN_SUPPORTS_CTAD(_ClassName)                    \
		template<class _Tag = void>                         \
		_ClassName(typename _Tag::__supports_ctad)->_ClassName<void>

#else
	#define NN_SUPPORTS_CTAD(_ClassName)                    static_assert(true)
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
// Although a constexpr if should discard any expression in the untaken
// branch, both gcc and MSVC will warn about unused / set-but-unused
// variables within the discarded expression.
//
// However all compilers will elide the expression if placed in a normal
// branch that is never taken, even in debug builds.
//
#define NN_UNUSED(_expression)                              \
	do                                                      \
	{                                                       \
		if (false)                                          \
		{                                                   \
			(void) (_expression);                           \
		}                                                   \
	} while (false)



// Mark as unreachable
//
// Marks code as unreachable, avoiding an unnecessary gcc -Wreturn-type
// warning when switching over a class enum.
//
// Example:
//
//	enum class Foo { A, B };
//
//	int Function(Foo f)
//	{
//		switch (f)
//		{
//			case Foo::A:
//				return 1;
//			case Foo::B:
//				return 2;
//		}
//
//		NN_UNREACHABLE();
//	}
//
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define NN_UNREACHABLE()                                __builtin_unreachable()
#elif NN_COMPILER_MSVC
	#define NN_UNREACHABLE()                                __assume(0)
#else
	#error "Unable to implement NN_UNREACHABLE!"
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



#endif // NANO_MACROS_H
