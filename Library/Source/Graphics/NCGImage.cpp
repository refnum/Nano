/*	NAME:
		NCGImage.cpp

	DESCRIPTION:
		CoreGraphics image object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NCFBundle.h"
#include "NCGImage.h"





//============================================================================
//		NCGImage::NCGImage : Constructor.
//----------------------------------------------------------------------------
NCGImage::NCGImage(const NString	&theName,
					const NString	&theType,
					const NString	&subDir,
					CFBundleRef		theBundle)
{	NCFBundle		cfBundle;
	NFile			theFile;



	// Validate our parameters
	NN_ASSERT(theName.IsNotEmpty());



	// Find the file
	if (theBundle != NULL)
		cfBundle.Set(theBundle, false);

	theFile = cfBundle.GetResource(theName, theType, subDir);



	// Initialize ourselves
	Set(NCoreGraphicsUtilities::CreateImageFromFile(theFile));
}





//============================================================================
//		NCGImage::NCGImage : Constructor.
//----------------------------------------------------------------------------
NCGImage::NCGImage(const NFile &theFile)
{


	// Initialize ourselves
	Set(NCoreGraphicsUtilities::CreateImageFromFile(theFile));
}





//============================================================================
//		NCGImage::NCGImage : Constructor.
//----------------------------------------------------------------------------
NCGImage::NCGImage(CGImageRef cgImage)
{


	// Initialize ourselves
	Set(cgImage, false);
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
//		NCGImage::GetWidth : Get the width.
//----------------------------------------------------------------------------
UInt32 NCGImage::GetWidth(void) const
{


	// Get the width
	//
	// Invalid images are mapped to 0, to allow callers to use a size
	// test to handle NULL images as per empty images.
	return(IsValid() ? CGImageGetWidth(*this) : 0);
}





//============================================================================
//		NCGImage::GetHeight : Get the height.
//----------------------------------------------------------------------------
UInt32 NCGImage::GetHeight(void) const
{


	// Get the height
	//
	// Invalid images are mapped to 0, to allow callers to use a size
	// test to handle NULL images as per empty images.
	return(IsValid() ? CGImageGetHeight(*this) : 0);
}





//============================================================================
//		NCGImage::GetBounds : Get the bounds.
//----------------------------------------------------------------------------
CGRect NCGImage::GetBounds(void) const
{	CGRect	theRect;



	// Get the bounds
	theRect.origin = CGPointZero;
	theRect.size   = GetSize();
	
	return(theRect);
}





//============================================================================
//		NCGImage::GetSize : Get the size.
//----------------------------------------------------------------------------
CGSize NCGImage::GetSize(void) const
{	CGSize	theSize;



	// Get the size
	theSize.width  = GetWidth();
	theSize.height = GetHeight();
	
	return(theSize);
}





//============================================================================
//		NCGImage::SetSize : Set the size.
//----------------------------------------------------------------------------
OSStatus NCGImage::SetSize(const CGSize &theSize)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Create the image
	if (!Set(NCoreGraphicsUtilities::CreateScaledImage(*this, theSize)))
		return(memFullErr);

	return(noErr);
}





//============================================================================
//		NCGImage::GetBitsPerPixel : Get the bits per pixel.
//----------------------------------------------------------------------------
UInt32 NCGImage::GetBitsPerPixel(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the bits per pixel
	return(CGImageGetBitsPerPixel(*this));
}





//============================================================================
//		NCGImage::GetBitsPerComponent : Get the bits per component.
//----------------------------------------------------------------------------
UInt32 NCGImage::GetBitsPerComponent(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the bits per component
	return(CGImageGetBitsPerComponent(*this));
}





//============================================================================
//		NCGImage::GetBytesPerRow : Get the bytes per row.
//----------------------------------------------------------------------------
UInt32 NCGImage::GetBytesPerRow(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the bytes per row
	return(GetWidth() * GetBitsPerPixel() / 8);
}



