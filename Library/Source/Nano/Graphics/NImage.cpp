/*	NAME:
		NImage.cpp

	DESCRIPTION:
		Image buffer.

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
#include "NImage.h"

// Nano
#include "NFileHandle.h"
#include "NFunction.h"





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define TAKE(...)                                           \
	NVectorSize                                             \
	{                                                       \
		__VA_ARGS__                                         \
	}





//=============================================================================
//		NImage::NImage : Constructor.
//-----------------------------------------------------------------------------
NImage::NImage(const NSize& theSize, NImageFormat theFormat, const NData& theData, size_t rowBytes)
	: mSize()
	, mData()
	, mFormat(NImageFormat::None)
	, mRowBytes(0)
{


	// Validate our parameters
	NN_REQUIRE(!theSize.IsEmpty());
	NN_REQUIRE(theSize.GetIntegral() == theSize);
	NN_REQUIRE(theSize.width >= 0);
	NN_REQUIRE(theSize.height >= 0);



	// Initialize ourselves
	mSize     = theSize;
	mData     = theData;
	mFormat   = theFormat;
	mRowBytes = rowBytes;



	// Create the image
	if (theData.IsEmpty() || mRowBytes == 0)
	{
		mRowBytes = GetWidth() * GetBytesPerPixel();
	}

	if (mData.IsEmpty())
	{
		mData.SetSize(GetHeight() * GetBytesPerRow());
	}

	NN_REQUIRE(mData.GetSize() >= (GetHeight() * GetBytesPerRow()));
	NN_REQUIRE(GetBytesPerRow() >= (GetWidth() * GetBytesPerPixel()));
}





//=============================================================================
//		NImage::NImage : Constructor.
//-----------------------------------------------------------------------------
NImage::NImage(const NData& theData)
	: mSize()
	, mData()
	, mFormat(NImageFormat::None)
	, mRowBytes(0)
{


	// Decode the image
	NStatus theErr = Decode(theData);
	NN_REQUIRE_NOT_ERR(theErr);
}





//=============================================================================
//		NImage::NImage : Constructor.
//-----------------------------------------------------------------------------
NImage::NImage(const NFile& theFile)
	: mSize()
	, mData()
	, mFormat(NImageFormat::None)
	, mRowBytes(0)
{


	// Load the image
	NStatus theErr = Load(theFile);
	NN_REQUIRE_NOT_ERR(theErr);
}





//=============================================================================
//		NImage::NImage : Constructor.
//-----------------------------------------------------------------------------
NImage::NImage()
	: mSize()
	, mData()
	, mFormat(NImageFormat::None)
	, mRowBytes(0)
{
}





//=============================================================================
//		NImage::IsValid : Is the image valid?
//-----------------------------------------------------------------------------
bool NImage::IsValid() const
{


	// Check our state
	return mFormat != NImageFormat::None && !mSize.IsEmpty();
}





//=============================================================================
//		NImage::Clear : Clear the image.
//-----------------------------------------------------------------------------
void NImage::Clear()
{


	// Reset our state
	mSize.Clear();
	mData.Clear();

	mFormat   = NImageFormat::None;
	mRowBytes = 0;
}





//=============================================================================
//		NImage::SetFormat : Set the format.
//-----------------------------------------------------------------------------
void NImage::SetFormat(NImageFormat theFormat)
{


	// Check our state
	if (mFormat == theFormat)
	{
		return;
	}


	// Set the format
	switch (mFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Invalid format!");
			return;

		case NImageFormat::A8:
			Convert_A8(theFormat);
			break;

		case NImageFormat::R8_G8_B8:
			Convert_R8_G8_B8(theFormat);
			break;

		case NImageFormat::B8_G8_R8:
			Convert_B8_G8_R8(theFormat);
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
			Convert_R8_G8_B8_A8(theFormat);
			break;

		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
			Convert_B8_G8_R8_A8(theFormat);
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
			Convert_A8_R8_G8_B8(theFormat);
			break;

		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			Convert_A8_B8_G8_R8(theFormat);
			break;
	}

	mFormat = theFormat;



	// Repack the image
	//
	// As the format may have shrunk we repack to reduce space.
	PackRows();
}





//=============================================================================
//		NImage::GetBitsPerPixel : Get the bits-per-pixel.
//-----------------------------------------------------------------------------
size_t NImage::GetBitsPerPixel() const
{


	// Get the bits per pixel
	switch (mFormat)
	{
		case NImageFormat::None:
			return 0;
			break;

		case NImageFormat::A8:
			return 8;
			break;

		case NImageFormat::R8_G8_B8:
		case NImageFormat::B8_G8_R8:
			return 24;
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			return 32;
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NImage::GetBitsPerComponent : Get the bits-per-component.
//-----------------------------------------------------------------------------
size_t NImage::GetBitsPerComponent() const
{


	// Get the bits per component
	switch (mFormat)
	{
		case NImageFormat::None:
			return 0;
			break;

		case NImageFormat::A8:
		case NImageFormat::R8_G8_B8:
		case NImageFormat::B8_G8_R8:
		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			return 8;
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		NImage::PackRows : Pack the rows.
//-----------------------------------------------------------------------------
void NImage::PackRows(size_t bytesPerRow)
{


	// Get the state we need
	size_t widthInBytes = GetWidth() * GetBytesPerPixel();

	if (bytesPerRow == 0)
	{
		bytesPerRow = widthInBytes;
	}

	NN_REQUIRE(bytesPerRow <= widthInBytes);



	// Pack the rows
	if (bytesPerRow != GetBytesPerRow())
	{
		NData newData(bytesPerRow * GetWidth(), nullptr, NDataSource::None);

		const uint8_t* srcRow = mData.GetData();
		uint8_t*       dstRow = newData.GetMutableData();

		for (size_t y = 0; y < GetHeight(); y++)
		{
			memcpy(dstRow, srcRow, widthInBytes);

			srcRow += mRowBytes;
			dstRow += widthInBytes;
		}

		mData     = newData;
		mRowBytes = widthInBytes;
	}
}





//=============================================================================
//		NImage::ForEachPixel : Process each pixel.
//-----------------------------------------------------------------------------
void NImage::ForEachPixel(const NFunctionEachImmutablePixel& theFunction) const
{


	// Process each pixel
	size_t bytesPerPixel = GetBytesPerPixel();

	ForEachRow(
	[&](size_t y, size_t theWidth, const uint8_t* rowPtr)
	{
		for (size_t x = 0; x < theWidth; x++)
		{
			if (!theFunction(x, y, rowPtr))
			{
				return false;
			}

			rowPtr += bytesPerPixel;
		}

		return true;
	});
}





//=============================================================================
//		NImage::ForEachPixel : Process each pixel.
//-----------------------------------------------------------------------------
void NImage::ForEachPixel(const NFunctionEachMutablePixel& theFunction)
{


	// Process each pixel
	size_t pixelBytes = GetBytesPerPixel();

	ForEachRow(
	[&](size_t y, size_t theWidth, uint8_t* rowPtr)
	{
		for (size_t x = 0; x < theWidth; x++)
		{
			if (!theFunction(x, y, rowPtr))
			{
				return false;
			}

			rowPtr += pixelBytes;
		}

		return true;
	});
}





//=============================================================================
//		NImage::ForEachRow : Process each row.
//-----------------------------------------------------------------------------
void NImage::ForEachRow(const NFunctionEachImmutableRow& theFunction) const
{


	// Get the state we need
	size_t         theWidth  = GetWidth();
	size_t         theHeight = GetHeight();
	size_t         rowBytes  = GetBytesPerRow();
	const uint8_t* rowPtr    = GetPixels();



	// Process the rows
	for (size_t y = 0; y < theHeight; y++)
	{
		if (!theFunction(y, theWidth, rowPtr))
		{
			return;
		}

		rowPtr += rowBytes;
	}
}





//=============================================================================
//		NImage::ForEachRow : Process each row.
//-----------------------------------------------------------------------------
void NImage::ForEachRow(const NFunctionEachMutableRow& theFunction)
{


	// Get the state we need
	size_t   theWidth  = GetWidth();
	size_t   theHeight = GetHeight();
	size_t   rowBytes  = GetBytesPerRow();
	uint8_t* rowPtr    = GetMutablePixels();



	// Process the rows
	for (size_t y = 0; y < theHeight; y++)
	{
		if (!theFunction(y, theWidth, rowPtr))
		{
			return;
		}

		rowPtr += rowBytes;
	}
}





//=============================================================================
//		NImage::Load : Load an image.
//-----------------------------------------------------------------------------
NStatus NImage::Load(const NFile& theFile, NImageFormat theFormat)
{


	// Load the image
	return Decode(NFileHandle::ReadData(theFile), theFormat);
}





//=============================================================================
//		NImage::Save : Save an image.
//-----------------------------------------------------------------------------
NStatus NImage::Save(const NFile& theFile, const NUTI& theType) const
{


	// Validate our parameters
	NN_REQUIRE(theFile.IsValid());


	// Get the state we need
	NUTI finalType = theType;

	if (!finalType.IsValid())
	{
		finalType = theFile.GetUTI();
	}

	if (!finalType.IsValid())
	{
		finalType = kNUTTypePNG;
	}



	// Save the image
	NData   theData = Encode(finalType);
	NStatus theErr  = NStatus::ExhaustedSrc;

	if (!theData.IsEmpty())
	{
		theErr = NFileHandle::WriteData(theFile, theData);
	}

	return theErr;
}





//=============================================================================
//		NImage::Decode : Decode an image.
//-----------------------------------------------------------------------------
NStatus NImage::Decode(const NData& theData, NImageFormat theFormat)
{


	// Validate our parameters
	NN_REQUIRE(!theData.IsEmpty());



	// Decode the image
	NStatus theErr = ImageDecode(theData);
	NN_EXPECT_NOT_ERR(theErr);

	if (theErr == NStatus::OK)
	{
		if (theFormat != NImageFormat::None && theFormat != mFormat)
		{
			SetFormat(theFormat);
		}
	}

	return theErr;
}





//=============================================================================
//		NImage::Encode : Encode an image.
//-----------------------------------------------------------------------------
NData NImage::Encode(const NUTI& theType) const
{


	// Validate our parameters
	NN_REQUIRE(theType.IsValid());



	// Encode the image
	return ImageEncode(theType);
}





#pragma mark private
//=============================================================================
//		NImage::Convert_A8 : Convert an A8 image.
//-----------------------------------------------------------------------------
void NImage::Convert_A8(NImageFormat theFormat)
{


	// Convert the image
	NImage tmpImage;

	switch (theFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Unable to convert image!");
			break;


		case NImageFormat::A8:
			// No-op
			break;

		case NImageFormat::R8_G8_B8:
		case NImageFormat::B8_G8_R8:
		case NImageFormat::R8_G8_B8_X8:
		case NImageFormat::B8_G8_R8_X8:
		case NImageFormat::X8_R8_G8_B8:
		case NImageFormat::X8_B8_G8_R8:
			tmpImage = NImage(GetSize(), theFormat);
			*this    = tmpImage;
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::B8_G8_R8_A8:
			tmpImage = NImage(GetSize(), NImageFormat::R8_G8_B8_A8);
			ForEachRow(
				NBindSelf(NImage::RowExpand8To32, kNArg2, kNArg3, size_t(3), &tmpImage, kNArg1));
			*this = tmpImage;
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::A8_B8_G8_R8:
			tmpImage = NImage(GetSize(), NImageFormat::R8_G8_B8_A8);
			ForEachRow(
				NBindSelf(NImage::RowExpand8To32, kNArg2, kNArg3, size_t(0), &tmpImage, kNArg1));
			*this = tmpImage;
			break;
	}
}





//=============================================================================
//		NImage::Convert_R8_G8_B8 : Convert an R8_G8_B8 image.
//-----------------------------------------------------------------------------
void NImage::Convert_R8_G8_B8(NImageFormat theFormat)
{


	// Convert the image
	NImage tmpImage;

	switch (theFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Unable to convert image!");
			break;

		case NImageFormat::A8:
			tmpImage = NImage(GetSize(), NImageFormat::A8);
			memset(tmpImage.GetMutablePixels(),
				   0xFF,
				   tmpImage.GetHeight() * tmpImage.GetBytesPerRow());
			*this = tmpImage;
			break;

		case NImageFormat::R8_G8_B8:
			// No-op
			break;

		case NImageFormat::B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowSwizzle24, kNArg2, kNArg3, TAKE(2, 1, 0)));
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
			tmpImage = NImage(GetSize(), NImageFormat::R8_G8_B8_A8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(0, 1, 2, 3),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;

		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
			tmpImage = NImage(GetSize(), NImageFormat::B8_G8_R8_A8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(2, 1, 0, 3),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
			tmpImage = NImage(GetSize(), NImageFormat::A8_R8_G8_B8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(3, 0, 1, 2),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;


		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			tmpImage = NImage(GetSize(), NImageFormat::A8_B8_G8_R8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(3, 2, 1, 0),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;
	}
}





//=============================================================================
//		NImage::Convert_B8_G8_R8 : Convert a B8_G8_R8 image.
//-----------------------------------------------------------------------------
void NImage::Convert_B8_G8_R8(NImageFormat theFormat)
{


	// Convert the image
	NImage tmpImage;

	switch (theFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Unable to convert image!");
			break;

		case NImageFormat::A8:
			tmpImage = NImage(GetSize(), NImageFormat::A8);
			memset(tmpImage.GetMutablePixels(),
				   0xFF,
				   tmpImage.GetHeight() * tmpImage.GetBytesPerRow());
			*this = tmpImage;
			break;

		case NImageFormat::R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowSwizzle24, kNArg2, kNArg3, TAKE(2, 1, 0)));
			break;

		case NImageFormat::B8_G8_R8:
			// No-op
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
			tmpImage = NImage(GetSize(), NImageFormat::R8_G8_B8_A8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(2, 1, 0, 3),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;

		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
			tmpImage = NImage(GetSize(), NImageFormat::B8_G8_R8_A8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(0, 1, 2, 3),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
			tmpImage = NImage(GetSize(), NImageFormat::A8_R8_G8_B8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(3, 2, 1, 0),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;


		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			tmpImage = NImage(GetSize(), NImageFormat::A8_B8_G8_R8);
			ForEachRow(NBindSelf(NImage::RowExpand24To32,
								 kNArg2,
								 kNArg3,
								 TAKE(3, 0, 1, 2),
								 &tmpImage,
								 kNArg1));
			*this = tmpImage;
			break;
	}
}





//=============================================================================
//		NImage::Convert_R8_G8_B8_A8 : Convert an R8_G8_B8_A8 image.
//-----------------------------------------------------------------------------
void NImage::Convert_R8_G8_B8_A8(NImageFormat theFormat)
{


	// Convert the image
	switch (theFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Unable to convert image!");
			break;

		case NImageFormat::A8:
			ForEachRow(NBindSelf(NImage::RowReduce32To8, kNArg2, kNArg3, size_t(3)));
			break;

		case NImageFormat::R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(0, 1, 2)));
			break;

		case NImageFormat::B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(2, 1, 0)));
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
			// No-op
			break;

		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(2, 1, 0, 3)));
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(3, 0, 1, 2)));
			break;


		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(3, 2, 1, 0)));
			break;
	}
}





//=============================================================================
//		NImage::Convert_B8_G8_R8_A8 : Convert a B8_G8_R8_A8 image.
//-----------------------------------------------------------------------------
void NImage::Convert_B8_G8_R8_A8(NImageFormat theFormat)
{


	// Convert the image
	switch (theFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Unable to convert image!");
			break;

		case NImageFormat::A8:
			ForEachRow(NBindSelf(NImage::RowReduce32To8, kNArg2, kNArg3, size_t(3)));
			break;

		case NImageFormat::R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(2, 1, 0)));
			break;

		case NImageFormat::B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(0, 1, 2)));
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(2, 1, 0, 3)));
			break;

		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
			// No-op
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(3, 2, 1, 0)));
			break;

		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(3, 0, 1, 2)));
			break;
	}
}





//=============================================================================
//		NImage::Convert_A8_R8_G8_B8 : Convert an A8_R8_G8_B8 image.
//-----------------------------------------------------------------------------
void NImage::Convert_A8_R8_G8_B8(NImageFormat theFormat)
{


	// Convert the image
	switch (theFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Unable to convert image!");
			break;

		case NImageFormat::A8:
			ForEachRow(NBindSelf(NImage::RowReduce32To8, kNArg2, kNArg3, size_t(0)));
			break;

		case NImageFormat::R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(1, 2, 3)));
			break;

		case NImageFormat::B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(3, 2, 1)));
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(1, 2, 3, 0)));
			break;

		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(3, 2, 1, 0)));
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
			// No-op
			break;

		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(0, 3, 2, 1)));
			break;
	}
}





//=============================================================================
//		NImage::Convert_A8_B8_G8_R8 : Convert an A8_B8_G8_R8 image.
//-----------------------------------------------------------------------------
void NImage::Convert_A8_B8_G8_R8(NImageFormat theFormat)
{


	// Convert the image
	switch (theFormat)
	{
		case NImageFormat::None:
			NN_LOG_ERROR("Unable to convert image!");
			break;

		case NImageFormat::A8:
			ForEachRow(NBindSelf(NImage::RowReduce32To8, kNArg2, kNArg3, size_t(0)));
			break;

		case NImageFormat::R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(3, 2, 1)));
			break;

		case NImageFormat::B8_G8_R8:
			ForEachRow(NBindSelf(NImage::RowReduce32To24, kNArg2, kNArg3, TAKE(1, 2, 3)));
			break;

		case NImageFormat::R8_G8_B8_A8:
		case NImageFormat::R8_G8_B8_X8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(3, 2, 1, 0)));
			break;

		case NImageFormat::B8_G8_R8_A8:
		case NImageFormat::B8_G8_R8_X8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(1, 2, 3, 0)));
			break;

		case NImageFormat::A8_R8_G8_B8:
		case NImageFormat::X8_R8_G8_B8:
			ForEachRow(NBindSelf(NImage::RowSwizzle32, kNArg2, kNArg3, TAKE(0, 3, 2, 1)));
			break;

		case NImageFormat::A8_B8_G8_R8:
		case NImageFormat::X8_B8_G8_R8:
			// No-op
			break;
	}
}





//=============================================================================
//		NImage::RowSwizzle24 : Swizzle a row of 24-bpp pixels.
//-----------------------------------------------------------------------------
bool NImage::RowSwizzle24(size_t theWidth, uint8_t* rowPtr, const NVectorSize& newOrder)
{


	// Validate our parameters
	NN_REQUIRE(newOrder.size() == 3);



	// Get the state we need
	uint8_t* pixelPtr = rowPtr;



	// Process the row
	for (size_t x = 0; x < theWidth; x++)
	{
		uint8_t tmpPixel[3];

		tmpPixel[0] = pixelPtr[0];
		tmpPixel[1] = pixelPtr[1];
		tmpPixel[2] = pixelPtr[2];

		pixelPtr[0] = tmpPixel[newOrder[0]];
		pixelPtr[1] = tmpPixel[newOrder[1]];
		pixelPtr[2] = tmpPixel[newOrder[2]];

		pixelPtr += 3;
	}

	return true;
}





//=============================================================================
//		NImage::RowSwizzle32 : Swizzle a row of 32-bpp pixels.
//-----------------------------------------------------------------------------
bool NImage::RowSwizzle32(size_t theWidth, uint8_t* rowPtr, const NVectorSize& newOrder)
{


	// Validate our parameters
	NN_REQUIRE(newOrder.size() == 4);



	// Get the state we need
	uint8_t* pixelPtr;
	pixelPtr = rowPtr;



	// Process the row
	for (size_t x = 0; x < theWidth; x++)
	{
		uint8_t tmpPixel[4];
		memcpy(&tmpPixel, pixelPtr, 4);

		pixelPtr[0] = tmpPixel[newOrder[0]];
		pixelPtr[1] = tmpPixel[newOrder[1]];
		pixelPtr[2] = tmpPixel[newOrder[2]];
		pixelPtr[3] = tmpPixel[newOrder[3]];

		pixelPtr += 4;
	}

	return true;
}





//=============================================================================
//		NImage::RowReduce32To24 : Reduce a 32bpp row to 24bpp.
//-----------------------------------------------------------------------------
bool NImage::RowReduce32To24(size_t theWidth, uint8_t* rowPtr, const NVectorSize& srcOrder)
{


	// Validate our parameters
	NN_REQUIRE(srcOrder.size() == 3);



	// Get the state we need
	const uint8_t* srcPixel = rowPtr;
	uint8_t*       dstPixel = rowPtr;



	// Process the row
	for (size_t x = 0; x < theWidth; x++)
	{
		uint8_t tmpPixel[4];
		memcpy(&tmpPixel, srcPixel, 4);

		dstPixel[0] = tmpPixel[srcOrder[0]];
		dstPixel[1] = tmpPixel[srcOrder[1]];
		dstPixel[2] = tmpPixel[srcOrder[2]];

		srcPixel += 4;
		dstPixel += 3;
	}

	return true;
}





//=============================================================================
//		NImage::RowReduce32To8 : Reduce a 32bpp row to 8bpp.
//-----------------------------------------------------------------------------
bool NImage::RowReduce32To8(size_t theWidth, uint8_t* rowPtr, size_t srcIndex)
{


	// Validate our parameters
	NN_REQUIRE(srcIndex <= 3);



	// Get the state we need
	const uint8_t* srcPixel = rowPtr;
	uint8_t*       dstPixel = rowPtr;



	// Process the row
	for (size_t x = 0; x < theWidth; x++)
	{
		dstPixel[0] = srcPixel[srcIndex];

		srcPixel += 4;
		dstPixel += 1;
	}

	return true;
}





//=============================================================================
//		NImage::RowExpand24To32 : Expand a 24bpp row into 32bpp.
//-----------------------------------------------------------------------------
bool NImage::RowExpand24To32(size_t             theWidth,
							 const uint8_t*     rowPtr,
							 const NVectorSize& dstOrder,
							 NImage*            dstImage,
							 size_t             y)
{


	// Validate our parameters
	NN_REQUIRE(dstOrder.size() == 4);
	NN_REQUIRE(dstImage->GetBytesPerPixel() == 4);
	NN_REQUIRE(dstImage->GetWidth() == theWidth);



	// Get the state we need
	const uint8_t* srcPixel = rowPtr;
	uint8_t*       dstPixel = dstImage->GetMutablePixels(0, y);



	// Process the row
	for (size_t x = 0; x < theWidth; x++)
	{
		dstPixel[dstOrder[0]] = srcPixel[0];
		dstPixel[dstOrder[1]] = srcPixel[1];
		dstPixel[dstOrder[2]] = srcPixel[2];
		dstPixel[dstOrder[3]] = 0xFF;

		srcPixel += 3;
		dstPixel += 4;
	}

	return true;
}





//=============================================================================
//		NImage::RowExpand8To32 : Expand an 8bpp row into 32bpp.
//-----------------------------------------------------------------------------
bool NImage::RowExpand8To32(size_t         theWidth,
							const uint8_t* rowPtr,
							size_t         dstIndex,
							NImage*        dstImage,
							size_t         y)
{


	// Validate our parameters
	NN_REQUIRE(dstImage->GetBytesPerPixel() == 4);
	NN_REQUIRE(dstImage->GetWidth() == theWidth);



	// Get the state we need
	const uint8_t* srcPixel = rowPtr;
	uint8_t*       dstPixel = dstImage->GetMutablePixels(0, y);



	// Process the row
	for (size_t x = 0; x < theWidth; x++)
	{
		dstPixel[dstIndex] = srcPixel[0];

		srcPixel += 1;
		dstPixel += 4;
	}

	return true;
}
