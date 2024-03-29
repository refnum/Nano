/*	NAME:
		TFileScanner.cpp

	DESCRIPTION:
		TFileScanner tests.

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
#include "NFile.h"
#include "NFileScanner.h"
#include "NFileUtils.h"
#include "NTestFixture.h"





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
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileScanner)
{
	NFileScanner fileScanner;
	NVectorFile  theFiles;
	NStatus      theErr;

	SETUP
	{
		theErr = NFileUtils::CreateDirectory(kPathTmpDirectory, NFileAction::CanDelete);
		REQUIRE(theErr == NStatus::OK);

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
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "Default")
{


	// Perform the test
	REQUIRE(fileScanner.GetRecurseDepth() == kNSizeMax);
	REQUIRE(fileScanner.GetFilterPath().IsEmpty());
	REQUIRE(fileScanner.GetFilterName().IsEmpty());
	REQUIRE(fileScanner.GetFilterItem() == nullptr);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "SetRecurseDepth")
{


	// Perform the test
	fileScanner.SetRecurseDepth(2);
	fileScanner.Start(kPathTmpDirectory);

	theFiles = fileScanner.GetFiles();
	REQUIRE(theFiles.size() == 6);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "SetFilterPath")
{


	// Perform the test
	fileScanner.SetFilterPath(".*two.*");
	fileScanner.Start(kPathTmpDirectory);

	theFiles = fileScanner.GetFiles();
	REQUIRE(theFiles.size() == 4);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "SetFilterName")
{


	// Perform the test
	fileScanner.SetFilterName(".*\\.dat");
	fileScanner.Start(kPathTmpDirectory);

	theFiles = fileScanner.GetFiles();
	REQUIRE(theFiles.size() == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "SetFilterItem/None")
{


	// Perform the test
	fileScanner.SetFilterItem(
	[](const NFile& /*theFile*/, bool& stopScan)
	{
		stopScan = true;
		return true;
	});

	fileScanner.Start(kPathTmpDirectory);

	theFiles = fileScanner.GetFiles();
	REQUIRE(theFiles.size() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "SetFilterItem/All")
{


	// Perform the test
	fileScanner.SetFilterItem(
	[](const NFile& /*theFile*/, bool& /*stopScan*/)
	{
		return true;
	});

	fileScanner.Start(kPathTmpDirectory);

	theFiles = fileScanner.GetFiles();
	REQUIRE(theFiles.size() == 6);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "SetFilterItem/Some")
{


	// Perform the test
	fileScanner.SetFilterItem(
	[](const NFile& theFile, bool& /*stopScan*/)
	{
		return theFile.IsFile();
	});

	fileScanner.Start(kPathTmpDirectory);

	theFiles = fileScanner.GetFiles();
	REQUIRE(theFiles.size() == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "GetFiles")
{


	// Perform the test
	fileScanner.Start(kPathTmpDirectory);

	theFiles = fileScanner.GetFiles();
	REQUIRE(theFiles.size() == 6);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "RangeBasedFor")
{


	// Perform the test
	size_t numItems = 0;
	size_t numFiles = 0;
	size_t numDirs  = 0;

	fileScanner.Start(kPathTmpDirectory);
	for (const auto& theFile : fileScanner)
	{
		numItems++;

		if (theFile.IsDirectory())
		{
			numDirs++;
		}

		else if (theFile.IsFile())
		{
			numFiles++;
		}
	}

	REQUIRE(numItems == 6);
	REQUIRE(numDirs == 4);
	REQUIRE(numFiles == 2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileScanner, "GetMatches")
{


	// Perform the test
	theFiles = NFileScanner::Scan(kPathTmpDirectory);
	REQUIRE(theFiles.size() == 6);

	theFiles = NFileScanner::Scan(kPathTmpDirectory, ".*\\.dat");
	REQUIRE(theFiles.size() == 2);
}
