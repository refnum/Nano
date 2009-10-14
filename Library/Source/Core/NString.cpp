/*	NAME:
		NString.cpp

	DESCRIPTION:
		Nano string object.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "pcre.h"

#include "NTextUtilities.h"
#include "NCFObject.h"
#include "NArray.h"
#include "NString.h"





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(OSType theType)
{


	// Assign the type
	*this = NTextUtilities::FromOSType(theType);
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const HFSUniStr255 &theString)
{


	// Initialize ourselves
	InitializeSelf(CFStringCreateWithCharacters(kCFAllocatorNano, theString.unicode, theString.length), true);
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const NData &theData, CFStringEncoding theEncoding)
{


	// Initialize ourselves
	InitializeSelf(CFStringCreateWithBytes(kCFAllocatorNano, theData.GetData(), theData.GetSize(), theEncoding, false), true);



	// Validate our state
	//
	// CFStringCreateWithBytes will fail if passed characters that are illegal
	// in the specified encoding, so we assert that a non-empty input succeeded.
	if (theData.IsNotEmpty())
		NN_ASSERT(IsNotEmpty());
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const char *theString, UInt32 theLength, CFStringEncoding theEncoding)
{


	// Calculate the length
	if (theLength == kStringLength)
		theLength = strlen(theString);



	// Initialize ourselves
	InitializeSelf(CFStringCreateWithBytes(kCFAllocatorNano, (const UInt8 *) theString, theLength, theEncoding, false), true);



	// Validate our state
	//
	// CFStringCreateWithBytes will fail if passed characters that are illegal
	// in the specified encoding, so we assert that a non-empty input succeeded.
	if (theLength > 0)
		NN_ASSERT(IsNotEmpty());
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const UniChar *theString, UInt32 theLength)
{


	// Calculate the length
	if (theLength == kStringLength)
		{
		theLength = 0;
		while (theString[theLength] != (UniChar) 0)
			theLength++;
		}



	// Initialize ourselves
	InitializeSelf(CFStringCreateWithCharacters(kCFAllocatorNano, theString, theLength), true);
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(CFStringRef cfString)
{


	// Initialize ourselves
	InitializeSelf(cfString, false);
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
UInt32 NString::GetSize(void) const
{


	// Get the size
	return(CFStringGetLength(*this));
}





//============================================================================
//		NString::GetHash : Get the object hash.
//----------------------------------------------------------------------------
CFHashCode NString::GetHash(void) const
{


	// Get the hash code
	//
	// Since strings are often immutable, a hash code provides a cheap way to
	// test for inequality when strings are used as keys for a std::map.
	//
	// To improve the performance of StringMapCompare, NString caches its hash
	// code to avoid unnecessary recalculations.
	if (mHashCode == kCFHashCodeNull)
		mHashCode = NCFObject::GetHash();
	
	return(mHashCode);
}





//============================================================================
//		NString::GetUTF8 : Get the string data.
//----------------------------------------------------------------------------
const char *NString::GetUTF8(void) const
{	CFIndex		theSize;



	// Get the data
	return((const char *) GetData(kCFStringEncodingUTF8, theSize));
}





//============================================================================
//		NString::GetUTF16 : Get the string data.
//----------------------------------------------------------------------------
const UniChar *NString::GetUTF16(void) const
{	CFIndex		theSize;



	// Get the data
	return((const UniChar *) GetData(kCFStringEncodingUTF16, theSize));
}





//============================================================================
//		NString::GetData : Get the string data.
//----------------------------------------------------------------------------
const UInt8 *NString::GetData(CFStringEncoding theEncoding, CFIndex &theSize) const
{	UInt32		*nullPtr;
	UInt8		*thePtr;



	// Get the size
	theSize = 0;

	CFStringGetBytes(*this, NormalizeRange(kCFRangeAll), theEncoding, '?', false, NULL, 0, &theSize);



	// Get the data
	//
	// If we can, return a direct pointer to the internal string data.
	thePtr = (UInt8 *) CFStringGetCStringPtr(*this, theEncoding);
	if (thePtr != NULL)
		return(thePtr);



	// Create our own buffer
	//
	// Since CFStringGetCStringPtr can always return NULL, we may have to use our
	// own buffer. This buffer is valid until the string is modified.
	//
	// The maximum amount of space we need for a trailing null 'byte' is 4 bytes
	// for UTF32, so we append a UInt32 to the calculated buffer size.
	mData.resize(theSize + sizeof(UInt32));
	if (mData.empty())
		{
		theSize = 0;
		return(NULL);
		}



	// Get the data
    thePtr  = &mData[0];
	nullPtr = (UInt32 *) (&thePtr[theSize]);
	
	nullPtr[0] = 0;
	CFStringGetBytes(*this, NormalizeRange(kCFRangeAll), theEncoding, '?', false, thePtr, theSize, &theSize);
	
	return(thePtr);
}





//============================================================================
//		NString::Find : Find a substring.
//----------------------------------------------------------------------------
CFRange NString::Find(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange) const
{	CFRangeList		theResults;



	// Find the string
	theResults = FindMatches(theString, theFlags, theRange, false);

	return(theResults.empty() ? kCFRangeNone : theResults[0]);
}





//============================================================================
//		NString::FindAll : Find every instance of a substring.
//----------------------------------------------------------------------------
CFRangeList NString::FindAll(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange) const
{	CFRangeList		theResults;



	// Find the strings
	theResults = FindMatches(theString, theFlags, theRange, true);

	return(theResults);
}





//============================================================================
//		NString::Replace : Replace a substring.
//----------------------------------------------------------------------------
void NString::Replace(const CFRange &theRange, const NString &replaceWith)
{


	// Replace the substring
	if (MakeMutable())
		{
		CFStringReplace(*this, NormalizeRange(theRange), replaceWith);
		ValueChanged();
		}
}





//============================================================================
//		NString::Replace : Replace a substring.
//----------------------------------------------------------------------------
bool NString::Replace(const NString &theString, const NString &replaceWith, CFOptionFlags theFlags, const CFRange &theRange)
{	CFRange		foundRange;



	// Replace the substring
	if (MakeMutable())
		{
		foundRange = Find(theString, theFlags, theRange);
		if (foundRange != kCFRangeNone)
			{
			Replace(foundRange, replaceWith);
			return(true);
			}
		}
	
	return(false);
}





//============================================================================
//		NString::ReplaceAll : Replace every instance of a substring.
//----------------------------------------------------------------------------
UInt32 NString::ReplaceAll(const NString &theString, const NString &replaceWith, CFOptionFlags theFlags, const CFRange &theRange)
{	CFRangeList						foundRanges;
	CFRangeListReverseIterator		theIter;



	// Replace the substring
	if (MakeMutable())
		{
		foundRanges = FindAll(theString, theFlags, theRange);

		for (theIter = foundRanges.rbegin(); theIter != foundRanges.rend(); theIter++)
			Replace(*theIter, replaceWith);
		}
	
	return(foundRanges.size());
}





//============================================================================
//		NString::StartsWith : Does the string start with a string?
//----------------------------------------------------------------------------
bool NString::StartsWith(const NString &theString, CFOptionFlags theFlags) const
{	CFRange		theRange;



	// Find the string
	theRange = Find(theString, theFlags);

	return(theRange.location == 0 &&
		   theRange.length	 == (CFIndex) theString.GetSize());
}





//============================================================================
//		NString::EndsWith : Does the string end with a string?
//----------------------------------------------------------------------------
bool NString::EndsWith(const NString &theString, CFOptionFlags theFlags) const
{	CFIndex		theLength, suffixLength;
	CFRange		theRange;



	// Get the state we need
	theLength	 = GetSize();
	suffixLength = theString.GetSize();



	// Find the string
	theRange = Find(theString, theFlags | kCFCompareBackwards);

	return(theRange.location == (theLength - suffixLength) &&
		   theRange.length   == suffixLength);
}





//============================================================================
//		NString::Contains : Does the string contain a string?
//----------------------------------------------------------------------------
bool NString::Contains(const NString &theString, CFOptionFlags theFlags) const
{	CFRange		theRange;



	// Find the string
	theRange = Find(theString, theFlags);

	return(theRange.length != 0);
}





//============================================================================
//		NString::CompareTo : Compare two strings.
//----------------------------------------------------------------------------
CFComparisonResult NString::CompareTo(const NString &theString, CFOptionFlags theFlags) const
{


	// Validate our parameters
	NN_ASSERT((theFlags & kCFCompareMaskNano) == 0);



	// Compare the strings
	return(CFStringCompare(*this, theString, theFlags));
}





//============================================================================
//		NString::EqualTo : Compare two strings.
//----------------------------------------------------------------------------
bool NString::EqualTo(const NString &theString, CFOptionFlags theFlags) const
{


	// Compare the strings
	//
	// Exact comparisons can use a mis-matched hash code to as a cheap test.
	if (theFlags == 0 && GetHash() != theString.GetHash())
		return(false);

	return(CompareTo(theString, theFlags) == kCFCompareEqualTo);
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
{	NCFObject	cfLocale;



    // Change the case
	if (MakeMutable() && cfLocale.Set(CFLocaleCopyCurrent()))
		{
		CFStringUppercase(*this, cfLocale);
		ValueChanged();
		}
}





//============================================================================
//		NString::MakeLower : Convert to lower case.
//----------------------------------------------------------------------------
void NString::MakeLower(void)
{	NCFObject	cfLocale;



    // Change the case
	if (MakeMutable() && cfLocale.Set(CFLocaleCopyCurrent()))
		{
		CFStringLowercase(*this, cfLocale);
		ValueChanged();
		}
}





//============================================================================
//      NString::MakeCapitals : Convert to capitalized form.
//----------------------------------------------------------------------------
void NString::MakeCapitals(bool eachWord)
{	NCFObject		cfLocale;
	CFRange			theRange;
	NString			theText;



    // Change the case
    if (MakeMutable() && cfLocale.Set(CFLocaleCopyCurrent()))
		{
		// Capitalize each word
		if (eachWord)
			CFStringCapitalize(*this, cfLocale);
		
		
		// Or each sentence
		else
			{
			// Start with lower case
			MakeLower();
			
			
			// Fix the first word
			theRange = Find("^(\\s*[a-z])", kCFComparePattern);
			if (theRange != kCFRangeNone)
				{
				theText = GetString(theRange).GetUpper();
				Replace(theRange, theText);
				}
			
			
			// Fix each subsequent sentance
			theRange = kCFRangeAll;
			do
				{
				theRange = Find("([\\.\\?\\!]\\s*[a-z])", kCFComparePattern, theRange);
				if (theRange != kCFRangeNone)
					{
					theText = GetString(theRange).GetUpper();
					Replace(theRange, theText);
					
					theRange.location += theRange.length;
					theRange.length    = kCFNotFound;
					}
				}
			while (theRange != kCFRangeNone);
			}
		
		
		// Update our state
		ValueChanged();
		}
}





//============================================================================
//		NString::GetLeft : Get a left-anchored substring.
//----------------------------------------------------------------------------
NString NString::GetLeft(UInt32 theLength) const
{


	// Check for overflow
	if (theLength >= GetSize())
		return(*this);
	
	
	
	// Get the substring
	return(GetString(0, theLength));
}





//============================================================================
//		NString::GetRight : Get a right-anchored substring.
//----------------------------------------------------------------------------
NString NString::GetRight(UInt32 theLength) const
{


	// Check for overflow
	if (theLength >= GetSize())
		return(*this);
	
	
	
	// Get the substring
	return(GetString(GetSize() - theLength, theLength));
}





//============================================================================
//		NString::GetString : Get a substring.
//----------------------------------------------------------------------------
NString NString::GetString(UInt32 theOffset, UInt32 theLength) const
{


	// Get the substring
	return(GetString(CFRangeMake(theOffset, theLength)));
}





//============================================================================
//		NString::GetString : Get a substring.
//----------------------------------------------------------------------------
NString NString::GetString(const CFRange &theRange) const
{	NString		theResult;



	// Get the substring
	if (theRange.location < (CFIndex) GetSize())
		theResult.Set(CFStringCreateWithSubstring(kCFAllocatorNano, *this, NormalizeRange(theRange)));

	return(theResult);
}





//============================================================================
//		NString::Split : Split a string.
//----------------------------------------------------------------------------
NStringList NString::Split(const NString &theString, bool isExact) const
{	NString					tmpString, subString, matchString;
	UInt32					n, numItems;
	NStringList				theStrings;
	NArray					theArray;
	CFRange					theRange;
	NStringListIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());



	// Split with an exact match
	if (isExact)
		{
		if (theArray.Set(CFStringCreateArrayBySeparatingStrings(kCFAllocatorNano, *this, theString)))
			{
			numItems = theArray.GetSize();
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

			if (theRange.length == 0)
				{
				subString = tmpString;
				tmpString.Clear();
				}
			else
				{
				subString = tmpString.GetLeft(	theRange.location);
				tmpString = tmpString.GetString(theRange.location + theRange.length);
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
}





//============================================================================
//		NString::Trim : Trim a string at both ends.
//----------------------------------------------------------------------------
void NString::Trim(const NString &theString, bool isExact)
{


	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());



	// Trim the string
	TrimLeft( theString, isExact);
	TrimRight(theString, isExact);
}





//============================================================================
//		NString::TrimLeft : Trim a string on the left.
//----------------------------------------------------------------------------
void NString::TrimLeft(const NString &theString, bool isExact)
{	NString		thePattern;



	// Trim the string
	if (MakeMutable())
		{
		thePattern.Format(isExact ? "^\\Q%@" : "^[\\Q%@\\E]+", theString);
		Replace(thePattern, "", kCFComparePattern);
		}
}





//============================================================================
//		NString::TrimRight : Trim a string on the right.
//----------------------------------------------------------------------------
void NString::TrimRight(const NString &theString, bool isExact)
{	NString		thePattern;



	// Trim the string
	if (MakeMutable())
		{
		thePattern.Format(isExact ? "\\Q%@$" : "[\\Q%@\\E]+$", theString);
		Replace(thePattern, "", kCFComparePattern);
		}
}





//============================================================================
//		NString::TrimLeft : Trim a string on the left.
//----------------------------------------------------------------------------
void NString::TrimLeft(UInt32 theLength)
{	CFRange		theRange;



	// Trim the string
	if (MakeMutable())
		{
		theRange = CFRangeMake(0, theLength);

		CFStringDelete(*this, NormalizeRange(theRange));
		ValueChanged();
		}
}





//============================================================================
//		NString::TrimRight : Trim a string on the right.
//----------------------------------------------------------------------------
void NString::TrimRight(UInt32 theLength)
{	CFRange		theRange;



	// Trim the string
	if (MakeMutable())
		{
		theRange = CFRangeMake(GetSize() - theLength, theLength);

		CFStringDelete(*this, NormalizeRange(theRange));
		ValueChanged();
		}
}





//============================================================================
//		NString::Format : Printf into a string.
//----------------------------------------------------------------------------
void NString::Format(const NString &formatString, va_list argList)
{


	// Format the string
	Set(CFStringCreateWithFormatAndArguments(kCFAllocatorNano, NULL, formatString, argList));
}





//============================================================================
//		NString::+= : Append to a string.
//----------------------------------------------------------------------------
const NString& NString::operator += (const NString &theString)
{


	// Append the string
	if (MakeMutable())
		{
		CFStringAppend(*this, theString);
		ValueChanged();
		}
	
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
//		NString::== : Equality operator
//----------------------------------------------------------------------------
bool NString::operator == (const char *theString) const
{


	// Compare the objects
	//
	// Although NComparable provides == and != operators, we must provide char*
	// versions to support comparison wth string literals:
	//
	//		if (theString == "something")
	//			; // Do something
	//
	// Otherwise, since NComparable does not provide a char* constructor, we will
	// be cast to a CFTypeRef (void*) for a simple pointer equality comparison.
	//
	// If we provide a char* version then we must also provide an NString version,
	// to ensure that "theString == otherString" is not ambiguous.
	//
	// We implement == and != operators in terms of EqualTo, since this can perform
	// a faster hash-based comparison than the general NComparable::Compare.
	return(EqualTo(theString, 0));
}





//============================================================================
//		NString::== : Equality operator
//----------------------------------------------------------------------------
bool NString::operator == (const NString &theString) const
{


	// Compare the objects
	//
	// See == operator for background.
	return(EqualTo(theString, 0));
}





//============================================================================
//		NString::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NString::operator != (const char *theString) const
{


	// Compare the objects
	//
	// See == operator for background.
	return(!EqualTo(theString, 0));
}





//============================================================================
//		NString::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NString::operator != (const NString &theString) const
{


	// Compare the objects
	//
	// See == operator for background.
	return(!EqualTo(theString, 0));
}





//============================================================================
//		NString::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NString::GetNullValue(void)
{	static CFStringRef		sNullValue = CFStringCreateWithCString(kCFAllocatorNano, "", kCFStringEncodingUTF8);



	// Get the value
	return(sNullValue);
}





//============================================================================
//		NString::DoObjectChanging : Handle an object change.
//----------------------------------------------------------------------------
void NString::DoObjectChanging(void)
{


	// Update our state
	ValueChanged();
}





//============================================================================
//		NString::Compare : Compare two objects.
//----------------------------------------------------------------------------
CFComparisonResult NString::Compare(const NComparable &theObject) const
{	const NString	*theString = dynamic_cast<const NString*>(&theObject);



	// Validate our parameters
	NN_ASSERT(theString != NULL);



	// Compare the strings
	return(CFStringCompare(*this, *theString, 0));
}





//============================================================================
//		NString::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NString::InitializeSelf(CFStringRef cfString, bool takeOwnership)
{


	// Initialize ourselves
	mHashCode = kCFHashCodeNull;



	// Assign the value
	Set(cfString, takeOwnership);
	
	if (cfString != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		NString::FindMatches : Find a string.
//----------------------------------------------------------------------------
CFRangeList NString::FindMatches(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange, bool doAll) const
{	CFRangeList		theResults;
	CFRange			findRange;
	bool			isPattern;



	// Check our state
	if (IsEmpty() || theString.IsEmpty())
		return(theResults);



	// Get the state we need
	findRange = NormalizeRange(theRange);
	theFlags  = NormalizeFlags(theFlags, isPattern);



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
CFRangeList NString::FindString(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange, bool doAll) const
{	UInt32			n, numItems;
	CFRange			foundRange;
	CFRangeList		theResult;
	NCFObject		cfArray;



	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.length) <= (CFIndex) GetSize());
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
				foundRange = *((const CFRange *) CFArrayGetValueAtIndex(cfArray, n));
				theResult.push_back(foundRange);
				}
			}
		}
	
	return(theResult);
}





//============================================================================
//      NString::FindPattern : Find a pattern.
//----------------------------------------------------------------------------
CFRangeList NString::FindPattern(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange, bool doAll) const
{	int						n, errorPos, numMatches, matchStart, matchEnd;
    NString                 searchText, matchText;
	int						regFlags, regErr;
	std::vector<int>		theMatches;
	const char				*searchPtr;
	CFRange					matchRange;
	CFRangeList				theResult;
	const char				*errorMsg;
	pcre					*regExp;



	// Validate our parameters
	NN_ASSERT(theString.IsNotEmpty());
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.length) <= (CFIndex) GetSize());
	NN_ASSERT(theFlags == 0 || theFlags == kCFCompareCaseInsensitive);



	// Get the state we need
	numMatches = 0;
	regFlags   = PCRE_UTF8;
	
	if (theFlags & kCFCompareCaseInsensitive)
		regFlags |= PCRE_CASELESS;

	if (theRange.location == 0)
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
				matchRange = kCFRangeNone;
			else
				{
				matchText           = NString(searchPtr, matchStart, kCFStringEncodingUTF8);
				matchRange.location = theRange.location + matchText.GetSize();

				matchText         = NString(searchPtr + matchStart, matchEnd - matchStart, kCFStringEncodingUTF8);
				matchRange.length = matchText.GetSize();
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





//============================================================================
//		NString::NormalizeRange : Normalize a CFRange.
//----------------------------------------------------------------------------
CFRange NString::NormalizeRange(const CFRange &theRange) const
{	CFRange		theResult;
	CFIndex		maxLength;



	// Normalize the range
	maxLength = GetSize();

	theResult.location = theRange.location;
    theResult.length   = (theRange.length == (CFIndex) kStringLength) ? maxLength : theRange.length;
	
	if ((theResult.location + theResult.length) > maxLength)
		theResult.length = maxLength - theResult.location;
	
	return(theResult);
}





//============================================================================
//		NString::NormalizeFlags : Normalize a CFOptionFlags.
//----------------------------------------------------------------------------
CFOptionFlags NString::NormalizeFlags(CFOptionFlags theFlags, bool &isPattern) const
{


	// Normalize the flags
	isPattern  = ((theFlags & kCFComparePattern) == kCFComparePattern);
	theFlags  &= ~kCFCompareMaskNano;

	return(theFlags);
}





//============================================================================
//      NString::ValueChanged : Our value has been changed.
//----------------------------------------------------------------------------
void NString::ValueChanged(void)
{


	// Reset our state
	//
	// State that depends on our value needs to be reset whenever it changes.
	//
	// To help expose stale pointers returned through GetData(), we scrub the
	// buffer in debug builds (vs just freeing the memory).
	mHashCode = kCFHashCodeNull;

#if NN_DEBUG
	if (!mData.empty())
		memset(&mData[0], 'X', mData.size());
#else
	mData.clear();
#endif
}





//============================================================================
//      NString::FormatArgs : Printf into a string.
//----------------------------------------------------------------------------
void NString::FormatArgs(const NString &formatString, ...)
{   va_list     argList;



    // Format the string
    va_start(argList, formatString);
    Format(formatString, argList);
    va_end(argList);
}





