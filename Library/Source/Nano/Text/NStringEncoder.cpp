/*	NAME:
		NStringEncoder.cpp

	DESCRIPTION:
		String encoder.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ConvertUTF.h"

#include "NUnicodeParser.h"
#include "NStringEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const uint64_t kStringTerminator									= 0;
static const char     kStringInvalidChar								= '?';


// Legacy encodings
static const utf32_t kASCII_to_UTF32[256]								= {
								// http://en.wikipedia.org/wiki/ASCII
								0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 
								0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F, 
								0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, 
								0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, 
								0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 
								0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F, 
								0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 
								0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F,
								0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F, 0x003F };

static const utf32_t kMacRoman_to_UTF32[256]							= {
								// http://en.wikipedia.org/wiki/Mac_OS_Roman
								0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, 
								0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F, 
								0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F, 
								0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F, 
								0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 
								0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F, 
								0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 
								0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F, 
								0x00C4, 0x00C5, 0x00C7, 0x00C9, 0x00D1, 0x00D6, 0x00DC, 0x00E1, 0x00E0, 0x00E2, 0x00E4, 0x00E3, 0x00E5, 0x00E7, 0x00E9, 0x00E8, 
								0x00EA, 0x00EB, 0x00ED, 0x00EC, 0x00EE, 0x00EF, 0x00F1, 0x00F3, 0x00F2, 0x00F4, 0x00F6, 0x00F5, 0x00FA, 0x00F9, 0x00FB, 0x00FC, 
								0x2020, 0x00B0, 0x00A2, 0x00A3, 0x00A7, 0x2022, 0x00B6, 0x00DF, 0x00AE, 0x00A9, 0x2122, 0x00B4, 0x00A8, 0x2260, 0x00C6, 0x00D8, 
								0x221E, 0x00B1, 0x2264, 0x2265, 0x00A5, 0x00B5, 0x2202, 0x2211, 0x220F, 0x03C0, 0x222B, 0x00AA, 0x00BA, 0x03A9, 0x00E6, 0x00F8, 
								0x00BF, 0x00A1, 0x00AC, 0x221A, 0x0192, 0x2248, 0x2206, 0x00AB, 0x00BB, 0x2026, 0x00A0, 0x00C0, 0x00C3, 0x00D5, 0x0152, 0x0153, 
								0x2013, 0x2014, 0x201C, 0x201D, 0x2018, 0x2019, 0x00F7, 0x25CA, 0x00FF, 0x0178, 0x2044, 0x20AC, 0x2039, 0x203A, 0xFB01, 0xFB02, 
								0x2021, 0x00B7, 0x201A, 0x201E, 0x2030, 0x00C2, 0x00CA, 0x00C1, 0x00CB, 0x00C8, 0x00CD, 0x00CE, 0x00CF, 0x00CC, 0x00D3, 0x00D4, 
								0xF8FF, 0x00D2, 0x00DA, 0x00DB, 0x00D9, 0x0131, 0x02C6, 0x02DC, 0x00AF, 0x02D8, 0x02D9, 0x02DA, 0x00B8, 0x02DD, 0x02DB, 0x02C7 };

static const utf32_t kISOLatin1_to_UTF32[256]							= {
								// http://en.wikipedia.org/wiki/ISO/IEC_8859-1
								0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
								0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
								0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
								0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
								0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
								0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
								0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
								0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x0000,
								0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
								0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
								0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
								0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
								0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
								0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
								0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
								0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF };

static const utf32_t kWindowsLatin1_to_UTF32[256]						= {
								// http://en.wikipedia.org/wiki/Windows-1252
								0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
								0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
								0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
								0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
								0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
								0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
								0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
								0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
								0x20AC, 0x0000, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x0000, 0x017D, 0x0000,
								0x0000, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x0000, 0x017E, 0x0178,
								0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
								0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
								0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
								0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
								0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
								0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF };





//============================================================================
//		NStringEncoder::NStringEncoder : Constructor.
//----------------------------------------------------------------------------
NStringEncoder::NStringEncoder(void)
{
}





//============================================================================
//		NStringEncoder::~NStringEncoder : Destructor.
//----------------------------------------------------------------------------
NStringEncoder::~NStringEncoder(void)
{
}





//============================================================================
//		NStringEncoder::Convert : Convert a string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::Convert(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NStringEncoding		genericSrc, genericDst;
	NUnicodeParser		theParser;
	NData				rawSrc;
	NStatus				theErr;



	// Get the state we need
	genericSrc = GetEncodingGeneric(srcEncoding);
	genericDst = GetEncodingGeneric(dstEncoding);
	rawSrc     = srcData;



	// Prepare the input
	if (IsEncodingUTF(genericSrc))
		{
		theParser.RemoveBOM(rawSrc, srcEncoding);
		RemoveTerminator(   rawSrc, srcEncoding);
	
		SwapUTF(rawSrc, srcEncoding, genericSrc);
		}



	// Convert the string
	switch (genericSrc) {
		case kNStringEncodingUTF8:
			theErr = ConvertFromUTF8( rawSrc, dstData, genericDst);
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertFromUTF16(rawSrc, dstData, genericDst);
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertFromUTF32(rawSrc, dstData, genericDst);
			break;
		
		case kNStringEncodingASCII:
		case kNStringEncodingMacRoman:
		case kNStringEncodingISOLatin1:
		case kNStringEncodingWindowsLatin1:
			theErr = ConvertFromLegacy(rawSrc, dstData, genericSrc, genericDst);
			break;

		default:
			NN_LOG("Unknown encoding: %d", genericSrc);
			theErr = kNErrParam;
			break;
		}



	// Prepare the output
	if (theErr == kNoErr && IsEncodingUTF(genericDst))
		SwapUTF(dstData, genericDst, dstEncoding);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertToUTF32 : Convert a character to UTF32.
//----------------------------------------------------------------------------
utf32_t NStringEncoder::ConvertToUTF32(NStringEncoding srcEncoding, NIndex srcSize, const void *srcPtr)
{	UTF32					*dstStart32, *dstEnd32;
	const UTF32				*srcStart32, *srcEnd32;
	const UTF16				*srcStart16, *srcEnd16;
	const UTF8				*srcStart8, *srcEnd8;
	ConversionResult		theResult;
	NData					dstData;
	UTF32					dstChar;
	NStatus					theErr;



	// Get the state we need
	srcStart8 = (const UTF8 *) (((const uint8_t *) srcPtr) + 0);
	srcEnd8   = (const UTF8 *) (((const uint8_t *) srcPtr) + srcSize);

	srcStart16 = (const UTF16 *) srcStart8;
	srcEnd16   = (const UTF16 *) srcEnd8;

	srcStart32 = (const UTF32 *) srcStart8;
	srcEnd32   = (const UTF32 *) srcEnd8;

	dstStart32 = &dstChar;
	dstEnd32   =  dstStart32 + 1;



	// Adjust the encoding
	//
	// Endian-specific types are made generic if they match the native endian-ness,
	// avoiding the slower generic conversion since no swapping is required.
	switch (srcEncoding) {
		case kNStringEncodingUTF16BE:
			if (kNEndianNative == kNEndianBig)
				srcEncoding = kNStringEncodingUTF16;
			break;

		case kNStringEncodingUTF16LE:
			if (kNEndianNative == kNEndianLittle)
				srcEncoding = kNStringEncodingUTF16;
			break;

		case kNStringEncodingUTF32BE:
			if (kNEndianNative == kNEndianBig)
				srcEncoding = kNStringEncodingUTF32;
			break;

		case kNStringEncodingUTF32LE:
			if (kNEndianNative == kNEndianLittle)
				srcEncoding = kNStringEncodingUTF32;
			break;
		
		default:
			break;
		}



	// Convert the character
	//
	// Non UTF characters currently go through a relatively slow generic conversion.
	switch (srcEncoding) {
		case kNStringEncodingUTF8:
			theResult = ConvertUTF8toUTF32( &srcStart8,  srcEnd8,  &dstStart32, dstEnd32, lenientConversion);
			break;
		
		case kNStringEncodingUTF16:
			theResult = ConvertUTF16toUTF32(&srcStart16, srcEnd16, &dstStart32, dstEnd32, lenientConversion);
			break;

		case kNStringEncodingUTF32:
			NN_UNUSED(srcEnd32);
			dstChar   = *srcStart32;
			theResult = conversionOK;
			break;

		default:
			dstChar = 0;
			theErr  = Convert(NData(srcSize, srcPtr), dstData, srcEncoding, kNStringEncodingUTF32);
			theResult = (theErr == kNoErr) ? conversionOK : sourceIllegal;

			if (theErr == kNoErr)
				{
				NN_ASSERT(dstData.GetSize() == 4);
				memcpy(&dstChar, dstData.GetData(), sizeof(dstChar));
				}
			break;
		}



	// Handle failure
	if (theResult != conversionOK)
		{
		NN_LOG("Unable to convert to UTF32");
		dstChar = 0;
		}
	
	return(dstChar);
}





//============================================================================
//		NStringEncoder::AddTerminator : Add a terminating null character.
//----------------------------------------------------------------------------
void NStringEncoder::AddTerminator(NData &theData, NStringEncoding theEncoding)
{	NIndex			sizeData, sizeTerm;
	bool			modifyData;
	const uint8_t	*thePtr;



	// Get the state we need
	sizeData = theData.GetSize();
	sizeTerm = GetMaxCharSize(theEncoding);

	NN_ASSERT(sizeTerm > 0 && sizeTerm <= (NIndex) sizeof(kStringTerminator));



	// Add the terminator
	modifyData = (sizeData < sizeTerm);

	if (!modifyData)
		{
		thePtr     = theData.GetData(sizeData - sizeTerm);
		modifyData = (memcmp(thePtr, &kStringTerminator, sizeTerm) != 0);
		}

	if (modifyData)
		theData.SetSize(sizeData + sizeTerm);
}





//============================================================================
//		NStringEncoder::RemoveTerminator : Remove a terminating null character.
//----------------------------------------------------------------------------
void NStringEncoder::RemoveTerminator(NData &theData, NStringEncoding theEncoding)
{	NIndex			sizeData, sizeTerm;
	bool			modifyData;
	const uint8_t	*thePtr;



	// Get the state we need
	sizeData = theData.GetSize();
	sizeTerm = GetMaxCharSize(theEncoding);

	NN_ASSERT(sizeTerm > 0 && sizeTerm <= (NIndex) sizeof(kStringTerminator));



	// Remove the terminator
	modifyData = (sizeData >= sizeTerm);
	
	if (modifyData)
		{
		thePtr     = theData.GetData(sizeData - sizeTerm);
		modifyData = (memcmp(thePtr, &kStringTerminator, sizeTerm) == 0);
		}

	if (modifyData)
		theData.SetSize(sizeData - sizeTerm);
}





//============================================================================
//		NStringEncoder::GetEncoding : Get a string encoding.
//----------------------------------------------------------------------------
NStringEncoding NStringEncoder::GetEncoding(const NData &theData)
{	NStringEncoding		theEncoding;
	NUnicodeParser		theParser;
	NRange				theRange;



	// Determine the encoding
	//
	// For now we assume a BOM, or default to UTF8. This should be improved with
	// additional heuristics (check for UTF16/32 encodings of common vowels in
	// both BE and LE, etc).
	theEncoding = theParser.GetBOM(theData, theRange);

	if (theEncoding == kNStringEncodingInvalid)
		theEncoding = kNStringEncodingUTF8;

	return(theEncoding);
}





//============================================================================
//		NStringEncoder::GetSize : Get a string length.
//----------------------------------------------------------------------------
NIndex NStringEncoder::GetSize(const void *thePtr, NStringEncoding theEncoding)
{	NIndex				theSize;
	const utf32_t		*ptr32;
	const utf16_t		*ptr16;



	// Get the state we need
	ptr32   = (const utf32_t *) thePtr;
	ptr16   = (const utf16_t *) thePtr;
	theSize = 0;



	// Get the size
	switch (theEncoding) {
		case kNStringEncodingUTF8:
		case kNStringEncodingASCII:
		case kNStringEncodingMacRoman:
		case kNStringEncodingISOLatin1:
		case kNStringEncodingWindowsLatin1:
			theSize = (NIndex) strlen((const char *) thePtr);
			break;

		case kNStringEncodingUTF16:
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			while (*ptr16++ != 0x00)
				theSize += sizeof(utf16_t);
			break;

		case kNStringEncodingUTF32:
		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			while (*ptr32++ != 0x00)
				theSize += sizeof(utf32_t);
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			break;
		}

	return(theSize);
}





//============================================================================
//		NStringEncoder::IsEncodingUTF : Is an encoding a UTF encoding?
//----------------------------------------------------------------------------
bool NStringEncoder::IsEncodingUTF(NStringEncoding theEncoding)
{	bool	isUTF;



	// Is an encoding a Unicode encoding?
	switch (theEncoding) {
		case kNStringEncodingUTF8:
		case kNStringEncodingUTF16:
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
		case kNStringEncodingUTF32:
		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			isUTF = true;
			break;
		
		default:
			isUTF = false;
			break;
		}
	
	return(isUTF);
}





//============================================================================
//		NStringEncoder::GetEncodingGeneric : Get the generic form of an encoding.
//----------------------------------------------------------------------------
NStringEncoding NStringEncoder::GetEncodingGeneric(NStringEncoding theEncoding)
{


	// Get the generic form
	switch (theEncoding) {
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			theEncoding = kNStringEncodingUTF16;
			break;

		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			theEncoding = kNStringEncodingUTF32;
			break;
		
		default:
			break;
		}
	
	return(theEncoding);
}





//============================================================================
//		NStringEncoder::GetEncodingEndian : Get the EndianFormat of an encoding.
//----------------------------------------------------------------------------
NEndianFormat NStringEncoder::GetEncodingEndian(NStringEncoding theEncoding)
{	NEndianFormat		theFormat;



	// Get the endian format
	switch (theEncoding) {
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF32BE:
			theFormat = kNEndianBig;
			break;

		case kNStringEncodingUTF16LE:
		case kNStringEncodingUTF32LE:
			theFormat = kNEndianLittle;
			break;

		default:
			theFormat = kNEndianNative;
			break;
		}
	
	return(theFormat);
}





#pragma mark protected
//============================================================================
//		NStringEncoder::GetMaxCharSize : Get the maximum size of a character.
//----------------------------------------------------------------------------
NIndex NStringEncoder::GetMaxCharSize(NStringEncoding theEncoding)
{	NIndex		theSize;



	// Get the state we need
	switch (theEncoding) {
		case kNStringEncodingUTF8:
		case kNStringEncodingASCII:
		case kNStringEncodingMacRoman:
		case kNStringEncodingISOLatin1:
		case kNStringEncodingWindowsLatin1:
			theSize = (NIndex) sizeof(utf8_t);
			break;

		case kNStringEncodingUTF16:
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			theSize = (NIndex) sizeof(utf16_t);
			break;

		case kNStringEncodingUTF32:
		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			theSize = (NIndex) sizeof(utf32_t);
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			theSize = 0;
			break;
		}

	return(theSize);
}





//============================================================================
//		NStringEncoder::ConvertFromLegacy : Convert from a legacy encoding.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertFromLegacy(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NIndex				n, numChars;
	const utf32_t		*srcTable;
	NData				dataUTF32;
	const uint8_t		*srcChars;
	utf32_t				*dstChars;
	utf32_t				dstChar;
	NStatus				theErr;



	// Get the state we need
	numChars = srcData.GetSize() / sizeof(uint8_t);
	srcChars = (const uint8_t *) srcData.GetData();
	srcTable = GetLegacyToUTF32( srcEncoding);
	
	if (srcTable == NULL)
		return(kNErrNotSupported);



	// Convert to UTF32
	dataUTF32.SetSize(numChars * sizeof(utf32_t));
	dstChars = (utf32_t *) dataUTF32.GetData();

	for (n = 0; n < numChars; n++)
		{
		dstChar = srcTable[srcChars[n]];
		
		if (dstChar == 0x0000)
			dstChars[n] = kStringInvalidChar;
		else
			dstChars[n] = dstChar;
		}
	
	
	
	// Convert to the final encoding
	theErr = Convert(dataUTF32, dstData, kNStringEncodingUTF32, dstEncoding);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertToLegacy : Convert to a legacy encoding.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertToLegacy(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NIndex							n, numChars;
	const NUTF32LegacyMap			*srcTable;
	NData							dataUTF32;
	const utf32_t					*srcChars;
	uint8_t							*dstChars;
	NUTF32LegacyMapConstIterator	theIter;
	NStatus							theErr;



	// Convert to UTF32
	theErr = Convert(srcData, dataUTF32, srcEncoding, kNStringEncodingUTF32);
	if (theErr != kNoErr)
		return(theErr);



	// Get the state we need
	numChars = dataUTF32.GetSize() / sizeof(utf32_t);
	srcChars = (const utf32_t *) dataUTF32.GetData();
	srcTable = GetLegacyFromUTF32(dstEncoding);

	if (srcTable == NULL)
		return(kNErrNotSupported);



	// Convert to the final encoding
	dstData.SetSize(numChars * sizeof(uint8_t));
	dstChars = (uint8_t *) dstData.GetData();

	for (n = 0; n < numChars; n++)
		{
		theIter = srcTable->find(srcChars[n]);
		
		if (theIter != srcTable->end())
			dstChars[n] = theIter->second;
		else
			dstChars[n] = kStringInvalidChar;
		}
	
	return(kNoErr);
}





//============================================================================
//		NStringEncoder::ConvertFromUTF8 : Convert a UTF8 string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertFromUTF8(const NData &srcData, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			dstData = srcData;
			theErr  = kNoErr;
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertUTF8ToUTF16(srcData, dstData);
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertUTF8ToUTF32(srcData, dstData);
			break;

		case kNStringEncodingASCII:
		case kNStringEncodingMacRoman:
		case kNStringEncodingISOLatin1:
		case kNStringEncodingWindowsLatin1:
			theErr = ConvertToLegacy(srcData, dstData, kNStringEncodingUTF8, dstEncoding);
			break;

		default:
			NN_LOG("Unknown encoding: %d", dstEncoding);
			theErr = kNErrParam;
			break;
		}

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertFromUTF16 : Convert a UTF16 string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertFromUTF16(const NData &srcData, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertUTF16ToUTF8(srcData, dstData);
			break;

		case kNStringEncodingUTF16:
			dstData = srcData;
			theErr  = kNoErr;
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertUTF16ToUTF32(srcData, dstData);
			break;

		case kNStringEncodingASCII:
		case kNStringEncodingMacRoman:
		case kNStringEncodingISOLatin1:
		case kNStringEncodingWindowsLatin1:
			theErr = ConvertToLegacy(srcData, dstData, kNStringEncodingUTF16, dstEncoding);
			break;

		default:
			NN_LOG("Unknown encoding: %d", dstEncoding);
			theErr = kNErrParam;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertFromUTF32 : Convert a UTF32 string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertFromUTF32(const NData &srcData, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertUTF32ToUTF8(srcData, dstData);
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertUTF32ToUTF16(srcData, dstData);
			break;

		case kNStringEncodingUTF32:
			dstData = srcData;
			theErr  = kNoErr;
			break;

		case kNStringEncodingASCII:
		case kNStringEncodingMacRoman:
		case kNStringEncodingISOLatin1:
		case kNStringEncodingWindowsLatin1:
			theErr = ConvertToLegacy(srcData, dstData, kNStringEncodingUTF32, dstEncoding);
			break;

		default:
			NN_LOG("Unknown encoding: %d", dstEncoding);
			theErr = kNErrParam;
			break;
		}
	
	return(theErr);
}






//============================================================================
//		NStringEncoder::ConvertUTF8ToUTF16 : Convert UTF8 to UTF16.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF8ToUTF16(const NData &srcData, NData &dstData)
{	const UTF8				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	const uint8_t			*srcBase;
	uint8_t					*dstBase;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 2);

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF8 *) (srcBase + 0);
		srcEnd   = (const UTF8 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF16 *) (dstBase + 0);
		dstEnd   = (UTF16 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF8toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF8ToUTF32 : Convert UTF8 to UTF32.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF8ToUTF32(const NData &srcData, NData &dstData)
{	const UTF8				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	const uint8_t			*srcBase;
	uint8_t					*dstBase;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 4);

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF8 *) (srcBase + 0);
		srcEnd   = (const UTF8 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF32 *) (dstBase + 0);
		dstEnd   = (UTF32 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF8toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF16ToUTF8 : Convert UTF16 to UTF8.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF16ToUTF8(const NData &srcData, NData &dstData)
{	const UTF16				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	const uint8_t			*srcBase;
	uint8_t					*dstBase;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF16 *) (srcBase + 0);
		srcEnd   = (const UTF16 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF8 *) (dstBase + 0);
		dstEnd   = (UTF8 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF16toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF16ToUTF32 : Convert UTF16 to UTF32.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF16ToUTF32(const NData &srcData, NData &dstData)
{	const UTF16				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	const uint8_t			*srcBase;
	uint8_t					*dstBase;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 2);

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF16 *) (srcBase + 0);
		srcEnd   = (const UTF16 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF32 *) (dstBase + 0);
		dstEnd   = (UTF32 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF16toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF32ToUTF8 : Convert UTF32 to UTF8.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF32ToUTF8(const NData &srcData, NData &dstData)
{	const UTF32				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	const uint8_t			*srcBase;
	uint8_t					*dstBase;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF32 *) (srcBase + 0);
		srcEnd   = (const UTF32 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF8 *) (dstBase + 0);
		dstEnd   = (UTF8 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF32toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF32ToUTF16 : Convert UTF32 to UTF16.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF32ToUTF16(const NData &srcData, NData &dstData)
{	const UTF32				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	const uint8_t			*srcBase;
	uint8_t					*dstBase;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF32 *) (srcBase + 0);
		srcEnd   = (const UTF32 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF16 *) (dstBase + 0);
		dstEnd   = (UTF16 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF32toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF : Process a Unicode conversion.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF(NData &theData, const void *dataEnd, uint32_t theResult)
{	NStatus		theErr;



	// Process the result
	switch ((ConversionResult) theResult) {
		case conversionOK:
			theData.SetSize((NIndex) (((const uint8_t *) dataEnd) - theData.GetData()) );
			theErr = kNoErr;
			break;
			
		case targetExhausted:
			theData.SetSize(theData.GetSize() * 2);
			theErr = kNErrExhaustedDst;
			break;
			
		case sourceExhausted:
			theErr = kNErrExhaustedSrc;
			break;

		case sourceIllegal:
			theErr = kNErrMalformed;
			break;

		default:
			NN_LOG("Unknown result: %d", theResult);
			theErr = kNErrParam;
			break;
		}

	return(theErr);
}





//============================================================================
//		NStringEncoder::SwapUTF : Endian-swap UTF data.
//----------------------------------------------------------------------------
void NStringEncoder::SwapUTF(NData &theData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NIndex				n, charSize, dataSize;
	NEndianFormat		srcFormat, dstFormat;
	uint8_t				*dataPtr;



	// Validate our parameters
	NN_ASSERT(GetMaxCharSize(srcEncoding) == GetMaxCharSize(dstEncoding));
	NN_ASSERT(IsEncodingUTF( srcEncoding) && IsEncodingUTF( dstEncoding));



	// Get the state we need
	srcFormat = GetEncodingEndian(srcEncoding);
	dstFormat = GetEncodingEndian(dstEncoding);
	charSize  = GetMaxCharSize(srcEncoding);
	
	if (charSize == 1 || srcFormat == dstFormat)
		return;



	// Get the data
	dataPtr  = theData.GetData();
	dataSize = theData.GetSize();
	
	NN_ASSERT((dataSize % charSize) == 0);



	// Swap the data
	for (n = 0; n < dataSize; n += charSize)
		{
		switch (charSize) {
			case sizeof(uint16_t):
				NSwapUInt16((uint16_t *) dataPtr);
				break;
			
			case sizeof(uint32_t):
				NSwapUInt32((uint32_t *) dataPtr);
				break;

			default:
				NN_LOG("Unknown UTF char size: %d", charSize);
				return;
				break;
			}
		
		dataPtr += charSize;
		}
}





//============================================================================
//		NStringEncoder::GetLegacyToUTF32 : Get a legacy->UTF32 table.
//----------------------------------------------------------------------------
const utf32_t *NStringEncoder::GetLegacyToUTF32(NStringEncoding srcEncoding)
{	const utf32_t	*theTable;



	// Get the table
	switch (srcEncoding) {
		case kNStringEncodingASCII:				theTable = kASCII_to_UTF32;				break;
		case kNStringEncodingMacRoman:			theTable = kMacRoman_to_UTF32;			break;
		case kNStringEncodingISOLatin1:			theTable = kISOLatin1_to_UTF32;			break;
		case kNStringEncodingWindowsLatin1:		theTable = kWindowsLatin1_to_UTF32;		break;
		default:
			NN_LOG("Unknown encoding: %ld", srcEncoding);
			theTable = NULL;
			break;
		}
	
	return(theTable);
}





//============================================================================
//		NStringEncoder::GetLegacyFromUTF32 : Get a UTF32->legacy table.
//----------------------------------------------------------------------------
const NUTF32LegacyMap *NStringEncoder::GetLegacyFromUTF32(NStringEncoding dstEncoding)
{	static NMutex				sMutex;
	static NUTF32LegacyMap		sUTF32_to_ASCII;
	static NUTF32LegacyMap		sUTF32_to_MacRoman;
	static NUTF32LegacyMap		sUTF32_to_ISOLatin1;
	static NUTF32LegacyMap		sUTF32_to_WindowsLatin1;

	StLock					acquireLock(sMutex);
	const NUTF32LegacyMap	*theTable;



	// Populate the tables
	if (sUTF32_to_ASCII.empty())
		{
		PopulateLegacyMap(kNASCIIMax,	kASCII_to_UTF32,			sUTF32_to_ASCII);
		PopulateLegacyMap(255,			kMacRoman_to_UTF32,			sUTF32_to_MacRoman);
		PopulateLegacyMap(255,			kISOLatin1_to_UTF32,		sUTF32_to_ISOLatin1);
		PopulateLegacyMap(255,			kWindowsLatin1_to_UTF32,	sUTF32_to_WindowsLatin1);
		}



	// Get the table
	switch (dstEncoding) {
		case kNStringEncodingASCII:				theTable = &sUTF32_to_ASCII;			break;
		case kNStringEncodingMacRoman:			theTable = &sUTF32_to_MacRoman;			break;
		case kNStringEncodingISOLatin1:			theTable = &sUTF32_to_ISOLatin1;		break;
		case kNStringEncodingWindowsLatin1:		theTable = &sUTF32_to_WindowsLatin1;	break;
		default:
			NN_LOG("Unknown encoding: %ld", dstEncoding);
			theTable = NULL;
			break;
		}

	return(theTable);
}





//============================================================================
//		NStringEncoder::PopulateLegacyMap : Populate a UTF32->legacy map.
//----------------------------------------------------------------------------
void NStringEncoder::PopulateLegacyMap(NIndex maxChar, const utf32_t *toUTF32, NUTF32LegacyMap &fromUTF32)
{	NIndex		n;



	// Populate the table
	for (n = 0; n <= maxChar; n++)
		fromUTF32[toUTF32[n]] = (char) n;
}


