/*	NAME:
		NStringFormatter.inl

	DESCRIPTION:
		String formatter.

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
//		Macros
//-----------------------------------------------------------------------------
#define FMT_HEADER_ONLY





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoMacros.h"

NN_DIAGNOSTIC_PUSH();
NN_DIAGNOSTIC_IGNORE_CLANG("-Wsigned-enum-bitfield");
NN_DIAGNOSTIC_IGNORE_MSVC(4464)     // Relative include path contains '..'
NN_DIAGNOSTIC_IGNORE_MSVC(4582);    // Constructor is not implicitly called

#include "../fmt_2020_03_04/printf.h"

NN_DIAGNOSTIC_POP();





//=============================================================================
//		Internal function prototypes
//-----------------------------------------------------------------------------
NString NFormatArgsToString(const fmt::basic_string_view<char>& formatStr,
							fmt::format_args                    theArgs);

NString NSprintfArgsToString(const fmt::basic_string_view<char>&         formatStr,
							 fmt::basic_format_args<fmt::printf_context> theArgs);





//=============================================================================
//		NBaseFormatter::parse : Basic std::formatter parsing.
//-----------------------------------------------------------------------------
constexpr auto NBaseFormatter::parse(fmt::format_parse_context& theContext) const
{


	// Parse the context
	//
	// parse() is passed a character range that contains the portion of
	// the format string that has still to be parsed.
	//
	// E.g., the range will contain "f} - destination" when invoked with:
	//
	//		fmt::format("{:f} - destination", point{1, 2});
	//
	// A formatter should parse its specifiers until it reaches the end
	// of the range, or the closing '}', then return an iterator to the
	// closing '}'.
	//
	// This class assumes no specifiers are supported, so provides a
	// simple base class for objects that format with "{}".
	//
	auto theIter = theContext.begin();

	if (*theIter != '}')
	{
		throw fmt::format_error("Unexpected specifier!");
	}

	return theIter;
}





//=============================================================================
//		NFormatPackToString : Format a parameter pack to an NString.
//-----------------------------------------------------------------------------
template <typename S, typename... Args>
NString NFormatPackToString(const S& formatStr, Args&&... theArgs)
{


	// Format the string
	//
	// Formatting at the call site is done through an array of arguments, rather
	// than expanding the parameter pack, to minimise template bloat:
	//
	//	https://www.zverovich.net/2017/12/09/improving-compile-times.html
	//	https://www.zverovich.net/2016/11/05/reducing-printf-call-overhead.html
	//
	return NFormatArgsToString(formatStr,
							   {fmt::internal::make_args_checked<Args...>(formatStr, theArgs...)});
}





//=============================================================================
//		NSprintfPackToString : Sprintf a parameter pack to an NString.
//-----------------------------------------------------------------------------
template <typename S, typename... Args>
NString NSprintfPackToString(const S& formatStr, Args&&... theArgs)
{


	// Format the string
	try
	{
		return NSprintfArgsToString(formatStr,
									fmt::make_format_args<fmt::printf_context>(theArgs...));
	}
	catch (const std::exception& theErr)
	{
		return NFormatPackToString("sprintf failure: {}\n", theErr.what());
	}
}





//=============================================================================
//		NString formatter
//-----------------------------------------------------------------------------
template <>
class fmt::formatter<NString> : public NBaseFormatter
{
public:
	template <typename FormatContext>
	auto format(const NString& p, FormatContext& ctx)
	{
		return format_to(ctx.out(), p.GetUTF8());
	}
};
