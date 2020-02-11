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
#include "NMixinConcatenable.h"
#include "NMixinContainer.h"
#include "NMixinHashable.h"
#include "NRange.h"
#include "NStringEncodings.h"
#include "NanoMacros.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Sizes
static constexpr size_t kNStringLength                      = size_t(-1);


// String flags
using NStringFlags                                          = uint8_t;

static constexpr NStringFlags kNStringNone                  = NStringFlags(0);
static constexpr NStringFlags kNStringNoCase                = NStringFlags(1 << 0);
static constexpr NStringFlags kNStringNumeric               = NStringFlags(1 << 1);
static constexpr NStringFlags kNStringPattern               = NStringFlags(1 << 2);

static constexpr NStringFlags kNStringPatternNoCase         = kNStringPattern | kNStringNoCase;


// Transform flags
using NStringTransform                                      = uint8_t;

static constexpr NStringTransform kNStringTransformNone                 = NStringTransform(0);
static constexpr NStringTransform kNStringTransformToLower              = NStringTransform(1 << 0);
static constexpr NStringTransform kNStringTransformToUpper              = NStringTransform(1 << 1);
static constexpr NStringTransform kNStringTransformCapitalizeWords      = NStringTransform(1 << 2);
static constexpr NStringTransform kNStringTransformCapitalizeSentences  = NStringTransform(1 << 3);





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NData;


// Pattern group
//
// A pattern group identifies both the range that matched the pattern,
// and the ranges of individual capture groups within that pattern.
struct NPatternGroup
{
	NRange       thePattern;
	NVectorRange theGroups;
};

using NVectorPatternGroup = std::vector<NPatternGroup>;


