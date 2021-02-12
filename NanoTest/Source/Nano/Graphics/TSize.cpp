/*	NAME:
		TSize.cpp

	DESCRIPTION:
		NSize tests.

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
#include "NFormat.h"
#include "NSize.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TSize)
{
	NSize theSize;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "Default")
{


	// Perform the test
	REQUIRE(theSize.width == 0.0);
	REQUIRE(theSize.height == 0.0);
	REQUIRE(sizeof(theSize) == 16);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "Constructor")
{


	// Perform the test
	theSize = NSize(1, 2);
	REQUIRE(theSize.width == 1.0);
	REQUIRE(theSize.height == 2.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "Clear")
{


	// Perform the test
	theSize = NSize(1, 2);
	REQUIRE(theSize.width == 1.0);
	REQUIRE(theSize.height == 2.0);

	theSize.Clear();
	REQUIRE(theSize.width == 0.0);
	REQUIRE(theSize.height == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "IsEmpty")
{


	// Perform the test
	REQUIRE(theSize.IsEmpty());

	theSize = NSize(1, 2);
	REQUIRE(!theSize.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "GetIntegral")
{


	// Perform the test
	theSize = NSize(1, 2);
	REQUIRE(theSize.GetIntegral() == NSize(1, 2));

	theSize = NSize(1.4, 2.5);
	REQUIRE(theSize.GetIntegral() == NSize(2, 3));

	theSize = NSize(1.5, 2.6);
	REQUIRE(theSize.GetIntegral() == NSize(2, 3));


	theSize = NSize(1, 2);
	theSize.MakeIntegral();
	REQUIRE(theSize == NSize(1, 2));

	theSize = NSize(1.4, 2.5);
	theSize.MakeIntegral();
	REQUIRE(theSize == NSize(2, 3));

	theSize = NSize(1.5, 2.6);
	theSize.MakeIntegral();
	REQUIRE(theSize == NSize(2, 3));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "GetNormalized")
{


	// Perform the test
	theSize = NSize(1, 2);
	REQUIRE(theSize.GetNormalized() == NSize(1, 2));

	theSize = NSize(-1, 2);
	REQUIRE(theSize.GetNormalized() == NSize(1, 2));

	theSize = NSize(-1, -2);
	REQUIRE(theSize.GetNormalized() == NSize(1, 2));


	theSize = NSize(1, 2);
	theSize.Normalize();
	REQUIRE(theSize == NSize(1, 2));

	theSize = NSize(-1, 2);
	theSize.Normalize();
	REQUIRE(theSize == NSize(1, 2));

	theSize = NSize(-1, -2);
	theSize.Normalize();
	REQUIRE(theSize == NSize(1, 2));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "GetScaled")
{


	// Perform the test
	theSize = NSize(1, 2);
	REQUIRE(theSize.GetScaled(2) == NSize(2, 4));

	theSize = NSize(1.5, 2.1);
	REQUIRE(theSize.GetScaled(2) == NSize(3, 4.2));

	theSize = NSize(-1.1, -2.4);
	REQUIRE(theSize.GetScaled(2) == NSize(-2.2, -4.8));


	theSize = NSize(1, 2);
	theSize.Scale(2);
	REQUIRE(theSize == NSize(2, 4));

	theSize = NSize(1.5, 2.1);
	theSize.Scale(2);
	REQUIRE(theSize == NSize(3, 4.2));

	theSize = NSize(-1.1, -2.4);
	theSize.Scale(2);
	REQUIRE(theSize == NSize(-2.2, -4.8));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "GetResized")
{


	// Perform the test
	theSize = NSize(1, 2);
	REQUIRE(theSize.GetResized(3, 4) == NSize(4, 6));

	theSize = NSize(1.5, 2.1);
	REQUIRE(theSize.GetResized(3.1, 7.1) == NSize(4.6, 9.2));

	theSize = NSize(-1.5, -2.4);
	REQUIRE(theSize.GetResized(1, -2.1) == NSize(-0.5, -4.5));


	theSize = NSize(1, 2);
	theSize.Resize(3, 4);
	REQUIRE(theSize == NSize(4, 6));

	theSize = NSize(1.5, 2.1);
	theSize.Resize(3.1, 7.1);
	REQUIRE(theSize == NSize(4.6, 9.2));

	theSize = NSize(-1.5, -2.4);
	theSize.Resize(1, -2.1);
	REQUIRE(theSize == NSize(-0.5, -4.5));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "CompareEqual")
{


	// Perform the test
	NSize sizeA(3.8, 1.3);
	NSize sizeB(8.1, 4.9);

	REQUIRE(sizeA == sizeA);
	REQUIRE(sizeA != sizeB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "CompareOrder")
{


	// Perform the test
	NSize sizeA(3.8, 1.3);
	NSize sizeB(8.1, 4.9);

	REQUIRE(sizeA < sizeB);
	REQUIRE(sizeB > sizeA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSize, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", NSize()) == "{width = 0, height = 0}");
	REQUIRE(NFormat("{}", NSize(1.5, 2)) == "{width = 1.5, height = 2}");
	REQUIRE(NFormat("{}", NSize(1, -2.5)) == "{width = 1, height = -2.5}");
}
