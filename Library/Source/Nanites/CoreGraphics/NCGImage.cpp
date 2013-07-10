/*	NAME:
		NCGImage.cpp

	DESCRIPTION:
		CoreGraphics image.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphics.h"
#include "NCFData.h"
#include "NCGColor.h"
#include "NCGImage.h"





//============================================================================
//		NCGImage::NCGImage : Constructor.
//----------------------------------------------------------------------------
NCGImage::NCGImage(const NImage &theImage)
			: NImage(theImage)
{
}





//============================================================================
//		NCGImage::NCGImage : Constructor.
//----------------------------------------------------------------------------
NCGImage::NCGImage(CGImageRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCGImage::NCGImage : Constructor.
//----------------------------------------------------------------------------
NCGImage::NCGImage(void)
{
}





//============================================================================
//		NCGImage::~NCGImage : Destructor.
//----------------------------------------------------------------------------
NCGImage::~NCGImage(void)
{
}





//============================================================================
//		NCGImage::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCGImage::GetObject(bool cloneData) const
{	NCFObject			cgDataProvider, theObject;
	CGBitmapInfo		bitmapInfo;
	NCFData				theData;



	// Get the state we need
	bitmapInfo = GetBitmapInfo(GetFormat());
	theData    = GetData();



	// Prepare the data
	if (cloneData)
		cgDataProvider.SetObject(CGDataProviderCreateWithCFData(theData.GetObject()));
	else
		cgDataProvider.SetObject(CGDataProviderCreateWithData(NULL, theData.GetData(), theData.GetSize(), NULL));

	if (!cgDataProvider.IsValid())
		return(theObject);



	// Get the object
	theObject.SetObject(CGImageCreate(	GetWidth(),
										GetHeight(),
										GetBitsPerComponent(),
										GetBitsPerPixel(),
										GetBytesPerRow(),
										NCGColor::GetDeviceRGB(),
										bitmapInfo, cgDataProvider,
										NULL, false, kCGRenderingIntentDefault));

	return(theObject);
}





//============================================================================
//		NCGImage::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCGImage::SetObject(CGImageRef cfObject, bool takeOwnership)
{	size_t				theWidth, theHeight, bitsPerPixel, bitsPerComponent;
	NCFObject			theObject(cfObject, takeOwnership);
	NCFObject			cgColorSpace, cgContext;
	CGBitmapInfo		bitmapInfo;
	NImageFormat		theFormat;
	NImage				theImage;
	bool				isValid;



	// Get the state we need
	theWidth         = CGImageGetWidth(           cfObject);
	theHeight        = CGImageGetHeight(          cfObject);
	bitsPerPixel     = CGImageGetBitsPerPixel(    cfObject);
	bitsPerComponent = CGImageGetBitsPerComponent(cfObject);



	// Select the image format
	if (bitsPerPixel == 8 && bitsPerComponent == 8)
		{
		// Convert indexed images to 32bpp without alpha
		theFormat    = kNImageFormat_RGBX_8888;
		bitmapInfo   = kCGBitmapByteOrder32Big | kCGImageAlphaNoneSkipLast;
		cgColorSpace = NCGColor::GetDeviceRGB();
		}

	else if (bitsPerPixel == 24 && bitsPerComponent == 8)
		{
		// Convert 24bpp images to 32bpp without alpha
		theFormat    = kNImageFormat_RGBX_8888;
		bitmapInfo   = kCGBitmapByteOrder32Big | kCGImageAlphaNoneSkipLast;
		cgColorSpace = NCGColor::GetDeviceRGB();
		}

	else if (bitsPerPixel == 32 && bitsPerComponent == 8)
		{
		// Convert 32bpp images to 32bpp with alpha
		theFormat    = kNImageFormat_RGBA_8888;
		bitmapInfo   = kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast;
		cgColorSpace = NCGColor::GetDeviceRGB();
		}

	else
		{
		NN_LOG("Unknown image format: %d/%d", bitsPerPixel, bitsPerComponent);
		theFormat  = kNImageFormatNone;
		bitmapInfo = kCGImageAlphaNone;
		}

	if (!cgColorSpace.IsValid())
		return(false);



	// Set the object
	theImage = NImage(NSize(theWidth, theHeight), theFormat);
	isValid  = cgContext.SetObject(CGBitmapContextCreate(	theImage.GetPixels(),
															theWidth,
															theHeight,
															bitsPerComponent,
															theImage.GetBytesPerRow(),
															cgColorSpace,
															bitmapInfo));

	if (isValid)
		{
		CGContextDrawImage(cgContext, ToCG(theImage.GetBounds()), cfObject);
		*this = theImage;
		}

	return(isValid);
}





//============================================================================
//		NCGImage::GetBitmapInfo : Get a CGBitmapInfo.
//----------------------------------------------------------------------------
CGBitmapInfo NCGImage::GetBitmapInfo(NImageFormat theFormat)
{	CGBitmapInfo	bitmapInfo;



	// Get the bitmap info
	bitmapInfo = 0;
	
	switch (theFormat) {
		case kNImageFormat_RGB_888:
			bitmapInfo = kCGBitmapByteOrder32Big	| kCGImageAlphaNone;
			break;

		case kNImageFormat_BGR_888:
			bitmapInfo = kCGBitmapByteOrder32Little	| kCGImageAlphaNone;
			break;

		case kNImageFormat_RGBX_8888:
			bitmapInfo = kCGBitmapByteOrder32Big	| kCGImageAlphaNoneSkipLast;
			break;

		case kNImageFormat_RGBA_8888:
			bitmapInfo = kCGBitmapByteOrder32Big	| kCGImageAlphaPremultipliedLast;
			break;

		case kNImageFormat_XRGB_8888:
			bitmapInfo = kCGBitmapByteOrder32Big	| kCGImageAlphaNoneSkipFirst;
			break;

		case kNImageFormat_ARGB_8888:
			bitmapInfo = kCGBitmapByteOrder32Big	| kCGImageAlphaPremultipliedFirst;
			break;

		case kNImageFormat_BGRX_8888:
			bitmapInfo = kCGBitmapByteOrder32Little	| kCGImageAlphaNoneSkipFirst;
			break;

		case kNImageFormat_BGRA_8888:
			bitmapInfo = kCGBitmapByteOrder32Little	| kCGImageAlphaPremultipliedFirst;
			break;

		default:
			NN_LOG("Unknown image format: %d", theFormat);
			bitmapInfo = 0;
			break;
		}
	
	return(bitmapInfo);
}





