/*	NAME:
		NDataCompressor.h

	DESCRIPTION:
		Data compressor.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATACOMPRESSOR_HDR
#define NDATACOMPRESSOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Compression algorithms
//
// These values are considered to be fixed, and will never change.
typedef enum {
	kNCompressionNull			= 0x6E6E756C,
	kNCompressionZLib			= 0x6E7A6C62
} NCompression;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Compression header
#pragma pack(push, 4)

typedef struct {
	uint32_t		compression;
	uint32_t		reserved;
	uint32_t		origSize;
} NCompressionHeader;

#pragma pack(pop)





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataCompressor {
public:
										NDataCompressor(void);
	virtual							   ~NDataCompressor(void);


	// Compress/decompress data
	//
	// Compressed data is returned with a NCompressionHeader prefix.
	//
	// If this header is removed from the data and stored elsewhere, it must be
	// supplied externally to decompress the data.
	//
	// External headers are assumed to be in native endian. Internal headers are
	// stored in big endian.
	NData								Compress(  const NData &srcData,       NCompression theCompression=kNCompressionZLib);
	NData								Decompress(const NData &srcData, const NCompressionHeader *theHeader=NULL);


	// Get the required size for compressed data
	//
	// Returns the maximum size required for data compressed with the specified
	// algorithm, including a NCompressionHeader prefix.
	NIndex								GetRequiredSize(NIndex theSize, NCompression theCompression=kNCompressionZLib);


private:
	NStatus								Null_Compress(  const NData &srcData, NData &dstData);
	NStatus								Null_Decompress(const NData &srcData, NData &dstData);
	NIndex								Null_RequiredSize(NIndex theSize);
	
	NStatus								ZLib_Compress(  const NData &srcData, NData &dstData);
	NStatus								ZLib_Decompress(const NData &srcData, NData &dstData);
	NIndex								ZLib_RequiredSize(NIndex theSize);


private:


};





#endif // NDATACOMPRESSOR_HDR


