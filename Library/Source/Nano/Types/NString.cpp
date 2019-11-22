/*	NAME:
		NString.cpp

	DESCRIPTION:
		String object.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
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





//=============================================================================
//		NString::NString : Constructor.
//-----------------------------------------------------------------------------
NString::NString()
{
}



// Nano 3.x implementation
/*
 #include "NEncoder.h"
 #include "NSTLUtilities.h"
 #include "NString.h"
 #include "NTargetSystem.h"
 #include "NTextUtilities.h"
 #include "pcre.h"





   //============================================================================
   //		Public constants
   //----------------------------------------------------------------------------
   const NIndex  kNStringLength                             = kNIndexNone;
   const NString kNStringWhitespace                         = "\\s";





   //============================================================================
   //		Internal constants
   //----------------------------------------------------------------------------
   const float kAppendGrowth                                = 1.5f;





   //============================================================================
   //		Implementation
   //----------------------------------------------------------------------------
   NENCODABLE_DEFINE(NString);





   //============================================================================
   //		NString::NString : Constructor.
   //----------------------------------------------------------------------------
   NString::NString(const utf8_t *noCopyText)
   {


	// Initialize ourselves
 * this                                                     = GetConstantString(noCopyText);
   }





   //============================================================================
   //		NString::NString : Constructor.
   //----------------------------------------------------------------------------
   NString::NString(const void *copyText, NIndex numBytes, NStringEncoding theEncoding)
   {	NStringEncoder		theEncoder;



	// Get the state we need
	if (copyText == NULL)
		numBytes                                            = 0;

	if (numBytes == kNStringLength)
		numBytes                                            = theEncoder.GetSize(copyText, theEncoding);



	// Initialize ourselves
	ValueChanged(NULL);

	if (numBytes != 0)
		SetData(NData(numBytes, copyText), theEncoding);
   }





   //============================================================================
   //		NString::NString : Constructor.
   //----------------------------------------------------------------------------
   NString::NString(const NData &theData, NStringEncoding theEncoding)
   {


	// Initialize ourselves
	SetData(theData, theEncoding);
   }





   //============================================================================
   //		NString::NString : Constructor.
   //----------------------------------------------------------------------------
   NString::NString(utf8_t theChar)
   {


	// Initialize ourselves
	SetData(NData(1, &theChar), kNStringEncodingUTF8);
   }





   //============================================================================
   //		NString::NString : Constructor.
   //----------------------------------------------------------------------------
   NString::NString(void)
   {


	// Initialise ourselves
	ValueChanged(NULL);
   }





   //============================================================================
   //		NString::~NString : Destructor.
   //----------------------------------------------------------------------------
   NString::~NString(void)
   {
   }





   //============================================================================
   //		NString::Clear : Clear the value.
   //----------------------------------------------------------------------------
   void NString::Clear(void)
   {


	// Clear the value
	NSharedValueString::Clear();

	ValueChanged(NULL);
   }





   //============================================================================
   //		NString::GetSize : Get the size.
   //----------------------------------------------------------------------------
   NIndex NString::GetSize(void) const
   {	const NStringValue		*theValue;



	// Get the size
	theValue                                                = GetImmutable();

	return(theValue->theSize);
   }





   //============================================================================
   //		NString::GetText : Get the text.
   //----------------------------------------------------------------------------
   const char *NString::GetText(NStringEncoding theEncoding) const
   {	const NStringValue		*theValue;
	const NData				*theData;



	// Get the state we need
	theValue                                                = GetImmutable();
	theData                                                 = &theValue->theData;



	// Re-encode if necessary
	//
	// Strings that require encoding conversion are converted via a per-object
	// buffer, allowing the pointer to persist until we are modified.
	if (theValue->theEncoding != theEncoding)
		{
		mData                                               = GetData(theEncoding, kNStringNullTerminate);
		theData                                             = &mData;
		}



	// Get the text
	return((const char *) theData->GetData());
   }





   //============================================================================
   //		NString::GetUTF8 : Get the text as UTF8.
   //----------------------------------------------------------------------------
   const utf8_t *NString::GetUTF8(void) const
   {


	// Get the string
	return((const utf8_t *) GetText(kNStringEncodingUTF8));
   }





   //============================================================================
   //		NString::GetUTF16 : Get the text as UTF16.
   //----------------------------------------------------------------------------
   const utf16_t *NString::GetUTF16(void) const
   {


	// Get the string
	return((const utf16_t *) GetText(kNStringEncodingUTF16));
   }





   //============================================================================
   //		NString::GetUTF32 : Get the text as UTF32.
   //----------------------------------------------------------------------------
   const utf32_t *NString::GetUTF32(void) const
   {


	// Get the string
	return((const utf32_t *) GetText(kNStringEncodingUTF32));
   }





   //============================================================================
   //		NString::GetData : Get the string.
   //----------------------------------------------------------------------------
   NData NString::GetData(NStringEncoding theEncoding, NStringRendering renderAs) const
   {	NStringEncoder			theEncoder;
	NUnicodeParser			theParser;
	const NStringValue		*theValue;
	NData					theData;
	NStatus					theErr;



	// Get the state we need
	theValue                                                = GetImmutable();
	theErr                                                  = theEncoder.Convert(theValue->theData, theData, theValue->theEncoding, theEncoding);

	if (theErr != kNoErr)
		NN_LOG("Unable to convert '%@' to encoding %d", *this, theEncoding);



	// Render the string
	if (theErr == kNoErr)
		{
		if (renderAs & kNStringUnicodeBOM)
			theParser.AddBOM(theData, theEncoding);

		if (renderAs & kNStringNullTerminate)
			theEncoder.AddTerminator(theData, theEncoding);
		}

	return(theData);
   }





   //============================================================================
   //		NString::SetData : Set the string.
   //----------------------------------------------------------------------------
   NStatus NString::SetData(const NData &theData, NStringEncoding theEncoding)
   {	NStringEncoding			bestEncoding;
	NStringEncoder			theEncoder;
	NStringValue			*theValue;
	NData					bestData;
	NStatus					theErr;



	// Get the state we need
	theValue                                                = GetMutable();
	bestEncoding                                            = GetBestEncoding(theData, theEncoding);



	// Convert the data
	theErr                                                  = theEncoder.Convert(theData, bestData, theEncoding, bestEncoding);
	NN_ASSERT_NOERR(theErr);

	if (theErr != kNoErr)
		return(theErr);



	// Update our state
	theEncoder.AddTerminator(bestData, bestEncoding);

	theValue->theEncoding                                   = bestEncoding;
	theValue->theData                                       = bestData;

	ValueChanged(theValue);

	return(kNoErr);
   }





   //============================================================================
   //		NString::Find : Find a substring.
   //----------------------------------------------------------------------------
   NRange NString::Find(const NString &theString, NStringFlags theFlags, const NRange &theRange) const
   {	NRangeList		theResults;



	// Find the string
	theResults                                              = FindMatches(theString, theFlags, theRange, false);

	return(theResults.empty() ? kNRangeNone : theResults[0]);
   }





   //============================================================================
   //		NString::FindAll : Find every instance of a substring.
   //----------------------------------------------------------------------------
   NRangeList NString::FindAll(const NString &theString, NStringFlags theFlags, const NRange &theRange) const
   {	NRangeList		theResults;



	// Find the strings
	theResults                                              = FindMatches(theString, theFlags, theRange, true);

	return(theResults);
   }





   //============================================================================
   //		NString::Replace : Replace a substring.
   //----------------------------------------------------------------------------
   void NString::Replace(const NRange &theRange, const NString &replaceWith)
   {	NString		thePrefix, theSuffix, theResult;



	// Replace the string
	if (IsFullRange(theRange))
 * this                                                     = replaceWith;



	// Replace a substring
	else
		{
		if (theRange.GetFirst() != 0)
			thePrefix                                       = GetString(NRange(0, theRange.GetFirst()));

		if (theRange.GetNext() < GetSize())
			theSuffix                                       = GetString(NRange(theRange.GetNext(), kNStringLength));

 * this                                                     = thePrefix + replaceWith + theSuffix;
		}
   }





   //============================================================================
   //		NString::Replace : Replace a substring.
   //----------------------------------------------------------------------------
   bool NString::Replace(const NString &theString, const NString &replaceWith, NStringFlags theFlags, const NRange &theRange)
   {	NRange		foundRange;
	bool		wasFound;



	// Replace the substring
	foundRange                                              = Find(theString, theFlags, theRange);
	wasFound                                                = (foundRange != kNRangeNone);

	if (wasFound)
		Replace(foundRange, replaceWith);

	return(wasFound);
   }





   //============================================================================
   //		NString::ReplaceAll : Replace every instance of a substring.
   //----------------------------------------------------------------------------
   NIndex NString::ReplaceAll(const NString &theString, const NString &replaceWith, NStringFlags theFlags, const NRange &theRange)
   {	NString					newString, thePrefix;
	NRangeList				foundRanges;
	NRange					foundRange;
	NUnicodeParser			theParser;
	NIndex					theIndex;
	NRangeListIterator		theIter;



	// Get the state we need
	foundRanges                                             = FindAll(theString, theFlags, theRange);
	if (foundRanges.empty())
		return(0);



	// Add the prefix
	//
	// To perform well when replacing a large number of small items (e.g., converting
	// line endings) we parse the string once then rebuild it using the original text
	// and the replaced ranges.
	theParser                                               = GetParser();
	theIndex                                                = 0;

	for (theIter                                            = foundRanges.begin(); theIter != foundRanges.end(); theIter++)
		{
		foundRange                                          = *theIter;

		if (foundRange.GetFirst() != theIndex)
			newString += GetString(theParser, NRange(theIndex, foundRange.GetFirst() - theIndex));

		newString += replaceWith;
		theIndex                                            = foundRange.GetNext();
		}

	if (theIndex < GetSize())
		newString += GetString(theParser, NRange(theIndex, kNStringLength));



	// Replace the string
 * this                                                     = newString;

	return((NIndex) foundRanges.size());
   }





   //============================================================================
   //		NString::Transform : Transform the string.
   //----------------------------------------------------------------------------
   void NString::Transform(NStringTransform theTransform, const NRange &theRange)
   {	NString		newString;



	// Validate our parameters
	if (theTransform & kNStringToLowerCase)
		NN_ASSERT(!(theTransform & kNStringToUpperCase));

	if (theTransform & kNStringToUpperCase)
		NN_ASSERT(!(theTransform & kNStringToLowerCase));

	if (theTransform & kNStringCapitalizeWords)
		NN_ASSERT(!(theTransform & kNStringCapitalizeSentences));

	if (theTransform & kNStringCapitalizeSentences)
		NN_ASSERT(!(theTransform & kNStringCapitalizeWords));



	// Get the state we need
	newString                                               = GetString(theRange);
	if (newString.IsEmpty())
		return;



	// Apply the transforms
	if (theTransform & kNStringToLowerCase)
		newString.CapitalizeCharacters(false);

	if (theTransform & kNStringToUpperCase)
		newString.CapitalizeCharacters(true);

	if (theTransform & kNStringCapitalizeWords)
		newString.CapitalizeWords();

	if (theTransform & kNStringCapitalizeSentences)
		newString.CapitalizeSentences();

	if (theTransform & kNStringStripDiacritics)
		newString                                           = NTargetSystem::TransformString(newString, theTransform);

	Replace(theRange, newString);
   }





   //============================================================================
   //		NString::GetUpper : Get as upper case.
   //----------------------------------------------------------------------------
   NString NString::GetUpper(void) const
   {	NString		theString;



	// Get the string
	theString                                               = *this;
	theString.Transform(kNStringToUpperCase);

	return(theString);
   }





   //============================================================================
   //		NString::GetLower : Get as lower case.
   //----------------------------------------------------------------------------
   NString NString::GetLower(void) const
   {	NString		theString;



	// Get the string
	theString                                               = *this;
	theString.Transform(kNStringToLowerCase);

	return(theString);
   }





   //============================================================================
   //		NString::StartsWith : Does the string start with a string?
   //----------------------------------------------------------------------------
   bool NString::StartsWith(const NString &theString, NStringFlags theFlags) const
   {	const utf8_t	*thisUTF8, *prefixUTF8;
	NString			matchString;
	NRange			theRange;
	NIndex			theSize;
	bool			isMatch;



	// Simple test
	if (theFlags == kNStringNone)
		{
		theSize                                             = theString.GetSize();
		isMatch                                             = (theSize <= GetSize());

		if (isMatch)
			{
			thisUTF8                                        =           GetUTF8();
			prefixUTF8                                      = theString.GetUTF8();

			NN_ASSERT(strlen(prefixUTF8) <= strlen(thisUTF8));
			isMatch                                         = (memcmp(thisUTF8, prefixUTF8, strlen(prefixUTF8)) == 0);
			}
		}


	// General test
	else
		{
		if (theFlags & kNStringPattern)
			matchString.Format("^%@",       theString);
		else
			matchString.Format("^\\Q%@\\E", theString);

		theRange                                            = Find(matchString, theFlags | kNStringPattern);
		isMatch                                             = !theRange.IsEmpty();
		}

	return(isMatch);
   }





   //============================================================================
   //		NString::EndsWith : Does the string end with a string?
   //----------------------------------------------------------------------------
   bool NString::EndsWith(const NString &theString, NStringFlags theFlags) const
   {	NIndex			theSize, thisLen, suffixLen;
	const utf8_t	*thisUTF8, *suffixUTF8;
	NString			matchString;
	NRange			theRange;
	bool			isMatch;



	// Simple test
	if (theFlags == kNStringNone)
		{
		theSize                                             = theString.GetSize();
		isMatch                                             = (theSize <= GetSize());

		if (isMatch)
			{
			thisUTF8                                        =           GetUTF8();
			suffixUTF8                                      = theString.GetUTF8();

			thisLen                                         = (NIndex) strlen(thisUTF8);
			suffixLen                                       = (NIndex) strlen(suffixUTF8);

			NN_ASSERT(suffixLen <= thisLen);
			isMatch                                         = (memcmp(thisUTF8 + thisLen - suffixLen, suffixUTF8, suffixLen) == 0);
			}
		}


	// General test
	else
		{
		if (theFlags & kNStringPattern)
			matchString.Format("%@$",       theString);
		else
			matchString.Format("\\Q%@\\E$", theString);

		theRange                                            = Find(matchString, theFlags | kNStringPattern);
		isMatch                                             = !theRange.IsEmpty();
		}

	return(isMatch);
   }





   //============================================================================
   //		NString::Contains : Does the string contain a string?
   //----------------------------------------------------------------------------
   bool NString::Contains(const NString &theString, NStringFlags theFlags) const
   {	NRange		theRange;



	// Find the string
	theRange                                                = Find(theString, theFlags);

	return(!theRange.IsEmpty());
   }





   //============================================================================
   //		NString::Compare : Compare the value.
   //----------------------------------------------------------------------------
   NComparison NString::Compare(const NString &theValue) const
   {


	// Compare the value
	return(Compare(theValue, kNStringNone));
   }





   //============================================================================
   //		NString::Compare : Compare the value.
   //----------------------------------------------------------------------------
   NComparison NString::Compare(const NString &theString, NStringFlags theFlags) const
   {	NIndex					indexA, indexB, sizeA, sizeB;
	bool					ignoreCase, isNumeric;
	uint64_t				numberA, numberB;
	const NStringValue		*valueA, *valueB;
	utf32_t					charA, charB;
	NComparison				theResult;



	// Validate our parameters
	NN_ASSERT((theFlags & kNStringPattern)   == kNStringNone);
	NN_ASSERT((theFlags & kNStringBackwards) == kNStringNone);



	// Get the state we need
	valueA                                                  =           GetImmutable();
	valueB                                                  = theString.GetImmutable();

	ignoreCase                                              = ((theFlags & kNStringNoCase)  != kNStringNone);
	isNumeric                                               = ((theFlags & kNStringNumeric) != kNStringNone);



	// Check for identity
	if (valueA == valueB)
		return(kNCompareEqualTo);



	// Check for literal comparison
	//
	// Dictionary keys are normally compared with NStringHashCompare, which
	// uses a hash code test before trying a literal comparison.
	//
	// Since most keys are UTF8 strings, we special-case this comparison with
	// a simple strcmp of the raw UTF8 characters.
	if (theFlags == kNStringNone)
		{
		if (valueA->theEncoding == kNStringEncodingUTF8 && valueB->theEncoding == kNStringEncodingUTF8)
			return(GetComparison(strcmp(GetUTF8(), theString.GetUTF8())));
		}



	// Parse the strings
	//
	// Parsers are declared here to avoid creating them until we know we need
	// to do a slow compare (to ensure the common fast case above doesn't have
	// to create/destroy objects it doesn't need).
	NUnicodeParser			parserA, parserB;

	parserA                                                 =           GetParser();
	parserB                                                 = theString.GetParser();

	sizeA                                                   = parserA.GetSize();
	sizeB                                                   = parserB.GetSize();



	// Compare the strings
	//
	// Should probably use ICU for standard Unicode collation.
	theResult                                               = GetComparison(sizeA, sizeB);
	indexA                                                  = 0;
	indexB                                                  = 0;

	while (indexA < sizeA && indexB < sizeB)
		{
		// Get the characters
		charA                                               = parserA.GetChar(indexA, ignoreCase);
		charB                                               = parserB.GetChar(indexB, ignoreCase);



		// Compare numerically
		if (isNumeric && parserA.IsDigit(charA) && parserB.IsDigit(charB))
			{
			// Parse the number
			//
			// This will advance the index to the last digit of each number.
			numberA                                         = GetNumber(parserA, indexA, sizeA, charA);
			numberB                                         = GetNumber(parserB, indexB, sizeB, charB);



			// Compare numerically
			if (numberA != numberB)
				{
				theResult                                   = GetComparison(numberA, numberB);
				break;
				}

			if (indexA == (sizeA-1) || indexB == (sizeB-1))
				break;
			}



		// Compare alphabetically
		if (charA != charB)
			{
			theResult                                       = GetComparison(charA, charB);
			break;
			}



		// Advance to the next character
		indexA++;
		indexB++;
		}

	return(theResult);
   }





   //============================================================================
   //		NString::EqualTo : Compare two strings.
   //----------------------------------------------------------------------------
   bool NString::EqualTo(const NString &theString, NStringFlags theFlags) const
   {


	// Check the size
	//
	// Since we only want to know if the strings are equal/not, we can do a
	// fast test on length before looking at the string data.
	if (GetSize() != theString.GetSize())
		return(false);



	// Check the hash code
	//
	// When case is significant, we can do a fast test for inequality using
	// the hash code.
	//
	// Since two different strings may hash to the same value, if we can't
	// prove inequality we must fall through to the general case.
	if ((theFlags & kNStringNoCase) == kNStringNone)
		{
		if (GetHash() != theString.GetHash())
			return(false);
		}



	// General equality
	return(Compare(theString, theFlags) == kNCompareEqualTo);
   }





   //============================================================================
   //		NString::GetLeft : Get a left-anchored substring.
   //----------------------------------------------------------------------------
   NString NString::GetLeft(NIndex theSize) const
   {


	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Check the size
	if (theSize >= GetSize())
		return(*this);



	// Get the substring
	return(GetString(NRange(0, theSize)));
   }





   //============================================================================
   //		NString::GetRight : Get a right-anchored substring.
   //----------------------------------------------------------------------------
   NString NString::GetRight(NIndex theSize) const
   {


	// Validate our parameters
	NN_ASSERT(theSize >= 0);



	// Check the size
	if (theSize >= GetSize())
		return(*this);



	// Get the substring
	return(GetString(NRange(GetSize() - theSize, theSize)));
   }





   //============================================================================
   //		NString::GetString : Get a substring.
   //----------------------------------------------------------------------------
   NString NString::GetString(NIndex thePosition) const
   {


	// Validate our parameters
	NN_ASSERT(thePosition >= 0);



	// Check the size
	if (thePosition >= GetSize())
		return("");



	// Get the substring
	return(GetString(NRange(thePosition, kNStringLength)));
   }





   //============================================================================
   //		NString::GetString : Get a substring.
   //----------------------------------------------------------------------------
   NString NString::GetString(const NRange &theRange) const
   {


	// Get the string
	if (IsFullRange(theRange))
		return(*this);


	// Get a substring
	else
		return(GetString(GetParser(), theRange));
   }





   //============================================================================
   //		NString::Split : Split a string.
   //----------------------------------------------------------------------------
   NStringList NString::Split(const NString &theString, NStringFlags theFlags) const
   {	NString						subString, matchString;
	NRange						theRange, subRange;
	NRangeList					theMatches;
	NIndex						offsetPrev;
	NUnicodeParser				theParser;
	NRangeListConstIterator		iterMatch;
	NStringList					theResult;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Get the state we need
	matchString                                             = GetWhitespacePattern(theString, theFlags);
	theParser                                               = GetParser();
	offsetPrev                                              = 0;



	// Find the split points
	theMatches                                              = FindAll(matchString, theFlags);

	if (theMatches.empty())
		theResult.push_back(*this);



	// Split the string
	//
	// If we found anything, we add a final range to the matches to ensure we
	// include any text beyond the last split point into the result.
	if (!theMatches.empty())
		theMatches.push_back(NRange(GetSize(), 1));

	for (iterMatch                                          = theMatches.begin(); iterMatch != theMatches.end(); iterMatch++)
		{
		theRange                                            = *iterMatch;
		subRange                                            = NRange(offsetPrev, theRange.GetLocation() - offsetPrev);
		subString                                           = GetString(theParser, subRange);

		theResult.push_back(subString);
		offsetPrev                                          = theRange.GetNext();
		}



	// Discard leading/trailing dividers
	if (!theResult.empty() && theResult.front().IsEmpty())
		vector_pop_front(theResult);

	if (!theResult.empty() && theResult.back().IsEmpty())
		theResult.pop_back();

	return(theResult);
   }





   //============================================================================
   //		NString::Join : Join strings.
   //----------------------------------------------------------------------------
   NString NString::Join(const NStringList &theStrings, const NString &joinWith, bool skipEmpty)
   {	NString						theResult;
	NStringListConstIterator	theIter;



	// Combine the strings
	for (theIter                                            = theStrings.begin(); theIter != theStrings.end(); theIter++)
		{
		if (skipEmpty && theIter->IsEmpty())
			continue;

		theResult += *theIter;

		if ((theIter + 1) != theStrings.end())
			theResult += joinWith;
		}

	return(theResult);
   }





   //============================================================================
   //		NString::TrimLeft : Trim a string on the left.
   //----------------------------------------------------------------------------
   void NString::TrimLeft(const NString &theString, NStringFlags theFlags)
   {	bool		isWhitespace;
	NString		trimString;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Check our state
	if (IsEmpty())
		return;



	// Get the state we need
	isWhitespace                                            = (theFlags == kNStringNone && theString == kNStringWhitespace);
	if (!isWhitespace)
		{
		trimString                                          = GetWhitespacePattern(theString, theFlags);

		if (theFlags & kNStringPattern)
			trimString.Format("(?-m)^%@",       trimString);
		else
			trimString.Format("(?-m)^\\Q%@\\E", trimString);
		}



	// Trim the string
	if (isWhitespace)
		TrimWhitespace(true, false);
	else
		Replace(trimString, "", theFlags | kNStringPattern);
   }





   //============================================================================
   //		NString::TrimRight : Trim a string on the right.
   //----------------------------------------------------------------------------
   void NString::TrimRight(const NString &theString, NStringFlags theFlags)
   {	bool		isWhitespace, isSimple;
	NString		trimString;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Check our state
	if (IsEmpty())
		return;



	// Get the state we need
	isWhitespace                                            = (theFlags == kNStringNone && theString == kNStringWhitespace);
	isSimple                                                = (theFlags == kNStringNone);

	if (!isWhitespace && !isSimple)
		{
		trimString                                          = GetWhitespacePattern(theString, theFlags);

		if (theFlags & kNStringPattern)
			trimString.Format("(?-m)%@$",       trimString);
		else
			trimString.Format("(?-m)\\Q%@\\E$", trimString);
		}



	// Trim the string
	if (isWhitespace)
		TrimWhitespace(false, true);

	else if (isSimple)
		{
		if (EndsWith(theString))
			TrimRight(theString.GetSize());
		}

	else
		Replace(trimString, "", theFlags | kNStringPattern);
   }





   //============================================================================
   //		NString::Trim : Trim a string at both ends.
   //----------------------------------------------------------------------------
   void NString::Trim(const NString &theString, NStringFlags theFlags)
   {	bool	isWhitespace;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Check our state
	if (IsEmpty())
		return;



	// Get the state we need
	isWhitespace                                            = (theFlags == kNStringNone && theString == kNStringWhitespace);



	// Trim the string
	if (isWhitespace)
		TrimWhitespace(true, true);
	else
		{
		TrimLeft( theString, theFlags);
		TrimRight(theString, theFlags);
		}
   }





   //============================================================================
   //		NString::TrimLeft : Trim a string on the left.
   //----------------------------------------------------------------------------
   void NString::TrimLeft(NIndex theSize)
   {


	// Trim the string
	Trim(NRange(0, theSize));
   }





   //============================================================================
   //		NString::TrimRight : Trim a string on the right.
   //----------------------------------------------------------------------------
   void NString::TrimRight(NIndex theSize)
   {


	// Trim the string
	Trim(NRange(GetSize() - theSize, theSize));
   }





   //============================================================================
   //		NString::Trim : Trim a string.
   //----------------------------------------------------------------------------
   void NString::Trim(const NRange &theRange)
   {	NRange				finalRange, byteRange;
	NStringValue		*theValue;
	NData				theData;



	// Get the state we need
	theValue                                                = GetMutable();
	finalRange                                              = GetNormalized(theRange);

	if (finalRange.IsEmpty())
		return;



	// Trim the string
	byteRange                                               = GetParser().GetRange(finalRange);
	theValue->theData.RemoveData(byteRange);
	ValueChanged(theValue);
   }





   //============================================================================
   //		NString::Format : Format the string.
   //----------------------------------------------------------------------------
   void NString::Format(const NString &theFormat, NN_FORMAT_ARGS_PARAM)
   {	NStringFormatter	theFormatter;



	// Format the string
 * this                                                     = theFormatter.Format(theFormat, NN_FORMAT_ARGS_LIST);
   }





   //============================================================================
   //		NString::== : Equality operator
   //----------------------------------------------------------------------------
   bool NString::operator == (const NString &theString) const
   {


	// Compare the objects
	//
	// Since we just need to test equality, we can use a case-sensitive
	// EqualTo to perform a fast hash-based test before a full comparison.
	return(EqualTo(theString, kNStringNone));
   }





   //============================================================================
   //		NString::!= : Inequality operator.
   //----------------------------------------------------------------------------
   bool NString::operator != (const NString &theString) const
   {


	// Compare the objects
	//
	// Since we just need to test equality, we can use a case-sensitive
	// EqualTo to perform a fast hash-based test before a full comparison.
	return(!EqualTo(theString, kNStringNone));
   }





   //============================================================================
   //		NString::+= : Append a string.
   //----------------------------------------------------------------------------
   const NString& NString::operator += (const NString &theString)
   {	const NStringValue		*otherValue;
	NStringEncoder			theEncoder;
	NStringValue			*theValue;
	NData					theData;
	NStatus					theErr;



	// Check our parameters
	if (theString.IsEmpty())
		return(*this);



	// Get the state we need
	theValue                                                = GetMutable();
	otherValue                                              = theString.GetImmutable();



	// Prepare the data
	if (theValue->theEncoding == otherValue->theEncoding)
		theData                                             = otherValue->theData;
	else
		{
		theErr                                              = theEncoder.Convert(otherValue->theData, theData, otherValue->theEncoding, theValue->theEncoding);
		NN_ASSERT_NOERR(theErr);

		if (theErr != kNoErr)
			return(*this);

		theEncoder.AddTerminator(theData, theValue->theEncoding);
		}



	// Append the string
	//
	// To avoid re-parsing, we update the size directly.
	//
	// Strings that are appended to are often appended to multiple times, e.g., while
	// concatenating output to a buffer. To help amortize the cost of growing the data,
	// we reserve some additional space when appending to the string.
	theEncoder.RemoveTerminator(theValue->theData, theValue->theEncoding);

	theValue->theSize += otherValue->theSize;
	theValue->theData += theData;
	theValue->theData.Reserve((NIndex) (theValue->theSize * kAppendGrowth));

	ValueChanged(theValue, false);

	return(*this);
   }





   //============================================================================
   //		NString::+ : Append a string.
   //----------------------------------------------------------------------------
   NString NString::operator + (const NString &theString) const
   {	NString		theResult;



	// Check our parameters
	if (theString.IsEmpty())
		return(*this);



	// Append the string
	theResult                                               = *this;
	theResult += theString;

	return(theResult);
   }





 #pragma mark protected
   //============================================================================
   //		NString::GetNullValue : Get the null value.
   //----------------------------------------------------------------------------
   const NStringValue *NString::GetNullValue(void) const
   {	static NStringValue		sNullValue                        = { 0, kNStringEncodingUTF8, NData(1) };



	// Get the value
	return(&sNullValue);
   }





   //============================================================================
   //		NString::CalculateHash : Calculate the hash.
   //----------------------------------------------------------------------------
   NHashCode NString::CalculateHash(void) const
   {	NHashCode		theResult;



	// Get the hash code
	theResult                                               = kNHashCodeNone;

	if (!IsEmpty())
		theResult                                           = GetData().GetHash();

	return(theResult);
   }





   //============================================================================
   //      NString::EncodeSelf : Encode the object.
   //----------------------------------------------------------------------------
   void NString::EncodeSelf(NEncoder &theEncoder) const
   {


	// Encode the object
	theEncoder.EncodeString(kNEncoderValueKey, *this);
   }





   //============================================================================
   //      NString::DecodeSelf : Decode the object.
   //----------------------------------------------------------------------------
   void NString::DecodeSelf(const NEncoder &theEncoder)
   {


	// Decode the object
 * this                                                     = theEncoder.DecodeString(kNEncoderValueKey);
   }





 #pragma mark private
   //============================================================================
   //      NString::ValueChanged : Our value has been changed.
   //----------------------------------------------------------------------------
   void NString::ValueChanged(NStringValue *theValue, bool updateSize)
   {


	// Update our value
	if (theValue != NULL && updateSize)
		theValue->theSize                                   = GetParser().GetSize();



	// Update the debug summary
 #if NN_DEBUG
	UpdateSummary("%s", GetUTF8());
 #endif



	// Reset our state
	//
	// State that depends on our value needs to be reset whenever it changes.
	//
	// To help expose stale pointers returned through GetUTFxxx(), we scrub
	// this buffer in debug builds (vs just freeing the memory).
	ClearHash();

 #if NN_DEBUG
	if (!mData.IsEmpty())
		memset(mData.GetData(), 'N', mData.GetSize());
 #else
	mData.Clear();
 #endif
   }





   //============================================================================
   //		NString::FindMatches : Find a string.
   //----------------------------------------------------------------------------
   NRangeList NString::FindMatches(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
   {	bool			isBackwards, isPattern;
	NRangeList		theResults;
	NRange			finalRange;



	// Get the state we need
	finalRange                                              = GetNormalized(theRange);
	isBackwards                                             = ((theFlags & kNStringBackwards) == kNStringBackwards);
	isPattern                                               = ((theFlags & kNStringPattern)   == kNStringPattern);



	// Check the size
	//
	// A literal search string can be checked for length, however a pattern-based
	// search may be larger than the text it matches ("^\s+" will match " ").
	if (IsEmpty() || theString.IsEmpty())
		return(theResults);

	if (!isPattern && theString.GetSize() > finalRange.GetSize())
		return(theResults);



	// Reverse search
	//
	// Reverse searches are very inefficient, since we do a forwards search for
	// every instance which we then need to reverse. This should be improved.
	if (isBackwards)
		{
		// Find everything
		if (isPattern)
			theResults                                      = FindPattern(theString, theFlags, finalRange, true);
		else
			theResults                                      = FindString( theString, theFlags, finalRange, true);


		// Adjust the result
		if (!theResults.empty())
			{
			if (doAll)
				reverse(theResults);
			else
				theResults                                  = nvector(theResults.back());
			}
		}


	// Forward search
	else
		{
		if (isPattern)
			theResults                                      = FindPattern(theString, theFlags, finalRange, doAll);
		else
			theResults                                      = FindString( theString, theFlags, finalRange, doAll);
		}

	return(theResults);
   }





   //============================================================================
   //      NString::FindString : Find a string.
   //----------------------------------------------------------------------------
   NRangeList NString::FindString(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
   {	NIndex				sizeB, n, offsetB, limitA, rangeFirst, rangeLast;
	bool				ignoreCase, updateB;
	NUnicodeParser		parserA, parserB;
	utf32_t				charA, charB;
	NRange				findRange;
	NRangeList			theResult;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Get the state we need
	parserA                                                 =           GetParser();
	parserB                                                 = theString.GetParser();

	sizeB                                                   = parserB.GetSize();
	charB                                                   = 0;

	rangeFirst                                              = theRange.GetFirst();
	rangeLast                                               = theRange.GetLast();

	limitA                                                  = std::max((NIndex) 1, rangeLast - sizeB);
	ignoreCase                                              = (theFlags & kNStringNoCase);

	findRange                                               = kNRangeNone;
	updateB                                                 = true;
	offsetB                                                 = 0;



	// Find the string
	for (n                                                  = rangeFirst; n <= rangeLast; n++)
		{
		// Get the state we need
		charA                                               = parserA.GetChar(n, ignoreCase);

		if (updateB)
			{
			charB                                           = parserB.GetChar(offsetB, ignoreCase);
			updateB                                         = false;
			}



		// Check for a match
		if (findRange.IsEmpty())
			{
			// Found a match
			if (charA == charB)
				{
				findRange                                   = NRange(n, 1);
				updateB                                     = true;
				offsetB                                     = 1;
				}

			// No match found
			else
				{ }
			}


		// Continue matching
		else
			{
			// Continue the match
			if (charA == charB)
				{
				findRange.SetSize(findRange.GetSize() + 1);
				updateB                                     = true;
				offsetB += 1;
				}

			// Cancel the match
			//
			// Since we're inside a match we need to keep charA at the same position
			// so that it can be re-tested against the first character in B.
			//
			// As the loop counter is an NIndex, and so signed, we can just decrement
			// it now so that the loop advances it back to the current position.
			else
				{
				findRange                                   = kNRangeNone;
				updateB                                     = true;
				offsetB                                     = 0;

				n--;
				}
			}



		// Save the match
		if (offsetB == sizeB)
			{
			theResult.push_back(findRange);
			if (!doAll)
				break;

			findRange                                       = kNRangeNone;
			updateB                                         = true;
			offsetB                                         = 0;
			}



		// Check the limit
		//
		// If we've found a match we need to search to the end of the range, but once we
		// pass the final B-sized section without a match then we know it can't fit.
		if (n > limitA && findRange.IsEmpty())
			break;
		}

	return(theResult);
   }





   //============================================================================
   //      NString::FindPattern : Find a pattern.
   //----------------------------------------------------------------------------
   NRangeList NString::FindPattern(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
   {	NIndex					n, matchStart, matchEnd, offsetFirst, offsetLast, searchSize, searchOffset;
	int						regFlags, regErr, numMatches, errorPos;
	const utf8_t			*textUTF8, *searchUTF8, *errorMsg;
	const NRangeList		*theRanges;
	NRange					matchRange;
	std::vector<int>		theMatches;
	NUnicodeParser			theParser;
	NRangeList				theResult;
	NData					dataUTF8;
	pcre					*regExp;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Get the state we need
	numMatches                                              = 0;
	regFlags                                                = PCRE_MULTILINE | PCRE_DOTALL | PCRE_UTF8 | PCRE_NO_UTF8_CHECK;

	if (theFlags & kNStringNoCase)
		regFlags |= PCRE_CASELESS;

	dataUTF8                                                = GetData(kNStringEncodingUTF8);
	textUTF8                                                = (const utf8_t *) dataUTF8.GetData();

	theParser.Parse(dataUTF8, kNStringEncodingUTF8);
	theRanges                                               = theParser.GetRanges();



	// Get the range to search
	NN_ASSERT(theRange.GetFirst() < (NIndex) theRanges->size());
	NN_ASSERT(theRange.GetLast()  < (NIndex) theRanges->size());

	offsetFirst                                             = theRanges->at(theRange.GetFirst()).GetFirst();
	offsetLast                                              = theRanges->at(theRange.GetLast() ).GetLast();

	searchUTF8                                              = textUTF8   + offsetFirst;
	searchSize                                              = offsetLast - offsetFirst + 1;
	searchOffset                                            = 0;



	// Compile the expression
	regExp                                                  = pcre_compile(theString.GetUTF8(), regFlags, &errorMsg, &errorPos, NULL);
	if (regExp == NULL)
		{
		NN_LOG("Unable to compile '%@': %s (%d)", theString, errorMsg, errorPos);
		return(theResult);
		}

	regErr                                                  = pcre_fullinfo(regExp, NULL, PCRE_INFO_CAPTURECOUNT, &numMatches);
	if (regErr != 0)
		{
		NN_LOG("Unable to extract capture count");
		return(theResult);
		}



	// Execute the expression
	theMatches.resize((numMatches+1) * 3);

	while (searchOffset >= 0)
		{
		// Apply the expression
		regErr                                              = pcre_exec(regExp, NULL, searchUTF8, searchSize, searchOffset, PCRE_NO_UTF8_CHECK, &theMatches[0], (int) theMatches.size());
		NN_ASSERT(regErr == PCRE_ERROR_NOMATCH || regErr > 0);



		// Collect the results
		//
		// Offsets are returned as byte offsets relative to the start of the search
		// string, which we convert into absolute character offsets.
		if (regErr > 0)
			{
			for (n                                          = 0; n <= numMatches; n++)
				{
				matchStart                                  = theMatches[(n * 2) + 0];
				matchEnd                                    = theMatches[(n * 2) + 1];
				matchRange                                  = kNRangeNone;

				if (matchStart != -1)
					{
					matchStart                              = GetCharacterOffset(theRanges, offsetFirst + matchStart);
					matchEnd                                = GetCharacterOffset(theRanges, offsetFirst + matchEnd);
					matchRange                              = NRange(matchStart, matchEnd - matchStart);
					}

				theResult.push_back(matchRange);
				}
			}



		// Advance the search
		if (doAll)
			{
			matchEnd                                        = theMatches[1];

			if (regErr > 0 && matchEnd != searchSize)
				searchOffset                                = matchEnd;
			else
				searchOffset                                = -1;
			}
		else
			searchOffset                                    = -1;
		}



	// Clean up
	pcre_free(regExp);

	return(theResult);
   }





   //============================================================================
   //		NString::CapitalizeCharacters : Capitalize characters.
   //----------------------------------------------------------------------------
   void NString::CapitalizeCharacters(bool toUpper)
   {	NIndex					n, theSize;
	NUnicodeParser			theParser;
	utf32_t					theChar;
	NData					theData;



	// Get the state we need
	theParser                                               = GetParser();
	theSize                                                 = theParser.GetSize();



	// Capitalize characters
	for (n                                                  = 0; n < theSize; n++)
		{
		theChar                                             = theParser.GetChar(n);
		theChar                                             = toUpper ? theParser.GetUpper(theChar) : theParser.GetLower(theChar);

		theData.AppendData((NIndex) sizeof(theChar), &theChar);
		}



	// Update the string
	SetData(theData, kNStringEncodingUTF32);
   }





   //============================================================================
   //      NString::CapitalizeWords : Capitalize words.
   //----------------------------------------------------------------------------
   void NString::CapitalizeWords(void)
   {	NIndex					n, theSize;
	NUnicodeParser			theParser;
	utf32_t					theChar;
	bool					toUpper;
	NData					theData;



	// Get the state we need
	theParser                                               = GetParser();
	theSize                                                 = theParser.GetSize();
	toUpper                                                 = true;



	// Capitalize words
	for (n                                                  = 0; n < theSize; n++)
		{
		theChar                                             = theParser.GetChar(n);

		if (theParser.IsAlpha(theChar))
			{
			if (toUpper)
				{
				theChar                                     = theParser.GetUpper(theChar);
				toUpper                                     = false;
				}
			}
		else
			toUpper                                         = true;

		theData.AppendData((NIndex) sizeof(theChar), &theChar);
		}



	// Update the string
	SetData(theData, kNStringEncodingUTF32);
   }





   //============================================================================
   //      NString::CapitalizeSentences : Capitalize sentences.
   //----------------------------------------------------------------------------
   void NString::CapitalizeSentences(void)
   {	NIndex					n, theSize;
	NUnicodeParser			theParser;
	utf32_t					theChar;
	bool					toUpper;
	NData					theData;



	// Get the state we need
	theParser                                               = GetParser();
	theSize                                                 = theParser.GetSize();
	toUpper                                                 = true;



	// Capitalize sentences
	for (n                                                  = 0; n < theSize; n++)
		{
		theChar                                             = theParser.GetChar(n);

		if (toUpper)
			{
			if (theParser.IsAlpha(theChar))
				{
				theChar                                     = theParser.GetUpper(theChar);
				toUpper                                     = false;
				}
			}
		else
			toUpper                                         = (theChar == (utf32_t) '!' ||
					   theChar == (utf32_t) '.' ||
					   theChar == (utf32_t) '?');

		theData.AppendData((NIndex) sizeof(theChar), &theChar);
		}



	// Update the string
	SetData(theData, kNStringEncodingUTF32);
   }





   //============================================================================
   //		NString::TrimWhitespace : Trim whitespace from the string.
   //----------------------------------------------------------------------------
   void NString::TrimWhitespace(bool fromLeft, bool fromRight)
   {	const NStringValue		*theValue;



	// Validate our state
	NN_ASSERT(!IsEmpty());



	// Get the state we need
	theValue                                                = GetImmutable();



	// Trim the whitespace
	//
	// Trimming whitespace from a UTF8 string is a very common operation
	// when parsing XML, so we special-case that encoding for performance.
	if (theValue->theEncoding == kNStringEncodingUTF8)
		TrimWhitespaceUTF8(   fromLeft, fromRight);
	else
		TrimWhitespaceGeneric(fromLeft, fromRight);
   }





   //============================================================================
   //		NString::TrimWhitespaceUTF8 : Trim whitespace from a UTF8 string.
   //----------------------------------------------------------------------------
   void NString::TrimWhitespaceUTF8(bool fromLeft, bool fromRight)
   {	NIndex					theSize, indexStart, indexEnd;
	const utf8_t			*textUTF8;
	const NStringValue		*theValue;



	// Validate our state
	NN_ASSERT(!IsEmpty());



	// Get the state we need
	theValue                                                = GetImmutable();
	theSize                                                 = theValue->theSize;
	textUTF8                                                = (const utf8_t *) theValue->theData.GetData();

	indexStart                                              = 0;
	indexEnd                                                = theSize - 1;



	// Trim the left
	if (fromLeft)
		{
		while (indexStart <= indexEnd && isspace(textUTF8[indexStart]))
			indexStart++;
		}



	// Trim the right
	//
	// In UTF8, a multi-byte sequence always has the high bit set in every
	// byte in the sequence.
	//
	// This means that we can just walk back a byte at a time, since any
	// byte within a multi-byte sequence will fail the isspace test.
	if (fromRight)
		{
		while (indexEnd >= indexStart && isspace(textUTF8[indexEnd]))
			indexEnd--;
		}



	// Update our state
	if (indexStart != 0 || indexEnd != (theSize-1))
		{
		theSize                                             = indexEnd - indexStart + 1;
		if (theSize <= 0)
			Clear();
		else
 * this                                                     = NString(&textUTF8[indexStart], theSize);
		}
   }





   //============================================================================
   //		NString::TrimWhitespaceGeneric : Trim whitespace from a generic string.
   //----------------------------------------------------------------------------
   void NString::TrimWhitespaceGeneric(bool fromLeft, bool fromRight)
   {	NIndex				theSize, indexStart, indexEnd;
	NUnicodeParser		theParser;



	// Validate our state
	NN_ASSERT(!IsEmpty());



	// Get the state we need
	theParser                                               = GetParser();
	theSize                                                 = theParser.GetSize();

	indexStart                                              = 0;
	indexEnd                                                = theSize - 1;



	// Trim the left
	if (fromLeft)
		{
		while (indexStart <= indexEnd && theParser.IsSpace(theParser.GetChar(indexStart)))
			indexStart++;
		}



	// Trim the right
	if (fromRight)
		{
		while (indexEnd >= indexStart && theParser.IsSpace(theParser.GetChar(indexEnd)))
			indexEnd--;
		}



	// Update our state
	if (indexStart != 0 || indexEnd != (theSize-1))
		{
		theSize                                             = indexEnd - indexStart + 1;
		if (theSize <= 0)
			Clear();
		else
 * this                                                     = GetString(theParser, NRange(indexStart, theSize));
		}
   }





   //============================================================================
   //      NString::GetCharacterOffset : Get the offset of a character.
   //----------------------------------------------------------------------------
   NIndex NString::GetCharacterOffset(const NRangeList *theRanges, NIndex byteOffset) const
   {	NIndex		n, numItems;



	// Locate the offset
	numItems                                                = (NIndex) theRanges->size();

	for (n                                                  = 0; n < numItems; n++)
		{
		if (theRanges->at(n).GetLocation() == byteOffset)
			return(n);
		}



	// Handle last character
	//
	// We allow the byte beyond the last character to be requested, which returns
	// a fake character one beyond the last character (so that a suitable range
	// can be calculated which takes in the last character).
	n                                                       = theRanges->back().GetNext();

	if (byteOffset == n)
		return(numItems);



	// Handle failure
	NN_LOG("Unable to lcoate character at offset %d", byteOffset);

	return(kNIndexNone);
   }





   //============================================================================
   //      NString::GetBestEncoding : Get the best encoding for a string.
   //----------------------------------------------------------------------------
   NStringEncoding NString::GetBestEncoding(const NData &theData, NStringEncoding theEncoding) const
   {	NStringEncoding			bestEncoding;
	NIndex					n, theSize;
	NUnicodeParser			theParser;
	const utf32_t			*chars32;
	const utf16_t			*chars16;
	NRange					bomRange;
	const utf8_t			*chars8;
	utf16_t					char16;
	utf32_t					char32;



	// Get the state we need
	if (theParser.GetBOM(theData, bomRange) == kNStringEncodingInvalid)
		bomRange                                            = kNRangeNone;



	// Get the best encoding
	//
	// We use UTF8 by default, switching to UTF16 for non-ASCII content.
	switch (theEncoding) {
		case kNStringEncodingUTF8:
		case kNStringEncodingASCII:
		case kNStringEncodingMacRoman:
		case kNStringEncodingISOLatin1:
		case kNStringEncodingWindowsLatin1:
			theSize                                         = theData.GetSize() / ((NIndex) sizeof(utf8_t));
			chars8                                          = (const utf8_t *) theData.GetData();
			bestEncoding                                    = kNStringEncodingUTF8;

			for (n                                          = 0; n < theSize; n++)
				{
				if (bomRange.Contains(n))
					continue;

				if (((uint32_t) chars8[n]) > kNASCIIMax)
					{
					bestEncoding                            = kNStringEncodingUTF16;
					break;
					}
				}
			break;


		case kNStringEncodingUTF16:
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			theSize                                         = theData.GetSize() / ((NIndex) sizeof(utf16_t));
			chars16                                         = (const utf16_t *) theData.GetData();
			bestEncoding                                    = kNStringEncodingUTF8;

			for (n                                          = 0; n < theSize; n++)
				{
				if (bomRange.Contains(n))
					continue;

				char16                                      = theParser.GetNativeUTF16(chars16[n], theEncoding);
				if (((uint32_t) char16) > kNASCIIMax)
					{
					bestEncoding                            = kNStringEncodingUTF16;
					break;
					}
				}
			break;


		case kNStringEncodingUTF32:
		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			theSize                                         = theData.GetSize() / ((NIndex) sizeof(utf32_t));
			chars32                                         = (const utf32_t *) theData.GetData();
			bestEncoding                                    = kNStringEncodingUTF8;

			for (n                                          = 0; n < theSize; n++)
				{
				if (bomRange.Contains(n))
					continue;

				char32                                      = theParser.GetNativeUTF32(chars32[n], theEncoding);
				if (((uint32_t) char32) > kNASCIIMax)
					{
					bestEncoding                            = kNStringEncodingUTF16;
					break;
					}
				}
			break;


		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			bestEncoding                                    = kNStringEncodingUTF16;
			break;
		}

	return(bestEncoding);
   }





   //============================================================================
   //      NString::GetWhitespacePattern : Get a whitespace pattern.
   //----------------------------------------------------------------------------
   NString NString::GetWhitespacePattern(const NString &theString, NStringFlags &theFlags) const
   {	NString		thePattern;



	// Get the state we need
	thePattern                                              = theString;



	// Convert whitespace to a pattern
	//
	// This allows callers to write foo(kNStringWhitespace) rather than
	// explicitly specifying a "+" suffix and kNStringPattern.
	if (thePattern == kNStringWhitespace)
		{
		thePattern += "+";
		theFlags   |= kNStringPattern;
		}



	// Disable pattern matching if we can
	//
	// Single-character patterns area treated as a (faster) literal match.
	else if (thePattern.GetSize() == 1)
		theFlags &= ~kNStringPattern;

	return(thePattern);
   }





   //============================================================================
   //        NString::GetParser : Get a parser.
   //----------------------------------------------------------------------------
   NUnicodeParser NString::GetParser(void) const
   {	const NStringValue		*theValue;
	NUnicodeParser			theParser;



	// Get the parser
	theValue                                                = GetImmutable();

	theParser.Parse(theValue->theData, theValue->theEncoding);

	return(theParser);
   }





   //============================================================================
   //      NString::GetNumber : Get a number.
   //----------------------------------------------------------------------------
   uint64_t NString::GetNumber(const NUnicodeParser &theParser, NIndex &theIndex, NIndex theSize, utf32_t theChar) const
   {	uint64_t	theNumber;



	// Validate our parameters
	NN_ASSERT(theIndex < theSize);
	NN_ASSERT(theParser.IsDigit(theChar));



	// Get the number
	theNumber                                               = 0;

	while (true)
		{
		NN_ASSERT(theNumber <= (kUInt64Max/10));
		theNumber                                           = (theNumber * 10) + (theChar - (utf32_t) '0');

		theIndex++;
		if (theIndex == theSize)
			break;

		theChar                                             = theParser.GetChar(theIndex);
		if (!theParser.IsDigit(theChar))
			break;
		}



	// Adjust the index
	//
	// We leave the index at the last digit in the number.
	theIndex--;

	return(theNumber);
   }





   //============================================================================
   //		NString::GetString : Get a substring.
   //----------------------------------------------------------------------------
   NString NString::GetString(const NUnicodeParser &theParser, const NRange &theRange) const
   {	NRange					finalRange, byteRange;
	const NStringValue		*theValue;
	NString					theResult;
	NData					theData;



	// Get the state we need
	theValue                                                = GetImmutable();
	finalRange                                              = GetNormalized(theRange);

	if (finalRange.IsEmpty())
		return(theResult);



	// Extract the string
	byteRange                                               = theParser.GetRange(finalRange);
	theData                                                 = theValue->theData.GetData(byteRange);
	theResult.SetData(theData, theValue->theEncoding);

	return(theResult);
   }





   //============================================================================
   //		NString::GetConstantString : Get a string constant.
   //----------------------------------------------------------------------------
   NString NString::GetConstantString(const utf8_t *theText)
   {
	// Eternals
	//
	// These must never be freed, otherwise they could be used after
	// they have been destroyed during static destruction
	static NConstantStringMap & sTable                      = *(new NConstantStringMap);
	static NMutex &             sLock                       = *(new NMutex);

	StLock								lockMutex(sLock);
	NString								theString;
	NConstantStringMapConstIterator		theIter;



	// Get the string
	//
	// Constant strings are cached by their pointer, which must persist.
	theIter                                                 = sTable.find(theText);

	if (theIter != sTable.end())
		{
		// Use the cache
		//
		// To help detect bugs where a mutable pointer was incorrectly passed to
		// the no-copy constructor, we validate that the input pointer still
		// matches the cached string in terms of content.
		theString                                           = theIter->second;
		NN_ASSERT(strcmp(theText, theString.GetUTF8()) == 0);
		}
	else
		{
		// Populate the cache
		//
		// Before inserting into the cache we calculate the string's hash code,
		// ensuring that the cached string won't need to become mutable if its
		// hash code is ever needed for a comparison.
		theString                                           = NString(theText, kNStringLength, kNStringEncodingUTF8);
		theString.GetHash();

		sTable[theText]                                     = theString;
		}

	return(theString);
   }


 */
// Nano 3.x implementation
