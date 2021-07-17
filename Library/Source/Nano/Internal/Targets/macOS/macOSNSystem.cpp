/*	NAME:
		macOSNSystem.cpp

	DESCRIPTION:
		macOS system support.

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
#include "NCommonDarwin.h"
#include "NCommonPOSIX.h"
#include "NVersion.h"





//=============================================================================
//		NSystem::GetEnv : Get an environment variable.
//-----------------------------------------------------------------------------
NString NSystem::GetEnv(const NString& theName)
{


	// Get the variable
	return NCommonPOSIX::EnvGet(theName);
}





//=============================================================================
//		NSystem::SetEnv : Set an environment variable.
//-----------------------------------------------------------------------------
void NSystem::SetEnv(const NString& theName, const NString& theValue)
{


	// Set the variable
	NCommonPOSIX::EnvSet(theName, theValue);
}





//=============================================================================
//		NSystem::GetPageSize : Get the page size.
//-----------------------------------------------------------------------------
size_t NSystem::GetPageSize()
{


	// Get the page size
	return NCommonDarwin::SystemPageSize();
}





//=============================================================================
//		NSystem::GetVersion : Get the OS version.
//-----------------------------------------------------------------------------
NVersion NSystem::GetVersion()
{


	// Get the version
	return NCommonDarwin::SystemVersion();
}





//=============================================================================
//		NSystem::SystemName : Get the OS name.
//-----------------------------------------------------------------------------
NString NSystem::SystemName(NOSName theName)
{


	// Get the version
	return NCommonDarwin::SystemName(theName);
}
