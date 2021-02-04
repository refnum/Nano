/*	NAME:
		NSystem.cpp

	DESCRIPTION:
		System support.

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





//=============================================================================
//		NSystem::GetName : Get the OS name.
//-----------------------------------------------------------------------------
NString NSystem::GetName(NOSName theName)
{


	// Get the name
	NString theText;

	switch (theName)
	{
		case NOSName::Platform:
			if (NN_TARGET_ANDROID)
			{
				theText = "Android";
			}
			else if (NN_TARGET_IOS)
			{
				theText = "iOS";
			}
			else if (NN_TARGET_LINUX)
			{
				theText = "Linux";
			}
			else if (NN_TARGET_MACOS)
			{
				theText = "macOS";
			}
			else if (NN_TARGET_TVOS)
			{
				theText = "tvOS";
			}
			else if (NN_TARGET_WINDOWS)
			{
				theText = "Windows";
			}
			break;


		case NOSName::Version:
			theText = GetVersion().GetString();
			break;


		case NOSName::Short:
			theText = NFormat("{} {}", GetName(NOSName::Platform), GetName(NOSName::Version));
			break;


		case NOSName::Detailed:
			NN_DIAGNOSTIC_IGNORE_MSVC(4840);
			theText = NFormat("{} {} ({})",
							  GetName(NOSName::Platform),
							  GetName(NOSName::Version),
							  GetName(NOSName::Build));
			break;


		case NOSName::Build:
		case NOSName::Maximum:
			theText = SystemName(theName);
			break;
	}

	return theText;
}
