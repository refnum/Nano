/*	NAME:
		NCGImage.h

	DESCRIPTION:
		CoreGraphics image object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGIMAGE_HDR
#define NCGIMAGE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NString.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGImage : public NCFObject {
public:
										NCGImage(const NString		&theName,
													const NString	&theType  = "",
													const NString	&subDir   = "",
													CFBundleRef		theBundle = NULL);

										NCGImage(const NFile &theFile);
										NCGImage(CGImageRef cgImage);

										NCGImage(void);
	virtual								~NCGImage(void);


	// Get the image dimensions
	//
	// If the image is invalid, a 0'd result is returned.
	UInt32								GetWidth(void)  const;
	UInt32								GetHeight(void) const;
	CGRect								GetBounds(void) const;


	// Get/set the image size
	//
	// Resizing the image will replace the underlying CGImageRef.
	//
	// If the image is invalid, a 0'd result is returned by GetSize.
	CGSize								GetSize(void) const;
	OSStatus							SetSize(const CGSize &theSize);


	// Get the bits per pixel
	UInt32								GetBitsPerPixel(void) const;


	// Get the bits per component
	UInt32								GetBitsPerComponent(void) const;


	// Get the bytes per row
	UInt32								GetBytesPerRow(void) const;
	

private:


};





#endif // NCGImage_HDR


