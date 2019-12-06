/*	NAME:
		NTestFixture.h

	DESCRIPTION:
		Nano test fixture for catch.

		Catch is a header-only library, distributed under the boost licence.

		It has been included directly with this Nanite as this is compatible
		with the Nano licence.

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
#ifndef NTEST_FIXTURE_H
#define NTEST_FIXTURE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTypes.h"

// Catch
#include "catch.hpp"





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
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
	TEST_CASE_METHOD(Fixture_##_fixture, "Nano/" #_fixture "/" __VA_ARGS__)





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NTestFixture
{
public:
										NTestFixture();
	virtual                            ~NTestFixture();

										NTestFixture(const NTestFixture&) = delete;
	NTestFixture&                       operator=(   const NTestFixture&) = delete;

										NTestFixture(NTestFixture&&) = delete;
	NTestFixture&                       operator=(   NTestFixture&&) = delete;


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
