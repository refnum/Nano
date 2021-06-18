/*	NAME:
		TCoreGraphics.cpp

	DESCRIPTION:
		NCoreGraphics tests.

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
#include "NCoreGraphics.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static NPoint     kTestNPoint(1, 2);
static NSize      kTestNSize(3, 4);
static NRectangle kTestNRectangle(kTestNPoint, kTestNSize);

static CGPoint kTestCGPoint{1, 2};
static CGSize  kTestCGSize{3, 4};
static CGRect  kTestCGRect{kTestCGPoint, kTestCGSize};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TCoreGraphics){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreGraphics, "ToCG")
{


	// Perform the test
	REQUIRE(ToCG(kTestNPoint) == kTestCGPoint);
	REQUIRE(ToCG(kTestNSize) == kTestCGSize);
	REQUIRE(ToCG(kTestNRectangle) == kTestCGRect);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreGraphics, "ToNN")
{


	// Perform the test
	REQUIRE(ToNN(kTestCGPoint) == kTestNPoint);
	REQUIRE(ToNN(kTestCGSize) == kTestNSize);
	REQUIRE(ToNN(kTestCGRect) == kTestNRectangle);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreGraphics, "Equality")
{


	// Perform the test
	REQUIRE(kTestCGPoint == kTestCGPoint);
	REQUIRE(kTestCGSize == kTestCGSize);
	REQUIRE(kTestCGRect == kTestCGRect);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreGraphics, "Inequality")
{


	// Perform the test
	CGPoint otherCGPoint{11, 22};
	CGSize  otherCGSize{33, 44};
	CGRect  otherCGRect{otherCGPoint, otherCGSize};

	REQUIRE(kTestCGPoint != otherCGPoint);
	REQUIRE(kTestCGSize != otherCGSize);
	REQUIRE(kTestCGRect != otherCGRect);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreGraphics, "CGRectMake")
{


	// Perform the test
	REQUIRE(CGRectMake(CGSizeMake(3, 4)) == CGRectMake(0, 0, 3, 4));
	REQUIRE(CGRectMake(3, 4) == CGRectMake(0, 0, 3, 4));
}
