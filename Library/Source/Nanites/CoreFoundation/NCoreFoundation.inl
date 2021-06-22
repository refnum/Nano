/*	NAME:
		NCoreFoundation.inl

	DESCRIPTION:
		CoreFoundation support.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
//		ToCF : Convert NArray to CFArrayRef.
//-----------------------------------------------------------------------------
inline NCFArray ToCF(const NArray& theArray)
{
	return NCFArray(theArray);
}





//=============================================================================
//		ToCF : Convert NData to CFDataRef.
//-----------------------------------------------------------------------------
inline NCFData ToCF(const NData& theData)
{
	return NCFData(theData);
}





//=============================================================================
//		ToCF : Convert NDate to CFDateRef.
//-----------------------------------------------------------------------------
inline NCFDate ToCF(const NDate& theDate)
{
	return NCFDate(theDate);
}





//=============================================================================
//		ToCF : Convert NDictionary to CFDictionaryRef.
//-----------------------------------------------------------------------------
inline NCFDictionary ToCF(const NDictionary& theDictionary)
{
	return NCFDictionary(theDictionary);
}





//=============================================================================
//		ToCF : Convert NNumber to CFNumberRef.
//-----------------------------------------------------------------------------
inline NCFNumber ToCF(const NNumber& theNumber)
{
	return NCFNumber(theNumber);
}





//=============================================================================
//		ToCF : Convert NRange to CFRange.
//-----------------------------------------------------------------------------
inline CFRange ToCF(const NRange& theRange)
{
	return CFRangeMake(CFIndex(theRange.GetLocation()), CFIndex(theRange.GetSize()));
}





//=============================================================================
//		ToCF : Convert NString to CFStringRef.
//-----------------------------------------------------------------------------
inline NCFString ToCF(const NString& theString)
{
	return NCFString(theString);
}





//=============================================================================
//		ToCF : Convert NURL to CFURLRef.
//-----------------------------------------------------------------------------
inline NCFURL ToCF(const NURL& theURL)
{
	return NCFURL(theURL);
}





//=============================================================================
//		ToNN : Convert CFArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFArrayRef cfArray)
{
	NCFArray theArray;

	bool wasOK = theArray.Set(cfArray);
	NN_REQUIRE(wasOK);

	return theArray.GetArray();
}





//=============================================================================
//		ToNN : Convert CFMutableArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFMutableArrayRef cfArray)
{
	return ToNN(CFArrayRef(cfArray));
}





//=============================================================================
//		ToNN : Convert CFDataRef to NData.
//-----------------------------------------------------------------------------
inline NData ToNN(CFDataRef cfData)
{
	NCFData theData;

	bool wasOK = theData.Set(cfData);
	NN_REQUIRE(wasOK);

	return theData.GetData();
}





//=============================================================================
//		ToNN : Convert CFMutableDataRef to NData.
//-----------------------------------------------------------------------------
inline NData ToNN(CFMutableDataRef cfData)
{
	return ToNN(CFDataRef(cfData));
}





//=============================================================================
//		ToNN : Convert CFDateRef to NDate.
//-----------------------------------------------------------------------------
inline NDate ToNN(CFDateRef cfDate)
{
	NCFDate theDate;

	bool wasOK = theDate.Set(cfDate);
	NN_REQUIRE(wasOK);

	return theDate.GetDate();
}





//=============================================================================
//		ToNN : Convert CFDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFDictionaryRef cfDictionary)
{
	NCFDictionary theDictionary;

	bool wasOK = theDictionary.Set(cfDictionary);
	NN_REQUIRE(wasOK);

	return theDictionary.GetDictionary();
}





//=============================================================================
//		ToNN : Convert CFMutableDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFMutableDictionaryRef cfDictionary)
{
	return ToNN(CFDictionaryRef(cfDictionary));
}





//=============================================================================
//		ToNN : Convert CFNumberRef to NNumber.
//-----------------------------------------------------------------------------
inline NNumber ToNN(CFNumberRef cfNumber)
{
	NCFNumber theNumber;

	bool wasOK = theNumber.Set(cfNumber);
	NN_REQUIRE(wasOK);

	return theNumber.GetNumber();
}





//=============================================================================
//		ToNN : Convert CFRange to NRange.
//-----------------------------------------------------------------------------
inline NRange ToNN(const CFRange& cfRange)
{
	return NRange(size_t(cfRange.location), size_t(cfRange.length));
}





//=============================================================================
//		ToNN : Convert CFStringRef to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(CFStringRef cfString)
{
	NCFString theString;

	bool wasOK = theString.Set(cfString);
	NN_REQUIRE(wasOK);

	return theString.GetString();
}





//=============================================================================
//		ToNN : Convert CFMutableStringRef to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(CFMutableStringRef cfString)
{
	return ToNN(CFStringRef(cfString));
}





//=============================================================================
//		ToNN : Convert CFURLRef to NURL.
//-----------------------------------------------------------------------------
inline NURL ToNN(CFURLRef cfURL)
{
	NCFURL theURL;

	bool wasOK = theURL.Set(cfURL);
	NN_REQUIRE(wasOK);

	return theURL.GetURL();
}





//=============================================================================
//		operator== : CFRange equality.
//-----------------------------------------------------------------------------
inline bool operator==(const CFRange& value1, const CFRange& value2)
{
	return value1.location == value2.location && value1.length == value2.length;
}





//=============================================================================
//		operator!= : CGPoint inequality.
//-----------------------------------------------------------------------------
inline bool operator!=(const CFRange& value1, const CFRange& value2)
{
	return value1.location != value2.location || value1.length != value2.length;
}
