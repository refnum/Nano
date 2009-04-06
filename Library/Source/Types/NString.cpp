/*	NAME:
		NString.cpp

	DESCRIPTION:
		Strings are stored as NULL-terminated UTF8 strings.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "pcre.h"

#include "NSTLUtilities.h"
#include "NString.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NString kNStringWhitespace									= "\\s";
const NIndex  kNStringSize											= -1;





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const void *thePtr, NIndex numBytes, NStringEncoding theEncoding)
{


	// Initialize ourselves
	if (numBytes == kNStringSize)
		numBytes = strlen((const char *) thePtr);
	
	SetData(NData(numBytes, thePtr), theEncoding);
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
NString::NString(const NStringUTF8 &theString)
{


	// Initialize ourselves
	SetData(NData(theString.GetSize(), theString.GetUTF8()), kNStringEncodingUTF8);
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(void)
{
}





//============================================================================
//		NString::~NString : Destructor.
//----------------------------------------------------------------------------
NString::~NString(void)
{
}





//============================================================================
//		NString::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NString::GetSize(void) const
{	const NStringValue		*theValue;



	// Get the size
	theValue = GetImmutable();

	return(theValue->theSize);
}





//============================================================================
//		NString::GetUTF8 : Get the string.
//----------------------------------------------------------------------------
const char *NString::GetUTF8(void) const
{	const NStringValue		*theValue;
	const char				*theText;



	// Get the string
	theValue = GetImmutable();
	theText  = (const char *) theValue->dataUTF8.GetData();
	
	return(theText);
}





//============================================================================
//		NString::GetData : Get the string.
//----------------------------------------------------------------------------
NData NString::GetData(NStringEncoding theEncoding, NIndex nullBytes) const
{	NStringEncoder			theEncoder;
	const NStringValue		*theValue;
	NData					theData;
	OSStatus				theErr;



	// Get the string
	theValue = GetImmutable();
	theErr   = theEncoder.Convert(theValue->dataUTF8, theData, kNStringEncodingUTF8, theEncoding);

	if (theErr != noErr)
		{
		NN_LOG("Unable to convert '%@' to encoding %d", *this, theEncoding);
		theData.Clear();
		}



	// Add the terminator
	if (theErr == noErr && nullBytes != 0)
		theData.AppendData(nullBytes);

	return(theData);
}





//============================================================================
//		NString::SetData : Set the string.
//----------------------------------------------------------------------------
void NString::SetData(const NData &theData, NStringEncoding theEncoding)
{	NStringEncoder			theEncoder;
	NStringValue			*theValue;
	OSStatus				theErr;



	// Set the string
	theValue = GetMutable();
	theErr   = theEncoder.Convert(theData, theValue->dataUTF8, theEncoding, kNStringEncodingUTF8);

	if (theErr != noErr)
		{
		NN_LOG("Unable to set string data");
		Clear();
		}
	
	
	
	// Update our state
	ValueChanged();
}





//============================================================================
//		NString::Find : Find a substring.
//----------------------------------------------------------------------------
NRange NString::Find(const NString &theString, NStringFlags theFlags, const NRange &theRange) const
{	NRangeList		theResults;



	// Find the string
	theResults = FindMatches(theString, theFlags, theRange, false);

	return(theResults.empty() ? kNRangeNone : theResults[0]);
}





//============================================================================
//		NString::FindAll : Find every instance of a substring.
//----------------------------------------------------------------------------
NRangeList NString::FindAll(const NString &theString, NStringFlags theFlags, const NRange &theRange) const
{	NRangeList		theResults;



	// Find the strings
	theResults = FindMatches(theString, theFlags, theRange, true);

	return(theResults);
}





//============================================================================
//		NString::Replace : Replace a substring.
//----------------------------------------------------------------------------
void NString::Replace(const NRange &theRange, const NString &replaceWith)
{	NString		thePrefix, theSuffix, theResult;



	// Get the state we need
	if (theRange.GetLocation() != 0)
		thePrefix = GetString(NRange(0, theRange.GetLocation() - 1));

	if (theRange.GetNext() < GetSize())
		theSuffix = GetString(theRange.GetNext());



	// Replace the string
	*this = thePrefix + replaceWith + theSuffix;
}





//============================================================================
//		NString::Replace : Replace a substring.
//----------------------------------------------------------------------------
bool NString::Replace(const NString &theString, const NString &replaceWith, NStringFlags theFlags, const NRange &theRange)
{	NRange		foundRange;
	bool		wasFound;



	// Replace the substring
	foundRange = Find(theString, theFlags, theRange);
	wasFound   = (foundRange != kNRangeNone);

	if (wasFound)
		Replace(foundRange, replaceWith);

	return(wasFound);
}





//============================================================================
//		NString::ReplaceAll : Replace every instance of a substring.
//----------------------------------------------------------------------------
UInt32 NString::ReplaceAll(const NString &theString, const NString &replaceWith, NStringFlags theFlags, const NRange &theRange)
{	NRangeList						foundRanges;
	NRangeListReverseIterator		theIter;



	// Replace the string
	foundRanges = FindAll(theString, theFlags, theRange);

	for (theIter = foundRanges.rbegin(); theIter != foundRanges.rend(); theIter++)
		Replace(*theIter, replaceWith);
	
	return(foundRanges.size());
}





//============================================================================
//		NString::StartsWith : Does the string start with a string?
//----------------------------------------------------------------------------
bool NString::StartsWith(const NString &theString, NStringFlags theFlags) const
{	NString		matchString;
	NRange		theRange;
	NIndex		theSize;
	bool		isMatch;



	// Simple test
	if (theFlags == kNStringNone)
		{
		theSize = theString.GetSize();
		isMatch = (theSize <= GetSize());

		if (isMatch)
			isMatch = (memcmp(theString.GetUTF8(), GetUTF8(), theSize) == 0);
		}


	// General test
	else
		{
		if (theFlags & kNStringPattern)
			matchString.Format("^%@",       theString);
		else
			matchString.Format("^\\Q%@\\E", theString);

		theRange = Find(matchString, theFlags | kNStringPattern);
		isMatch  = theRange.IsNotEmpty();
		}
	
	return(isMatch);
}





//============================================================================
//		NString::EndsWith : Does the string end with a string?
//----------------------------------------------------------------------------
bool NString::EndsWith(const NString &theString, NStringFlags theFlags) const
{	NString		matchString;
	NRange		theRange;
	NIndex		theSize;
	bool		isMatch;



	// Simple test
	if (theFlags == kNStringNone)
		{
		theSize = theString.GetSize();
		isMatch = (theSize <= GetSize());

		if (isMatch)
			isMatch = (memcmp(theString.GetUTF8(), GetUTF8() + GetSize() - theSize, theSize) == 0);
		}


	// General test
	else
		{
		if (theFlags & kNStringPattern)
			matchString.Format("%@$",       theString);
		else
			matchString.Format("\\Q%@\\E$", theString);

		theRange = Find(matchString, theFlags | kNStringPattern);
		isMatch  = theRange.IsNotEmpty();
		}
	
	return(isMatch);
}





//============================================================================
//		NString::Contains : Does the string contain a string?
//----------------------------------------------------------------------------
bool NString::Contains(const NString &theString, NStringFlags theFlags) const
{	NRange		theRange;



	// Find the string
	theRange = Find(theString, theFlags);

	return(theRange.IsNotEmpty());
}





//============================================================================
//		NString::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NString::Compare(const NString &theValue) const
{


	// Compare the values
	return(CompareTo(theValue, kNStringNone));
}





//============================================================================
//		NString::CompareTo : Compare two strings.
//----------------------------------------------------------------------------
NComparison NString::CompareTo(const NString &theString, NStringFlags theFlags) const
{	NComparison		theResult;

	// dair, to do
		// implement EqualTo in terms of this, want to have special-cases here
		// for equality via hash code and equality via shared pointer if not checking for case
	theResult = GetComparison(strcmp(GetUTF8(), theString.GetUTF8()));

	return(theResult);
}





//============================================================================
//		NString::EqualTo : Compare two strings.
//----------------------------------------------------------------------------
bool NString::EqualTo(const NString &theString, NStringFlags theFlags) const
{


	// Compare the strings
	//
	// Exact comparisons can use a mis-matched hash code as a cheap test.
	if (!(theFlags & kNStringNoCase) && GetHash() != theString.GetHash())
		return(false);

	return(CompareTo(theString, theFlags) == kNCompareEqualTo);
}





//============================================================================
//		NString::GetUpper : Get as upper case.
//----------------------------------------------------------------------------
NString NString::GetUpper(void) const
{	NString		theString;



	// Get the string
	theString = *this;
	theString.MakeUpper();
	
	return(theString);
}





//============================================================================
//		NString::GetLower : Get as lower case.
//----------------------------------------------------------------------------
NString NString::GetLower(void) const
{	NString		theString;



	// Get the string
	theString = *this;
	theString.MakeLower();
	
	return(theString);
}





//============================================================================
//      NString::GetCapitals : Get as capitalized.
//----------------------------------------------------------------------------
NString NString::GetCapitals(bool eachWord) const
{	NString		theString;



	// Get the string
	theString = *this;
	theString.MakeCapitals(eachWord);
    
	return(theString);
}





//============================================================================
//		NString::MakeUpper : Convert to upper case.
//----------------------------------------------------------------------------
void NString::MakeUpper(void)
{
	// dair, to do
}





//============================================================================
//		NString::MakeLower : Convert to lower case.
//----------------------------------------------------------------------------
void NString::MakeLower(void)
{
	// dair, to do
}





//============================================================================
//      NString::MakeCapitals : Convert to capitalized form.
//----------------------------------------------------------------------------
void NString::MakeCapitals(bool eachWord)
{	NRange			theRange;
	NString			theText;

	
	
	// Capitalize each word
	if (eachWord)
		; // dair, to do	CFStringCapitalize(*this, cfLocale);
		
		
	// Or each sentence
	else
		{
		// Start with lower case
		MakeLower();
			
			
		// Fix the first word
		theRange = Find("^(\\s*[a-z])", kNStringPattern);
		if (theRange.IsNotEmpty())
			{
			theText = GetString(theRange).GetUpper();
			Replace(theRange, theText);
			}
			
			
		// Fix each subsequent sentance
		theRange = kNRangeAll;
		do
			{
			theRange = Find("([\\.\\?\\!]\\s*[a-z])", kNStringPattern, theRange);
			if (theRange.IsNotEmpty())
				{
				theText = GetString(theRange).GetUpper();
				Replace(theRange, theText);
			
				theRange = NRange(theRange.GetNext(), kNIndexNone);
				}
			}
		while (theRange.IsNotEmpty());
		}
}





//============================================================================
//		NString::GetLeft : Get a left-anchored substring.
//----------------------------------------------------------------------------
NString NString::GetLeft(NIndex theSize) const
{


	// Check for overflow
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


	// Check for overflow
	if (theSize >= GetSize())
		return(*this);



	// Get the substring
	return(GetString(NRange(GetSize() - theSize, theSize)));
}





//============================================================================
//		NString::GetString : Get a substring.
//----------------------------------------------------------------------------
NString NString::GetString(const NRange &theRange) const
{	NString		theResult;



	// Get the substring
	if (theRange.GetLocation() < GetSize())
		; // dair, to do theResult.Set(CFStringCreateWithSubstring(kCFAllocatorNano, *this, NormalizeRange(theRange)));

	return(theResult);
}





//============================================================================
//		NString::Split : Split a string.
//----------------------------------------------------------------------------
NStringList NString::Split(const NString &theString, bool isExact) const
{
// dair, to do
/*
	NString					tmpString, subString, matchString;
	UInt32					n, numItems;
	NStringList				theStrings;
	NArray					theArray;
	NRange					theRange;
	NStringListIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());



	// Split with an exact match
	if (isExact || theString.GetSize() == 1)
		{
		if (theArray.Set(CFStringCreateArrayBySeparatingStrings(kCFAllocatorNano, *this, theString)))
			{
			numItems = theArray.GetSize();
			theStrings.reserve(numItems);
			
			for (n = 0; n < numItems; n++)
				theStrings.push_back(theArray.GetValueString(n));
			}
		}
	
	
	// Or split with any character
	else
		{
		matchString.Format("[\\Q%@\\E]", theString);
		tmpString = *this;
	
		while (tmpString.IsNotEmpty())
			{
			theRange = tmpString.Find(matchString, kCFComparePattern);

			if (theRange.Size == 0)
				{
				subString = tmpString;
				tmpString.Clear();
				}
			else
				{
				subString = tmpString.GetLeft(	theRange.location);
				tmpString = tmpString.GetString(theRange.location + theRange.Size);
				}
			
			theStrings.push_back(subString);
			}

		}



	// Clean up the results
	//
	// CFStringCreateArrayBySeparatingStrings can return empty strings in some situations:
	//
	//		- When target is empty, returns a list containing the empty string
	//		- When target equals split, returns a list containing two empty strings
	//		- When target starts with split, returns a list with an initial empty string
	//
	// The pattern matching case can also produce empty strings, and so we normalise both
	// sets of results to produce a list of "things separated by the split" without any
	// empty strings produced by the process of splitting.
	for (theIter = theStrings.begin(); theIter != theStrings.end(); )
		{
		if (theIter->IsEmpty())
			theIter = theStrings.erase(theIter);
		else
			theIter++;
		}

	return(theStrings);
*/
}





