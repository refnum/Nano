/*	NAME:
		NUnicodeView.inl

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
#include "NDebug.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUTF32Iterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type        =           utf32_t;
	using difference_type   =           utf32_t;
	using pointer           = const utf32_t*;
	using reference         = utf32_t&;


public:
	constexpr                           NUTF32Iterator(const NUnicodeView* theView, size_t theOffset);


	// Operators
	inline utf32_t                      operator*() const;

	inline NUTF32Iterator&              operator++();
	inline NUTF32Iterator               operator++(int);

	inline NUTF32Iterator               operator+(size_t n) const;

	inline bool                         operator==(const NUTF32Iterator& otherIter) const;
	inline bool                         operator!=(const NUTF32Iterator& otherIter) const;


private:
	const NUnicodeView*                 mView;
	size_t                              mOffset;
};





//=============================================================================
//		NUTF32Iterator::NUTF32Iterator : Constructor.
//-----------------------------------------------------------------------------
constexpr NUTF32Iterator::NUTF32Iterator(const NUnicodeView* theView, size_t theOffset)
	: mView(theView)
	, mOffset(theOffset)
{
}





//=============================================================================
//		NUTF32Iterator::operator* : Dereference operator.
//-----------------------------------------------------------------------------
inline utf32_t NUTF32Iterator::operator*() const
{


	// Get the code point
	return mView->GetCodePoint(mOffset);
}





//=============================================================================
//		NUTF32Iterator::operator++ : Prefix increment operator.
//-----------------------------------------------------------------------------
inline NUTF32Iterator& NUTF32Iterator::operator++()
{


	// Advance the iterator
	mOffset += mView->GetCodePointSize(mOffset);

	return *this;
}





//=============================================================================
//		NUTF32Iterator::operator++ : Postfix increment operator.
//-----------------------------------------------------------------------------
inline NUTF32Iterator NUTF32Iterator::operator++(int)
{


	// Advance the iterator
	NUTF32Iterator prevIter = *this;

	mOffset += mView->GetCodePointSize(mOffset);

	return prevIter;
}





//=============================================================================
//		NUTF32Iterator::operator+ : Addition operator.
//-----------------------------------------------------------------------------
inline NUTF32Iterator NUTF32Iterator::operator+(size_t n) const
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
inline bool NUTF32Iterator::operator==(const NUTF32Iterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(mView == otherIter.mView);


	// Compare the iterator
	return mOffset == otherIter.mOffset;
}





//=============================================================================
//		NUTF32Iterator::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
inline bool NUTF32Iterator::operator!=(const NUTF32Iterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(mView == otherIter.mView);


	// Compare the iterator
	return mOffset != otherIter.mOffset;
}





#pragma mark NUnicodeView
//=============================================================================
//		NUnicodeView::NUnicodeView : Constructor.
//-----------------------------------------------------------------------------
inline NUnicodeView::NUnicodeView(NStringEncoding theEncoding, size_t theSize, const void* theData)
	: mEncoding(theEncoding)
	, mSize(theSize)
	, mData(static_cast<const uint8_t*>(theData))
{


	// Validate our state
	NN_REQUIRE(IsValid());
}





//=============================================================================
//		NUnicodeView::GetCodePoint : Get a code point.
//-----------------------------------------------------------------------------
inline utf32_t NUnicodeView::GetCodePoint(size_t theOffset) const
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
inline size_t NUnicodeView::GetCodePointSize(size_t theOffset) const
{


	// Validate our parameters
	NN_REQUIRE(theOffset < mSize);



	// Get the code point size
	utf32_t codePoint = 0;

	return DecodeUTF(theOffset, codePoint);
}





//=============================================================================
//		NUnicodeView::begin : Get the begin iterator.
//-----------------------------------------------------------------------------
constexpr NUTF32Iterator NUnicodeView::begin() const
{


	// Get the iterator
	return NUTF32Iterator(this, 0);
}





//=============================================================================
//		NUnicodeView::end : Get the end iterator.
//-----------------------------------------------------------------------------
constexpr NUTF32Iterator NUnicodeView::end() const
{


	// Get the iterator
	return NUTF32Iterator(this, mSize);
}





#pragma mark private
//=============================================================================
//		NUnicodeView::IsValid : Is the view valid?
//-----------------------------------------------------------------------------
constexpr bool NUnicodeView::IsValid() const
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
