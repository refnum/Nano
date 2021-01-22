/*	NAME:
		NStringScanner.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NStringScanner.h"

// Nano
#include "NData.h"
#include "NStdAlgorithm.h"
#include "NUnicodeView.h"
#include "Nano_pcre2.h"





//=============================================================================
//		NStringScanner::Find : Find the first instance of a string.
//-----------------------------------------------------------------------------
NRange NStringScanner::Find(const NString& theString,
							const NString& searchFor,
							NStringFlags   theFlags,
							const NRange&  theRange)
{


	// Check our parameters
	NN_REQUIRE((theFlags & kNStringNumeric) == 0);



	// Find the first instance
	return FindFirst(theString, searchFor, theFlags, theRange).thePattern;
}





//=============================================================================
//		NStringScanner::FindAll : Find every instance of a string.
//-----------------------------------------------------------------------------
NVectorRange NStringScanner::FindAll(const NString& theString,
									 const NString& searchFor,
									 NStringFlags   theFlags,
									 const NRange&  theRange)
{


	// Check our parameters
	NN_REQUIRE((theFlags & kNStringNumeric) == 0);



	// Find every instance
	NVectorRange theResult;

	for (const auto& theMatch : FindAll(theString, searchFor, theFlags, theRange, kNSizeMax))
	{
		theResult.emplace_back(theMatch.thePattern);
	}

	return theResult;
}





//=============================================================================
//		NStringScanner::FindMatch : Find the first instance of pattern.
//-----------------------------------------------------------------------------
NPatternMatch NStringScanner::FindMatch(const NString& theString,
										const NString& searchFor,
										NStringFlags   theFlags,
										const NRange&  theRange)
{


	// Check our parameters
	NN_REQUIRE((theFlags & kNStringNumeric) == 0);
	NN_REQUIRE((theFlags & kNStringPattern) == 0);



	// Find the first instance
	return FindFirst(theString, searchFor, theFlags | kNStringPattern, theRange);
}





//=============================================================================
//		NStringScanner::FindMatches : Find every instance of a pattern.
//-----------------------------------------------------------------------------
NVectorPatternMatch NStringScanner::FindMatches(const NString& theString,
												  const NString& searchFor,
												  NStringFlags   theFlags,
												  const NRange&  theRange)
{


	// Check our parameters
	NN_REQUIRE((theFlags & kNStringNumeric) == 0);
	NN_REQUIRE((theFlags & kNStringPattern) == 0);



	// Find every instance
	return FindAll(theString, searchFor, theFlags | kNStringPattern, theRange, kNSizeMax);
}





//=============================================================================
//		NStringScanner::Replace : Replace a substring.
//-----------------------------------------------------------------------------
bool NStringScanner::Replace(NString&       theString,
							 const NString& searchFor,
							 const NString& replaceWith,
							 NStringFlags   theFlags,
							 const NRange&  theRange)
{


	// Find and replace
	NRange foundRange = Find(theString, searchFor, theFlags, theRange);
	bool   foundIt    = !foundRange.IsEmpty();

	if (foundIt)
	{
		ReplaceAll(theString, {foundRange}, replaceWith);
	}

	return foundIt;
}





//=============================================================================
//		NStringScanner::ReplaceAll : Replace substrings.
//-----------------------------------------------------------------------------
size_t NStringScanner::ReplaceAll(NString&       theString,
								  const NString& searchFor,
								  const NString& replaceWith,
								  NStringFlags   theFlags,
								  const NRange&  theRange)
{


	// Find and replace
	NVectorRange foundRanges = FindAll(theString, searchFor, theFlags, theRange);
	size_t       numFound    = foundRanges.size();

	if (numFound != 0)
	{
		ReplaceAll(theString, foundRanges, replaceWith);
	}

	return numFound;
}





//=============================================================================
//		NStringScanner::Replace : Replace a range within a string.
//-----------------------------------------------------------------------------
void NStringScanner::Replace(NString&       theString,
							 const NRange&  theRange,
							 const NString& replaceWith,
							 const NRange&  overRange)
{


	// Replace the range
	ReplaceAll(theString, {theRange}, replaceWith, overRange);
}





//=============================================================================
//		NStringScanner::ReplaceAll : Replace ranges within a string.
//-----------------------------------------------------------------------------
void NStringScanner::ReplaceAll(NString&            theString,
								const NVectorRange& theRanges,
								const NString&      replaceWith,
								const NRange&       theRange)
{


	// Get the state we need
	bool         replaceEmpty      = replaceWith.IsEmpty();
	NRange       finalRange        = theRange.GetNormalized(theString.GetSize());
	NVectorRange replacementRanges = GetReplacementRanges(theRanges, finalRange);


	// Replace the ranges
	if (!theString.IsEmpty() && !replacementRanges.empty() && !finalRange.IsEmpty())
	{
		NRange  previousRange;
		NString newString;


		// Replace the ranges
		for (const auto& replaceRange : replacementRanges)
		{
			previousRange.SetSize(replaceRange.GetLocation() - previousRange.GetLocation());

			newString += theString.GetSubstring(previousRange);
			if (!replaceEmpty)
			{
				newString += replaceWith;
			}

			previousRange.SetRange(replaceRange.GetNext(), 0);
		}


		// Append any suffix
		if (previousRange.GetLocation() != 0)
		{
			previousRange.SetSize(theString.GetSize() - previousRange.GetLocation());
			newString += theString.GetSubstring(previousRange);
		}

		theString = newString;
	}
}





//=============================================================================
//		NStringScanner::Split : Split a string.
//-----------------------------------------------------------------------------
NVectorString NStringScanner::Split(const NString& theString,
									const NString& splitWith,
									NStringFlags   theFlags,
									const NRange&  theRange)
{


	// Adjust the flags
	//
	// Most separators are not patterns so we leave the default
	// value of theFlags as kNStringNone.
	//
	// However, to provide our default behaviour of splitting on
	// whitespace, we must enable pattern matching if splitting
	// on our special whitespace pattern.
	if (splitWith == kNStringWhitespace && theFlags == kNStringNone)
	{
		theFlags |= kNStringPattern;
	}



	// Get the state we need
	NVectorRange  foundRanges = FindAll(theString, splitWith, theFlags, theRange);
	NVectorString theResult;



	// No split
	if (foundRanges.empty())
	{
		theResult.emplace_back(theString);
	}


	// Split by range
	else
	{
		NRange  previousRange;
		NString newString;

		theResult.reserve(foundRanges.size());

		for (const auto& foundRange : foundRanges)
		{
			previousRange.SetSize(foundRange.GetLocation() - previousRange.GetLocation());
			theResult.emplace_back(theString.GetSubstring(previousRange));

			previousRange.SetRange(foundRange.GetNext(), 0);
		}

		if (previousRange.GetLocation() != 0)
		{
			previousRange.SetSize(theString.GetSize() - previousRange.GetLocation());
			theResult.emplace_back(theString.GetSubstring(previousRange));
		}
	}

	return theResult;
}





#pragma mark private
//=============================================================================
//		NStringScanner::FindFirst : Find the first match.
//-----------------------------------------------------------------------------
NPatternMatch NStringScanner::FindFirst(const NString& theString,
										 const NString& searchFor,
										 NStringFlags   theFlags,
										 const NRange&  theRange)
{


	// Find the match
	NVectorPatternMatch theMatches = FindAll(theString, searchFor, theFlags, theRange, 1);
	NPatternMatch       theResult;

	if (!theMatches.empty())
	{
		theResult = theMatches.front();
	}

	return theResult;
}


//=============================================================================
//		NStringScanner::FindAll : Find all the matches.
//-----------------------------------------------------------------------------
NVectorPatternMatch NStringScanner::FindAll(const NString& theString,
										 const NString& searchFor,
										 NStringFlags   theFlags,
										 const NRange&  theRange,
										 size_t         maxResuilt)
{


	// Check our parameters
	NN_REQUIRE((theFlags & kNStringNumeric) == 0);



	// Get the state we need
	NRange              finalRange = theRange.GetNormalized(theString.GetSize());
	NVectorPatternMatch theResult;



	// Find the pattern
	if (!theString.IsEmpty() && !searchFor.IsEmpty() && !finalRange.IsEmpty())
	{
		theResult = FindPattern(theString, searchFor, theFlags, finalRange, maxResuilt);
	}

	return theResult;
}





//=============================================================================
//		NStringScanner::FindPattern : Find a pattern.
//-----------------------------------------------------------------------------
NVectorPatternMatch NStringScanner::FindPattern(const NString& theString,
												const NString& searchFor,
												NStringFlags   theFlags,
												const NRange&  theRange,
												size_t         maxResult)
{


	// Validate our parameters
	NN_REQUIRE(!theString.IsEmpty());
	NN_REQUIRE(!searchFor.IsEmpty());
	NN_REQUIRE(!theRange.IsEmpty());
	NN_REQUIRE(maxResult >= 1);



	// Get the state we need
	NString searchString = theString.GetSubstring(theRange);
	NData   searchUTF8   = searchString.GetData(NStringEncoding::UTF8);

	const utf8_t* searchText   = reinterpret_cast<const utf8_t*>(searchUTF8.GetData());
	size_t        searchSize   = searchUTF8.GetSize();
	size_t        searchOffset = 0;

	NVectorPatternMatch patternMatches;
	NPatternMatch       patternMatch;



	// Create the regexp
	pcre2_code*       regExp     = GetRegexp(searchFor, theFlags);
	pcre2_match_data* regMatches = pcre2_match_data_create_from_pattern(regExp, nullptr);


	// Find the patterns
	//
	// PCRE works in byte offsets, so we maintain our search inputs / outputs
	// in byte offsets until we have finishd processing.
	for (size_t n = 0; n < maxResult; n++)
	{
		// Perform the match
		int regErr = pcre2_match(regExp,
								 PCRE2_SPTR8(searchText),
								 searchSize,
								 searchOffset,
								 PCRE2_NO_UTF_CHECK,
								 regMatches,
								 nullptr);

		if (regErr < 0)
		{
			break;
		}


		// Save the pattern range, and any capture groups
		size_t* regOutput = pcre2_get_ovector_pointer(regMatches);

		for (int m = 0; m < regErr; m++)
		{
			size_t matchStart = regOutput[(2 * m) + 0];
			size_t matchEnd   = regOutput[(2 * m) + 1];

			NRange matchRange(matchStart, matchEnd - matchStart);

			if (m == 0)
			{
				patternMatch.thePattern = matchRange;
			}
			else
			{
				patternMatch.theGroups.emplace_back(matchRange);
			}
		}

		patternMatches.emplace_back(patternMatch);
		patternMatch.theGroups.clear();


		// Advance the search
		searchOffset = patternMatch.thePattern.GetNext();
	}



	// Clean up
	pcre2_match_data_free(regMatches);
	pcre2_code_free(regExp);

	return BytesToCodepoints(searchString, searchUTF8, theRange.GetLocation(), patternMatches);
}





//=============================================================================
//		NStringScanner::GetReplacementRanges : Get the replacement ranges.
//-----------------------------------------------------------------------------
NVectorRange NStringScanner::GetReplacementRanges(const NVectorRange& theRanges,
												  const NRange&       theRange)
{


	// Validate our parameters
	//
	// Replacement ranges may not overlap.
	NN_REQUIRE(!theRanges.empty());
	NN_REQUIRE(!theRange.IsEmpty());

#if NN_ENABLE_ASSERTIONS
	for (const auto& outerRange : theRanges)
	{
		for (const auto& innerRange : theRanges)
		{
			NN_REQUIRE(outerRange == innerRange ||
					   outerRange.GetIntersection(innerRange).IsEmpty());
		}
	}
#endif // NN_ENABLE_ASSERTIONS



	// Get the replacement ranges
	//
	// To perform replacements we intersect the proposed ranges with
	// the actual ranges, then sort them to ensure we can replace by
	// appending the unaffected ranges with the replacement text.
	NVectorRange theResult;

	theResult.reserve(theRanges.size());

	for (const auto& proposedRange : theRanges)
	{
		NRange actualRange = proposedRange.GetIntersection(theRange);
		if (!actualRange.IsEmpty())
		{
			theResult.emplace_back(actualRange);
		}
	}

	nstd::sort(theResult);

	return theResult;
}





//=============================================================================
//		NStringScanner::GetRegexp : Get the compiled expression.
//-----------------------------------------------------------------------------
pcre2_real_code_8* NStringScanner::GetRegexp(const NString& searchFor, NStringFlags theFlags)
{


	// Build the flags
	uint32_t regFlags = PCRE2_UTF | PCRE2_NO_UTF_CHECK;

	if (theFlags & kNStringNoCase)
	{
		regFlags |= PCRE2_CASELESS;
	}

	if (theFlags & kNStringPattern)
	{
		regFlags |= PCRE2_DOTALL;
	}
	else
	{
		regFlags |= PCRE2_LITERAL;
	}

	if (theFlags & kNStringMultiLine)
	{
		regFlags |= PCRE2_MULTILINE;
	}



	// Compile the expression
	int    errorCode   = 0;
	size_t errorOffset = 0;

	pcre2_code* regExp = pcre2_compile(PCRE2_SPTR8(searchFor.GetUTF8()),
									   ~static_cast<PCRE2_SIZE>(0),    // PCRE2_ZERO_TERMINATED
									   regFlags,
									   &errorCode,
									   &errorOffset,
									   nullptr);



	// Handle failure
	if (regExp == nullptr)
	{
		uint8_t errorMsg[256];
		pcre2_get_error_message(errorCode, errorMsg, sizeof(errorMsg));

		NN_LOG_ERROR("PCRE2 failed to compile '%s' at offset %zu: %s",
					 searchFor.GetUTF8(),
					 errorOffset,
					 errorMsg);
	}

	return regExp;
}





//=============================================================================
//		NStringScanner::BytesToCodepoints : Convert byte offsets to codepoint offsets.
//-----------------------------------------------------------------------------
NVectorPatternMatch NStringScanner::BytesToCodepoints(const NString&             theString,
													  const NData&               dataUTF8,
													  size_t                     rangeLocation,
													  const NVectorPatternMatch& bytePatternMatches)
{


	// Convert to codepoints offsets
	//
	// PCRE returns pattern groups as byte offsets relative to the start of
	// the search text.
	//
	// As PCRE always takes UTF8 text as input we can check to see if the
	// search text is in fact fixed width UTF8.
	//
	// If it is then any byte offset results are already codepoint offsets.
	NVectorPatternMatch codePointMatches(bytePatternMatches);

	if (!IsFixedWidthUTF8(dataUTF8))
	{
		// Get the state we need
		//
		// We track the offset used to perform the initial byte->codepoint
		// lookup for each group, allowing us to skip forward after the
		// group to minimise lookups.
		NVectorSize codePointOffsets(NUnicodeView(theString).GetCodePointOffsets());
		size_t      patternOffset = 0;
		size_t      nextPattern   = 0;

		for (auto& patternMatch : codePointMatches)
		{
			// Convert the pattern range
			//
			// The returned offset is that of the last codepoint in the pattern
			// itself, which gives us our starting point for the next pattern.
			patternOffset = nextPattern;
			patternMatch.thePattern =
				BytesToCodepoints(codePointOffsets, patternOffset, patternMatch.thePattern);
			nextPattern = patternMatch.thePattern.GetNext();



			// Convert the captured ranges
			//
			// As a captured groups may start from the start of the pattern
			// itself we use the same start offset.
			//
			// However as captured groups follow sequentially from each other
			// we can also use the end of each group as the starting point for
			// the next group.
			for (auto& theRange : patternMatch.theGroups)
			{
				theRange      = BytesToCodepoints(codePointOffsets, patternOffset, theRange);
				patternOffset = theRange.GetNext();
			}
		}
	}



	// Apply the search offset
	//
	// If we searched a substring of the original string then our
	// results need to be adjusted by the offset of that substring.
	//
	// The common case is that we're searching the entire string and
	// so we do this as a separate pass as required.
	if (rangeLocation != 0)
	{
		for (auto& patternMatch : codePointMatches)
		{
			patternMatch.thePattern.AddOffset(rangeLocation);

			for (auto& theRange : patternMatch.theGroups)
			{
				theRange.AddOffset(rangeLocation);
			}
		}
	}

	return codePointMatches;
}







//=============================================================================
//		NStringScanner::BytesToCodepoints : Convert byte offsets to codepoint offsets.
//-----------------------------------------------------------------------------
NRange NStringScanner::BytesToCodepoints(const NVectorSize& codePointOffsets,
										 size_t             codePointOffset,
										 const NRange&      byteRange)
{


	// Validate our parameters
	NN_REQUIRE(codePointOffset < codePointOffsets.size());
	NN_REQUIRE(!byteRange.IsEmpty());



	// Get the state we need
	size_t codePointLocation = kNNotFound;
	size_t codePointSize     = kNNotFound;
	size_t n                 = codePointOffset;


	// Find the codepoint offsets
	for (; n < codePointOffsets.size(); n++)
	{
		size_t byteOffset = codePointOffsets[n];

		if (codePointLocation == kNNotFound && byteOffset == byteRange.GetLocation())
		{
			codePointLocation = n;
		}

		else if (codePointSize == kNNotFound && byteOffset == byteRange.GetNext())
		{
			codePointSize = n - codePointLocation;
			break;
		}
	}



	// Update the range
	//
	// If the the byte range ends on the final codepoint then our byte range
	// will end exactly after the final codepoint, so we calculate the size
	// relative to the location.
	NN_REQUIRE(codePointLocation != kNNotFound);

	if (byteRange.GetNext() == codePointOffsets.size())
	{
		NN_REQUIRE(codePointSize == kNNotFound);
		codePointSize = n - codePointLocation;
	}

	return {codePointLocation, codePointSize};
}



//=============================================================================
//		NStringScanner::IsFixedWidthUTF8 : Is UTF8 data fixed-width?
//-----------------------------------------------------------------------------
bool NStringScanner::IsFixedWidthUTF8(const NData& dataUTF8)
{


	// Check the bytes
	size_t         theSize = dataUTF8.GetSize();
	const uint8_t* theData = dataUTF8.GetData();

	for (size_t n = 0; n < theSize; n++)
	{
		if ((theData[n] & kNUTF8VariableWidth) != 0)
		{
			return false;
		}
	}

	return true;
}
