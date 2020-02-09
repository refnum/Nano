/*	NAME:
		NUnicodeView.cpp

	DESCRIPTION:
		Unicode view.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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





//=============================================================================
//		NUTF32Iterator::NUTF32Iterator : Constructor.
//-----------------------------------------------------------------------------
NUTF32Iterator::NUTF32Iterator(const NUnicodeView* theView, size_t theOffset)
	: mView(theView)
	, mOffset(theOffset)
{
}





//=============================================================================
//		NUTF32Iterator::operator* : Dereference operator.
//-----------------------------------------------------------------------------
utf32_t NUTF32Iterator::operator*() const
{


	// Get the code point
	return mView->GetCodePoint(mOffset);
}





//=============================================================================
//		NUTF32Iterator::operator++ : Prefix increment operator.
//-----------------------------------------------------------------------------
NUTF32Iterator& NUTF32Iterator::operator++()
{


	// Advance the iterator
	mOffset += mView->GetCodePointSize(mOffset);

	return *this;
}





//=============================================================================
//		NUTF32Iterator::operator++ : Postfix increment operator.
//-----------------------------------------------------------------------------
NUTF32Iterator NUTF32Iterator::operator++(int)
{


	// Advance the iterator
	NUTF32Iterator prevIter = *this;

	mOffset += mView->GetCodePointSize(mOffset);

	return prevIter;
}





//=============================================================================
//		NUTF32Iterator::operator+ : Addition operator.
//-----------------------------------------------------------------------------
NUTF32Iterator NUTF32Iterator::operator+(size_t n) const
{


	// Advance the iterator
	NUTF32Iterator theIter = *this;

	for (size_t x = 0; x < n; x++)
	{
		++theIter;
	}

	return theIter;
}





//=============================================================================
//		NUTF32Iterator::operator== : Equality operator.
//-----------------------------------------------------------------------------
bool NUTF32Iterator::operator==(const NUTF32Iterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(mView == otherIter.mView);


	// Compare the iterator
	return mOffset == otherIter.mOffset;
}





//=============================================================================
//		NUTF32Iterator::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
bool NUTF32Iterator::operator!=(const NUTF32Iterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(mView == otherIter.mView);


	// Compare the iterator
	return mOffset != otherIter.mOffset;
}





//=============================================================================
//		NUnicodeView::NUnicodeView : Constructor.
//-----------------------------------------------------------------------------
NUnicodeView::NUnicodeView(NStringEncoding theEncoding, size_t theSize, const void* theData)
	: mEncoding(theEncoding)
	, mSize(theSize)
	, mData(static_cast<const uint8_t*>(theData))
{


	// Validate our state
	NN_REQUIRE(IsValid());
}





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
//		NUnicodeView::GetCodePoint : Get a code point.
//-----------------------------------------------------------------------------
utf32_t NUnicodeView::GetCodePoint(size_t theOffset) const
{


	// Validate our parameters
	NN_REQUIRE(theOffset < mSize);



	// Get the code point
	utf32_t codePoint = 0;

	(void) DecodeUTF(theOffset, codePoint);

	return codePoint;
}





//=============================================================================
//		NUnicodeView::GetCodePointSize : Get the size of a code point.
//-----------------------------------------------------------------------------
size_t NUnicodeView::GetCodePointSize(size_t theOffset) const
{


	// Validate our parameters
	NN_REQUIRE(theOffset < mSize);



	// Get the code point size
	utf32_t codePoint = 0;

	return DecodeUTF(theOffset, codePoint);
}





//=============================================================================
//		NUnicodeView::GetCodePointOffsets : Get the codepoint offsets.
//-----------------------------------------------------------------------------
NVectorSize NUnicodeView::GetCodePointOffsets(size_t maxSize) const
{


	// Get the offsets
	NVectorSize theOffsets;
	size_t      theOffset = 0;

	theOffsets.reserve(std::min(maxSize, GetMaxSize()));



	// Get the offsets
	while (theOffset < mSize)
	{
		// Decode the code point
		//
		// We stop when we run out of code points, or reach the limit.
		utf32_t codePoint     = 0;
		size_t  codePointSize = DecodeUTF(theOffset, codePoint);

		if (codePointSize == 0 || theOffsets.size() == maxSize)
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
	size_t maxSize       = (mSize / codePointSize);

	return maxSize;
}





//=============================================================================
//		NUnicodeView::begin : Get the begin iterator.
//-----------------------------------------------------------------------------
NUTF32Iterator NUnicodeView::begin() const
{


	// Get the iterator
	return NUTF32Iterator(this, 0);
}





//=============================================================================
//		NUnicodeView::end : Get the end iterator.
//-----------------------------------------------------------------------------
NUTF32Iterator NUnicodeView::end() const
{


	// Get the iterator
	return NUTF32Iterator(this, mSize);
}





#pragma mark private
//=============================================================================
//		NUnicodeView::IsValid : Is the view valid?
//-----------------------------------------------------------------------------
bool NUnicodeView::IsValid() const
{


	// Check the state
	bool isValid = (mSize != 0 && mData != nullptr);

	if (isValid)
	{
		isValid = (mEncoding == NStringEncoding::UTF8 || mEncoding == NStringEncoding::UTF16 ||
				   mEncoding == NStringEncoding::UTF32);
	}

	return isValid;
}





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

		if (nextUTF16[0] >= 0xD800 && nextUTF16[0] <= 0xDBFF)
		{
			if (theSize >= 4)
			{
				if (nextUTF16[1] >= 0xDC00 && nextUTF16[1] <= 0xDFFF)
				{
					utf32_t pairHi = utf32_t(nextUTF16[0] - 0xD800) << 10;
					utf32_t pairLo = utf32_t(nextUTF16[1] - 0xDC00) << 0;

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
