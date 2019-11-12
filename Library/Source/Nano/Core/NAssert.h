/*	NAME:
		NAssert.h

	DESCRIPTION:
		Assertions.

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
#ifndef NASSERT_H
#define NASSERT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NLog.h"





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Asserts
//
// Assertions are enabled when logging is enabled.
//
#define NN_ENABLE_ASSERTIONS                                NN_ENABLE_LOGGING


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
// The condition expression must not have side effects.
//
#if NN_ENABLE_ASSERTIONS

	#define _nn_require(_condition, _message, ...)                                          \
		do                                                                                  \
		{                                                                                   \
			if (NN_EXPECT_UNLIKELY(!(_condition)))                                          \
			{                                                                               \
				NN_LOG_ERROR("Requirement failed: %s" _message, #_condition, ##VA_ARGS);    \
				__builtin_trap();                                                           \
			}                                                                               \
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
						   _nn_require_1)                   \
		(__VA_ARGS__)

#else
	#define NN_REQUIRE(...)
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
// The condition expression must not have side effects.
//
#if NN_ENABLE_ASSERTIONS

	#define _nn_expect(_condition, _message, ...)                                           \
		do                                                                                  \
		{                                                                                   \
			if (NN_EXPECT_UNLIKELY(!(_condition)))                                          \
			{                                                                               \
				NN_LOG_ERROR("Expectation failed: %s" _message, #_condition, ##VA_ARGS);    \
			}                                                                               \
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
						  _nn_expect_1)                     \
		(__VA_ARGS__)

#else
	#define NN_EXPECT(...)
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
// The condition expression must not have side effects.
//
#define NN_REQUIRE_NOT_ERR(_error, ...)                     NN_REQUIRE((_error) == 0, ##__VA_ARGS__)

#define NN_REQUIRE_NOT_NULL(_pointer, ...)                  NN_REQUIRE((_pointer) != nullptr, ##__VA_ARGS__)


#define NN_EXPECT_NOT_ERR(_error, ...)                      NN_EXPECT((_error) == 0, ##__VA_ARGS__)

#define NN_EXPECT_NOT_NULL(_pointer, ...)                   NN_EXPECT((_pointer) != nullptr, ##__VA_ARGS__)



#endif // NASSERT_H
