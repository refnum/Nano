/*	NAME:
		NCoreGraphicsUtilities.cpp

	DESCRIPTION:
		CoreGraphics utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
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
{	CGColorSpaceRef		sColorSpace = CGColorSpaceCreateDeviceGray();



	// Get the color space
	return(sColorSpace);
}





//============================================================================
//		NCoreGraphicsUtilities::GetColorSpaceRGB : Get an RGB color space.
//----------------------------------------------------------------------------
CGColorSpaceRef NCoreGraphicsUtilities::GetColorSpaceRGB(void)
{	CGColorSpaceRef		sColorSpace = CGColorSpaceCreateDeviceRGB();



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
	if (theUTI.EqualTo(kUTNTypeJPEG) || theUTI.EqualTo(kUTNTypePNG))
		{
		if (cgDataProvider.Set(CreateDataProvider(theFile)))
			{
			if (theUTI.EqualTo(kUTNTypeJPEG))
				cgImage = CGImageCreateWithJPEGDataProvider(cgDataProvider, NULL, true, kCGRenderingIntentDefault);

			else if (theUTI.EqualTo(kUTNTypePNG))
				cgImage = CGImageCreateWithPNGDataProvider( cgDataProvider, NULL, true, kCGRenderingIntentDefault);
			}
		}



	// Or use ImageIO
	//
	// This is the preferred path for 10.4 and above.
	if (cgImage == NULL && CGImageSourceCreateWithData != NULL)
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
	NN_ASSERT(theData.GetSize() != 0);
	


	// Get the state we need
	//
	// ImageIO and QuickTime both require PICT files, not PICT data, and so
	// we may need to insert the standard zero-filled header for conversion.
	cgImage   = NULL;
	imageData = &theData;
	
	isJPEG = theUTI.EqualTo(kUTNTypeJPEG);
	isPNG  = theUTI.EqualTo(kUTNTypePNG);

	if (theUTI.EqualTo(kUTNTypePICT))
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
{	UInt32			rowBytes, theWidth, theHeight;
	NCGContext		cgContext;
	CGRect			theRect;
	NData			dataDst;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);
	NN_ASSERT(theSize.width >= 1.0f && theSize.height >= 1.0f);



	// Get the state we need
	theRect.origin = CGPointZero;
	theRect.size   = theSize;

	theWidth  = (UInt32) theSize.width;
	theHeight = (UInt32) theSize.height;
	rowBytes  = theWidth * sizeof(UInt32);



	// Scale the image
	cgContext = NCGContext(dataDst, theWidth, theHeight);

	cgContext.DrawImage(cgImage, theRect);



	// Create a new image
	return(CreateImageFromData(dataDst, theWidth, theHeight, rowBytes));
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

	imgDst.height   = imgSrc.height;
	imgDst.width    = imgSrc.width;
	imgDst.rowBytes = imgDst.width * sizeof(UInt32);



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



	// Create a new image
	return(CreateImageFromData(dataDst, imgDst.width, imgDst.height, imgDst.rowBytes));
}





//============================================================================
//      NCoreGraphicsUtilities::CreateIconFromImage : Create an icon from an image.
//----------------------------------------------------------------------------
IconRef NCoreGraphicsUtilities::CreateIconFromImage(CGImageRef cgImage)
{   UInt32              dataRGB[128 * 128] = { 0 };
    UInt8               dataA[  128 * 128] = { 0 };
    CGContextRef        cgContextRGB, cgContextA;
    CGColorSpaceRef     cgColorSpace;
    SInt32              theSize;
    IconRef             iconRef;
    IconFamilyHandle    iconHnd;
    Handle              tmpHnd;
    OSStatus            theErr;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);



    // Initialize ourselves
    theErr  = noErr;
    iconRef = NULL;



    // Draw the image
    //
    // Since 128x128-sized IconRefs are constructed from two blocks of data,
    // we render the image twice to obtain the image data and the mask data.
    //
    // If we could require 10.4 we could use kIconServices256PixelDataARGB
    // and create a 256x256 icon directly from the ARGB data, but for now we
    // use kCGImageAlphaOnly to support 10.3.
    cgColorSpace = CGColorSpaceCreateDeviceRGB();
    if (cgColorSpace == NULL)
        return(NULL);

    cgContextRGB  = CGBitmapContextCreate(dataRGB, 128, 128, 8, 128 * 4, cgColorSpace, kCGImageAlphaNoneSkipFirst);
    cgContextA    = CGBitmapContextCreate(dataA,   128, 128, 8, 128 * 1, NULL,         kCGImageAlphaOnly);

    if (cgContextRGB != NULL && cgContextA != NULL)
        {
        CGContextDrawImage(cgContextRGB, CGRectMake(0, 0, 128, 128), cgImage);
        CGContextDrawImage(cgContextA,   CGRectMake(0, 0, 128, 128), cgImage);
        }

    CFSafeRelease(cgColorSpace);
    CFSafeRelease(cgContextRGB);
    CFSafeRelease(cgContextA);



    // Create the icon family handle
    //
    // An icon family handle is just a fixed (big-endian) header, and tagged data.
    theSize = sizeof(OSType) + sizeof(OSType);
    iconHnd = (IconFamilyHandle) NewHandle(theSize);
    
    if (iconHnd == NULL)
        theErr = memFullErr;
    
    if (theErr == noErr)
        {
        (*iconHnd)->resourceType = EndianU32_NtoB(kIconFamilyType);
        (*iconHnd)->resourceSize = EndianU32_NtoB(theSize);
        }

	if (theErr == noErr)
		{
		theErr = PtrToHand(dataRGB, &tmpHnd, sizeof(dataRGB));
        if (theErr == noErr)
			{
            theErr = SetIconFamilyData(iconHnd, kThumbnail32BitData, tmpHnd);
			DisposeHandle(tmpHnd);
			}
		}

	if (theErr == noErr)
		{
		theErr = PtrToHand(dataA, &tmpHnd, sizeof(dataA));
        if (theErr == noErr)
			{
            theErr = SetIconFamilyData(iconHnd, kThumbnail8BitMask, tmpHnd);
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
	GraphicsExportComponent		qtExporter;
	NString						fileType;
	NData						theData;
	unsigned long				theSize;
	OSStatus					theErr;



	// Validate our parameters
	NN_ASSERT(cgImage != NULL);
	NN_ASSERT(theUTI.IsValid());



	// Initialize ourselves
	qtExporter = NULL;
	dataRef    = NULL;
	theHnd     = NULL;
	theErr     = noErr;



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





