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
// Nano
#include "NStringFormatter.h"

// System
#include <cstddef>
#include <string>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint8_t kNStringFlagIsLarge                = 0b10000000;
static constexpr uint8_t kNStringFlagIsSmallUTF16           = 0b01000000;
static constexpr uint8_t kNStringFlagSmallSizeMask          = 0b00011111;

static constexpr uint8_t kNStringSmallSizeMaxUTF8           = 26;
static constexpr uint8_t kNStringSmallSizeMaxUTF16          = 12;





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
constexpr NString::NString(const utf8_t* theString)
	: mString{}
{


	// Initialise ourselves
	//
	// constexpr construction allows compile-time creation from string literals.
	size_t numBytes = std::char_traits<utf8_t>::length(theString) * sizeof(utf8_t);

	if (!SetSmallUTF8(numBytes, theString))
	{
		SetText(NStringEncoding::UTF8, numBytes, theString);
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
	size_t numBytes = std::char_traits<utf16_t>::length(theString) * sizeof(utf16_t);

	if (!SetSmallUTF16(numBytes, theString))
	{
		SetText(NStringEncoding::UTF16, numBytes, theString);
	}
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
inline NString::NString(const utf32_t* theString)
	: mString{}
{


	// Set the text
	size_t theSize = std::char_traits<utf32_t>::length(theString);

	SetText(NStringEncoding::UTF32, theSize * sizeof(utf32_t), theString);
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
inline NString::NString(NStringEncoding theEncoding, size_t numBytes, const void* theData)
	: mString{}
{


	// Validate our parameters
	NN_REQUIRE(IsValidEncoding(theEncoding));
	NN_REQUIRE(numBytes == 0 || (numBytes != 0 && theData != nullptr));



	// Set the text
	SetText(theEncoding, numBytes, theData);
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
inline NString::NString(NStringEncoding theEncoding, const NData& theData)
	: mString{}
{


	// Validate our parameters
	NN_REQUIRE(IsValidEncoding(theEncoding));



	// Set the text
	SetData(theEncoding, theData);
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
inline NString::NString()
	: mString{}
{


	// Validate our state
	static_assert(sizeof(NStringStorage) == 32);

	static_assert(sizeof(mString.Small.theData) >=
				  ((kNStringSmallSizeMaxUTF8 + 1) * sizeof(utf8_t)));

	static_assert(sizeof(mString.Small.theData) >=
				  ((kNStringSmallSizeMaxUTF16 + 1) * sizeof(utf16_t)));

	static_assert(offsetof(NStringStorage, Small.theData) == 0);
	static_assert(offsetof(NStringStorage, Large.theState) == 0);

	static_assert(offsetof(NStringStorage, theFlags) == 27);
	static_assert(offsetof(NStringStorage, theHash) == 28);
}





//=============================================================================
//		NString::~NString : Destructor.
//-----------------------------------------------------------------------------
inline NString::~NString()
{


	// Clean up
	if (IsLarge())
	{
		ReleaseLarge();
	}
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
inline NString::NString(const NString& otherString)
	: mString{otherString.mString}
{


	// Initialise ourselves
	if (IsLarge())
	{
		RetainLarge();
	}
}





//=============================================================================
//		NString::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NString& NString::operator=(const NString& otherString)
{


	// Assign the string
	if (this != &otherString)
	{
		if (IsLarge())
		{
			ReleaseLarge();
		}

		mString = otherString.mString;

		if (IsLarge())
		{
			RetainLarge();
		}
	}

	return *this;
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
inline NString::NString(NString&& otherString)
	: mString{}
{


	// Initialise ourselves
	std::swap(mString, otherString.mString);
}





//=============================================================================
//		NString::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NString& NString::operator=(NString&& otherString)
{


	// Move the string
	if (this != &otherString)
	{
		std::swap(mString, otherString.mString);
		otherString.Clear();
	}

	return *this;
}





//=============================================================================
//		NString::Clear : Clear the string.
//-----------------------------------------------------------------------------
inline void NString::Clear()
{


	// Clear the string
	if (IsLarge())
	{
		ReleaseLarge();
	}

	mString = {};
}





//=============================================================================
//		NString::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline size_t NString::GetSize() const
{


	// Get the size
	if (IsSmall())
	{
		return GetSizeSmall();
	}
	else
	{
		return GetSizeLarge();
	}
}





//=============================================================================
//		NString::GetUTF8 : Get the text as UTF8.
//-----------------------------------------------------------------------------
inline const utf8_t* NString::GetUTF8() const
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
inline const utf16_t* NString::GetUTF16() const
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





#pragma mark NMixinHashable
//=============================================================================
//		NString::HashGet : Get the hash.
//-----------------------------------------------------------------------------
inline size_t NString::HashGet() const
{


	// Get the hash
	return mString.theHash;
}





//=============================================================================
//		NString::HashClear : Clear the hash.
//-----------------------------------------------------------------------------
inline void NString::HashClear()
{


	// Clear the hash
	mString.theHash = 0;
}





#pragma mark private
//=============================================================================
//		NString::IsSmall : Are we using small storage?
//-----------------------------------------------------------------------------
constexpr bool NString::IsSmall() const
{


	// Check the flag
	return !IsLarge();
}





//=============================================================================
//		NString::IsSmallUTF8 : Are we a small UTF8 string?
//-----------------------------------------------------------------------------
constexpr bool NString::IsSmallUTF8() const
{


	// Check the flag
	return IsSmall() && ((mString.theFlags & kNStringFlagIsSmallUTF16) == 0);
}





//=============================================================================
//		NString::IsSmallUTF16 : Are we a small UTF16 string?
//-----------------------------------------------------------------------------
constexpr bool NString::IsSmallUTF16() const
{


	// Check the flag
	return IsSmall() && ((mString.theFlags & kNStringFlagIsSmallUTF16) != 0);
}





//=============================================================================
//		NString::IsLarge : Are we using large storage?
//-----------------------------------------------------------------------------
constexpr bool NString::IsLarge() const
{


	// Check the flag
	return (mString.theFlags & kNStringFlagIsLarge) != 0;
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
//		NString::IsValidSmallUTF8 : Check for a small UTF8 string.
//-----------------------------------------------------------------------------
constexpr bool NString::IsValidSmallUTF8(size_t numBytes, const utf8_t* textUTF8) const
{


	// Check our parameters
	if ((numBytes % sizeof(utf8_t)) != 0)
	{
		return false;
	}



	// Check the size
	size_t numUTF8 = numBytes / sizeof(utf8_t);

	if (numUTF8 > kNStringSmallSizeMaxUTF8)
	{
		return false;
	}



	// Check the text
	for (size_t n = 0; n < numUTF8; n++)
	{
		if (textUTF8[n] & kNUTF8VariableWidth)
		{
			return false;
		}
	}

	return true;
}





//=============================================================================
//		NString::IsValidSmallUTF16 : Check for a small UTF16 string.
//-----------------------------------------------------------------------------
constexpr bool NString::IsValidSmallUTF16(size_t numBytes, const utf16_t* textUTF16) const
{


	// Check our parameters
	if ((numBytes % sizeof(utf16_t)) != 0)
	{
		return false;
	}



	// Check the size
	size_t numUTF16 = numBytes / sizeof(utf16_t);

	if (numUTF16 > kNStringSmallSizeMaxUTF16)
	{
		return false;
	}



	// Check the text
	for (size_t n = 0; n < numUTF16; n++)
	{
		if (textUTF16[n] >= kNUTF16SurrogatesStart && textUTF16[n] <= kNUTF16SurrogatesEnd)
		{
			return false;
		}
	}

	return true;
}





//=============================================================================
//		NString::SetSmallUTF8 : Attempt to set a UTF8 string as small.
//-----------------------------------------------------------------------------
constexpr bool NString::SetSmallUTF8(size_t numBytes, const utf8_t* textUTF8)
{


	// Initialise the string
	if (IsValidSmallUTF8(numBytes, textUTF8))
	{
		SetSmall(numBytes, textUTF8, NStringEncoding::UTF8);
		return true;
	}

	return false;
}





//=============================================================================
//		NString::SetSmallUTF16 : Attempt to set a UTF16 string as small.
//-----------------------------------------------------------------------------
constexpr bool NString::SetSmallUTF16(size_t numBytes, const utf16_t* textUTF16)
{


	// Initialise the string
	if (IsValidSmallUTF16(numBytes, textUTF16))
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


	// Set the flags
	size_t codeUnitSize = (theEncoding == NStringEncoding::UTF8) ? sizeof(utf8_t) : sizeof(utf16_t);
	size_t numCodePoints = numBytes / codeUnitSize;

	mString.theFlags = uint8_t(numCodePoints);

	if (theEncoding == NStringEncoding::UTF16)
	{
		mString.theFlags |= kNStringFlagIsSmallUTF16;
	}



	// Copy the data
	//
	// This is our constexpr path to allow in-place construction
	// from string literals so cannot use memcpy.
	//
	// When used at runtime we also zero-fill to avoid leaving
	// previous string data in our storage.
	const uint8_t* theData = static_cast<const uint8_t*>(theText);
	size_t         n       = 0;

	while (n < numBytes)
	{
		mString.Small.theData[n] = theData[n];
		n++;
	}

	while (n < sizeof(mString.Small.theData))
	{
		mString.Small.theData[n] = 0x00;
		n++;
	}
}





//=============================================================================
//		NString::GetSizeSmall : Get the small size.
//-----------------------------------------------------------------------------
inline size_t NString::GetSizeSmall() const
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Get the size
	//
	// The small size is stored in the flag byte.
	return size_t(mString.theFlags & kNStringFlagSmallSizeMask);
}





//=============================================================================
//		NString::GetSizeLarge : Get the large size.
//-----------------------------------------------------------------------------
inline size_t NString::GetSizeLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the size
	return mString.Large.theSlice.GetSize();
}





//=============================================================================
//		Global operator+ : Append a string literal.
//-----------------------------------------------------------------------------
template<typename T>
NString operator+(const T* theLiteral, const NString& theString)
{


	// Append the string
	return NString(theLiteral) + theString;
}





#pragma mark NFormat
//=============================================================================
//		NString formatter
//-----------------------------------------------------------------------------
template<>
class fmt::formatter<NString> : public NSimpleFormatter
{
public:
	template<typename FormatContext>
	auto format(const NString& theParam, FormatContext& theContext)
	{
		return format_to(theContext.out(), theParam.GetUTF8());
	}
};
