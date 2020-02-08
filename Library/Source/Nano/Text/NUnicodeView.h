/*	NAME:
		NUnicodeView.h

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
#ifndef NUNICODE_VIEW_H
#define NUNICODE_VIEW_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NStringEncodings.h"
#include "NanoConstants.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
class NString;
class NUnicodeView;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUTF32Iterator
{
public:
										NUTF32Iterator(const NUnicodeView* theView, size_t theOffset);


	// Operators
	utf32_t                             operator*() const;
	void                                operator++();
	bool                                operator!=(const NUTF32Iterator& otherIter) const;


private:
	const NUnicodeView*                 mView;
	size_t                              mOffset;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUnicodeView
{
public:
										NUnicodeView(NStringEncoding theEncoding, size_t theSize, const void* theData);
										NUnicodeView(const NString& theString);

									   ~NUnicodeView() = default;

										NUnicodeView(const NUnicodeView& otherView) = delete;
	NUnicodeView&                       operator=(   const NUnicodeView& otherView) = delete;

										NUnicodeView(NUnicodeView&& otherView) = delete;
	NUnicodeView&                       operator=(   NUnicodeView&& otherView) = delete;


	// Get a code point
	//
	// Returns 0 if no code point exists at the offset.
	utf32_t                             GetCodePoint(size_t theOffset) const;


	// Get the size of a code point
	//
	// Returns 0 if no code point exists at the offset.
	size_t                              GetCodePointSize(size_t theOffset) const;


	// Get the codepoint offsets
	//
	// Returns the byte offset for the specified number of codepoints.
	NVectorSize                         GetCodePointOffsets(size_t maxSize = kNSizeMax) const;


	// Get the size
	//
	// Returns the number of code points in the text.
	size_t                              GetSize() const;


	// Get the maximum size
	//
	// Returns the maximum number of code points in the text.
	size_t                              GetMaxSize() const;


	// Get UTF32 iterators
	//
	// Returns iterators suitable for a range-based for.
	NUTF32Iterator                      begin() const;
	NUTF32Iterator                      end()   const;


private:
	bool                                IsValid() const;

	size_t                              DecodeUTF(  size_t theOffset, utf32_t& codePoint) const;
	size_t                              DecodeUTF8( size_t theSize, const uint8_t* theData, utf32_t& codePoint) const;
	size_t                              DecodeUTF16(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const;
	size_t                              DecodeUTF32(size_t theSize, const uint8_t* theData, utf32_t& codePoint) const;


private:
	NStringEncoding                     mEncoding;
	size_t                              mSize;
	const uint8_t*                      mData;
};



#endif // NUNICODE_VIEW_H
