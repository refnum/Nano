/*	NAME:
		NString.h

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
#ifndef NSTRING_H
#define NSTRING_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NMixinContainer.h"
#include "NMixinHashable.h"
#include "NRange.h"
#include "NStringEncodings.h"
#include "NanoMacros.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NData;


// String storage
//
// Small strings are stored directly within the object.
//
// Larger strings are a view onto shared immutable state.
//
// Storage is 16-byte aligned to allow init/copy operations to be vectorised.
struct alignas(16) NStringStorage
{
	union
	{
		struct
		{
			uint8_t sizeFlags;
			uint8_t theData[23];
		} Small;

		struct
		{
			struct NStringState* theState;
			uint8_t              reserved[16];
		} Large;
	};

	mutable size_t theHash;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NString final
	: public NMixinContainer<NString>
	, public NMixinComparable<NString>
	, public NMixinHashable<NString>
{
public:
	constexpr                           NString(const utf8_t*  theString);
	constexpr                           NString(const utf16_t* theString);
										NString(const utf32_t* theString);

	inline                              NString();
	inline                             ~NString();

										NString(  const NString& otherString);
	NString&                            operator=(const NString& otherString);

										NString(  NString&& otherString);
	NString&                            operator=(NString&& otherString);


	// Clear the string
	void                                Clear();


	// Get the size
	//
	// Returns the number of code points (utf32_t characters) in the string.
	size_t                              GetSize() const;


	// Get the text
	//
	// Returns a NULL-terminated string that remains valid until the string is modified.
	const void*                         GetText(NStringEncoding theEncoding) const;

	constexpr const utf8_t*             GetUTF8()  const;
	constexpr const utf16_t*            GetUTF16() const;
	const utf32_t*                      GetUTF32() const;


public:
	// Fetch the hash
	size_t&                             FetchHash(bool updateHash) const;


private:
	constexpr bool                      IsSmall()      const;
	constexpr bool                      IsSmallUTF8()  const;
	constexpr bool                      IsSmallUTF16() const;
	constexpr bool                      IsLarge()      const;

	constexpr bool                      IsFixedWidthUTF8( size_t numUTF8,  const utf8_t* textUTF8)   const;
	constexpr bool                      IsFixedWidthUTF16(size_t numUTF16, const utf16_t* textUTF16) const;

	constexpr bool                      SetSmallUTF8( size_t numUTF8,  const utf8_t* textUTF8);
	constexpr bool                      SetSmallUTF16(size_t numUTF16, const utf16_t* textUTF16);
	constexpr void                      SetSmall(     size_t numBytes, const void* theText, NStringEncoding theEncoding);

	void                                MakeClone(const NString& otherString);
	void                                MakeLarge();

	const NData*                        GetEncoding(  NStringEncoding theEncoding);
	const NData*                        FetchEncoding(NStringEncoding theEncoding);
	void                                StoreEncoding(NStringEncoding theEncoding);
	void                                ReleaseEncodings();

	NStringState*                       GetLarge();
	void                                SetLarge(NStringState* theState);
	void                                RetainLarge();
	void                                ReleaseLarge();

	size_t                              GetSizeSmall() const;
	size_t                              GetSizeLarge() const;

	void                                SetText(size_t numBytes, const void* theText, NStringEncoding theEncoding);

	void                                SetTextSmall(size_t numBytes, const void* theText, NStringEncoding theEncoding);
	void                                SetTextLarge(size_t numBytes, const void* theText, NStringEncoding theEncoding);



private:
	NStringStorage                      mString;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NString.inl"


#endif // NSTRING_H
