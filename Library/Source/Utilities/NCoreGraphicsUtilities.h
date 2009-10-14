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
#include "NData.h"
#include "NFile.h"
#include "NUTI.h"





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
															UInt32			rowBytes,
															UInt32			bitsPerComponent = 8,
															UInt32			bitsPerPixel     = 32,
															CGBitmapInfo	bitmapInfo       = kCGImageAlphaPremultipliedFirst,
															CGColorSpaceRef	colorSpace       = NCoreGraphicsUtilities::GetColorSpaceRGB());

	static CGImageRef					CreateImageFromData(const NData &theData, const NUTI &theUTI="");


	// Create a scaled image
	static CGImageRef					CreateScaledImage(CGImageRef cgImage, const CGSize &theSize);


	// Create a rotated image
	//
	// Rotations must be +90/-90 degrees.
	static CGImageRef					CreateRotatedImage(CGImageRef cgImage, float rotateBy);


	// Create a flipped image
	static CGImageRef					CreateFlippedImage(CGImageRef cgImage, bool flipHoriz);


	// Create an IconRef from an image
	static IconRef						CreateIconFromImage(CGImageRef cgImage);


	// Create appropriately-encoded data from an image
	static NData						CreateDataFromImage(CGImageRef cgImage, const NUTI &theUTI);


private:
	static void							DataProviderRelease(void *info, const void *data, size_t size);


private:


};




#endif // NCOREGRAPHICSUTILITIES_HDR


