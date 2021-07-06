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
#include "NVersion.h"
#include "NWindows.h"

// System
#include <VersionHelpers.h>
#include <stdlib.h>
#include <sysinfoapi.h>





//=============================================================================
//		NSystem::GetEnv : Get an environment variable.
//-----------------------------------------------------------------------------
NString NSystem::GetEnv(const NString& theName)
{


	// Valiate our parameters
	NN_REQUIRE(!theName.IsEmpty());


	// Get the variable
	NString        theValue;
	const wchar_t* winName  = reinterpret_cast<const wchar_t*>(theName.GetUTF16());
	const utf16_t* winValue = reinterpret_cast<const utf16_t*>(_wgetenv(winName));

	if (winValue != nullptr)
	{
		theValue = NString(winValue);
	}

	return theValue;
}





//=============================================================================
//		NSystem::SetEnv : Set an environment variable.
//-----------------------------------------------------------------------------
void NSystem::SetEnv(const NString& theName, const NString& theValue)
{


	// Valiate our parameters
	NN_REQUIRE(!theName.IsEmpty());


	// Set the variable
	NString nameValue = theName + "=" + theValue;

	int sysErr = _wputenv(reinterpret_cast<const wchar_t*>(nameValue.GetUTF16()));
	NN_EXPECT_NOT_ERR(sysErr);
}





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
NVersion NSystem::GetVersion()
{


	// Get the version
	if (IsWindows10OrGreater())
	{
		return kNOSWindows_10;
	}
	else if (IsWindows8OrGreater())
	{
		return kNOSWindows_8;
	}
	else if (IsWindows7OrGreater())
	{
		return kNOSWindows_7;
	}
	else if (IsWindowsVistaOrGreater())
	{
		return kNOSWindows_Vista;
	}
	else if (IsWindowsXPOrGreater())
	{
		return kNOSWindows_XP;
	}

	return NVersion(kNOSWindows, 0);
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
			NString csdVersion = ToNN(theInfo.szCSDVersion);
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
