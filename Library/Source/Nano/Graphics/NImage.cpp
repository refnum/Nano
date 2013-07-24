/*	NAME:
		NImage.cpp

	DESCRIPTION:
		Image buffer.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NSTLUtilities.h"
#include "NTargetSystem.h"
#include "NTargetMath.h"
#include "NImage.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
#define SELECT														nvector<NIndex>





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(const NData &theData)
{	NStatus		theErr;



	// Initialize ourselves
	Clear();



	// Decode the image
	theErr = Decode(theData);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(const NFile &theFile)
{	NStatus		theErr;



	// Initialize ourselves
	Clear();



	// Load the image
	theErr = Load(theFile);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(const NSize &theSize, NImageFormat theFormat, const NData &theData, NIndex rowBytes)
{	NIndex		packedRow;



	// Validate our parameters
	NN_ASSERT(!theSize.IsEmpty());



	// Initialize ourselves
	Clear();

	mSize   = theSize;
	mFormat = theFormat;



	// Create the image
	packedRow = (NIndex) (mSize.width * GetBytesPerPixel());

	if (theData.IsEmpty())
		{
		mData.SetSize((NIndex) (mSize.height * packedRow));
		mRowBytes = packedRow;
		}
	else
		{
		mData     = theData;
		mRowBytes = (rowBytes != 0) ? rowBytes : packedRow;
		}

	NN_ASSERT(!mData.IsEmpty());
}





//============================================================================
//		NImage::NImage : Constructor.
//----------------------------------------------------------------------------
NImage::NImage(void)
{


	// Initialize ourselves
	Clear();
}





//============================================================================
//		NImage::~NImage : Destructor.
//----------------------------------------------------------------------------
NImage::~NImage(void)
{
}





//============================================================================
//		NImage::IsValid : Is the image valid?
//----------------------------------------------------------------------------
bool NImage::IsValid(void) const
{


	// Check our state
	return(mFormat != kNImageFormatNone);
}





//============================================================================
//		NImage::Clear : Clear the image.
//----------------------------------------------------------------------------
void NImage::Clear(void)
{


	// Reset our state
	mSize.Clear();
	mData.Clear();

	mFormat   = kNImageFormatNone;
	mRowBytes = 0;
}





//============================================================================
//		NImage::ForEachPixel : Process each pixel.
//----------------------------------------------------------------------------
void NImage::ForEachPixel(const NImageForEachImmutablePixelFunctor &theFunctor) const
{


	// Process each pixel
	ForEachRow(BindSelf(NImage::ForEachPixelInImmutableRow, kNArg1, kNArg2, GetBytesPerPixel(), theFunctor, kNArg3));
}





//============================================================================
//		NImage::ForEachPixel : Process each pixel.
//----------------------------------------------------------------------------
void NImage::ForEachPixel(const NImageForEachMutablePixelFunctor &theFunctor)
{


	// Process each pixel
	ForEachRow(BindSelf(NImage::ForEachPixelInMutableRow, kNArg1, kNArg2, GetBytesPerPixel(), theFunctor, kNArg3));
}





//============================================================================
//		NImage::ForEachRow : Process each row.
//----------------------------------------------------------------------------
void NImage::ForEachRow(const NImageForEachImmutableRowFunctor &theFunctor) const
{	NIndex			y, theWidth, theHeight, rowBytes;
	const uint8_t	*rowPtr;



	// Get the state we need
	theWidth   = GetWidth();
	theHeight  = GetHeight();
	rowPtr     = GetPixels();
	rowBytes   = GetBytesPerRow();



	// Process the rows
	for (y = 0; y < theHeight; y++)
		{
		if (!theFunctor(y, theWidth, rowPtr))
			return;

		rowPtr += rowBytes;
		}
}





//============================================================================
//		NImage::ForEachRow : Process each row.
//----------------------------------------------------------------------------
void NImage::ForEachRow(const NImageForEachMutableRowFunctor &theFunctor)
{	NIndex			y, theWidth, theHeight, rowBytes;
	uint8_t			*rowPtr;



	// Get the state we need
	theWidth   = GetWidth();
	theHeight  = GetHeight();
	rowPtr     = GetPixels();
	rowBytes   = GetBytesPerRow();



	// Process the rows
	for (y = 0; y < theHeight; y++)
		{
		if (!theFunctor(y, theWidth, rowPtr))
			return;

		rowPtr += rowBytes;
		}
}





//============================================================================
//		NImage::GetWidth : Get the width.
//----------------------------------------------------------------------------
NIndex NImage::GetWidth(void) const
{


	// Get the width
	return((NIndex) mSize.width);
}





//============================================================================
//		NImage::GetHeight : Get the height.
//----------------------------------------------------------------------------
NIndex NImage::GetHeight(void) const
{


	// Get the height
	return((NIndex) mSize.height);
}





//============================================================================
//		NImage::GetSize : Get the size.
//----------------------------------------------------------------------------
NSize NImage::GetSize(void) const
{


	// Get the size
	return(mSize);
}





//============================================================================
//		NImage::GetBounds : Get the bounds.
//----------------------------------------------------------------------------
NRectangle NImage::GetBounds(void) const
{


	// Get the bounds
	return(NRectangle(mSize));
}





//============================================================================
//		NImage::GetFormat : Get the format.
//----------------------------------------------------------------------------
NImageFormat NImage::GetFormat(void) const
{


	// Get the format
	return(mFormat);
}





//============================================================================
//		NImage::SetFormat : Set the format.
//----------------------------------------------------------------------------
void NImage::SetFormat(NImageFormat theFormat)
{


	// Convert the image
	switch (mFormat) {
		case kNImageFormat_RGB_888:
			Convert_RGB_888(theFormat);
			break;

		case kNImageFormat_BGR_888:
			Convert_BGR_888(theFormat);
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
			Convert_RGBA_8888(theFormat);
			break;

		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
			Convert_ARGB_8888(theFormat);
			break;

		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			Convert_BGRA_8888(theFormat);
			break;

		default:
			NN_LOG("Unknown image format: %ld", theFormat);
			return;
			break;
		}



	// Update our state
	mFormat = theFormat;
}





//============================================================================
//		NImage::GetBitsPerPixel : Get the bits-per-pixel.
//----------------------------------------------------------------------------
NIndex NImage::GetBitsPerPixel(void) const
{	NIndex		theValue;



	// Get the value
	switch (mFormat) {
		case kNImageFormat_RGB_888:
		case kNImageFormat_BGR_888:
			theValue = 24;
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			theValue = 32;
			break;
		
		default:
			NN_LOG("Invalid image format: %ld", mFormat);
			theValue = 0;
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NImage::GetBitsPerComponent : Get the bits-per-component.
//----------------------------------------------------------------------------
NIndex NImage::GetBitsPerComponent(void) const
{	NIndex		theValue;



	// Get the value
	switch (mFormat) {
		case kNImageFormat_RGB_888:
		case kNImageFormat_BGR_888:
			theValue = 8;
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			theValue = 8;
			break;

		default:
			NN_LOG("Invalid image format: %ld", mFormat);
			theValue = 0;
			break;
		}
	
	return(theValue);
}





//============================================================================
//		NImage::GetBytesPerPixel : Get the bytes-per-pixel.
//----------------------------------------------------------------------------
NIndex NImage::GetBytesPerPixel(void) const
{	NIndex	theValue;



	// Get the value
	theValue = (GetBitsPerPixel() / 8);
	NN_ASSERT((theValue * 8) == GetBitsPerPixel());
	
	return(theValue);
}





//============================================================================
//		NImage::GetBytesPerRow : Get the bytes-per-row.
//----------------------------------------------------------------------------
NIndex NImage::GetBytesPerRow(void) const
{


	// Get the value
	return(mRowBytes);
}





//============================================================================
//		NImage::GetPixels : Get the pixels.
//----------------------------------------------------------------------------
const uint8_t *NImage::GetPixels(NIndex x, NIndex y) const
{	NIndex			theOffset;
	const uint8_t	*thePtr;



	// Validate our parameters
	NN_ASSERT(x >= 0 && x <= (NIndex) mSize.width);
	NN_ASSERT(y >= 0 && y <= (NIndex) mSize.height);



	// Get the pixels
	theOffset = (y * GetBytesPerRow()) + (x * GetBytesPerPixel());
	thePtr    = mData.GetData(theOffset);

	return(thePtr);
}





//============================================================================
//		NImage::GetPixels : Get the pixels.
//----------------------------------------------------------------------------
uint8_t *NImage::GetPixels(NIndex x, NIndex y)
{	NIndex		theOffset;
	uint8_t		*thePtr;



	// Validate our parameters
	NN_ASSERT(x >= 0 && x <= (NIndex) mSize.width);
	NN_ASSERT(y >= 0 && y <= (NIndex) mSize.height);



	// Get the pixels
	theOffset = (y * GetBytesPerRow()) + (x * GetBytesPerPixel());
	thePtr    = mData.GetData(theOffset);

	return(thePtr);
}





//============================================================================
//		NImage::GetData : Get the data.
//----------------------------------------------------------------------------
NData NImage::GetData(void) const
{


	// Get the data
	return(mData);
}





//============================================================================
//		NImage::Load : Load an image.
//----------------------------------------------------------------------------
NStatus NImage::Load(const NFile &theFile, NImageFormat theFormat)
{	NData		theData;
	NStatus		theErr;



	// Load the image
	theData = NFileUtilities::GetFileData(theFile);
	theErr  = Decode(theData, theFormat);
	
	return(theErr);
}





//============================================================================
//		NImage::Save : Save an image.
//----------------------------------------------------------------------------
NStatus NImage::Save(const NFile &theFile, const NUTI &theType) const
{	NUTI		imageType;
	NData		theData;
	NStatus		theErr;



	// Get the state we need
	imageType = theType;

	if (!imageType.IsValid())
		imageType = theFile.GetUTI();

	if (!imageType.IsValid())
		imageType = kNUTTypePNG;



	// Save the image
	theData = Encode(imageType);
	theErr  = kNErrExhaustedSrc;

	if (!theData.IsEmpty())
		theErr = NFileUtilities::SetFileData(theFile, theData);
	
	return(theErr);
}





//============================================================================
//		NImage::Encode : Encode an image.
//----------------------------------------------------------------------------
NData NImage::Encode(const NUTI &theType) const
{


	// Encode the image
	return(NTargetSystem::ImageEncode(*this, theType));
}





//============================================================================
//		NImage::Decode : Decode an image.
//----------------------------------------------------------------------------
NStatus NImage::Decode(const NData &theData, NImageFormat theFormat)
{	NImage		theImage;



	// Decode the image
	theImage = NTargetSystem::ImageDecode(theData);
	if (!theImage.IsValid())
		return(kNErrMalformed);



	// Update our state
	*this = theImage;

	if (theFormat != kNImageFormatNone)
		SetFormat(theFormat);
	
	return(kNoErr);
}




#pragma mark private
//============================================================================
//		NImage::ForEachPixelInImmutableRow : Process each pixel in a row.
//----------------------------------------------------------------------------
bool NImage::ForEachPixelInImmutableRow(NIndex y, NIndex theWidth, NIndex pixelBytes, const NImageForEachImmutablePixelFunctor &theFunctor, const uint8_t *rowPtr) const
{	NIndex		x;



	// Process the row
	for (x = 0; x < theWidth; x++)
		{
		if (!theFunctor(x, y, rowPtr))
			return(false);

		rowPtr += pixelBytes;
		}

	return(true);
}





//============================================================================
//		NImage::ForEachPixelInMutableRow : Process each pixel in a row.
//----------------------------------------------------------------------------
bool NImage::ForEachPixelInMutableRow(NIndex y, NIndex theWidth, NIndex pixelBytes,  const NImageForEachMutablePixelFunctor &theFunctor, uint8_t *rowPtr)
{	NIndex		x;



	// Process the row
	for (x = 0; x < theWidth; x++)
		{
		if (!theFunctor(x, y, rowPtr))
			return(false);

		rowPtr += pixelBytes;
		}

	return(true);
}





//============================================================================
//		NImage::Convert_RGB_888 : Convert an RGB_888 image.
//----------------------------------------------------------------------------
void NImage::Convert_RGB_888(NImageFormat theFormat)
{	NImage		tmpImage;



	// Convert the image
	switch (theFormat) {
		case kNImageFormat_RGB_888:
			// No-op
			break;

		case kNImageFormat_BGR_888:
			ForEachRow(BindSelf(NImage::RowSwizzle24,		kNArg2, kNArg3, SELECT(2, 1, 0)));
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
			tmpImage = NImage(GetSize(), kNImageFormat_RGBA_8888);
			ForEachRow(BindSelf(NImage::RowExpand24To32,	kNArg2, kNArg3, SELECT(0, 1, 2, 3), &tmpImage, kNArg1));
			*this = tmpImage;
			break;

		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
			tmpImage = NImage(GetSize(), kNImageFormat_ARGB_8888);
			ForEachRow(BindSelf(NImage::RowExpand24To32,	kNArg2, kNArg3, SELECT(3, 0, 1, 2), &tmpImage, kNArg1));
			*this = tmpImage;
			break;

		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			tmpImage = NImage(GetSize(), kNImageFormat_BGRA_8888);
			ForEachRow(BindSelf(NImage::RowExpand24To32,	kNArg2, kNArg3, SELECT(2, 1, 0, 3), &tmpImage, kNArg1));
			*this = tmpImage;
			break;

		default:
			NN_LOG("Unable to convert image from %ld to %ld", mFormat, theFormat);
			break;
		}
}





//============================================================================
//		NImage::Convert_BGR_888 : Convert a BGR_888 image.
//----------------------------------------------------------------------------
void NImage::Convert_BGR_888(NImageFormat theFormat)
{	NImage		tmpImage;



	// Convert the image
	switch (theFormat) {
		case kNImageFormat_RGB_888:
			ForEachRow(BindSelf(NImage::RowSwizzle24,		kNArg2, kNArg3, SELECT(2, 1, 0)));
			break;

		case kNImageFormat_BGR_888:
			// No-op
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
			tmpImage = NImage(GetSize(), kNImageFormat_RGBA_8888);
			ForEachRow(BindSelf(NImage::RowExpand24To32,	kNArg2, kNArg3, SELECT(2, 1, 0, 3), &tmpImage, kNArg1));
			*this = tmpImage;
			break;

		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
			tmpImage = NImage(GetSize(), kNImageFormat_ARGB_8888);
			ForEachRow(BindSelf(NImage::RowExpand24To32,	kNArg2, kNArg3, SELECT(3, 2, 1, 0), &tmpImage, kNArg1));
			*this = tmpImage;
			break;

		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			tmpImage = NImage(GetSize(), kNImageFormat_BGRA_8888);
			ForEachRow(BindSelf(NImage::RowExpand24To32,	kNArg2, kNArg3, SELECT(0, 1, 2, 3), &tmpImage, kNArg1));
			*this = tmpImage;
			break;

		default:
			NN_LOG("Unable to convert image from %ld to %ld", mFormat, theFormat);
			break;
		}
}





//============================================================================
//		NImage::Convert_RGBA_8888 : Convert an RGBA_8888 image.
//----------------------------------------------------------------------------
void NImage::Convert_RGBA_8888(NImageFormat theFormat)
{


	// Convert the image
	switch (theFormat) {
		case kNImageFormat_RGB_888:
			ForEachRow(BindSelf(NImage::RowReduce32To24,	kNArg2, kNArg3, SELECT(0, 1, 2)));
			break;

		case kNImageFormat_BGR_888:
			ForEachRow(BindSelf(NImage::RowReduce32To24,	kNArg2, kNArg3, SELECT(2, 1, 0)));
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
			// No-op
			break;

		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
			ForEachRow(BindSelf(NImage::RowSwizzle32,		kNArg2, kNArg3, SELECT(3, 0, 1, 2)));
			break;

		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			ForEachRow(BindSelf(NImage::RowSwizzle32,		kNArg2, kNArg3, SELECT(2, 1, 0, 3)));
			break;

		default:
			NN_LOG("Unable to convert image from %ld to %ld", mFormat, theFormat);
			break;
		}	
}





//============================================================================
//		NImage::Convert_ARGB_8888 : Convert an ARGB_8888 image.
//----------------------------------------------------------------------------
void NImage::Convert_ARGB_8888(NImageFormat theFormat)
{


	// Convert the image
	switch (theFormat) {
		case kNImageFormat_RGB_888:
			ForEachRow(BindSelf(NImage::RowReduce32To24,	kNArg2, kNArg3, SELECT(1, 2, 3)));
			break;

		case kNImageFormat_BGR_888:
			ForEachRow(BindSelf(NImage::RowReduce32To24,	kNArg2, kNArg3, SELECT(3, 2, 1)));
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
			ForEachRow(BindSelf(NImage::RowSwizzle32,		kNArg2, kNArg3, SELECT(1, 2, 3, 0)));
			break;

		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
			// No-op
			break;

		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			ForEachRow(BindSelf(NImage::RowSwizzle32,		kNArg2, kNArg3, SELECT(3, 2, 1, 0)));
			break;

		default:
			NN_LOG("Unable to convert image from %ld to %ld", mFormat, theFormat);
			break;
		}
}





//============================================================================
//		NImage::Convert_BGRA_8888 : Convert a BGRA_8888 image.
//----------------------------------------------------------------------------
void NImage::Convert_BGRA_8888(NImageFormat theFormat)
{


	// Convert the image
	switch (theFormat) {
		case kNImageFormat_RGB_888:
			ForEachRow(BindSelf(NImage::RowReduce32To24,	kNArg2, kNArg3, SELECT(2, 1, 0)));
			break;

		case kNImageFormat_BGR_888:
			ForEachRow(BindSelf(NImage::RowReduce32To24,	kNArg2, kNArg3, SELECT(0, 1, 2)));
			break;

		case kNImageFormat_RGBX_8888:
		case kNImageFormat_RGBA_8888:
			ForEachRow(BindSelf(NImage::RowSwizzle32,		kNArg2, kNArg3, SELECT(2, 1, 0, 3)));
			break;

		case kNImageFormat_XRGB_8888:
		case kNImageFormat_ARGB_8888:
			ForEachRow(BindSelf(NImage::RowSwizzle32,		kNArg2, kNArg3, SELECT(3, 2, 1, 0)));
			break;

		case kNImageFormat_BGRX_8888:
		case kNImageFormat_BGRA_8888:
			// No-op
			break;

		default:
			NN_LOG("Unable to convert image from %ld to %ld", mFormat, theFormat);
			break;
		}
}





//============================================================================
//		NImage::RowSwizzle24 : Swizzle a row of 24-bpp pixels.
//----------------------------------------------------------------------------
bool NImage::RowSwizzle24(NIndex theWidth, uint8_t *rowPtr, const NIndexList &newOrder)
{	uint8_t		tmpPixel[3];
	uint8_t		*pixelPtr;
	NIndex		x;



	// Validate our parameters
	NN_ASSERT(newOrder.size() == 3);



	// Get the state we need
	pixelPtr = rowPtr;



	// Process the row
	for (x = 0; x < theWidth; x++)
		{
		tmpPixel[0] = pixelPtr[0];
		tmpPixel[1] = pixelPtr[1];
		tmpPixel[2] = pixelPtr[2];

		pixelPtr[0] = tmpPixel[newOrder[0]];
		pixelPtr[1] = tmpPixel[newOrder[1]];
		pixelPtr[2] = tmpPixel[newOrder[2]];

		pixelPtr += 3;
		}

	return(true);
}





//============================================================================
//		NImage::RowSwizzle32 : Swizzle a row of 32-bpp pixels.
//----------------------------------------------------------------------------
bool NImage::RowSwizzle32(NIndex theWidth, uint8_t *rowPtr, const NIndexList &newOrder)
{	uint8_t		tmpPixel[4];
	uint8_t		*pixelPtr;
	NIndex		x;



	// Validate our parameters
	NN_ASSERT(newOrder.size() == 4);



	// Get the state we need
	pixelPtr = rowPtr;



	// Process the row
	for (x = 0; x < theWidth; x++)
		{
		*((uint32_t *) &tmpPixel[0]) = *((uint32_t *) pixelPtr);

		pixelPtr[0] = tmpPixel[newOrder[0]];
		pixelPtr[1] = tmpPixel[newOrder[1]];
		pixelPtr[2] = tmpPixel[newOrder[2]];
		pixelPtr[3] = tmpPixel[newOrder[3]];

		pixelPtr += 4;
		}

	return(true);
}




//============================================================================
//		NImage::RowExpand24To32 : Expand a 24bpp row into 32bpp.
//----------------------------------------------------------------------------
bool NImage::RowExpand24To32(NIndex theWidth, const uint8_t *rowPtr, const NIndexList &dstOrder, NImage *dstImage, NIndex y)
{	const uint8_t	*srcPixel;
	uint8_t			*dstPixel;
	NIndex			x;



	// Validate our parameters
	NN_ASSERT(dstOrder.size()              == 4);
	NN_ASSERT(dstImage->GetBytesPerPixel() == 4);
	NN_ASSERT(dstImage->GetWidth()         == theWidth);



	// Get the state we need
	srcPixel = rowPtr;
	dstPixel = dstImage->GetPixels(0, y);



	// Process the row
	for (x = 0; x < theWidth; x++)
		{
		dstPixel[dstOrder[0]] = srcPixel[0];
		dstPixel[dstOrder[1]] = srcPixel[1];
		dstPixel[dstOrder[2]] = srcPixel[2];
		dstPixel[dstOrder[3]] = 0xFF;

		srcPixel += 3;
		dstPixel += 4;
		}

	return(true);
}





//============================================================================
//		NImage::RowReduce32To24 : Reduce a 32bpp row to 24bpp.
//----------------------------------------------------------------------------
bool NImage::RowReduce32To24(NIndex theWidth, uint8_t *rowPtr, const NIndexList &srcOrder)
{	uint8_t			tmpPixel[4];
	const uint8_t	*srcPixel;
	uint8_t			*dstPixel;
	NIndex			x;



	// Validate our parameters
	NN_ASSERT(srcOrder.size() == 3);



	// Get the state we need
	srcPixel = rowPtr;
	dstPixel = rowPtr;



	// Process the row
	for (x = 0; x < theWidth; x++)
		{
		*((uint32_t *) &tmpPixel[0]) = *((uint32_t *) srcPixel);

		dstPixel[0] = tmpPixel[srcOrder[0]];
		dstPixel[1] = tmpPixel[srcOrder[1]];
		dstPixel[2] = tmpPixel[srcOrder[2]];

		srcPixel += 4;
		dstPixel += 3;
		}

	return(true);
}

