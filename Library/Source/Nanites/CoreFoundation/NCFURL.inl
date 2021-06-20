/*	NAME:
		NCFURL.inl

	DESCRIPTION:
		CFURLRef wrapper.

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
#include "NCFString.h"





//=============================================================================
//		NCFURL::NCFURL : Constructor.
//-----------------------------------------------------------------------------
inline NCFURL::NCFURL(const NURL& theURL)
{


	// Initialise ourselves
	SetURL(theURL);
}





//=============================================================================
//		NCFURL::GetURL : Get the URL.
//-----------------------------------------------------------------------------
NURL NCFURL::GetURL() const
{


	// Get the URL
	CFURLRef cfURL = *this;
	NURL     theURL;

	if (cfURL != nullptr)
	{
		NCFString cfString;

		if (cfString.Set(CFURLGetString(cfURL)))
		{
			theURL.SetURL(cfString.GetString());
		}
	}

	return theURL;
}





//=============================================================================
//		NCFURL::SetURL : Set the URL.
//-----------------------------------------------------------------------------
bool NCFURL::SetURL(const NURL& theURL)
{


	// Set the URL
	return Set(CFURLCreateWithString(kCFAllocatorDefault, NCFString(theURL.GetURL()), nullptr));
}
