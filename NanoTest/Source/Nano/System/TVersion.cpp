/*	NAME:
		TVersion.cpp

	DESCRIPTION:
		NVersion tests.

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
#include "NTestFixture.h"
#include "NVersion.h"





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TVersion)
{
	NVersion theVersion;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "Default")
{


	// Perform the test
	REQUIRE(!theVersion.IsValid());
	REQUIRE(theVersion.GetProduct() == 0);
	REQUIRE(theVersion.GetMajor() == 0);
	REQUIRE(theVersion.GetMinor() == 0);
	REQUIRE(theVersion.GetPatch() == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "IsValid")
{


	// Perform the test
	REQUIRE(!theVersion.IsValid());

	theVersion = NVersion(1, 2, 3);
	REQUIRE(theVersion.IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "Clear")
{


	// Perform the test
	theVersion = NVersion(1, 2, 3);
	REQUIRE(theVersion.IsValid());

	theVersion.Clear();
	REQUIRE(!theVersion.IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "GetParts")
{


	// Perform the test
	theVersion = NVersion(1, 2, 3, 4, "RC5");
	REQUIRE(theVersion.GetProduct() == 1);
	REQUIRE(theVersion.GetMajor() == 2);
	REQUIRE(theVersion.GetMinor() == 3);
	REQUIRE(theVersion.GetPatch() == 4);
	REQUIRE(theVersion.GetTag() == "RC5");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "SetParts")
{


	// Perform the test
	REQUIRE(theVersion.GetProduct() == 0);
	theVersion.SetProduct(1);
	REQUIRE(theVersion.GetProduct() == 1);

	REQUIRE(theVersion.GetMajor() == 0);
	theVersion.SetMajor(2);
	REQUIRE(theVersion.GetMajor() == 2);

	REQUIRE(theVersion.GetMinor() == 0);
	theVersion.SetMinor(3);
	REQUIRE(theVersion.GetMinor() == 3);

	REQUIRE(theVersion.GetPatch() == 0);
	theVersion.SetPatch(4);
	REQUIRE(theVersion.GetPatch() == 4);

	REQUIRE(theVersion.GetTag() == "");
	theVersion.SetTag("RC5");
	REQUIRE(theVersion.GetTag() == "RC5");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "GetString")
{


	// Perform the test
	REQUIRE(theVersion.GetString() == "0.0");

	theVersion = NVersion(1, 2, 3, 4);
	REQUIRE(theVersion.GetString() == "2.3.4");

	theVersion = NVersion(1, 2, 3, 4, "a5");
	REQUIRE(theVersion.GetString() == "2.3.4a5");

	theVersion = NVersion(5, 6, 7, 0, "b8");
	REQUIRE(theVersion.GetString() == "6.7b8");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "SetString")
{


	// Perform the test
	theVersion.SetString("0.0");
	REQUIRE(theVersion.GetProduct() == 0);
	REQUIRE(theVersion.GetMajor() == 0);
	REQUIRE(theVersion.GetMinor() == 0);
	REQUIRE(theVersion.GetPatch() == 0);
	REQUIRE(theVersion.GetTag() == "");

	theVersion.SetString("1.0");
	REQUIRE(theVersion.GetProduct() == 0);
	REQUIRE(theVersion.GetMajor() == 1);
	REQUIRE(theVersion.GetMinor() == 0);
	REQUIRE(theVersion.GetPatch() == 0);
	REQUIRE(theVersion.GetTag() == "");

	theVersion.SetString("1.2");
	REQUIRE(theVersion.GetProduct() == 0);
	REQUIRE(theVersion.GetMajor() == 1);
	REQUIRE(theVersion.GetMinor() == 2);
	REQUIRE(theVersion.GetPatch() == 0);
	REQUIRE(theVersion.GetTag() == "");

	theVersion.SetString("1.2.3");
	REQUIRE(theVersion.GetProduct() == 0);
	REQUIRE(theVersion.GetMajor() == 1);
	REQUIRE(theVersion.GetMinor() == 2);
	REQUIRE(theVersion.GetPatch() == 3);
	REQUIRE(theVersion.GetTag() == "");

	theVersion.SetString("1.2a3");
	REQUIRE(theVersion.GetProduct() == 0);
	REQUIRE(theVersion.GetMajor() == 1);
	REQUIRE(theVersion.GetMinor() == 2);
	REQUIRE(theVersion.GetPatch() == 0);
	REQUIRE(theVersion.GetTag() == "a3");

	theVersion.SetString("1.2.3b4");
	REQUIRE(theVersion.GetProduct() == 0);
	REQUIRE(theVersion.GetMajor() == 1);
	REQUIRE(theVersion.GetMinor() == 2);
	REQUIRE(theVersion.GetPatch() == 3);
	REQUIRE(theVersion.GetTag() == "b4");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "Compare")
{


	// Perform the test
	REQUIRE(NVersion::Compare("1.0", "1.0a1") == NComparison::GreaterThan);
	REQUIRE(NVersion::Compare("1.0", "1.0b1") == NComparison::GreaterThan);

	REQUIRE(NVersion::Compare("1.0a1", "1.0") == NComparison::LessThan);
	REQUIRE(NVersion::Compare("1.0b1", "1.0") == NComparison::LessThan);

	REQUIRE(NVersion::Compare("1.0", "1.0.1a1") == NComparison::LessThan);
	REQUIRE(NVersion::Compare("1.0", "2.0") == NComparison::LessThan);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "CompareEqual")
{


	// Perform the test
	REQUIRE(kNOSAndroid_8 == kNOSAndroid_8);
	REQUIRE(kNOSAndroid_8 != kNOSiOS_14);

	REQUIRE(NVersion(0, 1, 2, 3) == NVersion(0, 1, 2, 3));
	REQUIRE(NVersion(0, 1, 2, 3, "a1") != NVersion(0, 1, 2, 3, "a2"));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TVersion, "ComareOrder")
{


	// Perform the test
	REQUIRE(kNOSiOS_13 < kNOSiOS_14);
	REQUIRE(kNOSiOS_14 < kNOSiOS_14_0_1);
	REQUIRE(kNOSiOS_14 < kNOSiOS_14_0_1);
	REQUIRE(kNOSiOS_14_0_1 < kNOSiOS_14_1);
	REQUIRE(kNOSiOS_14_1 <= kNOSiOS_14_1);

	REQUIRE(kNOSmacOS_11_2 > kNOSmacOS_11_1);
	REQUIRE(kNOSmacOS_11_1 > kNOSmacOS_11_0_1);
	REQUIRE(kNOSmacOS_11_0_1 > kNOSmacOS_11);
	REQUIRE(kNOSmacOS_11 >= kNOSmacOS_11);

	REQUIRE(NVersion(0, 1, 2, 0, "a1") < NVersion(0, 1, 2, 0, "a2"));
	REQUIRE(NVersion(0, 1, 2, 0, "a1") < NVersion(0, 1, 2, 0, "b1"));

	REQUIRE(NVersion(0, 1, 2, 3) > NVersion(0, 1, 2));
	REQUIRE(NVersion(0, 1, 2, 3) >= NVersion(0, 1, 2, 3));
}
