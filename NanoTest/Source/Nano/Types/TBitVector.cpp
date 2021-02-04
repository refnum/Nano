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
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TBitVector)
{
	NBitVector theVector;

	SETUP
	{
		theVector.AppendBit(true);
		theVector.AppendBit(false);
		theVector.AppendBit(true);
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "Manipulate")
{


	// Perform the test
	REQUIRE(theVector.GetSize() == 3);

	theVector.FlipBits();
	REQUIRE(theVector.GetBit(0) == false);
	REQUIRE(theVector.GetBit(1) == true);
	REQUIRE(theVector.GetBit(2) == false);
	REQUIRE(theVector.CountBits(false) == 2);
	REQUIRE(theVector.CountBits(true) == 1);
	REQUIRE(theVector.GetBits().GetSize() == 1);
	REQUIRE(theVector.GetBits(0, 3) == 0x02);

	theVector.FlipBit(1);
	REQUIRE(theVector.GetBit(1) == false);

	theVector.FlipBit(1);
	REQUIRE(theVector.FindFirstBit(true) == 1);
	REQUIRE(theVector.FindLastBit(true) == 1);
	REQUIRE(theVector.FindFirstBit(false) == 0);
	REQUIRE(theVector.FindLastBit(false) == 2);

	theVector.SetBit(1, true);
	REQUIRE(theVector.GetBit(1) == true);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "Set")
{


	// Perform the test
	theVector.SetBits(false);
	REQUIRE(!theVector.ContainsBit(true));
	REQUIRE(theVector.ContainsBit(false));

	theVector.SetBits(true);
	REQUIRE(theVector.ContainsBit(true));
	REQUIRE(!theVector.ContainsBit(false));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TBitVector, "Clear")
{


	// Perform the test
	theVector.Clear();
	REQUIRE(theVector.IsEmpty());
}
