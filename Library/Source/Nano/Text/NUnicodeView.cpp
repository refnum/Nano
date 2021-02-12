/*	NAME:
		NUnicodeView.cpp

	DESCRIPTION:
		Unicode view.

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
#include "NUnicodeView.h"

// Nano
#include "NString.h"
#include "NStringEncoder.h"





#pragma mark NUnicodeView
//=============================================================================
//		NUnicodeView::NUnicodeView : Constructor.
//-----------------------------------------------------------------------------
NUnicodeView::NUnicodeView(const NString& theString)
	: mEncoding(NStringEncoding::Unknown)
	, mSize(0)
	, mData(nullptr)
{


	// Initialise ourselves
	mData = static_cast<const uint8_t*>(theString.GetContent(&mEncoding, &mSize));



	// Validate our state
	NN_REQUIRE(IsValid());
}





//=============================================================================
//		NUnicodeView::GetCodePointOffsets : Get the codepoint offsets.
//-----------------------------------------------------------------------------
NVectorSize NUnicodeView::GetCodePointOffsets(size_t maxResult) const
{


	// Get the offsets
	NVectorSize theOffsets;
	size_t      theOffset = 0;

	theOffsets.reserve(std::min(maxResult, GetMaxSize()));



	// Get the offsets
	while (theOffset < mSize)
	{
		// Decode the code point
		//
		// We stop when we run out of code points, or reach the limit.
		utf32_t codePoint     = 0;
		size_t  codePointSize = DecodeUTF(theOffset, codePoint);

		if (codePointSize == 0 || theOffsets.size() == maxResult)
		{
			break;
		}



		// Save the offset
		theOffsets.push_back(theOffset);
		theOffset += codePointSize;
	}

	return theOffsets;
}





//=============================================================================
//		NUnicodeView::GetSize : Get the number of code points.
//-----------------------------------------------------------------------------
size_t NUnicodeView::GetSize() const
{


	// Get the state we need
	size_t theSize   = 0;
	size_t theOffset = 0;



	// Get the size
	while (theOffset < mSize)
	{
		// Decode the code point
		//
		// We stop when we run out of code points.
		utf32_t codePoint     = 0;
		size_t  codePointSize = DecodeUTF(theOffset, codePoint);

		if (codePointSize == 0)
		{
			break;
		}



		// Count the code points
		theOffset += codePointSize;
		theSize += 1;
	}

	return theSize;
}





//=============================================================================
//		NUnicodeView::GetMaxSize : Get the maximum number of code points.
//-----------------------------------------------------------------------------
size_t NUnicodeView::GetMaxSize() const
{


	// Get the maximum size
	//
	// The maximum size for variable-width encodings is when
	// every element encodes a single code point.
	size_t codePointSize = NStringEncoder::GetCodeUnitSize(mEncoding);
	size_t maxSize       = (codePointSize != 0) ? (mSize / codePointSize) : 0;

	return maxSize;
}





#pragma mark private
//=============================================================================
//		NUnicodeView::DecodeUTF : Decode UTF.
//-----------------------------------------------------------------------------
size_t NUnicodeView::DecodeUTF(size_t theOffset, utf32_t& codePoint) const
{


	// Validate our parameters
	NN_REQUIRE(theOffset < mSize);



	// Get the state we need
	codePoint            = 0;
	size_t codePointSize = 0;

	size_t         theSize = mSize - theOffset;
	const uint8_t* theData = &mData[theOffset];



	// Decode the code point
	switch (mEncoding)
	{
		case NStringEncoding::UTF8:
			codePointSize = DecodeUTF8(theSize, theData, codePoint);
			break;

		case NStringEncoding::UTF16:
			codePointSize = DecodeUTF16(theSize, theData, codePoint);
			break;

		case NStringEncoding::UTF32:
			codePointSize = DecodeUTF32(theSize, theData, codePoint);
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Invalid encoding!");
			break;
	}

	return codePointSize;
}





//=============================================================================
//		NUnicodeView::DecodeUTF8 : Decode UTF8.
//-----------------------------------------------------------------------------
size_t NUnicodeView::DecodeUTF8(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const
{


	// Validate our parameters
	NN_EXPECT(theSize >= 1);



	// Get the code point
	size_t codePointSize = 0;

	if (theSize >= 1)
	{
		// Single byte
		const utf8_t* nextUTF8 = reinterpret_cast<const utf8_t*>(theData);

		if (theSize >= 1 && ((nextUTF8[0] & 0b10000000) == 0b00000000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b01111111);

			codePoint     = byte0;
			codePointSize = 1;
		}


		// Two bytes
		else if (theSize >= 2 && ((nextUTF8[0] & 0b11100000) == 0b11000000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b00011111) << 6;
			utf32_t byte1 = utf32_t(nextUTF8[1] & 0b00111111) << 0;

			codePoint     = byte0 | byte1;
			codePointSize = 2;
		}


		// Three bytes
		else if (theSize >= 3 && ((nextUTF8[0] & 0b11110000) == 0b11100000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b00001111) << 12;
			utf32_t byte1 = utf32_t(nextUTF8[1] & 0b00111111) << 6;
			utf32_t byte2 = utf32_t(nextUTF8[2] & 0b00111111) << 0;

			codePoint     = byte0 | byte1 | byte2;
			codePointSize = 3;
		}


		// Four bytes
		else if (theSize >= 4 && ((nextUTF8[0] & 0b11111000) == 0b11110000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b00000111) << 18;
			utf32_t byte1 = utf32_t(nextUTF8[1] & 0b00111111) << 12;
			utf32_t byte2 = utf32_t(nextUTF8[2] & 0b00111111) << 6;
			utf32_t byte3 = utf32_t(nextUTF8[3] & 0b00111111) << 0;

			codePoint     = byte0 | byte1 | byte2 | byte3;
			codePointSize = 4;
		}


		// Invalid byte
		else
		{
			codePoint     = kNUTF32Replacement;
			codePointSize = 1;
		}
	}

	return codePointSize;
}





//=============================================================================
//		NUnicodeView::DecodeUTF16 : Decode UTF16.
//-----------------------------------------------------------------------------
size_t NUnicodeView::DecodeUTF16(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const
{


	// Validate our parameters
	NN_EXPECT(theSize >= 2);



	// Get the code point
	size_t codePointSize = 0;

	if (theSize >= 2)
	{
		// Surrogate pair
		const utf16_t* nextUTF16 = reinterpret_cast<const utf16_t*>(theData);

		if (nextUTF16[0] >= kNUTF16SurrogateHiStart && nextUTF16[0] <= kNUTF16SurrogateHiEnd)
		{
			if (theSize >= 4)
			{
				if (nextUTF16[1] >= kNUTF16SurrogateLoStart &&
					nextUTF16[1] <= kNUTF16SurrogateLoEnd)
				{
					utf32_t pairHi = utf32_t(nextUTF16[0] - kNUTF16SurrogateHiStart) << 10;
					utf32_t pairLo = utf32_t(nextUTF16[1] - kNUTF16SurrogateLoStart) << 0;

					codePoint     = 0x10000 + (pairHi | pairLo);
					codePointSize = 4;
				}
				else
				{
					// Invalid low component
					codePoint     = kNUTF32Replacement;
					codePointSize = 2;
				}
			}
			else
			{
				// Missing low component
				codePoint     = kNUTF32Replacement;
				codePointSize = 2;
			}
		}


		// Individual code point
		else
		{
			codePoint     = nextUTF16[0];
			codePointSize = 2;
		}
	}

	return codePointSize;
}





//=============================================================================
//		NUnicodeView::DecodeUTF32 : Decode UTF32.
//-----------------------------------------------------------------------------
size_t NUnicodeView::DecodeUTF32(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const
{


	// Validate our parameters
	NN_EXPECT(theSize >= 4);



	// Get the code point
	size_t codePointSize = 0;

	if (theSize >= 4)
	{
		const utf32_t* nextUTF32 = reinterpret_cast<const utf32_t*>(theData);

		codePoint     = *nextUTF32;
		codePointSize = 4;
	}

	return codePointSize;
}
