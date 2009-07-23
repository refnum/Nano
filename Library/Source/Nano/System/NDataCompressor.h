/*	NAME:
		NDataCompressor.h

	DESCRIPTION:
		Data compressor.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
	UInt32		compression;
	UInt32		reserved;
	UInt32		origSize;
} NCompressionHeader;

#pragma pack(pop)





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataCompressor {
public:
										NDataCompressor( void);
	virtual								~NDataCompressor(void);


	// Compress/decompress data
	//
	// Compressed data is returned with a NCompressionHeader prefix.
	//
	// If this header is removed from the data and stored elsewhere, it must be
	// supplied externally to decompress the data. External headers are assumed
	// to be in native endian format.
	NData								Compress(  const NData &srcData,       NCompression theCompression=kNCompressionZLib);
	NData								Decompress(const NData &srcData, const NCompressionHeader *theHeader=NULL);


private:
	NStatus								CompressNull(const NData &srcData, NData &dstData);
	NStatus								CompressZLib(const NData &srcData, NData &dstData);

	NStatus								DecompressNull(const NData &srcData, NData &dstData);
	NStatus								DecompressZLib(const NData &srcData, NData &dstData);


private:


};





#endif // NDATACOMPRESSOR_HDR


