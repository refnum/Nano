/*	NAME:
		WindowsNImage.cpp

	DESCRIPTION:
		Windows image support.

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
#include "NImage.h"

// Nano
#include "NFormat.h"
#include "NWindows.h"

// System
#include <tchar.h>
#include <wincodec.h>
#include <wincodecsdk.h>





//=============================================================================
//		Library glue
//-----------------------------------------------------------------------------
// SHCreateMemStream
#define INDEX_SHCreateMemStream                             LPCSTR(12)

typedef IStream*(__stdcall* SHCreateMemStreamProc)(const BYTE* pInit, UINT cbInit);





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//      CreateMemoryStream : Create a memory stream.
//----------------------------------------------------------------------------
static IStream* CreateMemoryStream(const NData& theData = NData())
{


	// Load the function
	static SHCreateMemStreamProc sCreateMemStream = []()
	{
		SHCreateMemStreamProc createMemStream = nullptr;
		HMODULE               hLibrary        = LoadLibrary(TEXT("shlwapi.dll"));

		if (hLibrary != nullptr)
		{
			createMemStream = reinterpret_cast<SHCreateMemStreamProc>(
				reinterpret_cast<void*>(GetProcAddress(hLibrary, INDEX_SHCreateMemStream)));
		}

		return createMemStream;
	}();



	// Create the stream
	IStream* theStream = nullptr;

	if (sCreateMemStream != nullptr)
	{
		NN_REQUIRE(theData.GetSize() <= size_t(UINT_MAX));
		theStream = sCreateMemStream(theData.GetData(), UINT(theData.GetSize()));
	}

	return theStream;
}





//=============================================================================
//		NImage::ImageDecode : Decode image data.
//-----------------------------------------------------------------------------
NStatus NImage::ImageDecode(const NData& theData)
{


	// Get the state we need
	NScopedCOM coInitialize;

	HRESULT  winErr    = ERROR_SUCCESS;
	IStream* theStream = CreateMemoryStream(theData);

	if (theStream == nullptr)
	{
		return NStatus::NotSupported;
	}



	// Create the decoder
	IWICImagingFactory* icFactory = nullptr;
	IWICBitmapDecoder*  icDecoder = nullptr;

	if (SUCCEEDED(winErr))
	{
		winErr = CoCreateInstance(CLSID_WICImagingFactory,
								  nullptr,
								  CLSCTX_INPROC_SERVER,
								  IID_PPV_ARGS(&icFactory));
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icFactory->CreateDecoderFromStream(theStream,
													0,
													WICDecodeMetadataCacheOnDemand,
													&icDecoder);
	}

	NN_EXPECT_SUCCESS(winErr);



	// Get the image data
	IWICBitmapFrameDecode* icFrame   = nullptr;
	UINT                   theWidth  = 0;
	UINT                   theHeight = 0;
	GUID                   dstPixels{};
	NImageFormat           dstFormat;

	if (SUCCEEDED(winErr))
	{
		winErr = icDecoder->GetFrame(0, &icFrame);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icFrame->GetSize(&theWidth, &theHeight);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icFrame->GetPixelFormat(&dstPixels);
	}

	NN_EXPECT_SUCCESS(winErr);



	// Prepare the destination format
	if (dstPixels == GUID_WICPixelFormat24bppRGB)
	{
		dstFormat = NImageFormat::R8_G8_B8;
	}

	else if (dstPixels == GUID_WICPixelFormat24bppBGR)
	{
		dstFormat = NImageFormat::B8_G8_R8;
	}

	else if (dstPixels == GUID_WICPixelFormat32bppBGR)
	{
		dstFormat = NImageFormat::B8_G8_R8_X8;
	}

	else if (dstPixels == GUID_WICPixelFormat32bppBGRA)
	{
		dstFormat = NImageFormat::B8_G8_R8_A8;
	}

	else
	{
		dstPixels = GUID_WICPixelFormat32bppBGRA;
		dstFormat = NImageFormat::B8_G8_R8_A8;
	}



	// Decode the image
	IWICBitmapSource* icBitmap = nullptr;
	UINT              rowBytes = 0;
	NImage            theImage;
	NStatus           theErr = NStatus::NotSupported;

	if (SUCCEEDED(winErr))
	{
		NN_REQUIRE(theImage.GetBytesPerRow() <= size_t(UINT_MAX));
		theImage = NImage(NSize(float32_t(theWidth), float32_t(theHeight)), dstFormat);
		rowBytes = UINT(theImage.GetBytesPerRow());
	}

	if (SUCCEEDED(winErr))
	{
		winErr = WICConvertBitmapSource(dstPixels, icFrame, &icBitmap);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icBitmap->CopyPixels(nullptr,
									  rowBytes,
									  rowBytes * theHeight,
									  theImage.GetMutablePixels());
	}

	NN_EXPECT_SUCCESS(winErr);

	if (SUCCEEDED(winErr))
	{
		*this  = theImage;
		theErr = NStatus::OK;
	}



	// Clean up
	WNSafeRelease(icBitmap);
	WNSafeRelease(icFrame);
	WNSafeRelease(icDecoder);
	WNSafeRelease(icFactory);

	WNSafeRelease(theStream);

	return theErr;
}





//=============================================================================
//		NImage::ImageEncode : Encode image data.
//-----------------------------------------------------------------------------
NData NImage::ImageEncode(const NUTI& theType) const
{


	// Validate our parameters and state
	NN_REQUIRE(theType.IsValid());
	NN_REQUIRE(IsValid());



	// Get the state we need
	NScopedCOM coInitialize;

	IStream* theStream = CreateMemoryStream();

	if (theStream == nullptr)
	{
		return NData();
	}



	// Prepare the source image
	//
	// Ideally we can use the image as-is, however if its format isn't one
	// that WIC supports we'll need to convert it before we can encode.
	GUID   srcFormat;
	NImage srcImage(*this);

	switch (srcImage.GetFormat())
	{
		case NImageFormat::R8_G8_B8:
			srcFormat = GUID_WICPixelFormat24bppRGB;
			break;

		case NImageFormat::B8_G8_R8:
			srcFormat = GUID_WICPixelFormat24bppBGR;
			break;

		case NImageFormat::B8_G8_R8_X8:
			srcFormat = GUID_WICPixelFormat32bppBGR;
			break;

		case NImageFormat::B8_G8_R8_A8:
			srcFormat = GUID_WICPixelFormat32bppBGRA;
			break;

		default:
			srcImage.SetFormat(NImageFormat::B8_G8_R8_A8);
			srcFormat = GUID_WICPixelFormat32bppBGRA;
			break;
	}

	NData srcData = srcImage.GetData();



	// Prepare the destination format
	//
	// Some encoders can only encode to a single format.
	GUID containerID;
	GUID dstPixels;

	if (theType == kNUTTypePNG)
	{
		containerID = GUID_ContainerFormatPng;
		dstPixels   = GUID_WICPixelFormat32bppBGRA;
	}

	else if (theType == kNUTTypeJPEG)
	{
		containerID = GUID_ContainerFormatJpeg;
		dstPixels   = GUID_WICPixelFormat24bppBGR;
	}

	else if (theType == kNUTTypeTIFF)
	{
		containerID = GUID_ContainerFormatTiff;
		dstPixels   = GUID_WICPixelFormat32bppBGRA;
	}

	else if (theType == kNUTTypeGIF)
	{
		containerID = GUID_ContainerFormatGif;
		dstPixels   = GUID_WICPixelFormat8bppIndexed;
	}

	else
	{
		NN_LOG_ERROR("Unsupported format: {}", theType);
		return NData();
	}



	// Create the converter
	//
	// The converter will handle transcoding from our source pixel format
	// into the encoder-supported format, including colour reduction.
	IWICImagingFactory*  icFactory   = nullptr;
	IWICBitmap*          icBitmap    = nullptr;
	IWICFormatConverter* icConverter = nullptr;

	NN_REQUIRE(srcImage.GetWidth() <= size_t(UINT_MAX));
	NN_REQUIRE(srcImage.GetHeight() <= size_t(UINT_MAX));
	NN_REQUIRE(srcImage.GetBytesPerRow() <= size_t(UINT_MAX));

	UINT    theWidth  = UINT(srcImage.GetWidth());
	UINT    theHeight = UINT(srcImage.GetHeight());
	UINT    rowBytes  = UINT(srcImage.GetBytesPerRow());
	HRESULT winErr    = ERROR_SUCCESS;

	if (SUCCEEDED(winErr))
	{
		winErr = CoCreateInstance(CLSID_WICImagingFactory,
								  nullptr,
								  CLSCTX_INPROC_SERVER,
								  IID_PPV_ARGS(&icFactory));
	}

	if (SUCCEEDED(winErr))
	{
		NN_REQUIRE(srcData.GetSize() <= size_t(UINT_MAX));
		winErr = icFactory->CreateBitmapFromMemory(theWidth,
												   theHeight,
												   srcFormat,
												   rowBytes,
												   UINT(srcData.GetSize()),
												   const_cast<BYTE*>(srcData.GetData()),
												   &icBitmap);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icFactory->CreateFormatConverter(&icConverter);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icConverter->Initialize(icBitmap,
										 dstPixels,
										 WICBitmapDitherTypeErrorDiffusion,
										 nullptr,
										 1.0,
										 WICBitmapPaletteTypeMedianCut);
	}

	NN_EXPECT_SUCCESS(winErr);



	// Create the encoder
	IWICBitmapEncoder*     icEncoder = nullptr;
	IWICBitmapFrameEncode* icFrame   = nullptr;

	if (SUCCEEDED(winErr))
	{
		winErr = icFactory->CreateEncoder(containerID, nullptr, &icEncoder);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icEncoder->Initialize(theStream, WICBitmapEncoderNoCache);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icEncoder->CreateNewFrame(&icFrame, nullptr);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icFrame->Initialize(nullptr);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icFrame->SetSize(theWidth, theHeight);
	}

	NN_EXPECT_SUCCESS(winErr);



	// Encode the image
	if (SUCCEEDED(winErr))
	{
		winErr = icFrame->WriteSource(icConverter, nullptr);
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icFrame->Commit();
	}

	if (SUCCEEDED(winErr))
	{
		winErr = icEncoder->Commit();
	}

	NN_EXPECT_SUCCESS(winErr);



	// Extract the data
	STATSTG streamInfo;
	NData   dstData;

	if (SUCCEEDED(winErr))
	{
		winErr = theStream->Stat(&streamInfo, STATFLAG_NONAME);
	}

	if (SUCCEEDED(winErr))
	{
		dstData.SetSize(size_t(ToNN(streamInfo.cbSize)));
	}

	if (SUCCEEDED(winErr))
	{
		winErr = theStream->Seek(ToWN(int64_t(0)), STREAM_SEEK_SET, nullptr);
	}

	if (SUCCEEDED(winErr))
	{
		NN_REQUIRE(dstData.GetSize() <= size_t(ULONG_MAX));
		winErr = theStream->Read(dstData.GetMutableData(), ULONG(dstData.GetSize()), nullptr);
	}

	NN_EXPECT_SUCCESS(winErr);

	if (FAILED(winErr))
	{
		dstData.Clear();
	}



	// Clean up
	WNSafeRelease(icFrame);
	WNSafeRelease(icEncoder);
	WNSafeRelease(icConverter);
	WNSafeRelease(icBitmap);
	WNSafeRelease(icFactory);

	WNSafeRelease(theStream);

	return dstData;
}
