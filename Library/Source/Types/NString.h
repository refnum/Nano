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
#include "NData.h"
#include "NRange.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
// Encodings
typedef enum {
	kNStringEncodingUTF8,
	kNStringEncodingUTF16,
	kNStringEncodingUTF32
} NStringEncoding;


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
typedef std::vector<UInt8>											NStringValue;
typedef NStringValue::iterator										NStringValueIterator;
typedef NStringValue::const_iterator								NStringValueConstIterator;

typedef std::vector<NString>										NStringList;
typedef NStringList::iterator										NStringListIterator;
typedef NStringList::const_iterator									NStringListConstIterator;

typedef std::map<NString, NString>									NStringMap;
typedef NStringMap::iterator										NStringMapIterator;
typedef NStringMap::const_iterator									NStringMapConstIterator;

typedef NSharedValue<NStringValue>									NSharedValueString;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NString :	public NContainer,
				public NHashable,
				public NComparable<NString>,
				public NSharedValueString {
public:
										NString(const char  *theText, NIndex theSize=kNStringSize, NStringEncoding theEncoding=kNStringEncodingUTF8);
										NString(const NData &theData,                              NStringEncoding theEncoding=kNStringEncodingUTF8);

										 NString(void);
	virtual								~NString(void);


	// Get the size
	//
	// GetSize returns the number of characters in the string, and GetSizeBytes
	// returns the number of bytes needed for a particular encoding.
	NIndex								GetSize(void) const;
	NIndex								GetSizeBytes(NStringEncoding theEncoding=kNStringEncodingUTF8) const;


	// Get the string data
	//
	// Returned pointers are to a NULL-terminated string in the specified encoding,
	// which are valid until the string object is modified or destroyed.
	//
	// Multi-byte encodings may contain embedded 0s, so must return an explicit size.
	const char							*GetUTF8(void) const;
	const UInt8							*GetData(NIndex &theSize, NStringEncoding theEncoding=kNStringEncodingUTF8) const;
	NData								 GetData(                 NStringEncoding theEncoding=kNStringEncodingUTF8) const;


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
    UInt32								ReplaceAll(const NString &theString, const NString &replaceWith, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll);


	// Test the contents
	bool								StartsWith(const NString &theString, NStringFlags theFlags=kNStringNone) const;
	bool								EndsWith(  const NString &theString, NStringFlags theFlags=kNStringNone) const;
	bool								Contains(  const NString &theString, NStringFlags theFlags=kNStringNone) const;


	// Compare two strings
	//
	// EqualTo defaults to a case-insensitive comparison, since case-sensitive comparisons can be obtained with '=='.
	NComparison							Compare(  const NString &theValue)                                        const;
	NComparison							CompareTo(const NString &theString, NStringFlags theFlags=kNStringNone)   const;
	bool								EqualTo(  const NString &theString, NStringFlags theFlags=kNStringNoCase) const;


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
	NString								GetLeft(  NIndex theSize)								   const;
	NString								GetRight( NIndex theSize)								   const;
	NString								GetString(NIndex theLocation, NIndex theSize=kNStringSize) const;
	NString								GetString(const NRange &theRange)						   const;


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

	void								TrimLeft( UInt32 theSize);
	void								TrimRight(UInt32 theSize);


// dair
/*
	// Printf into the string
	//
	// Format uses the standard CoreFoundation formatter, and accepts '%@' as an
	// argument specifier for NCFObjects (automatically casting to a CFTypeRef):
	//
	//		NString		value, output;
	//
	//		value = "World!";
	//		output.Format("Hello %@", value);
	//
	// Since CoreFoundation does not provide a UTF8-aware specifier, string data
	// must always be printed with '%S' (and GetUTF16) or '%@'.
	//
	// Using '%s' and GetUTF8 will produce unreliable results, since '%s' is
	// interpreted in the default system encoding (which may/may not be UTF8).
	void			    DECLARE_NSTRING_FORMAT(const NString &formatString, ...)
	void								Format(const NString &formatString, va_list argList);
*/


	// Operators
	const NString&						operator += (const NString &theString);
	const NString						operator +	(const NString &theString) const;


protected:
	// Get the null value
	const NStringValue					*GetNullValue(void) const;


	// Calculate the hash code
	NHashCode							CalculateHash(void) const;


private:
	void								SetValue(NIndex theSize, const void *thePtr, NStringEncoding theEncoding);
	void								ValueChanged(void);

	NRange								NormalizeRange(const NRange &theRange) const;

	NRangeList							FindMatches(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList							FindString( const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList							FindPattern(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;


private:
	mutable UInt8List					mData;
};





#endif // NSTRING_HDR


