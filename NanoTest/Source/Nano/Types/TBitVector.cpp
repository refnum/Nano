/*	NAME:
		TBitVector.cpp

	DESCRIPTION:
		NBitVector tests.

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
#include "NBitVector.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TBitVector)
{
	NBitVector theVector;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "Default")
{


	// Perform the test
	REQUIRE(theVector.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "Clear")
{


	// Perform the test
	theVector.AppendBit(true);
	REQUIRE(!theVector.IsEmpty());

	theVector.Clear();
	REQUIRE(theVector.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "GetSize")
{


	// Perform the test
	REQUIRE(theVector.GetSize() == 0);
	theVector.AppendBit(true);
	REQUIRE(theVector.GetSize() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "SetSize")
{


	// Perform the test
	REQUIRE(theVector.GetSize() == 0);

	theVector.SetSize(10);
	REQUIRE(theVector.GetSize() == 10);


	theVector.SetBits(1);
	REQUIRE(theVector.GetBit(0) == bool(1));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(1));
	REQUIRE(theVector.GetBit(3) == bool(1));
	REQUIRE(theVector.GetBit(4) == bool(1));
	REQUIRE(theVector.GetBit(5) == bool(1));
	REQUIRE(theVector.GetBit(6) == bool(1));
	REQUIRE(theVector.GetBit(7) == bool(1));
	REQUIRE(theVector.GetBit(8) == bool(1));
	REQUIRE(theVector.GetBit(9) == bool(1));

	theVector.SetSize(3);
	REQUIRE(theVector.GetBit(0) == bool(1));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(1));

	theVector.SetSize(10);
	REQUIRE(theVector.GetBit(0) == bool(1));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(1));
	REQUIRE(theVector.GetBit(3) == bool(0));
	REQUIRE(theVector.GetBit(4) == bool(0));
	REQUIRE(theVector.GetBit(5) == bool(0));
	REQUIRE(theVector.GetBit(6) == bool(0));
	REQUIRE(theVector.GetBit(8) == bool(0));
	REQUIRE(theVector.GetBit(9) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "GetCapacity")
{


	// Perform the test
	theVector.SetCapacity(11);
	REQUIRE(theVector.GetCapacity() >= 11);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "SetCapacity")
{


	// Perform the test
	theVector.SetCapacity(11);
	REQUIRE(theVector.GetCapacity() >= 11);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "GetData")
{


	// Perform the test
	REQUIRE(theVector.IsEmpty());
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	NData theData = theVector.GetData();
	REQUIRE(theData.GetSize() == 1);
	REQUIRE(theData.GetData()[0] == 0b00010000);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "SetData")
{


	// Perform the test
	uint8_t theValue = 0b10101010;

	REQUIRE(theVector.IsEmpty());
	theVector.SetData(NData(sizeof(theValue), &theValue));
	REQUIRE(!theVector.IsEmpty());

	REQUIRE(theVector.GetBit(0) == bool(1));
	REQUIRE(theVector.GetBit(1) == bool(0));
	REQUIRE(theVector.GetBit(2) == bool(1));
	REQUIRE(theVector.GetBit(3) == bool(0));
	REQUIRE(theVector.GetBit(4) == bool(1));
	REQUIRE(theVector.GetBit(5) == bool(0));
	REQUIRE(theVector.GetBit(6) == bool(1));
	REQUIRE(theVector.GetBit(7) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "GetBit")
{


	// Perform the test
	theVector.SetSize(9);
	REQUIRE(theVector.GetBit(0) == bool(0));
	REQUIRE(theVector.GetBit(1) == bool(0));
	REQUIRE(theVector.GetBit(2) == bool(0));
	REQUIRE(theVector.GetBit(3) == bool(0));
	REQUIRE(theVector.GetBit(4) == bool(0));
	REQUIRE(theVector.GetBit(5) == bool(0));
	REQUIRE(theVector.GetBit(6) == bool(0));
	REQUIRE(theVector.GetBit(7) == bool(0));
	REQUIRE(theVector.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "SetBit")
{


	// Perform the test
	theVector.SetSize(9);
	theVector.SetBit(0, 0);
	theVector.SetBit(1, 1);
	theVector.SetBit(2, 0);
	theVector.SetBit(3, 1);
	theVector.SetBit(4, 0);
	theVector.SetBit(5, 1);
	theVector.SetBit(6, 0);
	theVector.SetBit(7, 1);
	theVector.SetBit(8, 0);

	REQUIRE(theVector.GetBit(0) == bool(0));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(0));
	REQUIRE(theVector.GetBit(3) == bool(1));
	REQUIRE(theVector.GetBit(4) == bool(0));
	REQUIRE(theVector.GetBit(5) == bool(1));
	REQUIRE(theVector.GetBit(6) == bool(0));
	REQUIRE(theVector.GetBit(7) == bool(1));
	REQUIRE(theVector.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "HasBit")
{


	// Perform the test
	theVector.AppendBit(0);
	REQUIRE(theVector.HasBit(0));
	REQUIRE(!theVector.HasBit(1));

	theVector.Clear();
	theVector.AppendBit(1);
	REQUIRE(!theVector.HasBit(0));
	REQUIRE(theVector.HasBit(1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "SetBits")
{


	// Perform the test
	theVector.SetSize(9);
	theVector.SetBit(0, 0);
	theVector.SetBit(1, 1);
	theVector.SetBit(2, 0);
	theVector.SetBit(3, 1);
	theVector.SetBit(4, 0);
	theVector.SetBit(5, 1);
	theVector.SetBit(6, 0);
	theVector.SetBit(7, 1);
	theVector.SetBit(8, 0);

	theVector.SetBits(1, NRange(3, 4));
	REQUIRE(theVector.GetBit(0) == bool(0));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(0));
	REQUIRE(theVector.GetBit(3) == bool(1));
	REQUIRE(theVector.GetBit(4) == bool(1));
	REQUIRE(theVector.GetBit(5) == bool(1));
	REQUIRE(theVector.GetBit(6) == bool(1));
	REQUIRE(theVector.GetBit(7) == bool(1));
	REQUIRE(theVector.GetBit(8) == bool(0));

	theVector.SetBits(0, NRange(1, 6));
	REQUIRE(theVector.GetBit(0) == bool(0));
	REQUIRE(theVector.GetBit(1) == bool(0));
	REQUIRE(theVector.GetBit(2) == bool(0));
	REQUIRE(theVector.GetBit(3) == bool(0));
	REQUIRE(theVector.GetBit(4) == bool(0));
	REQUIRE(theVector.GetBit(5) == bool(0));
	REQUIRE(theVector.GetBit(6) == bool(0));
	REQUIRE(theVector.GetBit(7) == bool(1));
	REQUIRE(theVector.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "AppendBit")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(0);

	REQUIRE(theVector.GetBit(0) == bool(0));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(0));
	REQUIRE(theVector.GetBit(3) == bool(1));
	REQUIRE(theVector.GetBit(4) == bool(0));
	REQUIRE(theVector.GetBit(5) == bool(1));
	REQUIRE(theVector.GetBit(6) == bool(0));
	REQUIRE(theVector.GetBit(7) == bool(1));
	REQUIRE(theVector.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "FindFirst")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	REQUIRE(theVector.FindFirst(0) == 0);
	REQUIRE(theVector.FindFirst(1) == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "FindLast")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	REQUIRE(theVector.FindLast(0) == 8);
	REQUIRE(theVector.FindLast(1) == 5);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "CountBits")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	REQUIRE(theVector.CountBits(0) == 6);
	REQUIRE(theVector.CountBits(1) == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "CountLeadingBits")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	REQUIRE(theVector.CountLeadingBits(0) == 3);
	REQUIRE(theVector.CountLeadingBits(1) == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "CountTrailingBits")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	REQUIRE(theVector.CountTrailingBits(0) == 3);
	REQUIRE(theVector.CountTrailingBits(1) == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "FlipBit")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	theVector.FlipBit(1);
	theVector.FlipBit(4);
	theVector.FlipBit(7);

	REQUIRE(theVector.GetBit(0) == bool(0));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(0));
	REQUIRE(theVector.GetBit(3) == bool(1));
	REQUIRE(theVector.GetBit(4) == bool(0));
	REQUIRE(theVector.GetBit(5) == bool(1));
	REQUIRE(theVector.GetBit(6) == bool(0));
	REQUIRE(theVector.GetBit(7) == bool(1));
	REQUIRE(theVector.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "FlipBits")
{


	// Perform the test
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(1);
	theVector.AppendBit(0);
	theVector.AppendBit(0);
	theVector.AppendBit(0);

	theVector.FlipBits();

	REQUIRE(theVector.GetBit(0) == bool(1));
	REQUIRE(theVector.GetBit(1) == bool(1));
	REQUIRE(theVector.GetBit(2) == bool(1));
	REQUIRE(theVector.GetBit(3) == bool(0));
	REQUIRE(theVector.GetBit(4) == bool(0));
	REQUIRE(theVector.GetBit(5) == bool(0));
	REQUIRE(theVector.GetBit(6) == bool(1));
	REQUIRE(theVector.GetBit(7) == bool(1));
	REQUIRE(theVector.GetBit(8) == bool(1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "BitwiseNOT")
{


	// Perform the test
	NBitVector vectorA, vectorB;

	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);

	vectorB = ~vectorA;
	REQUIRE(vectorB.GetBit(0) == bool(1));
	REQUIRE(vectorB.GetBit(1) == bool(1));
	REQUIRE(vectorB.GetBit(2) == bool(1));
	REQUIRE(vectorB.GetBit(3) == bool(0));
	REQUIRE(vectorB.GetBit(4) == bool(0));
	REQUIRE(vectorB.GetBit(5) == bool(0));
	REQUIRE(vectorB.GetBit(6) == bool(1));
	REQUIRE(vectorB.GetBit(7) == bool(1));
	REQUIRE(vectorB.GetBit(8) == bool(1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "BitwiseAND")
{


	// Perform the test
	NBitVector vectorA, vectorB, vectorC;

	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);

	vectorB.AppendBit(0);
	vectorB.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(0);
	vectorB.AppendBit(0);

	vectorC = vectorA & vectorB;
	REQUIRE(vectorC.GetBit(0) == bool(0));
	REQUIRE(vectorC.GetBit(1) == bool(0));
	REQUIRE(vectorC.GetBit(2) == bool(0));
	REQUIRE(vectorC.GetBit(3) == bool(1));
	REQUIRE(vectorC.GetBit(4) == bool(0));
	REQUIRE(vectorC.GetBit(5) == bool(1));
	REQUIRE(vectorC.GetBit(6) == bool(0));
	REQUIRE(vectorC.GetBit(7) == bool(0));
	REQUIRE(vectorC.GetBit(8) == bool(0));

	vectorC = vectorA;
	vectorC &= vectorB;
	REQUIRE(vectorC.GetBit(0) == bool(0));
	REQUIRE(vectorC.GetBit(1) == bool(0));
	REQUIRE(vectorC.GetBit(2) == bool(0));
	REQUIRE(vectorC.GetBit(3) == bool(1));
	REQUIRE(vectorC.GetBit(4) == bool(0));
	REQUIRE(vectorC.GetBit(5) == bool(1));
	REQUIRE(vectorC.GetBit(6) == bool(0));
	REQUIRE(vectorC.GetBit(7) == bool(0));
	REQUIRE(vectorC.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "BitwiseOR")
{


	// Perform the test
	NBitVector vectorA, vectorB, vectorC;

	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);

	vectorB.AppendBit(0);
	vectorB.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(0);
	vectorB.AppendBit(0);

	vectorC = vectorA | vectorB;
	REQUIRE(vectorC.GetBit(0) == bool(0));
	REQUIRE(vectorC.GetBit(1) == bool(0));
	REQUIRE(vectorC.GetBit(2) == bool(1));
	REQUIRE(vectorC.GetBit(3) == bool(1));
	REQUIRE(vectorC.GetBit(4) == bool(1));
	REQUIRE(vectorC.GetBit(5) == bool(1));
	REQUIRE(vectorC.GetBit(6) == bool(1));
	REQUIRE(vectorC.GetBit(7) == bool(0));
	REQUIRE(vectorC.GetBit(8) == bool(0));

	vectorC = vectorA;
	vectorC |= vectorB;
	REQUIRE(vectorC.GetBit(0) == bool(0));
	REQUIRE(vectorC.GetBit(1) == bool(0));
	REQUIRE(vectorC.GetBit(2) == bool(1));
	REQUIRE(vectorC.GetBit(3) == bool(1));
	REQUIRE(vectorC.GetBit(4) == bool(1));
	REQUIRE(vectorC.GetBit(5) == bool(1));
	REQUIRE(vectorC.GetBit(6) == bool(1));
	REQUIRE(vectorC.GetBit(7) == bool(0));
	REQUIRE(vectorC.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "BitwiseXOR")
{


	// Perform the test
	NBitVector vectorA, vectorB, vectorC;

	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(1);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);

	vectorB.AppendBit(0);
	vectorB.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(0);
	vectorB.AppendBit(0);

	vectorC = vectorA ^ vectorB;
	REQUIRE(vectorC.GetBit(0) == bool(0));
	REQUIRE(vectorC.GetBit(1) == bool(0));
	REQUIRE(vectorC.GetBit(2) == bool(1));
	REQUIRE(vectorC.GetBit(3) == bool(0));
	REQUIRE(vectorC.GetBit(4) == bool(1));
	REQUIRE(vectorC.GetBit(5) == bool(0));
	REQUIRE(vectorC.GetBit(6) == bool(1));
	REQUIRE(vectorC.GetBit(7) == bool(0));
	REQUIRE(vectorC.GetBit(8) == bool(0));

	vectorC = vectorA;
	vectorC ^= vectorB;
	REQUIRE(vectorC.GetBit(0) == bool(0));
	REQUIRE(vectorC.GetBit(1) == bool(0));
	REQUIRE(vectorC.GetBit(2) == bool(1));
	REQUIRE(vectorC.GetBit(3) == bool(0));
	REQUIRE(vectorC.GetBit(4) == bool(1));
	REQUIRE(vectorC.GetBit(5) == bool(0));
	REQUIRE(vectorC.GetBit(6) == bool(1));
	REQUIRE(vectorC.GetBit(7) == bool(0));
	REQUIRE(vectorC.GetBit(8) == bool(0));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "Append")
{


	// Perform the test
	NBitVector vectorA, vectorB, vectorC;

	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);

	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);

	vectorC = vectorA + vectorB;
	REQUIRE(vectorC.GetBit(0) == bool(0));
	REQUIRE(vectorC.GetBit(1) == bool(0));
	REQUIRE(vectorC.GetBit(2) == bool(0));
	REQUIRE(vectorC.GetBit(3) == bool(1));
	REQUIRE(vectorC.GetBit(4) == bool(1));
	REQUIRE(vectorC.GetBit(5) == bool(1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "CompareEqual")
{


	// Perform the test
	NBitVector vectorA, vectorB;

	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	REQUIRE(vectorB != vectorA);

	vectorB = vectorA;
	REQUIRE(vectorB == vectorA);

	vectorB.AppendBit(1);
	REQUIRE(vectorB != vectorA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "CompareOrder")
{


	// Perform the test
	NBitVector vectorA, vectorB;

	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorA.AppendBit(0);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	vectorB.AppendBit(1);
	REQUIRE(vectorB > vectorA);
	REQUIRE(vectorA < vectorB);
}
