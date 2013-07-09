/*	NAME:
		TImage.cpp

	DESCRIPTION:
		NImage tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NSTLUtilities.h"
#include "NImage.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NSize kTestSize									= NSize(120, 72);
static const UInt8 kTestImage[]									= {	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44,
																	0x52, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x48, 0x04, 0x03, 0x00, 0x00, 0x00, 0xCF,
																	0x30, 0x2C, 0x4C, 0x00, 0x00, 0x00, 0x1E, 0x50, 0x4C, 0x54, 0x45, 0x00, 0x65, 0xBD, 0x09,
																	0x6A, 0xBF, 0x22, 0x7A, 0xC6, 0x4C, 0x93, 0xD1, 0x7F, 0xB2, 0xDE, 0xB2, 0xD0, 0xEB, 0xD4,
																	0xE5, 0xF4, 0xDC, 0xEA, 0xF6, 0xF6, 0xFA, 0xFD, 0xFF, 0xFF, 0xFF, 0x48, 0x1B, 0xA4, 0x5A,
																	0x00, 0x00, 0x00, 0x01, 0x62, 0x4B, 0x47, 0x44, 0x09, 0xF1, 0xD9, 0xA5, 0xEC, 0x00, 0x00,
																	0x01, 0x4F, 0x49, 0x44, 0x41, 0x54, 0x58, 0xC3, 0xAD, 0x98, 0x39, 0x1A, 0x82, 0x40, 0x0C,
																	0x46, 0x11, 0x2F, 0x60, 0xE1, 0x01, 0x68, 0xE8, 0xED, 0xA8, 0xED, 0xAC, 0xED, 0x3C, 0x82,
																	0xB5, 0x95, 0xBD, 0x8D, 0x07, 0x40, 0xCD, 0x6D, 0x2D, 0xC0, 0x85, 0x30, 0x33, 0x49, 0x7C,
																	0xD2, 0xF0, 0xB1, 0xFC, 0x90, 0x79, 0x64, 0xA5, 0x92, 0x61, 0xDB, 0x57, 0x81, 0xAD, 0x1D,
																	0x45, 0xD5, 0xB8, 0x7F, 0x6C, 0xFD, 0xDA, 0xF5, 0x55, 0x89, 0xE5, 0xBE, 0xF1, 0x6A, 0x97,
																	0x67, 0xD1, 0x62, 0xE9, 0x1B, 0x9F, 0xB6, 0x3E, 0xC8, 0x5C, 0x2C, 0x97, 0x95, 0x47, 0xBB,
																	0xD8, 0x49, 0x4A, 0x2C, 0x27, 0x8F, 0xB8, 0x93, 0xB4, 0xD8, 0x83, 0xBC, 0x95, 0x9C, 0xD8,
																	0x46, 0xFE, 0x06, 0x3D, 0x88, 0x0F, 0xDF, 0x47, 0x16, 0xF2, 0x0F, 0x68, 0x11, 0x91, 0x5E,
																	0x1F, 0x37, 0x4E, 0xD0, 0xE3, 0x9B, 0xA6, 0x96, 0x94, 0x90, 0x7F, 0x83, 0x7E, 0xAD, 0x71,
																	0xC2, 0xA0, 0x84, 0xBC, 0x93, 0x04, 0xDD, 0xE4, 0x49, 0x03, 0xF4, 0xFB, 0x25, 0x29, 0x73,
																	0x2C, 0xD0, 0x9F, 0xE5, 0xD5, 0x0E, 0xE4, 0x79, 0xB0, 0x36, 0xF2, 0xD2, 0xF3, 0x2D, 0xE4,
																	0xE5, 0x95, 0x19, 0xC8, 0x0D, 0xA6, 0xC5, 0xCB, 0xD6, 0xD7, 0x2C, 0x19, 0x66, 0xFB, 0x51,
																	0x1E, 0x89, 0xC7, 0x83, 0x73, 0xF7, 0xD4, 0xAE, 0xD8, 0x49, 0x5B, 0xE7, 0x73, 0xA1, 0x0C,
																	0x17, 0xA7, 0xF3, 0x26, 0x6F, 0x74, 0x87, 0x4D, 0xC2, 0x44, 0x7F, 0xC0, 0xCE, 0xE1, 0x44,
																	0x52, 0x85, 0x46, 0x7E, 0x3B, 0x46, 0x92, 0x94, 0xB6, 0x33, 0x96, 0x1E, 0x35, 0xA1, 0x58,
																	0x62, 0xD6, 0xC8, 0x63, 0x25, 0x41, 0x21, 0x8F, 0x15, 0x23, 0x8D, 0x3C, 0x56, 0x06, 0x35,
																	0xF2, 0x58, 0x01, 0x9E, 0x23, 0x8F, 0x94, 0xFE, 0x19, 0xF2, 0x50, 0xD3, 0x81, 0xC4, 0xC4,
																	0x6C, 0x02, 0x8C, 0x7C, 0x2A, 0xE4, 0x24, 0xC4, 0x3D, 0x49, 0x60, 0x90, 0x90, 0x24, 0xC9,
																	0x80, 0xA4, 0x21, 0x94, 0x00, 0x49, 0xEA, 0x25, 0x49, 0x9F, 0x94, 0x1B, 0x52, 0xE8, 0x48,
																	0x89, 0x45, 0xC5, 0x9D, 0xB4, 0x15, 0xA4, 0xA1, 0x21, 0xAD, 0x14, 0x69, 0xE2, 0x48, 0xFB,
																	0x88, 0x1A, 0x57, 0xD2, 0x32, 0x93, 0x66, 0x9D, 0x8C, 0x09, 0x68, 0x40, 0x41, 0xA3, 0x11,
																	0x1A, 0xCA, 0xFE, 0x35, 0x0E, 0x92, 0x41, 0x94, 0x8C, 0xC0, 0x64, 0xF8, 0x26, 0x63, 0x3F,
																	0xFA, 0xE1, 0xF0, 0xD3, 0xAF, 0x8E, 0x27, 0xE8, 0x93, 0x24, 0xBF, 0xC8, 0x73, 0x1B, 0xD6,
																	0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82 };

static const UInt8 kComponentNone								= 0x00;
static const UInt8 kComponentRed								= 0x11;
static const UInt8 kComponentGreen								= 0x22;
static const UInt8 kComponentBlue								= 0x33;
static const UInt8 kComponentAlpha								= 0x44;

static const NImageFormat kTestFormat							= kNImageFormat_RGBA_8888;
static const UInt8        kTestPixel[]							= { kComponentRed, kComponentGreen, kComponentBlue, kComponentAlpha };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NIMAGE(...)											TEST_NANO(TImage, ##__VA_ARGS__)

FIXTURE_NANO(TImage)
{
	NImage		theImage;
	NData		theData;
	NStatus		theErr;

	void EncodeDecode(const NUTI &theType);
	void Format(NImageFormat theFormat, const UInt8List &theComponents);
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("Default")
{


	// Perform the test
	REQUIRE(!theImage.IsValid());
	REQUIRE( theImage.GetBounds().IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("RGB_888")
{


	// Perform the test
	Format(kNImageFormat_RGB_888, nvector<UInt8>(kComponentRed, kComponentGreen, kComponentBlue));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("BGR_888")
{


	// Perform the test
	Format(kNImageFormat_BGR_888, nvector<UInt8>(kComponentBlue, kComponentGreen, kComponentRed));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("RGBX_8888")
{


	// Perform the test
	Format(kNImageFormat_RGBX_8888, nvector<UInt8>(kComponentRed, kComponentGreen, kComponentBlue, kComponentNone));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("RGBA_8888", "Test RGBA_8888 conversion")
{


	// Perform the test
	Format(kNImageFormat_RGBA_8888, nvector<UInt8>(kComponentRed, kComponentGreen, kComponentBlue, kComponentAlpha));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("XRGB_8888")
{


	// Perform the test
	Format(kNImageFormat_XRGB_8888, nvector<UInt8>(kComponentNone, kComponentRed, kComponentGreen, kComponentBlue));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("ARGB_8888")
{


	// Perform the test
	Format(kNImageFormat_ARGB_8888, nvector<UInt8>(kComponentAlpha, kComponentRed, kComponentGreen, kComponentBlue));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("BGRX_8888")
{


	// Perform the test
	Format(kNImageFormat_BGRX_8888, nvector<UInt8>(kComponentBlue, kComponentGreen, kComponentRed, kComponentNone));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("BGRA_8888")
{


	// Perform the test
	Format(kNImageFormat_BGRA_8888, nvector<UInt8>(kComponentBlue, kComponentGreen, kComponentRed, kComponentAlpha));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("PNG")
{


	// Perform the test
	EncodeDecode(kNUTTypePNG);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("JPEG")
{


	// Perform the test
	EncodeDecode(kNUTTypeJPEG);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("TIFF")
{


	// Perform the test
	EncodeDecode(kNUTTypeTIFF);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NIMAGE("GIF")
{


	// Perform the test
	EncodeDecode(kNUTTypeGIF);
}





#pragma mark internal
//============================================================================
//		TImage::EncodeDecode : Encode/decode an image.
//----------------------------------------------------------------------------
void TImage::EncodeDecode(const NUTI &theType)
{


	// Setup
	theErr = theImage.Decode(NData(sizeof(kTestImage), kTestImage));
	REQUIRE_NOERR(theErr);



	// Perform the test
	theData = theImage.Encode(theType);
	REQUIRE(!theData.IsEmpty());

	theErr = theImage.Decode(theData);
	REQUIRE_NOERR(theErr);
}





//============================================================================
//		TImage::Format : Image pixel formats.
//----------------------------------------------------------------------------
void TImage::Format(NImageFormat theFormat, const UInt8List &theComponents)
{	const UInt8		*pixelPtr;
	NIndex			n;



	// Setup
	theImage = NImage(NSize(1, 1), kTestFormat, NData(sizeof(kTestPixel), kTestPixel));



	// Convert to the format
	theImage.SetFormat(theFormat);
	pixelPtr = theImage.GetPixels();
	
	for (n = 0; n < (NIndex) theComponents.size(); n++)
		{
		if (theComponents[n] != kComponentNone)
			REQUIRE(pixelPtr[n] == theComponents[n]);
		}



	// Convert from the format
	theImage.SetFormat(kTestFormat);
	pixelPtr = theImage.GetPixels();

	for (n = 0; n < NN_ARRAY_SIZE(kTestPixel); n++)
		{
		if (kTestPixel[n] != kComponentAlpha)
			REQUIRE(pixelPtr[n] == kTestPixel[n]);
		}
}



