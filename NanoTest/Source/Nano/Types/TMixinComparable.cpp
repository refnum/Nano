/*	NAME:
		TMixinComparable.cpp

	DESCRIPTION:
		NMixinComparable tests.

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
#include "NMixinComparable.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Class Declaration
//-----------------------------------------------------------------------------
class TestComparable : public NMixinComparable<TestComparable>
{
public:
	TestComparable(int32_t theValue = 0)
		: mValue(theValue)
	{
	}

	bool CompareEqual(const TestComparable& otherObject) const
	{
		return CompareOrder(otherObject) == NComparison::EqualTo;
	}

	NComparison CompareOrder(const TestComparable& otherObject) const
	{
		return NCompare(mValue, otherObject.mValue);
	}

private:
	int32_t                             mValue;
};





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const TestComparable kTestObjectMinusOne(-1);
static const TestComparable kTestObjectZero(0);
static const TestComparable kTestObjectPlusOne(1);





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMixinComparable)
{
	TestComparable                      theObject;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TMixinComparable, "Default")
{
	// Perform the test
										REQUIRE(theObject == kTestObjectZero);
										REQUIRE(theObject != kTestObjectMinusOne);
										REQUIRE(theObject != kTestObjectPlusOne);

										REQUIRE(theObject <= kTestObjectZero);
										REQUIRE(theObject < kTestObjectPlusOne);

										REQUIRE(theObject >= kTestObjectZero);
										REQUIRE(theObject > kTestObjectMinusOne);
}