//============================================================================
//		NString::Trim : Trim a string at both ends.
//----------------------------------------------------------------------------
void NString::Trim(const NString &theString, bool isExact)
{


	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());



	// Whitespace trim
	if (theString == kNStringWhitespace)
		{
// dair, to do
//		if (MakeMutable())
//			CFStringTrimWhitespace(*this);
		}
	
	
	// General trim
	else
		{
		TrimLeft( theString, isExact);
		TrimRight(theString, isExact);
		}
}





//============================================================================
//		NString::TrimLeft : Trim a string on the left.
//----------------------------------------------------------------------------
void NString::TrimLeft(const NString &theString, bool isExact)
{	NString		thePattern;



	// Trim the string
	thePattern.Format(isExact ? "^\\Q%@" : "^[\\Q%@\\E]+", theString);
	Replace(thePattern, "", kNStringPattern);
}





//============================================================================
//		NString::TrimRight : Trim a string on the right.
//----------------------------------------------------------------------------
void NString::TrimRight(const NString &theString, bool isExact)
{	NString		thePattern;



	// Trim the string
	thePattern.Format(isExact ? "\\Q%@$" : "[\\Q%@\\E]+$", theString);
	Replace(thePattern, "", kNStringPattern);
}





//============================================================================
//		NString::TrimLeft : Trim a string on the left.
//----------------------------------------------------------------------------
void NString::TrimLeft(NIndex theSize)
{	NRange		theRange;



	// Trim the string
	theRange = NRange(0, theSize);

// dair, to do
//	CFStringDelete(*this, NormalizeRange(theRange));
}





