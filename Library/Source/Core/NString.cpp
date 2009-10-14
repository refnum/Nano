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


	// Initialize ourselves
	InitializeSelf(NULL);



	// Assign the type
	*this = NTextUtilities::FromOSType(theType);
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(CFStringRef cfString)
{


	// Initialize ourselves
	//
	// We will release the object when we initialize ourselves, so
	// we retain it first to avoid releasing our caller's reference.
	CFSafeRetain(cfString);
	
	InitializeSelf(cfString);
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const HFSUniStr255 &theString)
{


	// Initialize ourselves
	InitializeSelf(CFStringCreateWithCharacters(kCFAllocatorNano, theString.unicode, theString.length));
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const NData &theData, CFStringEncoding theEncoding)
{


	// Initialize ourselves
	InitializeSelf(CFStringCreateWithBytes(kCFAllocatorNano, theData.GetData(), theData.GetSize(), theEncoding, false));



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
	InitializeSelf(CFStringCreateWithBytes(kCFAllocatorNano, (const UInt8 *) theString, theLength, theEncoding, false));



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
	InitializeSelf(CFStringCreateWithCharacters(kCFAllocatorNano, theString, theLength));
}





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(void)
{


	// Initialize ourselves
	InitializeSelf(NULL);
}





//============================================================================
//		NString::~NString : Destructor.
//----------------------------------------------------------------------------
NString::~NString(void)
{
}





//============================================================================
//		NString::IsEmpty : Is the string empty?
//----------------------------------------------------------------------------
bool NString::IsEmpty(void) const
{


	// Check our size
	return(GetSize() == 0);
}





//============================================================================
//		NString::IsNotEmpty : Is the string not empty?
//----------------------------------------------------------------------------
bool NString::IsNotEmpty(void) const
{


	// Check our size
	return(GetSize() != 0);
}





