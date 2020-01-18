/*	NAME:
		NString.inl

	DESCRIPTION:
		String object.

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
// System
#include <cstddef>
#include <string>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint8_t kNStringFlagIsLarge                = 0b00000001;
static constexpr uint8_t kNStringFlagIsUTF16                = 0b00000010;
static constexpr uint8_t kNStringSmallSizeMask              = 0b11111000;
static constexpr uint8_t kNStringSmallSizeShift             = 3;
static constexpr uint8_t kNStringSmallSizeMax               = 23;

static constexpr utf8_t kNStringUTF8Null                    = utf8_t(0x00);
static constexpr utf8_t kNStringUTF8Variable                = utf8_t(0x80U);

static constexpr utf16_t kNStringUTF16Null                  = utf16_t(0x0000);
static constexpr utf16_t kNStringUTF16SurrogateStart        = utf16_t(0xD800);
static constexpr utf16_t kNStringUTF16SurrogateEnd          = utf16_t(0xDFFF);





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
constexpr NString::NString(const utf8_t* theString)
	: mString{}
{


	// Initialise ourselves
	//
	// constexpr construction allows compile-time creation from string literals.
	size_t theSize = std::char_traits<utf8_t>::length(theString) + 1;

	if (!SetSmallUTF8(theSize, theString))
	{
		SetText(NStringEncoding::UTF8, theSize * sizeof(utf8_t), theString);
	}
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
constexpr NString::NString(const utf16_t* theString)
	: mString{}
{


	// Initialise ourselves
	//
	// constexpr construction allows compile-time creation from string literals.
	size_t theSize = std::char_traits<utf16_t>::length(theString) + 1;

	if (!SetSmallUTF16(theSize, theString))
	{
		SetText(NStringEncoding::UTF16, theSize * sizeof(utf16_t), theString);
	}
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
NString::NString()
	: mString{}
{


	// Validate our state
	static_assert(sizeof(NStringStorage) == 32);
	static_assert(sizeof(mString.Small.theData) == kNStringSmallSizeMax);

	static_assert(offsetof(NStringStorage, Small.sizeFlags) == 0);
	static_assert(offsetof(NStringStorage, Small.theData) == 1);
	static_assert(offsetof(NStringStorage, Large.theState) == 0);
	static_assert(offsetof(NStringStorage, Large.reserved) == 8);
	static_assert(offsetof(NStringStorage, theHash) == 24);

	static_assert(alignof(std::max_align_t) > 1, "Large flag requires LSB be free");
	static_assert(NN_ENDIAN_LITTLE, "Small/Large flag no longer overlap!");
}





//=============================================================================
//		NString::~NString : Destructor.
//-----------------------------------------------------------------------------
NString::~NString()
{


	// Clean up
	if (IsLarge())
	{
		ReleaseLarge();
	}
}





//=============================================================================
//		NString::GetUTF8 : Get the text as UTF8.
//-----------------------------------------------------------------------------
constexpr const utf8_t* NString::GetUTF8() const
{


	// Get the text
	if (IsSmallUTF8())
	{
		return reinterpret_cast<const utf8_t*>(mString.Small.theData);
	}
	else
	{
		return static_cast<const utf8_t*>(GetText(NStringEncoding::UTF8));
	}
}





//=============================================================================
//		NString::GetUTF16 : Get the text as UTF16.
//-----------------------------------------------------------------------------
constexpr const utf16_t* NString::GetUTF16() const
{


	// Get the text
	if (IsSmallUTF16())
	{
		return reinterpret_cast<const utf16_t*>(mString.Small.theData);
	}
	else
	{
		return static_cast<const utf16_t*>(GetText(NStringEncoding::UTF16));
	}
}





#pragma mark private
//=============================================================================
//		NString::IsSmall : Are we using small storage?
//-----------------------------------------------------------------------------
constexpr bool NString::IsSmall() const
{


	// Check the flag
	return (mString.Small.sizeFlags & kNStringFlagIsLarge) == 0;
}





//=============================================================================
//		NString::IsSmallUTF8 : Are we a small UTF8 string?
//-----------------------------------------------------------------------------
constexpr bool NString::IsSmallUTF8() const
{


	// Check the flag
	return IsSmall() && ((mString.Small.sizeFlags & kNStringFlagIsUTF16) == 0);
}





//=============================================================================
//		NString::IsSmallUTF16 : Are we a small UTF16 string?
//-----------------------------------------------------------------------------
constexpr bool NString::IsSmallUTF16() const
{


	// Check the flag
	return IsSmall() && ((mString.Small.sizeFlags & kNStringFlagIsUTF16) != 0);
}





//=============================================================================
//		NString::IsLarge : Are we using large storage?
//-----------------------------------------------------------------------------
constexpr bool NString::IsLarge() const
{


	// Check the flag
	return !IsSmall();
}





//=============================================================================
//		NString::IsValidEncoding : Is this a valid encoding?
//-----------------------------------------------------------------------------
constexpr bool NString::IsValidEncoding(NStringEncoding theEncoding) const
{


	// Check the encoding
	return theEncoding != NStringEncoding::Unknown;
}





//=============================================================================
//		NString::IsFixedWidthUTF8 : Check for fixed-width UTF8.
//-----------------------------------------------------------------------------
constexpr bool NString::IsFixedWidthUTF8(size_t numUTF8, const utf8_t* textUTF8) const
{


	// Check the text
	for (size_t n = 0; n < numUTF8; n++)
	{
		if (textUTF8[n] & kNStringUTF8Variable)
		{
			return false;
		}
	}

	return true;
}





//=============================================================================
//		NString::IsFixedWidthUTF16 : Check for fixed-width UTF16.
//-----------------------------------------------------------------------------
constexpr bool NString::IsFixedWidthUTF16(size_t numUTF16, const utf16_t* textUTF16) const
{


	// Check the text
	for (size_t n = 0; n < numUTF16; n++)
	{
		if (textUTF16[n] >= kNStringUTF16SurrogateStart &&
			textUTF16[n] <= kNStringUTF16SurrogateEnd)
		{
			return false;
		}
	}

	return true;
}





//=============================================================================
//		NString::SetSmallUTF8 : Attempt to set a UTF8 string as small.
//-----------------------------------------------------------------------------
constexpr bool NString::SetSmallUTF8(size_t numUTF8, const utf8_t* textUTF8)
{


	// Initialise the string
	size_t numBytes = numUTF8 * sizeof(utf8_t);

	if (numBytes < kNStringSmallSizeMax && IsFixedWidthUTF8(numUTF8, textUTF8))
	{
		SetSmall(numBytes, textUTF8, NStringEncoding::UTF8);
		return true;
	}

	return false;
}





//=============================================================================
//		NString::SetSmallUTF16 : Attempt to set a UTF16 string as small.
//-----------------------------------------------------------------------------
constexpr bool NString::SetSmallUTF16(size_t numUTF16, const utf16_t* textUTF16)
{


	// Initialise the string
	size_t numBytes = numUTF16 * sizeof(utf16_t);

	if (numBytes < kNStringSmallSizeMax && IsFixedWidthUTF16(numUTF16, textUTF16))
	{
		SetSmall(numBytes, textUTF16, NStringEncoding::UTF16);
		return true;
	}

	return false;
}





//=============================================================================
//		NString::SetSmall : Set a small string.
//-----------------------------------------------------------------------------
constexpr void NString::SetSmall(size_t numBytes, const void* theText, NStringEncoding theEncoding)
{


	// Validate our parameters
	NN_EXPECT(numBytes < kNStringSmallSizeMax);
	NN_EXPECT(theEncoding == NStringEncoding::UTF8 || theEncoding == NStringEncoding::UTF16);



	// Initialise the flags
	mString.Small.sizeFlags = uint8_t(numBytes << kNStringSmallSizeShift);

	if (theEncoding == NStringEncoding::UTF16)
	{
		mString.Small.sizeFlags |= kNStringFlagIsUTF16;
	}



	// Copy the data
	const uint8_t* theData = static_cast<const uint8_t*>(theText);

	for (size_t n = 0; n < numBytes; n++)
	{
		mString.Small.theData[n] = theData[n];
	}
}
