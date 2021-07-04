/*	NAME:
		TImage.cpp

	DESCRIPTION:
		NImage tests.

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
#include "NFormat.h"
#include "NImage.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NSize   kTestSize                          = NSize(120, 72);
static constexpr uint8_t kTestImage[]                       = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
	0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x48, 0x04, 0x03, 0x00, 0x00, 0x00, 0xCF, 0x30, 0x2C,
	0x4C, 0x00, 0x00, 0x00, 0x1E, 0x50, 0x4C, 0x54, 0x45, 0x00, 0x65, 0xBD, 0x09, 0x6A, 0xBF, 0x22,
	0x7A, 0xC6, 0x4C, 0x93, 0xD1, 0x7F, 0xB2, 0xDE, 0xB2, 0xD0, 0xEB, 0xD4, 0xE5, 0xF4, 0xDC, 0xEA,
	0xF6, 0xF6, 0xFA, 0xFD, 0xFF, 0xFF, 0xFF, 0x48, 0x1B, 0xA4, 0x5A, 0x00, 0x00, 0x00, 0x01, 0x62,
	0x4B, 0x47, 0x44, 0x09, 0xF1, 0xD9, 0xA5, 0xEC, 0x00, 0x00, 0x01, 0x4F, 0x49, 0x44, 0x41, 0x54,
	0x58, 0xC3, 0xAD, 0x98, 0x39, 0x1A, 0x82, 0x40, 0x0C, 0x46, 0x11, 0x2F, 0x60, 0xE1, 0x01, 0x68,
	0xE8, 0xED, 0xA8, 0xED, 0xAC, 0xED, 0x3C, 0x82, 0xB5, 0x95, 0xBD, 0x8D, 0x07, 0x40, 0xCD, 0x6D,
	0x2D, 0xC0, 0x85, 0x30, 0x33, 0x49, 0x7C, 0xD2, 0xF0, 0xB1, 0xFC, 0x90, 0x79, 0x64, 0xA5, 0x92,
	0x61, 0xDB, 0x57, 0x81, 0xAD, 0x1D, 0x45, 0xD5, 0xB8, 0x7F, 0x6C, 0xFD, 0xDA, 0xF5, 0x55, 0x89,
	0xE5, 0xBE, 0xF1, 0x6A, 0x97, 0x67, 0xD1, 0x62, 0xE9, 0x1B, 0x9F, 0xB6, 0x3E, 0xC8, 0x5C, 0x2C,
	0x97, 0x95, 0x47, 0xBB, 0xD8, 0x49, 0x4A, 0x2C, 0x27, 0x8F, 0xB8, 0x93, 0xB4, 0xD8, 0x83, 0xBC,
	0x95, 0x9C, 0xD8, 0x46, 0xFE, 0x06, 0x3D, 0x88, 0x0F, 0xDF, 0x47, 0x16, 0xF2, 0x0F, 0x68, 0x11,
	0x91, 0x5E, 0x1F, 0x37, 0x4E, 0xD0, 0xE3, 0x9B, 0xA6, 0x96, 0x94, 0x90, 0x7F, 0x83, 0x7E, 0xAD,
	0x71, 0xC2, 0xA0, 0x84, 0xBC, 0x93, 0x04, 0xDD, 0xE4, 0x49, 0x03, 0xF4, 0xFB, 0x25, 0x29, 0x73,
	0x2C, 0xD0, 0x9F, 0xE5, 0xD5, 0x0E, 0xE4, 0x79, 0xB0, 0x36, 0xF2, 0xD2, 0xF3, 0x2D, 0xE4, 0xE5,
	0x95, 0x19, 0xC8, 0x0D, 0xA6, 0xC5, 0xCB, 0xD6, 0xD7, 0x2C, 0x19, 0x66, 0xFB, 0x51, 0x1E, 0x89,
	0xC7, 0x83, 0x73, 0xF7, 0xD4, 0xAE, 0xD8, 0x49, 0x5B, 0xE7, 0x73, 0xA1, 0x0C, 0x17, 0xA7, 0xF3,
	0x26, 0x6F, 0x74, 0x87, 0x4D, 0xC2, 0x44, 0x7F, 0xC0, 0xCE, 0xE1, 0x44, 0x52, 0x85, 0x46, 0x7E,
	0x3B, 0x46, 0x92, 0x94, 0xB6, 0x33, 0x96, 0x1E, 0x35, 0xA1, 0x58, 0x62, 0xD6, 0xC8, 0x63, 0x25,
	0x41, 0x21, 0x8F, 0x15, 0x23, 0x8D, 0x3C, 0x56, 0x06, 0x35, 0xF2, 0x58, 0x01, 0x9E, 0x23, 0x8F,
	0x94, 0xFE, 0x19, 0xF2, 0x50, 0xD3, 0x81, 0xC4, 0xC4, 0x6C, 0x02, 0x8C, 0x7C, 0x2A, 0xE4, 0x24,
	0xC4, 0x3D, 0x49, 0x60, 0x90, 0x90, 0x24, 0xC9, 0x80, 0xA4, 0x21, 0x94, 0x00, 0x49, 0xEA, 0x25,
	0x49, 0x9F, 0x94, 0x1B, 0x52, 0xE8, 0x48, 0x89, 0x45, 0xC5, 0x9D, 0xB4, 0x15, 0xA4, 0xA1, 0x21,
	0xAD, 0x14, 0x69, 0xE2, 0x48, 0xFB, 0x88, 0x1A, 0x57, 0xD2, 0x32, 0x93, 0x66, 0x9D, 0x8C, 0x09,
	0x68, 0x40, 0x41, 0xA3, 0x11, 0x1A, 0xCA, 0xFE, 0x35, 0x0E, 0x92, 0x41, 0x94, 0x8C, 0xC0, 0x64,
	0xF8, 0x26, 0x63, 0x3F, 0xFA, 0xE1, 0xF0, 0xD3, 0xAF, 0x8E, 0x27, 0xE8, 0x93, 0x24, 0xBF, 0xC8,
	0x73, 0x1B, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};

static constexpr uint8_t kValueNone                         = 0x00;
static constexpr uint8_t kValueRed                          = 0x11;
static constexpr uint8_t kValueGreen                        = 0x22;
static constexpr uint8_t kValueBlue                         = 0x33;
static constexpr uint8_t kValueAlpha                        = 0x44;

static constexpr NImageFormat kTestFormat                   = NImageFormat::R8_G8_B8_A8;
static constexpr uint8_t      kTestPixel[]                  = {kValueRed, kValueGreen, kValueBlue, kValueAlpha};
static constexpr uint8_t      kTestPixel_A[]                = {kValueNone, kValueNone, kValueNone, kValueAlpha};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TImage)
{
	NImage theImage;

	SETUP
	{
		theImage = NImage(NSize(1, 1), kTestFormat, NData(sizeof(kTestPixel), kTestPixel));
	}

	void EncodeDecode(const NUTI& theType);
	void SetFormat(NImageFormat theFormat, const NVectorUInt8& theComponents);
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "Default")
{


	// Perform the test
	theImage = NImage();

	REQUIRE(!theImage.IsValid());
	REQUIRE(theImage.GetBounds().IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "IsValid")
{


	// Perform the test
	REQUIRE(theImage.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "Clear")
{


	// Perform the test
	REQUIRE(theImage.IsValid());

	theImage.Clear();
	REQUIRE(!theImage.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetWidth")
{


	// Perform the test
	REQUIRE(theImage.GetWidth() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetHeight")
{


	// Perform the test
	REQUIRE(theImage.GetHeight() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetSize")
{


	// Perform the test
	REQUIRE(theImage.GetSize() == NSize(1, 1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetBounds")
{


	// Perform the test
	REQUIRE(theImage.GetBounds() == NRectangle(0, 0, 1, 1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetFormat")
{


	// Perform the test
	REQUIRE(theImage.GetFormat() == kTestFormat);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/A8")
{


	// Perform the test
	SetFormat(NImageFormat::A8, {kValueAlpha});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/R8_G8_B8")
{


	// Perform the test
	SetFormat(NImageFormat::R8_G8_B8, {kValueRed, kValueGreen, kValueBlue});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/B8_G8_R8")
{


	// Perform the test
	SetFormat(NImageFormat::B8_G8_R8, {kValueBlue, kValueGreen, kValueRed});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/R8_G8_B8_A8")
{


	// Perform the test
	SetFormat(NImageFormat::R8_G8_B8_A8, {kValueRed, kValueGreen, kValueBlue, kValueAlpha});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/R8_G8_B8_X8")
{


	// Perform the test
	SetFormat(NImageFormat::R8_G8_B8_X8, {kValueRed, kValueGreen, kValueBlue, kValueNone});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/B8_G8_R8_A8")
{


	// Perform the test
	SetFormat(NImageFormat::B8_G8_R8_A8, {kValueBlue, kValueGreen, kValueRed, kValueAlpha});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/B8_G8_R8_X8")
{


	// Perform the test
	SetFormat(NImageFormat::B8_G8_R8_X8, {kValueBlue, kValueGreen, kValueRed, kValueNone});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/A8_R8_G8_B8")
{


	// Perform the test
	SetFormat(NImageFormat::A8_R8_G8_B8, {kValueAlpha, kValueRed, kValueGreen, kValueBlue});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/X8_R8_G8_B8")
{


	// Perform the test
	SetFormat(NImageFormat::X8_R8_G8_B8, {kValueNone, kValueRed, kValueGreen, kValueBlue});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/A8_B8_G8_R8")
{


	// Perform the test
	SetFormat(NImageFormat::A8_B8_G8_R8,
			  {
				  kValueAlpha,
			  });
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "SetFormat/X8_B8_G8_R8")
{


	// Perform the test
	SetFormat(NImageFormat::X8_B8_G8_R8, {kValueNone, kValueBlue, kValueGreen, kValueRed});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetBitsPerPixel")
{


	// Perform the test
	REQUIRE(theImage.GetBitsPerPixel() == 32);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetBitsPerComponent")
{


	// Perform the test
	REQUIRE(theImage.GetBitsPerComponent() == 8);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetBytesPerPixel")
{


	// Perform the test
	REQUIRE(theImage.GetBytesPerPixel() == 4);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetBytesPerRow")
{


	// Perform the test
	REQUIRE(theImage.GetBytesPerPixel() == 4);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetPixels")
{


	// Perform the test
	REQUIRE(memcmp(theImage.GetPixels(), kTestPixel, sizeof(kTestPixel)) == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetMutablePixels")
{


	// Perform the test
	REQUIRE(memcmp(theImage.GetMutablePixels(), kTestPixel, sizeof(kTestPixel)) == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "GetData")
{


	// Perform the test
	REQUIRE(theImage.GetData() == NData(sizeof(kTestPixel), kTestPixel));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "PackRows")
{


	// Perform the test
	NData theData(20 * 4, nullptr, NDataSource::Zero);

	theImage = NImage(NSize(10, 1), NImageFormat::R8_G8_B8_A8, theData, 20 * 4);
	REQUIRE(theImage.GetBytesPerRow() == (20 * 4));

	theImage.PackRows();
	REQUIRE(theImage.GetBytesPerRow() == (10 * 4));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "ForEachPixel/Immutable")
{


	// Perform the test
	size_t numPixels = 0;

	theImage.ForEachPixel(
		[&](size_t, size_t, const uint8_t*)
		{
			numPixels++;
			return true;
		});

	REQUIRE(numPixels == 1);
}
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "ForEachPixel/Mutable")
{


	// Perform the test
	size_t numPixels = 0;

	theImage.ForEachPixel(
		[&](size_t, size_t, uint8_t*)
		{
			numPixels++;
			return true;
		});

	REQUIRE(numPixels == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "ForEachRow/Immutable")
{


	// Perform the test
	size_t numRows = 0;

	theImage.ForEachRow(
		[&](size_t, size_t, const uint8_t*)
		{
			numRows++;
			return true;
		});

	REQUIRE(numRows == 1);
}
//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "ForEachRow/Mutable")
{


	// Perform the test
	size_t numRows = 0;

	theImage.ForEachRow(
		[&](size_t, size_t, uint8_t*)
		{
			numRows++;
			return true;
		});

	REQUIRE(numRows == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "Encode/PNG")
{


	// Perform the test
	EncodeDecode(kNUTTypePNG);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "Encode/JPEG")
{


	// Perform the test
	EncodeDecode(kNUTTypeJPEG);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "Encode/TIFF")
{


	// Perform the test
	EncodeDecode(kNUTTypeTIFF);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TImage, "Encode/GIF")
{


	// Perform the test
	EncodeDecode(kNUTTypeGIF);
}





#pragma mark internal
//=============================================================================
//		Fixture_TImage::EncodeDecode : Encode/decode an image.
//-----------------------------------------------------------------------------
void Fixture_TImage::EncodeDecode(const NUTI& theType)
{


	// Perform the test
	if constexpr (NN_TARGET_ANDROID || NN_TARGET_LINUX)
	{
		NN_LOG_UNIMPLEMENTED("for {}", theType);
	}
	else
	{
		NData theData(sizeof(kTestImage), kTestImage);
		REQUIRE(!theData.IsEmpty());

		NFile theFile = NFileUtils::GetTemporary("TImage");
		REQUIRE(theFile.IsValid());


		NStatus theErr = theImage.Decode(theData);
		REQUIRE_NOT_ERR(theErr);
		REQUIRE(theImage.GetSize() == kTestSize);

		theData = theImage.Encode(theType);
		REQUIRE(!theData.IsEmpty());


		theErr = theImage.Save(theFile, theType);
		REQUIRE_NOT_ERR(theErr);

		theErr = theImage.Load(theFile);
		REQUIRE_NOT_ERR(theErr);
		REQUIRE(theImage.GetSize() == kTestSize);
	}
}





//=============================================================================
//		Fixture_TImage::SetFormat : Set the format.
//-----------------------------------------------------------------------------
void Fixture_TImage::SetFormat(NImageFormat theFormat, const NVectorUInt8& theComponents)
{


	// Validate our state
	REQUIRE(theImage.IsValid());



	// Convert to the format
	theImage.SetFormat(theFormat);
	const uint8_t* pixelPtr = theImage.GetPixels();

	for (size_t n = 0; n < theComponents.size(); n++)
	{
		if (theComponents[n] != kValueNone)
		{
			REQUIRE(pixelPtr[n] == theComponents[n]);
		}
	}



	// Convert from the format
	theImage.SetFormat(kTestFormat);
	pixelPtr = theImage.GetPixels();

	for (size_t n = 0; n < std::size(kTestPixel); n++)
	{
		if (theFormat == NImageFormat::A8)
		{
			REQUIRE(pixelPtr[n] == kTestPixel_A[n]);
		}
		else if (kTestPixel[n] != kValueAlpha)
		{
			REQUIRE(pixelPtr[n] == kTestPixel[n]);
		}
	}
}
