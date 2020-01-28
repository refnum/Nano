/*	NAME:
		NStringEncoder.h

	DESCRIPTION:
		String encoder.

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
#ifndef NSTRING_ENCODER_H
#define NSTRING_ENCODER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NStringEncodings.h"
#include "NanoConstants.h"

// System
#include <unordered_map>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
using NUTF32LegacyMap                                       = std::unordered_map<utf32_t, uint8_t>;





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
class NData;
class NUnicodeView;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NStringEncoder
{
public:
	// Convert text
	//
	// Any BOM in the input text is always removed.
	//
	// The output text will always conain a null terminator.
	void                                Convert(NStringEncoding srcEncoding,
												const NData&    srcData,
												NStringEncoding dstEncoding,
												NData&          dstData);


	// Get a native encoding
	//
	// Returns the native equivalent of a potentially endian-specific encoding.
	static NStringEncoding              GetNativeEncoding(NStringEncoding theEncoding);


	// Get the size of a code unit
	static size_t                       GetCodeUnitSize(NStringEncoding theEncoding);


private:
	void                                ProcessInput( NStringEncoding theEncoding, NData& theData);
	void                                ProcessOutput(NStringEncoding theEncoding, NData& theData);

	void                                RemoveBOM(NData& theData);
	void                                RemoveNull(NStringEncoding theEncoding, NData& theData);
	void                                SwapUTF(   NStringEncoding theEncoding, NData& theData);

	size_t                              GetElementSize(NStringEncoding theEncoding);

	void                                ConvertFromUTF8( const NData& srcData, NStringEncoding dstEncoding, NData& dstData);
	void                                ConvertFromUTF16(const NData& srcData, NStringEncoding dstEncoding, NData& dstData);
	void                                ConvertFromUTF32(const NData& srcData, NStringEncoding dstEncoding, NData& dstData);

	void                                ConvertToUTF8( NUnicodeView& srcView, NData& dstData);
	void                                ConvertToUTF16(NUnicodeView& srcView, NData& dstData);
	void                                ConvertToUTF32(NUnicodeView& srcView, NData& dstData);

	void                                ConvertFromLegacy(NStringEncoding srcEncoding,
														  const NData&    srcData,
														  NStringEncoding dstEncoding,
														  NData&          dstData);

	void                                ConvertToLegacy(NUnicodeView& srcView, NStringEncoding dstEncoding, NData& dstData);

	const utf32_t*                      GetLegacyToUTF32(  NStringEncoding srcEncoding);
	const NUTF32LegacyMap*              GetLegacyFromUTF32(NStringEncoding dstEncoding);
	NUTF32LegacyMap                     GetLegacyMap(size_t maxChar, const utf32_t* toUTF32);
};



#endif // NSTRING_ENCODER_H
