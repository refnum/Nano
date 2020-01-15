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


	// Get the codepoint
	return mView->GetCodepoint(mOffset);
}





//=============================================================================
//		NUTF32Iterator::operator++ : Increment operator.
//-----------------------------------------------------------------------------
void NUTF32Iterator::operator++()
{


	// Advance the iterator
	mOffset += mView->GetSize(mOffset);
}





//=============================================================================
//		NUTF32Iterator::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
bool NUTF32Iterator::operator!=(const NUTF32Iterator& otherIter) const
{


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
	NN_REQUIRE(mEncoding == NStringEncoding::UTF8 || mEncoding == NStringEncoding::UTF16 ||
			   mEncoding == NStringEncoding::UTF32);

	NN_REQUIRE(mSize != 0);
	NN_REQUIRE_NOT_NULL(mData);
}





//=============================================================================
//		NUnicodeView::GetCodepoint : Get a codepoint.
//-----------------------------------------------------------------------------
utf32_t NUnicodeView::GetCodepoint(size_t theOffset) const
{


	// Get the codepoint
	utf32_t codePoint = 0;

	if (theOffset < mSize)
	{
		// Get the state we need
		size_t         theSize = mSize - theOffset;
		const uint8_t* theData = &mData[theOffset];


		// Decode the codepoint
		switch (mEncoding)
		{
			case NStringEncoding::UTF8:
				DecodeUTF8(theSize, theData, codePoint);
				break;

			case NStringEncoding::UTF16:
				DecodeUTF16(theSize, theData, codePoint);
				break;

			case NStringEncoding::UTF32:
				DecodeUTF32(theSize, theData, codePoint);
				break;

			default:
				NN_LOG_UNIMPLEMENTED("Invalid encoding!");
				break;
		}
	}

	return codePoint;
}





//=============================================================================
//		NUnicodeView::GetSize : Get the size of a codepoint.
//-----------------------------------------------------------------------------
size_t NUnicodeView::GetSize(size_t theOffset) const
{


	// Get the size
	size_t charSize = 0;

	if (theOffset < mSize)
	{
		// Get the state we need
		size_t         theSize   = mSize - theOffset;
		const uint8_t* theData   = &mData[theOffset];
		utf32_t        codePoint = 0;


		// Decode the codepoint
		switch (mEncoding)
		{
			case NStringEncoding::UTF8:
				charSize = DecodeUTF8(theSize, theData, codePoint);
				break;

			case NStringEncoding::UTF16:
				charSize = DecodeUTF16(theSize, theData, codePoint);
				break;

			case NStringEncoding::UTF32:
				charSize = DecodeUTF32(theSize, theData, codePoint);
				break;

			default:
				NN_LOG_UNIMPLEMENTED("Invalid encoding!");
				break;
		}
	}

	return charSize;
}





//=============================================================================
//		NUnicodeView::GetMaxSize : Get the maximum size.
//-----------------------------------------------------------------------------
size_t NUnicodeView::GetMaxSize() const
{


	// Get the maximum size
	size_t maxSize = 0;

	switch (mEncoding)
	{
		case NStringEncoding::UTF8:
			maxSize = (mSize / sizeof(utf8_t)) * sizeof(utf32_t);
			break;

		case NStringEncoding::UTF16:
			maxSize = (mSize / sizeof(utf16_t)) * sizeof(utf32_t);
			break;

		case NStringEncoding::UTF32:
			maxSize = (mSize / sizeof(utf32_t)) * sizeof(utf32_t);
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Invalid encoding!");
			break;
	}

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
//		NUnicodeView::DecodeUTF8 : Decode UTF8.
//-----------------------------------------------------------------------------
size_t NUnicodeView::DecodeUTF8(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const
{


	// Get the codepoint
	size_t charSize = 0;

	if (theSize >= 1)
	{
		// Single byte
		const utf8_t* nextUTF8 = reinterpret_cast<const utf8_t*>(theData);

		if (theSize >= 1 && ((nextUTF8[0] & 0b10000000) == 0b00000000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b01111111);

			codePoint = byte0;
			charSize  = 1;
		}


		// Two bytes
		else if (theSize >= 2 && ((nextUTF8[0] & 0b11100000) == 0b11000000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b00011111) << 6;
			utf32_t byte1 = utf32_t(nextUTF8[1] & 0b00111111) << 0;

			codePoint = byte0 | byte1;
			charSize  = 2;
		}


		// Three bytes
		else if (theSize >= 3 && ((nextUTF8[0] & 0b11110000) == 0b11100000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b00001111) << 12;
			utf32_t byte1 = utf32_t(nextUTF8[1] & 0b00111111) << 6;
			utf32_t byte2 = utf32_t(nextUTF8[2] & 0b00111111) << 0;

			codePoint = byte0 | byte1 | byte2;
			charSize  = 3;
		}


		// Four bytes
		else if (theSize >= 4 && ((nextUTF8[0] & 0b11111000) == 0b11110000))
		{
			utf32_t byte0 = utf32_t(nextUTF8[0] & 0b00000111) << 18;
			utf32_t byte1 = utf32_t(nextUTF8[1] & 0b00111111) << 12;
			utf32_t byte2 = utf32_t(nextUTF8[2] & 0b00111111) << 6;
			utf32_t byte3 = utf32_t(nextUTF8[3] & 0b00111111) << 0;

			codePoint = byte0 | byte1 | byte2 | byte3;
			charSize  = 4;
		}


		// Invalid byte
		else
		{
			codePoint = kNUTF32Replacement;
			charSize  = 1;
		}
	}

	return charSize;
}





//=============================================================================
//		NUnicodeView::DecodeUTF16 : Decode UTF16.
//-----------------------------------------------------------------------------
size_t NUnicodeView::DecodeUTF16(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const
{


	// Get the codepoint
	size_t charSize = 0;

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

					codePoint = 0x10000 + (pairHi | pairLo);
					charSize  = 4;
				}
				else
				{
					// Invalid low component
					codePoint = kNUTF32Replacement;
					charSize  = 2;
				}
			}
			else
			{
				// Missing low component
				codePoint = kNUTF32Replacement;
				charSize  = 2;
			}
		}


		// Individual codepoint
		else
		{
			codePoint = nextUTF16[0];
			charSize  = 2;
		}
	}

	return charSize;
}





//=============================================================================
//		NUnicodeView::DecodeUTF32 : Decode UTF32.
//-----------------------------------------------------------------------------
size_t NUnicodeView::DecodeUTF32(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const
{


	// Get the codepoint
	size_t charSize = 0;

	if (theSize >= 4)
	{
		const utf32_t* nextUTF32 = reinterpret_cast<const utf32_t*>(theData);

		codePoint = *nextUTF32;
		charSize  = 4;
	}

	return charSize;
}
