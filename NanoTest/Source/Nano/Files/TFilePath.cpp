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

static const NString kStemA                                 = "four";
static const NString kStemB                                 = "33333";
static const NString kStemC                                 = "222";

static const NString kFilenameA                             = kStemA;
static const NString kFilenameB                             = kStemB + "." + kExtension;
static const NString kFilenameC                             = kStemC + "." + kExtension;

static const NFilePath kPathTmpDirectory =
	NFileUtils::GetLocation(NFileLocation::AppTemporaries, "TFilePath");

static const NFilePath kPathTmpOne                          = kPathTmpDirectory.GetChild("one");
static const NFilePath kPathTmpOneTwo                       = kPathTmpOne.GetChild("two");
static const NFilePath kPathTmpOneTwoThree                  = kPathTmpOneTwo.GetChild("three");

static const NFilePath kPathTmpA                            = kPathTmpOneTwoThree.GetChild(kFilenameA);
static const NFilePath kPathTmpB                            = kPathTmpOneTwo.GetChild(kFilenameB);
static const NFilePath kPathTmpC                            = kPathTmpOne.GetChild(kFilenameC);





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
NANO_TEST(TFilePath, "IsAbsolute")
{


	// Perform the test
	REQUIRE(kPathTmpA.IsAbsolute());
	REQUIRE(kPathTmpB.IsAbsolute());
	REQUIRE(kPathTmpC.IsAbsolute());

	REQUIRE(!NFilePath("one").IsAbsolute());
	REQUIRE(!NFilePath("one").GetChild("two").IsAbsolute());
	REQUIRE(!NFilePath("one").GetChild("two").GetChild("three").IsAbsolute());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "IsRelative")
{


	// Perform the test
	REQUIRE(!kPathTmpA.IsRelative());
	REQUIRE(!kPathTmpB.IsRelative());
	REQUIRE(!kPathTmpC.IsRelative());

	REQUIRE(NFilePath("one").IsRelative());
	REQUIRE(NFilePath("one").GetChild("two").IsRelative());
	REQUIRE(NFilePath("one").GetChild("two").GetChild("three").IsRelative());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "IsRoot")
{


	// Perform the test
	thePath = kPathTmpA;
	REQUIRE(!thePath.IsRoot());

	while (!thePath.IsRoot())
	{
		thePath = thePath.GetParent();
	}

	REQUIRE(thePath.IsRoot());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "Clear")
{


	// Perform the test
	thePath = kPathTmpA;
	REQUIRE(thePath.IsValid());

	thePath.Clear();
	REQUIRE(!thePath.IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetRoot")
{


	// Perform the test
	REQUIRE(kPathTmpA.GetRoot().IsValid());
	REQUIRE(kPathTmpB.GetRoot().IsValid());
	REQUIRE(kPathTmpC.GetRoot().IsValid());

	REQUIRE(!NFilePath("one").GetRoot().IsValid());
	REQUIRE(!NFilePath("one").GetChild("two").GetRoot().IsValid());
	REQUIRE(!NFilePath("one").GetChild("two").GetChild("three").GetRoot().IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetParent")
{


	// Perform the test
	NFilePath parentA = kPathTmpDirectory.GetChild("one").GetChild("two").GetChild("three");
	NFilePath parentB = kPathTmpDirectory.GetChild("one").GetChild("two");
	NFilePath parentC = kPathTmpDirectory.GetChild("one");

	REQUIRE(kPathTmpA.GetParent() == parentA);
	REQUIRE(kPathTmpB.GetParent() == parentB);
	REQUIRE(kPathTmpC.GetParent() == parentC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetChild")
{


	// Perform the test
	NFilePath parent3 = kPathTmpDirectory.GetChild("one").GetChild("two").GetChild("three");
	NFilePath parent2 = kPathTmpDirectory.GetChild("one").GetChild("two");
	NFilePath parent1 = kPathTmpDirectory.GetChild("one");

	REQUIRE(parent3.GetChild(kFilenameA) == kPathTmpA);
	REQUIRE(parent2.GetChild(kFilenameB) == kPathTmpB);
	REQUIRE(parent1.GetChild(kFilenameC) == kPathTmpC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetFilename/Absolute")
{


	// Perform the test
	REQUIRE(kPathTmpA.GetFilename() == kFilenameA);
	REQUIRE(kPathTmpB.GetFilename() == kFilenameB);
	REQUIRE(kPathTmpC.GetFilename() == kFilenameC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetFilename/Relative")
{


	// Perform the test
	thePath = NFilePath("foo");

	REQUIRE(thePath.GetChild(kFilenameA).GetFilename() == kFilenameA);
	REQUIRE(thePath.GetChild(kFilenameB).GetFilename() == kFilenameB);
	REQUIRE(thePath.GetChild(kFilenameC).GetFilename() == kFilenameC);

	REQUIRE(NFilePath(kFilenameA).GetFilename() == kFilenameA);
	REQUIRE(NFilePath(kFilenameB).GetFilename() == kFilenameB);
	REQUIRE(NFilePath(kFilenameC).GetFilename() == kFilenameC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetFilename/Absolute")
{


	// Perform the test
	thePath = kPathTmpA;
	REQUIRE(thePath.GetFilename() == kFilenameA);
	REQUIRE(thePath.GetFilename() != kFilenameB);

	thePath.SetFilename(kFilenameB);
	REQUIRE(thePath.GetFilename() != kFilenameA);
	REQUIRE(thePath.GetFilename() == kFilenameB);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetFilename/Relative")
{


	// Perform the test
	NVectorFilePath thePaths;

	thePaths.push_back(NFilePath("foo").GetChild(kFilenameA));
	thePaths.push_back(NFilePath(kFilenameA));

	for (auto thePath : thePaths)
	{
		REQUIRE(thePath.GetFilename() == kFilenameA);
		REQUIRE(thePath.GetFilename() != kFilenameB);

		thePath.SetFilename(kFilenameB);
		REQUIRE(thePath.GetFilename() != kFilenameA);
		REQUIRE(thePath.GetFilename() == kFilenameB);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetStem/Absolute")
{


	// Perform the test
	REQUIRE(kPathTmpA.GetStem() == kStemA);
	REQUIRE(kPathTmpB.GetStem() == kStemB);
	REQUIRE(kPathTmpC.GetStem() == kStemC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetStem/Relative")
{


	// Perform the test
	thePath = NFilePath("foo");

	REQUIRE(thePath.GetChild(kFilenameA).GetStem() == kStemA);
	REQUIRE(thePath.GetChild(kFilenameB).GetStem() == kStemB);
	REQUIRE(thePath.GetChild(kFilenameC).GetStem() == kStemC);

	REQUIRE(NFilePath(kFilenameA).GetStem() == kStemA);
	REQUIRE(NFilePath(kFilenameB).GetStem() == kStemB);
	REQUIRE(NFilePath(kFilenameC).GetStem() == kStemC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetStem/Absolute")
{


	// Perform the test
	thePath = kPathTmpA;
	REQUIRE(thePath.GetStem() == kStemA);
	REQUIRE(thePath.GetStem() != kStemB);

	thePath.SetStem(kStemB);
	REQUIRE(thePath.GetStem() != kStemA);
	REQUIRE(thePath.GetStem() == kStemB);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetStem/Relative")
{


	// Perform the test
	NVectorFilePath thePaths;

	thePaths.push_back(NFilePath("foo").GetChild(kFilenameA));
	thePaths.push_back(NFilePath(kFilenameA));

	for (auto thePath : thePaths)
	{
		REQUIRE(thePath.GetStem() == kStemA);
		REQUIRE(thePath.GetStem() != kStemB);

		thePath.SetStem(kStemB);
		REQUIRE(thePath.GetStem() != kStemA);
		REQUIRE(thePath.GetStem() == kStemB);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetExtension/Absolute")
{


	// Perform the test
	REQUIRE(kPathTmpA.GetExtension() == "");
	REQUIRE(kPathTmpB.GetExtension() == kExtension);
	REQUIRE(kPathTmpC.GetExtension() == kExtension);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetExtension/Relative")
{


	// Perform the test
	thePath = NFilePath("foo");

	REQUIRE(thePath.GetChild(kFilenameA).GetExtension() == "");
	REQUIRE(thePath.GetChild(kFilenameB).GetExtension() == kExtension);
	REQUIRE(thePath.GetChild(kFilenameC).GetExtension() == kExtension);

	REQUIRE(NFilePath(kFilenameA).GetExtension() == "");
	REQUIRE(NFilePath(kFilenameB).GetExtension() == kExtension);
	REQUIRE(NFilePath(kFilenameC).GetExtension() == kExtension);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetExtension/Absolute")
{


	// Perform the test
	thePath = kPathTmpA;
	REQUIRE(thePath.GetExtension() == "");
	REQUIRE(!thePath.GetFilename().Contains("."));

	thePath.SetExtension("");
	REQUIRE(thePath.GetExtension() == "");
	REQUIRE(!thePath.GetFilename().Contains("."));

	thePath.SetExtension("abc");
	REQUIRE(thePath.GetExtension() == "abc");
	REQUIRE(thePath.GetFilename() == (kFilenameA + ".abc"));

	thePath.SetExtension("123456");
	REQUIRE(thePath.GetExtension() == "123456");
	REQUIRE(thePath.GetFilename() == (kFilenameA + ".123456"));

	thePath.SetExtension("");
	REQUIRE(thePath.GetExtension() == "");
	REQUIRE(!thePath.GetFilename().Contains("."));

	REQUIRE(thePath == kPathTmpA);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetExtension/Relative")
{


	// Perform the test
	NVectorFilePath thePaths;

	thePaths.push_back(NFilePath("foo").GetChild(kFilenameA));
	thePaths.push_back(NFilePath(kFilenameA));

	for (auto thePath : thePaths)
	{
		REQUIRE(thePath.GetExtension() == "");
		REQUIRE(!thePath.GetFilename().Contains("."));

		thePath.SetExtension("");
		REQUIRE(thePath.GetExtension() == "");
		REQUIRE(!thePath.GetFilename().Contains("."));

		thePath.SetExtension("abc");
		REQUIRE(thePath.GetExtension() == "abc");
		REQUIRE(thePath.GetFilename() == (kFilenameA + ".abc"));

		thePath.SetExtension("123456");
		REQUIRE(thePath.GetExtension() == "123456");
		REQUIRE(thePath.GetFilename() == (kFilenameA + ".123456"));

		thePath.SetExtension("");
		REQUIRE(thePath.GetExtension() == "");
		REQUIRE(!thePath.GetFilename().Contains("."));
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetParts")
{


	// Perform the test
	NVectorString theParts;

	theParts = kPathTmpA.GetRoot().GetParts();
	REQUIRE(theParts.size() == 1);

	theParts = kPathTmpA.GetParts();
	REQUIRE(theParts.size() > 3);

	thePath.SetParts(theParts);
	REQUIRE(thePath == kPathTmpA);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetParts")
{


	// Perform the test
	NVectorString theParts = kPathTmpA.GetParts();
	REQUIRE(theParts.size() > 3);

	thePath = kPathTmpB;
	thePath.SetParts(theParts);

	REQUIRE(thePath == kPathTmpA);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetPath")
{


	// Perform the test
	NString theValue;

	theValue = kPathTmpA.GetPath();
	REQUIRE(!theValue.IsEmpty());

	theValue = kPathTmpB.GetPath();
	REQUIRE(!theValue.IsEmpty());

	theValue = kPathTmpC.GetPath();
	REQUIRE(!theValue.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "SetPath")
{


	// Perform the test
	thePath.SetPath(kPathTmpA.GetPath());
	REQUIRE(thePath == kPathTmpA);

	thePath.SetPath(kPathTmpB.GetPath());
	REQUIRE(thePath == kPathTmpB);

	thePath.SetPath(kPathTmpC.GetPath());
	REQUIRE(thePath == kPathTmpC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetUTF8")
{


	// Perform the test
	REQUIRE(kPathTmpA.GetUTF8() != nullptr);
	REQUIRE(kPathTmpB.GetUTF8() != nullptr);
	REQUIRE(kPathTmpC.GetUTF8() != nullptr);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "GetUTF16")
{


	// Perform the test
	REQUIRE(kPathTmpA.GetUTF16() != nullptr);
	REQUIRE(kPathTmpB.GetUTF16() != nullptr);
	REQUIRE(kPathTmpC.GetUTF16() != nullptr);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "Appendable")
{


	// Perform the test
	thePath = kPathTmpDirectory;
	thePath += NFilePath("one");
	thePath += NFilePath("two");
	thePath += NFilePath("three");

	NN_REQUIRE(thePath == kPathTmpOneTwoThree);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "CompareEqual")
{


	// Perform the test
	REQUIRE(kPathTmpOne == kPathTmpOne);
	REQUIRE(kPathTmpOne != kPathTmpOneTwo);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "CompareOrder")
{


	// Perform the test
	REQUIRE(kPathTmpOne <= kPathTmpOne);
	REQUIRE(kPathTmpOne <= kPathTmpOneTwo);
	REQUIRE(kPathTmpOne < kPathTmpOneTwo);

	REQUIRE(kPathTmpOneTwo >= kPathTmpOneTwo);
	REQUIRE(kPathTmpOneTwo >= kPathTmpOne);
	REQUIRE(kPathTmpOneTwo > kPathTmpOne);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFilePath, "Hashable")
{


	// Perform the test
	REQUIRE(thePath.GetHash() == 0);

	REQUIRE(kPathTmpA.GetHash() != 0);
	REQUIRE(kPathTmpB.GetHash() != 0);
	REQUIRE(kPathTmpC.GetHash() != 0);
}
