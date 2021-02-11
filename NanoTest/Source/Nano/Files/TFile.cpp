/*	NAME:
		TFile.cpp

	DESCRIPTION:
		NFile tests.

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
#include "NFileUtils.h"
#include "NFormat.h"
#include "NStdAlgorithm.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Paths
static const NFilePath kPathTmpDirectory =
	NFileUtils::GetLocation(NFileLocation::AppTemporaries, "TFile");

static const NString kExtensionTmpFileA                     = "aaa";
static const NString kExtensionTmpFileB                     = "bbb";

static const NString kNameTmpFileA                          = "TFile_A." + kExtensionTmpFileA;
static const NString kNameTmpFileB                          = "TFile_B." + kExtensionTmpFileB;

static const NFilePath kPathTmpFileA                        = kPathTmpDirectory.GetChild(kNameTmpFileA);
static const NFilePath kPathTmpFileB                        = kPathTmpDirectory.GetChild(kNameTmpFileB);

#if NN_TARGET_WINDOWS
static const NFilePath kPathFile                            = NString("c:\\Windows\\regedit.exe");
static const NFilePath kPathDirectory                       = NString("c:\\Windows\\System");
static const NFilePath kPathDoesNotExist                    = NString("c:\\63785644-da36-4148-939f-4416cb5ea56e");
#else
static const NFilePath kPathFile                            = NString("/bin/bash");
static const NFilePath kPathDirectory                       = NString("/tmp");
static const NFilePath kPathDoesNotExist                    = NString("/63785644-da36-4148-939f-4416cb5ea56e");
#endif





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFile)
{
	NFile   theFile;
	NStatus theErr;

	SETUP
	{
		theErr = NFileUtils::CreateDirectory(kPathTmpDirectory, NFileAction::CanDelete);
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
NANO_TEST(TFile, "Default")
{


	// Perform the test
	REQUIRE(!theFile.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Constructor")
{


	// Perform the test
	REQUIRE(NFile(kPathFile).IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Clear")
{


	// Perform the test
	theFile = NFile(kPathFile);
	REQUIRE(theFile.IsValid());

	theFile.Clear();
	REQUIRE(!theFile.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "GetPath/SetPath")
{


	// Perform the test
	REQUIRE(theFile.GetPath() != kPathFile);
	theFile.SetPath(kPathFile);
	REQUIRE(theFile.GetPath() == kPathFile);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Refresh")
{


	// Perform the test
	theFile.SetPath(kPathFile);
	theFile.Refresh();
	REQUIRE(theFile.GetPath() == kPathFile);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Status")
{


	// Perform the test
	theFile.SetPath(kPathFile);
	REQUIRE(theFile.Exists());
	REQUIRE(theFile.IsFile());
	REQUIRE(!theFile.IsDirectory());

	theFile.SetPath(kPathDirectory);
	REQUIRE(theFile.Exists());
	REQUIRE(!theFile.IsFile());
	REQUIRE(theFile.IsDirectory());

	theFile.SetPath(kPathDoesNotExist);
	REQUIRE(!theFile.Exists());
	REQUIRE(!theFile.IsFile());
	REQUIRE(!theFile.IsDirectory());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Permission")
{


	// Perform the test
	theFile.SetPath(kPathFile);
	REQUIRE(theFile.CanRead());
	REQUIRE((!theFile.CanWrite() || NN_TARGET_WINDOWS));
	REQUIRE(theFile.CanExecute());

	theFile.SetPath(kPathDirectory);
	REQUIRE(theFile.CanRead());
	REQUIRE(theFile.CanWrite());
	REQUIRE(theFile.CanExecute());

	theFile.SetPath(kPathDoesNotExist);
	REQUIRE(!theFile.CanRead());
	REQUIRE(!theFile.CanWrite());
	REQUIRE(!theFile.CanExecute());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Time")
{


	// Perform the test
	theFile.SetPath(kPathFile);
	REQUIRE(theFile.GetCreationTime() != 0.0);
	REQUIRE(theFile.GetModifiedTime() != 0.0);
	REQUIRE(theFile.GetModifiedTime() >= theFile.GetCreationTime());

	theFile.SetPath(kPathDirectory);
	REQUIRE(theFile.GetCreationTime() != 0.0);
	REQUIRE(theFile.GetModifiedTime() != 0.0);
	REQUIRE(theFile.GetModifiedTime() >= theFile.GetCreationTime());

	theFile.SetPath(kPathDoesNotExist);
	REQUIRE(theFile.GetCreationTime() == 0.0);
	REQUIRE(theFile.GetModifiedTime() == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "GetName/SetName")
{


	// Perform the test
	NFile tmpFileA(kPathTmpFileA);
	NFile tmpFileB(kPathTmpFileB);

	REQUIRE(tmpFileA.GetName() == kNameTmpFileA);
	REQUIRE(tmpFileB.GetName() == kNameTmpFileB);

	theErr = tmpFileA.SetName(kNameTmpFileB, false);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileA.GetName() == kNameTmpFileB);


	theErr = tmpFileB.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!NFile(kPathTmpFileA).Exists());
	REQUIRE(NFile(kPathTmpFileB).Exists());

	theErr = tmpFileB.SetName(kNameTmpFileA, true);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(NFile(kPathTmpFileA).Exists());
	REQUIRE(!NFile(kPathTmpFileB).Exists());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "GetExtension/SetExtension")
{


	// Perform the test
	NFile tmpFileA(kPathTmpFileA);
	NFile tmpFileB(kPathTmpFileB);

	REQUIRE(tmpFileA.GetExtension() == kExtensionTmpFileA);
	REQUIRE(tmpFileB.GetExtension() == kExtensionTmpFileB);

	theErr = tmpFileA.SetExtension(kExtensionTmpFileB, false);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileA.GetExtension() == kExtensionTmpFileB);


	theErr = tmpFileB.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!NFile(kPathTmpFileA).Exists());
	REQUIRE(NFile(kPathTmpFileB).Exists());

	theErr = tmpFileB.SetExtension(kExtensionTmpFileA, true);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!NFile(kPathTmpFileA).Exists());
	REQUIRE(!NFile(kPathTmpFileB).Exists());
	REQUIRE(tmpFileB.Exists());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "GetChild")
{


	// Perform the test
	NFile tmpDir(kPathTmpDirectory);
	NFile tmpFileA(kPathTmpFileA);
	NFile tmpFileB(kPathTmpFileB);

	REQUIRE(tmpDir.GetChild(kNameTmpFileA) == tmpFileA);
	REQUIRE(tmpDir.GetChild(kNameTmpFileB) == tmpFileB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "GetParent")
{


	// Perform the test
	NFile tmpDir(kPathTmpDirectory);
	NFile tmpFileA(kPathTmpFileA);
	NFile tmpFileB(kPathTmpFileB);

	REQUIRE(tmpFileA.GetParent() == tmpDir);
	REQUIRE(tmpFileB.GetParent() == tmpDir);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "GetSize")
{


	// Perform the test
	NFile tmpFile(kPathTmpFileA);

	theErr = tmpFile.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFile.IsFile());
	REQUIRE(tmpFile.GetSize() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "GetChildren")
{


	// Perform the test
	NFile tmpDir(kPathTmpDirectory);
	NFile tmpFileA(kPathTmpFileA);
	NFile tmpFileB(kPathTmpFileB);

	theErr = tmpFileA.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileA.GetParent() == tmpDir);

	theErr = tmpFileB.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileB.GetParent() == tmpDir);

	NVectorFile theChildren = tmpDir.GetChildren();
	REQUIRE(theChildren.size() == 2);
	REQUIRE(nstd::contains(theChildren, tmpFileA));
	REQUIRE(nstd::contains(theChildren, tmpFileB));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "CreateFile")
{


	// Perform the test
	theFile.SetPath(kPathTmpFileA);
	REQUIRE(!theFile.Exists());

	theErr = theFile.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.Exists());
	REQUIRE(theFile.IsFile());
	REQUIRE(!theFile.IsDirectory());

	theErr = theFile.Delete();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!theFile.Exists());
	REQUIRE(!theFile.IsFile());
	REQUIRE(!theFile.IsDirectory());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "CreateDirectory")
{


	// Perform the test
	theFile.SetPath(kPathTmpFileA);
	REQUIRE(!theFile.Exists());

	theErr = theFile.CreateDirectory();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.Exists());
	REQUIRE(!theFile.IsFile());
	REQUIRE(theFile.IsDirectory());

	theErr = theFile.Delete();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!theFile.Exists());
	REQUIRE(!theFile.IsFile());
	REQUIRE(!theFile.IsDirectory());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Delete")
{


	// Perform the test
	theFile.SetPath(kPathTmpFileA);
	REQUIRE(!theFile.Exists());


	theErr = theFile.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.Exists());

	theErr = theFile.Delete(NFileAction::CanDelete);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!theFile.Exists());


	theErr = theFile.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.Exists());

	theErr = theFile.Delete(NFileAction::CanTrash);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!theFile.Exists());


	theErr = theFile.CreateDirectory();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.Exists());

	theErr = theFile.Delete(NFileAction::CanDelete);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!theFile.Exists());


	theErr = theFile.CreateDirectory();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.Exists());

	theErr = theFile.Delete(NFileAction::CanTrash);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(!theFile.Exists());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "DeleteChildren")
{


	// Perform the test
	NFile tmpDir(kPathTmpDirectory);
	NFile tmpFileA(kPathTmpFileA);
	NFile tmpFileB(kPathTmpFileB);

	theErr = tmpFileA.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileA.GetParent() == tmpDir);

	theErr = tmpFileB.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileB.GetParent() == tmpDir);

	NVectorFile theChildren = tmpDir.GetChildren();
	REQUIRE(theChildren.size() == 2);
	REQUIRE(nstd::contains(theChildren, tmpFileA));
	REQUIRE(nstd::contains(theChildren, tmpFileB));

	theErr = tmpDir.DeleteChildren();
	REQUIRE(theErr == NStatus::OK);

	theChildren = tmpDir.GetChildren();
	REQUIRE(theChildren.empty());
	REQUIRE(!tmpFileA.Exists());
	REQUIRE(!tmpFileB.Exists());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "MoveTo")
{


	// Perform the test
	NFile tmpDir(kPathTmpDirectory);
	NFile tmpFileA(kPathTmpFileA);
	NFile tmpDirB = tmpDir.GetChild("one").GetChild("two").GetChild("three");

	theErr = tmpFileA.CreateFile();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileA.IsFile());

	theErr = tmpDirB.CreateDirectory();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpDirB.IsDirectory());

	REQUIRE(tmpDir.GetChildren().size() == 2);
	REQUIRE(tmpDirB.GetChildren().size() == 0);

	theErr = tmpFileA.MoveTo(tmpDirB);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(tmpFileA.GetParent() == tmpDirB);

	REQUIRE(tmpDir.GetChildren().size() == 1);
	REQUIRE(tmpDirB.GetChildren().size() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "CompareEqual")
{


	// Perform the test
	NFile fileA(kPathFile);
	NFile fileB(kPathDirectory);

	REQUIRE(fileA == fileA);
	REQUIRE(fileB == fileB);

	REQUIRE(fileA != fileB);
	REQUIRE(fileB != fileA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "CompareOrder")
{


	// Perform the test
	NFile fileA(kPathFile);
	NFile fileB(kPathDirectory);

	REQUIRE(fileA <= fileB);
	REQUIRE(fileA < fileB);

	REQUIRE(fileB >= fileA);
	REQUIRE(fileB > fileA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFile, "Format")
{


	// Perform the test
	theFile = kPathFile;
	REQUIRE(NFormat("{}", theFile) == theFile.GetPath().GetPath());
}
