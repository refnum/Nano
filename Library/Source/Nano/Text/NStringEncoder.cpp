/*	NAME:
		NStringEncoder.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NStringEncoder.h"

// Nano
#include "NByteSwap.h"
#include "NData.h"
#include "NStdAlgorithm.h"
#include "NUnicodeView.h"
#include "NanoConstants.h"
#include "NanoTypes.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Misc
inline constexpr size_t  kStringMaxASCII                    = 0x7F;
inline constexpr size_t  kStringMaxLegacy                   = 0xFF;
inline constexpr utf32_t kStringInvalidLegacy               = 0xDFFF;

// BOM
inline constexpr uint8_t  kBOM_UTF8[]                       = {0xEF, 0xBB, 0xBF};
inline constexpr uint16_t kBOM_UTF16[]                      = {0xFEFF};
inline constexpr uint16_t kBOM_UTF16BE[]                    = {NByteSwap::SwapNtoB(kBOM_UTF16[0])};
inline constexpr uint16_t kBOM_UTF16LE[]                    = {NByteSwap::SwapNtoL(kBOM_UTF16[0])};
inline constexpr uint32_t kBOM_UTF32[]                      = {0x0000FEFF};
inline constexpr uint32_t kBOM_UTF32BE[]                    = {NByteSwap::SwapNtoB(kBOM_UTF32[0])};
inline constexpr uint32_t kBOM_UTF32LE[]                    = {NByteSwap::SwapNtoL(kBOM_UTF32[0])};


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
	0x0000, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF,
	0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF,
	0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0x0020, 0x0021, 0x0022, 0x0023,
	0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B,
	0x003C, 0x003D, 0x003E, 0x003F, 0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053,
	0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B,
	0x006C, 0x006D, 0x006E, 0x006F, 0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF,
	0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF,
	0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF,
	0xDFFF, 0xDFFF, 0xDFFF, 0xDFFF, 0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
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
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 0x20AC, 0xDFFF, 0x201A, 0x0192,
	0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0xDFFF, 0x017D, 0xDFFF,
	0xDFFF, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A,
	0x0153, 0xDFFF, 0x017E, 0x0178, 0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
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
void NStringEncoder::Convert(NStringEncoding    srcEncoding,
							 const NData&       srcData,
							 NStringEncoding    dstEncoding,
							 NData&             dstData,
							 NStringEncodeFlags theFlags)
{


	// Get the state we need
	NStringEncoding nativeSrcEncoding = GetNativeEncoding(srcEncoding);
	NStringEncoding nativeDstEncoding = GetNativeEncoding(dstEncoding);

	bool  swapInput  = ShouldSwap(srcEncoding);
	bool  swapOutput = ShouldSwap(dstEncoding);
	NData inputData  = srcData;



	// Convert the text
	ProcessInput(theFlags, swapInput, nativeSrcEncoding, inputData);

	ConvertText(nativeSrcEncoding, inputData, nativeDstEncoding, dstData);

	ProcessOutput(theFlags, swapOutput, nativeDstEncoding, dstData);
}





//=============================================================================
//		NStringEncoder::GetUnknownEncoding : Get an unknown encoding.
//-----------------------------------------------------------------------------
NStringEncoding NStringEncoder::GetUnknownEncoding(const NData& theData)
{


	// Get the state we need
	NStringEncoding theEncoding = NStringEncoding::Unknown;
	const uint8_t*  thePtr      = theData.GetData();
	size_t          theSize     = theData.GetSize();



	// Check for a BOM
	if (theEncoding == NStringEncoding::Unknown && theSize >= sizeof(kBOM_UTF8))
	{
		if (memcmp(thePtr, kBOM_UTF8, sizeof(kBOM_UTF8)) == 0)
		{
			theEncoding = NStringEncoding::UTF8;
		}
	}

	if (theEncoding == NStringEncoding::Unknown && theSize >= sizeof(kBOM_UTF16))
	{
		static_assert(sizeof(kBOM_UTF16BE) == sizeof(kBOM_UTF16));
		static_assert(sizeof(kBOM_UTF16LE) == sizeof(kBOM_UTF16));

		if (memcmp(thePtr, kBOM_UTF16, sizeof(kBOM_UTF16)) == 0)
		{
			theEncoding = NStringEncoding::UTF16;
		}
		else if (memcmp(thePtr, kBOM_UTF16BE, sizeof(kBOM_UTF16BE)) == 0)
		{
			theEncoding = NStringEncoding::UTF16BE;
		}
		else if (memcmp(thePtr, kBOM_UTF16LE, sizeof(kBOM_UTF16LE)) == 0)
		{
			theEncoding = NStringEncoding::UTF16LE;
		}
	}

	if (theEncoding == NStringEncoding::Unknown && theSize >= sizeof(kBOM_UTF32))
	{
		static_assert(sizeof(kBOM_UTF32BE) == sizeof(kBOM_UTF32));
		static_assert(sizeof(kBOM_UTF32LE) == sizeof(kBOM_UTF32));

		if (memcmp(thePtr, kBOM_UTF32, sizeof(kBOM_UTF32)) == 0)
		{
			theEncoding = NStringEncoding::UTF32;
		}
		else if (memcmp(thePtr, kBOM_UTF32BE, sizeof(kBOM_UTF32BE)) == 0)
		{
			theEncoding = NStringEncoding::UTF32BE;
		}
		else if (memcmp(thePtr, kBOM_UTF32LE, sizeof(kBOM_UTF32LE)) == 0)
		{
			theEncoding = NStringEncoding::UTF32LE;
		}
	}



	// Check the content
	//
	// For now assume any non-BOM content is UTF8.
	if (theEncoding == NStringEncoding::Unknown && theSize != 0)
	{
		theEncoding = NStringEncoding::UTF8;
	}

	return theEncoding;
}





#pragma mark privae
//=============================================================================
//		NStringEncoder:::ProcessInput : Process the input.
//-----------------------------------------------------------------------------
void NStringEncoder::ProcessInput(NStringEncodeFlags theFlags,
								  bool               swapInput,
								  NStringEncoding    srcEncoding,
								  NData&             srcData)
{


	// Process the input
	if (swapInput)
	{
		SwapUTF(srcEncoding, srcData);
	}

	if (theFlags & NStringEncode::RemoveBOM)
	{
		RemoveBOM(srcEncoding, srcData);
	}

	if (theFlags & NStringEncode::RemoveTerminator)
	{
		RemoveTerminator(srcEncoding, srcData);
	}
}





//=============================================================================
//		NStringEncoder:::ProcessOutput : Process the output data.
//-----------------------------------------------------------------------------
void NStringEncoder::ProcessOutput(NStringEncodeFlags theFlags,
								   bool               swapOutput,
								   NStringEncoding    dstEncoding,
								   NData&             dstData)
{


	// Process the output
	if (theFlags & NStringEncode::AddBOM)
	{
		AddBOM(dstEncoding, dstData);
	}

	if (theFlags & NStringEncode::AddTerminator)
	{
		AddTerminator(dstEncoding, dstData);
	}

	if (swapOutput)
	{
		SwapUTF(dstEncoding, dstData);
	}
}





//=============================================================================
//		NStringEncoder:::AddBOM : Add the BOM.
//-----------------------------------------------------------------------------
void NStringEncoder::AddBOM(NStringEncoding theEncoding, NData& theData)
{


	// Remove the BOM
	switch (theEncoding)
	{
		case NStringEncoding::UTF8:
			theData.Insert(0, sizeof(kBOM_UTF8), kBOM_UTF8);
			break;

		case NStringEncoding::UTF16:
			theData.Insert(0, sizeof(kBOM_UTF16), kBOM_UTF16);
			break;

		case NStringEncoding::UTF32:
			theData.Insert(0, sizeof(kBOM_UTF32), kBOM_UTF32);
			break;

		default:
			break;
	}
}





//=============================================================================
//		NStringEncoder:::RemoveBOM : Remove the BOM.
//-----------------------------------------------------------------------------
void NStringEncoder::RemoveBOM(NStringEncoding theEncoding, NData& theData)
{


	// Remove the BOM
	switch (theEncoding)
	{
		case NStringEncoding::UTF8:
			if (theData.StartsWith(NData(sizeof(kBOM_UTF8), kBOM_UTF8)))
			{
				theData.Remove(NRange(0, sizeof(kBOM_UTF8)));
			}
			break;

		case NStringEncoding::UTF16:
			if (theData.StartsWith(NData(sizeof(kBOM_UTF16), kBOM_UTF16)))
			{
				theData.Remove(NRange(0, sizeof(kBOM_UTF16)));
			}
			break;

		case NStringEncoding::UTF32:
			if (theData.StartsWith(NData(sizeof(kBOM_UTF32), kBOM_UTF32)))
			{
				theData.Remove(NRange(0, sizeof(kBOM_UTF32)));
			}
			break;

		default:
			break;
	}
}





//=============================================================================
//		NStringEncoder::AddTerminator : Add the terminator.
//-----------------------------------------------------------------------------
void NStringEncoder::AddTerminator(NStringEncoding theEncoding, NData& theData)
{


	// Add the terminator
	theData.SetSize(theData.GetSize() + GetCodeUnitSize(theEncoding));
}





//=============================================================================
//		NStringEncoder:::RemoveTerminator : Remove the terminator.
//-----------------------------------------------------------------------------
void NStringEncoder::RemoveTerminator(NStringEncoding theEncoding, NData& theData)
{


	// Remove the terminator
	NData theTerminator(GetCodeUnitSize(theEncoding), nullptr, NDataSource::Zero);

	if (theData.EndsWith(theTerminator))
	{
		theData.SetSize(theData.GetSize() - theTerminator.GetSize());
	}
}





//=============================================================================
//		NStringEncoder::ShouldSwap : Should we endian-swap the data?
//-----------------------------------------------------------------------------
bool NStringEncoder::ShouldSwap(NStringEncoding theEncoding)
{


	// Check the encoding
	bool shouldSwap = false;

	switch (theEncoding)
	{
		case NStringEncoding::UTF16BE:
		case NStringEncoding::UTF32BE:
			shouldSwap = NN_ENDIAN_LITTLE;
			break;

		case NStringEncoding::UTF16LE:
		case NStringEncoding::UTF32LE:
			shouldSwap = NN_ENDIAN_BIG;
			break;

		default:
			break;
	}

	return shouldSwap;
}





//=============================================================================
//		NStringEncoder::SwapUTF : Endian-swap UTF data.
//-----------------------------------------------------------------------------
void NStringEncoder::SwapUTF(NStringEncoding theEncoding, NData& theData)
{


	// Get the state we need
	size_t codeUnitSize = GetCodeUnitSize(theEncoding);



	// Swap the data
	if (codeUnitSize != 1)
	{
		NN_REQUIRE((theData.GetSize() % codeUnitSize) == 0);
		size_t numElements = theData.GetSize() / codeUnitSize;

		NByteSwap::Swap(numElements, codeUnitSize, theData.GetMutableData());
	}
}





//=============================================================================
//		NStringEncoder:::ConvertText : Convert the text.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertText(NStringEncoding srcEncoding,
								 const NData&    srcData,
								 NStringEncoding dstEncoding,
								 NData&          dstData)
{


	// Convert the text
	switch (srcEncoding)
	{
		case NStringEncoding::UTF8:
			ConvertFromUTF8(srcData, dstEncoding, dstData);
			break;

		case NStringEncoding::UTF16:
			ConvertFromUTF16(srcData, dstEncoding, dstData);
			break;

		case NStringEncoding::UTF32:
			ConvertFromUTF32(srcData, dstEncoding, dstData);
			break;

		case NStringEncoding::ASCII:
		case NStringEncoding::MacRoman:
		case NStringEncoding::ISOLatin1:
		case NStringEncoding::WindowsLatin1:
			ConvertFromLegacy(srcEncoding, srcData, dstEncoding, dstData);
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown encoding!");
			break;
	}
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
//		NStringEncoder::ConvertFromLegacy : Convert from a legacy encoding.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertFromLegacy(NStringEncoding srcEncoding,
									   const NData&    srcData,
									   NStringEncoding dstEncoding,
									   NData&          dstData)
{


	// No-op conversions
	if ((srcEncoding == dstEncoding) ||
		(srcEncoding == NStringEncoding::ASCII && dstEncoding == NStringEncoding::UTF8))
	{
		dstData = srcData;
	}



	// Convert via UTF32
	else
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

			theResult.push_back(codePoint);
		}



		// Convert to the final encoding
		NData dataUTF32(theResult.size() * sizeof(utf32_t), theResult.data());

		ConvertFromUTF32(dataUTF32, dstEncoding, dstData);
	}
}





//=============================================================================
//		NStringEncoder::ConvertToUTF8 : Convert to UTF8.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertToUTF8(NUnicodeView& srcView, NData& dstData)
{


	// Get the state we need
	NVectorUTF8 theResult;

	theResult.reserve(srcView.GetMaxSize());



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

	theResult.reserve(srcView.GetMaxSize());



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

			utf16_t pairHi = kNUTF16SurrogateHiStart + ((codePoint >> 10) & 0b1111111111);
			utf16_t pairLo = kNUTF16SurrogateLoStart + ((codePoint >> 0) & 0b1111111111);

			theResult.push_back(pairHi);
			theResult.push_back(pairLo);
		}
		else
		{
			NN_EXPECT(codePoint <= 0x10FFFF);
		}
	}



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

	theResult.reserve(srcView.GetMaxSize());



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



	// Trim the result
	dstData.SetData(theResult.size() * sizeof(utf32_t), theResult.data());
}





//=============================================================================
//		NStringEncoder::ConvertToLegacy : Convert to a legacy encoding.
//-----------------------------------------------------------------------------
void NStringEncoder::ConvertToLegacy(NUnicodeView&   srcView,
									 NStringEncoding dstEncoding,
									 NData&          dstData)
{


	// Get the state we need
	const NMapUTF32Legacy* srcTable = GetLegacyFromUTF32(dstEncoding);

	NVectorUInt8 theResult;

	theResult.reserve(srcView.GetMaxSize());



	// Convert to the encoding
	for (utf32_t codePoint : srcView)
	{
		theResult.push_back(nstd::fetch(*srcTable, codePoint, kNASCIIReplacement));
	}



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
const NMapUTF32Legacy* NStringEncoder::GetLegacyFromUTF32(NStringEncoding dstEncoding)
{
	static NMapUTF32Legacy sUTF32_to_ASCII = GetLegacyMap(kStringMaxASCII, kASCII_to_UTF32);

	static NMapUTF32Legacy sUTF32_to_MacRoman = GetLegacyMap(kStringMaxLegacy, kMacRoman_to_UTF32);

	static NMapUTF32Legacy sUTF32_to_ISOLatin1 =
		GetLegacyMap(kStringMaxLegacy, kISOLatin1_to_UTF32);

	static NMapUTF32Legacy sUTF32_to_WindowsLatin1 =
		GetLegacyMap(kStringMaxLegacy, kWindowsLatin1_to_UTF32);



	// Get the table
	const NMapUTF32Legacy* theTable = nullptr;

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
NMapUTF32Legacy NStringEncoder::GetLegacyMap(size_t maxChar, const utf32_t* toUTF32)
{


	// Populate the table
	NMapUTF32Legacy fromUTF32;

	for (size_t n = 0; n <= maxChar; n++)
	{
		utf32_t codePoint = toUTF32[n];
		if (codePoint != kStringInvalidLegacy)
		{
			fromUTF32[codePoint] = uint8_t(n);
		}
	}

	return fromUTF32;
}
