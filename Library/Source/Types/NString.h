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
#include "NContainer.h"
#include "NHashable.h"
#include "NSharedValue.h"
#include "NStringFormatter.h"
#include "NStringEncoder.h"
#include "NData.h"
#include "NRange.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// Flags
typedef NBitfield NStringFlags;

static const NStringFlags kNStringNone								= 0;
static const NStringFlags kNStringNoCase							= (1 << 0);
static const NStringFlags kNStringNumeric							= (1 << 1);
static const NStringFlags kNStringPattern							= (1 << 2);
static const NStringFlags kNStringNumericNoCase						= kNStringNumeric | kNStringNoCase;
static const NStringFlags kNStringPatternNoCase						= kNStringPattern | kNStringNoCase;


// Misc
class NString;

extern const NString kNStringWhitespace;
extern const NIndex  kNStringSize;





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Value
typedef struct {
	NIndex			theSize;
	NData			dataUTF8;
} NStringValue;

typedef NSharedValue<NStringValue>									NSharedValueString;


// Lists
typedef std::vector<NString>										NStringList;
typedef NStringList::iterator										NStringListIterator;
typedef NStringList::const_iterator									NStringListConstIterator;

typedef std::map<NString, NString>									NStringMap;
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
										NString(const void			*thePtr, NIndex numBytes=kNStringSize, NStringEncoding theEncoding=kNStringEncodingUTF8);
										NString(const NData			&theData,                              NStringEncoding theEncoding=kNStringEncodingUTF8);
										NString(const NStringUTF8	&theString);
										
										 NString(void);
	virtual								~NString(void);


	// Get the size
	NIndex								GetSize(void) const;


	// Get/set the string
	//
	// GetUTF8 returns a NULL-terminated string. GetData can optionally include
	// a NULL terminator, of the specified size, to the encoded string.
	//
	// Data passed to SetData should not be NULL terminated. 
	const char							*GetUTF8(void) const;

	NData								 GetData(                      NStringEncoding theEncoding=kNStringEncodingUTF8, NIndex nullBytes=0) const;
	void								 SetData(const NData &theData, NStringEncoding theEncoding=kNStringEncodingUTF8);


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
	// When replacing a string, returns the number of replacements made.
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
	//
	// Capitalization can be performed on each word, or each sentence.
	NString								GetUpper(void)                  const;
	NString								GetLower(void)                  const;
    NString                             GetCapitals(bool eachWord=true) const;

	void								MakeUpper(void);
	void								MakeLower(void);
    void								MakeCapitals(bool eachWord=true);


	// Extract a substring
	NString								GetLeft( NIndex theSize)          const;
	NString								GetRight(NIndex theSize)          const;
	NString								GetString(const NRange &theRange) const;


	// Split the string
	//
	// Can split with an exact substring, or a list of matching characters.
	NStringList							Split(const NString &theString, bool isExact=false) const;


	// Trim the string
	//
	// Can trim with an an exact substring, or a list of matching characters.
	void								Trim(     const NString &theString=kNStringWhitespace, bool isExact=false);
	void								TrimLeft( const NString &theString=kNStringWhitespace, bool isExact=false);
	void								TrimRight(const NString &theString=kNStringWhitespace, bool isExact=false);

	void								TrimLeft( NIndex theSize);
	void								TrimRight(NIndex theSize);


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

	NRange								NormalizeRange(const NRange &theRange) const;

	NRangeList							FindMatches(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList							FindString( const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList							FindPattern(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;


};





#endif // NSTRING_HDR


