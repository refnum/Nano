/*	NAME:
		WindowsNSystem.cpp

	DESCRIPTION:
		Windows system support.

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
#include "NSystem.h"

// Nano
#include "NFormat.h"
#include "NSharedWindows.h"

// System
#include <VersionHelpers.h>
#include <Windows.h>
#include <sysinfoapi.h>





//=============================================================================
//		NSystem::GetPageSize : Get the page size.
//-----------------------------------------------------------------------------
size_t NSystem::GetPageSize()
{


	// Get the page size
	SYSTEM_INFO systemInfo{};

	GetSystemInfo(&systemInfo);
	NN_REQUIRE(systemInfo.dwPageSize > 0);

	return size_t(systemInfo.dwPageSize);
}





//=============================================================================
//		NSystem::GetVersion : Get the OS version.
//-----------------------------------------------------------------------------
NOSVersion NSystem::GetVersion()
{


	// Get the version
	NOSVersion theVersion = kNOSWindows;

	if (IsWindows10OrGreater())
	{
		theVersion = kNOSWindows_10;
	}
	else if (IsWindows8OrGreater())
	{
		theVersion = kNOSWindows_8;
	}
	else if (IsWindows7OrGreater())
	{
		theVersion = kNOSWindows_7;
	}
	else if (IsWindowsVistaOrGreater())
	{
		theVersion = kNOSWindows_Vista;
	}
	else if (IsWindowsXPOrGreater())
	{
		theVersion = kNOSWindows_XP;
	}

	return theVersion;
}





//=============================================================================
//		NSystem::SystemName : Get the OS name.
//-----------------------------------------------------------------------------
NString NSystem::SystemName(NOSName theName)
{


	// Validate our parameters and state
	NN_REQUIRE(theName == NOSName::Build || theName == NOSName::Maximum);



	// Get the state we need
	NN_DIAGNOSTIC_PUSH();
	NN_DIAGNOSTIC_IGNORE_MSVC(4996);    // Deprecated function

	OSVERSIONINFOEX theInfo{};
	theInfo.dwOSVersionInfoSize = sizeof(theInfo);

	BOOL wasOK = GetVersionEx((LPOSVERSIONINFO) &theInfo);
	NN_EXPECT(wasOK);

	NN_DIAGNOSTIC_POP();



	// Get the name
	NString theText;

	if (wasOK)
	{
		if (theName == NOSName::Build)
		{
			theText = NFormat("{}", theInfo.dwBuildNumber);
		}
		else
		{
			NString csdVersion = NSharedWindows::ToString(theInfo.szCSDVersion);
			if (!csdVersion.IsEmpty())
			{
				csdVersion += " ";
			}

			NN_DIAGNOSTIC_IGNORE_MSVC(4840);
			theText = NFormat("{} [{}{}.{}-{}]",
							  NSystem::GetName(NOSName::Detailed),
							  csdVersion,
							  theInfo.wServicePackMajor,
							  theInfo.wServicePackMinor,
							  theInfo.wProductType);
		}
	}

	return theText;
}
