/*	NAME:
		NTextUtils.h

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
#ifndef NTEXT_UTILS_H
#define NTEXT_UTILS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
class NData;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NTextUtils
{
public:
	// Convert case
	static constexpr utf32_t            GetLower(utf32_t theChar);
	static constexpr utf32_t            GetUpper(utf32_t theChar);


	// Test a character
	static constexpr bool               IsDigit(   utf32_t theChar);
	static constexpr bool               IsAlpha(   utf32_t theChar);
	static constexpr bool               IsSentence(utf32_t theChar);


	// Get a hex string
	static NString                      GetHex(const NData& theData);
	static NString                      GetHex(size_t theSize, const void* thePtr);


	// Remove empty
	//
	// Remove empty strings from a list
	static NVectorString                RemoveEmpty(const NVectorString& theStrings);


	// Join strings
	static NString                      Join(const NVectorString& theStrings, const NString& joinWith = "");
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NTextUtils.inl"


#endif // NTEXT_UTILS_H
