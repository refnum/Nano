/*	NAME:
		NDebug.h

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
#ifndef NDEBUG_H
#define NDEBUG_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoMacros.h"

#if defined(__cplusplus)
	#include "Nano_fmt.h"
#endif


// System
#if defined(__cplusplus)
	#include <utility>
#endif // defined(__cplusplus)





//=============================================================================
//		Configuration
//-----------------------------------------------------------------------------
// Logging
//
// Logging is enabled in debug builds.
//
#define NN_ENABLE_LOGGING                                   NN_DEBUG


// Asserts
//
// Assertions are enabled when logging is enabled.
//
#define NN_ENABLE_ASSERTIONS                                NN_ENABLE_LOGGING





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDebug.inl"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Log levels
//
// The log level indicates the priority of the message.
enum NLogLevel
{
	kNLogLevelInfo,
	kNLogLevelWarning,
	kNLogLevelError
};





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
// Log with std::format-style formatting
#if defined(__cplusplus)
void NanoLogFormatArgs(NLogLevel                           logLevel,
					   const char*                         filePath,
					   int                                 lineNum,
					   const fmt::basic_string_view<char>& formatStr,
					   fmt::format_args                    theArgs);

#endif // defined(__cplusplus)


// Log with printf-style formatting
NN_VALIDATE_PRINTF(4, 5)
void NanoLogPrintf(NLogLevel logLevel, const char* filePath, int lineNum, const char* theMsg, ...);


// Log a message
//
// Accepts either std::format-style or printf-style formatting automatically.
//
//		https://fmt.dev/latest/syntax.html
//
//		https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification
//		https://en.cppreference.com/w/c/io/fprintf
//
// Example:
//
//		NanoLog(kNLogLevelInfo, "{} {}", "Hello", "World");
//
//		NanoLog(kNLogLevelInfo, "%s %s", "Hello", "World");
//
// std::format formatting errors are reported as a constexpr compile failure:
//
//		Constexpr variable 'invalid_format' must
//		be initialized by a constant expression.
//
// printf formatting errors are reported as a compile error.
//
#if defined(__cplusplus)

	#define NanoLog(_level, _format, ...)                                                       \
		do                                                                                      \
		{                                                                                       \
			if constexpr (_nn_has_format_specifiers(_format))                                   \
			{                                                                                   \
				NanoLogFormat(_level, __FILE__, __LINE__, FMT_STRING(_format), ##__VA_ARGS__);  \
			}                                                                                   \
			else                                                                                \
			{                                                                                   \
				NN_DIAGNOSTIC_PUSH();                                                           \
				NN_DIAGNOSTIC_IGNORE_CLANG("-Wformat-extra-args");                              \
				NanoLogPrintf(_level, __FILE__, __LINE__, _format, ##__VA_ARGS__);              \
				NN_DIAGNOSTIC_POP();                                                            \
			}                                                                                   \
		} while (false)

#else

	#define NanoLog(_level, _format, ...)                                       \
		do                                                                      \
		{                                                                       \
			NanoLogPrintf(_level, __FILE__, __LINE__, _format, ##__VA_ARGS__);  \
		} while (false)

#endif // defined(__cplusplus)





//=============================================================================
//		Logging
//-----------------------------------------------------------------------------
// Log a message
//
// Example:
//
//		NN_LOG_INFO("This is just information.");
//
//		NN_LOG_WARNING("This is a warning!");
//
//		NN_LOG_ERROR("Something has gone wrong!");
//
#if NN_ENABLE_LOGGING

	#define NN_LOG_INFO(_message, ...)                          \
		do                                                      \
		{                                                       \
			NanoLog(kNLogLevelInfo, _message, ##__VA_ARGS__);   \
		} while (false)

	#define NN_LOG_WARNING(_message, ...)                           \
		do                                                          \
		{                                                           \
			NanoLog(kNLogLevelWarning, _message, ##__VA_ARGS__);    \
		} while (false)

	#define NN_LOG_ERROR(_message, ...)                         \
		do                                                      \
		{                                                       \
			NanoLog(kNLogLevelError, _message, ##__VA_ARGS__);  \
		} while (false)

#else

	#define NN_LOG_INFO(...)                                \
		do                                                  \
		{                                                   \
		} while (false)

	#define NN_LOG_WARNING(...)                             \
		do                                                  \
		{                                                   \
		} while (false)

	#define NN_LOG_ERROR(...)                               \
		do                                                  \
		{                                                   \
		} while (false)

#endif


// Log unimplemented code
//
// Example:
//
//		void FunctionOne()
//		{
//			NN_LOG_UNIMPLEMENTED();
//		}
//
//		void FunctionTwo(bool resetValue)
//		{
//			if (resetValue)
//				NN_LOG_UNIMPLEMENTED("values can't be reset");
//		}
//
#if NN_ENABLE_LOGGING

	#define _nn_log_unimplemented(_message, ...)                                        \
		do                                                                              \
		{                                                                               \
			NN_LOG_WARNING("%s is unimplemented" _message, __func__, ##__VA_ARGS__);    \
		} while (false)

	#define _nn_log_unimplemented_0(...)                    _nn_log_unimplemented("!")

	#define _nn_log_unimplemented_N(_message, ...)          \
		_nn_log_unimplemented(", " _message, ##__VA_ARGS__)

	#define _nn_log_unimplemented_0_TO_N(_0, _1, _2, _3, _4, _5, ...)   _5

	#define NN_LOG_UNIMPLEMENTED(...)                           \
		_nn_log_unimplemented_0_TO_N(__VA_ARGS__,               \
									 _nn_log_unimplemented_N,   \
									 _nn_log_unimplemented_N,   \
									 _nn_log_unimplemented_N,   \
									 _nn_log_unimplemented_N,   \
									 _nn_log_unimplemented_0)(__VA_ARGS__)

#else

	#define NN_LOG_UNIMPLEMENTED(...)                       \
		do                                                  \
		{                                                   \
		} while (false)

#endif


// Conditional logging
//
// Perform a logging expression conditionally.
//
// Example:
//
//		NN_CHECK(numKeys < 100, NN_LOG_WARNING("Cache is too large"));
//
#if NN_ENABLE_LOGGING

	#define NN_CHECK(_condition, _expression)               \
		do                                                  \
		{                                                   \
			_nn_validate_condition((_condition));           \
															\
			if (NN_UNLIKELY(!(_condition)))                 \
			{                                               \
				_expression;                                \
			}                                               \
		} while (false)

#else

	#define NN_CHECK(_condition, _expression)               \
		do                                                  \
		{                                                   \
			NN_UNUSED(_condition);                          \
		} while (false)

#endif





//=============================================================================
//		Assertions
//-----------------------------------------------------------------------------
// Requirement
//
// A requirement expresses a condition that must be met.
//
// A failed requirement is fatal.
//
// Example:
//
//		void SetPointer(void* thePtr)
//		{
//			NN_REQUIRE(thePtr != nullptr);
//
//			mPtrThatIsDereferencedLater = thePtr;
//		}
//
#if NN_ENABLE_ASSERTIONS

	#define _nn_require(_condition, _message, ...)                                                  \
		do                                                                                          \
		{                                                                                           \
			_nn_validate_condition((_condition));                                                   \
																									\
			if (NN_EXPECT_UNLIKELY(!(_condition)))                                                  \
			{                                                                                       \
				_nn_invoke_assertion(                                                               \
					NN_LOG_ERROR("Requirement failed: %s" _message, #_condition, ##__VA_ARGS__);    \
					NN_DEBUG_BREAK(););                                                             \
			}                                                                                       \
		} while (false)

	#define _nn_require_1(_condition)                       _nn_require(_condition, "")

	#define _nn_require_N(_condition, _message, ...)        \
		_nn_require(_condition, ", " _message, ##__VA_ARGS__)

	#define _nn_require_1_TO_N(_0, _1, _2, _3, _4, _5, ...) _5

	#define NN_REQUIRE(...)                                 \
		_nn_require_1_TO_N(__VA_ARGS__,                     \
						   _nn_require_N,                   \
						   _nn_require_N,                   \
						   _nn_require_N,                   \
						   _nn_require_N,                   \
						   _nn_require_1)(__VA_ARGS__)

#else

	#define NN_REQUIRE(_condition, ...)                     \
		do                                                  \
		{                                                   \
			NN_UNUSED(_condition);                          \
		} while (false)

#endif



// Expectation
//
// An expectation expresses a condition that should be met.
//
// A failed expectation is not fatal.
//
// Example:
//
//		void SetPointer(void* thePtr)
//		{
//			NN_EXPECT(thePtr != nullptr);
//
//			mPtrThatIsCheckedBeforeUse = thePtr;
//		}
//
#if NN_ENABLE_ASSERTIONS

	#define _nn_expect(_condition, _message, ...)                                                   \
		do                                                                                          \
		{                                                                                           \
			_nn_validate_condition((_condition));                                                   \
																									\
			if (NN_EXPECT_UNLIKELY(!(_condition)))                                                  \
			{                                                                                       \
				_nn_invoke_assertion(                                                               \
					NN_LOG_ERROR("Expectation failed: %s" _message, #_condition, ##__VA_ARGS__););  \
			}                                                                                       \
		} while (false)

	#define _nn_expect_1(_condition)                        _nn_expect(_condition, "")

	#define _nn_expect_N(_condition, _message, ...)         \
		_nn_expect(_condition, ", " _message, ##__VA_ARGS__)

	#define _nn_expect_1_TO_N(_0, _1, _2, _3, _4, _5, ...)  _5

	#define NN_EXPECT(...)                                  \
		_nn_expect_1_TO_N(__VA_ARGS__,                      \
						  _nn_expect_N,                     \
						  _nn_expect_N,                     \
						  _nn_expect_N,                     \
						  _nn_expect_N,                     \
						  _nn_expect_1)(__VA_ARGS__)

#else

	#define NN_EXPECT(_condition, ...)                      \
		do                                                  \
		{                                                   \
			NN_UNUSED(_condition);                          \
		} while (false)

#endif


// Helpers
//
// Validate common situations.
//
// Example:
//
//		void SetValue(const char* theValue)
//		{
//
//			// Validate our parameters
//			NN_REQUIRE_NOT_NULL(theValue);
//
//
//			// Set the value
//			NStatus theErr = SaveValueToFile(theValue);
//			NN_EXPECT_NOT_ERR(theErr);
//		}
//
#define NN_REQUIRE_NOT_ERR(_error, ...)                     \
	do                                                      \
	{                                                       \
		NN_REQUIRE((_error) == 0, ##__VA_ARGS__);           \
	} while (false)

#define NN_REQUIRE_NOT_NULL(_pointer, ...)                  \
	do                                                      \
	{                                                       \
		NN_REQUIRE((_pointer) != nullptr, ##__VA_ARGS__);   \
	} while (false)


#define NN_EXPECT_NOT_ERR(_error, ...)                      \
	do                                                      \
	{                                                       \
		NN_EXPECT((_error) == 0, ##__VA_ARGS__);            \
	} while (false)

#define NN_EXPECT_NOT_NULL(_pointer, ...)                   \
	do                                                      \
	{                                                       \
		NN_EXPECT((_pointer) != nullptr, ##__VA_ARGS__);    \
	} while (false)



#endif // NDEBUG_H
