/*	NAME:
		NCoreGraphicsUtilities.h

	DESCRIPTION:
		CoreGraphics utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOREGRAPHICSUTILITIES_HDR
#define NCOREGRAPHICSUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NColor.h"
#include "NData.h"
#include "NFile.h"
#include "NUTI.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Shadow
typedef struct {
	NColor		color;
	float		blur;
	CGSize		offset;
} CGShadowInfo;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Shadows
static const CGShadowInfo kCGShadowNone								= { NColor(0.0f, 0.0f, 0.0f, 0.00f), 0.0f, CGSizeZero };
static const CGShadowInfo kCGShadowLight							= { NColor(0.0f, 0.0f, 0.0f, 0.25f), 2.0f, CGSizeZero };
static const CGShadowInfo kCGShadowDefault							= { NColor(0.0f, 0.0f, 0.0f, 0.33f), 2.5f, CGSizeZero };
static const CGShadowInfo kCGShadowMedium							= { NColor(0.0f, 0.0f, 0.0f, 0.50f), 3.0f, CGSizeZero };
static const CGShadowInfo kCGShadowHeavy							= { NColor(0.0f, 0.0f, 0.0f, 1.00f), 3.0f, CGSizeZero };





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCoreGraphicsUtilities {
public:
	// Get color space
	static CGColorSpaceRef				GetColorSpaceGray(void);
	static CGColorSpaceRef				GetColorSpaceRGB( void);


	// Create a data provider
	static CGDataProviderRef			CreateDataProvider(const NData &theData);
	static CGDataProviderRef			CreateDataProvider(const NFile &theFile);


	// Create an image from a file
	static CGImageRef					CreateImageFromFile(const NFile &theFile);


	// Create an image from data
	//
	// An image can be created from a buffer of raw data (with the specified dimensions
	// and format), or from buffer of data encoded in a particular format (if known).
	static CGImageRef					CreateImageFromData(const NData		&theData,
															UInt32			theWidth,
															UInt32			theHeight,
															UInt32			rowBytes         = 0,
															UInt32			bitsPerComponent = 8,
															UInt32			bitsPerPixel     = 32,
															CGBitmapInfo	bitmapInfo       = kCGImageAlphaPremultipliedFirst,
															CGColorSpaceRef	colorSpace       = NCoreGraphicsUtilities::GetColorSpaceRGB());

	static CGImageRef					CreateImageFromData(const NData &theData, const NUTI &theUTI=NUTI());


	// Create a scaled image
	static CGImageRef					CreateScaledImage(CGImageRef cgImage, const CGSize &theSize);


	// Create a rotated image
	//
	// Rotations must be +90/-90 degrees.
	static CGImageRef					CreateRotatedImage(CGImageRef cgImage, float rotateBy);


	// Create a flipped image
	static CGImageRef					CreateFlippedImage(CGImageRef cgImage, bool flipHoriz);


	// Create a premultiplied/demultiplied image
	static CGImageRef					CreatePremultipliedImage(CGImageRef cgImage);
	static CGImageRef					CreateDemultipliedImage( CGImageRef cgImage);


	// Create a reflection mask
	//
	// The reflection's alpha runs from alphaMax at the top to 0.0 at the bottom, scaled by alphaScale.
	static CGImageRef					CreateReflectionMask(const CGSize &theSize, float alphaMax=0.5f, float alphaScale=1.0f);


	// Create an image mask
	static CGImageRef					CreateImageMask(CGImageRef cgImage);
	

	// Create an IconRef from an image
	static IconRef						CreateIconFromImage(CGImageRef cgImage);


	// Create appropriately-encoded data from an image
	static NData						CreateDataFromImage(CGImageRef cgImage, const NUTI &theUTI);


	// Quarter-scale a 32-bpp image
	static void							ScaleQuarter8888(UInt32 srcWidth, UInt32 srcHeight, UInt32 srcRowBytes, const UInt8 *srcData,
														 UInt32 dstWidth, UInt32 dstHeight, UInt32 dstRowBytes,       UInt8 *dstData);


private:
	static void							DataProviderRelease(void *info, const void *data, size_t size);


private:


};




#endif // NCOREGRAPHICSUTILITIES_HDR


