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
#include "NStringComparator.h"
#include "NStringEncoder.h"
#include "NThread.h"
#include "NUnicodeView.h"

// System
#include <atomic>
#include <cstddef>





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// Encoded string data
//
// The data for a string in a particular encoding.
struct NStringData
{
	std::atomic<struct NStringData*> nextData;
	NStringEncoding                  theEncoding;
	NData                            theData;
};


// Large string state
//
// Holds the state for large strings.
struct NStringState
{
	std::atomic_size_t numOwners;
	size_t             theSize;
	NStringData        stringData;
};





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
NString::NString(const utf32_t* theString)
	: mString{}
{


	// Set the text
	size_t theSize = std::char_traits<utf32_t>::length(theString);

	SetText(NStringEncoding::UTF32, theSize * sizeof(utf32_t), theString);
}





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
NString::NString(NStringEncoding theEncoding, size_t numBytes, const void* theData)
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
NString::NString(NStringEncoding theEncoding, const NData& theData)
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
NString::NString(const NString& otherString)
	: mString{}
{


	// Initialise ourselves
	MakeClone(otherString);
}





//=============================================================================
//		NString::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NString& NString::operator=(const NString& otherString)
{


	// Assign the string
	if (this != &otherString)
	{
		MakeClone(otherString);
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
	MakeClone(otherString);
	otherString.Clear();
}





//=============================================================================
//		NString::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NString& NString::operator=(NString&& otherString)
{


	// Move the string
	if (this != &otherString)
	{
		Clear();
		MakeClone(otherString);
		otherString.Clear();
	}

	return *this;
}





//=============================================================================
//		NString::Clear : Clear the string.
//-----------------------------------------------------------------------------
void NString::Clear()
{


	// Clear the string
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
	NN_REQUIRE(IsValidEncoding(theEncoding));



	// Get small text
	if (theEncoding == NStringEncoding::UTF8 && IsSmallUTF8())
	{
		return GetUTF8();
	}

	else if (theEncoding == NStringEncoding::UTF16 && IsSmallUTF16())
	{
		return GetUTF16();
	}



	// Get arbitrary text
	//
	// If we're using large storage, or an encoding that's not supported
	// by small storage, we return the contents of the encoded text data.
	//
	// We can cast away const as transcoding does not change our state.
	NString*     thisString = const_cast<NString*>(this);
	const NData* theData    = thisString->FetchEncoding(theEncoding);

	return theData->GetData();
}





//=============================================================================
//		NString::GetUTF32 : Get the text as UTF32.
//-----------------------------------------------------------------------------
const utf32_t* NString::GetUTF32() const
{


	// Get the text
	return static_cast<const utf32_t*>(GetText(NStringEncoding::UTF32));
}





//=============================================================================
//		NString::GetEncodings : Get the encodings held by the string.
//-----------------------------------------------------------------------------
NVectorStringEncoding NString::GetEncodings() const
{


	// Small encodings
	if (IsSmallUTF8())
	{
		return {NStringEncoding::UTF8};
	}

	else if (IsSmallUTF16())
	{
		return {NStringEncoding::UTF16};
	}



	// Large encodings
	const NStringData*    stringData = &mString.Large.theState->stringData;
	NVectorStringEncoding theEncodings;


	// Get the data
	while (stringData != nullptr)
	{
		theEncodings.push_back(stringData->theEncoding);
		stringData = stringData->nextData;
	}

	return theEncodings;
}





//=============================================================================
//		NString::GetData : Get the text.
//-----------------------------------------------------------------------------
NData NString::GetData(NStringEncoding theEncoding) const
{


	// Validate our parameters
	NN_REQUIRE(IsValidEncoding(theEncoding));



	// Get the state we need
	size_t theSize = GetSize();
	NData  theData;



	// Get small text
	//
	// Small text must be copied, although it will typically result
	// in a small storage data object as well.
	if (theEncoding == NStringEncoding::UTF8 && IsSmallUTF8())
	{
		theData.SetData(theSize * sizeof(utf8_t), GetUTF8());
	}

	else if (theEncoding == NStringEncoding::UTF16 && IsSmallUTF16())
	{
		theData.SetData(theSize * sizeof(utf16_t), GetUTF16());
	}



	// Get large text
	//
	// Large text may need to be transcoded, but any result can share
	// our internal storage since removing the terminator just needs
	// to shrink the slice of the returned data.
	//
	// We can cast away const as transcoding does not change our state.
	else
	{
		NString* thisString = const_cast<NString*>(this);

		theData = *(thisString->FetchEncoding(theEncoding));
		theData.SetSize(theData.GetSize() - NStringEncoder::GetCodeUnitSize(theEncoding));
	}

	return theData;
}





//=============================================================================
//		NString::SetData : Set the text.
//-----------------------------------------------------------------------------
void NString::SetData(NStringEncoding theEncoding, const NData& theData)
{


	// Validate our parameters
	NN_REQUIRE(IsValidEncoding(theEncoding));



	// Set the text
	SetText(theEncoding, theData.GetSize(), theData.GetData());



	// Update our state
	ClearHash();
}





//=============================================================================
//		NString::GetContent : Get the string content.
//-----------------------------------------------------------------------------
const void* NString::GetContent(NStringEncoding* theEncoding, size_t* theSize) const
{


	// Validate our parameters
	NN_REQUIRE(theEncoding != nullptr);
	NN_REQUIRE(theSize != nullptr);



	// Get small text
	//
	// Small text can return a pointer to the small storage.
	const void* theData = nullptr;

	if (IsSmall())
	{
		if (IsSmallUTF8())
		{
			*theEncoding = NStringEncoding::UTF8;
			*theSize     = GetSizeSmall() * sizeof(utf8_t);
			theData      = mString.Small.theData;
		}
		else
		{
			*theEncoding = NStringEncoding::UTF16;
			*theSize     = GetSizeSmall() * sizeof(utf16_t);
			theData      = mString.Small.theData;
		}
	}



	// Get large text
	//
	// Large text returns a pointer to the large storage, minus the final
	// terminator after the content.
	else
	{
		NN_REQUIRE(IsLarge());

		const NStringData& stringData = mString.Large.theState->stringData;

		*theEncoding = stringData.theEncoding;
		*theSize     = stringData.theData.GetSize() - NStringEncoder::GetCodeUnitSize(*theEncoding);
		theData      = stringData.theData.GetData();
	}

	return theData;
}





//=============================================================================
//		NString::Compare : Compare the string.
//-----------------------------------------------------------------------------
NComparison NString::Compare(const NString& theString, NStringFlags theFlags) const
{


	// Compare the string
	return NStringComparator::Compare(*this, theString, theFlags);
}





//=============================================================================
//		NString::EqualTo : Are two strings equal?
//-----------------------------------------------------------------------------
bool NString::EqualTo(const NString& theString, NStringFlags theFlags) const
{


	// Compare the strings
	return Compare(theString, theFlags) == NComparison::EqualTo;
}





//=============================================================================
//		NString::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NString::CompareEqual(const NString& theString) const
{


	// Compare the size
	//
	// A different size means no equality.
	if (GetSize() != theString.GetSize())
	{
		return false;
	}



	// Compare the hash
	//
	// A different hash means no equality.
	if (GetHash() != theString.GetHash())
	{
		return false;
	}



	// Compare the text
	//
	// A hash collision could produce equal hashes so check the content.
	return CompareOrder(theString) == NComparison::EqualTo;
}





//=============================================================================
//		NString::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NString::CompareOrder(const NString& theString) const
{


	// Order by comparison
	return Compare(theString, kNStringNone);
}





//=============================================================================
//		NString::+= : Concatenate operator.
//-----------------------------------------------------------------------------
const NString& NString::operator+=(const NString& theString)
{


	// Append the string
	//
	// Appended strings are normalized to UTF8 as this is the encoding
	// most likely to allow us to continue to use small storage.
	SetData(NStringEncoding::UTF8,
			GetData(NStringEncoding::UTF8) + theString.GetData(NStringEncoding::UTF8));

	return *this;
}





#pragma mark public
//=============================================================================
//		NString::UpdateHash : Update the hash.
//-----------------------------------------------------------------------------
size_t NString::UpdateHash(NHashAction theAction)
{


	// Update the hash
	//
	// We always use a 32-bit hash, even on 64-bit sysems, to maximise
	// storage space for small objects.
	switch (theAction)
	{
		case NHashAction::Get:
			break;

		case NHashAction::Clear:
			mString.theHash = 0;
			break;

		case NHashAction::Update:
			mString.theHash = NDataDigest::GetRuntime32(GetData(NStringEncoding::UTF8));
			break;
	}

	return mString.theHash;
}





#pragma mark private
//=============================================================================
//		NString::MakeClone : Make a clone of another object.
//-----------------------------------------------------------------------------
void NString::MakeClone(const NString& theString)
{


	// Validate our parameters and state
	NN_REQUIRE(this != &theString);



	// Copy the string
	mString = theString.mString;

	if (IsLarge())
	{
		RetainLarge();
	}
}





//=============================================================================
//		NString::MakeLarge : Make the string use large storage.
//-----------------------------------------------------------------------------
void NString::MakeLarge()
{


	// Switch to large storage
	//
	// Our small strings are all fixed-width so the number of codepoints
	// gives us the number of bytes directly.
	if (IsSmall())
	{
		size_t theSize = size_t(mString.theFlags & kNStringFlagSmallSizeMask);

		if (IsSmallUTF8())
		{
			SetTextLarge(NStringEncoding::UTF8, theSize * sizeof(utf8_t), mString.Small.theData);
		}
		else
		{
			SetTextLarge(NStringEncoding::UTF16, theSize * sizeof(utf16_t), mString.Small.theData);
		}
	}
}





//=============================================================================
//		NString::FetchEncoding : Fetch the string data in an encoding.
//-----------------------------------------------------------------------------
const NData* NString::FetchEncoding(NStringEncoding theEncoding)
{


	// Validate our parameters
	NN_REQUIRE(IsValidEncoding(theEncoding));



	// Update our state
	//
	// Dynamic encoding requires large string storage.
	MakeLarge();



	// Get the data
	//
	// If the data doesn't exist in this encoding then it must be added.
	const NData* theData = GetEncoding(theEncoding);
	if (theData == nullptr)
	{
		AddEncoding(theEncoding);

		theData = GetEncoding(theEncoding);
		NN_REQUIRE(theData != nullptr);
	}

	return theData;
}





//=============================================================================
//		NString::GetEncoding : Get the string data in an encoding.
//-----------------------------------------------------------------------------
const NData* NString::GetEncoding(NStringEncoding theEncoding) const
{


	// Validate our parameters and state
	NN_REQUIRE(IsValidEncoding(theEncoding));
	NN_REQUIRE(IsLarge());



	// Get the state we need
	const NStringData* stringData = &mString.Large.theState->stringData;



	// Get the data
	while (stringData != nullptr)
	{
		if (stringData->theEncoding == theEncoding)
		{
			return &stringData->theData;
		}

		stringData = stringData->nextData;
	}

	return nullptr;
}





//=============================================================================
//		NString::AddEncoding : Add a copy of the string data in an encoding.
//-----------------------------------------------------------------------------
void NString::AddEncoding(NStringEncoding theEncoding)
{


	// Validate our parameters and state
	NN_REQUIRE(IsValidEncoding(theEncoding));
	NN_REQUIRE(IsLarge());



	// Get the state we need
	NStringData* stringData = &mString.Large.theState->stringData;



	// Encode the string
	//
	// The canonical data has no BOM and already has a terminator so we
	// don't need any further processing beyond the content.
	NStringData* newData = new NStringData{};
	newData->theEncoding = theEncoding;

	NStringEncoder::Convert(stringData->theEncoding,
							stringData->theData,
							newData->theEncoding,
							newData->theData,
							kNStringEncoderNone);



	// Add the new encoding
	bool didSwap = false;

	do
	{
		NStringData* nextData = stringData->nextData.load();
		newData->nextData     = nextData;
		didSwap               = stringData->nextData.compare_exchange_strong(nextData, newData);

		if (!didSwap)
		{
			NThread::Pause();
		}
	} while (!didSwap);
}





//=============================================================================
//		NString::ReleaseEncodings : Release the encodings.
//-----------------------------------------------------------------------------
void NString::ReleaseEncodings()
{


	// Get the state we need
	NStringData* theData = mString.Large.theState->stringData.nextData.exchange(nullptr);



	// Release the encodings
	while (theData != nullptr)
	{
		NStringData* nextData = theData->nextData;

		delete theData;
		theData = nextData;
	}
}





//=============================================================================
//		NString::SetLarge : Set the large state.
//-----------------------------------------------------------------------------
void NString::SetLarge(NStringState* theState)
{


	// Set the large state
	mString.Large.theState = theState;

	mString.theFlags = kNStringFlagIsLarge;
}





//=============================================================================
//		NString::RetainLarge : Retain the large state.
//-----------------------------------------------------------------------------
void NString::RetainLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(mString.Large.theState->numOwners != 0);



	// Retain the state
	mString.Large.theState->numOwners += 1;
}





