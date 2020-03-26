/*	NAME:
		TFileUtils.cpp

	DESCRIPTION:
		NFileUtils tests.

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
#include "NFileUtils.h"
#include "NTestFixture.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
#if NN_TARGET_ANDROID
static const NString kPathTmpDirectory                      = "/tmp/TFileScanner";
static const NString kPathTmpChildA                         = kPathTmpDirectory + "/one/two/three/four";
static const NString kPathTmpChildB                         = kPathTmpDirectory + "/one/two/33333.dat";
static const NString kPathTmpChildC                         = kPathTmpDirectory + "/one/222.dat";

#elif NN_TARGET_IOS
static const NString kPathTmpDirectory                      = "/tmp/TFileScanner";
static const NString kPathTmpChildA                         = kPathTmpDirectory + "/one/two/three/four";
static const NString kPathTmpChildB                         = kPathTmpDirectory + "/one/two/33333.dat";
static const NString kPathTmpChildC                         = kPathTmpDirectory + "/one/222.dat";


#elif NN_TARGET_LINUX
static const NString kPathTmpDirectory                      = "/tmp/TFileScanner";
static const NString kPathTmpChildA                         = kPathTmpDirectory + "/one/two/three/four";
static const NString kPathTmpChildB                         = kPathTmpDirectory + "/one/two/33333.dat";
static const NString kPathTmpChildC                         = kPathTmpDirectory + "/one/222.dat";


#elif NN_TARGET_MACOS
static const NString kPathTmpDirectory                      = "/tmp/TFileScanner";
static const NString kPathTmpChildA                         = kPathTmpDirectory + "/one/two/three/four";
static const NString kPathTmpChildB                         = kPathTmpDirectory + "/one/two/33333.dat";
static const NString kPathTmpChildC                         = kPathTmpDirectory + "/one/222.dat";


#elif NN_TARGET_TVOS
static const NString kPathTmpDirectory                      = "/tmp/TFileScanner";
static const NString kPathTmpChildA                         = kPathTmpDirectory + "/one/two/three/four";
static const NString kPathTmpChildB                         = kPathTmpDirectory + "/one/two/33333.dat";
static const NString kPathTmpChildC                         = kPathTmpDirectory + "/one/222.dat";


#elif NN_TARGET_WINDOWS
static const NString kPathTmpDirectory                      = "c:\\Windows\\Temp\\TFileScanner";
static const NString kPathTmpChildA                         = kPathTmpDirectory + "\\one\\two\\three\\four";
static const NString kPathTmpChildB                         = kPathTmpDirectory + "\\one\\two\\33333.dat";
static const NString kPathTmpChildC                         = kPathTmpDirectory + "\\one\\222.dat";

#else
	#error "Unknown target"
#endif





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileUtils)
{
	NStatus theErr;

	SETUP
	{
		if (NFile(kPathTmpDirectory).Exists())
		{
			theErr = NFile(kPathTmpDirectory).Delete();
			REQUIRE(theErr == NStatus::OK);
		}

		theErr = NFileUtils::CreateDirectory(kPathTmpChildA);
		REQUIRE(theErr == NStatus::OK);

		theErr = NFile(kPathTmpChildB).CreateFile();
		REQUIRE(theErr == NStatus::OK);

		theErr = NFile(kPathTmpChildC).CreateFile();
		REQUIRE(theErr == NStatus::OK);
	}

	TEARDOWN
	{
		theErr = NFile(kPathTmpDirectory).Delete();
		REQUIRE(theErr == NStatus::OK);
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "GetPathPart")
{


	// Perform the test
	NString theParent    = kPathTmpDirectory + kNPathSeparator + "one";
	NString theName      = "222.dat";
	NString theExtension = "dat";

	REQUIRE(NFileUtils::GetPathPart(kPathTmpChildC, NPathPart::Parent) == theParent);
	REQUIRE(NFileUtils::GetPathPart(kPathTmpChildC, NPathPart::Name) == theName);
	REQUIRE(NFileUtils::GetPathPart(kPathTmpChildC, NPathPart::Extension) == theExtension);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "GetChildren")
{


	// Perform the test
	NVectorFile theFiles = NFileUtils::GetChildren(kPathTmpDirectory);
	REQUIRE(theFiles.size() == 1);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "CreateDirectories")
{


	// Perform the test
	theErr = NFileUtils::CreateDirectory(kPathTmpChildA + kNPathSeparator + "aa" + kNPathSeparator +
										 "bb" + kNPathSeparator + "cc" + kNPathSeparator + "dd");
	REQUIRE(theErr == NStatus::OK);

	NVectorFile theFiles = NFileUtils::GetChildren(kPathTmpChildA);
	REQUIRE(theFiles.size() == 1);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "Rename")
{


	// Perform the test
	theErr = NFileUtils::Delete(kPathTmpChildB);
	REQUIRE(theErr == NStatus::OK);

	theErr = NFileUtils::Rename(kPathTmpChildC, kPathTmpChildB);
	REQUIRE(theErr == NStatus::OK);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "Exchange")
{


	// Perform the test
	theErr = NFileUtils::Exchange(kPathTmpChildB, kPathTmpChildC);
	REQUIRE(theErr == NStatus::OK);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "Delete")
{


	// Perform the test
	theErr = NFileUtils::Delete(kPathTmpChildB, false);
	REQUIRE(theErr == NStatus::OK);

	theErr = NFileUtils::Delete(kPathTmpChildC, true);
	REQUIRE(theErr == NStatus::OK);
}
