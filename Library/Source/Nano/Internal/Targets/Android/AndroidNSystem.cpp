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
#include "NFormat.h"
#include "NNumber.h"
#include "NSharedLinux.h"
#include "NSharedPOSIX.h"
#include "NVersion.h"

// System
#include <sys/system_properties.h>





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetSystemProperty : Get a system property.
//-----------------------------------------------------------------------------
static NString GetSystemProperty(const NString& theName)
{


	// Get the property
	char theBuffer[PROP_VALUE_MAX + 1]{};
	int  strLen = __system_property_get(theName.GetUTF8(), theBuffer);
	NN_EXPECT(strLen != 0);

	return theBuffer;
}





#pragma mark NSystem
//=============================================================================
//		NSystem::GetEnv : Get an environment variable.
//-----------------------------------------------------------------------------
NString NSystem::GetEnv(const NString& theName)
{


	// Get the variable
	return NSharedPOSIX::EnvGet(theName);
}





//=============================================================================
//		NSystem::SetEnv : Set an environment variable.
//-----------------------------------------------------------------------------
void NSystem::SetEnv(const NString& theName, const NString& theValue)
{


	// Set the variable
	NSharedPOSIX::EnvSet(theName, theValue);
}





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
NVersion NSystem::GetVersion()
{


	// Get the state we need
	NString       theText   = GetSystemProperty("ro.build.version.release");
	NVectorString theTokens = theText.Split(".");



	// Get the version
	NVersion theVersion(kNOSAndroid, 0, 0, 0);

	if (theTokens.size() > 0)
	{
		theVersion.SetMajor(NNumber(theTokens[0]).GetUInt8());
	}

	if (theTokens.size() > 1)
	{
		theVersion.SetMinor(NNumber(theTokens[1]).GetUInt8());
	}

	if (theTokens.size() > 2)
	{
		theVersion.SetPatch(NNumber(theTokens[2]).GetUInt8());
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



	// Get the name
	NString theText;

	if (theName == NOSName::Build)
	{
		theText = GetSystemProperty("ro.build.version.incremental");
	}
	else
	{
		theText = NFormat("{} [{}]",
						  NSystem::GetName(NOSName::Detailed),
						  GetSystemProperty("ro.build.version.base_os"));
	}

	return theText;
}