//=============================================================================
//		NString::ReleaseLarge : Release the large state.
//-----------------------------------------------------------------------------
void NString::ReleaseLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(mString.Large.theState->numOwners != 0);



	// Release the state
	//
	// The last owner releases the state.
	if (mString.Large.theState->numOwners.fetch_sub(1) == 1)
	{
		ReleaseEncodings();

		delete mString.Large.theState;
		mString.Large.theState = nullptr;
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
	// The small size is stored in the flag byte.
	return size_t(mString.theFlags & kNStringFlagSmallSizeMask);
}





//=============================================================================
//		NString::GetSizeLarge : Get the large size.
//-----------------------------------------------------------------------------
size_t NString::GetSizeLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the size
	return mString.Large.theState->theSize;
}





//=============================================================================
//		NString::SetText : Set the text.
//-----------------------------------------------------------------------------
void NString::SetText(NStringEncoding theEncoding, size_t numBytes, const void* theText)
{


	// Validate our parameters
	NN_REQUIRE(IsValidEncoding(theEncoding));



	// Check for ASCII
	//
	// ASCII text is auto-promoted to UTF8 as it is a simple subset.
	if (theEncoding == NStringEncoding::ASCII)
	{
		theEncoding = NStringEncoding::UTF8;
	}



	// Check for small storage
	bool setSmall = false;

	if (theEncoding == NStringEncoding::UTF8)
	{
		setSmall = IsValidSmallUTF8(numBytes, static_cast<const utf8_t*>(theText));
	}

	else if (theEncoding == NStringEncoding::UTF16)
	{
		setSmall = IsValidSmallUTF16(numBytes, static_cast<const utf16_t*>(theText));
	}



	// Set the text
	if (setSmall)
	{
		SetTextSmall(theEncoding, numBytes, theText);
	}
	else
	{
		SetTextLarge(theEncoding, numBytes, theText);
	}
}





