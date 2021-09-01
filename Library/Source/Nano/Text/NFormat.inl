/*	NAME:
		NFormat.inl

	DESCRIPTION:
		String formatting.

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
//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
#define FMT_HEADER_ONLY





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NColor.h"
#include "NDate.h"
#include "NFile.h"
#include "NFileInfo.h"
#include "NFilePath.h"
#include "NNumber.h"
#include "NPoint.h"
#include "NRectangle.h"
#include "NSize.h"
#include "NTimeUtils.h"
#include "NURL.h"
#include "NUTI.h"
#include "NVector.h"
#include "NanoMacros.h"


// Fmtlib
NN_DIAGNOSTIC_PUSH();
NN_DIAGNOSTIC_IGNORE_CLANG("-Wsigned-enum-bitfield");
NN_DIAGNOSTIC_IGNORE_MSVC(C4464_relative_include_path_contains_dotdot);
NN_DIAGNOSTIC_IGNORE_MSVC(C4582_constructor_not_implicitly_called);

#include "fmt_printf.h"

NN_DIAGNOSTIC_POP();





//=============================================================================
//		Internal Prototypes
//-----------------------------------------------------------------------------
NString NFormatArgsToString(const fmt::basic_string_view<char>& formatStr,
							fmt::format_args                    theArgs);

NString NSprintfArgsToString(const fmt::basic_string_view<char>&         formatStr,
							 fmt::basic_format_args<fmt::printf_context> theArgs);





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
// Check printf arguments
//
// The test branch is never taken so never generate code, however it allows
// us to validate the supplied arguments against printf.
//
// As std::format allows additional behaviour beyond printf we must suppress
// some warnings that are accepted by std::format by rejected by printf.
//
// These include additional arguments, positional parameters, and non-POD
// arguments.
//
// For GCC we currently skip this validation entirely due to a bug when
// evaluating _Pragma within a macro:
//
//	https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55578
//
#if NN_COMPILER_GCC
	#define NN_FORMAT_CHECK_PRINTF(_format, ...)            \
		do                                                  \
		{                                                   \
		} while (false)

#else
	#define NN_FORMAT_CHECK_PRINTF(_format, ...)                                                \
		do                                                                                      \
		{                                                                                       \
			if constexpr (false)                                                                \
			{                                                                                   \
				NN_DIAGNOSTIC_PUSH();                                                           \
				NN_DIAGNOSTIC_IGNORE_CLANG("-Wformat-extra-args");                              \
				NN_DIAGNOSTIC_IGNORE_CLANG("-Wformat-non-iso");                                 \
				NN_DIAGNOSTIC_IGNORE_GCC("-Wformat-extra-args");                                \
				NN_DIAGNOSTIC_IGNORE_MSVC(C4474_too_many_arguments_for_format_string);          \
				NN_DIAGNOSTIC_IGNORE_MSVC(C4476_unknown_type_field_character_in_format);        \
				NN_DIAGNOSTIC_IGNORE_MSVC(C4840_non_pod_argument_passed_to_variadic_function);  \
				printf(_format, ##__VA_ARGS__);                                                 \
				NN_DIAGNOSTIC_POP();                                                            \
			}                                                                                   \
		} while (false)

#endif





//=============================================================================
//		NFormatPackToString : Format a parameter pack to an NString.
//-----------------------------------------------------------------------------
template<typename S, typename... Args>
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
	return NFormatArgsToString(formatStr, {fmt::make_args_checked<Args...>(formatStr, theArgs...)});
}





//=============================================================================
//		NSprintfPackToString : Sprintf a parameter pack to an NString.
//-----------------------------------------------------------------------------
template<typename S, typename... Args>
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





#pragma mark NSimpleFormatter
//=============================================================================
//		NSimpleFormatter::parse : Simple std::formatter parsing.
//-----------------------------------------------------------------------------
constexpr auto NSimpleFormatter::parse(fmt::format_parse_context& theContext) const
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
	if (theIter != theContext.end() && *theIter != '}')
	{
		throw fmt::format_error("Unexpected specifier!");
	}

	return theIter;
}





#pragma mark NColor
//=============================================================================
//		NColor formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NColor> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NColor& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(),
						 "{{r = {:.2g}, g = {:.2g}, b = {:.2g}, a = {:.2g}}}",
						 theParam.GetRed(),
						 theParam.GetGreen(),
						 theParam.GetBlue(),
						 theParam.GetAlpha());
	}
};





#pragma mark NDate
//=============================================================================
//		NDate formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NDate> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NDate& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(),
						 "{:04d}-{:02d}-{:02d}",
						 theParam.GetYear(),
						 theParam.GetMonth(),
						 theParam.GetDay());
	}
};





#pragma mark NFile
//=============================================================================
//		NFile formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NFile> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NFile& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{}", theParam.GetPath().GetUTF8());
	}
};





#pragma mark NFileInfo
//=============================================================================
//		NFileInfo formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NFileInfo> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NFileInfo& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{}", theParam.GetPath().GetUTF8());
	}
};





#pragma mark NFilePath
//=============================================================================
//		NFilePath formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NFilePath> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NFilePath& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{}", theParam.GetUTF8());
	}
};





#pragma mark NNumber
//=============================================================================
//		NNumber formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NNumber> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NNumber& theParam, FormatContext& theContext)
	{
		if (theParam.IsReal())
		{
			return format_to(theContext.out(), "{}", theParam.GetFloat64());
		}
		else if (theParam.IsSigned())
		{
			return format_to(theContext.out(), "{}", theParam.GetInt64());
		}
		else
		{
			return format_to(theContext.out(), "{}", theParam.GetUInt64());
		}
	}
};





#pragma mark NPoint
//=============================================================================
//		NPoint formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NPoint> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NPoint& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{{x = {}, y = {}}}", theParam.x, theParam.y);
	}
};





#pragma mark NRange
//=============================================================================
//		NRange formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NRange> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NRange& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(),
						 "{{location = {}, size = {}}}",
						 theParam.GetLocation(),
						 theParam.GetSize());
	}
};





#pragma mark NRectangle
//=============================================================================
//		NRectangle formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NRectangle> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NRectangle& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(),
						 "{{origin = {{x = {}, y = {}}}, size = {{width = {}, height = {}}}}}",
						 theParam.origin.x,
						 theParam.origin.y,
						 theParam.size.width,
						 theParam.size.height);
	}
};





#pragma mark NSize
//=============================================================================
//		NSize formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NSize> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NSize& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(),
						 "{{width = {}, height = {}}}",
						 theParam.width,
						 theParam.height);
	}
};





#pragma mark NString
//=============================================================================
//		NString formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NString> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NString& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{}", theParam.GetUTF8());
	}
};





#pragma mark NTime
//=============================================================================
//		NTime formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NTime> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NTime& theParam, FormatContext& theContext)
	{
		struct tm timeLocal = NTimeUtils::ToTmUTC(theParam);

		return format_to(theContext.out(),
						 "{:04d}-{:02d}-{:02d}T{:02d}:{:02d}:{:02d}Z",
						 timeLocal.tm_year + 1900,
						 timeLocal.tm_mon + 1,
						 timeLocal.tm_mday,
						 timeLocal.tm_hour,
						 timeLocal.tm_min,
						 timeLocal.tm_sec);
	}
};





#pragma mark NUTI
//=============================================================================
//		NUTI formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NUTI> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NUTI& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{}", theParam.GetType().GetUTF8());
	}
};





#pragma mark NURL
//=============================================================================
//		NURL formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NURL> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NURL& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{}", theParam.GetURL().GetUTF8());
	}
};





#pragma mark NVector
//=============================================================================
//		NVector formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NVector> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NVector& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), "{{x = {}, y = {}}}", theParam.x, theParam.y);
	}
};
