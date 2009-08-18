/*	NAME:
		NString.cpp

	DESCRIPTION:
		Strings are stored as NULL-terminated UTF8 or UTF16 data, without any
		leading BOM.

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

#include "NTextUtilities.h"
#include "NSTLUtilities.h"
#include "NEncoder.h"
#include "NString.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NIndex  kNStringLength										= kNIndexNone;
const NString kNStringEmpty											= "";
const NString kNStringSpace											= " ";
const NString kNStringNewline										= "\n";
const NString kNStringWhitespace									= "\\s";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
DEFINE_NENCODABLE(NString);





//============================================================================
//		NString::NString : Constructor.
//----------------------------------------------------------------------------
NString::NString(const void *thePtr, NIndex numBytes, NStringEncoding theEncoding)
{	const char		*charPtr;



	// Get the state we need
	charPtr = (const char *) thePtr;
	if (charPtr == NULL)
		numBytes = 0;

	if (numBytes == kNStringLength)
		{
		NN_ASSERT(theEncoding == kNStringEncodingUTF8);
		numBytes = (NIndex) strlen(charPtr);
		}



	// Initialize ourselves
	if (numBytes != 0)
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
NString::NString(UTF8Char theChar)
{


	// Initialize ourselves
	SetData(NData(1, &theChar, kNStringEncodingUTF8);
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
	const NData				*theData;



	// Get the state we need
	//
	// Strings that require encoding conversion are converted via a per-object
	// buffer, allowing the pointer to persist until we are modified.
	theValue = GetImmutable();
	theData  = &theValue->theData;

	if (theValue->theEncoding != kNStringEncodingUTF8)
		{
		mData   = GetData(kNStringEncodingUTF8, true);
		theData = &mData;
		}



	// Get the string
	theText = (const char *) theData->GetData();

	return(theText);
}





//============================================================================
//		NString::GetUTF16 : Get the string.
//----------------------------------------------------------------------------
const UTF16Char *NString::GetUTF16(void) const
{	const NStringValue		*theValue;
	const UTF16Char			*theText;
	const NData				*theData;



	// Get the state we need
	//
	// Strings that require encoding conversion are converted via a per-object
	// buffer, allowing the pointer to persist until we are modified.
	theValue = GetImmutable();
	theData  = &theValue->theData;

	if (theValue->theEncoding != kNStringEncodingUTF16)
		{
		mData   = GetData(kNStringEncodingUTF16, kNStringNullTerminate);
		theData = &mData;
		}



	// Get the string
	theText = (const UTF16Char *) theData->GetData();

	return(theText);
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
	theValue = GetImmutable();
	theErr   = theEncoder.Convert(theValue->theData, theData, theValue->theEncoding, theEncoding);

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
	theValue     = GetMutable();
	bestEncoding = GetBestEncoding(theData, theEncoding);



	// Convert the data
	theErr = theEncoder.Convert(theData, bestData, theEncoding, bestEncoding);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != kNoErr)
		return(theErr);



	// Update our state
	theEncoder.AddTerminator(bestData, bestEncoding);

	theValue->theEncoding = bestEncoding;
	theValue->theData     = bestData;

	ValueChanged(theValue);
	
	return(kNoErr);
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
	if (theRange.GetFirst() != 0)
		thePrefix = GetString(NRange(0, theRange.GetFirst()));

	if (theRange.GetNext() < GetSize())
		theSuffix = GetString(NRange(theRange.GetNext(), kNStringLength));



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
NIndex NString::ReplaceAll(const NString &theString, const NString &replaceWith, NStringFlags theFlags, const NRange &theRange)
{	NRangeList						foundRanges;
	NRangeListReverseIterator		theIter;



	// Replace the string
	foundRanges = FindAll(theString, theFlags, theRange);

	for (theIter = foundRanges.rbegin(); theIter != foundRanges.rend(); theIter++)
		Replace(*theIter, replaceWith);
	
	return((NIndex) foundRanges.size());
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
		isMatch  = !theRange.IsEmpty();
		}
	
	return(isMatch);
}





//============================================================================
//		NString::EndsWith : Does the string end with a string?
//----------------------------------------------------------------------------
bool NString::EndsWith(const NString &theString, NStringFlags theFlags) const
{	NString			matchString;
	NRange			theRange;
	NIndex			theSize;
	bool			isMatch;



	// Simple test
	if (theFlags == kNStringNone)
		{
		theSize = theString.GetSize();
		isMatch = (theSize <= GetSize());

		if (isMatch)
			isMatch = (memcmp(GetUTF8() + GetSize() - theSize, theString.GetUTF8(), theSize) == 0);
		}


	// General test
	else
		{
		if (theFlags & kNStringPattern)
			matchString.Format("%@$",       theString);
		else
			matchString.Format("\\Q%@\\E$", theString);

		theRange = Find(matchString, theFlags | kNStringPattern);
		isMatch  = !theRange.IsEmpty();
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
	NUnicodeParser			parserA, parserB;
	UInt64					numberA, numberB;
	UTF32Char				charA, charB;
	NComparison				theResult;



	// Check for identity
	if (GetImmutable() == theString.GetImmutable())
		return(kNCompareEqualTo);



	// Get the state we need
	parserA =           GetParser();
	parserB = theString.GetParser();
	
	sizeA = parserA.GetSize();
	sizeB = parserB.GetSize();

	ignoreCase = ((theFlags & kNStringNoCase)  != kNStringNone);
	isNumeric  = ((theFlags & kNStringNumeric) != kNStringNone);



	// Compare the strings
	//
	// Should probably use ICU for standard Unicode collation.
	theResult = GetComparison(sizeA, sizeB);
	indexA    = 0;
	indexB    = 0;

	while (indexA < sizeA && indexB < sizeB)
		{
		// Get the characters
		charA = parserA.GetChar(indexA, ignoreCase);
		charB = parserB.GetChar(indexB, ignoreCase);
		
		indexA++;
		indexB++;



		// Compare numerically
		if (isNumeric && parserA.IsDigit(charA) && parserB.IsDigit(charB))
			{
			// Collect the values
			numberA = 0;
			numberB = 0;
			
			do
				{
				NN_ASSERT(numberA <= (kUInt64Max/10));
				numberA = (numberA * 10) + (charA - (UTF32Char) '0');
				charA   = parserA.GetChar(indexA);
				indexA++;
				}
			while (parserA.IsDigit(charA) && indexA < sizeA);

			do
				{
				NN_ASSERT(numberB <= (kUInt64Max/10));
				numberB = (numberB * 10) + (charB - (UTF32Char) '0');
				charB   = parserB.GetChar(indexB);
				indexB++;
				}
			while (parserB.IsDigit(charB) && indexB < sizeB);


			// Compare by value
			if (numberA != numberB)
				{
				theResult = GetComparison(numberA, numberB);
				break;
				}
			
			if (indexA == sizeA || indexB == sizeB)
				break;
			}



		// Compare alphabetically
		if (charA != charB)
			{
			theResult = GetComparison(charA, charB);
			break;
			}
		}
	
	return(theResult);
}





//============================================================================
//		NString::EqualTo : Compare two strings.
//----------------------------------------------------------------------------
bool NString::EqualTo(const NString &theString, NStringFlags theFlags) const
{	bool	areEqual;



	// Check for equality
	//
	// Exact comparisons can use a mis-matched hash code as a cheap test.
	if (!(theFlags & kNStringNoCase) && GetHash() != theString.GetHash())
		areEqual = false;
	else
		areEqual = (Compare(theString, theFlags) == kNCompareEqualTo);
	
	return(areEqual);
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
NString NString::GetCapitals(NStringCapitalization theStyle) const
{	NString		theString;



	// Get the string
	theString = *this;
	theString.MakeCapitals(theStyle);
    
	return(theString);
}





//============================================================================
//		NString::MakeUpper : Convert to upper case.
//----------------------------------------------------------------------------
void NString::MakeUpper(void)
{


	// Update the string
	CapitalizeCharacters(true);
}





//============================================================================
//		NString::MakeLower : Convert to lower case.
//----------------------------------------------------------------------------
void NString::MakeLower(void)
{


	// Update the string
	CapitalizeCharacters(false);
}





//============================================================================
//      NString::MakeCapitals : Convert to capitalized form.
//----------------------------------------------------------------------------
void NString::MakeCapitals(NStringCapitalization theStyle)
{


	// Update the string
	switch (theStyle) {
		case kNCapitalizeWords:
			CapitalizeWords();
			break;
		
		case kNCapitalizeSentences:
			CapitalizeSentences();
			break;
		
		default:
			NN_LOG("Unknown capitalization style: %d", theStyle);
			break;
		}
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
{	NIndex					offsetFirst, offsetLast;
	NRange					subRange, byteRange;
	const NStringValue		*theValue;
	NString					theResult;
	NRangeList				theRanges;
	NData					theData;



	// Get the state we need
	theValue = GetImmutable();
	subRange = theRange.GetNormalized(GetSize());



	// Check the size
	if (subRange.IsEmpty())
		return(theResult);



	// Identify the bytes to extract
	theRanges = GetParser().GetRanges();

	NN_ASSERT(subRange.GetFirst() < (NIndex) theRanges.size());
	NN_ASSERT(subRange.GetLast()  < (NIndex) theRanges.size());

	offsetFirst = theRanges[subRange.GetFirst()].GetFirst();
	offsetLast  = theRanges[subRange.GetLast()].GetLast();
	
	byteRange.SetLocation(offsetFirst);
	byteRange.SetSize(    offsetLast - offsetFirst + 1);



	// Extract the string
	theData = theValue->theData.GetData(byteRange);
	theResult.SetData(theData, theValue->theEncoding);

	return(theResult);
}





//============================================================================
//		NString::Split : Split a string.
//----------------------------------------------------------------------------
NStringList NString::Split(const NString &theString, NStringFlags theFlags) const
{	NString						subString, matchString;
	NRange						theRange, subRange;
	NRangeList					theMatches;
	NIndex						offsetPrev;
	NRangeListConstIterator		iterMatch;
	NStringList					theResult;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Get the state we need
	matchString = GetWhitespacePattern(theString, theFlags);
	offsetPrev  = 0;



	// Find the split points
	//
	// If we find anything, a final range is added to ensure we include
	// any text beyond the last split point into the result.
	theMatches = FindAll(matchString, theFlags);

	if (!theMatches.empty())
		theMatches.push_back(NRange(GetSize(), 1));



	// Split the string
	for (iterMatch = theMatches.begin(); iterMatch != theMatches.end(); iterMatch++)
		{
		theRange = *iterMatch;
		
		if (theRange.GetLocation() > offsetPrev)
			{
			subRange  = NRange(offsetPrev, theRange.GetLocation() - offsetPrev);
			subString = GetString(subRange);

			NN_ASSERT(!subString.IsEmpty());
			theResult.push_back(subString);
			}
		
		offsetPrev = theRange.GetNext();
		}
	
	return(theResult);
}





//============================================================================
//		NString::Join : Join strings.
//----------------------------------------------------------------------------
NString NString::Join(const NStringList &theStrings, const NString &joinWith, bool skipEmpty)
{	NString						theResult;
	NStringListConstIterator	theIter;



	// Combine the strings
	for (theIter = theStrings.begin(); theIter != theStrings.end(); theIter++)
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
{	NString		trimString;



	// Get the state we need
	trimString = GetWhitespacePattern(theString, theFlags);

	if (theFlags & kNStringPattern)
		trimString.Format("^%@",       trimString);
	else
		trimString.Format("^\\Q%@\\E", trimString);



	// Trim the string
	Replace(trimString, "", theFlags | kNStringPattern);
}





//============================================================================
//		NString::TrimRight : Trim a string on the right.
//----------------------------------------------------------------------------
void NString::TrimRight(const NString &theString, NStringFlags theFlags)
{	NString		trimString;



	// Get the state we need
	trimString = GetWhitespacePattern(theString, theFlags);

	if (theFlags & kNStringPattern)
		trimString.Format("%@$",       trimString);
	else
		trimString.Format("\\Q%@\\E$", trimString);



	// Trim the string
	Replace(trimString, "", theFlags | kNStringPattern);
}





//============================================================================
//		NString::Trim : Trim a string at both ends.
//----------------------------------------------------------------------------
void NString::Trim(const NString &theString, NStringFlags theFlags)
{


	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());



	// Trim the string
	TrimLeft( theString, theFlags);
	TrimRight(theString, theFlags);
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
{	NIndex				offsetFirst, offsetLast;
	NRange				trimRange, byteRange;
	NStringValue		*theValue;
	NString				theResult;
	NRangeList			theRanges;
	NData				theData;



	// Get the state we need
	theValue  = GetMutable();
	theRanges = GetParser().GetRanges();
	trimRange = theRange.GetNormalized(GetSize());



	// Identify the bytes to remove
	NN_ASSERT(trimRange.GetFirst() < (NIndex) theRanges.size());
	NN_ASSERT(trimRange.GetLast()  < (NIndex) theRanges.size());
	
	offsetFirst = theRanges[trimRange.GetFirst()].GetFirst();
	offsetLast  = theRanges[trimRange.GetLast()].GetLast();
	
	byteRange.SetLocation(offsetFirst);
	byteRange.SetSize(    offsetLast - offsetFirst + 1);



	// Trim the string
	if (!byteRange.IsEmpty())
		{
		theValue->theData.RemoveData(byteRange);
		ValueChanged(theValue);
		}
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
//		NString::+= : Append a string.
//----------------------------------------------------------------------------
const NString& NString::operator += (const NString &theString)
{	const NStringValue		*otherValue;
	NStringEncoder			theEncoder;
	NStringValue			*theValue;
	NData					theData;
	NStatus					theErr;



	// Get the state we need
	theValue   = GetMutable();
	otherValue = theString.GetImmutable();

	theEncoder.RemoveTerminator(theValue->theData, theValue->theEncoding);



	// Append the string
	if (theValue->theEncoding == otherValue->theEncoding)
		theValue->theData += otherValue->theData;
	else
		{
		theErr = theEncoder.Convert(otherValue->theData, theData, otherValue->theEncoding, theValue->theEncoding);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == kNoErr)
			{
			theValue->theData += theData;
			theEncoder.AddTerminator(theValue->theData, theValue->theEncoding);
			}
		}



	// Update our state
	ValueChanged(theValue);
	
	return(*this);
}





//============================================================================
//		NString::+ : Append a string.
//----------------------------------------------------------------------------
const NString NString::operator + (const NString &theString) const
{	NString		theResult;



	// Append the string
	theResult  = *this;
	theResult += theString;
	
	return(theResult);
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
{	static NStringValue		sNullValue = { 0, kNStringEncodingUTF8, NData(1) };



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
		theResult = NHashable::CalculateHash(theValue->theData.GetSize(), theValue->theData.GetData());

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
	*this = theEncoder.DecodeString(kNEncoderValueKey);
}





//============================================================================
//      NString::ValueChanged : Our value has been changed.
//----------------------------------------------------------------------------
#pragma mark -
void NString::ValueChanged(NStringValue *theValue)
{


	// Update our value
	theValue->theSize = GetParser().GetSize();



	// Reset our state
	//
	// State that depends on our value needs to be reset whenever it changes.
	//
	// To help expose stale pointers returned through GetData(), we scrub the
	// buffer in debug builds (vs just freeing the memory).
	ClearHash();

#if NN_DEBUG
	if (!mData.IsEmpty())
		memset(theValue->theData.GetData(), 'X', theValue->theData.GetSize());
#else
	theValue->theData.Clear();
#endif
}





//============================================================================
//		NString::FindMatches : Find a string.
//----------------------------------------------------------------------------
NRangeList NString::FindMatches(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
{	bool			isBackwards, isPattern;
	NRangeList		theResults;
	NRange			findRange;



	// Get the state we need
	findRange   = theRange.GetNormalized(GetSize());
	isBackwards = ((theFlags & kNStringBackwards) == kNStringBackwards);
	isPattern   = ((theFlags & kNStringPattern)   == kNStringPattern);



	// Check the size
	//
	// A literal search string can be checked for length, however a pattern-based
	// search may be larger than the text it matches ("^\s+" will match " ").
	if (IsEmpty() || theString.IsEmpty())
		return(theResults);
	
	if (!isPattern && theString.GetSize() > findRange.GetSize())
		return(theResults);



	// Find the string
	//
	// Backwards searching is only supported for non-pattern searches, and with
	// a forwards search which is then reversed - this should be improved.
	if (isPattern)
		theResults = FindPattern(theString, theFlags, findRange, doAll);
	else
		{
		if (isBackwards)
			{
			theResults = FindString( theString, theFlags, findRange, true);
			if (!theResults.empty())
				{
				if (doAll)
					reverse(theResults);
				else
					theResults = vector(theResults.back());
				}
			}
		else
			theResults = FindString( theString, theFlags, findRange, doAll);
		}
		
	return(theResults);
}





//============================================================================
//      NString::FindString : Find a string.
//----------------------------------------------------------------------------
NRangeList NString::FindString(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const
{	NIndex				sizeA, sizeB, n, offsetB, limitA;
	bool				ignoreCase, updateB;
	NUnicodeParser		parserA, parserB;
	UTF32Char			charA, charB;
	NRange				findRange;
	NRangeList			theResult;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());
	


	// Get the state we need
	parserA =           GetParser();
	parserB = theString.GetParser();
	
	sizeA = parserA.GetSize();
	sizeB = parserB.GetSize();
	
	charA = 0;
	charB = 0;

	limitA     = std::max((NIndex) 1, std::min(theRange.GetLast(), theRange.GetSize() - sizeB));
	ignoreCase = (theFlags & kNStringNoCase);

	findRange = kNRangeNone;
	updateB   = true;
	offsetB   = 0;



	// Find the string
	for (n = theRange.GetFirst(); n <= theRange.GetLast(); n++)
		{
		// Get the state we need
		charA = parserA.GetChar(n, ignoreCase);
		
		if (updateB)
			{
			charB   = parserB.GetChar(offsetB, ignoreCase);
			updateB = false;
			}



		// Check for a match
		if (findRange.IsEmpty())
			{
			// Found a match
			if (charA == charB)
				{
				findRange = NRange(n, 1);
				updateB   = true;
				offsetB   = 1;
				}
			
			// No match found
			else
				;
			}
		
		
		// Continue matching
		else
			{
			// Continue the match
			if (charA == charB)
				{
				findRange.SetSize(findRange.GetSize() + 1);
				updateB  = true;
				offsetB += 1;
				}
			
			// Cancel the match
			else
				{
				findRange = kNRangeNone;
				updateB   = true;
				offsetB   = 0;
				}
			}



		// Save the match
		if (offsetB == sizeB)
			{
			theResult.push_back(findRange);
			if (!doAll)
				break;

			findRange = kNRangeNone;
			updateB   = true;
			offsetB   = 0;
			}
		
		
		
		// Check the limit
		//
		// If we've found a match we need to search to the end of the range, but once we
		// pass the final B-sized section without a match then we know it can't fit.
		if (n >= limitA && findRange.IsEmpty())
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
	const char				*textUTF8, *searchUTF8, *errorMsg;
	NRangeList				theResult, theRanges;
	NRange					matchRange;
	std::vector<int>		theMatches;
	NUnicodeParser			theParser;
	NData					dataUTF8;
	pcre					*regExp;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());
	


	// Get the state we need
	numMatches = 0;
	regFlags   = PCRE_MULTILINE | PCRE_DOTALL | PCRE_UTF8 | PCRE_NO_UTF8_CHECK;
	
	if (theFlags & kNStringNoCase)
		regFlags |= PCRE_CASELESS;

	dataUTF8 = GetData(kNStringEncodingUTF8);
	textUTF8 = (const char *) dataUTF8.GetData();

	theParser = GetParser(dataUTF8, kNStringEncodingUTF8);
	theRanges = theParser.GetRanges();



	// Get the range to search
	NN_ASSERT(theRange.GetFirst() < (NIndex) theRanges.size());
	NN_ASSERT(theRange.GetLast()  < (NIndex) theRanges.size());

	offsetFirst = theRanges[theRange.GetFirst()].GetFirst();
	offsetLast  = theRanges[theRange.GetLast()].GetLast();

	searchUTF8   = textUTF8   + offsetFirst;
	searchSize   = offsetLast - offsetFirst + 1;
	searchOffset = 0;



	// Compile the expression
	regExp = pcre_compile(theString.GetUTF8(), regFlags, &errorMsg, &errorPos, NULL);
	if (regExp == NULL)
		{
		NN_LOG("Unable to compile '%@': %s (%d)", theString, errorMsg, errorPos);
		return(theResult);
		}

	regErr = pcre_fullinfo(regExp, NULL, PCRE_INFO_CAPTURECOUNT, &numMatches);
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
		regErr = pcre_exec(regExp, NULL, searchUTF8, searchSize, searchOffset, PCRE_NO_UTF8_CHECK, &theMatches[0], (int) theMatches.size());
		NN_ASSERT(regErr == PCRE_ERROR_NOMATCH || regErr > 0);



		// Collect the results
		//
		// Offsets are returned as byte offsets relative to the start of the search
		// string, which we convert into absolute character offsets.
		if (regErr > 0)
			{
			for (n = 0; n <= numMatches; n++)
				{
				matchStart = theMatches[(n * 2) + 0];
				matchEnd   = theMatches[(n * 2) + 1];
				matchRange = kNRangeNone;
			
				if (matchStart != -1)
					{
					matchStart = GetCharacterOffset(theRanges, offsetFirst + matchStart);
					matchEnd   = GetCharacterOffset(theRanges, offsetFirst + matchEnd);
					matchRange = NRange(matchStart, matchEnd - matchStart);
					}

				theResult.push_back(matchRange);
				}
			}



		// Advance the search
		if (doAll)
			{
			matchEnd = theMatches[1];
			
			if (regErr > 0 && matchEnd != searchSize)
				searchOffset = matchEnd;
			else
				searchOffset = -1;
			}
		else
			searchOffset = -1;
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
	UTF32Char				theChar;
	NData					theData;



	// Get the state we need
	theParser = GetParser();
	theSize   = theParser.GetSize();



	// Convert to upper case
	for (n = 0; n < theSize; n++)
		{
		theChar = theParser.GetChar(n);
		theChar = toUpper ? theParser.GetUpper(theChar) : theParser.GetLower(theChar);
		
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
	UTF32Char				theChar;
	bool					toUpper;
	NData					theData;



	// Get the state we need
	theParser = GetParser();
	theSize   = theParser.GetSize();
	toUpper   = true;



	// Capitalize words
	for (n = 0; n < theSize; n++)
		{
		theChar = theParser.GetChar(n);

		if (theParser.IsAlpha(theChar))
			{
			if (toUpper)
				{
				theChar = theParser.GetUpper(theChar);
				toUpper = false;
				}
			}
		else
			toUpper = true;
		
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
	UTF32Char				theChar;
	bool					toUpper;
	NData					theData;



	// Get the state we need
	theParser = GetParser();
	theSize   = theParser.GetSize();
	toUpper   = true;



	// Capitalize sentences
	for (n = 0; n < theSize; n++)
		{
		theChar = theParser.GetChar(n);

		if (toUpper)
			{
			if (theParser.IsAlpha(theChar))
				{
				theChar = theParser.GetUpper(theChar);
				toUpper = false;
				}
			}
		else
			toUpper = (theChar == (UTF32Char) '!' ||
					   theChar == (UTF32Char) '.' ||
					   theChar == (UTF32Char) '?');
		
		theData.AppendData((NIndex) sizeof(theChar), &theChar);
		}



	// Update the string
	SetData(theData, kNStringEncodingUTF32);
}





//============================================================================
//		NString::GetParser : Get a parser.
//----------------------------------------------------------------------------
NUnicodeParser NString::GetParser(void) const
{	const NStringValue		*theValue;



	// Get the parser
	theValue = GetImmutable();
	
	return(GetParser(theValue->theData, theValue->theEncoding));
}





//============================================================================
//		NString::GetParser : Get a parser.
//----------------------------------------------------------------------------
NUnicodeParser NString::GetParser(const NData &theData, NStringEncoding theEncoding) const
{	NUnicodeParser			theParser;



	// Get the parser
	theParser.Parse(theData, theEncoding);

	return(theParser);
}





//============================================================================
//      NString::GetCharacterOffset : Get the offset of a character.
//----------------------------------------------------------------------------
NIndex NString::GetCharacterOffset(const NRangeList &theRanges, NIndex byteOffset) const
{	NIndex		n, numItems;



	// Locate the offset
	numItems = (NIndex) theRanges.size();
	
	for (n = 0; n < numItems; n++)
		{
		if (theRanges[n].GetLocation() == byteOffset)
			return(n);
		}
	
	
	
	// Handle last character
	//
	// We allow the byte beyond the last character to be requested, which returns
	// a fake character one beyond the last character (so that a suitable range
	// can be calculated which takes in the last character).
	n = theRanges.back().GetNext();
	
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
	const UTF32Char			*chars32;
	const UTF16Char			*chars16;
	const UTF8Char			*chars8;
	UTF16Char				char16;
	UTF32Char				char32;



	// Get the best encoding
	//
	// We use UTF8 by default, switching to UTF16 for non-ASCII content.
	switch (theEncoding) {
		case kNStringEncodingUTF8:
			theSize      = theData.GetSize() / ((NIndex) sizeof(UTF8Char));
			chars8       = (const UTF8Char *) theData.GetData();
			bestEncoding = kNStringEncodingUTF8;
			
			for (n = 0; n < theSize; n++)
				{
				if (chars8[n] > kASCIILimit)
					{
					bestEncoding = kNStringEncodingUTF16;
					break;
					}
				}
			break;


		case kNStringEncodingUTF16:
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			theSize      = theData.GetSize() / ((NIndex) sizeof(UTF16Char));
			chars16      = (const UTF16Char *) theData.GetData();
			bestEncoding = kNStringEncodingUTF8;
			
			for (n = 0; n < theSize; n++)
				{
				char16 = theParser.GetNativeUTF16(chars16[n], theEncoding);
				if (char16 > kASCIILimit)
					{
					bestEncoding = kNStringEncodingUTF16;
					break;
					}
				}
			break;


		case kNStringEncodingUTF32:
		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			theSize      = theData.GetSize() / ((NIndex) sizeof(UTF32Char));
			chars32      = (const UTF32Char *) theData.GetData();
			bestEncoding = kNStringEncodingUTF8;
			
			for (n = 0; n < theSize; n++)
				{
				char32 = theParser.GetNativeUTF32(chars32[n], theEncoding);
				if (char32 > kASCIILimit)
					{
					bestEncoding = kNStringEncodingUTF16;
					break;
					}
				}
			break;


		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			bestEncoding = kNStringEncodingUTF16;
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
	thePattern = theString;



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


