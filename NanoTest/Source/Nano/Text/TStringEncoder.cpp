/*	NAME:
		TStringEncoder.cpp

	DESCRIPTION:
		NStringEncoder tests.

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
// Nano
#include "NData.h"
#include "NStringEncoder.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Text
static const uint8_t kTextUTF8[] =
	{0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21};

static const uint16_t kTextUTF16[]                          = {0x0048,
									  0x0065,
									  0x006c,
									  0x006c,
									  0x006f,
									  0x0020,
									  0x0057,
									  0x006f,
									  0x0072,
									  0x006c,
									  0x0064,
									  0x0021};

static const uint32_t kTextUTF32[]                          = {0x000048,
									  0x000065,
									  0x00006c,
									  0x00006c,
									  0x00006f,
									  0x000020,
									  0x000057,
									  0x00006f,
									  0x000072,
									  0x00006c,
									  0x000064,
									  0x000021};


// BOM
static constexpr uint8_t kBOM_UTF8[]                        = {0xEF, 0xBB, 0xBF};
static constexpr uint8_t kBOM_UTF16BE[]                     = {0xFE, 0xFF};
static constexpr uint8_t kBOM_UTF16LE[]                     = {0xFF, 0xFE};
static constexpr uint8_t kBOM_UTF32BE[]                     = {0x00, 0x00, 0xFE, 0xFF};
static constexpr uint8_t kBOM_UTF32LE[]                     = {0xFF, 0xFE, 0x00, 0x00};


// Terminator
static constexpr uint8_t kTerminator_UTF8[]                 = {0x00};
static constexpr uint8_t kTerminator_UTF16[]                = {0x00, 0x00};
static constexpr uint8_t kTerminator_UTF32[]                = {0x00, 0x00, 0x00, 0x00};





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TStringEncoder)
{
	NData dataUTF8;
	NData dataUTF16;
	NData dataUTF32;
	NData dataOut;

	SETUP
	{
		dataUTF8.SetData(sizeof(kTextUTF8), kTextUTF8);
		dataUTF16.SetData(sizeof(kTextUTF16), kTextUTF16);
		dataUTF32.SetData(sizeof(kTextUTF32), kTextUTF32);
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "Convert")
{


	// Perform the test
	NStringEncoder::Convert(NStringEncoding::UTF8,
							dataUTF8,
							NStringEncoding::UTF16,
							dataOut,
							kNStringEncoderNone);

	REQUIRE(!dataOut.IsEmpty());
	REQUIRE(dataOut == dataUTF16);


	NStringEncoder::Convert(NStringEncoding::UTF16,
							dataOut,
							NStringEncoding::UTF32,
							dataOut,
							kNStringEncoderNone);

	REQUIRE(!dataOut.IsEmpty());
	REQUIRE(dataOut == dataUTF32);


	NStringEncoder::Convert(NStringEncoding::UTF32,
							dataOut,
							NStringEncoding::ASCII,
							dataOut,
							kNStringEncoderNone);

	REQUIRE(!dataOut.IsEmpty());


	NStringEncoder::Convert(NStringEncoding::ASCII,
							dataOut,
							NStringEncoding::MacRoman,
							dataOut,
							kNStringEncoderNone);

	REQUIRE(!dataOut.IsEmpty());


	NStringEncoder::Convert(NStringEncoding::MacRoman,
							dataOut,
							NStringEncoding::ISOLatin1,
							dataOut,
							kNStringEncoderNone);

	REQUIRE(!dataOut.IsEmpty());


	NStringEncoder::Convert(NStringEncoding::ISOLatin1,
							dataOut,
							NStringEncoding::WindowsLatin1,
							dataOut,
							kNStringEncoderNone);

	REQUIRE(!dataOut.IsEmpty());


	NStringEncoder::Convert(NStringEncoding::WindowsLatin1,
							dataOut,
							NStringEncoding::UTF8,
							dataOut,
							kNStringEncoderNone);

	REQUIRE(!dataOut.IsEmpty());
	REQUIRE(dataOut == dataUTF8);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "BOM/UTF8")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF8,
							dataUTF8,
							NStringEncoding::UTF8,
							dataAdded,
							kNStringEncoderAddBOM);

	REQUIRE(dataAdded.GetSize() == (dataUTF8.GetSize() + 3));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF8), kBOM_UTF8)));


	NStringEncoder::Convert(NStringEncoding::UTF8,
							dataAdded,
							NStringEncoding::UTF8,
							dataRemoved,
							kNStringEncoderRemoveBOM);

	REQUIRE(dataRemoved == dataUTF8);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "BOM/UTF16")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF16,
							dataUTF16,
							NStringEncoding::UTF16BE,
							dataAdded,
							kNStringEncoderAddBOM);

	REQUIRE(dataAdded.GetSize() == (dataUTF16.GetSize() + 2));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF16BE), kBOM_UTF16BE)));


	NStringEncoder::Convert(NStringEncoding::UTF16BE,
							dataAdded,
							NStringEncoding::UTF16,
							dataRemoved,
							kNStringEncoderRemoveBOM);

	REQUIRE(dataRemoved == dataUTF16);


	NStringEncoder::Convert(NStringEncoding::UTF16,
							dataRemoved,
							NStringEncoding::UTF16LE,
							dataAdded,
							kNStringEncoderAddBOM);

	REQUIRE(dataAdded.GetSize() == (dataUTF16.GetSize() + 2));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF16LE), kBOM_UTF16LE)));


	NStringEncoder::Convert(NStringEncoding::UTF16LE,
							dataAdded,
							NStringEncoding::UTF16,
							dataRemoved,
							kNStringEncoderRemoveBOM);

	REQUIRE(dataRemoved == dataUTF16);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "BOM/UTF32")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF32,
							dataUTF32,
							NStringEncoding::UTF32BE,
							dataAdded,
							kNStringEncoderAddBOM);

	REQUIRE(dataAdded.GetSize() == (dataUTF32.GetSize() + 4));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF32BE), kBOM_UTF32BE)));


	NStringEncoder::Convert(NStringEncoding::UTF32BE,
							dataAdded,
							NStringEncoding::UTF32,
							dataRemoved,
							kNStringEncoderRemoveBOM);

	REQUIRE(dataRemoved == dataUTF32);


	NStringEncoder::Convert(NStringEncoding::UTF32,
							dataRemoved,
							NStringEncoding::UTF32LE,
							dataAdded,
							kNStringEncoderAddBOM);

	REQUIRE(dataAdded.GetSize() == (dataUTF32.GetSize() + 4));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF32LE), kBOM_UTF32LE)));


	NStringEncoder::Convert(NStringEncoding::UTF32LE,
							dataAdded,
							NStringEncoding::UTF32,
							dataRemoved,
							kNStringEncoderRemoveBOM);

	REQUIRE(dataRemoved == dataUTF32);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "Terminator/UTF8")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF8,
							dataUTF8,
							NStringEncoding::UTF8,
							dataAdded,
							kNStringEncoderAddTerminator);

	REQUIRE(dataAdded.GetSize() == (dataUTF8.GetSize() + 1));
	REQUIRE(dataAdded.EndsWith(NData(sizeof(kTerminator_UTF8), kTerminator_UTF8)));


	NStringEncoder::Convert(NStringEncoding::UTF8,
							dataAdded,
							NStringEncoding::UTF8,
							dataRemoved,
							kNStringEncoderRemoveTerminator);

	REQUIRE(dataRemoved == dataUTF8);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "Terminator/UTF16")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF16,
							dataUTF16,
							NStringEncoding::UTF16,
							dataAdded,
							kNStringEncoderAddTerminator);

	REQUIRE(dataAdded.GetSize() == (dataUTF16.GetSize() + 2));
	REQUIRE(dataAdded.EndsWith(NData(sizeof(kTerminator_UTF16), kTerminator_UTF16)));


	NStringEncoder::Convert(NStringEncoding::UTF16,
							dataAdded,
							NStringEncoding::UTF16,
							dataRemoved,
							kNStringEncoderRemoveTerminator);

	REQUIRE(dataRemoved == dataUTF16);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "Terminator/UTF32")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF32,
							dataUTF32,
							NStringEncoding::UTF32,
							dataAdded,
							kNStringEncoderAddTerminator);

	REQUIRE(dataAdded.GetSize() == (dataUTF32.GetSize() + 4));
	REQUIRE(dataAdded.EndsWith(NData(sizeof(kTerminator_UTF32), kTerminator_UTF32)));


	NStringEncoder::Convert(NStringEncoding::UTF32,
							dataAdded,
							NStringEncoding::UTF32,
							dataRemoved,
							kNStringEncoderRemoveTerminator);

	REQUIRE(dataRemoved == dataUTF32);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "BOMTerminator/UTF8")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF8,
							dataUTF8,
							NStringEncoding::UTF8,
							dataAdded,
							kNStringEncoderBOMTerminator);

	REQUIRE(dataAdded.GetSize() == (dataUTF8.GetSize() + 2));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF8), kBOM_UTF8)));
	REQUIRE(dataAdded.EndsWith(NData(sizeof(kTerminator_UTF8), kTerminator_UTF8)));


	NStringEncoder::Convert(NStringEncoding::UTF8,
							dataAdded,
							NStringEncoding::UTF8,
							dataRemoved,
							kNStringEncoderNone);

	REQUIRE(dataRemoved == dataUTF8);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "BOMTerminator/UTF16")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF16,
							dataUTF16,
							NStringEncoding::UTF16BE,
							dataAdded,
							kNStringEncoderBOMTerminator);

	REQUIRE(dataAdded.GetSize() == (dataUTF16.GetSize() + 4));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF16BE), kBOM_UTF16BE)));
	REQUIRE(dataAdded.EndsWith(NData(sizeof(kTerminator_UTF16), kTerminator_UTF16)));


	NStringEncoder::Convert(NStringEncoding::UTF16,
							dataAdded,
							NStringEncoding::UTF16,
							dataRemoved,
							kNStringEncoderNone);

	REQUIRE(dataRemoved == dataUTF16);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "BOMTerminator/UTF32")
{


	// Perform the test
	NData dataAdded, dataRemoved;

	NStringEncoder::Convert(NStringEncoding::UTF32,
							dataUTF16,
							NStringEncoding::UTF32BE,
							dataAdded,
							kNStringEncoderBOMTerminator);

	REQUIRE(dataAdded.GetSize() == (dataUTF32.GetSize() + 8));
	REQUIRE(dataAdded.StartsWith(NData(sizeof(kBOM_UTF32BE), kBOM_UTF32BE)));
	REQUIRE(dataAdded.EndsWith(NData(sizeof(kTerminator_UTF32), kTerminator_UTF32)));


	NStringEncoder::Convert(NStringEncoding::UTF32,
							dataAdded,
							NStringEncoding::UTF32,
							dataRemoved,
							kNStringEncoderNone);

	REQUIRE(dataRemoved == dataUTF32);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "GetNativeEncoding")
{


	// Perform the test
	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF16BE) == NStringEncoding::UTF16);
	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF16LE) == NStringEncoding::UTF16);

	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF32BE) == NStringEncoding::UTF32);
	REQUIRE(NStringEncoder::GetNativeEncoding(NStringEncoding::UTF32LE) == NStringEncoding::UTF32);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringEncoder, "GetCodeUnitSize")
{


	// Perform the test
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::UTF8) == sizeof(utf8_t));

	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::UTF16) == sizeof(utf16_t));
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::UTF16BE) == sizeof(utf16_t));
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::UTF16LE) == sizeof(utf16_t));

	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::UTF32) == sizeof(utf32_t));
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::UTF32BE) == sizeof(utf32_t));
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::UTF32LE) == sizeof(utf32_t));

	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::ASCII) == sizeof(char));
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::MacRoman) == sizeof(char));
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::ISOLatin1) == sizeof(char));
	REQUIRE(NStringEncoder::GetCodeUnitSize(NStringEncoding::WindowsLatin1) == sizeof(char));
}
