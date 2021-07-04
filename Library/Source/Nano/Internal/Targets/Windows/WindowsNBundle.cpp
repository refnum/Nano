/*	NAME:
		WindowsNBundle.cpp

	DESCRIPTION:
		Windows bundle support.

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
#include "NBundle.h"

// Nano
#include "NFormat.h"
#include "NPropertyList.h"
#include "NWindows.h"





//=============================================================================
//		NBundle::GetBundle : Get the bundle for an ID.
//-----------------------------------------------------------------------------
NFile NBundle::GetBundle(const NString& bundleID) const
{


	// Locate the executable
	NFile theFile;

	if (bundleID.IsEmpty())
	{
		TCHAR theBuffer[MAX_PATH];

		if (GetModuleFileName(nullptr, theBuffer, MAX_PATH))
		{
			theFile = NFile(ToNN(theBuffer));
		}
	}
	else
	{
		NN_LOG_WARNING("Unable to locate bundle {}", bundleID);
	}



	// Locate the bundle
	//
	// If the executable is within a bundle then we return the root bundle
	// folder, otherwise we return the directory containing the executable.
	theFile = theFile.GetParent();

	if (theFile.GetName() == "Windows")
	{
		NFile theParent = theFile.GetParent();
		if (theParent.GetName() == "Contents")
		{
			theFile = theParent.GetParent();
		}
	}

	return theFile;
}





//=============================================================================
//		NBundle::GetResources : Get the resources directory for the bundle.
//-----------------------------------------------------------------------------
NFile NBundle::GetResources(const NFile& theBundle) const
{


	// Get the resources
	return theBundle.GetChild("Contents\\Resources");
}





//=============================================================================
//		NBundle::GetInfoDictionary : Get the Info.plist dictionary for a bundle.
//-----------------------------------------------------------------------------
NDictionary NBundle::GetInfoDictionary(const NFile& theBundle) const
{


	// Get the info
	NFile theFile = theBundle.GetChild("Contents\\Info.plist");

	if (theFile.IsFile())
	{
		return NPropertyList::Load(theFile);
	}

	return {};
}





//=============================================================================
//		NBundle::GetExecutable : Get an executable from a bundle.
//-----------------------------------------------------------------------------
NFile NBundle::GetExecutable(const NFile& theBundle, const NString& theName) const
{


	// Get the executable
	return theBundle.GetChild("Contents\\Windows").GetChild(theName);
}
