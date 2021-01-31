/*	NAME:
		NTextUtils.cpp

	DESCRIPTION:
		Text utilities.

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
#include "NTextUtils.h"

// Nano
#include "NData.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr char kHexMap[] =
	{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};





//=============================================================================
//		NTextUtils::GetHex : Get a hex string.
//-----------------------------------------------------------------------------
NString NTextUtils::GetHex(const NData& theData)
{


	// Get the hex
	return NTextUtils::GetHex(theData.GetSize(), theData.GetData());
}





//=============================================================================
//		NTextUtils::GetHex : Get a hex string.
//-----------------------------------------------------------------------------
NString NTextUtils::GetHex(size_t theSize, const void* thePtr)
{


	// Get the hex
	const uint8_t*    theBytes = static_cast<const uint8_t*>(thePtr);
	std::vector<char> hexChars(theSize * 2);

	for (size_t n = 0; n < theSize; n++)
	{
		hexChars[(2 * n) + 0] = kHexMap[(theBytes[n] & 0xF0) >> 4];
		hexChars[(2 * n) + 1] = kHexMap[(theBytes[n] & 0x0F) >> 0];
	}

	return NString(NStringEncoding::UTF8, hexChars.size(), &hexChars[0]);
}





//=============================================================================
//		NTextUtils::RemoveEmpty : Remove empty strings from a list.
//-----------------------------------------------------------------------------
NVectorString NTextUtils::RemoveEmpty(const NVectorString& theStrings)
{


	// Remove any empty strings
	NVectorString theResult(theStrings);

	nstd::erase_if(theResult, [](const NString& theString) {
		return theString.IsEmpty();
	});

	return theResult;
}





//=============================================================================
//		NTextUtils::Join : Join a list of strings.
//-----------------------------------------------------------------------------
NString NTextUtils::Join(const NVectorString& theStrings, const NString& joinWith)
{


	// Join the strings
	NString theResult;

	for (const auto& theString : theStrings)
	{
		theResult += theString;
		theResult += joinWith;
	}

	theResult.RemoveSuffix(joinWith.GetSize());

	return theResult;
}





//=============================================================================
//		NTextUtils::Repeat : Repeat a string.
//-----------------------------------------------------------------------------
NString NTextUtils::Repeat(size_t numRepeats, const NString& theString)
{


	// Get the string
	NString theResult;

	for (size_t n = 0; n < numRepeats; n++)
	{
		theResult += theString;
	}

	return theResult;
}
