/*	NAME:
		NString.cpp

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
#include "NString.h"

// Nano
#include "NData.h"
#include "NDataDigest.h"

// System
#include <atomic>
#include <cstddef>





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// Large string state
//
// Holds the state for large strings.
struct NStringState
{
	std::atomic_size_t numOwners;
	NStringEncoding    theEncoding;
	size_t             theSize;
	NData              theData;
};





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
NString::NString(const utf32_t* theString)
	: mString{}
{


	// Validate our state
	static_assert(sizeof(NStringStorage) == 32);
	static_assert(offsetof(NStringStorage, Small.sizeFlags) == 0);
	static_assert(offsetof(NStringStorage, Small.theData) == 1);
	static_assert(offsetof(NStringStorage, Large.theState) == 0);
	static_assert(offsetof(NStringStorage, Large.reserved) == 8);
	static_assert(offsetof(NStringStorage, theHash) == 24);

	static_assert(sizeof(mString.Small.theData) == kNStringSmallSizeMax);

	static_assert(alignof(std::max_align_t) > 1, "Large flag requires LSB be free");
	static_assert(NN_ENDIAN_LITTLE, "Small/Large flag no longer overlap!");
	static_assert(kNStringEmptyUTF8 == 0, "Small UTF8 should be zero-initialised");



	// Set the text
	size_t theSize = std::char_traits<utf32_t>::length(theString) + 1;

	SetText(theSize * sizeof(utf32_t), theString, NStringEncoding::UTF16);
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
NString::NString(const NString& otherString)
	: mString{}
{


	// Initialise ourselves
	MakeCopy(otherString);
}





//=============================================================================
//		NString::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NString& NString::operator=(const NString& otherString)
{


	// Assign the value
	if (this != &otherString)
	{
		MakeCopy(otherString);
	}

	return *this;
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
NString::NString(NString&& otherString)
	: mString{}
{


	// Initialise ourselves
	MakeCopy(otherString);
	otherString.Clear();
}





//=============================================================================
//		NString::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NString& NString::operator=(NString&& otherString)
{


	// Move the value
	if (this != &otherString)
	{
		Clear();
		MakeCopy(otherString);
		otherString.Clear();
	}

	return *this;
}





//=============================================================================
//		NString::Clear : Clear the string.
//-----------------------------------------------------------------------------
void NString::Clear()
{


	// Clear the data
	if (IsLarge())
	{
		ReleaseLarge();
	}

	memset(&mString, 0x00, sizeof(mString));
}





//=============================================================================
//		NString::GetSize : Get the size.
//-----------------------------------------------------------------------------
size_t NString::GetSize() const
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
//		NString::GetText : Get the text.
//-----------------------------------------------------------------------------
const void* NString::GetText(NStringEncoding theEncoding) const
{


	// Validate our parameters
	NN_EXPECT(theEncoding != NStringEncoding::None);



	// Get the text
	switch (theEncoding)
	{
		case NStringEncoding::None:
			return nullptr;
			break;

		case NStringEncoding::UTF8:
			if (IsSmall())
			{
				return GetUTF8();
			}
			else
			{
				// TODO
				return nullptr;
			}
			break;

		case NStringEncoding::UTF16:
			if (IsSmall())
			{
				return GetUTF16();
			}
			else
			{
				// TODO
				return nullptr;
			}
			break;

		case NStringEncoding::UTF32:
			// TODO
			return nullptr;
			break;
	}

	return nullptr;
}





//=============================================================================
//		NString::GetUTF32 : Get the text as UTF32.
//-----------------------------------------------------------------------------
const utf32_t* NString::GetUTF32() const
{


	// Get the text
	return static_cast<const utf32_t*>(GetText(NStringEncoding::UTF32));
}





#pragma mark public
//=============================================================================
//		NString::FetchHash : Fetch the hash.
//-----------------------------------------------------------------------------
size_t& NString::FetchHash(bool updateHash) const
{


	// Fetch the hash
	if (updateHash)
	{
		const utf8_t* theText = GetUTF8();
		mString.theHash       = NDataDigest::GetRuntime(strlen(theText), theText);
	}

	return mString.theHash;
}





#pragma mark private
//=============================================================================
//		NString::MakeCopy : Make a copy of another object.
//-----------------------------------------------------------------------------
void NString::MakeCopy(const NString& otherString)
{


	// Validate our parameters and state
	NN_REQUIRE(this != &otherString);
	NN_REQUIRE(IsSmall());



	// Copy the string
	mString = otherString.mString;

	if (IsLarge())
	{
		RetainLarge();
	}
}





//=============================================================================
//		NString::GetLarge : Get the large state.
//-----------------------------------------------------------------------------
NStringState* NString::GetLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the large state
	const NStringState* theState = mString.Large.theState;
	uintptr_t statePtr = reinterpret_cast<uintptr_t>(theState) & ~uintptr_t(kNStringFlagIsLarge);

	return reinterpret_cast<NStringState*>(statePtr);
}





//=============================================================================
//		NString::SetLarge : Get the large state.
//-----------------------------------------------------------------------------
void NString::SetLarge(NStringState* theState)
{


	// Set the large state
	uintptr_t statePtr     = reinterpret_cast<uintptr_t>(theState) | uintptr_t(kNStringFlagIsLarge);
	mString.Large.theState = reinterpret_cast<NStringState*>(statePtr);



	// Validate our state
	NN_REQUIRE(IsLarge());
}





//=============================================================================
//		NString::RetainLarge : Retain the large state.
//-----------------------------------------------------------------------------
void NString::RetainLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the state we need
	NStringState* largeState = GetLarge();
	NN_REQUIRE(largeState->numOwners != 0);



	// Retain the state
	largeState->numOwners += 1;
}





//=============================================================================
//		NString::ReleaseLarge : Release the large state.
//-----------------------------------------------------------------------------
void NString::ReleaseLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the state we need
	NStringState* largeState = GetLarge();
	NN_REQUIRE(largeState->numOwners != 0);



	// Release the state
	//
	// The last owner releases the state.
	if (largeState->numOwners.fetch_sub(1) == 1)
	{
		delete largeState;
	}
}





//=============================================================================
//		NString::GetSizeSmall : Get the small size.
//-----------------------------------------------------------------------------
size_t NString::GetSizeSmall() const
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Get the size
	//
	// The size is stored in the top bits of the storage flag byte, and
	// holds the number of used bytes including the terminating null.
	//
	// As we only use small stoarge for fixed-width strings we can get the
	// size in codepoints by multiplying by the element size.
	size_t theSize =
		size_t((mString.Small.sizeFlags & kNStringSmallSizeMask) >> kNStringSmallSizeShift);

	if (mString.Small.sizeFlags & kNStringFlagIsUTF16)
	{
		theSize *= sizeof(utf16_t);
	}
	else
	{
		theSize *= sizeof(utf8_t);
	}



	// Remove the terminator
	if (theSize > 0)
	{
		theSize--;
	}

	return theSize;
}





//=============================================================================
//		NString::GetSizeLarge : Get the large size.
//-----------------------------------------------------------------------------
size_t NString::GetSizeLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the size
	//
	// We can cast away const as we only need to read the size.
	NString*      thisString = const_cast<NString*>(this);
	NStringState* largeState = thisString->GetLarge();

	return largeState->theSize;
}





//=============================================================================
//		NString::SetText : Set the text.
//-----------------------------------------------------------------------------
void NString::SetText(size_t numBytes, const void* theText, NStringEncoding theEncoding)
{


	// Get the state we need
	bool isSmall = (numBytes <= kNStringSmallSizeMax);

	if (isSmall)
	{
		switch (theEncoding)
		{
			case NStringEncoding::UTF8:
				isSmall = IsFixedWidthUTF8(numBytes, static_cast<const utf8_t*>(theText));
				break;

			case NStringEncoding::UTF16:
				isSmall = IsFixedWidthUTF16(numBytes, static_cast<const utf16_t*>(theText));
				break;

			default:
				isSmall = false;
				break;
		}
	}



	// Set the text
	if (isSmall)
	{
		SetTextSmall(numBytes, theText, theEncoding);
	}
	else
	{
		SetTextLarge(numBytes, theText, theEncoding);
	}
}





//=============================================================================
//		NString::SetTextSmall : Set small text.
//-----------------------------------------------------------------------------
void NString::SetTextSmall(size_t numBytes, const void* theText, NStringEncoding theEncoding)
{


	// Validate our parameters
	NN_REQUIRE(numBytes <= kNStringSmallSizeMax);
	NN_REQUIRE(theEncoding == NStringEncoding::UTF8 || theEncoding == NStringEncoding::UTF16);

	if (theEncoding == NStringEncoding::UTF8)
	{
		NN_REQUIRE(IsFixedWidthUTF8(numBytes, static_cast<const utf8_t*>(theText)));
	}
	else
	{
		NN_REQUIRE(IsFixedWidthUTF16(numBytes, static_cast<const utf16_t*>(theText)));
	}



	// Set the text
	SetSmall(numBytes, theText, theEncoding);
}





//=============================================================================
//		NString::SetTextLarge : Set large text.
//-----------------------------------------------------------------------------
void NString::SetTextLarge(size_t numBytes, const void* theText, NStringEncoding theEncoding)
{


	// Validate our parameters
	NN_REQUIRE(numBytes > kNStringSmallSizeMax);



	// Create the state
	NStringState* theState = new NStringState;

	theState->numOwners   = 1;
	theState->theEncoding = theEncoding;
	theState->theSize     = 0;
	theState->theData.SetData(numBytes, theText);



	// Swtich to large data
	SetLarge(theState);
}
