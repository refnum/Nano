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

#include "NCFObject.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt32 kStringLength								= (UInt32) -1;





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
//		Class declaration
//----------------------------------------------------------------------------
class NString : public NCFObject {
public:
										NString(OSType				  theType);
										NString(CFStringRef			 cfString);
										NString(const HFSUniStr255 &theString);
										NString(const NData			 &theData,								   CFStringEncoding theEncoding=kCFStringEncodingUTF8);
										NString(const char		   *theString, UInt32 theLength=kStringLength, CFStringEncoding theEncoding=kCFStringEncodingUTF8);
										NString(const UniChar	   *theString, UInt32 theLength=kStringLength);

										NString(void);
	virtual								~NString(void);


	// Is the string empty?
	bool								IsEmpty(   void) const;
	bool								IsNotEmpty(void) const;


	// Get the string size
	UInt32								GetSize(void) const;


	// Clear the string
	void								Clear(void);


	// Get the string data
	//
	// The returned pointer is to a NULL-terminated string in the specified encoding,
	// and is valid until the string object is disposed of or modified.
	const char							*GetUTF8( void) const;
	const UniChar						*GetUTF16(void) const;


	// Pattern-match the string
	//
	// When matching a pattern, the first range returned is the substring matched
	// by the entire regular expression. Subsequent ranges correspond to any
	// parenthesized sub-expressions within the pattern.
	bool								IsMatch(	const NString &thePattern, CFOptionFlags theFlags=0) const;
	CFRangeList							FindPattern(const NString &thePattern, CFOptionFlags theFlags=0) const;


	// Search the string
	//
	// If the length of theRange is kCFNotFound, the search is to the end of the string.
	CFRange								Find(		const NString &theString, CFOptionFlags theFlags=0, const CFRange &theRange=kCFRangeAll) const;
	CFRangeList							FindAll(	const NString &theString, CFOptionFlags theFlags=0, const CFRange &theRange=kCFRangeAll) const;


	// Replace a substring
	void								Replace(   const CFRange &theRange,	 const NString &replaceWith);
	void								ReplaceAll(const NString &theString, const NString &replaceWith);


	// Compare two strings
	CFComparisonResult					Compare(const NString &theString, CFOptionFlags theFlags=0) const;
	bool								EqualTo(const NString &theString, bool ignoreCase=false)	const;
	

	// Change the case
	NString								GetUpper(void) const;
	NString								GetLower(void) const;

	void								MakeUpper(void);
	void								MakeLower(void);


	// Test the contents
	bool								StartsWith(const NString &theString, CFOptionFlags theFlags=0) const;
	bool								EndsWith(  const NString &theString, CFOptionFlags theFlags=0) const;
	bool								Contains(  const NString &theString, CFOptionFlags theFlags=0) const;


	// Extract the contents
	NString								GetLeft(  UInt32 theLength)									const;
	NString								GetRight( UInt32 theLength)									const;
	NString								GetString(UInt32 theOffset, UInt32 theLength=kStringLength) const;
	NString								GetString(const CFRange &theRange)							const;


	// Trim the string
	void								Trim(const NString &theString);
	void								TrimWhitespace(void);


	// Split the string
	//
	// Splits the string into a list of strings that are separated by splitWith
	// (if splitExact is true), or by any of the characters in splitWith.
	NStringList							Split(const NString &splitWith, bool splitExact=false) const;


	// Printf into the string
	//
	// The format string is assumed to be in UTF8 format.
	void								Format(const char *formatString, ...);
	void								Format(const char *formatString, va_list argList);


	// Operators
	const bool							operator == (const NString &theString) const;
	const bool							operator != (const NString &theString) const;
	const bool							operator <	(const NString &theString) const;
	const NString&						operator += (const NString &theString);
	const NString						operator +	(const NString &theString) const;


protected:
	// Get the null value
	CFTypeRef							GetNullValue(void);


	// Handle an object change
	void								DoObjectChanging(void);


private:
	void								InitializeSelf(CFStringRef cfString);

	const UInt8							*GetData(CFStringEncoding theEncoding) const;


private:
	mutable NData						mData;
};





#endif // NSTRING_HDR
