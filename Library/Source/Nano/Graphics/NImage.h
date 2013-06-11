/*	NAME:
		NImage.h

	DESCRIPTION:
		Image buffer.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NIMAGE_HDR
#define NIMAGE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"
#include "NUTI.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Image formats
//
// Image formats define the size, and order, of components within an image:
//
//		kNImageFormat_RGBX_8888		[R] [G] [B] [X]
//		kNImageFormat_RGBA_8888		[R] [G] [B] [A]
//		kNImageFormat_XRGB_8888		[X] [R] [G] [B]
//		kNImageFormat_ARGB_8888		[A] [R] [G] [B]
//
typedef enum {
	kNImageFormatNone,
	kNImageFormat_RGBX_8888,
	kNImageFormat_RGBA_8888,
	kNImageFormat_XRGB_8888,
	kNImageFormat_ARGB_8888
} NImageFormat;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NImage {
public:
										NImage(const NData &theData);
										NImage(const NFile &theFile);

										NImage(	const NSize	   &theSize,
												NImageFormat	theFormat = kNImageFormat_RGBA_8888,
												const NData	   &theData   = NData(),
												UInt32			rowBytes  = 0);

										NImage(void);
	virtual							   ~NImage(void);


	// Is the image valid?
	bool								IsValid(void) const;


	// Clear the image
	void								Clear(void);


	// Get the dimensions
	UInt32								GetWidth( void) const;
	UInt32								GetHeight(void) const;

	NSize								GetSize(  void) const;
	NRectangle							GetBounds(void) const;


	// Get the format
	NImageFormat						GetFormat(void) const;


	// Get the structure
	UInt32								GetBitsPerPixel(    void) const;
	UInt32								GetBitsPerComponent(void) const;
	UInt32								GetBytesPerPixel(   void) const;
	UInt32								GetBytesPerRow(     void) const;


	// Get the pixels
	//
	// Const access is preferred, to avoid duplicating the underlying image data.
	const UInt8						   *GetPixels(UInt32 x=0, UInt32 y=0) const;
	UInt8							   *GetPixels(UInt32 x=0, UInt32 y=0);


	// Get the data
	NData								GetData(void) const;


	// Load/save the image
	NStatus								Load(const NFile &theFile);
	NStatus								Save(const NFile &theFile, const NUTI &theType=NUTI()) const;


	// Encode/decode the image
	NData								Encode(const NUTI &theType=kNUTTypePNG) const;
	NStatus								Decode(const NData &theData);


private:
	NSize								mSize;
	NData								mData;
	NImageFormat						mFormat;
	UInt32								mRowBytes;
};







#endif // NCOLOR_HDR


