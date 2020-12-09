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
#include "NFileInfo.h"
#include "NFileUtils.h"
#include "NStdAlgorithm.h"
#include "NTestFixture.h"

// System
#include <set>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const NFilePath kPathTmpDirectory =
	NFileUtils::GetLocation(NFileLocation::AppTemporaries, "TFileScanner");

static const NFilePath kPathTmpOne                          = kPathTmpDirectory.GetChild("one");
static const NFilePath kPathTmpOneTwo                       = kPathTmpOne.GetChild("two");
static const NFilePath kPathTmpOneTwoThree                  = kPathTmpOneTwo.GetChild("three");

static const NFilePath kPathTmpChildA                       = kPathTmpOneTwoThree.GetChild("four");
static const NFilePath kPathTmpChildB                       = kPathTmpOneTwo.GetChild("33333.dat");
static const NFilePath kPathTmpChildC                       = kPathTmpOne.GetChild("222.dat");





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileUtils)
{
	NStatus theErr;

	SETUP
	{
		theErr = NFileUtils::CreateDirectory(kPathTmpDirectory, NFileAction::CanDelete);

		theErr = NFileUtils::CreateDirectory(kPathTmpChildA);
		REQUIRE(theErr == NStatus::OK);

		theErr = NFileUtils::CreateFile(kPathTmpChildB);
		REQUIRE(theErr == NStatus::OK);

		theErr = NFileUtils::CreateFile(kPathTmpChildC);
		REQUIRE(theErr == NStatus::OK);
	}

	TEARDOWN
	{
		theErr = NFileUtils::Delete(kPathTmpDirectory);
		REQUIRE(theErr == NStatus::OK);
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "CreateFile")
{


	// Perform the test
	NFilePath thePath = kPathTmpChildC;
	thePath.SetExtension("bak");

	theErr = NFileUtils::CreateFile(thePath);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(NFileInfo(thePath).IsFile());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "CreateDirectory")
{


	// Perform the test
	NFilePath thePath = kPathTmpChildA.GetChild("aa").GetChild("bb").GetChild("cc").GetChild("dd");

	theErr = NFileUtils::CreateDirectory(thePath);
	REQUIRE(theErr == NStatus::OK);

	NVectorFilePath theChildren = NFileUtils::GetChildren(kPathTmpChildA);
	REQUIRE(theChildren.size() == 1);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "Delete")
{


	// Perform the test
	theErr = NFileUtils::Delete(kPathTmpChildB, NFileAction::CanDelete);
	REQUIRE(theErr == NStatus::OK);

	theErr = NFileUtils::Delete(kPathTmpChildC, NFileAction::CanTrash);
	REQUIRE(theErr == NStatus::OK);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "DeleteChildren")
{


	// Perform the test
	REQUIRE(NFileUtils::GetChildren(kPathTmpDirectory).size() != 0);

	theErr = NFileUtils::DeleteChildren(kPathTmpDirectory, NFileAction::CanDelete);
	REQUIRE(theErr == NStatus::OK);

	REQUIRE(NFileUtils::GetChildren(kPathTmpDirectory).size() == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "GetChildren")
{


	// Perform the test
	NVectorFilePath theChildren = NFileUtils::GetChildren(kPathTmpDirectory);
	REQUIRE(theChildren.size() == 1);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
#include "NPOSIX.h"

NANO_TEST(TFileUtils, "GetLocation")
{


	// Perform the test
NN_LOG_INFO("AppCaches");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::AppCaches).IsValid());

NN_LOG_INFO("AppSupport");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::AppSupport).IsValid());

NN_LOG_INFO("AppTemporaries");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::AppTemporaries).IsValid());

NN_LOG_INFO("SharedSupport");

NFilePath thePath;
thePath = NPOSIX::getenv("XDG_DATA_DIRS");
NN_LOG_INFO("thePath.1=[{}]", thePath);
if (thePath.GetPath().Contains(":"))
{
NN_LOG_INFO("thePath.2=[{}]", thePath);
	thePath = thePath.GetPath().Split(":").front();
NN_LOG_INFO("thePath.3=[{}]", thePath);
}

NN_LOG_INFO("thePath.4=[{}]", thePath);
if (!thePath.IsValid())
{
NN_LOG_INFO("thePath.5=[{}]", thePath);
	thePath = NString("/usr/local/share");
NN_LOG_INFO("thePath.6=[{}]", thePath);
}
NN_LOG_INFO("thePath.7=[{}]", thePath);


	REQUIRE(NFileUtils::GetLocation(NFileLocation::SharedSupport).IsValid());

NN_LOG_INFO("UserDesktop");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::UserDesktop).IsValid());

NN_LOG_INFO("UserDocuments");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::UserDocuments).IsValid());

NN_LOG_INFO("UserDownloads");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::UserDownloads).IsValid());

NN_LOG_INFO("UserHome");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::UserHome).IsValid());

NN_LOG_INFO("UserLogs");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::UserLogs).IsValid());

NN_LOG_INFO("UserPictures");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::UserPictures).IsValid());

NN_LOG_INFO("UserPreferences");
	REQUIRE(NFileUtils::GetLocation(NFileLocation::UserPreferences).IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileUtils, "GetUniqueChild")
{


	// Perform the test
	std::set<NFilePath> prevPaths;

	for (auto n = 0; n < 30; n++)
	{
		NFilePath path1 = NFileUtils::GetUniqueChild(kPathTmpOne);
		NFilePath path2 = NFileUtils::GetUniqueChild(kPathTmpOne, "Test");
		NFilePath path3 = NFileUtils::GetUniqueChild(kPathTmpOne, "Test.dat");

		REQUIRE(!nstd::contains(prevPaths, path1));
		prevPaths.insert(path1);

		REQUIRE(!nstd::contains(prevPaths, path2));
		prevPaths.insert(path2);

		REQUIRE(!nstd::contains(prevPaths, path3));
		prevPaths.insert(path3);
	}
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
