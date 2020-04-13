/*	NAME:
		TFileHandle.cpp

	DESCRIPTION:
		NFileHandle tests.

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
#include "NData.h"
#include "NFile.h"
#include "NFileHandle.h"
#include "NFileUtils.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Misc
static constexpr size_t  kLargeSize                         = 9000;
static constexpr size_t  kBufferSize                        = 9;
static constexpr uint8_t kBufferData[kBufferSize]           = {'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a'};

static const NString kTestString                            = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
static const NData   kTestData                              = kTestString.GetData(NStringEncoding::UTF32BE);


// Paths
static const NFilePath kPathTmpDirectory =
	NFileUtils::GetLocation(NFileLocation::AppTemporaries, "TFileHandle");

static const NFilePath kPathTmpFile                         = kPathTmpDirectory.GetChild("TFileHandle.dat");





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileHandle)
{
	NFileHandle fileHnd;
	NStatus     theErr;

	uint64_t sizeRead;
	uint64_t sizeWritten;

	SETUP
	{
		theErr = NFileUtils::CreateDirectory(kPathTmpDirectory, NFileAction::CanDelete);
		REQUIRE(theErr == NStatus::OK);

		sizeRead    = 0;
		sizeWritten = 0;
	}

	TEARDOWN
	{
		if (fileHnd.IsOpen())
		{
			fileHnd.Close();
		}

		theErr = NFileUtils::Delete(kPathTmpDirectory);
		REQUIRE(theErr == NStatus::OK);
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "Default")
{


	// Validate our state
	REQUIRE(!fileHnd.IsOpen());
	REQUIRE(!fileHnd.GetPath().IsValid());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenRead")
{


	// Perform the test
	theErr = fileHnd.Open(kPathTmpFile, NFileAccess::ReadOnly);
	REQUIRE(theErr == NStatus::NotFound);
	REQUIRE(!fileHnd.IsOpen());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenWrite")
{


	// Perform the test
	theErr = fileHnd.Open(kPathTmpFile, NFileAccess::WriteOnly);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());
	REQUIRE(fileHnd.GetPosition() == 0);

	sizeWritten = kLargeSize;
	theErr      = fileHnd.Write(kBufferSize, kBufferData, sizeWritten);

	REQUIRE(sizeWritten == kBufferSize);
	REQUIRE(theErr == NStatus::OK);

	theErr = fileHnd.Flush();
	REQUIRE(theErr == NStatus::OK);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenWriteRead")
{


	// Perform the test
	theErr = fileHnd.Open(kPathTmpFile, NFileAccess::WriteOnly);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());
	REQUIRE(fileHnd.GetPosition() == 0);

	sizeWritten = kLargeSize;
	theErr      = fileHnd.Write(kBufferSize, kBufferData, sizeWritten);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(sizeWritten == kBufferSize);
	REQUIRE(fileHnd.GetPosition() == sizeWritten);

	fileHnd.Close();
	REQUIRE(!fileHnd.IsOpen());



	theErr = fileHnd.Open(kPathTmpFile, NFileAccess::ReadOnly);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());
	REQUIRE(fileHnd.GetPosition() == 0);

	uint8_t tmpBuffer[kBufferSize];
	sizeRead = kLargeSize;
	theErr   = fileHnd.Read(kBufferSize, tmpBuffer, sizeRead);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(sizeRead == kBufferSize);

	REQUIRE(memcmp(tmpBuffer, kBufferData, kBufferSize) == 0);

	sizeRead = kLargeSize;
	theErr   = fileHnd.Read(kBufferSize, tmpBuffer, sizeRead);
	REQUIRE(theErr == NStatus::ExhaustedSrc);
	REQUIRE(sizeRead == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenTemporary/Unnamed")
{


	// Perform the test
	theErr = fileHnd.OpenTemporary();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());
	REQUIRE(fileHnd.GetPosition() == 0);

	sizeWritten = kLargeSize;
	theErr      = fileHnd.Write(kBufferSize, kBufferData, sizeWritten);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(sizeWritten == kBufferSize);
	REQUIRE(fileHnd.GetPosition() == sizeWritten);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenTemporary/Named")
{


	// Perform the test
	theErr = fileHnd.OpenTemporary("Test.dat");
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());
	REQUIRE(fileHnd.GetPosition() == 0);

	sizeWritten = kLargeSize;
	theErr      = fileHnd.Write(kBufferSize, kBufferData, sizeWritten);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(sizeWritten == kBufferSize);
	REQUIRE(fileHnd.GetPosition() == sizeWritten);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "Close")
{


	// Perform the test
	theErr = fileHnd.OpenTemporary();
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());

	fileHnd.Close();
	REQUIRE(!fileHnd.IsOpen());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "GetSize/SetSize")
{


	// Perform the test
	theErr = fileHnd.Open(kPathTmpFile, NFileAccess::ReadWrite);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());
	REQUIRE(fileHnd.GetPosition() == 0);
	REQUIRE(fileHnd.GetSize() == 0);

	theErr = fileHnd.SetSize(kLargeSize);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.GetSize() == kLargeSize);

	fileHnd.Close();


	NData zeroData(kLargeSize, nullptr, NDataSource::Zero);
	NData fileData(kLargeSize, nullptr, NDataSource::None);

	theErr = fileHnd.Open(kPathTmpFile, NFileAccess::ReadOnly);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(fileHnd.IsOpen());
	REQUIRE(fileHnd.GetPosition() == 0);
	REQUIRE(fileHnd.GetSize() == kLargeSize);

	sizeRead = kLargeSize;
	theErr   = fileHnd.Read(fileData.GetSize(), fileData.GetMutableData(), sizeRead);
	REQUIRE(theErr == NStatus::OK);

	REQUIRE(fileData == zeroData);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "WriteText/ReadText")
{


	// Perform the test
	NString theText;

	theText = NFileHandle::ReadText(kPathTmpFile);
	REQUIRE(theText.IsEmpty());


	theErr = NFileHandle::WriteText(kPathTmpFile, kTestString);
	REQUIRE(theErr == NStatus::OK);

	theText = NFileHandle::ReadText(kPathTmpFile);
	REQUIRE(theText == kTestString);


	theErr = NFileHandle::WriteText(kPathTmpFile, kTestString, NStringEncoding::UTF16);
	REQUIRE(theErr == NStatus::OK);

	theText = NFileHandle::ReadText(kPathTmpFile, NStringEncoding::UTF16);
	REQUIRE(theText == kTestString);


	theErr = NFileHandle::WriteText(kPathTmpFile, kTestString, NStringEncoding::UTF32);
	REQUIRE(theErr == NStatus::OK);

	theText = NFileHandle::ReadText(kPathTmpFile, NStringEncoding::UTF32);
	REQUIRE(theText == kTestString);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "WriteData/ReadData")
{


	// Perform the test
	NData theData;

	theData = NFileHandle::ReadData(kPathTmpFile);
	REQUIRE(theData.IsEmpty());


	theErr = NFileHandle::WriteData(kPathTmpFile, kTestData);
	REQUIRE(theErr == NStatus::OK);

	theData = NFileHandle::ReadData(kPathTmpFile);
	REQUIRE(theData == kTestData);
}
