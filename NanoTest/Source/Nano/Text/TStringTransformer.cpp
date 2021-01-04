/*	NAME:
		TStringTransformer.cpp

	DESCRIPTION:
		NStringTransformer tests.

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
#include "NStringTransformer.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NString kTestWordLower                         = "testing";
static const NString kTestWordUpper                         = "TESTING";

static const NString kTestWordsLower                        = "get wordS iN caps";
static const NString kTestWordsUpper                        = "GET WORDS IN CAPS";
static const NString kTestWordsCapitals                     = "Get Words In Caps";

static const NString kTestSentenceLower                     = "get, Sentences? in Caps!";
static const NString kTestSentenceUpper                     = "GET, SENTENCES? IN CAPS!";
static const NString kTestSentenceCapitals                  = "Get, sentences? In caps!";





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(NStringTransformer)
{
	NString theInput;
	NString theOutput;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringTransformer, "LowerCase")
{


	// Perform the test
	REQUIRE(NStringTransformer::Transform(kTestWordLower, kNStringTransformToLower) ==
			kTestWordLower);
	REQUIRE(NStringTransformer::Transform(kTestWordUpper, kNStringTransformToLower) ==
			kTestWordLower);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringTransformer, "UpperCase")
{


	// Perform the test
	REQUIRE(NStringTransformer::Transform(kTestWordLower, kNStringTransformToUpper) ==
			kTestWordUpper);
	REQUIRE(NStringTransformer::Transform(kTestWordUpper, kNStringTransformToUpper) ==
			kTestWordUpper);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringTransformer, "CapitaliseWords")
{


	// Perform the test
	REQUIRE(NStringTransformer::Transform(kTestWordsLower, kNStringTransformCapitalizeWords) ==
			kTestWordsCapitals);
	REQUIRE(NStringTransformer::Transform(kTestWordsUpper, kNStringTransformCapitalizeWords) ==
			kTestWordsCapitals);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringTransformer, "CapitaliseSentences")
{


	// Perform the test
	REQUIRE(
		NStringTransformer::Transform(kTestSentenceLower, kNStringTransformCapitalizeSentences) ==
		kTestSentenceCapitals);
	REQUIRE(
		NStringTransformer::Transform(kTestSentenceUpper, kNStringTransformCapitalizeSentences) ==
		kTestSentenceCapitals);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringTransformer, "Range")
{


	// Perform the test
	REQUIRE(NStringTransformer::Transform("rANge", kNStringTransformToLower, NRange(1, 2)) ==
			"range");
	REQUIRE(NStringTransformer::Transform("rANge", kNStringTransformToUpper, NRange(0, 3)) ==
			"RANge");
}
