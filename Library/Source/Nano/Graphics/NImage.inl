/*	NAME:
		NImage.inl

	DESCRIPTION:
		Image buffer.

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





//=============================================================================
//		NImage::GetWidth : Get the width.
//-----------------------------------------------------------------------------
inline size_t NImage::GetWidth() const
{


	// Get the width
	return size_t(mSize.width);
}





//=============================================================================
//		NImage::GetHeight : Get the height.
//-----------------------------------------------------------------------------
inline size_t NImage::GetHeight() const
{


	// Get the height
	return size_t(mSize.height);
}





//=============================================================================
//		NImage::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline NSize NImage::GetSize() const
{


	// Get the size
	return mSize;
}





//=============================================================================
//		NImage::GetBounds : Get the bounds.
//-----------------------------------------------------------------------------
inline NRectangle NImage::GetBounds() const
{


	// Get the bounds
	return NRectangle(mSize);
}





//=============================================================================
//		NImage::GetFormat : Get the format.
//-----------------------------------------------------------------------------
inline NImageFormat NImage::GetFormat() const
{


	// Get the format
	return mFormat;
}





//=============================================================================
//		NImage::GetBytesPerPixel : Get the bytes-per-pixel.
//-----------------------------------------------------------------------------
inline size_t NImage::GetBytesPerPixel() const
{


	// Get the value
	size_t numBytes = (GetBitsPerPixel() / 8);
	NN_REQUIRE((numBytes * 8) == GetBitsPerPixel());

	return numBytes;
}





//=============================================================================
//		NImage::GetBytesPerRow : Get the bytes-per-row.
//-----------------------------------------------------------------------------
inline size_t NImage::GetBytesPerRow() const
{


	// Get the value
	return mRowBytes;
}





//=============================================================================
//		NImage::GetPixels : Get the pixels.
//-----------------------------------------------------------------------------
inline const uint8_t* NImage::GetPixels(size_t x, size_t y) const
{


	// Validate our parameters
	NN_REQUIRE(x >= 0 && x < GetWidth());
	NN_REQUIRE(y >= 0 && y < GetHeight());



	// Get the pixels
	size_t theOffset = (y * GetBytesPerRow()) + (x * GetBytesPerPixel());

	return mData.GetData(theOffset);
}





//=============================================================================
//		NImage::GetMutablePixels : Get the pixels.
//-----------------------------------------------------------------------------
inline uint8_t* NImage::GetMutablePixels(size_t x, size_t y)
{


	// Validate our parameters
	NN_REQUIRE(x >= 0 && x < GetWidth());
	NN_REQUIRE(y >= 0 && y < GetHeight());



	// Get the pixels
	size_t theOffset = (y * GetBytesPerRow()) + (x * GetBytesPerPixel());

	return mData.GetMutableData(theOffset);
}





//=============================================================================
//		NImage::GetData : Get the data.
//-----------------------------------------------------------------------------
inline NData NImage::GetData() const
{


	// Get the data
	return mData;
}
