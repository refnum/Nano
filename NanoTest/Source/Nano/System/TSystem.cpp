/*	NAME:
		TSystem.cpp

	DESCRIPTION:
		NSystem tests.

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
// Nano
#include "NSystem.h"
#include "NTestFixture.h"
#include "NVersion.h"





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TSystem){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TSystem, "GetEnv")
{


	// Perform the test
	REQUIRE(!NSystem::GetEnv("PATH").IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TSystem, "SetEnv")
{


	// Perform the test
	NString theName  = "TSystem_setenv";
	NString theValue = "0123456789";

	REQUIRE(NSystem::GetEnv(theName) == "");

	NSystem::SetEnv(theName, theValue);
	REQUIRE(NSystem::GetEnv(theName) == theValue);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TSystem, "GetPageSize")
{


	// Perform the test
	REQUIRE(NSystem::GetPageSize() > 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TSystem, "GetVersion")
{


	// Perform the test
	NVersion theVersion = NSystem::GetVersion();
	REQUIRE(theVersion.IsValid());

#if NN_TARGET_ANDROID
	REQUIRE(theVersion >= kNOSAndroid_11);

#elif NN_TARGET_LINUX
	REQUIRE(theVersion >= kNOSLinux_5);

#elif NN_TARGET_IOS
	REQUIRE(theVersion >= kNOSiOS_14);

#elif NN_TARGET_MACOS
	REQUIRE(theVersion >= kNOSmacOS_11);

#elif NN_TARGET_TVOS
	REQUIRE(theVersion >= kNOStvOS_14);

#elif NN_TARGET_WINDOWS
	REQUIRE(theVersion >= kNOSWindows_8);
#else
	NN_LOG_WARNING("Unknown platform!");
#endif
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TSystem, "GetName")
{


	// Perform the test
	REQUIRE(!NSystem::GetName(NOSName::Platform).IsEmpty());
	REQUIRE(!NSystem::GetName(NOSName::Version).IsEmpty());
	REQUIRE(!NSystem::GetName(NOSName::Build).IsEmpty());
	REQUIRE(!NSystem::GetName(NOSName::Short).IsEmpty());
	REQUIRE(!NSystem::GetName(NOSName::Detailed).IsEmpty());
	REQUIRE(!NSystem::GetName(NOSName::Maximum).IsEmpty());
}