//============================================================================
//		NString::TrimRight : Trim a string on the right.
//----------------------------------------------------------------------------
void NString::TrimRight(NIndex theSize)
{	NRange	theRange;



	// Trim the string
	theRange = NRange(GetSize() - theSize, theSize);

// dair, to do
//	CFStringDelete(*this, NormalizeRange(theRange));
}





//============================================================================
//		NString::Format : Format the string.
//----------------------------------------------------------------------------
void NString::Format(const NString &theFormat, FORMAT_ARGS_PARAM)
{	NStringFormatter		theFormatter;



	// Format the string
	*this = theFormatter.Format(theFormat, FORMAT_ARGS_LIST);
}





//============================================================================
//		NString::+= : Append to a string.
//----------------------------------------------------------------------------
const NString& NString::operator += (const NString &theString)
{	const NStringValue		*otherValue;
	const NData				*otherData;
	NStringValue			*theValue;
	NData					*theData;



	// Get the state we need
	theValue = GetMutable();
	theData  = &theValue->dataUTF8;
	
	otherValue = theString.GetImmutable();
	otherData  = &otherValue->dataUTF8;



	// Append the string
	theData->SetSize(theData->GetSize() - 1);
	theData->AppendData(*otherData);



	// Update our state
	ValueChanged();
	
	return(*this);
}





