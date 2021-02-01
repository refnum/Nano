/*	NAME:
		AndroidNSystem.cpp

	DESCRIPTION:
		Android system support.

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
#include "NNumber.h"
#include "NSharedLinux.h"


// System
#include <sys/system_properties.h>





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
	char theBuffer[PROP_VALUE_MAX + 1]{};
	int  strLen = __system_property_get("ro.build.version.release", theBuffer);
	NN_EXPECT(strLen != 0);



	// Encode the version
	NVectorString theTokens  = NString(theBuffer).Split(".");
	NOSVersion    theVersion = kNOSAndroid;

	if (theTokens.size() > 0)
	{
		uint32_t majorVersion = NNumber(theTokens[0]).GetUInt32();
		theVersion |= NOSVersion((majorVersion & 0xFF) * 0x10000);
	}

	if (theTokens.size() > 1)
	{
		uint32_t minorVersion = NNumber(theTokens[1]).GetUInt32();
		theVersion |= NOSVersion((minorVersion & 0xFF) * 0x100);
	}

	if (theTokens.size() > 2)
	{
		uint32_t patchVersion = NNumber(theTokens[2]).GetUInt32();
		theVersion |= NOSVersion(patchVersion & 0xFF);
	}

	return theVersion;
}