// String storage
//
// Small strings are stored directly within the object.
//
// Larger strings are a view onto shared immutable state.
//
//
// The storage structure is 16-byte aligned, and a multiple of 8 bytes
// in size, to allow initialisation / copy operations to be vectorised.
//
// The small data field must be the first field to ensure it has the
// correct alignment to be recast to any type that is <= 16 bytes.
//
NN_STRUCT_PACK_1(alignas(16) NStringStorage {
	union
	{
		struct
		{
			uint8_t theData[27];
		} Small;

		struct
		{
			struct NStringState* theState;
		} Large;
	};

	uint8_t  theFlags;
	uint32_t theHash;
});





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NString final
	: public NMixinHashable<NString>
	, public NMixinComparable<NString>
	, public NMixinConcatenable<NString>
	, public NMixinContainer<NString>
{
public:
	constexpr                           NString(const utf8_t*  theString);
	constexpr                           NString(const utf16_t* theString);
										NString(const utf32_t* theString);

										NString(NStringEncoding theEncoding, size_t numBytes, const void* theData);
										NString(NStringEncoding theEncoding, const NData& theData);

										NString();
									   ~NString();

										NString(  const NString& otherString);
	NString&                            operator=(const NString& otherString);

										NString(  NString&& otherString);
	NString&                            operator=(NString&& otherString);


	// Clear the string
	void                                Clear();


	// Get the size
	//
	// Returns the number of code points (utf32_t elements) in the string,
	// not counting the null terminator.
	size_t                              GetSize() const;


	// Get the text
	//
	// Returns a null-terminated string that is valid until the string is modified.
	//
	// A UTF8 / UTF16 result may be invalidated by requesting any other encoding.
	const void*                         GetText(NStringEncoding theEncoding) const;

	constexpr const utf8_t*             GetUTF8()  const;
	constexpr const utf16_t*            GetUTF16() const;
	const utf32_t*                      GetUTF32() const;


	// Get the current encodings
	//
	// Returns the list of encodings that can be returned without transcoding.
	NVectorStringEncoding               GetEncodings() const;


	// Get/set the string data
	//
	// As strings may contain embedded nulls the string data does not need to
	// have a null terminator nor will a terminator be added on retrieval.
	//
	// A string will a trailing null element will preserves that null as part of
	// its content, just as it would a null at any other location.
	NData                               GetData(NStringEncoding theEncoding) const;
	void                                SetData(NStringEncoding theEncoding, const NData& theData);


	// Get the string content
	//
	// Returns a pointer to the primary string encoding, its size in bytes,
	// and the data in that encoding.
	//
	// The results are valid until the string is modified.
	const void*                         GetContent(NStringEncoding* theEncoding, size_t* theSize) const;


	// Find a string
	//
	// Find the first, or all, instances of a substring.
	//
	// kNStringPattern return the ranges of the overall match, ignoring
	// any capture groups for pattern searches.
	//
	// Supports kNStringNoCase, kNStringPattern, and kNStringMultiLine.
	NRange                              Find(const NString& theString,
											 NStringFlags   theFlags = kNStringNone,
											 const NRange&  theRange = kNRangeAll) const;

	NVectorRange                        FindAll(const NString& theString,
												NStringFlags   theFlags = kNStringNone,
												const NRange&  theRange = kNRangeAll) const;


	// Find a capturing pattern
	//
	// Find the first, or all, instances of a substring.
	//
	// kNStringPattern return the ranges of the overall pattern, including
	// ny capture groups within that pattern.
	//
	// Supports kNStringNoCase, kNStringPattern, and kNStringMultiLine.
	NPatternGroup                       FindGroup(const NString& theString,
												  NStringFlags   theFlags = kNStringNone,
												  const NRange&  theRange = kNRangeAll) const;

	NVectorPatternGroup                 FindAllGroups(const NString& theString,
													  NStringFlags   theFlags = kNStringNone,
													  const NRange&  theRange = kNRangeAll) const;


	// Transform the string
	NString                             GetTransformed(NStringTransform theTransform, const NRange& theRange = kNRangeAll);
	NString                             GetLower() const;
	NString                             GetUpper() const;


	// Test the string
	//
	// Supports kNStringNoCasea and kNStringPattern.
	bool                                StartsWith(const NString& theString, NStringFlags theFlags = kNStringNone) const;
	bool                                EndsWith(  const NString& theString, NStringFlags theFlags = kNStringNone) const;
	bool                                Contains(  const NString& theString, NStringFlags theFlags = kNStringNone) const;


	// Compare the string
	//
	// The equality (==, !=) and order (<, <=, >, >= operarators are case-sensitive.
	//
	// Supports kNStringNoCasea and kNStringNumeric.
	NComparison                         Compare(const NString& theString, NStringFlags theFlags = kNStringNone)   const;
	bool                                EqualTo(const NString& theString, NStringFlags theFlags = kNStringNoCase) const;


	// Compare an object
	bool                                CompareEqual(const NString& theString) const;
	NComparison                         CompareOrder(const NString& theString) const;


	// Operators
	const NString&                      operator+=(const NString& theString);


public:
	// Update the hash
	size_t                              UpdateHash(NHashAction theAction);


private:
	constexpr bool                      IsSmall()      const;
	constexpr bool                      IsSmallUTF8()  const;
	constexpr bool                      IsSmallUTF16() const;
	constexpr bool                      IsLarge()      const;

	constexpr bool                      IsValidEncoding(NStringEncoding theEncoding)  const;
	constexpr bool                      IsValidSmallUTF8( size_t numBytes,            const utf8_t* textUTF8)   const;
	constexpr bool                      IsValidSmallUTF16(size_t numBytes,            const utf16_t* textUTF16) const;

	constexpr size_t                    GetLength(  NStringEncoding theEncoding, const void* theString);
	constexpr size_t                    GetCodeUnit(NStringEncoding theEncoding, const void* theString);

	constexpr bool                      SetSmallUTF8( size_t numBytes, const utf8_t* textUTF8);
	constexpr bool                      SetSmallUTF16(size_t numBytes, const utf16_t* textUTF16);
	constexpr void                      SetSmall(     size_t numBytes, const void* theText, NStringEncoding theEncoding);

	void                                MakeClone(const NString& theString);
	void                                MakeLarge();

	const NData*                        FetchEncoding(NStringEncoding theEncoding);
	const NData*                        GetEncoding(  NStringEncoding theEncoding) const;
	void                                AddEncoding(  NStringEncoding theEncoding);
	void                                ReleaseEncodings();

	void                                SetLarge(NStringState* theState);
	void                                RetainLarge();
	void                                ReleaseLarge();

	size_t                              GetSizeSmall() const;
	size_t                              GetSizeLarge() const;

	void                                SetText(     NStringEncoding theEncoding, size_t numBytes, const void* theText);
	void                                SetTextSmall(NStringEncoding theEncoding, size_t numBytes, const void* theText);
	void                                SetTextLarge(NStringEncoding theEncoding, size_t numBytes, const void* theText);


private:
	NStringStorage                      mString;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NString.inl"


#endif // NSTRING_H
