/*	NAME:
		TRectangle.cpp

	DESCRIPTION:
		NRectangle tests.

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
#include "NRectangle.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TRectangle)
{
	NRectangle theRect;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "Default")
{


	// Perform the test
	REQUIRE(theRect.origin.IsZero());
	REQUIRE(theRect.size.IsEmpty());
	REQUIRE(sizeof(theRect) == 32);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "Constructor")
{


	// Perform the test
	theRect = NRectangle(1, 2, 3, 4);
	REQUIRE(theRect.origin == NPoint(1, 2));
	REQUIRE(theRect.size == NSize(3, 4));

	theRect = NRectangle(3, 4);
	REQUIRE(theRect.origin == NPoint(0, 0));
	REQUIRE(theRect.size == NSize(3, 4));

	theRect = NRectangle(NPoint(1, 2), NSize(3, 4));
	REQUIRE(theRect.origin == NPoint(1, 2));
	REQUIRE(theRect.size == NSize(3, 4));

	theRect = NRectangle(NSize(3, 4));
	REQUIRE(theRect.origin == NPoint(0, 0));
	REQUIRE(theRect.size == NSize(3, 4));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "Clear")
{


	// Perform the test
	theRect = NRectangle(1, 2, 3, 4);
	REQUIRE(!theRect.origin.IsZero());
	REQUIRE(!theRect.size.IsEmpty());

	theRect.Clear();
	REQUIRE(theRect.origin.IsZero());
	REQUIRE(theRect.size.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "IsEmpty")
{


	// Perform the test
	REQUIRE(theRect.IsEmpty());

	theRect = NRectangle(1, 2);
	REQUIRE(!theRect.IsEmpty());

	theRect = NRectangle(1, 2, 0, 0);
	REQUIRE(theRect.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetPoint")
{


	// Perform the test
	theRect = NRectangle(100, 100);

	REQUIRE(theRect.GetPoint(NPosition::Alert) == NPoint(50, 100.0 / 3.0));
	REQUIRE(theRect.GetPoint(NPosition::Center) == NPoint(50, 50));
	REQUIRE(theRect.GetPoint(NPosition::Left) == NPoint(0, 50));
	REQUIRE(theRect.GetPoint(NPosition::Right) == NPoint(100, 50));
	REQUIRE(theRect.GetPoint(NPosition::Top) == NPoint(50, 0));
	REQUIRE(theRect.GetPoint(NPosition::TopLeft) == NPoint(0, 0));
	REQUIRE(theRect.GetPoint(NPosition::TopRight) == NPoint(100, 0));
	REQUIRE(theRect.GetPoint(NPosition::Bottom) == NPoint(50, 100));
	REQUIRE(theRect.GetPoint(NPosition::BottomLeft) == NPoint(0, 100));
	REQUIRE(theRect.GetPoint(NPosition::BottomRight) == NPoint(100, 100));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetMinMidMax")
{


	// Perform the test
	theRect = NRectangle(10, 10, 90, 90);

	REQUIRE(theRect.GetMinX() == 10);
	REQUIRE(theRect.GetMidX() == 55);
	REQUIRE(theRect.GetMaxX() == 100);

	REQUIRE(theRect.GetMinY() == 10);
	REQUIRE(theRect.GetMidY() == 55);
	REQUIRE(theRect.GetMaxY() == 100);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "Contains")
{


	// Perform the test
	NRectangle rectA(0, 0, 100, 100);
	NRectangle rectB(50, 50, 100, 100);
	NRectangle rectC(200, 200, 100, 100);

	REQUIRE(rectA.Contains(rectA));
	REQUIRE(!rectA.Contains(rectB));
	REQUIRE(!rectA.Contains(rectC));

	REQUIRE(rectA.Contains(rectA.origin));
	REQUIRE(rectA.Contains(rectB.origin));
	REQUIRE(!rectA.Contains(rectC.origin));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "Intersects")
{


	// Perform the test
	NRectangle rectA(0, 0, 100, 100);
	NRectangle rectB(50, 50, 100, 100);
	NRectangle rectC(200, 200, 100, 100);

	REQUIRE(rectA.Intersects(rectA));
	REQUIRE(rectA.Intersects(rectB));
	REQUIRE(!rectA.Intersects(rectC));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetIntegral")
{


	// Perform the test
	theRect = NRectangle(1, 2, 3, 4);
	REQUIRE(theRect.GetIntegral() == NRectangle(1, 2, 3, 4));

	theRect = NRectangle(1.4, 2.5, 3.1, 4.2);
	REQUIRE(theRect.GetIntegral() == NRectangle(1, 2, 4, 5));

	theRect = NRectangle(1.5, 2.6, 3.6, 4.7);
	REQUIRE(theRect.GetIntegral() == NRectangle(1, 2, 5, 6));


	theRect = NRectangle(1, 2, 3, 4);
	theRect.MakeIntegral();
	REQUIRE(theRect == NRectangle(1, 2, 3, 4));

	theRect = NRectangle(1.4, 2.5, 3.1, 4.2);
	theRect.MakeIntegral();
	REQUIRE(theRect == NRectangle(1, 2, 4, 5));

	theRect = NRectangle(1.5, 2.6, 3.6, 4.7);
	theRect.MakeIntegral();
	REQUIRE(theRect == NRectangle(1, 2, 5, 6));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetNormalized")
{


	// Perform the test
	theRect = NRectangle(1, 2, 3, 4);
	REQUIRE(theRect.GetNormalized() == NRectangle(1, 2, 3, 4));

	theRect = NRectangle(-1, 2, -3, 4);
	REQUIRE(theRect.GetNormalized() == NRectangle(-1, 2, 3, 4));

	theRect = NRectangle(-1, -2, -3, -4);
	REQUIRE(theRect.GetNormalized() == NRectangle(-1, -2, 3, 4));


	theRect = NRectangle(1, 2, 3, 4);
	theRect.Normalize();
	REQUIRE(theRect == NRectangle(1, 2, 3, 4));

	theRect = NRectangle(-1, 2, -3, 4);
	theRect.Normalize();
	REQUIRE(theRect == NRectangle(-1, 2, 3, 4));

	theRect = NRectangle(-1, -2, -3, -4);
	theRect.Normalize();
	REQUIRE(theRect == NRectangle(-1, -2, 3, 4));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetOffset")
{


	// Perform the test
	theRect = NRectangle(1, 2, 3, 4);
	REQUIRE(theRect.GetOffset(-1, -2) == NRectangle(0, 0, 3, 4));

	theRect = NRectangle(1.4, 2.5, 3.6, 4.7);
	REQUIRE(theRect.GetOffset(1, 2) == NRectangle(2.4, 4.5, 3.6, 4.7));


	theRect = NRectangle(1, 2, 3, 4);
	theRect.Offset(-1, -2);
	REQUIRE(theRect == NRectangle(0, 0, 3, 4));

	theRect = NRectangle(1.4, 2.5, 3.6, 4.7);
	theRect.Offset(1, 2);
	REQUIRE(theRect == NRectangle(2.4, 4.5, 3.6, 4.7));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetScaled")
{


	// Perform the test
	theRect = NRectangle(1, 2, 1, 2);
	REQUIRE(theRect.GetScaled(2) == NRectangle(0.5, 1, 2, 4));

	theRect = NRectangle(1, 2, 1.5, 2.5);
	REQUIRE(theRect.GetScaled(2) == NRectangle(0.25, 0.75, 3, 5));

	theRect = NRectangle(2, 4, -10, -6);
	REQUIRE(theRect.GetScaled(2) == NRectangle(-3, 1, 20, 12));


	theRect = NRectangle(1, 2, 1, 2);
	theRect.Scale(2);
	REQUIRE(theRect == NRectangle(0.5, 1, 2, 4));

	theRect = NRectangle(1, 2, 1.5, 2.5);
	theRect.Scale(2);
	REQUIRE(theRect == NRectangle(0.25, 0.75, 3, 5));

	theRect = NRectangle(2, 4, -10, -6);
	theRect.Scale(2);
	REQUIRE(theRect == NRectangle(-3, 1, 20, 12));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetInset")
{


	// Perform the test
	theRect = NRectangle(1, 2, 5, 6);
	REQUIRE(theRect.GetInset(1, 1) == NRectangle(2, 3, 3, 4));

	theRect = NRectangle(1.5, 2.5, 15.5, 16.8);
	REQUIRE(theRect.GetInset(2, 2) == NRectangle(3.5, 4.5, 11.5, 12.8));

	theRect = NRectangle(-1.5, -2.5, -15, -12);
	REQUIRE(theRect.GetInset(3, 3) == NRectangle(1.5, 0.5, 9, 6));


	theRect = NRectangle(1, 2, 5, 6);
	theRect.Inset(1, 1);
	REQUIRE(theRect == NRectangle(2, 3, 3, 4));

	theRect = NRectangle(1.5, 2.5, 15.5, 16.8);
	theRect.Inset(2, 2);
	REQUIRE(theRect == NRectangle(3.5, 4.5, 11.5, 12.8));

	theRect = NRectangle(-1.5, -2.5, -15, -12);
	theRect.Inset(3, 3);
	REQUIRE(theRect == NRectangle(1.5, 0.5, 9, 6));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetScaledToFit")
{


	// Perform the test
	NRectangle largeRect(10, 10, 200, 100);


	theRect = NRectangle(0, 0, 10, 10);
	REQUIRE(theRect.GetScaledToFit(largeRect) == NRectangle(60, 10, 100, 100));

	theRect = NRectangle(30, 70, 10, 20);
	REQUIRE(theRect.GetScaledToFit(largeRect) == NRectangle(85, 10, 50, 100));

	theRect = NRectangle(40, 60, 20, 10);
	REQUIRE(theRect.GetScaledToFit(largeRect) == NRectangle(10, 10, 200, 100));


	theRect = NRectangle(0, 0, 10, 10);
	theRect.ScaleToFit(largeRect);
	REQUIRE(theRect == NRectangle(60, 10, 100, 100));

	theRect = NRectangle(30, 70, 10, 20);
	theRect.ScaleToFit(largeRect);
	REQUIRE(theRect == NRectangle(85, 10, 50, 100));

	theRect = NRectangle(40, 60, 20, 10);
	theRect.ScaleToFit(largeRect);
	REQUIRE(theRect == NRectangle(10, 10, 200, 100));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetPositioned")
{


	// Perform the test
	NRectangle largeRect(10, 10, 200, 100);
	theRect = NRectangle(50, 50);

	REQUIRE(theRect.GetPositioned(NPosition::Alert, largeRect) ==
			NRectangle(85, 26.0 + 2.0 / 3.0, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::Center, largeRect) == NRectangle(85, 35, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::Left, largeRect) == NRectangle(10, 35, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::Right, largeRect) == NRectangle(160, 35, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::Top, largeRect) == NRectangle(85, 10, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::TopLeft, largeRect) == NRectangle(10, 10, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::TopRight, largeRect) == NRectangle(160, 10, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::Bottom, largeRect) == NRectangle(85, 60, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::BottomLeft, largeRect) == NRectangle(10, 60, 50, 50));
	REQUIRE(theRect.GetPositioned(NPosition::BottomRight, largeRect) ==
			NRectangle(160, 60, 50, 50));


	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::Alert, largeRect);
	REQUIRE(theRect == NRectangle(85, 26.0 + 2.0 / 3.0, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::Center, largeRect);
	REQUIRE(theRect == NRectangle(85, 35, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::Left, largeRect);
	REQUIRE(theRect == NRectangle(10, 35, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::Right, largeRect);
	REQUIRE(theRect == NRectangle(160, 35, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::Top, largeRect);
	REQUIRE(theRect == NRectangle(85, 10, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::TopLeft, largeRect);
	REQUIRE(theRect == NRectangle(10, 10, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::TopRight, largeRect);
	REQUIRE(theRect == NRectangle(160, 10, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::Bottom, largeRect);
	REQUIRE(theRect == NRectangle(85, 60, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::BottomLeft, largeRect);
	REQUIRE(theRect == NRectangle(10, 60, 50, 50));

	theRect = NRectangle(50, 50);
	theRect.SetPosition(NPosition::BottomRight, largeRect);
	REQUIRE(theRect == NRectangle(160, 60, 50, 50));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetUnion")
{


	// Perform the test
	NRectangle rectA(0, 0, 100, 100);
	NRectangle rectB(-50, 50, 150, 150);
	NRectangle rectC(200, -200, 100, -350);

	theRect = rectA.GetUnion(rectB);
	REQUIRE(theRect == NRectangle(-50, 0, 150, 200));

	theRect = rectA.GetUnion(rectC);
	REQUIRE(theRect == NRectangle(0, -550, 300, 650));

	theRect = rectB.GetUnion(rectC);
	REQUIRE(theRect == NRectangle(-50, -550, 350, 750));


	theRect = rectA;
	theRect.UnionWith(rectB);
	REQUIRE(theRect == NRectangle(-50, 0, 150, 200));

	theRect = rectA;
	theRect.UnionWith(rectC);
	REQUIRE(theRect == NRectangle(0, -550, 300, 650));

	theRect = rectB;
	theRect.UnionWith(rectC);
	REQUIRE(theRect == NRectangle(-50, -550, 350, 750));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "GetIntersection")
{


	// Perform the test
	NRectangle rectA(0, 0, 100, 100);
	NRectangle rectB(-50, 50, 150, 150);
	NRectangle rectC(200, -200, 100, -350);

	theRect = rectA.GetIntersection(rectB);
	REQUIRE(theRect == NRectangle(0, 50, 100, 50));

	theRect = rectB.GetIntersection(rectA);
	REQUIRE(theRect == NRectangle(0, 50, 100, 50));

	theRect = rectC.GetIntersection(rectC);
	REQUIRE(theRect == NRectangle(200, -550, 100, 350));


	theRect = rectA;
	theRect.IntersectWith(rectB);
	REQUIRE(theRect == NRectangle(0, 50, 100, 50));

	theRect = rectB;
	theRect.IntersectWith(rectA);
	REQUIRE(theRect == NRectangle(0, 50, 100, 50));

	theRect = rectC;
	theRect.IntersectWith(rectC);
	REQUIRE(theRect == NRectangle(200, -550, 100, 350));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "CompareEqual")
{


	// Perform the test
	NRectangle rectA(8.1, 4.9, 1.4, 8.3);
	NRectangle rectB(3.8, 1.3, 3.1, 8.1);

	REQUIRE(rectA == rectA);
	REQUIRE(rectA != rectB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "CompareOrder")
{


	// Perform the test
	NRectangle rectA(8.1, 4.9, 1.4, 8.3);
	NRectangle rectB(3.8, 1.3, 3.1, 8.1);

	REQUIRE(rectA < rectB);
	REQUIRE(rectB > rectA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRectangle, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", NRectangle()) ==
			"{origin = {x = 0, y = 0}, size = {width = 0, height = 0}}");
	REQUIRE(NFormat("{}", NRectangle(1.5, 2)) ==
			"{origin = {x = 0, y = 0}, size = {width = 1.5, height = 2}}");
	REQUIRE(NFormat("{}", NRectangle(1, -2.5, 3, 4)) ==
			"{origin = {x = 1, y = -2.5}, size = {width = 3, height = 4}}");
}
