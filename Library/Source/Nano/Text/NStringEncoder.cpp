/*	NAME:
		NStringEncoder.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NStringEncoder.h"

// Nano
#include "NByteSwap.h"
#include "NData.h"
#include "NUnicodeView.h"
#include "NanoConstants.h"
#include "NanoTypes.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Misc
static constexpr size_t kStringMaxASCII                     = 0x7F;
static constexpr size_t kStringMaxLegacy                    = 0xFF;

// BOM
static constexpr uint8_t kBOM_UTF8[]                        = {0xEF, 0xBB, 0xBF};
static constexpr uint8_t kBOM_UTF16BE[]                     = {0xFE, 0xFF};
static constexpr uint8_t kBOM_UTF16LE[]                     = {0xFF, 0xFE};
static constexpr uint8_t kBOM_UTF32BE[]                     = {0x00, 0x00, 0xFE, 0xFF};
static constexpr uint8_t kBOM_UTF32LE[]                     = {0xFF, 0xFE, 0x00, 0x00};


// Legacy encodings
static constexpr utf32_t kASCII_to_UTF32[256]               = {
	// http://en.wikipedia.org/wiki/ASCII
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B,
	0x000C, 0x000D, 0x000E, 0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
	0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023,
	0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B,
	0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053,
	0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B,
	0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
	0x003F, 0x003F, 0x003F, 0x003F};

static constexpr utf32_t kMacRoman_to_UTF32[256]            = {
	// http://en.wikipedia.org/wiki/Mac_OS_Roman
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B,
	0x000C, 0x000D, 0x000E, 0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
	0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023,
	0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B,
	0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053,
	0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B,
	0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 0x00C4, 0x00C5, 0x00C7, 0x00C9,
	0x00D1, 0x00D6, 0x00DC, 0x00E1, 0x00E0, 0x00E2, 0x00E4, 0x00E3, 0x00E5, 0x00E7, 0x00E9, 0x00E8,
	0x00EA, 0x00EB, 0x00ED, 0x00EC, 0x00EE, 0x00EF, 0x00F1, 0x00F3, 0x00F2, 0x00F4, 0x00F6, 0x00F5,
	0x00FA, 0x00F9, 0x00FB, 0x00FC, 0x2020, 0x00B0, 0x00A2, 0x00A3, 0x00A7, 0x2022, 0x00B6, 0x00DF,
	0x00AE, 0x00A9, 0x2122, 0x00B4, 0x00A8, 0x2260, 0x00C6, 0x00D8, 0x221E, 0x00B1, 0x2264, 0x2265,
	0x00A5, 0x00B5, 0x2202, 0x2211, 0x220F, 0x03C0, 0x222B, 0x00AA, 0x00BA, 0x03A9, 0x00E6, 0x00F8,
	0x00BF, 0x00A1, 0x00AC, 0x221A, 0x0192, 0x2248, 0x2206, 0x00AB, 0x00BB, 0x2026, 0x00A0, 0x00C0,
	0x00C3, 0x00D5, 0x0152, 0x0153, 0x2013, 0x2014, 0x201C, 0x201D, 0x2018, 0x2019, 0x00F7, 0x25CA,
	0x00FF, 0x0178, 0x2044, 0x20AC, 0x2039, 0x203A, 0xFB01, 0xFB02, 0x2021, 0x00B7, 0x201A, 0x201E,
	0x2030, 0x00C2, 0x00CA, 0x00C1, 0x00CB, 0x00C8, 0x00CD, 0x00CE, 0x00CF, 0x00CC, 0x00D3, 0x00D4,
	0xF8FF, 0x00D2, 0x00DA, 0x00DB, 0x00D9, 0x0131, 0x02C6, 0x02DC, 0x00AF, 0x02D8, 0x02D9, 0x02DA,
	0x00B8, 0x02DD, 0x02DB, 0x02C7};

static constexpr utf32_t kISOLatin1_to_UTF32[256]           = {
	// http://en.wikipedia.org/wiki/ISO/IEC_8859-1
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0021, 0x0022, 0x0023,
	0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B,
	0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053,
	0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B,
	0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
	0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3,
	0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB,
	0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
	0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF, 0x00E0, 0x00E1, 0x00E2, 0x00E3,
	0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB,
	0x00FC, 0x00FD, 0x00FE, 0x00FF};

static constexpr utf32_t kWindowsLatin1_to_UTF32[256]       = {
	// http://en.wikipedia.org/wiki/Windows-1252
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B,
	0x000C, 0x000D, 0x000E, 0x000F, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
	0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F, 0x0020, 0x0021, 0x0022, 0x0023,
	0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B,
	0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053,
	0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B,
	0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 0x20AC, 0x0000, 0x201A, 0x0192,
	0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0000, 0x017D, 0x0000,
	0x0000, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A,
	0x0153, 0x0000, 0x017E, 0x0178, 0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
	0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF, 0x00B0, 0x00B1, 0x00B2, 0x00B3,
	0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB,
	0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7,
	0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF, 0x00E0, 0x00E1, 0x00E2, 0x00E3,
	0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
	0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB,
	0x00FC, 0x00FD, 0x00FE, 0x00FF};





//=============================================================================
//		NStringEncoder::Convert : Convert text.
//-----------------------------------------------------------------------------
void NStringEncoder::Convert(NStringEncoding srcEncoding,
							 const NData&    srcData,
							 NStringEncoding dstEncoding,
							 NData&          dstData)
{


	// Get the state we need
	NStringEncoding inputEncoding  = GetNativeEncoding(srcEncoding);
	NStringEncoding outputEncoding = GetNativeEncoding(dstEncoding);
	NData           inputData      = srcData;



	// Convert the text
	ProcessInput(srcEncoding, inputData);

	switch (inputEncoding)
	{
		case NStringEncoding::UTF8:
			ConvertFromUTF8(inputData, outputEncoding, dstData);
			break;

		case NStringEncoding::UTF16:
			ConvertFromUTF16(inputData, outputEncoding, dstData);
			break;

		case NStringEncoding::UTF32:
			ConvertFromUTF32(inputData, outputEncoding, dstData);
			break;

		case NStringEncoding::ASCII:
		case NStringEncoding::MacRoman:
		case NStringEncoding::ISOLatin1:
		case NStringEncoding::WindowsLatin1:
			ConvertFromLegacy(inputEncoding, inputData, outputEncoding, dstData);
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown encoding!");
			break;
	}

	ProcessOutput(dstEncoding, dstData);
}





//=============================================================================
//		NStringEncoder::GetNativeEncoding : Get a native encoding.
//-----------------------------------------------------------------------------
NStringEncoding NStringEncoder::GetNativeEncoding(NStringEncoding theEncoding)
{


	// Get the native encoding
	switch (theEncoding)
	{
		case NStringEncoding::UTF16BE:
		case NStringEncoding::UTF16LE:
			theEncoding = NStringEncoding::UTF16;
			break;

		case NStringEncoding::UTF32BE:
		case NStringEncoding::UTF32LE:
			theEncoding = NStringEncoding::UTF32;
			break;

		default:
			break;
	}

	return theEncoding;
}





#pragma mark privae
//=============================================================================
//		NStringEncoder:::ProcessInput : Process the input data.
//-----------------------------------------------------------------------------
void NStringEncoder::ProcessInput(NStringEncoding theEncoding, NData& theData)
{


	// Prepare the data
	RemoveBOM(theData);
	RemoveNull(theEncoding, theData);

	SwapUTF(theEncoding, theData);
}





//=============================================================================
//		NStringEncoder:::ProcessOutput : Process the output data.
//-----------------------------------------------------------------------------
void NStringEncoder::ProcessOutput(NStringEncoding theEncoding, NData& theData)
{


	// Prepare the data
	SwapUTF(theEncoding, theData);
}





//=============================================================================
//		NStringEncoder:::RemoveBOM : Remove a BOM.
//-----------------------------------------------------------------------------
void NStringEncoder::RemoveBOM(NData& theData)
{


	// Remove any BOM
#define REMOVE_BOM(_bom)                                    \
	do                                                      \
	{                                                       \
		if (theData.StartsWith(NData(sizeof(_bom), _bom)))  \
		{                                                   \
			theData.RemoveData(NRange(0, sizeof(_bom)));    \
			return;                                         \
		}                                                   \
	} while (0)

	REMOVE_BOM(kBOM_UTF8);
	REMOVE_BOM(kBOM_UTF16BE);
	REMOVE_BOM(kBOM_UTF16LE);
	REMOVE_BOM(kBOM_UTF32BE);
	REMOVE_BOM(kBOM_UTF32LE);

#undef REMOVE_BOM
}





//=============================================================================
//		NStringEncoder::RemoveNull : Remove a null terminator.
//-----------------------------------------------------------------------------
void NStringEncoder::RemoveNull(NStringEncoding theEncoding, NData& theData)
{


	// Get the state we need
	size_t nullSize = GetElementSize(theEncoding);
	NData  nullBytes(nullSize, nullptr, NDataSource::Zero);

	if (theData.EndsWith(nullBytes))
	{
		theData.RemoveData(NRange(theData.GetSize() - nullSize, nullSize));
	}
}





//=============================================================================
//		NStringEncoder::SwapUTF : Endian-swap UTF data.
//-----------------------------------------------------------------------------
void NStringEncoder::SwapUTF(NStringEncoding theEncoding, NData& theData)
{


	// Get the state we need
	NStringEncoding nativeEncoding = GetNativeEncoding(theEncoding);
	size_t          elementSize    = GetElementSize(theEncoding);



	// Swap the data
	if (elementSize != 1 && theEncoding != nativeEncoding)
	{
		size_t numElements = theData.GetSize() / elementSize;
		NByteSwap::SwapN(numElements, elementSize, theData.GetMutableData());
	}
}





//=============================================================================
//		NStringEncoder::GetElementSize : Get the size of an encoding element.
//-----------------------------------------------------------------------------
size_t NStringEncoder::GetElementSize(NStringEncoding theEncoding)
{


	// Get the size
	size_t theSize = 0;

	switch (theEncoding)
	{
		case NStringEncoding::Unknown:
			theSize = 0;
			break;

		case NStringEncoding::UTF8:
		case NStringEncoding::ASCII:
		case NStringEncoding::MacRoman:
		case NStringEncoding::ISOLatin1:
		case NStringEncoding::WindowsLatin1:
			theSize = sizeof(utf8_t);
			break;

		case NStringEncoding::UTF16:
		case NStringEncoding::UTF16BE:
		case NStringEncoding::UTF16LE:
			theSize = sizeof(utf16_t);
			break;

		case NStringEncoding::UTF32:
		case NStringEncoding::UTF32BE:
		case NStringEncoding::UTF32LE:
			theSize = sizeof(utf32_t);
			break;
	}

	return theSize;
}





//=============================================================================
//		NStringEncoder::ConvertFromUTF8 : Convert a UTF8 string.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertFromUTF8(const NData&    srcData,
									 NStringEncoding dstEncoding,
									 NData&          dstData)
{


	// Convert the string
	NUnicodeView srcView(NStringEncoding::UTF8, srcData.GetSize(), srcData.GetData());

	switch (dstEncoding)
	{
		case NStringEncoding::UTF8:
			dstData = srcData;
			break;

		case NStringEncoding::UTF16:
			ConvertToUTF16(srcView, dstData);
			break;

		case NStringEncoding::UTF32:
			ConvertToUTF32(srcView, dstData);
			break;

		case NStringEncoding::ASCII:
		case NStringEncoding::MacRoman:
		case NStringEncoding::ISOLatin1:
		case NStringEncoding::WindowsLatin1:
			ConvertToLegacy(srcView, dstEncoding, dstData);
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown encoding!");
			break;
	}
}





//=============================================================================
//		NStringEncoder::ConvertFromUTF16 : Convert a UTF16 string.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertFromUTF16(const NData&    srcData,
									  NStringEncoding dstEncoding,
									  NData&          dstData)
{


	// Convert the string
	NUnicodeView srcView(NStringEncoding::UTF16, srcData.GetSize(), srcData.GetData());

	switch (dstEncoding)
	{
		case NStringEncoding::UTF8:
			ConvertToUTF8(srcView, dstData);
			break;

		case NStringEncoding::UTF16:
			dstData = srcData;
			break;

		case NStringEncoding::UTF32:
			ConvertToUTF32(srcView, dstData);
			break;

		case NStringEncoding::ASCII:
		case NStringEncoding::MacRoman:
		case NStringEncoding::ISOLatin1:
		case NStringEncoding::WindowsLatin1:
			ConvertToLegacy(srcView, dstEncoding, dstData);
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown encoding!");
			break;
	}
}





//=============================================================================
//		NStringEncoder::ConvertFromUTF32 : Convert a UTF32 string.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertFromUTF32(const NData&    srcData,
									  NStringEncoding dstEncoding,
									  NData&          dstData)
{


	// Convert the string
	NUnicodeView srcView(NStringEncoding::UTF32, srcData.GetSize(), srcData.GetData());

	switch (dstEncoding)
	{
		case NStringEncoding::UTF8:
			ConvertToUTF8(srcView, dstData);
			break;

		case NStringEncoding::UTF16:
			ConvertToUTF16(srcView, dstData);
			break;

		case NStringEncoding::UTF32:
			dstData = srcData;
			break;

		case NStringEncoding::ASCII:
		case NStringEncoding::MacRoman:
		case NStringEncoding::ISOLatin1:
		case NStringEncoding::WindowsLatin1:
			ConvertToLegacy(srcView, dstEncoding, dstData);
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown encoding!");
			break;
	}
}





//=============================================================================
//		NStringEncoder::ConvertToUTF8 : Convert to UTF8.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertToUTF8(NUnicodeView& srcView, NData& dstData)
{


	// Get the state we need
	NVectorUTF8 theResult;

	theResult.reserve(srcView.GetMaxSize() * sizeof(utf8_t));



	// Convert to UTF8
	for (utf32_t codePoint : srcView)
	{
		if (codePoint <= 0x7F)
		{
			utf8_t byte0 = utf8_t((codePoint >> 0) & 0b01111111);

			theResult.push_back(byte0);
		}

		else if (codePoint <= 0x7FF)
		{
			utf8_t byte0 = utf8_t((codePoint >> 6) & 0b00011111);
			utf8_t byte1 = utf8_t((codePoint >> 0) & 0b00111111);

			theResult.push_back(byte0);
			theResult.push_back(byte1);
		}

		else if (codePoint <= 0xFFFF)
		{
			utf8_t byte0 = utf8_t((codePoint >> 12) & 0b00001111);
			utf8_t byte1 = utf8_t((codePoint >> 6) & 0b00111111);
			utf8_t byte2 = utf8_t((codePoint >> 0) & 0b00111111);

			theResult.push_back(byte0);
			theResult.push_back(byte1);
			theResult.push_back(byte2);
		}

		else if (codePoint <= 0x10FFFF)
		{
			utf8_t byte0 = utf8_t((codePoint >> 18) & 0b00000111);
			utf8_t byte1 = utf8_t((codePoint >> 12) & 0b00111111);
			utf8_t byte2 = utf8_t((codePoint >> 6) & 0b00111111);
			utf8_t byte3 = utf8_t((codePoint >> 0) & 0b00111111);

			theResult.push_back(byte0);
			theResult.push_back(byte1);
			theResult.push_back(byte2);
			theResult.push_back(byte3);
		}

		else
		{
			NN_EXPECT(codePoint <= 0x10FFFF);
		}
	}

	theResult.push_back(0);



	// Trim the result
	dstData.SetData(theResult.size() * sizeof(utf8_t), theResult.data());
}





//=============================================================================
//		NStringEncoder::ConvertToUTF16 : Convert to UTF16.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertToUTF16(NUnicodeView& srcView, NData& dstData)
{


	// Get the state we need
	NVectorUTF16 theResult;

	theResult.reserve(srcView.GetMaxSize() * sizeof(utf16_t));



	// Convert to UTF16
	for (utf32_t codePoint : srcView)
	{
		if (codePoint <= 0xFFFF)
		{
			theResult.push_back(utf16_t(codePoint));
		}

		else if (codePoint <= 0x10FFFF)
		{
			codePoint -= 0x10000;

			utf16_t pairHi = 0xD800 + ((codePoint >> 10) & 0b1111111111);
			utf16_t pairLo = 0xDC00 + ((codePoint >> 0) & 0b1111111111);

			theResult.push_back(pairHi);
			theResult.push_back(pairLo);
		}
		else
		{
			NN_EXPECT(codePoint <= 0x10FFFF);
		}
	}

	theResult.push_back(0);



	// Trim the result
	dstData.SetData(theResult.size() * sizeof(utf16_t), theResult.data());
}





//=============================================================================
//		NStringEncoder::ConvertToUTF32 : Convert to UTF32.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertToUTF32(NUnicodeView& srcView, NData& dstData)
{


	// Get the state we need
	NVectorUTF32 theResult;

	theResult.reserve(srcView.GetMaxSize() * sizeof(utf32_t));



	// Convert to UTF32
	for (utf32_t codePoint : srcView)
	{
		if (codePoint <= 0x10FFFF)
		{
			theResult.push_back(codePoint);
		}
		else
		{
			NN_EXPECT(codePoint <= 0x10FFFF);
		}
	}

	theResult.push_back(0);



	// Trim the result
	dstData.SetData(theResult.size() * sizeof(utf32_t), theResult.data());
}





//=============================================================================
//		NStringEncoder::ConvertFromLegacy : Convert from a legacy encoding.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertFromLegacy(NStringEncoding srcEncoding,
									   const NData&    srcData,
									   NStringEncoding dstEncoding,
									   NData&          dstData)
{


	// Get the state we need
	size_t         numChars = srcData.GetSize() / sizeof(uint8_t);
	const uint8_t* srcChars = srcData.GetData();
	const utf32_t* srcTable = GetLegacyToUTF32(srcEncoding);

	NVectorUTF32 theResult;

	theResult.reserve(numChars * sizeof(utf32_t));



	// Convert to UTF32
	for (size_t n = 0; n < numChars; n++)
	{
		utf32_t codePoint = srcTable[srcChars[n]];
		if (codePoint == 0x0000)
		{
			codePoint = kNUTF32Replacement;
		}

		theResult.push_back(codePoint);
	}

	theResult.push_back(0);



	// Convert to the final encoding
	NData dataUTF32(theResult.size() * sizeof(utf32_t), theResult.data());

	return Convert(NStringEncoding::UTF32, dataUTF32, dstEncoding, dstData);
}





//=============================================================================
//		NStringEncoder::ConvertToLegacy : Convert to a legacy encoding.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertToLegacy(NUnicodeView&   srcView,
									 NStringEncoding dstEncoding,
									 NData&          dstData)
{


	// Get the state we need
	const NUTF32LegacyMap* srcTable = GetLegacyFromUTF32(dstEncoding);

	NVectorUInt8 theResult;

	theResult.reserve(srcView.GetMaxSize() * sizeof(char));



	// Convert to the encoding
	for (utf32_t codePoint : srcView)
	{
		// dair, use nstd::fetch?
		auto theIter = srcTable->find(codePoint);

		if (theIter != srcTable->end())
		{
			theResult.push_back(theIter->second);
		}
		else
		{
			theResult.push_back(kNASCIIReplacement);
		}
	}

	theResult.push_back(0);



	// Trim the result
	dstData.SetData(theResult.size() * sizeof(uint8_t), theResult.data());
}





//=============================================================================
//		NStringEncoder::GetLegacyToUTF32 : Get a legacy to UTF32 table.
//-----------------------------------------------------------------------------
const utf32_t* NStringEncoder::GetLegacyToUTF32(NStringEncoding srcEncoding)
{


	// Get the table
	const utf32_t* theTable = nullptr;

	switch (srcEncoding)
	{
		case NStringEncoding::ASCII:
			theTable = kASCII_to_UTF32;
			break;

		case NStringEncoding::MacRoman:
			theTable = kMacRoman_to_UTF32;
			break;

		case NStringEncoding::ISOLatin1:
			theTable = kISOLatin1_to_UTF32;
			break;

		case NStringEncoding::WindowsLatin1:
			theTable = kWindowsLatin1_to_UTF32;
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown legacy encoding!");
			break;
	}

	return theTable;
}





//=============================================================================
//		NStringEncoder::GetLegacyFromUTF32 : Get a UTF32 to legacy table.
//-----------------------------------------------------------------------------
const NUTF32LegacyMap* NStringEncoder::GetLegacyFromUTF32(NStringEncoding dstEncoding)
{
	static NUTF32LegacyMap sUTF32_to_ASCII    = GetLegacyMap(kStringMaxASCII, kASCII_to_UTF32);
	static NUTF32LegacyMap sUTF32_to_MacRoman = GetLegacyMap(kStringMaxLegacy, kMacRoman_to_UTF32);
	static NUTF32LegacyMap sUTF32_to_ISOLatin1 =
		GetLegacyMap(kStringMaxLegacy, kISOLatin1_to_UTF32);
	static NUTF32LegacyMap sUTF32_to_WindowsLatin1 =
		GetLegacyMap(kStringMaxLegacy, kWindowsLatin1_to_UTF32);



	// Get the table
	const NUTF32LegacyMap* theTable = nullptr;

	switch (dstEncoding)
	{
		case NStringEncoding::ASCII:
			theTable = &sUTF32_to_ASCII;
			break;

		case NStringEncoding::MacRoman:
			theTable = &sUTF32_to_MacRoman;
			break;

		case NStringEncoding::ISOLatin1:
			theTable = &sUTF32_to_ISOLatin1;
			break;

		case NStringEncoding::WindowsLatin1:
			theTable = &sUTF32_to_WindowsLatin1;
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown legacy encoding!");
			break;
	}

	return theTable;
}





//=============================================================================
//		NStringEncoder::GetLegacyMap : Get a UTF32->legacy map.
//-----------------------------------------------------------------------------
NUTF32LegacyMap NStringEncoder::GetLegacyMap(size_t maxChar, const utf32_t* toUTF32)
{


	// Populate the table
	NUTF32LegacyMap fromUTF32;

	for (size_t n = 0; n <= maxChar; n++)
	{
		fromUTF32[toUTF32[n]] = uint8_t(n);
	}

	return fromUTF32;
}
