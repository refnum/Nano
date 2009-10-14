/*	NAME:
		NString.cpp

	DESCRIPTION:
		Nano string object.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NString.h"





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
	InitializeSelf(CFStringCreateWithCharacters(NULL, theString.unicode, theString.length));
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
	InitializeSelf(CFStringCreateWithBytes(NULL, (const UInt8 *) theString, theLength, theEncoding, false));



	// Validate our state
	//
	// We assert our size to ensure that we catch situations where invalid data
	// (e.g., in the wrong endian-ness, or containing illegal characters) for
	// the encoding was passed to us.
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
	InitializeSelf(CFStringCreateWithCharacters(NULL, theString, theLength));
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
//		NString::Find : Find a substring.
//----------------------------------------------------------------------------
CFRange NString::Find(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange) const
{	CFRange		findRange, foundRange;



	// Prepare the range
	findRange = theRange;
	if (findRange.length == 0)
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
	if (findRange.length == 0)
		findRange.length = GetSize();



	// Find the substrings
	if (cfArray.Set(CFStringCreateArrayWithFindResults(NULL, *this, theString, findRange, theFlags)))
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
void NString::Replace(const CFRange &theRange,  const NString &replaceWith)
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
		   theRange.length   == (CFIndex) theString.GetSize());
}





//============================================================================
//		NString::EndsWith : Does the string end with a string?
//----------------------------------------------------------------------------
bool NString::EndsWith(const NString &theString, CFOptionFlags theFlags) const
{	CFIndex		theLength, otherLength;
	CFRange		theRange;



	// Get the state we need
	theLength   = GetSize();
	otherLength = theString.GetSize();



	// Find the string
	theRange = Find(theString, theFlags | kCFCompareBackwards);

	return(theRange.location == theLength - otherLength &&
		   theRange.length   == theLength);
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
//		NString::GetString : Get a substring.
//----------------------------------------------------------------------------
NString NString::GetString(UInt32 theOffset, UInt32 theLength) const
{	NString		theResult;
	CFIndex		maxLength;
	CFRange		theRange;



	// Prepare the range
	maxLength = GetSize();
	
	theRange.location = theOffset;
	theRange.length   = (theLength == kStringLength ? maxLength : theLength);
	
	if ((theRange.location + theRange.length) > maxLength)
		theRange.length = maxLength - theRange.location;



	// Extract the sub-string
	theResult.Set(CFStringCreateWithSubstring(NULL, *this, theRange));

	return(theResult);
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
//		NString::TrimString : Trim a string.
//----------------------------------------------------------------------------
void NString::TrimString(const NString &theString)
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
	Set(CFStringCreateWithFormatAndArguments(NULL, NULL, theFormat, argList));
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
	return(CFStringCreateMutable(NULL, 0));
}





//============================================================================
//		NString::DoObjectChanged : Handle an object change.
//----------------------------------------------------------------------------
void NString::DoObjectChanged(void)
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
	thePtr  = mData.GetData();
	nullPtr = (UInt32 *) (&thePtr[theSize]);
	
	nullPtr[0] = 0;
	CFStringGetBytes(*this, CFRangeMake(0, GetSize()), theEncoding, '?', false, thePtr, theSize, &theSize);
	
	return(thePtr);
}

