/*	NAME:
		TMixinAttributes.cpp

	DESCRIPTION:
		NMixinAttributes tests.

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
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinAttributes.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class TestAttributes : public NMixinAttributes<TestAttributes, uint32_t>
{
};





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint32_t kTestAttribute1 = uint32_t(1) << 0;
static constexpr uint32_t kTestAttribute2 = uint32_t(1) << 1;
static constexpr uint32_t kTestAttribute3 = uint32_t(1) << 30;
static constexpr uint32_t kTestAttribute4 = uint32_t(1) << 31;

static constexpr uint32_t kTestAttributesNone = uint32_t(0);
static constexpr uint32_t kTestAttributesAll  = ~kTestAttributesNone;





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMixinAttributes)
{
	TestAttributes                      theObject;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMixinAttributes, "Default")
{
	// Perform the test
										REQUIRE(theObject.GetAttributes() == kTestAttributesNone);
										REQUIRE(!theObject.HasAttribute(kTestAttribute1));
										REQUIRE(!theObject.HasAttribute(kTestAttribute2));
										REQUIRE(!theObject.HasAttribute(kTestAttribute3));
										REQUIRE(!theObject.HasAttribute(kTestAttribute4));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMixinAttributes, "Set")
{


	// Perform the test
	theObject.SetAttributes(kTestAttribute1 | kTestAttribute3);
	REQUIRE(theObject.HasAttribute(kTestAttribute1));
	REQUIRE(!theObject.HasAttribute(kTestAttribute2));
	REQUIRE(theObject.HasAttribute(kTestAttribute3));
	REQUIRE(!theObject.HasAttribute(kTestAttribute4));

	theObject.SetAttributes(kTestAttribute2 | kTestAttribute4);
	REQUIRE(theObject.HasAttribute(kTestAttribute1));
	REQUIRE(theObject.HasAttribute(kTestAttribute2));
	REQUIRE(theObject.HasAttribute(kTestAttribute3));
	REQUIRE(theObject.HasAttribute(kTestAttribute4));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMixinAttributes, "Clear")
{


	// Perform the test
	theObject.SetAttributes(kTestAttribute1 | kTestAttribute2 | kTestAttribute3 | kTestAttribute4);
	theObject.SetAttributes(kTestAttributesNone, kTestAttribute1 | kTestAttribute4);
	REQUIRE(!theObject.HasAttribute(kTestAttribute1));
	REQUIRE(theObject.HasAttribute(kTestAttribute2));
	REQUIRE(theObject.HasAttribute(kTestAttribute3));
	REQUIRE(!theObject.HasAttribute(kTestAttribute4));

	theObject.ClearAttributes(kTestAttribute2 | kTestAttribute3);
	REQUIRE(!theObject.HasAttribute(kTestAttribute1));
	REQUIRE(!theObject.HasAttribute(kTestAttribute2));
	REQUIRE(!theObject.HasAttribute(kTestAttribute3));
	REQUIRE(!theObject.HasAttribute(kTestAttribute4));

	theObject.SetAttributes(kTestAttribute1 | kTestAttribute2);
	theObject.SetAttributes(kTestAttribute3 | kTestAttribute4, kTestAttributesAll);
	REQUIRE(!theObject.HasAttribute(kTestAttribute1));
	REQUIRE(!theObject.HasAttribute(kTestAttribute2));
	REQUIRE(theObject.HasAttribute(kTestAttribute3));
	REQUIRE(theObject.HasAttribute(kTestAttribute4));

	theObject.SetAttributes(kTestAttributesAll);
	REQUIRE(theObject.GetAttributes() == kTestAttributesAll);
	REQUIRE(theObject.HasAttribute(kTestAttribute1));
	REQUIRE(theObject.HasAttribute(kTestAttribute2));
	REQUIRE(theObject.HasAttribute(kTestAttribute3));
	REQUIRE(theObject.HasAttribute(kTestAttribute4));

	theObject.ClearAttributes();
	REQUIRE(theObject.GetAttributes() == kTestAttributesNone);
	REQUIRE(!theObject.HasAttribute(kTestAttribute1));
	REQUIRE(!theObject.HasAttribute(kTestAttribute2));
	REQUIRE(!theObject.HasAttribute(kTestAttribute3));
	REQUIRE(!theObject.HasAttribute(kTestAttribute4));
}
