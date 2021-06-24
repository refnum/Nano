/*	NAME:
		NDataEncoder.cpp

	DESCRIPTION:
		Data encoder.

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
#include "NDataEncoder.h"

// Nano
#include "NData.h"
#include "NString.h"
#include "Nano_libb64.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		HexToNibble : Convert a hex character to a nibble.
//----------------------------------------------------------------------------
constexpr uint8_t HexToNibble(utf8_t hexChar)
{


	// Decode the character
	if (hexChar >= '0' && hexChar <= '9')
	{
		return uint8_t(hexChar - '0');
	}
	else if (hexChar >= 'A' && hexChar <= 'F')
	{
		return uint8_t(hexChar - 'A' + 10);
	}
	else if (hexChar >= 'a' && hexChar <= 'f')
	{
		return uint8_t(hexChar - 'a' + 10);
	}

	return 0;
}





#pragma mark NDataEncoder
//=============================================================================
//		NDataEncoder::Encode : Encode to text.
//-----------------------------------------------------------------------------
NString NDataEncoder::Encode(NDataEncoding theEncoding, const NData& theValue)
{


	// Encode the data
	switch (theEncoding)
	{
		case NDataEncoding::Hex:
			return Hex_Encode(theValue);
			break;

		case NDataEncoding::Base64:
			return B64_Encode(theValue);
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NDataEncoder::Decode : Decode from text.
//-----------------------------------------------------------------------------
NData NDataEncoder::Decode(NDataEncoding theEncoding, const NString& theValue)
{


	// Decide the data
	switch (theEncoding)
	{
		case NDataEncoding::Hex:
			return Hex_Decode(theValue);
			break;

		case NDataEncoding::Base64:
			return B64_Decode(theValue);
			break;
	}

	NN_UNREACHABLE();
}





#pragma mark private
//=============================================================================
//		NDataEncoder::Hex_Encode : Encode to hex.
//-----------------------------------------------------------------------------
NString NDataEncoder::Hex_Encode(const NData& theValue)
{


	// Get the state we need
	const uint8_t* srcBytes = theValue.GetData();
	size_t         srcSize  = theValue.GetSize();

	NData   dstData(srcSize * 2, nullptr, NDataSource::None);
	utf8_t* dstChars = reinterpret_cast<utf8_t*>(dstData.GetMutableData());



	// Encode the bytes
	for (size_t n = 0; n < srcSize; n++)
	{
		uint8_t nibbleHi = uint8_t((srcBytes[n] >> 4) & 0x0F);
		uint8_t nibbleLo = uint8_t((srcBytes[n] >> 0) & 0x0F);

		dstChars[(n * 2) + 0] = "0123456789ABCDEF"[nibbleHi];
		dstChars[(n * 2) + 1] = "0123456789ABCDEF"[nibbleLo];
	}

	return NString(NStringEncoding::UTF8, dstData);
}





//=============================================================================
//		NDataEncoder::Hex_Decode : Decode from hex.
//-----------------------------------------------------------------------------
NData NDataEncoder::Hex_Decode(const NString& theValue)
{


	// Validate our parameters
	NN_EXPECT((theValue.GetSize() % 2) == 0, "Input is one nibble short!");



	// Get the state we need
	const utf8_t* srcChars = theValue.GetUTF8();
	size_t        srcSize  = strlen(srcChars);

	NData    dstData(srcSize / 2, nullptr, NDataSource::None);
	uint8_t* dstBytes = dstData.GetMutableData();
	size_t   dstSize  = dstData.GetSize();



	// Decode the hex
	for (size_t n = 0; n < dstSize; n++)
	{
		utf8_t hexHi = srcChars[(n * 2) + 0];
		utf8_t hexLo = srcChars[(n * 2) + 1];

		dstBytes[n] = uint8_t((HexToNibble(hexHi) << 4) | HexToNibble(hexLo));
	}

	return dstData;
}





//=============================================================================
//		NDataEncoder::B64_Encode : Encode to Base64.
//-----------------------------------------------------------------------------
NString NDataEncoder::B64_Encode(const NData& theValue)
{


	// Get the state we need
	const uint8_t* srcBytes = theValue.GetData();
	size_t         srcSize  = theValue.GetSize();

	NData dstData(srcSize * 2, nullptr, NDataSource::None);
	char* dstChars = reinterpret_cast<char*>(dstData.GetMutableData());



	// Encode the bytes
	base64_encodestate stateB64{};
	size_t             dstSize = 0;

	base64_init_encodestate(&stateB64);
	stateB64.chars_per_line = 72;

	dstSize += base64_encode_block(srcBytes, srcSize, dstChars, &stateB64);
	dstSize += base64_encode_blockend(dstChars + dstSize, &stateB64);



	// Trim newlines
	//
	// To ensure consistent output we always trim any trailing newline.
	if (dstSize > 0 && dstChars[dstSize - 1] == '\n')
	{
		dstSize--;
	}

	dstData.SetSize(size_t(dstSize));

	return NString(NStringEncoding::UTF8, dstData);
}





//=============================================================================
//		NDataEncoder::B64_Decode : Decode from Base64.
//-----------------------------------------------------------------------------
NData NDataEncoder::B64_Decode(const NString& theValue)
{


	// Get the state we need
	const char* srcChars = reinterpret_cast<const char*>(theValue.GetUTF8());
	size_t      srcSize  = strlen(srcChars);

	NData    dstData(srcSize, nullptr, NDataSource::None);
	uint8_t* dstBytes = dstData.GetMutableData();



	// Decode the bytes
	base64_decodestate stateB64;

	base64_init_decodestate(&stateB64);

	size_t dstSize = base64_decode_block(srcChars, srcSize, dstBytes, &stateB64);



	// Trim the data
	//
	// Our output buffer is always over-sized so we trim to the actual size.
	dstData.SetSize(dstSize);

	return dstData;
}