//============================================================================
//		NString::+ : Append to a string.
//----------------------------------------------------------------------------
const NString NString::operator + (const NString &theString) const
{	NString		newString;



	// Append the string
	newString  = *this;
	newString += theString;
	
	return(newString);
}





//============================================================================
//		NString::NStringUTF8 : NStringUTF8 operator.
//----------------------------------------------------------------------------
NString::operator NStringUTF8(void) const
{	NStringUTF8		theResult;



	// Get the value
	theResult = NStringUTF8(GetUTF8(), GetSize());
	
	return(theResult);
}





//============================================================================
//		NString::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NString::operator NFormatArgument(void) const
{	NStringUTF8		theResult;



	// Get the value
	theResult = NStringUTF8(GetUTF8(), GetSize());
	
	return(NFormatArgument(theResult));
}





//============================================================================
//		NString::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
const NStringValue *NString::GetNullValue(void) const
{	static NStringValue		sNullValue = { 0, NData() };



	// Get the value
	return(&sNullValue);
}





//============================================================================
//		NString::CalculateHash : Calculate the hash code.
//----------------------------------------------------------------------------
NHashCode NString::CalculateHash(void) const
{	const NStringValue		*theValue = GetImmutable();
	NHashCode				theResult;



	// Calculate the hash code
	if (theValue->theSize == 0)
		theResult = kNHashCodeNone;
	else
		theResult = NHashable::CalculateHash(theValue->dataUTF8.GetSize(), theValue->dataUTF8.GetData());

	return(theResult);
}





