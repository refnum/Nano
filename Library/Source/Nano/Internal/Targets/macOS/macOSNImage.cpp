/*	NAME:
		macOSNImage.cpp

	DESCRIPTION:
		macOS image support.

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
#include "NCFData.h"
#include "NCGImage.h"
#include "NFormat.h"

// System
#include <CoreServices/CoreServices.h>
#include <ImageIO/CGImageDestination.h>
#include <ImageIO/CGImageSource.h>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// NCFObject helpers
using NCGImageSource                                        = NCFObject<CGImageSourceRef>;
using NCGImageDestination                                   = NCFObject<CGImageDestinationRef>;





//=============================================================================
//		NImage::ImageDecode : Decode image data.
//-----------------------------------------------------------------------------
NStatus NImage::ImageDecode(const NData& theData)
{


	// Get the state we need
	NCGImageSource cgImageSource;
	NCFData        cfData;

	if (!cfData.Set(CFDataCreateWithBytesNoCopy(kCFAllocatorDefault,
												theData.GetData(),
												CFIndex(theData.GetSize()),
												kCFAllocatorNull)))
	{
		return NStatus::Memory;
	}

	if (!cgImageSource.Set(CGImageSourceCreateWithData(cfData, nullptr)))
	{
		return NStatus::Memory;
	}



	// Decode the image
	NCGImage cgImage;

	if (cgImage.Set(CGImageSourceCreateImageAtIndex(cgImageSource, 0, nullptr)))
	{
		*this = cgImage.GetImage();
		return NStatus::OK;
	}

	return NStatus::NotSupported;
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
	CFStringRef dstFormat = nullptr;

	if (theType == kNUTTypePNG)
	{
		dstFormat = kUTTypePNG;
	}
	else if (theType == kNUTTypeJPEG)
	{
		dstFormat = kUTTypeJPEG;
	}
	else if (theType == kNUTTypeTIFF)
	{
		dstFormat = kUTTypeTIFF;
	}
	else if (theType == kNUTTypeGIF)
	{
		dstFormat = kUTTypeGIF;
	}
	else
	{
		NN_LOG_ERROR("Unsupported format: {}", theType);
		return NData();
	}



	// Encode the image
	NCGImageDestination cgImageDst;
	NCGImage            cgImage;
	NCFMutableData      cfData;

	if (cfData.Set(CFDataCreateMutable(kCFAllocatorDefault, 0)) &&
		cgImageDst.Set(CGImageDestinationCreateWithData(cfData, dstFormat, 1, nullptr)) &&
		cgImage.SetImage(*this))
	{
		CGImageDestinationAddImage(cgImageDst, cgImage, nullptr);

		if (CGImageDestinationFinalize(cgImageDst))
		{
			return cfData.GetData();
		}
	}

	return NData();
}
