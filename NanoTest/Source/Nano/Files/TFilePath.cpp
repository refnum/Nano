/*	NAME:
		TFilePath.cpp

	DESCRIPTION:
		NFilePath tests.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#include "NFilePath.h"
#include "NFileUtils.h"
#include "NTestFixture.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const NString kExtension                             = "dat";
static const NString kNameBNoExtension                      = "33333";
static const NString kNameCNoExtension                      = "222";

static const NString kNameA                                 = "four";
static const NString kNameB                                 = kNameBNoExtension + "." + kExtension;
static const NString kNameC                                 = kNameCNoExtension + "." + kExtension;

static const NString kPathTmpDirectory =
	NFileUtils::GetLocation(NFileLocation::AppTemporaries, "TFilePath");

static const NString kPathTmpOne                            = kPathTmpDirectory + kNPathSeparator + "one";
static const NString kPathTmpOneTwo                         = kPathTmpOne + kNPathSeparator + "two";
static const NString kPathTmpOneTwoThree                    = kPathTmpOneTwo + kNPathSeparator + "three";

static const NString kPathTmpA                              = kPathTmpOneTwoThree + kNPathSeparator + kNameA;
static const NString kPathTmpB                              = kPathTmpOneTwo + kNPathSeparator + kNameB;
static const NString kPathTmpC                              = kPathTmpOne + kNPathSeparator + kNameC;





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFilePath)
{
	NFilePath thePath;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "Default")
{


	// Perform the test
	REQUIRE(!thePath.IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "Clear")
{


	// Perform the test
	thePath.SetPath(kPathTmpA);
	REQUIRE(thePath.IsValid());

	thePath.Clear();
	REQUIRE(!thePath.IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetPath")
{


	// Perform the test
	REQUIRE(NFilePath(kPathTmpA).GetPath() == kPathTmpA);
	REQUIRE(NFilePath(kPathTmpB).GetPath() == kPathTmpB);
	REQUIRE(NFilePath(kPathTmpC).GetPath() == kPathTmpC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetPath")
{


	// Perform the test
	thePath.SetPath(kPathTmpA);
	REQUIRE(thePath.GetPath() == kPathTmpA);

	thePath.SetPath(kPathTmpB);
	REQUIRE(thePath.GetPath() == kPathTmpB);

	thePath.SetPath(kPathTmpC);
	REQUIRE(thePath.GetPath() == kPathTmpC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetParent")
{


	// Perform the test
	NString parentA = kPathTmpDirectory + kNPathSeparator + "one" + kNPathSeparator + "two" +
					  kNPathSeparator + "three";
	NString parentB = kPathTmpDirectory + kNPathSeparator + "one" + kNPathSeparator + "two";
	NString parentC = kPathTmpDirectory + kNPathSeparator + "one";

	REQUIRE(NFilePath(kPathTmpA).GetParent() == parentA);
	REQUIRE(NFilePath(kPathTmpB).GetParent() == parentB);
	REQUIRE(NFilePath(kPathTmpC).GetParent() == parentC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetName")
{


	// Perform the test
	REQUIRE(NFilePath(kPathTmpA).GetName() == kNameA);
	REQUIRE(NFilePath(kPathTmpB).GetName() == kNameB);
	REQUIRE(NFilePath(kPathTmpC).GetName() == kNameC);

	REQUIRE(NFilePath(kPathTmpA).GetName(true) == kNameA);
	REQUIRE(NFilePath(kPathTmpB).GetName(true) == kNameB);
	REQUIRE(NFilePath(kPathTmpC).GetName(true) == kNameC);

	REQUIRE(NFilePath(kPathTmpA).GetName(false) == kNameA);
	REQUIRE(NFilePath(kPathTmpB).GetName(false) == kNameBNoExtension);
	REQUIRE(NFilePath(kPathTmpC).GetName(false) == kNameCNoExtension);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetExtension")
{


	// Perform the test
	REQUIRE(NFilePath(kPathTmpA).GetExtension() == "");
	REQUIRE(NFilePath(kPathTmpB).GetExtension() == kExtension);
	REQUIRE(NFilePath(kPathTmpC).GetExtension() == kExtension);
}
