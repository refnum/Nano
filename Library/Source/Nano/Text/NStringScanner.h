/*	NAME:
		NStringScanner.h

	DESCRIPTION:
		String scanner.

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
#ifndef NSTRING_SCANNER_H
#define NSTRING_SCANNER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NRange.h"
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
struct pcre2_real_code_8;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NStringScanner
{
public:
	// Find the first instance of a string
	//
	// NStringFind::Pattern searches return the range of the overall pattern,
	// ignoring any capture groups within that pattern.
	//
	// Supports NStringFind::NoCase, NStringFind::Pattern, and NStringFind::MultiLine.
	static NRange                       Find(const NString&   theString,
											 const NString&   searchFor,
											 NStringFindFlags theFlags,
											 const NRange&    searchRange);


	// Find every instance of a string
	//
	// NStringFind::Pattern searches return the ranges of the overall pattern,
	// ignoring any capture groups within that pattern.
	//
	// Supports NStringFind::NoCase, NStringFind::Pattern, and NStringFind::MultiLine.
	static NVectorRange                 FindAll(const NString&   theString,
												const NString&   searchFor,
												NStringFindFlags theFlags,
												const NRange&    theRange);


	// Find the first instance of a pattern
	//
	// Returns the match for the pattern and any capture groups
	// within that pattern.
	//
	// Supports NStringFind::NoCase and NStringFind::MultiLine.
	static NPatternMatch                FindMatch(const NString&   theString,
												  const NString&   searchFor,
												  NStringFindFlags theFlags,
												  const NRange&    theRange);


	// Find every instance of a pattern
	//
	// Returns the matches for the pattern and any capture groups
	// within that pattern.
	//
	// Supports NStringFind::NoCase and NStringFind::MultiLine.
	static NVectorPatternMatch          FindMatches(const NString&   theString,
													const NString&   searchFor,
													NStringFindFlags theFlags,
													const NRange&    theRange);


	// Replace a substring
	//
	// ReplaceAll returns the number of instances replaced within the string.
	//
	// Supports NStringFind::NoCase, NStringFind::Pattern, and NStringFind::MultiLine.
	static bool                         Replace(NString&         theString,
												const NString&   searchFor,
												const NString&   replaceWith,
												NStringFindFlags theFlags = kNStringFindExact,
												const NRange&    theRange = kNRangeAll);

	static size_t                       ReplaceAll(NString&         theString,
												   const NString&   searchFor,
												   const NString&   replaceWith,
												   NStringFindFlags theFlags = kNStringFindExact,
												   const NRange&    theRange = kNRangeAll);


	// Replace a range
	//
	// Each replacement range is intersected with the applicable range,
	// and the resulting intersection is replaced with the string.
	static void                         Replace(NString&       theString,
												const NRange&  theRange,
												const NString& replaceWith,
												const NRange&  overRange = kNRangeAll);

	static void                         ReplaceAll(NString&            theString,
												   const NVectorRange& theRanges,
												   const NString&      replaceWith,
												   const NRange&       theRange = kNRangeAll);


	// Split a string
	//
	// Split the string into a list of elements, divided by the separator.
	//
	// A separator that appears at the start/end of the string, or adjacent
	// separators within the string, will produce an empty string.
	//
	// NTextUtils::RemoveEmpty can be used to discard empty elements.
	//
	//
	// The default behaviour is to split on whitespace.
	//
	// As most separators are not patterns theFlags is promoted to
	// a pattern search if splitWithin remains the default value.
	//
	// Supports NStringFind::NoCase, NStringFind::Pattern, and NStringFind::MultiLine.
	static NVectorString                Split(const NString&   theString,
											  const NString&   splitWith = kNStringWhitespace,
											  NStringFindFlags theFlags  = kNStringFindExact,
											  const NRange&    theRange  = kNRangeAll);


private:
	static NPatternMatch                FindFirst(const NString&   theString,
												  const NString&   searchFor,
												  NStringFindFlags theFlags,
												  const NRange&    theRange);

	static NVectorPatternMatch          FindAll(const NString&   theString,
												const NString&   searchFor,
												NStringFindFlags theFlags,
												const NRange&    theRange,
												size_t           maxResult);


	static NVectorPatternMatch          FindPattern(const NString&   theString,
													const NString&   searchFor,
													NStringFindFlags theFlags,
													const NRange&    theRange,
													size_t           maxResult);

	static NVectorRange                 GetReplacementRanges(const NVectorRange& theRanges, const NRange& theRange);

	static pcre2_real_code_8*           GetRegexp(const NString& searchFor, NStringFindFlags theFlags);

	static NVectorPatternMatch          BytesToCodepoints(const NString&             theString,
														  const NData&               dataUTF8,
														  size_t                     rangeLocation,
														  const NVectorPatternMatch& bytePatternMatches);

	static NRange                       BytesToCodepoints(const NVectorSize& codePointOffsets,
														  size_t             codePointOffset,
														  const NRange&      byteRange);

	static bool                         IsFixedWidthUTF8(const NData& dataUTF8);
};



#endif // NSTRING_SCANNER_H
