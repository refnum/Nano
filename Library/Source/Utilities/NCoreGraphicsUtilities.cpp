/*	NAME:
		NCoreGraphicsUtilities.cpp

	DESCRIPTION:
		CoreGraphics utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <Accelerate/Accelerate.h>

#include "NCGContext.h"
#include "NDictionary.h"
#include "NFileUtilities.h"
#include "NMathUtilities.h"
#include "NQuickTimeUtilities.h"
#include "NCoreGraphicsUtilities.h"





//============================================================================
//		NCoreGraphicsUtilities::GetColorSpaceGray : Get a gray color space.
//----------------------------------------------------------------------------
CGColorSpaceRef NCoreGraphicsUtilities::GetColorSpaceGray(void)
{	static CGColorSpaceRef		sColorSpace = CGColorSpaceCreateDeviceGray();



	// Get the color space
	return(sColorSpace);
}





//============================================================================
//		NCoreGraphicsUtilities::GetColorSpaceRGB : Get an RGB color space.
//----------------------------------------------------------------------------
CGColorSpaceRef NCoreGraphicsUtilities::GetColorSpaceRGB(void)
{	static CGColorSpaceRef		sColorSpace = CGColorSpaceCreateDeviceRGB();



	// Get the color space
	return(sColorSpace);
}





//============================================================================
//		NCoreGraphicsUtilities::CreateDataProvider : Create a data provider.
//----------------------------------------------------------------------------
CGDataProviderRef NCoreGraphicsUtilities::CreateDataProvider(const NData &theData)
{	CGDataProviderRef	cgDataProvider;
	CFDataRef			cfData;



	// Retain the data
	//
	// Since we do not know how long the data provider will need access to
	// the data for, we need to retain the data and provide a release callback.
	cfData = theData;
	CFSafeRetain(cfData);



	// Create the provider
	cgDataProvider = CGDataProviderCreateWithData((void *) cfData, theData.GetData(), theData.GetSize(), DataProviderRelease);
	
	return(cgDataProvider);
}





//============================================================================
//		NCoreGraphicsUtilities::CreateDataProvider : Create a data provider.
//----------------------------------------------------------------------------
CGDataProviderRef NCoreGraphicsUtilities::CreateDataProvider(const NFile &theFile)
{	CGDataProviderRef	cgDataProvider;



	// Validate our parameters
	NN_ASSERT(theFile.IsFile());



	// Create the provider
	cgDataProvider = CGDataProviderCreateWithURL(theFile.GetURL());
	
	return(cgDataProvider);
}





//============================================================================
//		NCoreGraphicsUtilities::CreateImageFromFile : Create an image from a file.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateImageFromFile(const NFile &theFile)
{	NCFObject			cgDataProvider, cgImageSource;
	ComponentInstance	qtImporter;
	FSRef				theFSRef;
	OSType				dataType;
	Handle				dataRef;
	CGImageRef			cgImage;
	NUTI				theUTI;
	OSStatus			theErr;



	// Validate our parameters
	NN_ASSERT(theFile.IsFile());



	// Get the state we need
	cgImage = NULL;
	theUTI  = theFile.GetUTI();



	// Use a built-in provider
	//
	// We use built-in providers where possible, since this allows CG to
	// defer decompressing the image data until it is actually drawn.
	if (theUTI == kUTNTypeJPEG || theUTI == kUTNTypePNG)
		{
		if (cgDataProvider.Set(CreateDataProvider(theFile)))
			{
			if (theUTI == kUTNTypeJPEG)
				cgImage = CGImageCreateWithJPEGDataProvider(cgDataProvider, NULL, true, kCGRenderingIntentDefault);

			else if (theUTI == kUTNTypePNG)
				cgImage = CGImageCreateWithPNGDataProvider( cgDataProvider, NULL, true, kCGRenderingIntentDefault);
			}
		}



	// Or use ImageIO
	//
	// This is the preferred path for 10.4 and above.
	if (cgImage == NULL && CGImageSourceCreateWithURL != NULL)
		{
		if (cgImageSource.Set(CGImageSourceCreateWithURL(theFile.GetURL(), NULL)))
			cgImage = CGImageSourceCreateImageAtIndex(cgImageSource, 0, NULL);
		}



	// Or use QuickTime
	//
	// This is the fallback path for 10.3.
	if (cgImage == NULL)
		{
		theFSRef   = theFile.GetFSRef();
		qtImporter = NULL;
		dataRef    = NULL;

		theErr = QTNewDataReferenceFromFSRef(&theFSRef, 0, &dataRef, &dataType);
		
		if (theErr == noErr)
			theErr = GetGraphicsImporterForDataRef(dataRef, dataType, &qtImporter);

		if (theErr == noErr)
			{
			theErr = GraphicsImportCreateCGImage(qtImporter, &cgImage, kGraphicsImportCreateCGImageUsingCurrentSettings);
			NN_ASSERT_NOERR(theErr);
			}
		
		if (qtImporter != NULL)
			CloseComponent(qtImporter);
		
		if (dataRef != NULL)
			DisposeHandle(dataRef);
		}
	
	return(cgImage);
}





//============================================================================
//		NCoreGraphicsUtilities::CreateImageFromData : Create an image from data.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateImageFromData(const NData		&theData,
														UInt32			theWidth,
														UInt32			theHeight,
														UInt32			rowBytes,
														UInt32			bitsPerComponent,
														UInt32			bitsPerPixel,
														CGBitmapInfo	bitmapInfo,
														CGColorSpaceRef	colorSpace)
{	NCFObject		cgDataProvider;
	CGImageRef		cgImage;



	// Supply our defaults
	//
	// As per NCGContext, we map kCGBitmapByteOrderDefault to native endian-ness.
	if (rowBytes == 0)
		rowBytes = theWidth * (bitsPerPixel / 8);

	if ((bitmapInfo & kCGBitmapByteOrderMask) == kCGBitmapByteOrderDefault)
		bitmapInfo |= kCGBitmapByteOrder32Host;



	// Create the image
	cgImage = NULL;

	if (cgDataProvider.Set(CreateDataProvider(theData)))
		cgImage = CGImageCreate(theWidth, theHeight, bitsPerComponent, bitsPerPixel, rowBytes,
								colorSpace, bitmapInfo, cgDataProvider, NULL, true,
								kCGRenderingIntentDefault);

	return(cgImage);
}





//============================================================================
//		NCoreGraphicsUtilities::CreateImageFromData : Create an image from data.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateImageFromData(const NData &theData, const NUTI &theUTI)
{	NCFObject			cgDataProvider, cgImageSource;
	bool				isJPEG, isPNG;
	NDictionary			formatDict;
	const NData			*imageData;
	NData				pictData;
	NFile				theFile;
	CGImageRef			cgImage;
	OSStatus			theErr;



	// Validate our parameters
	NN_ASSERT(theData.IsNotEmpty());
	


	// Get the state we need
	//
	// ImageIO and QuickTime both require PICT files, not PICT data, and so
	// we may need to insert the standard zero-filled header for conversion.
	cgImage   = NULL;
	imageData = &theData;
	
	isJPEG = (theUTI == kUTNTypeJPEG);
	isPNG  = (theUTI == kUTNTypePNG);

	if (theUTI == kUTNTypePICT)
		{
		if (pictData.SetSize(kFileHeaderSizePICT) && pictData.AppendData(theData.GetSize(), theData.GetData()) != NULL)
			imageData = &pictData;
		}



	// Use a built-in provider
	//
	// We use built-in providers where possible, since this allows CG to
	// defer decompressing the image data until it is actually drawn.
	if (isJPEG || isPNG)
		{
		if (cgDataProvider.Set(CreateDataProvider(*imageData)))
			{
			if (isJPEG)
				cgImage = CGImageCreateWithJPEGDataProvider(cgDataProvider, NULL, true, kCGRenderingIntentDefault);

			else if (isPNG)
				cgImage = CGImageCreateWithPNGDataProvider( cgDataProvider, NULL, true, kCGRenderingIntentDefault);
			}
		}



	// Or use ImageIO
	//
	// This is the preferred path for 10.4 and above.
	if (cgImage == NULL && CGImageSourceCreateWithData != NULL)
		{
		formatDict.SetValueString(kCGImageSourceTypeIdentifierHint, theUTI);
		
		if (cgImageSource.Set(CGImageSourceCreateWithData(*imageData, formatDict)))
			cgImage = CGImageSourceCreateImageAtIndex(cgImageSource, 0, NULL);
		}



	// Or use QuickTime
	//
	// We could use a pointer data reference handle, but for now we just fall
	// back to a temporary file since we only need this for 10.3.
	if (cgImage == NULL)
		{
		theFile = NFileUtilities::GetTempFile();
		theErr  = NFileUtilities::SetFileData(theFile, *imageData);
			
		if (theErr == noErr)
			{
			theFile.SetExtension(theUTI.GetFileExtension(), true);
			cgImage = CreateImageFromFile(theFile);
			}
			
		theFile.Delete();
		}
	
	return(cgImage);
}





//============================================================================
//		NCoreGraphicsUtilities::CreateScaledImage : Create a scaled image.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateScaledImage(CGImageRef cgImage, const CGSize &theSize)
{	UInt32			srcWidth, srcHeight, dstWidth, dstHeight;
	NCGContext		srcContext, dstContext;
	NData			srcData, dstData;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);
	NN_ASSERT(theSize.width >= 1.0f && theSize.height >= 1.0f);



	// Get the state we need
	srcWidth  = (UInt32) CGImageGetWidth( cgImage);
	srcHeight = (UInt32) CGImageGetHeight(cgImage);
	
	dstWidth  = (UInt32) theSize.width;
	dstHeight = (UInt32) theSize.height;



	// Scale by 50%
	if (srcWidth == (dstWidth*2) && srcHeight == (dstHeight*2))
		{
		srcContext = NCGContext(srcData, srcWidth, srcHeight);
		dstContext = NCGContext(dstData, dstWidth, dstHeight);

		srcContext.DrawImage(cgImage, CGRectMake(0.0f, 0.0f, srcWidth, srcHeight));
		NCoreGraphicsUtilities::ScaleQuarter8888(srcWidth, srcHeight, srcWidth * sizeof(UInt32), srcData.GetData(),
												 dstWidth, dstHeight, dstWidth * sizeof(UInt32), dstData.GetData());
		}


	// Scale by arbitrary amount
	else
		{
		dstContext = NCGContext(dstData, dstWidth, dstHeight);
		dstContext.DrawImage(cgImage, CGRectMake(0.0f, 0.0f, dstWidth, dstHeight));
		}



	// Create a new image
	return(CreateImageFromData(dstData, dstWidth, dstHeight));
}





//============================================================================
//		NCoreGraphicsUtilities::CreateRotatedImage : Create a rotated image.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateRotatedImage(CGImageRef cgImage, float rotateBy)
{	NCGContext			contextSrc, contextDst;
	NData				dataSrc, dataDst;
	Pixel_8888			bgPixel = { 0 };
	vImage_Buffer		imgSrc, imgDst;
	uint8_t				theMode;
	HIRect				theRect;
	vImage_Error		vecErr;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);
	NN_ASSERT(NMathUtilities::AreEqual(fabs(rotateBy), 90.0f));



	// Get the state we need
	//
	// Since we always rotate by 90', the width/height are just swapped.
	theRect.origin      = kHIPointZero;
	theRect.size.height = CGImageGetHeight(cgImage);
	theRect.size.width  = CGImageGetWidth( cgImage);

	imgSrc.height   = (UInt32) theRect.size.height;
	imgSrc.width    = (UInt32) theRect.size.width;
	imgSrc.rowBytes = imgSrc.width * sizeof(UInt32);

	imgDst.height   = imgSrc.width;
	imgDst.width    = imgSrc.height;
	imgDst.rowBytes = imgDst.width * sizeof(UInt32);



	// Prepare the image buffers
	contextSrc = NCGContext(dataSrc, imgSrc.width, imgSrc.height);
	contextDst = NCGContext(dataDst, imgDst.width, imgDst.height);

	imgSrc.data = dataSrc.GetData();
	imgDst.data = dataDst.GetData();



	// Rotate the image
	contextSrc.DrawImage(cgImage, theRect);

	theMode = NMathUtilities::AreEqual(rotateBy, 90.0f) ? kRotate90DegreesCounterClockwise : kRotate90DegreesClockwise;
	vecErr  = vImageRotate90_ARGB8888(&imgSrc, &imgDst, theMode, bgPixel, kvImageNoFlags);
	NN_ASSERT_NOERR(vecErr);


	// Create a new image
	return(CreateImageFromData(dataDst, imgDst.width, imgDst.height, imgDst.rowBytes));
}





//============================================================================
//		NCoreGraphicsUtilities::CreateFlippedImage : Create a flipped image.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateFlippedImage(CGImageRef cgImage, bool flipHoriz)
{	NCGContext			contextSrc, contextDst;
	NData				dataSrc, dataDst;
	vImage_Buffer		imgSrc, imgDst;
	HIRect				theRect;
	vImage_Error		vecErr;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);



	// Get the state we need
	theRect.origin      = kHIPointZero;
	theRect.size.height = CGImageGetHeight(cgImage);
	theRect.size.width  = CGImageGetWidth( cgImage);

	imgSrc.height   = (UInt32) theRect.size.height;
	imgSrc.width    = (UInt32) theRect.size.width;
	imgSrc.rowBytes = imgSrc.width * sizeof(UInt32);

	imgDst = imgSrc;



	// Prepare the image buffers
	contextSrc = NCGContext(dataSrc, imgSrc.width, imgSrc.height);
	contextDst = NCGContext(dataDst, imgDst.width, imgDst.height);

	imgSrc.data = dataSrc.GetData();
	imgDst.data = dataDst.GetData();



	// Flip the image
	contextSrc.DrawImage(cgImage, theRect);

	if (flipHoriz)
		vecErr = vImageHorizontalReflect_ARGB8888(&imgSrc, &imgDst, kvImageNoFlags);
	else
		vecErr =   vImageVerticalReflect_ARGB8888(&imgSrc, &imgDst, kvImageNoFlags);

	NN_ASSERT_NOERR(vecErr);



	// Create a new image
	return(CreateImageFromData(dataDst, imgDst.width, imgDst.height, imgDst.rowBytes));
}





//============================================================================
//		NCoreGraphicsUtilities::CreatePremultipledImage : Create a premultiplied image.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreatePremultipliedImage(CGImageRef cgImage)
{	NCGContext			contextSrc, contextDst;
	NData				dataSrc, dataDst;
	vImage_Buffer		imgSrc, imgDst;
	HIRect				theRect;
	vImage_Error		vecErr;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);



	// Get the state we need
	theRect.origin      = kHIPointZero;
	theRect.size.height = CGImageGetHeight(cgImage);
	theRect.size.width  = CGImageGetWidth( cgImage);

	imgSrc.height   = (UInt32) theRect.size.height;
	imgSrc.width    = (UInt32) theRect.size.width;
	imgSrc.rowBytes = imgSrc.width * sizeof(UInt32);

	imgDst = imgSrc;



	// Prepare the image buffers
	contextSrc = NCGContext(dataSrc, imgSrc.width, imgSrc.height);
	contextDst = NCGContext(dataDst, imgDst.width, imgDst.height);

	imgSrc.data = dataSrc.GetData();
	imgDst.data = dataDst.GetData();



	// Flip the image
	contextSrc.DrawImage(cgImage, theRect);

	vecErr = vImagePremultiplyData_ARGB8888(&imgSrc, &imgDst, kvImageNoFlags);
	NN_ASSERT_NOERR(vecErr);



	// Create a new image
	return(CreateImageFromData(dataDst, imgDst.width, imgDst.height, imgDst.rowBytes));
}





//============================================================================
//		NCoreGraphicsUtilities::CreateDemultipledImage : Create a demultiplied image.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateDemultipliedImage(CGImageRef cgImage)
{	NCGContext			contextSrc, contextDst;
	NData				dataSrc, dataDst;
	vImage_Buffer		imgSrc, imgDst;
	HIRect				theRect;
	vImage_Error		vecErr;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);



	// Get the state we need
	theRect.origin      = kHIPointZero;
	theRect.size.height = CGImageGetHeight(cgImage);
	theRect.size.width  = CGImageGetWidth( cgImage);

	imgSrc.height   = (UInt32) theRect.size.height;
	imgSrc.width    = (UInt32) theRect.size.width;
	imgSrc.rowBytes = imgSrc.width * sizeof(UInt32);

	imgDst = imgSrc;



	// Prepare the image buffers
	contextSrc = NCGContext(dataSrc, imgSrc.width, imgSrc.height);
	contextDst = NCGContext(dataDst, imgDst.width, imgDst.height);

	imgSrc.data = dataSrc.GetData();
	imgDst.data = dataDst.GetData();



	// Flip the image
	contextSrc.DrawImage(cgImage, theRect);

	vecErr = vImageUnpremultiplyData_ARGB8888(&imgSrc, &imgDst, kvImageNoFlags);
	NN_ASSERT_NOERR(vecErr);



	// Create a new image
	return(CreateImageFromData(dataDst, imgDst.width, imgDst.height, imgDst.rowBytes));
}





//============================================================================
//      NCoreGraphicsUtilities::CreateReflectionMask : Create a reflection mask.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateReflectionMask(const CGSize &theSize, float alphaMax, float alphaScale)
{	UInt32		y, theWidth, theHeight, rowBytes, grayByte;
	float		rowAlpha, maxHeight;
	NCFObject	cgDataProvider;
	NData		theData;
	UInt8		*rowPtr;



	// Get the state we need
	theWidth  = (UInt32) theSize.width;
	theHeight = (UInt32) theSize.height;
	rowBytes  = theWidth * sizeof(UInt8);
	
	if (!theData.SetSize(theWidth * theHeight))
		return(NULL);



	// Draw the image
	//
	// Black pixels are opaque, white are transparent.
	rowPtr    = theData.GetData();
	maxHeight = theHeight * alphaScale;

	for (y = 0; y < theHeight; y++)
		{
		rowAlpha = ((float) y) / maxHeight;
		rowAlpha = (1.0f - alphaMax) + rowAlpha;

		rowAlpha = GET_CLAMPED_VALUE(rowAlpha, 0.0f, 1.0f);
		grayByte = (UInt8) (rowAlpha * 255.0f);

		memset(rowPtr, grayByte, theWidth);
		rowPtr += theWidth;
		}



	// Create the mask
	if (!cgDataProvider.Set(CreateDataProvider(theData)))
		return(NULL);

	return(CGImageMaskCreate(theWidth, theHeight, 8, 8, rowBytes, cgDataProvider, NULL, false));
}





//============================================================================
//      NCoreGraphicsUtilities::CreateImageMask : Create an image mask.
//----------------------------------------------------------------------------
CGImageRef NCoreGraphicsUtilities::CreateImageMask(CGImageRef cgImage)
{	UInt32			theWidth, theHeight, rowBytes;
	NCFObject		cgDataProvider;
	NCGContext		cgContext;
	CGRect			theRect;
	NData			theData;



	// Get the state we need
	theWidth  = (UInt32) CGImageGetWidth( cgImage);
	theHeight = (UInt32) CGImageGetHeight(cgImage);
	theRect   = CGRectMake(0.0f, 0.0f, theWidth, theHeight);
	rowBytes  = theWidth * sizeof(UInt8);

	if (!theData.SetSize(rowBytes * theHeight))
		return(NULL);



	// Draw the image
	//
	// Black pixels are opaque, white are transparent. Since we composite the
	// image onto a white background, clear areas in the image remain transparent.
	if (!cgContext.Set(CGBitmapContextCreate(theData.GetData(), theWidth, theHeight, 8, rowBytes, GetColorSpaceGray(), kCGImageAlphaNone)))
		return(NULL);

	cgContext.SetFillColor(kColorWhite);
	cgContext.FillRect(theRect);
	cgContext.DrawImage(cgImage, theRect);



	// Create the mask
	if (!cgDataProvider.Set(CreateDataProvider(theData)))
		return(NULL);

	return(CGImageMaskCreate(theWidth, theHeight, 8, 8, rowBytes, cgDataProvider, NULL, false));
}





//============================================================================
//      NCoreGraphicsUtilities::CreateIconFromImage : Create an icon from an image.
//----------------------------------------------------------------------------
IconRef NCoreGraphicsUtilities::CreateIconFromImage(CGImageRef cgImage)
{   UInt32              dataRGB[128 * 128] = { 0 };
    UInt8               dataA[  128 * 128] = { 0 };
    CGContextRef        cgContextRGB, cgContextA;
	OSType				imageType, maskType;
    UInt32              dataSize, iconSize;
    CGColorSpaceRef     cgColorSpace;
    IconRef             iconRef;
    IconFamilyHandle    iconHnd;
    Handle              tmpHnd;
    OSStatus            theErr;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);



    // Initialize ourselves
    theErr   = noErr;
    iconRef  = NULL;
	iconSize = std::max(CGImageGetWidth(cgImage), CGImageGetHeight(cgImage));



	// Identify the most appropriate icon size
	if (iconSize >= 128)
		{
		imageType = kThumbnail32BitData;
		maskType  = kThumbnail8BitMask;
		iconSize  = 128;
		}

	else if (iconSize >= 48)
		{
		imageType = kHuge32BitData;
		maskType  = kHuge8BitMask;
		iconSize  = 48;
		}
	else if (iconSize >= 32)

		{
		imageType = kLarge32BitData;
		maskType  = kLarge8BitMask;
		iconSize  = 32;
		}
	else
		{
		imageType = kSmall32BitData;
		maskType  = kSmall8BitMask;
		iconSize  = 16;
		}

	NN_ASSERT((iconSize * iconSize * 4) <= sizeof(dataRGB));
	NN_ASSERT((iconSize * iconSize * 1) <= sizeof(dataA  ));



    // Draw the image
	//
	// Apart from kIconServices256PixelDataARGB, icons are constructed from
	// two blocks of data - the image and the mask.
	cgColorSpace = CGColorSpaceCreateDeviceRGB();
    if (cgColorSpace == NULL)
        return(NULL);

    cgContextRGB  = CGBitmapContextCreate(dataRGB, iconSize, iconSize, 8, iconSize * 4, cgColorSpace, kCGImageAlphaNoneSkipFirst);
    cgContextA    = CGBitmapContextCreate(dataA,   iconSize, iconSize, 8, iconSize * 1, NULL,         kCGImageAlphaOnly);

    if (cgContextRGB != NULL && cgContextA != NULL)
        {
        CGContextDrawImage(cgContextRGB, CGRectMake(0, 0, iconSize, iconSize), cgImage);
        CGContextDrawImage(cgContextA,   CGRectMake(0, 0, iconSize, iconSize), cgImage);
        }

    CFSafeRelease(cgColorSpace);
    CFSafeRelease(cgContextRGB);
    CFSafeRelease(cgContextA);



    // Create the icon family handle
    //
    // An icon family handle is just a fixed (big-endian) header, and tagged data.
    dataSize = sizeof(OSType) + sizeof(OSType);
    iconHnd  = (IconFamilyHandle) NewHandle(dataSize);
    
    if (iconHnd == NULL)
        theErr = memFullErr;
    
    if (theErr == noErr)
        {
        (*iconHnd)->resourceType = EndianU32_NtoB(kIconFamilyType);
        (*iconHnd)->resourceSize = EndianU32_NtoB(dataSize);
        }

	if (theErr == noErr)
		{
		theErr = PtrToHand(dataRGB, &tmpHnd, iconSize * iconSize * 4);
        if (theErr == noErr)
			{
            theErr = SetIconFamilyData(iconHnd, imageType, tmpHnd);
			DisposeHandle(tmpHnd);
			}
		}

	if (theErr == noErr)
		{
		theErr = PtrToHand(dataA, &tmpHnd, iconSize * iconSize * 1);
        if (theErr == noErr)
			{
            theErr = SetIconFamilyData(iconHnd, maskType, tmpHnd);
			DisposeHandle(tmpHnd);
			}
		}



    // Create the IconRef
    if (theErr == noErr)
        theErr = GetIconRefFromIconFamilyPtr(*iconHnd, GetHandleSize((Handle) iconHnd), &iconRef);



    // Clean up
    if (iconHnd != NULL)
        DisposeHandle((Handle) iconHnd);
    
    return(iconRef);
}





//============================================================================
//		NCoreGraphicsUtilities::CreateDataFromImage : Encode an image to data.
//----------------------------------------------------------------------------
NData NCoreGraphicsUtilities::CreateDataFromImage(CGImageRef cgImage, const NUTI &theUTI)
{	Handle						dataRef, theHnd;
	NCFObject					cgImageDst;
	GraphicsExportComponent		qtExporter;
	NString						fileType;
	NData						theData;
	unsigned long				theSize;
	OSStatus					theErr;
	bool						wasOK;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);
	NN_ASSERT(theUTI.IsValid());



	// Initialize ourselves
	qtExporter = NULL;
	dataRef    = NULL;
	theHnd     = NULL;
	theErr     = noErr;



	// Use ImageIO
	//
	// This is the preferred path for 10.4 and above.
	if (CGImageDestinationCreateWithData != NULL)
		{
		theData.SetSize(1);

		if (cgImageDst.Set(CGImageDestinationCreateWithData(theData, theUTI.GetValue(), 1, NULL)))
			{
			CGImageDestinationAddImage(cgImageDst, cgImage, NULL);
			
			wasOK = CGImageDestinationFinalize(cgImageDst);
			NN_ASSERT(wasOK);
			
			if (wasOK)
				return(theData);
			}
		}



	// Create the data reference
	//
	// The data reference is created for an empty handle, which will be
	// expanded by the exporter to contain the encoded data.
	theHnd = NewHandle(0);
	if (theHnd != NULL)
		dataRef = NQuickTimeUtilities::CreateDataReferenceFromHandle(theHnd, theUTI);
	
	if (dataRef == NULL)
		theErr = memFullErr;



	// Export the image
	if (theErr == noErr)
		theErr = OpenADefaultComponent(GraphicsExporterComponentType, theUTI.GetFileType(), &qtExporter);
	
	if (theErr == noErr)
		theErr = GraphicsExportSetInputCGImage(qtExporter, cgImage);

	if (theErr == noErr)
		theErr = GraphicsExportSetOutputDataReference(qtExporter, dataRef, HandleDataHandlerSubType);

	if (theErr == noErr)
		theErr = GraphicsExportDoExport(qtExporter, &theSize);

	if (theErr == noErr)
		theData.SetData(theSize, *theHnd);

	NN_ASSERT_NOERR(theErr);



	// Clean up
	if (qtExporter != NULL)
		CloseComponent(qtExporter);
	
	if (dataRef != NULL)
		DisposeHandle(dataRef);
	
	if (theHnd != NULL)
		DisposeHandle(theHnd);

	return(theData);
}





//============================================================================
//		NCoreGraphicsUtilities::ScaleQuarter8888 : Quarter-scale a 32-bpp image.
//----------------------------------------------------------------------------
//		Note :	When scaling by 50%, we can use an integer math trick:
//
//					http://www.compuphase.com/graphic/scale3.htm
//					http://labs.trolltech.com/blogs/2009/01/20/50-scaling-of-argb32-image/
//
//				This ignores gamma, but produces good quality results in very
//				little time compared to a filtered scale.
//----------------------------------------------------------------------------
void NCoreGraphicsUtilities::ScaleQuarter8888(UInt32 srcWidth, UInt32 srcHeight, UInt32 srcRowBytes, const UInt8 *srcData,
											  UInt32 dstWidth, UInt32 dstHeight, UInt32 dstRowBytes,       UInt8 *dstData)
{	UInt32	x, y, *dstRow, *srcRowA, *srcRowB;



	// Validate our parameters
	NN_ASSERT(srcWidth  == (dstWidth * 2));
	NN_ASSERT(srcHeight == (dstHeight * 2));
	NN_ASSERT(srcRowBytes >= (srcWidth * sizeof(UInt32)));
	NN_ASSERT(dstRowBytes >= (dstWidth * sizeof(UInt32)));

	(void) srcWidth;
	(void) srcHeight;



	// Scale the image
	#define AVG_8888(a, b)				(((((a) ^ (b)) & 0xFEFEFEFE) >> 1) + ((a) & (b)))

	for (y = 0; y < dstHeight; y++)
		{
		srcRowA = (UInt32 *) (srcData + (((y*2)+0) * srcRowBytes));
		srcRowB = (UInt32 *) (srcData + (((y*2)+1) * srcRowBytes));
		dstRow  = (UInt32 *) (dstData + (  y       * dstRowBytes));
		
		for (x = 0; x < dstWidth; x++)
			dstRow[x] = AVG_8888(	AVG_8888(srcRowA[(x*2)+0], srcRowA[(x*2)+1]),
									AVG_8888(srcRowB[(x*2)+0], srcRowB[(x*2)+1]));
		}
	
	#undef AVG_8888
}





//============================================================================
//		NCoreGraphicsUtilities::DataProviderRelease : Data provider callback.
//----------------------------------------------------------------------------
#pragma mark -
void NCoreGraphicsUtilities::DataProviderRelease(void *info, const void */*data*/, size_t /*size*/)
{	CFDataRef		cfData = (CFDataRef) info;



	// Validate our parameters
	NN_ASSERT(cfData != NULL);



	// Release the data
	CFSafeRelease(cfData);
}





