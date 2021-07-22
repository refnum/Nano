/*	NAME:
		NTestFixture.h

	DESCRIPTION:
		Nano test fixture for catch.

		Catch is a header-only library, distributed under the boost licence.

		It has been included directly with this Nanite as this is compatible
		with the Nano licence.

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
#ifndef NTEST_FIXTURE_H
#define NTEST_FIXTURE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoMacros.h"
#include "NanoTypes.h"

// Catch
NN_DIAGNOSTIC_PUSH();
NN_DIAGNOSTIC_IGNORE_MSVC(4365);    // Signed / unsigned mismatch
NN_DIAGNOSTIC_IGNORE_MSVC(4388);    // Signed / unsigned mismatch
NN_DIAGNOSTIC_IGNORE_MSVC(4583);    // Destructor is not implicitly called
NN_DIAGNOSTIC_IGNORE_MSVC(4868);    // Left-to-right evaluation order
NN_DIAGNOSTIC_IGNORE_MSVC(4996);    // Deprecated function
NN_DIAGNOSTIC_IGNORE_MSVC(5204);    // Destructor is not virtual
NN_DIAGNOSTIC_IGNORE_MSVC(5219);    // Implicit conversion

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"

NN_DIAGNOSTIC_POP();





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Catch Glue
//
// Catch2 invokes test methods via a subclass of the test fixture.
//
// We can't map SETUP and TEARDOWN to the constructor/destructor of the
// test fixture as those macros don't have access to the fixture name.
//
// We also can't invoke SetUp() and TearDown() within the base class of
// all fixtures as the test fixture's cosntructor has yet to run at the
// point that the base class is constructed.
//
// To allow SetUp() and TearDown() to be invoked around the test() method,
// and after the fixture has been fully constructed, we need to clone the
// internal Catch2 class and invoke test() via a wrapper method that can
// do the setup-test-teardown sequence.
//
#define _nano_INTERNAL_CATCH_TEST_CASE_METHOD2(TestName, ClassName, ...)    \
	CATCH_INTERNAL_START_WARNINGS_SUPPRESSION                               \
	CATCH_INTERNAL_SUPPRESS_GLOBALS_WARNINGS                                \
	namespace                                                               \
	{                                                                       \
	struct TestName : INTERNAL_CATCH_REMOVE_PARENS(ClassName)               \
	{                                                                       \
		void InvokeTest();                                                  \
		void test();                                                        \
	};                                                                      \
	Catch::AutoReg INTERNAL_CATCH_UNIQUE_NAME(autoRegistrar)(               \
		Catch::makeTestInvoker(&TestName::InvokeTest),                      \
		CATCH_INTERNAL_LINEINFO,                                            \
		#ClassName,                                                         \
		Catch::NameAndTags{__VA_ARGS__});                                   \
	}                                                                       \
	CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION                                \
																			\
	void TestName::InvokeTest()                                             \
	{                                                                       \
		SetUp();                                                            \
		test();                                                             \
		TearDown();                                                         \
	}                                                                       \
																			\
	void TestName::test()

#define _nano_INTERNAL_CATCH_TEST_CASE_METHOD(ClassName, ...)       \
	_nano_INTERNAL_CATCH_TEST_CASE_METHOD2(                         \
		INTERNAL_CATCH_UNIQUE_NAME(____C_A_T_C_H____T_E_S_T____),   \
		ClassName,                                                  \
		__VA_ARGS__)

#define _nano_TEST_CASE_METHOD(className, ...)              \
	_nano_INTERNAL_CATCH_TEST_CASE_METHOD(className, __VA_ARGS__)



// Fixture
//
// A fixture derived from NTestFixture can be defined with:
//
//		NANO_FIXTURE(SomeFixture)
//		{
//			void* someData;
//
//			SETUP
//			{
//				someData = nullptr;
//			}
//
//			TEARDOWN
//			{
//				if (someData != nullptr)
//					free(someData);
//			}
//		};
//
// SETUP is optional, and is executed before the test.
//
// TEARDOWN is optional, and is executed after the test.
//
#define NANO_FIXTURE(_fixture)                              struct Fixture_##_fixture : public NTestFixture
#define SETUP                                               void SetUp() final
#define TEARDOWN                                            void TearDown() final



// Tests
//
// A test that uses a fixture can be defined with:
//
//		NANO_TEST(SomeFixture, "Allocate")
//		{
//			someData = malloc(1);
//			REQUIRE(someData != nullptr;
//		}
//
// This will create a "Nano/SomeFixture/Allocate" test.
//
//
// A list of []-delimited tags can also be provided:
//
//		NANO_TEST(SomeFixture, "Allocate", "[memory]")
//		{
//			someData = malloc(1);
//			REQUIRE(someData != nullptr;
//		}
//
// Tags may be used to group tests for execution.
//
#define NANO_TEST(_fixture, ...)                            \
	_nano_TEST_CASE_METHOD(Fixture_##_fixture, "Nano/" #_fixture "/" __VA_ARGS__)


// Errors
#define REQUIRE_NOT_ERR(_error)                             REQUIRE(!_nn_is_error(_error))





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NTestFixture
{
public:
										NTestFixture();
	virtual                            ~NTestFixture() = default;

										NTestFixture(const NTestFixture& otherFixture) = delete;
	NTestFixture&                       operator=(   const NTestFixture& otherFixture) = delete;

										NTestFixture(NTestFixture&& otherFixture) = delete;
	NTestFixture&                       operator=(   NTestFixture&& otherFixture) = delete;


	// Pre/post test hooks
	virtual void                        SetUp();
	virtual void                        TearDown();


	// Get the elapsed time
	NInterval                           GetTime() const;


	// Check the elapsed time
	bool                                TimeUnder(NInterval theTime) const;


private:
	NInterval                           mTimeStart;
};



#endif // NTEST_FIXTURE_H
