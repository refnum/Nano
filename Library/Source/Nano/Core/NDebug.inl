/*	NAME:
		NDebug.inl

	DESCRIPTION:
		Debug logging and assrtions.

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





//=============================================================================
//		Diagnostics
//-----------------------------------------------------------------------------
// Work around https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53431
//
// Prior to gcc 9.2 using #pragma diagnostic within a macro works to suppress
// warnings works as expected C but has no effect in C++.
//
// Both NanoLog() and NFormat() need to support unused arguments for their
// std::format path, but unused arguments triggers a warning on the printf
// path despite that code being discarded.
//
// We work around that by suppressing that warning around the printf.
//
//
// That workaround has no effect in older gcc's, because it takes place
// within a macro, so for these versions we need to disable this globally.
//
// We do so in code, rather than with a command line flag, so that we can
// still get that warning on non-macro calls to printf in gcc 9.2+.
//
#if defined(__cplusplus) && NN_COMPILER_GCC && NN_COMPILER_GCC < 90201
	NN_DIAGNOSTIC_IGNORE_GCC("-Wformat-extra-args");
#endif





#if defined(__cplusplus)
//=============================================================================
//		NanoLogFormat : Log with std::format-style formatting.
//-----------------------------------------------------------------------------
template <typename L, typename P, typename Ln, typename S, typename... Args>
void NanoLogFormat(L logLevel, P filePath, Ln lineNum, const S& formatStr, Args&&... theArgs)
{


	// Format the string
	//
	// Formatting at the call site is done through an array of arguments, rather
	// than expanding the parameter pack, to minimise template bloat:
	//
	//	https://www.zverovich.net/2017/12/09/improving-compile-times.html
	//	https://www.zverovich.net/2016/11/05/reducing-printf-call-overhead.html
	//
	NanoLogFormatArgs(logLevel,
					  filePath,
					  lineNum,
					  formatStr,
					  {fmt::internal::make_args_checked<Args...>(formatStr, theArgs...)});
}
#endif // defined(__cplusplus)





//=============================================================================
//		_nn_has_format_specifiers : Does a string use std::format specifiers?
//-----------------------------------------------------------------------------
#if defined(__cplusplus)
template <size_t N>
constexpr bool _nn_has_format_specifiers(char const (&theChars)[N]) noexcept
{


	// Check the string
	//
	// Although this can produce false positives its purpose is to aid
	// in the transition from printf-style specifiers to std::format.
	//
	// The side effects of a mis-categorised string are harmless:
	//
	//	o "%s{}" will be seen as a std::format string, but the '%s'
	//    will simply be preserved as-is in the output.
	//
	//  o "{%s}" will also be seen as a std::format string, but will
	//    be rejected at compile time as an invalid format.
	//
	// Either case can be easily adjusted to select std::format or
	// printf-style formatting as required.
	//
	size_t numBraces = 0;

	for (size_t n = 0; n < N; n++)
	{
		switch (theChars[n])
		{
			case '{':
			case '}':
				numBraces += 1;
				break;
		}
	}

	return (numBraces != 0) && ((numBraces % 2) == 0);
}
#else
	#define _nn_has_format_specifiers(...)                  false
#endif // defined(__cplusplus)





//=============================================================================
//		_nn_has_assignment : Does an expression contain an assignment?
//-----------------------------------------------------------------------------
#if defined(__cplusplus)
template <size_t N>
constexpr bool _nn_has_assignment(char const (&theChars)[N])
{


	// Get the state we need
	char prevChar = '?';
	char theChar  = '?';



	// Scan the string
	for (size_t n = 1; n < (N - 1); n++)
	{
		prevChar = theChar;
		theChar  = theChars[n];


		// Reject '++' and '--'
		if (theChar == '+' || theChar == '-')
		{
			if (prevChar == theChar)
			{
				return true;
			}
		}


		// Reject '=', except for '==', '<=', '>=', and '!='
		else if (theChar == '=')
		{
			if (theChars[n + 1] == '=')
			{
				n++;
				continue;
			}

			if (prevChar == '>' || prevChar == '<' || prevChar == '!')
			{
				continue;
			}

			return true;
		}
	}

	return false;
}
#else
	#define _nn_has_assignment(...)                         false
#endif // defined(__cplusplus)





//=============================================================================
//		_nn_is_known_constant : Is an expression a compile-time constant?
//-----------------------------------------------------------------------------
#if NN_COMPILER_CLANG || NN_COMPILER_GCC
	#define _nn_is_known_constant(_expression)              __builtin_constant_p(_expression)
#else
	#define _nn_is_known_constant(_expression)              false
#endif





//=============================================================================
//		_nn_validate_condition : Is an assert condition valid?
//-----------------------------------------------------------------------------
#if defined(__cplusplus)

	#define _nn_validate_condition(_condition)                                          \
		do                                                                              \
		{                                                                               \
			static_assert(!_nn_has_assignment(#_condition), "Unexpected assignment!");  \
			static_assert(!_nn_is_known_constant(_condition), "Use static_assert!");    \
		} while (false)

#else

	#define _nn_validate_condition(_condition)

#endif // defined(__cplusplus)
