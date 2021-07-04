/*	NAME:
		TCGShading.cpp

	DESCRIPTION:
		NCGShading tests.

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
#include "NCGShading.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NVectorShadingSample kTestSamples{{0.0, kNColorBlack}, {1.0, kNColorWhite}};
static const NPoint               kTestPoint(1.0, 2.0);
static const CGFloat              kTestRadius(3.0);





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TCGShading)
{
	NCGShading cgShading;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "Default")
{


	// Perform the test
	REQUIRE(!cgShading.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "Constructor/Copy")
{


	// Perform the test
	NCGShading otherShading(cgShading);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "Constructor/Move")
{


	// Perform the test
	NCGShading otherShading(std::move(NCGShading()));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "Assignment/Copy")
{


	// Perform the test
	NCGShading otherShading = cgShading;
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "Assignment/Move")
{


	// Perform the test
	NCGShading otherShading = std::move(NCGShading());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "IsValid")
{


	// Perform the test
	REQUIRE(!cgShading.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetMode")
{


	// Perform the test
	REQUIRE(cgShading.GetMode() == NShadingMode::None);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "SetMode")
{


	// Perform the test
	cgShading.SetMode(NShadingMode::Linear);
	REQUIRE(cgShading.GetMode() == NShadingMode::Linear);

	cgShading.SetMode(NShadingMode::Radial);
	REQUIRE(cgShading.GetMode() == NShadingMode::Radial);

	cgShading.SetMode(NShadingMode::None);
	REQUIRE(cgShading.GetMode() == NShadingMode::None);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetSamples")
{


	// Perform the test
	REQUIRE(cgShading.GetSamples().empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "SetSamples")
{


	// Perform the test
	cgShading.SetSamples(kTestSamples);
	REQUIRE(cgShading.GetSamples() == kTestSamples);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetStartPoint")
{


	// Perform the test
	REQUIRE(cgShading.GetStartPoint().IsZero());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetStartPoint")
{


	// Perform the test
	cgShading.SetStartPoint(kTestPoint);
	REQUIRE(cgShading.GetStartPoint() == kTestPoint);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetStartExtend")
{


	// Perform the test
	REQUIRE(cgShading.GetStartExtend() == false);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "SetStartExtend")
{


	// Perform the test
	cgShading.SetStartExtend(true);
	REQUIRE(cgShading.GetStartExtend() == true);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetStartRadius")
{


	// Perform the test
	cgShading.SetMode(NShadingMode::Radial);
	REQUIRE(cgShading.GetStartRadius() == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "SetStartRadius")
{


	// Perform the test
	cgShading.SetMode(NShadingMode::Radial);
	cgShading.SetStartRadius(kTestRadius);
	REQUIRE(cgShading.GetStartRadius() == kTestRadius);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetEndPoint")
{


	// Perform the test
	REQUIRE(cgShading.GetEndPoint().IsZero());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetEndPoint")
{


	// Perform the test
	cgShading.SetEndPoint(kTestPoint);
	REQUIRE(cgShading.GetEndPoint() == kTestPoint);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetEndExtend")
{


	// Perform the test
	REQUIRE(cgShading.GetEndExtend() == false);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "SetEndExtend")
{


	// Perform the test
	cgShading.SetEndExtend(true);
	REQUIRE(cgShading.GetEndExtend() == true);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "GetEndRadius")
{


	// Perform the test
	cgShading.SetMode(NShadingMode::Radial);
	REQUIRE(cgShading.GetEndRadius() == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "SetEndRadius")
{


	// Perform the test
	cgShading.SetMode(NShadingMode::Radial);
	cgShading.SetEndRadius(kTestRadius);
	REQUIRE(cgShading.GetEndRadius() == kTestRadius);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGShading, "CGShadingRef")
{


	// Perform the test
	cgShading.SetMode(NShadingMode::Linear);
	cgShading.SetSamples(kTestSamples);
	cgShading.SetEndPoint(kTestPoint);
	cgShading.SetEndExtend(true);

	REQUIRE(CGShadingRef(cgShading) != nullptr);
}
