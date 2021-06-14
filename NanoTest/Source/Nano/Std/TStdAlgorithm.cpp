/*	NAME:
		TStdAlgorithm.cpp

	DESCRIPTION:
		NStdAlgorithm tests.

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
#include "NStdAlgorithm.h"
#include "NTestFixture.h"

// System
#include <map>
#include <unordered_map>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint32_t kTestArray[]                      = {1, 2, 3};

static const NVectorUInt32 kTestVector1                     = {1, 2, 3};
static const NVectorUInt32 kTestVector2                     = {4, 5, 6, 7, 8, 9};

static const std::map<uint32_t, uint32_t> kTestMap{{1u, 111u}, {2u, 222u}, {3u, 333u}};

static const std::unordered_map<uint32_t, uint32_t> kTestUnorderedMap{{3u, 333u},
																	  {2u, 222u},
																	  {1u, 111u}};

static const std::set<uint32_t> kTestSet{1, 2, 3};

static const std::unordered_set<uint32_t> kTestUnorderedSet{3, 2, 1};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TStdAlgorithm){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:appended")
{


	// Perform the test
	auto theVector = nstd::appended(kTestVector1, kTestVector2);

	REQUIRE(theVector.size() == 9);
	REQUIRE(theVector[0] == 1);
	REQUIRE(theVector[2] == 3);
	REQUIRE(theVector[3] == 4);
	REQUIRE(theVector[8] == 9);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:contains")
{


	// Perform the test
	for (auto n : kTestVector1)
	{
		uint32_t valueGood = n;
		uint32_t valueBad  = n + (10 * n);

		REQUIRE(nstd::contains(kTestMap, valueGood));
		REQUIRE(!nstd::contains(kTestMap, valueBad));

		REQUIRE(nstd::contains(kTestUnorderedMap, valueGood));
		REQUIRE(!nstd::contains(kTestUnorderedMap, valueBad));

		REQUIRE(nstd::contains(kTestSet, valueGood));
		REQUIRE(!nstd::contains(kTestSet, valueBad));

		REQUIRE(nstd::contains(kTestUnorderedSet, valueGood));
		REQUIRE(!nstd::contains(kTestUnorderedSet, valueBad));

		REQUIRE(nstd::contains(kTestVector1, valueGood));
		REQUIRE(!nstd::contains(kTestVector1, valueBad));

		REQUIRE(nstd::contains(kTestArray, valueGood));
		REQUIRE(!nstd::contains(kTestArray, valueBad));
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:equal")
{


	// Perform the test
	REQUIRE(nstd::equal(kTestVector1, kTestVector1));
	REQUIRE(!nstd::equal(kTestVector1, kTestVector2));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:erase")
{


	// Perform the test
	auto theVector = kTestVector1;
	REQUIRE(theVector[0] == 1);

	nstd::erase(theVector, 1);

	REQUIRE(theVector.size() == 2);
	REQUIRE(theVector[0] == 2);
	REQUIRE(theVector[1] == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:erase_if")
{


	// Perform the test
	auto theVector = kTestVector1;
	REQUIRE(theVector[0] != 2);
	REQUIRE(theVector.size() == 3);

	nstd::erase_if(theVector,
				   [](uint32_t theValue)
				   {
					   return theValue != 2;
				   });

	REQUIRE(theVector.size() == 1);
	REQUIRE(theVector[0] == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:extract_back")
{


	// Perform the test
	auto theVector = kTestVector1;
	auto theValue  = nstd::extract_back(theVector);

	REQUIRE(theVector.size() == 2);
	REQUIRE(theVector[0] == 1);
	REQUIRE(theVector[1] == 2);
	REQUIRE(theValue == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:extract_front")
{


	// Perform the test
	auto theVector = kTestVector1;
	auto theValue  = nstd::extract_front(theVector);

	REQUIRE(theVector.size() == 2);
	REQUIRE(theVector[0] == 2);
	REQUIRE(theVector[1] == 3);
	REQUIRE(theValue == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:fetch")
{


	// Perform the test
	REQUIRE(nstd::fetch(kTestUnorderedMap, 1, 666) == 111);
	REQUIRE(nstd::fetch(kTestUnorderedMap, 5, 666) == 666);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:find")
{


	// Perform the test
	REQUIRE(nstd::find(kTestVector1, 1) != kTestVector1.end());
	REQUIRE(nstd::find(kTestVector1, 5) == kTestVector1.end());

	REQUIRE(nstd::find(kTestUnorderedMap, 1) != kTestUnorderedMap.end());
	REQUIRE(nstd::find(kTestUnorderedMap, 5) == kTestUnorderedMap.end());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:front")
{


	// Perform the test
	NVectorUInt32 vector1 = kTestVector1;
	NVectorUInt32 vector2;

	REQUIRE(!vector1.empty());
	REQUIRE(vector2.empty());

	uint32_t* ptr1 = nstd::front(vector1);
	uint32_t* ptr2 = nstd::front(vector2);

	REQUIRE(ptr1 == &vector1[0]);
	REQUIRE(ptr2 == nullptr);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:keys")
{


	// Perform the test
	NVectorUInt32 theKeys = nstd::keys(kTestMap);

	REQUIRE(theKeys.size() == 3);
	REQUIRE(theKeys[0] == 1);
	REQUIRE(theKeys[1] == 2);
	REQUIRE(theKeys[2] == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:move_back")
{


	// Perform the test
	auto vector1 = kTestVector1;
	auto vector2 = kTestVector1;

	nstd::move_back(vector1, vector2);

	REQUIRE(vector1.size() == 6);
	REQUIRE(vector2.size() == 0);

	REQUIRE(vector1[0] == 1);
	REQUIRE(vector1[1] == 2);
	REQUIRE(vector1[2] == 3);
	REQUIRE(vector1[3] == 1);
	REQUIRE(vector1[4] == 2);
	REQUIRE(vector1[5] == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:pop_front")
{


	// Perform the test
	auto theVector = kTestVector1;
	nstd::pop_front(theVector);

	REQUIRE(theVector.size() == 2);
	REQUIRE(theVector[0] == 2);
	REQUIRE(theVector[1] == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:push_back")
{


	// Perform the test
	auto theVector = kTestVector1;
	nstd::push_back(theVector, kTestVector2);

	REQUIRE(theVector.size() == 9);
	REQUIRE(theVector[0] == 1);
	REQUIRE(theVector[2] == 3);
	REQUIRE(theVector[3] == 4);
	REQUIRE(theVector[8] == 9);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:push_front")
{


	// Perform the test
	auto theVector = kTestVector1;
	nstd::push_front(theVector, 0);

	REQUIRE(theVector.size() == 4);
	REQUIRE(theVector[0] == 0);
	REQUIRE(theVector[1] == 1);
	REQUIRE(theVector[2] == 2);
	REQUIRE(theVector[3] == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:reverse")
{


	// Perform the test
	auto theVector = kTestVector1;

	nstd::reverse(theVector);
	REQUIRE(theVector.size() == 3);
	REQUIRE(theVector[0] == 3);
	REQUIRE(theVector[1] == 2);
	REQUIRE(theVector[2] == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:reversed")
{


	// Perform the test
	auto theVector = nstd::reversed(kTestVector1);

	REQUIRE(theVector.size() == 3);
	REQUIRE(theVector[0] == 3);
	REQUIRE(theVector[1] == 2);
	REQUIRE(theVector[2] == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:search")
{


	// Perform the test
	NVectorUInt32 theNeedle{7, 8};

	auto iterFind   = nstd::search(kTestVector2, theNeedle);
	auto iterResult = kTestVector2.begin() + 3;

	REQUIRE(iterFind == iterResult);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:search/Iterators")
{


	// Perform the test
	NVectorUInt32 theNeedle{7, 8};

	auto haystackBegin = kTestVector2.begin();
	auto haystackEnd   = kTestVector2.end();

	auto needleBegin = theNeedle.begin();
	auto needleEnd   = theNeedle.end();

	auto iterFind   = nstd::search(haystackBegin, haystackEnd, needleBegin, needleEnd);
	auto iterResult = haystackBegin + 3;

	REQUIRE(iterFind == iterResult);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:shuffle")
{


	// Perform the test
	NVectorUInt32 theVector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	nstd::shuffle(theVector);

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





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:sort")
{


	// Perform the test
	NVectorUInt32 theVector{3, 1, 2};
	nstd::sort(theVector);

	REQUIRE(theVector[0] == 1);
	REQUIRE(theVector[1] == 2);
	REQUIRE(theVector[2] == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:sort/Comparator")
{


	// Perform the test
	NVectorUInt32 theVector{3, 1, 2};
	nstd::sort(theVector, std::greater<uint32_t>());

	REQUIRE(theVector[0] == 3);
	REQUIRE(theVector[1] == 2);
	REQUIRE(theVector[2] == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:stable_uniquify")
{


	// Perform the test
	NVectorUInt32 theVector{3, 3, 2, 1, 3, 1, 2, 2};
	nstd::stable_uniquify(theVector);

	REQUIRE(theVector.size() == 3);
	REQUIRE(theVector[0] == 3);
	REQUIRE(theVector[1] == 2);
	REQUIRE(theVector[2] == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:uniquify")
{


	// Perform the test
	NVectorUInt32 theVector{3, 3, 2, 1, 3, 1, 2, 2};
	nstd::uniquify(theVector);

	REQUIRE(theVector.size() == 3);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStdAlgorithm, "nstd:values")
{


	// Perform the test
	NVectorUInt32 theKeys = nstd::values(kTestUnorderedMap);

	nstd::sort(theKeys);
	REQUIRE(theKeys.size() == 3);
	REQUIRE(theKeys[0] == 111);
	REQUIRE(theKeys[1] == 222);
	REQUIRE(theKeys[2] == 333);
}
