/*	NAME:
		NCFString.cpp

	DESCRIPTION:
		CFStringRef wrapper.

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

// Nano
#include "NCFData.h"





//=============================================================================
//		NCFString::GetString : Get the string.
//-----------------------------------------------------------------------------
NString NCFString::GetString() const
{


	// Get the string
	CFStringRef cfString = *this;
	NString     theString;

	if (cfString != nullptr)
	{
		// Direct access to UTF8
		const utf8_t* textUTF8 = CFStringGetCStringPtr(cfString, kCFStringEncodingUTF8);
		if (textUTF8 != nullptr)
		{
			theString = NString(textUTF8);
		}
		else
		{
			// Direct access to UTF16
			const UniChar* textUTF16 = CFStringGetCharactersPtr(cfString);
			if (textUTF16 != nullptr)
			{
				theString = NString(reinterpret_cast<const utf16_t*>(textUTF16));
			}
			else
			{
				NCFData cfData;

				// Convert to UTF8 through a temporary buffer
				if (cfData.Set(CFStringCreateExternalRepresentation(kCFAllocatorDefault,
																	cfString,
																	kCFStringEncodingUTF8,
																	0)))
				{
					theString.SetData(NStringEncoding::UTF8, cfData.GetData());
				}
				else
				{
					NN_LOG_ERROR("Unable to convert CFString to NString!");
				}
			}
		}
	}

	return theString;
}





//=============================================================================
//		NCFString::SetString : Set the string.
//-----------------------------------------------------------------------------
bool NCFString::SetString(const NString& theString)
{


	// Set the string
	return Set(
		CFStringCreateWithCString(kCFAllocatorDefault, theString.GetUTF8(), kCFStringEncodingUTF8));
}
