/*	NAME:
		NString.h

	DESCRIPTION:
		String object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTRING_HDR
#define NSTRING_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NStringEncoder.h"
#include "NUnicodeParser.h"
#include "NSharedValue.h"
#include "NContainer.h"
#include "NHashable.h"
#include "NRange.h"
#include "NData.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// Flags
typedef NBitfield NStringFlags;

static const NStringFlags kNStringNone								= 0;
static const NStringFlags kNStringNoCase							= (1 << 0);
static const NStringFlags kNStringBackwards							= (1 << 1);
static const NStringFlags kNStringNumeric							= (1 << 2);
static const NStringFlags kNStringPattern							= (1 << 3);
static const NStringFlags kNStringNumericNoCase						= kNStringNumeric | kNStringNoCase;
static const NStringFlags kNStringPatternNoCase						= kNStringPattern | kNStringNoCase;


// Capitalization
typedef enum {
	kNCapitalizeWords,
	kNCapitalizeSentences
} NStringCapitalization;


// Misc
class NString;

extern const NString kNStringWhitespace;





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Value
typedef struct {
	NIndex				theSize;
	NStringEncoding		theEncoding;
	NData				theData;
} NStringValue;

typedef NSharedValue<NStringValue>									NSharedValueString;


// Lists
typedef std::vector<NString>										NStringList;
typedef NStringList::iterator										NStringListIterator;
typedef NStringList::const_iterator									NStringListConstIterator;

typedef std::map<NString, NString, NHashableCompare<NString> >		NStringMap;
typedef NStringMap::iterator										NStringMapIterator;
typedef NStringMap::const_iterator									NStringMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NString :	public NContainer,
				public NHashable,
				public NComparable<NString>,
				public NSharedValueString {
public:
										NString(const void			*thePtr, NIndex numBytes=kNIndexNone, NStringEncoding theEncoding=kNStringEncodingUTF8);
										NString(const NData			&theData,                             NStringEncoding theEncoding=kNStringEncodingUTF8);
										NString(const NStringUTF8	&theString);
										
										 NString(void);
	virtual								~NString(void);


	// Get the size
	NIndex								GetSize(void) const;


	// Get/set the string
	//
	// GetUTF8/16 return a NULL-terminated string, valid until the string is modified.
	const char							*GetUTF8( void) const;
	const UTF16Char						*GetUTF16(void) const;

	NData								 GetData(                      NStringEncoding theEncoding=kNStringEncodingUTF8, bool nullTerminate=false) const;
	OSStatus							 SetData(const NData &theData, NStringEncoding theEncoding=kNStringEncodingUTF8);


	// Find a substring
	//
	// If the size of theRange is kNIndexNone, the search is to the end of the string.
	//
	// When finding a regexp, the first returned range is the substring matched by the
	// entire expression with subsequent ranges for parenthesized sub-expressions.
	NRange								Find(	const NString &theString, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll) const;
	NRangeList							FindAll(const NString &theString, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll) const;


	// Replace a substring
	//
	// ReplaceAll returns the number of replacements made.
	void								Replace(   const NRange  &theRange,  const NString &replaceWith);
    bool								Replace(   const NString &theString, const NString &replaceWith, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll);
    NIndex								ReplaceAll(const NString &theString, const NString &replaceWith, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll);


	// Test the contents
	bool								StartsWith(const NString &theString, NStringFlags theFlags=kNStringNone) const;
	bool								EndsWith(  const NString &theString, NStringFlags theFlags=kNStringNone) const;
	bool								Contains(  const NString &theString, NStringFlags theFlags=kNStringNone) const;


	// Compare two strings
	//
	// EqualTo defaults to a case-insensitive comparison, since case-sensitive comparisons can be obtained with '=='.
	NComparison							Compare(const NString &theValue)                                        const;
	NComparison							Compare(const NString &theString, NStringFlags theFlags)                const;
	bool								EqualTo(const NString &theString, NStringFlags theFlags=kNStringNoCase) const;


	// Change the case
	NString								GetUpper(void)                                                const;
	NString								GetLower(void)                                                const;
    NString                             GetCapitals(NStringCapitalization theStyle=kNCapitalizeWords) const;

	void								MakeUpper(void);
	void								MakeLower(void);
    void								MakeCapitals(NStringCapitalization theStyle=kNCapitalizeWords);


	// Extract a substring
	NString								GetLeft( NIndex theSize)          const;
	NString								GetRight(NIndex theSize)          const;
	NString								GetString(const NRange &theRange) const;


	// Split the string
	//
	// kNStringWhitespace is always interpreted as a pattern.
	NStringList							Split(const NString &theString, NStringFlags theFlags=kNStringNone) const;


	// Trim the string
	//
	// kNStringWhitespace is always interpreted as a pattern.
	void								TrimLeft( const NString &theString=kNStringWhitespace, NStringFlags theFlags=kNStringNone);
	void								TrimRight(const NString &theString=kNStringWhitespace, NStringFlags theFlags=kNStringNone);
	void								Trim(     const NString &theString=kNStringWhitespace, NStringFlags theFlags=kNStringNone);

	void								TrimLeft( NIndex theSize);
	void								TrimRight(NIndex theSize);
	void								Trim(const NRange &theRange);


	// Format the string
	void								Format(const NString &theFormat, FORMAT_ARGS);


	// Operators
	const NString&						operator += (const NString &theString);
	const NString						operator +	(const NString &theString) const;

										operator NStringUTF8(    void) const;
										operator NFormatArgument(void) const;


protected:
	// Get the null value
	const NStringValue					*GetNullValue(void) const;


	// Calculate the hash code
	NHashCode							CalculateHash(void) const;


private:
	void								ValueChanged(NStringValue *theValue);

	NRangeList							FindMatches(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList							FindString( const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList							FindPattern(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;

	void								CapitalizeCharacters(bool toUpper);
	void								CapitalizeWords(void);
	void								CapitalizeSentences(void);
	
	NUnicodeParser						GetParser(void)                                              const;
	NUnicodeParser						GetParser(const NData &theData, NStringEncoding theEncoding) const;

	NIndex								GetCharacterOffset(  const NRangeList &theRanges, NIndex          byteOffset)  const;
	NStringEncoding						GetBestEncoding(     const NData      &theData,   NStringEncoding theEncoding) const;
	NString								GetWhitespacePattern(const NString    &theString, NStringFlags    &theFlags)   const;


private:
	mutable NData						mData;
};





#endif // NSTRING_HDR


