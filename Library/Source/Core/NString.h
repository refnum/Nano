/*	NAME:
		NString.h

	DESCRIPTION:
		Nano string object.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTRING_HDR
#define NSTRING_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>
#include <map>

#include "NComparable.h"
#include "NContainer.h"
#include "NCFObject.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
static const UInt32 kStringLength								= (UInt32) -1;
static const char  *kStringWhitespace							= " \t\r\n";


// CFStringCompareFlags
//
// Rather than define a separate set of flags that share the same values,
// NString supports an extended set of CFStringCompareFlags.
//
// These values currently occupy an unused portion of the bitfield, and are
// masked off prior to use to avoid clashes with future CFString options.
//
// As such their values may change, and should not be saved to disk.
static const CFOptionFlags kCFCompareMaskNano					= 0xFF000000;
static const CFOptionFlags kCFComparePattern					= (1 << 24);

static const CFOptionFlags kCFCompareNoCase						= kCFCompareCaseInsensitive;
static const CFOptionFlags kCFComparePatternNoCase				= kCFComparePattern | kCFCompareNoCase;





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NString;

typedef std::vector<NString>									NStringList;
typedef NStringList::iterator									NStringListIterator;
typedef NStringList::const_iterator								NStringListConstIterator;

typedef std::map<NString, NString>								NStringMap;
typedef NStringMap::iterator									NStringMapIterator;
typedef NStringMap::const_iterator								NStringMapConstIterator;

typedef std::vector<CFRange>									CFRangeList;
typedef CFRangeList::iterator									CFRangeListIterator;
typedef CFRangeList::const_iterator								CFRangeListConstIterator;
typedef CFRangeList::reverse_iterator							CFRangeListReverseIterator;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Declare a 0-argument Format
#define DECLARE_NSTRING_FORMAT_0()										\
																		\
	Format(const NString &formatString)									\
	{																	\
		Set(formatString, false);										\
	}


// Declare an N-argument format
#define DECLARE_NSTRING_FORMAT_N(_types, _args, _pods)					\
																		\
	template <_types> void												\
	Format(const NString &formatString, _args)							\
	{																	\
		FormatArgs(formatString.GetUTF8(), _pods);						\
	}


// Declare a POD converter
//
// Non-POD types can passed to Format if a suitable GetPOD conversion method
// is defined. If no conversion can be found, gcc's existing warning about
// passing a non-POD object to a variadic function will be invoked.
#define DECLARE_NSTRING_GETPOD(_src, _dst)								\
																		\
	inline _dst GetPOD(_src theValue)									\
	{																	\
		return((_dst) theValue);										\
	}


// Declare a Format(...)
//
// Since C++ does not currently support default types in template definitions,
// we use a set of macros to construct templated functions that can take from
// 0 to N arguments.
//
// To simplify extending this to larger values of N, those functions are
// built out of common components.
#define TYPES_T1											  class T1
#define TYPES_T2									TYPES_T1, class T2
#define TYPES_T3									TYPES_T2, class T3
#define TYPES_T4									TYPES_T3, class T4
#define TYPES_T5									TYPES_T4, class T5
#define TYPES_T6									TYPES_T5, class T6
#define TYPES_T7									TYPES_T6, class T7
#define TYPES_T8									TYPES_T7, class T8
#define TYPES_T9									TYPES_T8, class T9
#define TYPES_T10									TYPES_T9, class T10

#define ARGS_T1												 const T1  &arg1
#define ARGS_T2										ARGS_T1, const T2  &arg2
#define ARGS_T3										ARGS_T2, const T3  &arg3
#define ARGS_T4										ARGS_T3, const T4  &arg4
#define ARGS_T5										ARGS_T4, const T5  &arg5
#define ARGS_T6										ARGS_T5, const T6  &arg6
#define ARGS_T7										ARGS_T6, const T7  &arg7
#define ARGS_T8										ARGS_T7, const T8  &arg8
#define ARGS_T9										ARGS_T8, const T9  &arg9
#define ARGS_T10									ARGS_T9, const T10 &arg10

#define GETPOD_T1											   GetPOD(arg1)
#define GETPOD_T2									GETPOD_T1, GetPOD(arg2)
#define GETPOD_T3									GETPOD_T2, GetPOD(arg3)
#define GETPOD_T4									GETPOD_T3, GetPOD(arg4)
#define GETPOD_T5									GETPOD_T4, GetPOD(arg5)
#define GETPOD_T6									GETPOD_T5, GetPOD(arg6)
#define GETPOD_T7									GETPOD_T6, GetPOD(arg7)
#define GETPOD_T8									GETPOD_T7, GetPOD(arg8)
#define GETPOD_T9									GETPOD_T8, GetPOD(arg9)
#define GETPOD_T10									GETPOD_T9, GetPOD(arg10)

#define DECLARE_NSTRING_FORMAT(...)										\
	DECLARE_NSTRING_FORMAT_0()											\
	DECLARE_NSTRING_FORMAT_N(TYPES_T1,  ARGS_T1,  GETPOD_T1)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T2,  ARGS_T2,  GETPOD_T2)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T3,  ARGS_T3,  GETPOD_T3)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T4,  ARGS_T4,  GETPOD_T4)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T5,  ARGS_T5,  GETPOD_T5)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T6,  ARGS_T6,  GETPOD_T6)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T7,  ARGS_T7,  GETPOD_T7)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T8,  ARGS_T8,  GETPOD_T8)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T9,  ARGS_T9,  GETPOD_T9)			\
	DECLARE_NSTRING_FORMAT_N(TYPES_T10, ARGS_T10, GETPOD_T10)





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NString :	public NCFObject,
				public NComparable,
				public NContainer {
public:
										NString(OSType				  theType);
										NString(const HFSUniStr255 &theString);
										NString(const NData			 &theData,								   CFStringEncoding theEncoding=kCFStringEncodingUTF8);
										NString(const char		   *theString, UInt32 theLength=kStringLength, CFStringEncoding theEncoding=kCFStringEncodingUTF8);
										NString(const UniChar	   *theString, UInt32 theLength=kStringLength);
										NString(CFStringRef			 cfString=NULL);

	virtual								~NString(void);


	// Get the size
	UInt32								GetSize(void) const;


	// Get the string data
	//
	// The returned pointer is to a NULL-terminated string in the specified encoding,
	// and is valid until the string object is modified or destroyed.
	//
	// Multi-byte encodings may contain embedded 0s, so must return an explicit size.
	const char							*GetUTF8( void) const;
	const UniChar						*GetUTF16(void) const;
	const UInt8							*GetData(CFStringEncoding theEncoding, CFIndex &theSize) const;


	// Find a substring
	//
	// If the length of theRange is kCFNotFound, the search is to the end of the string.
	//
	// Supports kCFComparePattern for regex matching. The first returned range is the substring
	// matched by the entire expression, with subsequent ranges for parenthesized sub-expressions.
	CFRange								Find(	const NString &theString, CFOptionFlags theFlags=0, const CFRange &theRange=kCFRangeAll) const;
	CFRangeList							FindAll(const NString &theString, CFOptionFlags theFlags=0, const CFRange &theRange=kCFRangeAll) const;


	// Replace a substring
	//
	// Supports kCFComparePattern for regex matching, and returns the number of replacements.
	void								Replace(   const CFRange &theRange,  const NString &replaceWith);
    bool								Replace(   const NString &theString, const NString &replaceWith, CFOptionFlags theFlags=0, const CFRange &theRange=kCFRangeAll);
    UInt32								ReplaceAll(const NString &theString, const NString &replaceWith, CFOptionFlags theFlags=0, const CFRange &theRange=kCFRangeAll);


	// Test the contents
	//
	// Supports kCFComparePattern for regex matching.
	bool								StartsWith(const NString &theString, CFOptionFlags theFlags=0) const;
	bool								EndsWith(  const NString &theString, CFOptionFlags theFlags=0) const;
	bool								Contains(  const NString &theString, CFOptionFlags theFlags=0) const;


	// Compare two strings
	//
	// EqualTo defaults to a case-insensitive comparison, since case-sensitive comparisons can be obtained with '=='.
	CFComparisonResult					CompareTo(const NString &theString, CFOptionFlags theFlags=0)                const;
	bool								EqualTo(  const NString &theString, CFOptionFlags theFlags=kCFCompareNoCase) const;


	// Change the case
	//
	// Capitalization can be performed on each word, or each sentence.
	NString								GetUpper(void) const;
	NString								GetLower(void) const;
    NString                             GetCapitals(bool eachWord=true) const;

	void								MakeUpper(void);
	void								MakeLower(void);
    void								MakeCapitals(bool eachWord=true);


	// Extract a substring
	NString								GetLeft(  UInt32 theLength)									const;
	NString								GetRight( UInt32 theLength)									const;
	NString								GetString(UInt32 theOffset, UInt32 theLength=kStringLength) const;
	NString								GetString(const CFRange &theRange)							const;


	// Split the string
	//
	// Can split with an exact substring, or a list of matching characters.
	NStringList							Split(const NString &theString, bool isExact=false) const;


	// Trim the string
	//
	// Can trim with an an exact substring, or a list of matching characters.
	void								Trim(     const NString &theString=kStringWhitespace, bool isExact=false);
	void								TrimLeft( const NString &theString=kStringWhitespace, bool isExact=false);
	void								TrimRight(const NString &theString=kStringWhitespace, bool isExact=false);

	void								TrimLeft( UInt32 theLength);
	void								TrimRight(UInt32 theLength);


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


	// Operators
	const NString&						operator += (const NString &theString);
	const NString						operator +	(const NString &theString) const;
	bool								operator == (const char    *theString) const;
	bool								operator == (const NString &theString) const;
	bool								operator != (const char    *theString) const;
	bool								operator != (const NString &theString) const;


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);


	// Handle an object change
	void								DoObjectChanging(void);


	// Compare two objects
	CFComparisonResult					Compare(const NComparable &theObject) const;


private:
	void								InitializeSelf(CFStringRef cfString, bool takeOwnership);

	CFRangeList							FindMatches(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange, bool doAll) const;
	CFRangeList							FindString( const NString &theString, CFOptionFlags theFlags, const CFRange &theRange, bool doAll) const;
	CFRangeList							FindPattern(const NString &theString, CFOptionFlags theFlags, const CFRange &theRange, bool doAll) const;

	CFRange								NormalizeRange(const CFRange &theRange)                 const;
	CFOptionFlags						NormalizeFlags(CFOptionFlags theFlags, bool &isPattern) const;

    void                                FormatArgs(const NString &formatString, ...);

	DECLARE_NSTRING_GETPOD(bool,				bool);
	DECLARE_NSTRING_GETPOD(UInt8,				UInt8);
	DECLARE_NSTRING_GETPOD(SInt8,				SInt8);
	DECLARE_NSTRING_GETPOD(UInt16,				UInt16);
	DECLARE_NSTRING_GETPOD(SInt16,				SInt16);
	DECLARE_NSTRING_GETPOD(UInt32,				UInt32);
	DECLARE_NSTRING_GETPOD(SInt32,				SInt32);
	DECLARE_NSTRING_GETPOD(UInt64,				UInt64);
	DECLARE_NSTRING_GETPOD(SInt64,				SInt64);
	DECLARE_NSTRING_GETPOD(Float32,				Float32);
	DECLARE_NSTRING_GETPOD(Float64,				Float64);
	DECLARE_NSTRING_GETPOD(const NCFObject &,	CFTypeRef);
	DECLARE_NSTRING_GETPOD(const void *,		const void *);


private:
	mutable std::vector<UInt8>			mData;
};





#endif // NSTRING_HDR
