/*	NAME:
		NStringEncoder.h

	DESCRIPTION:
		String encoder.

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
#ifndef NSTRING_ENCODER_H
#define NSTRING_ENCODER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFlags.h"
#include "NStringEncodings.h"
#include "NanoConstants.h"

// System
#include <unordered_map>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Encoder flags
//
// A removal flag will remove the feature if present.
//
// An addition flag will always add the feature to the output.
//
// Removal flags are applied before addition flags. This allows text to be
// normalised by requesting both addition and removal of a feature.
//
// For example, kNStringEncodeBOMTerminator will ensure the output string has
// both a BOM and terminator regardless of the presence of a BOM or terminator
// in the input string.
enum class NStringEncode
{
	RemoveBOM,
	RemoveTerminator,
	AddBOM,
	AddTerminator
};

NN_FLAG_ENUM(NStringEncode, NStringEncodeFlags);

inline constexpr NStringEncodeFlags kNStringEncodeNone;

inline constexpr NStringEncodeFlags kNStringEncodeBOM =
	NStringEncode::RemoveBOM | NStringEncode::AddBOM;

inline constexpr NStringEncodeFlags kNStringEncodeTerminator =
	NStringEncode::RemoveTerminator | NStringEncode::AddTerminator;

inline constexpr NStringEncodeFlags kNStringEncodeBOMTerminator =
	kNStringEncodeBOM | kNStringEncodeTerminator;





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Classes
class NData;
class NUnicodeView;


// Map
using NMapUTF32Legacy = std::unordered_map<utf32_t, uint8_t>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NStringEncoder
{
public:
	// Convert text
	static void                         Convert(NStringEncoding    srcEncoding,
												const NData&       srcData,
												NStringEncoding    dstEncoding,
												NData&             dstData,
												NStringEncodeFlags theFlags = kNStringEncodeNone);


	// Get a native encoding
	//
	// Returns the native equivalent to any endian-specific encodings.
	static constexpr NStringEncoding    GetNativeEncoding(NStringEncoding theEncoding);


	// Identify an unknown encoding
	//
	// Returns NStringEncoding::Unknown if the encoding could not be determined.
	static NStringEncoding              GetUnknownEncoding(const NData& theData);


	// Get the size of a code unit
	static constexpr size_t             GetCodeUnitSize(NStringEncoding theEncoding);


private:
	static void                         ProcessInput(NStringEncodeFlags theFlags,
													 bool               swapInput,
													 NStringEncoding    srcEncoding,
													 NData&             srcData);

	static void                         ProcessOutput(NStringEncodeFlags theFlags,
													  bool               swapOutput,
													  NStringEncoding    dstEncoding,
													  NData&             dstData);

	static void                         AddBOM(   NStringEncoding theEncoding, NData& theData);
	static void                         RemoveBOM(NStringEncoding theEncoding, NData& theData);

	static void                         AddTerminator(   NStringEncoding theEncoding, NData& theData);
	static void                         RemoveTerminator(NStringEncoding theEncoding, NData& theData);

	static bool                         ShouldSwap(NStringEncoding theEncoding);
	static void                         SwapUTF(   NStringEncoding theEncoding, NData& theData);


	static void                         ConvertText(NStringEncoding srcEncoding,
													const NData&    srcData,
													NStringEncoding dstEncoding,
													NData&          dstData);

	static void                         ConvertFromUTF8( const NData& srcData, NStringEncoding dstEncoding, NData& dstData);
	static void                         ConvertFromUTF16(const NData& srcData, NStringEncoding dstEncoding, NData& dstData);
	static void                         ConvertFromUTF32(const NData& srcData, NStringEncoding dstEncoding, NData& dstData);
	static void                         ConvertFromLegacy(NStringEncoding srcEncoding,
														  const NData&    srcData,
														  NStringEncoding dstEncoding,
														  NData&          dstData);

	static void                         ConvertToUTF8(  NUnicodeView& srcView, NData& dstData);
	static void                         ConvertToUTF16( NUnicodeView& srcView, NData& dstData);
	static void                         ConvertToUTF32( NUnicodeView& srcView, NData& dstData);
	static void                         ConvertToLegacy(NUnicodeView& srcView, NStringEncoding dstEncoding, NData& dstData);

	static const utf32_t*               GetLegacyToUTF32(  NStringEncoding srcEncoding);
	static const NMapUTF32Legacy*       GetLegacyFromUTF32(NStringEncoding dstEncoding);
	static NMapUTF32Legacy              GetLegacyMap(size_t maxChar, const utf32_t* toUTF32);
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NStringEncoder.inl"


#endif // NSTRING_ENCODER_H
