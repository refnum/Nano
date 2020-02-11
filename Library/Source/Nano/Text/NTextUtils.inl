/*	NAME:
		NTextUtils.inl

	DESCRIPTION:
		Text utilities.

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





//=============================================================================
//		NTextUtils::GetLower : Convert to lower case.
//-----------------------------------------------------------------------------
constexpr utf32_t NTextUtils::GetLower(utf32_t theChar)
{


	// Convert to lower case
	if (theChar >= U'A' && theChar <= U'Z')
	{
		theChar = U'a' + (theChar - U'A');
	}

	return theChar;
}





//=============================================================================
//		NTextUtils::GetUpper : Convert to upper case.
//-----------------------------------------------------------------------------
constexpr utf32_t NTextUtils::GetUpper(utf32_t theChar)
{


	// Convert to upper case
	if (theChar >= U'a' && theChar <= U'z')
	{
		theChar = U'A' + (theChar - U'a');
	}

	return theChar;
}





//=============================================================================
//		NTextUtils::IsDigit : Is a character a digit?
//-----------------------------------------------------------------------------
constexpr bool NTextUtils::IsDigit(utf32_t theChar)
{


	// Test the character
	return theChar >= U'0' && theChar <= U'9';
}





//=============================================================================
//		NTextUtils::IsAlpha : Is a character alphabetic?
//-----------------------------------------------------------------------------
constexpr bool NTextUtils::IsAlpha(utf32_t theChar)
{


	// Test the character
	return (theChar >= U'A' && theChar <= U'Z') || (theChar >= U'a' && theChar <= U'z');
}





//=============================================================================
//		NTextUtils::IsSentence : Is a character a sentence terminator?
//-----------------------------------------------------------------------------
constexpr bool NTextUtils::IsSentence(utf32_t theChar)
{


	// Test the character
	return theChar == U'!' || theChar == U'.' || theChar == U'?';
}
