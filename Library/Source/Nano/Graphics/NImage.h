/*	NAME:
		NImage.h

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
#ifndef NIMAGE_H
#define NIMAGE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NData.h"
#include "NFile.h"
#include "NRectangle.h"
#include "NUTI.h"

// System
#include <functional>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Image formats
//
// A format defines the component byte order, the component size in bits,
// and the overall pixel size in bits.
enum class NImageFormat
{
	None,
	A8,

	R8_G8_B8,
	B8_G8_R8,

	R8_G8_B8_A8,
	R8_G8_B8_X8,

	B8_G8_R8_A8,
	B8_G8_R8_X8,

	A8_R8_G8_B8,
	X8_R8_G8_B8,

	A8_B8_G8_R8,
	X8_B8_G8_R8
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Functions
using NFunctionEachImmutablePixel =
	std::function<bool(size_t x, size_t y, const uint8_t* pixelPtr)>;

using NFunctionEachMutablePixel                             = std::function<bool(size_t x, size_t y, uint8_t* pixelPtr)>;

using NFunctionEachImmutableRow =
	std::function<bool(size_t y, size_t theWidth, const uint8_t* rowPtr)>;

using NFunctionEachMutableRow                               = std::function<bool(size_t y, size_t theWidth, uint8_t* rowPtr)>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NImage
{
public:
										NImage(const NSize& theSize,
											   NImageFormat theFormat = NImageFormat::R8_G8_B8_A8,
											   const NData& theData   = NData(),
		   size_t       rowBytes  = 0);

										NImage(const NData& theData);
										NImage(const NFile& theFile);

										NImage();


	// Is the image valid?
	bool                                IsValid() const;


	// Clear the image
	void                                Clear();


	// Get the dimensions
	size_t                              GetWidth()  const;
	size_t                              GetHeight() const;

	NSize                               GetSize()   const;
	NRectangle                          GetBounds() const;


	// Get/set the format
	NImageFormat                        GetFormat() const;
	void                                SetFormat(NImageFormat theFormat);


	// Get the structure
	size_t                              GetBitsPerPixel()     const;
	size_t                              GetBitsPerComponent() const;
	size_t                              GetBytesPerPixel()    const;
	size_t                              GetBytesPerRow()      const;


	// Get the pixels
	//
	// A request for mutable access may need to copy the data.
	const uint8_t*                      GetPixels(       size_t x = 0, size_t y = 0) const;
	uint8_t*                            GetMutablePixels(size_t x = 0, size_t y = 0);


	// Get the data
	NData                               GetData() const;


	// Pack the rows
	//
	// Packs the data to the specified bytes per row, or the minimum
	// required width if no width is specified.
	void                                PackRows(size_t bytesPerRow = 0);


	// Process each pixel
	void                                ForEachPixel(const NFunctionEachImmutablePixel& theFunction) const;
	void                                ForEachPixel(const NFunctionEachMutablePixel&   theFunction);


	// Process each row
	void                                ForEachRow(const NFunctionEachImmutableRow& theFunction) const;
	void                                ForEachRow(const NFunctionEachMutableRow&   theFunction);


	// Load/save the image
	//
	// Images are loaded in the requested format, or their existing format
	// if no format is specified.
	//
	// Images are saved to the UTI of their file ("foo.jpg" will produce
	// a JPEG) and can also be saved to a specific type.
	NStatus                             Load(const NFile& theFile, NImageFormat theFormat = NImageFormat::None);
	NStatus                             Save(const NFile& theFile, const NUTI& theType = NUTI()) const;


	// Encode/decode the image
	//
	// Images are decoded to the requested format, or their existing format
	// if no format is specified.
	NStatus                             Decode(const NData& theData, NImageFormat theFormat = NImageFormat::None);
	NData                               Encode(const NUTI& theType = kNUTTypePNG) const;


private:
	NStatus                             ImageDecode(const NData& theData);
	NData                               ImageEncode(const NUTI& theType) const;

	void                                Convert_A8(         NImageFormat theFormat);
	void                                Convert_R8_G8_B8(   NImageFormat theFormat);
	void                                Convert_B8_G8_R8(   NImageFormat theFormat);
	void                                Convert_R8_G8_B8_A8(NImageFormat theFormat);
	void                                Convert_B8_G8_R8_A8(NImageFormat theFormat);
	void                                Convert_A8_R8_G8_B8(NImageFormat theFormat);
	void                                Convert_A8_B8_G8_R8(NImageFormat theFormat);

	void                                RepackRows();

	bool                                RowSwizzle24(   size_t theWidth, uint8_t* rowPtr, const NVectorSize& newOrder);
	bool                                RowSwizzle32(   size_t theWidth, uint8_t* rowPtr, const NVectorSize& newOrder);
	bool                                RowReduce32To24(size_t theWidth, uint8_t* rowPtr, const NVectorSize& srcOrder);
	bool                                RowReduce32To8( size_t theWidth, uint8_t* rowPtr, size_t srcIndex);

	bool                                RowExpand24To32(size_t             theWidth,
														const uint8_t*     rowPtr,
														const NVectorSize& dstOrder,
														NImage*            dstImage,
														size_t             y);
	bool                                RowExpand8To32(size_t         theWidth,
													   const uint8_t* rowPtr,
													   size_t         dstIndex,
													   NImage*        dstImage,
													   size_t         y);



private:
	NSize                               mSize;
	NData                               mData;
	NImageFormat                        mFormat;
	size_t                              mRowBytes;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NImage.inl"



#endif // NIMAGE_H
