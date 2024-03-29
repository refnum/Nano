/*	NAME:
		TFileMap.cpp

	DESCRIPTION:
		NFileMap tests.

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
#include "NData.h"
#include "NFile.h"
#include "NFileHandle.h"
#include "NFileMap.h"
#include "NFileUtils.h"
#include "NTestFixture.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static constexpr size_t  kBufferSize                        = 9;
static constexpr uint8_t kBufferData[kBufferSize]           = {'T', 'e', 's', 't', ' ', 'd', 'a', 't', 'a'};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileMap)
{
	NFileMap fileMap;
	NFile    theFile;
	NStatus  theErr;

	SETUP
	{
		theFile = NFileUtils::GetTemporary();
		theErr  = NFileHandle::WriteData(theFile, NData(kBufferSize, kBufferData));
		REQUIRE_NOT_ERR(theErr);
	}

	TEARDOWN
	{
		if (theFile.Exists())
		{
			theErr = theFile.Delete();
			REQUIRE_NOT_ERR(theErr);
		}
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileMap, "Default")
{


	// Perform the test
	REQUIRE(!fileMap.IsOpen());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileMap, "Open")
{


	// Perform the test
	theErr = fileMap.Open(theFile);
	REQUIRE_NOT_ERR(theErr);

	REQUIRE(fileMap.IsOpen());

	fileMap.Close();
	REQUIRE(!fileMap.IsOpen());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFileMap, "Map")
{


	// Perform the test
	theErr = fileMap.Open(theFile);
	REQUIRE_NOT_ERR(theErr);

	const void* thePtr = fileMap.Map(0, kBufferSize);
	REQUIRE(memcmp(thePtr, kBufferData, kBufferSize) == 0);

	fileMap.Unmap(thePtr);

	fileMap.Close();
	REQUIRE(!fileMap.IsOpen());
}