//============================================================================
//		NString::Clear : Clear the string.
//----------------------------------------------------------------------------
void NString::Clear(void)
{


	// Clear the string
	CFStringDelete(*this, CFRangeMake(0, GetSize()));
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
//		NString::GetUTF8 : Get the string data.
//----------------------------------------------------------------------------
const char *NString::GetUTF8(void) const
{


	// Get the data
	return((const char *) GetData(kCFStringEncodingUTF8));
}





//============================================================================
//		NString::GetUTF16 : Get the string data.
//----------------------------------------------------------------------------
const UniChar *NString::GetUTF16(void) const
{


	// Get the data
	return((const UniChar *) GetData(kCFStringEncodingUTF16));
}





//============================================================================
//		NString::IsMatch : Does a pattern match a string?
//----------------------------------------------------------------------------
bool NString::IsMatch(const NString &thePattern, CFOptionFlags theFlags) const
{


	// Find the pattern
	return(!FindPattern(thePattern, theFlags).empty());
}





//============================================================================
//		NString::FindPattern : Find a pattern within the string.
//----------------------------------------------------------------------------
CFRangeList NString::FindPattern(const NString &thePattern, CFOptionFlags theFlags) const
{	int						n, errorPos, numMatches, matchStart, matchEnd;
	int						regFlags, regErr;
	std::vector<int>		theMatches;
	CFRangeList				theResult;
	const char				*errorMsg;
	pcre					*regExp;



	// Validate our parameters
	NN_ASSERT(thePattern.IsNotEmpty());
	NN_ASSERT(theFlags == 0 || theFlags == kCFCompareCaseInsensitive);

	(void) theFlags;
	


	// Get the state we need
	numMatches = 0;
	regFlags   = PCRE_UTF8;
	
	if (regFlags & kCFCompareCaseInsensitive)
		regFlags |= PCRE_CASELESS;



	// Compile the expression
	regExp = pcre_compile(thePattern.GetUTF8(), regFlags, &errorMsg, &errorPos, NULL);
	if (regExp == NULL)
		{
		NN_LOG("Unable to compile '%s': %s (%d)", thePattern.GetUTF8(), errorMsg, errorPos);
		return(theResult);
		}



	// Execute the expression
	regErr = pcre_fullinfo(regExp, NULL, PCRE_INFO_CAPTURECOUNT, &numMatches);
	NN_ASSERT(regErr == 0);

	if (regErr == 0)
		{
		theMatches.resize((numMatches+1) * 3);

		regErr = pcre_exec(regExp, NULL, GetUTF8(), GetSize(), 0, PCRE_NO_UTF8_CHECK, &theMatches[0], theMatches.size());
		NN_ASSERT(regErr == PCRE_ERROR_NOMATCH || regErr > 0);
		}



	// Collect the results
	if (regErr > 0)
		{
		for (n = 0; n <= numMatches; n++)
			{
			matchStart = theMatches[(n * 2) + 0];
			matchEnd   = theMatches[(n * 2) + 1];
			
			if (matchStart == -1)
				theResult.push_back(CFRangeMake(0, 0));
			else
				theResult.push_back(CFRangeMake(matchStart, matchEnd - matchStart));
			}
		}



	// Clean up
	pcre_free(regExp);

	return(theResult);
}





//============================================================================
//		NString::Find : Find a substring.
//----------------------------------------------------------------------------
CFRange NString::Find(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange) const
{	CFRange		findRange, foundRange;



	// Prepare the range
	findRange = theRange;
	if (findRange.length == kCFNotFound)
		findRange.length = GetSize() - findRange.location;



	// Find the substring
	if (!CFStringFindWithOptions(*this, theString, findRange, theFlags, &foundRange))
		foundRange = CFRangeMake(0, 0);
	
	return(foundRange);
}





//============================================================================
//		NString::FindAll : Find every instance of a substring.
//----------------------------------------------------------------------------
CFRangeList NString::FindAll(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange) const
{	const CFRange	*foundRange;
	UInt32			n, numItems;
	CFRange			findRange;
	CFRangeList		theResult;
	NCFObject		cfArray;



	// Prepare the range
	findRange = theRange;
	if (findRange.length == kCFNotFound)
		findRange.length = GetSize();



	// Find the substrings
	if (cfArray.Set(CFStringCreateArrayWithFindResults(kCFAllocatorNano, *this, theString, findRange, theFlags)))
		{
		numItems = CFArrayGetCount(cfArray);
		
		for (n = 0; n < numItems; n++)
			{
			foundRange = (const CFRange *) CFArrayGetValueAtIndex(cfArray, n);
			theResult.push_back(*foundRange);
			}
		}
	
	return(theResult);
}





//============================================================================
//		NString::Replace : Replace a substring.
//----------------------------------------------------------------------------
void NString::Replace(const CFRange &theRange,	const NString &replaceWith)
{


	// Replace the substring
	CFStringReplace(*this, theRange, replaceWith);
}





//============================================================================
//		NString::ReplaceAll : Replace every instance of a substring.
//----------------------------------------------------------------------------
void NString::ReplaceAll(const NString &theString, const NString &replaceWith)
{	CFRangeList						theList;
	CFRangeListReverseIterator		theIter;



	// Replace the substrings
	theList = FindAll(theString);
	
	for (theIter = theList.rbegin(); theIter != theList.rend(); theIter++)
		Replace(*theIter, replaceWith);
}





//============================================================================
//		NString::Compare : Compare two strings.
//----------------------------------------------------------------------------
CFComparisonResult NString::Compare(const NString &theString, CFOptionFlags theFlags) const
{


	// Compare the strings
	return(CFStringCompare(*this, theString, theFlags));
}





//============================================================================
//		NString::EqualTo : Compare two strings.
//----------------------------------------------------------------------------
bool NString::EqualTo(const NString &theString, bool ignoreCase) const
{


	// Compare the strings
	return(Compare(theString, ignoreCase ? kCFCompareCaseInsensitive : 0) == kCFCompareEqualTo);
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
//		NString::MakeUpper : Convert to upper case.
//----------------------------------------------------------------------------
void NString::MakeUpper(void)
{


	// Change the case
	CFStringUppercase(*this, NULL);
}





//============================================================================
//		NString::MakeLower : Convert to lower case.
//----------------------------------------------------------------------------
void NString::MakeLower(void)
{


	// Change the case
	CFStringLowercase(*this, NULL);
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
		   theRange.length == suffixLength);
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
	CFIndex		maxLength;
	CFRange		tmpRange;



	// Prepare the range
	maxLength = GetSize();
	tmpRange  = theRange;
	
	if (tmpRange.length == (CFIndex) kStringLength)
		tmpRange.length = maxLength;
	
	if ((tmpRange.location + tmpRange.length) > maxLength)
		tmpRange.length = maxLength - tmpRange.location;



	// Extract the substring
	theResult.Set(CFStringCreateWithSubstring(kCFAllocatorNano, *this, tmpRange));

	return(theResult);
}





//============================================================================
//		NString::Trim : Trim a string.
//----------------------------------------------------------------------------
void NString::Trim(const NString &theString)
{


	// Trim the string
	CFStringTrim(*this, theString);
}





//============================================================================
//		NString::TrimWhitespace : Trim whitespace.
//----------------------------------------------------------------------------
void NString::TrimWhitespace(void)
{


	// Trim the string
	CFStringTrimWhitespace(*this);
}





//============================================================================
//		NString::Split : Split a string.
//----------------------------------------------------------------------------
NStringList NString::Split(const NString &splitWith, bool splitExact) const
{	NString			tmpString, subString;
	UInt32			n, numItems;
	NStringList		theStrings;
	NArray			theArray;
	CFRange			theRange;



	// Validate our parameters
	NN_ASSERT(splitWith.IsNotEmpty());



	// Decide how we need to split
	if (splitWith.GetSize() == 1)
		splitExact = false;
	
	
	
	// Split with an exact match
	if (splitExact)
		{
		tmpString = *this;
	
		while (tmpString.IsNotEmpty())
			{
			theRange = tmpString.Find(splitWith);

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
	
	
	// Or split with any character
	else
		{
		if (theArray.Set(CFStringCreateArrayBySeparatingStrings(kCFAllocatorNano, *this, splitWith)))
			{
			numItems = theArray.GetSize();
			for (n = 0; n < numItems; n++)
				theStrings.push_back(theArray.GetValueString(n));
			}
		}
	
	return(theStrings);
}





//============================================================================
//		NString::Format : Printf into a string.
//----------------------------------------------------------------------------
void NString::Format(const char *formatString, ...)
{	NString		theFormat(formatString, kStringLength, kCFStringEncodingUTF8);
	va_list		argList;



	// Format the string
	va_start(argList, formatString);
	Format(formatString, argList);
	va_end(argList);
}





//============================================================================
//		NString::Format : Printf into a string.
//----------------------------------------------------------------------------
void NString::Format(const char *formatString, va_list argList)
{	NString		theFormat(formatString, kStringLength, kCFStringEncodingUTF8);



	// Format the string
	Set(CFStringCreateWithFormatAndArguments(kCFAllocatorNano, NULL, theFormat, argList));
}





//============================================================================
//		NString::== : Are two strings equal?
//----------------------------------------------------------------------------
const bool NString::operator == (const NString &theString) const
{


	// Compare the strings
	return(Compare(theString) == kCFCompareEqualTo);
}





//============================================================================
//		NString::!= : Are two strings not equal?
//----------------------------------------------------------------------------
const bool NString::operator != (const NString &theString) const
{


	// Compare the strings
	return(Compare(theString) != kCFCompareEqualTo);
}





//============================================================================
//		NString::< : Comparison operator.
//----------------------------------------------------------------------------
const bool NString::operator < (const NString &theString) const
{


	// Compare the strings
	return(Compare(theString) == kCFCompareLessThan);
}





//============================================================================
//		NString::+= : Append to a string.
//----------------------------------------------------------------------------
const NString& NString::operator += (const NString &theString)
{


	// Append the string
	CFStringAppend(*this, theString);
	
	return(*this);
}





//============================================================================
//		NString::+ : Append to a string.
//----------------------------------------------------------------------------
const NString NString::operator + (const NString &theString) const
{	NString		newString;



	// Append the string
	newString = *this;
	
	CFStringAppend(newString, theString);
	
	return(newString);
}





//============================================================================
//		NString::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NString::GetNullValue(void)
{


	// Get the value
	return(CFStringCreateMutable(kCFAllocatorNano, 0));
}





//============================================================================
//		NString::DoObjectChanging : Handle an object change.
//----------------------------------------------------------------------------
void NString::DoObjectChanging(void)
{


	// Release the data
	mData.Clear();
}





//============================================================================
//		NString::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NString::InitializeSelf(CFStringRef cfString)
{


	// Initialize ourselves
	SetMode(kCFDuplicate);
	
	Set(cfString);



	// Validate our state
	NN_ASSERT(IsValid());
}





//============================================================================
//		NString::GetData : Get the string data.
//----------------------------------------------------------------------------
const UInt8 *NString::GetData(CFStringEncoding theEncoding) const
{	UInt32		*nullPtr;
	UInt8		*thePtr;
	CFIndex		theSize;



	// Prepare the buffer
	//
	// The maximum amount of space we need for a trailing null 'byte' is 4
	// bytes (UTF32), and so we append a UInt32 to the buffer.
	CFStringGetBytes(*this, CFRangeMake(0, GetSize()), theEncoding, '?', false, NULL, 0, &theSize);

	if (!mData.SetSize(theSize + sizeof(UInt32)))
		return(NULL);



	// Convert the string
	thePtr	= mData.GetData();
	nullPtr = (UInt32 *) (&thePtr[theSize]);
	
	nullPtr[0] = 0;
	CFStringGetBytes(*this, CFRangeMake(0, GetSize()), theEncoding, '?', false, thePtr, theSize, &theSize);
	
	return(thePtr);
}






