/*	NAME:
		NStringTransformer.cpp

	DESCRIPTION:
		String transformer.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#include "NStringTransformer.h"

// Nano
#include "NData.h"
#include "NTextUtils.h"





//=============================================================================
//		NStringTransformer::Transform : Transform a string.
//-----------------------------------------------------------------------------
NString NStringTransformer::Transform(const NString&   theString,
									  NStringTransform theTransform,
									  const NRange&    theRange)
{


	// Validate our parameters
	if (theTransform & kNStringTransformToLower)
	{
		NN_REQUIRE(!(theTransform & kNStringTransformToUpper));
	}

	if (theTransform & kNStringTransformToUpper)
	{
		NN_REQUIRE(!(theTransform & kNStringTransformToLower));
	}

	if (theTransform & kNStringTransformCapitalizeWords)
	{
		NN_REQUIRE(!(theTransform & kNStringTransformCapitalizeSentences));
	}

	if (theTransform & kNStringTransformCapitalizeSentences)
	{
		NN_REQUIRE(!(theTransform & kNStringTransformCapitalizeWords));
	}



	// Transform the string
	NString theResult = theString;

	if (!theResult.IsEmpty() && theTransform != kNStringTransformNone)
	{
		// Get the state we need
		NData theData   = theString.GetData(NStringEncoding::UTF32);
		bool  didUpdate = false;

		NRange finalRange = theRange.GetNormalized(theString.GetSize());
		size_t numUTF32   = finalRange.GetSize();

		utf32_t* textUTF32 = reinterpret_cast<utf32_t*>(theData.GetMutableData());
		textUTF32 += finalRange.GetLocation();



		// Transform the text
		if (theTransform & kNStringTransformToLower)
		{
			if (CapitalizeCharacters(numUTF32, textUTF32, false))
			{
				didUpdate = true;
			}
		}
		else if (theTransform & kNStringTransformToUpper)
		{
			if (CapitalizeCharacters(numUTF32, textUTF32, true))
			{
				didUpdate = true;
			}
		}

		if (theTransform & kNStringTransformCapitalizeWords)
		{
			if (CapitalizeWords(numUTF32, textUTF32))
			{
				didUpdate = true;
			}
		}
		else if (theTransform & kNStringTransformCapitalizeSentences)
		{
			if (CapitalizeSentences(numUTF32, textUTF32))
			{
				didUpdate = true;
			}
		}



		// Update the result
		//
		// If nothing actually changed then we return the original, to
		// allow large strings to continue share data with each other.
		if (didUpdate)
		{
			theResult.SetData(NStringEncoding::UTF32, theData);
		}
	}

	return theResult;
}





//=============================================================================
//		NStringTransformer::CapitalizeCharacters : Capitalize characters.
//-----------------------------------------------------------------------------
bool NStringTransformer::CapitalizeCharacters(size_t numUTF32, utf32_t* textUTF32, bool toUpper)
{


	// Capitalize characters
	bool didUpdate = false;

	for (size_t n = 0; n < numUTF32; n++)
	{
		// Get the state we need
		utf32_t oldChar = textUTF32[n];
		utf32_t newChar = oldChar;



		// Apply the transform
		if (toUpper)
		{
			newChar = NTextUtils::GetUpper(oldChar);
		}
		else
		{
			newChar = NTextUtils::GetLower(oldChar);
		}


		// Update the text
		if (newChar != oldChar)
		{
			textUTF32[n] = newChar;
			didUpdate    = true;
		}
	}

	return didUpdate;
}





//=============================================================================
//		NStringTransformer::CapitalizeWords : Capitalize words.
//-----------------------------------------------------------------------------
bool NStringTransformer::CapitalizeWords(size_t numUTF32, utf32_t* textUTF32)
{


	// Get the state we need
	bool toUpper   = true;
	bool didUpdate = false;



	// Capitalize words
	for (size_t n = 0; n < numUTF32; n++)
	{
		// Get the state we need
		utf32_t oldChar = textUTF32[n];
		utf32_t newChar = oldChar;


		// Apply the transform
		if (NTextUtils::IsAlpha(oldChar))
		{
			if (toUpper)
			{
				newChar = NTextUtils::GetUpper(oldChar);
				toUpper = false;
			}
			else
			{
				newChar = NTextUtils::GetLower(oldChar);
			}
		}
		else
		{
			toUpper = true;
		}


		// Update the text
		if (newChar != oldChar)
		{
			textUTF32[n] = newChar;
			didUpdate    = true;
		}
	}

	return didUpdate;
}





//=============================================================================
//		NStringTransformer::CapitalizeSentences : Capitalize sentences.
//-----------------------------------------------------------------------------
bool NStringTransformer::CapitalizeSentences(size_t numUTF32, utf32_t* textUTF32)
{


	// Get the state we need
	bool toUpper   = true;
	bool didUpdate = false;



	// Capitalize sentences
	for (size_t n = 0; n < numUTF32; n++)
	{
		// Get the state we need
		utf32_t oldChar = textUTF32[n];
		utf32_t newChar = oldChar;


		// Apply the transform
		if (toUpper)
		{
			if (NTextUtils::IsAlpha(oldChar))
			{
				newChar = NTextUtils::GetUpper(oldChar);
				toUpper = false;
			}
		}
		else
		{
			newChar = NTextUtils::GetLower(oldChar);
			toUpper = NTextUtils::IsSentence(oldChar);
		}


		// Update the text
		if (newChar != oldChar)
		{
			textUTF32[n] = newChar;
			didUpdate    = true;
		}
	}

	return didUpdate;
}
