/*	NAME:
		NDataCompressor.cpp

	DESCRIPTION:
		Data compressor.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "zlib_nano.h"

#include "NByteSwap.h"
#include "NDataCompressor.h"





//============================================================================
//		NDataCompressor::NDataCompressor : Constructor.
//----------------------------------------------------------------------------
NDataCompressor::NDataCompressor(void)
{
}





//============================================================================
//		NDataCompressor::~NDataCompressor : Destructor.
//----------------------------------------------------------------------------
NDataCompressor::~NDataCompressor(void)
{
}





//============================================================================
//		NDataCompressor::Compress : Compress data.
//----------------------------------------------------------------------------
NData NDataCompressor::Compress(const NData &srcData, NCompression compressWith)
{	NCompressionHeader		theHeader;
	NData					dstData;
	NStatus					theErr;



	// Validate our parameters
	NN_ASSERT(((UInt32) srcData.GetSize()) <= kUInt32Max);



	// Prepare the header
	theHeader.compression = SwapUInt32_NtoB(compressWith);
	theHeader.reserved    = SwapUInt32_NtoB((UInt32) 0);
	theHeader.origSize    = SwapUInt32_NtoB(srcData.GetSize());

	dstData.AppendData(sizeof(theHeader), &theHeader);



	// Compress the data
	switch (compressWith) {
		case kNCompressionNull:
			theErr = Null_Compress(srcData, dstData);
			break;
		
		case kNCompressionZLib:
			theErr = ZLib_Compress(srcData, dstData);
			break;
		
		default:
			NN_LOG("Unknown compression: %08X", compressWith);
			theErr = kNErrParam;
			break;
		}



	// Check we actually compressed
	//
	// If the data was incompressible then we switch back to null compression
	// to minimise CPU overhead when decompressing.
	if (compressWith != kNCompressionNull)
		{
		if (dstData.GetSize() >= (NIndex) (srcData.GetSize()+sizeof(theHeader)))
			{
			NN_LOG("Unable to compress using %08X, switching to null", compressWith);
			return(Compress(srcData, kNCompressionNull));
			}
		}



	// Handle failure
	if (theErr != kNoErr)
		dstData.Clear();

	return(dstData);
}





//============================================================================
//		NDataCompressor::Decompress : Decompress data.
//----------------------------------------------------------------------------
NData NDataCompressor::Decompress(const NData &srcData, const NCompressionHeader *theHeader)
{	NData					srcHeaderless;
	NIndex					headerSize;
	NCompressionHeader		srcHeader;
	NData					dstData;
	NStatus					theErr;



	// Get the state we need
	headerSize = sizeof(srcHeader);



	// Extract the header
	if (theHeader != NULL)
		{
		srcHeaderless = srcData;
		srcHeader     = *theHeader;
		}
	else if (srcData.GetSize() >= headerSize)
		{
		memcpy(&srcHeader, srcData.GetData(), headerSize);
		srcHeaderless = NData(srcData.GetSize() - headerSize, srcData.GetData(headerSize), false);

		srcHeader.compression = SwapUInt32_BtoN(srcHeader.compression);
		srcHeader.reserved    = SwapUInt32_BtoN(srcHeader.reserved);
		srcHeader.origSize    = SwapUInt32_BtoN(srcHeader.origSize);
		}
	else
		{
		NN_LOG("No header: srcData is %ld bytes, expected at least %ld", srcData.GetSize(), headerSize);
		return(dstData);
		}



	// Prepare the data
	if (!dstData.SetSize(srcHeader.origSize))
		{
		NN_LOG("Unable to decompress %ld bytes", srcHeader.origSize);
		return(dstData);
		}



	// Decompress the data
	switch (srcHeader.compression) {
		case kNCompressionNull:
			theErr = Null_Decompress(srcHeaderless, dstData);
			break;
		
		case kNCompressionZLib:
			theErr = ZLib_Decompress(srcHeaderless, dstData);
			break;
		
		default:
			NN_LOG("Unknown compression: %08X", srcHeader.compression);
			theErr = kNErrParam;
			break;
		}



	// Handle failure
	if (theErr != kNoErr)
		dstData.Clear();

	return(dstData);
}





//============================================================================
//		NDataCompressor::GetRequiredSize : Get the required size.
//----------------------------------------------------------------------------
NIndex NDataCompressor::GetRequiredSize(NIndex theSize, NCompression compressWith)
{	NIndex		theResult;



	// Validate our parameters
	NN_ASSERT(((UInt32) theSize) <= kUInt32Max);



	// Get the size
	theResult = sizeof(NCompressionHeader);

	switch (compressWith) {
		case kNCompressionNull:
			theResult += Null_RequiredSize(theSize);
			break;
		
		case kNCompressionZLib:
			theResult += ZLib_RequiredSize(theSize);
			break;
		
		default:
			NN_LOG("Unknown compression: %08X", compressWith);
			theResult = 0;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NDataCompressor::Null_Compress : Compress using null compression.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NDataCompressor::Null_Compress(const NData &srcData, NData &dstData)
{


	// Compress the data
	if (dstData.AppendData(srcData.GetSize(), srcData.GetData()) == NULL)
		return(kNErrMemory);

	return(kNoErr);
}





//============================================================================
//		NDataCompressor::Null_Decompress : Decompress using null compression.
//----------------------------------------------------------------------------
NStatus NDataCompressor::Null_Decompress(const NData &srcData, NData &dstData)
{


	// Validate our parameters
	NN_ASSERT(srcData.GetSize() == dstData.GetSize());



	// Decompress the data
	memcpy(dstData.GetData(), srcData.GetData(), dstData.GetSize());

	return(kNoErr);
}





//============================================================================
//		NDataCompressor::Null_RequiredSize : Get the null required size.
//----------------------------------------------------------------------------
NIndex NDataCompressor::Null_RequiredSize(NIndex theSize)
{


	// Get the size
	return(theSize);
}





//============================================================================
//		NDataCompressor::ZLib_Compress : Compress using ZLib.
//----------------------------------------------------------------------------
NStatus NDataCompressor::ZLib_Compress(const NData &srcData, NData &dstData)
{	uLong			srcSize, dstSize, dstOrigSize;
	const Bytef		*srcBuffer;
	Bytef			*dstBuffer;
	int				zErr;



	// Get the state we need
	srcSize   =                 srcData.GetSize();
	srcBuffer = (const Bytef *) srcData.GetData();
	
	dstOrigSize = dstData.GetSize();
	dstSize     = compressBound(srcSize);
	dstBuffer   = (Bytef *) dstData.AppendData(dstSize);

	if (dstBuffer == NULL)
		return(kNErrMemory);



	// Compress the data
	zErr = compress2(dstBuffer, &dstSize, srcBuffer, srcSize, 9);
	NN_ASSERT_NOERR(zErr);
	
	if (zErr != Z_OK)
		return(kNErrExhaustedDst);



	// Adjust the buffer size
	NN_ASSERT(dstData.GetSize() >= (NIndex) (dstOrigSize + dstSize));
	dstData.SetSize(dstOrigSize + dstSize);
	
	return(kNoErr);
}





//============================================================================
//		NDataCompressor::ZLib_Decompress : Decompress using ZLib.
//----------------------------------------------------------------------------
NStatus NDataCompressor::ZLib_Decompress(const NData &srcData, NData &dstData)
{	uLongf			srcSize, dstSize;
	Bytef			*dstBuffer;
	const Bytef		*srcBuffer;
	int				zErr;



	// Validate our parameters
	NN_ASSERT(srcData.GetSize() < dstData.GetSize());



	// Get the state we need
	srcSize   =                 srcData.GetSize();
	srcBuffer = (const Bytef *) srcData.GetData();

	dstSize   =           dstData.GetSize();
	dstBuffer = (Bytef *) dstData.GetData();



	// Decompress the data
	zErr = uncompress(dstBuffer, &dstSize, srcBuffer, srcSize);
	NN_ASSERT_NOERR(zErr);
	
	if (zErr != Z_OK)
		return(kNErrExhaustedSrc);



	// Validate the buffer size
	NN_ASSERT(dstData.GetSize() == (NIndex) dstSize);
	
	return(kNoErr);
}





//============================================================================
//		NDataCompressor::ZLib_RequiredSize : Get the ZLib required size.
//----------------------------------------------------------------------------
NIndex NDataCompressor::ZLib_RequiredSize(NIndex theSize)
{


	// Get the size
	return(compressBound(theSize));
}













