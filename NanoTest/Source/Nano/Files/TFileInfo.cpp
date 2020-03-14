/*	NAME:
		TFileInfo.cpp

	DESCRIPTION:
		NFileInfo tests.

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
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
#if NN_TARGET_ANDROID
static const NString kPathFile                              = "/bin/bash";
static const NString kPathDirectory                         = "/tmp";
static const NString kPathDoesNotExist                      = "/63785644-da36-4148-939f-4416cb5ea56e";

#elif NN_TARGET_IOS
static const NString kPathFile                              = "/bin/bash";
static const NString kPathDirectory                         = "/tmp";
static const NString kPathDoesNotExist                      = "/63785644-da36-4148-939f-4416cb5ea56e";


#elif NN_TARGET_LINUX
static const NString kPathFile                              = "/bin/bash";
static const NString kPathDirectory                         = "/tmp";
static const NString kPathDoesNotExist                      = "/63785644-da36-4148-939f-4416cb5ea56e";


#elif NN_TARGET_MACOS
static const NString kPathFile                              = "/bin/bash";
static const NString kPathDirectory                         = "/tmp";
static const NString kPathDoesNotExist                      = "/63785644-da36-4148-939f-4416cb5ea56e";


#elif NN_TARGET_TVOS
static const NString kPathFile                              = "/bin/bash";
static const NString kPathDirectory                         = "/tmp";
static const NString kPathDoesNotExist                      = "/63785644-da36-4148-939f-4416cb5ea56e";

#elif NN_TARGET_WINDOWS
static const NString kPathFile                              = "c:\\Windows\\WindowsShell.Manifest";
static const NString kPathDirectory                         = "c:\\Windows\\temp";
static const NString kPathDoesNotExist                      = "c:\\63785644-da36-4148-939f-4416cb5ea56e";

#else
	#error "Unknown target"
#endif





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFileInfo)
{
	NFileInfo theInfo;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "SetPath")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.GetPath() == kPathFile);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Refresh")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	theInfo.Refresh();
	REQUIRE(theInfo.GetPath() == kPathFile);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Status")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.Exists());
	REQUIRE(theInfo.IsFile());
	REQUIRE(!theInfo.IsDirectory());

	theInfo.SetPath(kPathDirectory);
	REQUIRE(theInfo.Exists());
	REQUIRE(!theInfo.IsFile());
	REQUIRE(theInfo.IsDirectory());

	theInfo.SetPath(kPathDoesNotExist);
	REQUIRE(!theInfo.Exists());
	REQUIRE(!theInfo.IsFile());
	REQUIRE(!theInfo.IsDirectory());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Permission")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.CanRead());
	REQUIRE(!theInfo.CanWrite());
	REQUIRE(theInfo.CanExecute());

	theInfo.SetPath(kPathDirectory);
	REQUIRE(theInfo.CanRead());
	REQUIRE(theInfo.CanWrite());
	REQUIRE(theInfo.CanExecute());

	theInfo.SetPath(kPathDoesNotExist);
	REQUIRE(!theInfo.CanRead());
	REQUIRE(!theInfo.CanWrite());
	REQUIRE(!theInfo.CanExecute());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "Timestamps")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.GetCreationTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() >= theInfo.GetCreationTime());

	theInfo.SetPath(kPathDirectory);
	REQUIRE(theInfo.GetCreationTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() != 0.0);
	REQUIRE(theInfo.GetModifiedTime() >= theInfo.GetCreationTime());

	theInfo.SetPath(kPathDoesNotExist);
	REQUIRE(theInfo.GetCreationTime() == 0.0);
	REQUIRE(theInfo.GetModifiedTime() == 0.0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TFileInfo, "File size")
{


	// Perform the test
	theInfo.SetPath(kPathFile);
	REQUIRE(theInfo.GetFileSize() != 0);
}