//=============================================================================
//		NString::SetTextSmall : Set small text.
//-----------------------------------------------------------------------------
void NString::SetTextSmall(NStringEncoding theEncoding, size_t numBytes, const void* theText)
{


	// Validate our parameters
	NN_REQUIRE(theEncoding == NStringEncoding::UTF8 || theEncoding == NStringEncoding::UTF16);

	if (theEncoding == NStringEncoding::UTF8)
	{
		NN_REQUIRE((numBytes / sizeof(utf8_t)) <= kNStringSmallSizeMaxUTF8);
		NN_REQUIRE(IsValidSmallUTF8(numBytes, static_cast<const utf8_t*>(theText)));
	}
	else
	{
		NN_REQUIRE((numBytes / sizeof(utf16_t)) <= kNStringSmallSizeMaxUTF16);
		NN_REQUIRE(IsValidSmallUTF16(numBytes, static_cast<const utf16_t*>(theText)));
	}



	// Set when large
	//
	// Any existing large state must be released.
	if (IsLarge())
	{
		Clear();
	}



	// Set the text
	SetSmall(numBytes, theText, theEncoding);
}





//=============================================================================
//		NString::SetTextLarge : Set large text.
//-----------------------------------------------------------------------------
void NString::SetTextLarge(NStringEncoding theEncoding, size_t numBytes, const void* theText)
{


	// Check for existing state
	//
	// If we are the sole owner of some existing state then we can reuse
	// that state (and potentially its data) when assigning the content.
	//
	// Any alternative encodings are now obsolete so must be released.
	NStringState* existingState = nullptr;

	if (IsLarge())
	{
		if (mString.Large.theState->numOwners == 1)
		{
			ReleaseEncodings();

			existingState = mString.Large.theState;
		}
	}



	// Create the state
	NUnicodeView theView(theEncoding, numBytes, theText);

	NStringState* theState = existingState;

	if (theState == nullptr)
	{
		theState = new NStringState{};
	}

	theState->numOwners              = 1;
	theState->theSize                = theView.GetSize();
	theState->stringData.nextData    = nullptr;
	theState->stringData.theEncoding = theEncoding;



	// Create the data
	//
	// The canonical data maintains the original encoding and adds a null
	// terminator.
	//
	// An exception to this is endian-specific Unicode encodings. These are
	// converted to their native equivalent and stored without any BOM.
	NStringEncoder::Convert(theEncoding,
							NData(numBytes, theText),
							NStringEncoder::GetNativeEncoding(theEncoding),
							theState->stringData.theData,
							kNStringEncoderRemoveBOM | kNStringEncoderTerminator);



	// Switch to large data
	if (existingState == nullptr)
	{
		Clear();
	}

	SetLarge(theState);
}