//============================================================================
//      NString::ValueChanged : Our value has been changed.
//----------------------------------------------------------------------------
#pragma mark -
void NString::ValueChanged(void)
{


	// Reset our state
	//
	// State that depends on our value needs to be reset whenever it changes.
	ClearHash();
}





//============================================================================
//		NString::NormalizeRange : Normalize a range.
//----------------------------------------------------------------------------
NRange NString::NormalizeRange(const NRange &theRange) const
{	NRange		theResult;
	NIndex		theSize;



	// Normalize the range
	theSize   = GetSize();
	theResult = theRange.GetNormalized(theSize);
	
	if (theResult.GetNext() > theSize)
		theResult.SetSize(theSize - theResult.GetLocation());
	
	return(theResult);
}





//============================================================================
//		NString::FindMatches : Find a string.
//----------------------------------------------------------------------------
NRangeList NString::FindMatches(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
{	NRangeList		theResults;
	NRange			findRange ;
	bool			isPattern;



	// Check our state
	if (IsEmpty() || theString.IsEmpty())
		return(theResults);



	// Get the state we need
	findRange = NormalizeRange(theRange);



	// Find the string
	if (isPattern)
		theResults = FindPattern(theString, theFlags, findRange, doAll);
	else
		theResults = FindString( theString, theFlags, findRange, doAll);
	
	return(theResults);
}





//============================================================================
//      NString::FindString : Find a string.
//----------------------------------------------------------------------------
NRangeList NString::FindString(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
{
// dair, to do
/*
	UInt32			n, numItems;
	NRange			foundRange;
	NRangeList		theResult;
	NCFObject		cfArray;



	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.Size) <= (CFIndex) GetSize());
	NN_ASSERT((theFlags & kCFCompareMaskNano) == 0);



	// Find a single substring
	if (!doAll)
		{
		if (CFStringFindWithOptions(*this, theString, theRange, theFlags, &foundRange))
			theResult.push_back(foundRange);
		}
	

	
	// Find every instance of the substring
	else
		{
		if (cfArray.Set(CFStringCreateArrayWithFindResults(kCFAllocatorNano, *this, theString, theRange, theFlags)))
			{
			numItems = CFArrayGetCount(cfArray);

			for (n = 0; n < numItems; n++)
				{
				foundRange = *((const NRange *) CFArrayGetValueAtIndex(cfArray, n));
				theResult.push_back(foundRange);
				}
			}
		}
	
	return(theResult);
*/
}





//============================================================================
//      NString::FindPattern : Find a pattern.
//----------------------------------------------------------------------------
NRangeList NString::FindPattern(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
{	int						n, errorPos, numMatches, matchStart, matchEnd;
    NString                 searchText, matchText;
	int						regFlags, regErr;
	std::vector<int>		theMatches;
	const char				*searchPtr;
	NRange					matchRange;
	NRangeList				theResult;
	const char				*errorMsg;
	pcre					*regExp;



	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());
	NN_ASSERT(theRange.GetLocation() >= 0 && theRange.GetNext() <= GetSize());
	NN_ASSERT(theFlags == kNStringNone || theFlags == kNStringNoCase);



	// Get the state we need
	numMatches = 0;
	regFlags   = PCRE_UTF8;
	
	if (theFlags & kNStringNoCase)
		regFlags |= PCRE_CASELESS;

	if (theRange.GetLocation() == 0)
		searchPtr  = GetUTF8();
	else
		{
		searchText = GetString(theRange);
		searchPtr  = searchText.GetUTF8();
		}



	// Compile the expression
	regExp = pcre_compile(theString.GetUTF8(), regFlags, &errorMsg, &errorPos, NULL);
	if (regExp == NULL)
		{
		NN_LOG("Unable to compile '%s': %s (%d)", theString.GetUTF8(), errorMsg, errorPos);
		return(theResult);
		}



	// Execute the expression
	regErr = pcre_fullinfo(regExp, NULL, PCRE_INFO_CAPTURECOUNT, &numMatches);
	NN_ASSERT(regErr == 0);

	if (regErr == 0)
		{
		theMatches.resize((numMatches+1) * 3);

		regErr = pcre_exec(regExp, NULL, searchPtr, strlen(searchPtr), 0, PCRE_NO_UTF8_CHECK, &theMatches[0], theMatches.size());
		NN_ASSERT(regErr == PCRE_ERROR_NOMATCH || regErr > 0);
		}



	// Collect the results
	//
	// Matches are returned as start/end offsets within the UTF8 search string.
	//
	// To convert these offsets to a character index, we must extract and measure
	// the relevant portions of the UTF8 string to handle multi-byte sequences in
	// the UTF8 string which correspond to a single character.
	if (regErr > 0)
		{
		for (n = 0; n <= numMatches; n++)
			{
			// Identify the range
            matchStart = theMatches[(n * 2) + 0];
            matchEnd   = theMatches[(n * 2) + 1];
			
			if (matchStart == -1)
				matchRange = kNRangeNone;
			else
				{
				matchText = NString(searchPtr, matchStart, kNStringEncodingUTF8);
				matchRange.SetLocation(theRange.GetLocation() + matchText.GetSize());

				matchText = NString(searchPtr + matchStart, matchEnd - matchStart, kNStringEncodingUTF8);
				matchRange.SetSize(matchText.GetSize());
				}



			// Collect the match
			theResult.push_back(matchRange);
			
			if (!doAll)
				break;
			}
		}



	// Clean up
	pcre_free(regExp);

	return(theResult);
}

