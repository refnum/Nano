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
#include "NFileHandle.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Misc
static constexpr uint32_t kBufferSize                       = 9;
static constexpr uint8_t  kBufferData[kBufferSize]          = {'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a'};


// Paths
#if NN_TARGET_ANDROID
static const NString kPathTmpFile                           = "/tmp/4bf32e59-38be-4151-b31b-e734de7f738a.tmp";

#elif NN_TARGET_IOS
static const NString kPathTmpFile                           = "/tmp/4bf32e59-38be-4151-b31b-e734de7f738a.tmp";


#elif NN_TARGET_LINUX
static const NString kPathTmpFile                           = "/tmp/4bf32e59-38be-4151-b31b-e734de7f738a.tmp";


#elif NN_TARGET_MACOS
static const NString kPathTmpFile                           = "/tmp/4bf32e59-38be-4151-b31b-e734de7f738a.tmp";


#elif NN_TARGET_TVOS
static const NString kPathTmpFile                           = "/tmp/4bf32e59-38be-4151-b31b-e734de7f738a.tmp";

#elif NN_TARGET_WINDOWS
static const NString kPathTmpFile                           = "c:\\temp\\tmp/4bf32e59-38be-4151-b31b-e734de7f738a.tmp";

#else
	#error "Unknown target"
#endif





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileHandle)
{
	NFileHandle theFile;
	NStatus     theErr;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "Default")
{


	// Validate our state
	REQUIRE(!theFile.IsOpen());
	REQUIRE(theFile.GetPath().IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenRead")
{


	// Perform the test
	theErr = theFile.Open(kPathTmpFile, NFileAccess::Read);
	REQUIRE(theErr == NStatus::NotFound);
	REQUIRE(!theFile.IsOpen());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenWrite")
{


	// Perform the test
	theErr = theFile.Open(kPathTmpFile, NFileAccess::Write);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.IsOpen());
	REQUIRE(theFile.GetPosition() == 0);

	uint64_t numWritten = 123456;
	theErr              = theFile.Write(kBufferSize, kBufferData, numWritten);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(numWritten == kBufferSize);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileHandle, "OpenWriteRead")
{


	// Perform the test
	theErr = theFile.Open(kPathTmpFile, NFileAccess::Write);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.IsOpen());
	REQUIRE(theFile.GetPosition() == 0);

	uint64_t numWritten = 123456;
	theErr              = theFile.Write(kBufferSize, kBufferData, numWritten);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(numWritten == kBufferSize);
	REQUIRE(theFile.GetPosition() == numWritten);

	theFile.Close();
	REQUIRE(!theFile.IsOpen());
	REQUIRE(theFile.GetPosition() == 0);



	theErr = theFile.Open(kPathTmpFile, NFileAccess::Read);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(theFile.IsOpen());
	REQUIRE(theFile.GetPosition() == 0);

	uint8_t  tmpBuffer[kBufferSize];
	uint64_t numRead = 123456;
	theErr           = theFile.Read(kBufferSize, tmpBuffer, numRead);
	REQUIRE(theErr == NStatus::OK);
	REQUIRE(numRead == kBufferSize);

	REQUIRE(memcmp(tmpBuffer, kBufferData, kBufferSize) == 0);

	numRead = 123456;
	theErr  = theFile.Read(kBufferSize, tmpBuffer, numRead);
	REQUIRE(theErr == NStatus::ExhaustedSrc);
	REQUIRE(numRead == 0);
}
