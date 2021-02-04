/*	NAME:
		LinuxNSystem.cpp

	DESCRIPTION:
		Linux system support.

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
#include "NSharedLinux.h"

// System
#include <sys/utsname.h>





//=============================================================================
//		NSystem::GetPageSize : Get the page size.
//-----------------------------------------------------------------------------
size_t NSystem::GetPageSize()
{


	// Get the page size
	return NSharedLinux::SystemPageSize();
}





//=============================================================================
//		NSystem::GetVersion : Get the OS version.
//-----------------------------------------------------------------------------
NOSVersion NSystem::GetVersion()
{


	// Get the version
	struct utsname sysInfo
	{
	};
	int majorVersion = 0;
	int minorVersion = 0;
	int patchVersion = 0;

	int sysErr = uname(&sysInfo);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr == 0)
	{
		if (sscanf(sysInfo.release, "%d.%d.%d", &majorVersion, &minorVersion, &patchVersion) != 3)
		{
			if (sscanf(sysInfo.release, "%d.%d", &majorVersion, &minorVersion) != 2)
			{
				sysErr = -1;
			}
		}
	}



	// Encode the version
	NOSVersion theVersion = kNOSLinux;

	theVersion |= NOSVersion((osVersion.majorVersion & 0xFF) * 0x10000);
	theVersion |= NOSVersion((osVersion.minorVersion & 0xFF) * 0x100);
	theVersion |= NOSVersion((osVersion.patchVersion & 0xFF));

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
	NString fullVersion = NSharedLinux::GetProcFile(NFilePath("/proc/version"));



	// Get the name
	NString theText;

	if (theName == NOSName::Build)
	{
		theText = fullVersion.GetMatch("Linux version .*?-(.*?) ");
	}
	else
	{
		theText = NFormat("{} [{}]", NSystem::GetName(NOSName::Detailed), fullVersion);
	}

	return theText;
}
