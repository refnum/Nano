/*	NAME:
		NCGImage.cpp

	DESCRIPTION:
		CoreGraphics image.

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
#include "NCGImage.h"

// Nano
#include "NCFData.h"
#include "NCGColor.h"
#include "NCGContext.h"
#include "NCoreGraphics.h"





//=============================================================================
//		NCGImage::NCGImage : Constructor.
//-----------------------------------------------------------------------------
NCGImage::NCGImage(const NImage& theImage)
{


	// Initialise ourselves
	SetImage(theImage);
}





//=============================================================================
//		NCGImage::GetImage : Get the image.
//-----------------------------------------------------------------------------
NImage NCGImage::GetImage() const
{


	// Get the state we need
	CGImageRef cgImage = *this;

	size_t theWidth         = CGImageGetWidth(cgImage);
	size_t theHeight        = CGImageGetHeight(cgImage);
	size_t bitsPerPixel     = CGImageGetBitsPerPixel(cgImage);
	size_t bitsPerComponent = CGImageGetBitsPerComponent(cgImage);



	// Select the image format

	NImageFormat theFormat  = NImageFormat::None;
	CGBitmapInfo bitmapInfo = kCGImageAlphaNone;

	if (bitsPerPixel == 8 && bitsPerComponent == 8)
	{
		// Convert indexed images to 32bpp without alpha
		theFormat = NImageFormat::R8_G8_B8_X8;
		bitmapInfo =
			CGBitmapInfo(kCGBitmapByteOrder32Big) | CGBitmapInfo(kCGImageAlphaNoneSkipLast);
	}

	else if (bitsPerPixel == 24 && bitsPerComponent == 8)
	{
		// Convert 24bpp images to 32bpp without alpha
		theFormat = NImageFormat::R8_G8_B8_X8;
		bitmapInfo =
			CGBitmapInfo(kCGBitmapByteOrder32Big) | CGBitmapInfo(kCGImageAlphaNoneSkipLast);
	}

	else if (bitsPerPixel == 32 && bitsPerComponent == 8)
	{
		// Convert 32bpp images to 32bpp with alpha
		theFormat = NImageFormat::R8_G8_B8_A8;
		bitmapInfo =
			CGBitmapInfo(kCGBitmapByteOrder32Big) | CGBitmapInfo(kCGImageAlphaPremultipliedLast);
	}

	else
	{
		NN_LOG_ERROR("Unknown image format:{}/{}", bitsPerPixel, bitsPerComponent);
	}



	// Draw the image
	if (theFormat != NImageFormat::None)
	{
		NImage     theImage(NSize(theWidth, theHeight), theFormat);
		NCGContext cgContext;

		if (cgContext.Set(CGBitmapContextCreate(theImage.GetMutablePixels(),
												theWidth,
												theHeight,
												bitsPerComponent,
												theImage.GetBytesPerRow(),
												NCGColor::GetDeviceRGB(),
												bitmapInfo)))
		{
			CGContextDrawImage(cgContext, ToCG(theImage.GetBounds()), cgImage);
			return theImage;
		}
	}

	return NImage();
}





//=============================================================================
//		NCGImage::SetImage : Set the image.
//-----------------------------------------------------------------------------
bool NCGImage::SetImage(const NImage& theImage, NCFSource theSource)
{


	// Get the state we need
	CGBitmapInfo bitmapInfo = GetBitmapInfo(theImage.GetFormat());
	NData        theData(theImage.GetData());



	// Get the image
	NCGDataProvider cgDataProvider;
	bool            wasOK = false;

	if (theSource == NCFSource::Copy)
	{
		wasOK = cgDataProvider.Set(CGDataProviderCreateWithCFData(NCFData(theData)));
	}
	else
	{
		wasOK = cgDataProvider.Set(
			CGDataProviderCreateWithData(nullptr, theData.GetData(), theData.GetSize(), nullptr));
	}

	if (wasOK)
	{
		wasOK = Set(CGImageCreate(theImage.GetWidth(),
								  theImage.GetHeight(),
								  theImage.GetBitsPerComponent(),
								  theImage.GetBitsPerPixel(),
								  theImage.GetBytesPerRow(),
								  NCGColor::GetDeviceRGB(),
								  bitmapInfo,
								  cgDataProvider,
								  nullptr,
								  false,
								  kCGRenderingIntentDefault));
	}

	return wasOK;
}





//=============================================================================
//		NCGImage::GetBitmapInfo : Convert an NImageFormat to a CGBitmapInfo.
//-----------------------------------------------------------------------------
CGBitmapInfo NCGImage::GetBitmapInfo(NImageFormat theFormat)
{


	// Get the bitmap info
	CGBitmapInfo bitmapInfo = 0;

	switch (theFormat)
	{
		case NImageFormat::None:
		case NImageFormat::A8:
			NN_LOG_UNIMPLEMENTED();
			break;

		case NImageFormat::R8_G8_B8:
			bitmapInfo = CGBitmapInfo(kCGBitmapByteOrder32Big) | CGBitmapInfo(kCGImageAlphaNone);
			break;

		case NImageFormat::B8_G8_R8:
			bitmapInfo = CGBitmapInfo(kCGBitmapByteOrder32Little) | CGBitmapInfo(kCGImageAlphaNone);
			break;

		case NImageFormat::R8_G8_B8_X8:
			bitmapInfo =
				CGBitmapInfo(kCGBitmapByteOrder32Big) | CGBitmapInfo(kCGImageAlphaNoneSkipLast);
			break;

		case NImageFormat::B8_G8_R8_X8:
			bitmapInfo =
				CGBitmapInfo(kCGBitmapByteOrder32Little) | CGBitmapInfo(kCGImageAlphaNoneSkipFirst);
			break;

		case NImageFormat::X8_R8_G8_B8:
			bitmapInfo =
				CGBitmapInfo(kCGBitmapByteOrder32Big) | CGBitmapInfo(kCGImageAlphaNoneSkipFirst);
			break;

		case NImageFormat::X8_B8_G8_R8:
			bitmapInfo =
				CGBitmapInfo(kCGBitmapByteOrder32Little) | CGBitmapInfo(kCGImageAlphaNoneSkipLast);
			break;

		case NImageFormat::R8_G8_B8_A8:
			bitmapInfo = CGBitmapInfo(kCGBitmapByteOrder32Big) |
						 CGBitmapInfo(kCGImageAlphaPremultipliedLast);
			break;

		case NImageFormat::B8_G8_R8_A8:
			bitmapInfo = CGBitmapInfo(kCGBitmapByteOrder32Little) |
						 CGBitmapInfo(kCGImageAlphaPremultipliedFirst);
			break;

		case NImageFormat::A8_R8_G8_B8:
			bitmapInfo = CGBitmapInfo(kCGBitmapByteOrder32Big) |
						 CGBitmapInfo(kCGImageAlphaPremultipliedFirst);
			break;

		case NImageFormat::A8_B8_G8_R8:
			bitmapInfo = CGBitmapInfo(kCGBitmapByteOrder32Little) |
						 CGBitmapInfo(kCGImageAlphaPremultipliedLast);
			break;
	}

	return bitmapInfo;
}
