/*	NAME:
		NStringEncodings.h

	DESCRIPTION:
		String encodings.

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
#ifndef NSTRING_ENCODINGS_H
#define NSTRING_ENCODINGS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NanoTypes.h"

// System
#include <vector>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// String encoding
enum class NStringEncoding
{
	// Meta
	Unknown,


	// Unicode
	//
	// UTF16 and UTF32 store data in native byte order.
	UTF8,
	UTF16,
	UTF32,


	// Unicode
	//
	// BE and LE encodings store data in big-endian or little-endian order.
	UTF16BE,
	UTF16LE,

	UTF32BE,
	UTF32LE,


	// Legacy
	//
	// Conversion from legacy encodings to Unicode is exact.
	//
	// Conversion from Unicode to legacy encodings may be lossy, as
	// characters that cannot be represented in the legacy encoding
	// are replaced with a placeholder.
	ASCII,
	MacRoman,
	ISOLatin1,       // ISO 8859-1
	WindowsLatin1    // ANSI 1252
};


// Replacements
static constexpr utf32_t kNUTF32Replacement                 = 0xFFFD;
static constexpr char    kNASCIIReplacement                 = '?';


// Misc
static constexpr utf8_t  kNUTF8VariableWidth                = utf8_t(0x80U);
static constexpr utf16_t kNUTF16SurrogatesStart             = utf16_t(0xD800);
static constexpr utf16_t kNUTF16SurrogatesEnd               = utf16_t(0xDFFF);
static constexpr utf16_t kNUTF16SurrogateHiStart            = utf16_t(0xD800);
static constexpr utf16_t kNUTF16SurrogateHiEnd              = utf16_t(0xDBFF);
static constexpr utf16_t kNUTF16SurrogateLoStart            = utf16_t(0xDC00);
static constexpr utf16_t kNUTF16SurrogateLoEnd              = utf16_t(0xDFFF);





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
using NVectorStringEncoding                                 = std::vector<NStringEncoding>;


#endif // NSTRING_ENCODINGS_H
