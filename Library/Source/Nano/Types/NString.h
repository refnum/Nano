/*	NAME:
		NString.h

	DESCRIPTION:
		String object.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NSTRING_H
#define NSTRING_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NMixinContainer.h"
#include "NMixinHashable.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NString
	: public NMixinContainer<NString>
	, public NMixinHashable<NString>
	, public NMixinComparable<NString>
{
public:
										NString();


private:
};



// Nano 3.x implementation
/*
 #include "NComparable.h"
 #include "NContainer.h"
 #include "NData.h"
 #include "NDebuggable.h"
 #include "NEncodable.h"
 #include "NHashable.h"
 #include "NLock.h"
 #include "NRange.h"
 #include "NSharedValue.h"
 #include "NStringEncoder.h"
 #include "NStringFormatter.h"
 #include "NUnicodeParser.h"





   //============================================================================
   //      Constants
   //----------------------------------------------------------------------------
   // Flags
   typedef NBitfield NStringFlags;

   static const NStringFlags kNStringNone                   = 0;
   static const NStringFlags kNStringNoCase                 = (1 << 0);
   static const NStringFlags kNStringBackwards              = (1 << 1);
   static const NStringFlags kNStringNumeric                = (1 << 2);
   static const NStringFlags kNStringPattern                = (1 << 3);
   static const NStringFlags kNStringNumericNoCase          = kNStringNumeric | kNStringNoCase;
   static const NStringFlags kNStringPatternNoCase          = kNStringPattern | kNStringNoCase;


   // Renderings
   typedef NBitfield NStringRendering;

   static const NStringRendering kNStringRenderNone         = 0;
   static const NStringRendering kNStringNullTerminate      = (1 << 0);
   static const NStringRendering kNStringUnicodeBOM         = (1 << 1);


   // Transforms
   typedef NBitfield NStringTransform;

   static const NStringTransform kNStringTransformNone          = 0;
   static const NStringTransform kNStringToLowerCase            = (1 << 0);
   static const NStringTransform kNStringToUpperCase            = (1 << 1);
   static const NStringTransform kNStringCapitalizeWords        = (1 << 2);
   static const NStringTransform kNStringCapitalizeSentences    = (1 << 3);
   static const NStringTransform kNStringStripDiacritics        = (1 << 4);


   // Misc
   class NString;

   extern const NIndex  kNStringLength;
   extern const NString kNStringWhitespace;





   //============================================================================
   //      Types
   //----------------------------------------------------------------------------
   // Value
   typedef struct {
	NIndex                              theSize;
	NStringEncoding                     theEncoding;
	NData                               theData;
   } NStringValue;

   typedef NSharedValue<NStringValue>									NSharedValueString;
   typedef NHashableCompare<NString>									NStringHashCompare;


   // Lists
   typedef std::vector<NString>										NStringList;
   typedef NStringList::iterator										NStringListIterator;
   typedef NStringList::const_iterator									NStringListConstIterator;

   typedef std::map<NString, NString, NStringHashCompare>				NStringMap;
   typedef NStringMap::iterator										NStringMapIterator;
   typedef NStringMap::const_iterator									NStringMapConstIterator;

   typedef std::map<const utf8_t *, NString>							NConstantStringMap;
   typedef NConstantStringMap::iterator								NConstantStringMapIterator;
   typedef NConstantStringMap::const_iterator							NConstantStringMapConstIterator;





   //============================================================================
   //		Class declaration
   //----------------------------------------------------------------------------
   class NString :	public NContainer,
				public NHashable,
				public NEncodable,
				public NDebuggable,
				public NComparable<NString>,
				public NSharedValueString {
   public:
										NENCODABLE_DECLARE(NString);


	// Pointer constructor
	//
	// Strings can be created from external pointers in no-copy or copy modes.
	//
	// No-copy mode is extremely efficient, and will only create one instance of an NString
	// for each unique pointer. This mode requires that the pointer persist for the lifetime
	// of the process, the text be NULL-terminated, and the text be in UTF8 encoding.
	//
	// Copy mode copies the text data when creating the string, allowing the supplied pointer
	// to be disposed of or mutated without affecting the string. The number of bytes in the
	// text must be supplied (although kNStringLength can be used for implicit length), as
	// well as the encoding (which defaults to UTF8).
	//
	// No-copy behaviour is typically used for string literals, while copy behaviour is used
	// for dynamic text data:
	//
	//		NString GetStringNoCopy(void)
	//		{	NString		theString;
	//
	//			theString = "Hello";
	//			return(theString);
	//		}
	//
	//		NString GetStringCopy(void)
	//		{	NString		theString;
	//			char		*thePtr;
	//
	//			thePtr = malloc(12);
	//			strcpy(thePtr, "Hello World");
	//
	//			theString = NString(thePtr, kNStringLength);
	//			free(thePtr);
	//
	//			return(theString);
	//		}
	//
	// The default parameters were chosen to allow NStrings to be created from string literals
	// without copying, although this requires that text which must be copied must be created
	// with an explicit NString().
										NString(const utf8_t *noCopyText);
										NString(const void   *copyText, NIndex numBytes, NStringEncoding theEncoding=kNStringEncodingUTF8);

										NString(const NData &theData, NStringEncoding theEncoding=kNStringEncodingUTF8);
										NString(      utf8_t theChar);

										NString();
	virtual                            ~NString();


	// Clear the value
	void                                Clear();


	// Get the size
	//
	// Returns the number of code points (utf32_t characters) in the string.
	NIndex                              GetSize() const;


	// Get the text
	//
	// Returns a NULL-terminated string that remains valid until the string is modified.
	const char						   *GetText(NStringEncoding theEncoding) const;
	const utf8_t					   *GetUTF8()      const;
	const utf16_t					   *GetUTF16()    const;
	const utf32_t					   *GetUTF32()    const;


	// Get/set the string
	//
	// By default GetData returns a UTF8 encoded string, without any BOM or NULL terminator.
	NData                               GetData(                      NStringEncoding theEncoding=kNStringEncodingUTF8, NStringRendering renderAs=kNStringRenderNone) const;
	NStatus                             SetData(const NData &theData, NStringEncoding theEncoding=kNStringEncodingUTF8);


	// Find a substring
	//
	// When finding a regexp, the first returned range is the substring matched by the
	// entire expression with subsequent ranges for parenthesized sub-expressions.
	NRange                              Find(	  const NString &theString, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll) const;
	NRangeList                          FindAll(const NString &theString, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll) const;


	// Replace a substring
	//
	// ReplaceAll returns the number of replacements made.
	void                                Replace(   const NRange  &theRange,  const NString &replaceWith);
	bool                                Replace(   const NString &theString, const NString &replaceWith, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll);
	NIndex                              ReplaceAll(const NString &theString, const NString &replaceWith, NStringFlags theFlags=kNStringNone, const NRange &theRange=kNRangeAll);


	// Transform the contents
	//
	// Multiple tranformations may be applied at once, however their order is undefined.
	void                                Transform(NStringTransform theTransform, const NRange &theRange=kNRangeAll);


	// Convert to upper/lower case
	NString                             GetUpper() const;
	NString                             GetLower() const;


	// Test the contents
	bool                                StartsWith(const NString &theString, NStringFlags theFlags=kNStringNone) const;
	bool                                EndsWith(  const NString &theString, NStringFlags theFlags=kNStringNone) const;
	bool                                Contains(  const NString &theString, NStringFlags theFlags=kNStringNone) const;


	// Compare two strings
	//
	// EqualTo defaults to a case-insensitive comparison, since case-sensitive comparisons can be obtained with '=='.
	NComparison                         Compare(const NString &theValue)                                        const;
	NComparison                         Compare(const NString &theString, NStringFlags theFlags)                const;
	bool                                EqualTo(const NString &theString, NStringFlags theFlags=kNStringNoCase) const;


	// Extract a substring
	NString                             GetLeft(  NIndex theSize)          const;
	NString                             GetRight( NIndex theSize)          const;
	NString                             GetString(NIndex thePosition)      const;
	NString                             GetString(const NRange &theRange)  const;


	// Split the string
	//
	// Two consecutive dividers produce an empty string:
	//
	//		split "a,,b"  with "," produces {"a", "", "b"}
	//
	// Leading/trailing dividers do not produce an empty string:
	//
	//		split ",a,b," with "," produces {"a", "b"}
	//
	// kNStringWhitespace is always interpreted as a pattern.
	NStringList                         Split(const NString &theString, NStringFlags theFlags=kNStringNone) const;


	// Join strings
	static NString                      Join(const NStringList &theStrings, const NString &joinWith=" ", bool skipEmpty=true);


	// Trim the string
	//
	// kNStringWhitespace is always interpreted as a pattern.
	void                                TrimLeft( const NString &theString=kNStringWhitespace, NStringFlags theFlags=kNStringNone);
	void                                TrimRight(const NString &theString=kNStringWhitespace, NStringFlags theFlags=kNStringNone);
	void                                Trim(     const NString &theString=kNStringWhitespace, NStringFlags theFlags=kNStringNone);

	void                                TrimLeft(  NIndex  theSize);
	void                                TrimRight( NIndex  theSize);
	void                                Trim(const NRange &theRange);


	// Format the string
	void                                Format(const NString &theFormat, NN_FORMAT_ARGS);


	// Operators
	bool                                operator == (const NString &theString) const;
	bool                                operator != (const NString &theString) const;

	const NString&                      operator += (const NString &theString);
		  NString						operator +	(const NString &theString) const;


   protected:
	// Get the null value
	const NStringValue					*GetNullValue() const;


	// Calculate the hash
	NHashCode                           CalculateHash() const;


	// Encode/decode the object
	void                                EncodeSelf(      NEncoder &theEncoder) const;
	void                                DecodeSelf(const NEncoder &theEncoder);


   private:
	void                                ValueChanged(NStringValue *theValue, bool updateSize=true);

	NRangeList                          FindMatches(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList                          FindString( const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;
	NRangeList                          FindPattern(const NString &theString, NStringFlags theFlags, const NRange &theRange, bool doAll) const;

	void                                CapitalizeCharacters(bool toUpper);
	void                                CapitalizeWords();
	void                                CapitalizeSentences();

	void                                TrimWhitespace(       bool fromLeft, bool fromRight);
	void                                TrimWhitespaceUTF8(   bool fromLeft, bool fromRight);
	void                                TrimWhitespaceGeneric(bool fromLeft, bool fromRight);

	NIndex                              GetCharacterOffset(  const NRangeList *theRanges, NIndex          byteOffset)  const;
	NStringEncoding                     GetBestEncoding(     const NData      &theData,   NStringEncoding theEncoding) const;
	NString                             GetWhitespacePattern(const NString    &theString, NStringFlags    &theFlags)   const;

	NUnicodeParser                      GetParser()                                                                                   const;
	uint64_t                            GetNumber(const NUnicodeParser &theParser, NIndex &theIndex, NIndex theSize, utf32_t theChar) const;
	NString                             GetString(const NUnicodeParser &theParser, const NRange &theRange)                            const;

	static NString                      GetConstantString(const utf8_t *theText);


   private:
	mutable NData                       mData;
   };





   //============================================================================
   //		Inline functions
   //----------------------------------------------------------------------------
   // Get an NComparison from two values using Compare
   //
   // Although NComparable.h provides a templated GetComparison, we provide
   // a specialisation to avoid a second call to Compare for the >= case.
   inline NComparison GetComparison(const NString &a, const NString &b)
   {
										return(a.Compare(b));
   }


   // Format a string
   //
   // An NString can not be constructed directly from a list of formatting
   // parameters, to avoid ambiguity with construction from char* strings.
   //
   // This function can be used to simplify situations where a local is
   // created purely to receive a format which is then passed elsewhere.
   inline NString NFormatString(const NString &theFormat, NN_FORMAT_ARGS)
   {	NString		theResult;

	theResult.Format(theFormat, NN_FORMAT_ARGS_LIST);
										return(theResult);
   }

 */
// Nano 3.x implementation



#endif // NSTRING_H
