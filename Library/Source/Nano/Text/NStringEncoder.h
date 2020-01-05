/*	NAME:
		NStringEncoder.h

	DESCRIPTION:
		String encoder.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTRINGENCODER_HDR
#define NSTRINGENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NByteSwap.h"
#include "NData.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// String encodings
typedef enum {
	// Meta
	kNStringEncodingInvalid,


	// Unicode
	//
	// Generic UTF16 and UTF32 encodings store data in the byte order
	// of the BOM, or kEndianNative order if no BOM is present.
	kNStringEncodingUTF8,
	kNStringEncodingUTF16,
	kNStringEncodingUTF16BE,
	kNStringEncodingUTF16LE,
	kNStringEncodingUTF32,
	kNStringEncodingUTF32BE,
	kNStringEncodingUTF32LE,
	
	
	// Legacy
	//
	// Although conversion from legacy to Unicode is exact, converting
	// from Unicode to legacy may be lossy.
	//
	// Characters that can not be represented in a legacy encoding are
	// replaced with a placeholder character.
	kNStringEncodingASCII,
	kNStringEncodingMacRoman,
	kNStringEncodingISOLatin1,				// ISO 8859-1
	kNStringEncodingWindowsLatin1			// ANSI 1252
} NStringEncoding;


// Misc
static const uint32_t kNASCIIMax									= 0x7F;





//============================================================================
//      Types
//----------------------------------------------------------------------------
typedef std::map<utf32_t, uint8_t>									NUTF32LegacyMap;
typedef NUTF32LegacyMap::iterator									NUTF32LegacyMapIterator;
typedef NUTF32LegacyMap::const_iterator								NUTF32LegacyMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStringEncoder {
public:
										NStringEncoder(void);
	virtual							   ~NStringEncoder(void);


	// Convert a string
	//
	// Meta-data such as a BOM prefix or null terminator are removed.
	NStatus								Convert(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding);


	// Convert a character
	utf32_t								ConvertToUTF32(NStringEncoding srcEncoding, NIndex srcSize, const void *srcPtr);


	// Add/remove a null terminator
	//
	// The null terminator is the maximum character size for the encoding.
	void								AddTerminator(   NData &theData, NStringEncoding theEncoding);
	void								RemoveTerminator(NData &theData, NStringEncoding theEncoding);


	// Get a string encoding
	//
	// Returns kNStringEncodingInvalid if the encoding is unknown.
	NStringEncoding						GetEncoding(const NData &theData);


	// Get a string length
	//
	// Returns the number of bytes in a NULL-terminated string, not counting the terminator.
	NIndex								GetSize(const void *thePtr, NStringEncoding theEncoding);


	// Is an encoding a UTF encoding?
	static bool							IsEncodingUTF(NStringEncoding theEncoding);


	// Get the generic form of an encoding
	//
	// Returns the encoding unchanged if it has no generic form.
	static NStringEncoding				GetEncodingGeneric(NStringEncoding theEncoding);


	// Get the EndianFormat of an encoding
	//
	// Returns kNEndianNative if the encoding has no endian-specific form.
	static NEndianFormat				GetEncodingEndian(NStringEncoding theEncoding);


private:
	NIndex								GetMaxCharSize(NStringEncoding theEncoding);

	NStatus								ConvertFromLegacy(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding);
	NStatus								ConvertToLegacy(  const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding);

	NStatus								ConvertFromUTF8( const NData &srcData, NData &dstData, NStringEncoding dstEncoding);
	NStatus								ConvertFromUTF16(const NData &srcData, NData &dstData, NStringEncoding dstEncoding);
	NStatus								ConvertFromUTF32(const NData &srcData, NData &dstData, NStringEncoding dstEncoding);

	NStatus								ConvertUTF8ToUTF16(const NData &srcData, NData &dstData);
	NStatus								ConvertUTF8ToUTF32(const NData &srcData, NData &dstData);

	NStatus								ConvertUTF16ToUTF8( const NData &srcData, NData &dstData);
	NStatus								ConvertUTF16ToUTF32(const NData &srcData, NData &dstData);

	NStatus								ConvertUTF32ToUTF8( const NData &srcData, NData &dstData);
	NStatus								ConvertUTF32ToUTF16(const NData &srcData, NData &dstData);
	
	NStatus								ConvertUTF(NData &theData, const void *dataEnd, uint32_t theResult);
	void								SwapUTF(   NData &theData, NStringEncoding srcEncoding, NStringEncoding dstEncoding);
	
	const utf32_t					   *GetLegacyToUTF32(  NStringEncoding srcEncoding);
	const NUTF32LegacyMap			   *GetLegacyFromUTF32(NStringEncoding dstEncoding);
	void								PopulateLegacyMap(NIndex maxChar, const utf32_t *toUTF32, NUTF32LegacyMap &fromUTF32);
};





#endif // NSTRINGENCODER_HDR
