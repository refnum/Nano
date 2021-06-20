/*	NAME:
		NDataCompressor.cpp

	DESCRIPTION:
		Data compressor.

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
#include "NDataCompressor.h"

// Nano
#include "NData.h"
#include "Nano_zlib.h"





//=============================================================================
//		NDataCompressor::Compress : Compress data.
//-----------------------------------------------------------------------------
NData NDataCompressor::Compress(NCompression theCompression, const NData& theData)
{


	// Compress the data
	switch (theCompression)
	{
		case NCompression::Null:
			return Null_Compress(theData);
			break;

		case NCompression::ZLib:
			return ZLib_Compress(theData);
			break;
	}
}





//=============================================================================
//		NDataCompressor::Decompress : Decompress data.
//-----------------------------------------------------------------------------
NData NDataCompressor::Decompress(NCompression theCompression, const NData& theData)
{


	// Decompress the data
	switch (theCompression)
	{
		case NCompression::Null:
			return Null_Decompress(theData);
			break;

		case NCompression::ZLib:
			return ZLib_Decompress(theData);
			break;
	}
}





//=============================================================================
//		NDataCompressor::GetMaxSize : Get the maximum size.
//-----------------------------------------------------------------------------
size_t NDataCompressor::GetMaxSize(NCompression theCompression, size_t theSize)
{


	// Get the maximum size
	switch (theCompression)
	{
		case NCompression::Null:
			return Null_MaxSize(theSize);
			break;

		case NCompression::ZLib:
			return ZLib_MaxSize(theSize);
			break;
	}
}





#pragma mark private
//=============================================================================
//		NDataCompressor::Null_Compress : Compress using null compression.
//-----------------------------------------------------------------------------
NData NDataCompressor::Null_Compress(const NData& theData)
{


	// Compress the data
	return theData;
}





//=============================================================================
//		NDataCompressor::Null_Decompress : Decompress using null compression.
//-----------------------------------------------------------------------------
NData NDataCompressor::Null_Decompress(const NData& theData)
{


	// Decompress the data
	return theData;
}





//=============================================================================
//		NDataCompressor::Null_MaxSize : Get the max size for null compression.
//-----------------------------------------------------------------------------
size_t NDataCompressor::Null_MaxSize(size_t theSize)
{


	// Get the size
	return theSize;
}





//=============================================================================
//		NDataCompressor::ZLib_Compress : Compress using zlib compression.
//-----------------------------------------------------------------------------
NData NDataCompressor::ZLib_Compress(const NData& theData)
{


	// Validate our state
	static_assert(sizeof(uLongf) == sizeof(size_t));



	// Get the state we need
	const Bytef* srcBytes = reinterpret_cast<const Bytef*>(theData.GetData());
	uLongf       srcSize  = theData.GetSize();

	NData  dstData(ZLib_MaxSize(srcSize), nullptr, NDataSource::None);
	Bytef* dstBytes = reinterpret_cast<Bytef*>(dstData.GetMutableData());
	uLongf dstSize  = dstData.GetSize();



	// Compress the data
	int zErr = compress2(dstBytes, &dstSize, srcBytes, srcSize, 9);
	NN_EXPECT_NOT_ERR(zErr);

	if (zErr != Z_OK)
	{
		dstSize = 0;
	}



	// Adjust the buffer
	dstData.SetSize(size_t(dstSize));

	return dstData;
}





//=============================================================================
//		NDataCompressor::ZLib_Decompress : Decompress using zlib compression.
//-----------------------------------------------------------------------------
NData NDataCompressor::ZLib_Decompress(const NData& theData)
{


	// Get the state we need
	//
	// The output buffer is initially sized for a 4:1 compression ratio,
	// but will grow as needed if this proves insufficient.
	constexpr size_t kZLibRatio = 4;

	const Bytef* srcBytes = reinterpret_cast<const Bytef*>(theData.GetData());
	uLongf       srcSize  = theData.GetSize();

	NData  dstData(srcSize * kZLibRatio, nullptr, NDataSource::None);
	Bytef* dstBytes = nullptr;
	uLongf dstSize  = 0;



	// Decompress the data
	//
	// If the output buffer is too small we double it on each pass.
	int zErr = Z_OK;

	do
	{
		dstBytes = reinterpret_cast<Bytef*>(dstData.GetMutableData());
		dstSize  = dstData.GetSize();

		zErr = uncompress(dstBytes, &dstSize, srcBytes, srcSize);
		if (zErr == Z_BUF_ERROR)
		{
			dstData.Append(dstData.GetSize(), nullptr, NDataSource::None);
		}
	} while (zErr == Z_BUF_ERROR);

	if (zErr != Z_OK)
	{
		dstSize = 0;
	}



	// Adjust the buffer
	dstData.SetSize(dstSize);

	return dstData;
}





//=============================================================================
//		NDataCompressor::ZLib_MaxSize : Get the max size for zlib compression.
//-----------------------------------------------------------------------------
size_t NDataCompressor::ZLib_MaxSize(size_t theSize)
{


	// Validate our state
	static_assert(sizeof(uLongf) == sizeof(size_t));



	// Get the size
	return size_t(compressBound(uLong(theSize)));
}
