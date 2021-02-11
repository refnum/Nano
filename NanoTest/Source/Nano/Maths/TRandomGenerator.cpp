/*	NAME:
		TRandomGenerator.cpp

	DESCRIPTION:
		NRandomGenerator tests.

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
#include "NRandomGenerator.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TRandomGenerator){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRandomGenerator, "GetStream")
{


	// Perform the test
	NRandomGenerator theGenerator(123);
	REQUIRE(theGenerator.GetStream().GetSeed() == 123);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRandomGenerator, "MinMax")
{


	// Perform the test
	NRandomGenerator theGenerator;

	REQUIRE(theGenerator.min() == 0);
	REQUIRE(theGenerator.max() == kNUInt64Max);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRandomGenerator, "Invoke")
{


	// Perform the test
	NRandomGenerator theGenerator;

	REQUIRE(theGenerator() != 0);
	REQUIRE(theGenerator() != 123456789);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TRandomGenerator, "Shuffle")
{


	// Perform the test
	NVectorUInt32    theVector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	NRandomGenerator theGenerator;

	std::shuffle(theVector.begin(), theVector.end(), theGenerator);

	size_t numDisplaced = 0;
	for (size_t n = 0; n < theVector.size(); n++)
	{
		if (theVector[n] != n)
		{
			numDisplaced++;
		}
	}

	REQUIRE(numDisplaced >= 5);
}
