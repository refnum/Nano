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
#include "NCocoa.h"





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
//		ToCF : Convert NArray to CFArrayRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
CFArrayRef ToCF(const NArray& theArray)
{
	return CFArrayRef(ToNS(theArray));
}





//=============================================================================
//		ToCF : Convert NData to CFDataRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
CFDataRef ToCF(const NData& theData)
{
	return CFDataRef(ToNS(theData));
}





//=============================================================================
//		ToCF : Convert NDate to CFDateRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
CFDateRef ToCF(const NDate& theDate)
{
	return CFDateRef(ToNS(theDate));
}





//=============================================================================
//		ToCF : Convert NDictionary to CFDictionaryRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
CFDictionaryRef ToCF(const NDictionary& theDictionary)
{
	return CFDictionaryRef(ToNS(theDictionary));
}





//=============================================================================
//		ToCF : Convert NNumber to CFNumberRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
CFNumberRef ToCF(const NNumber& theNumber)
{
	return CFNumberRef(ToNS(theNumber));
}





//=============================================================================
//		ToCF : Convert NRange to CFRange.
//-----------------------------------------------------------------------------
inline CFRange ToCF(const NRange& theRange)
{
	return CFRangeMake(theRange.GetLocation(), theRange.GetSize());
}





//=============================================================================
//		ToCF : Convert NString to CFStringRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
CFStringRef ToCF(const NString& theString)
{
	return CFStringRef(ToNS(theString));
}





//=============================================================================
//		ToCF : Convert NURL to CFURLRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
CFURLRef ToCF(const NURL& theURL)
{
	return CFURLRef(ToNS(theURL));
}





//=============================================================================
//		ToCF : Convert CFArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFArrayRef cfArray)
{
	NCFArray theArray;

	bool wasOK = theArray.SetObject(cfArray);
	NN_REQUIRE(wasOK);

	return theArray.GetArray();
}





//=============================================================================
//		ToCF : Convert CFMutableArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFMutableArrayRef cfArray)
{
	return ToNN(CFArrayRef(cfData));
}





//=============================================================================
//		ToCF : Convert CFDataRef to NData.
//-----------------------------------------------------------------------------
inline NData ToNN(CFDataRef cfData)
{
	NCFData theData;

	bool wasOK = theData.SetObject(cfData);
	NN_REQUIRE(wasOK);

	return theData.GetData();
}





//=============================================================================
//		ToCF : Convert CFMutableDataRef to NData.
//-----------------------------------------------------------------------------
inline NData ToNN(CFMutableDataRef cfData)
{
	return ToNN(CFDataRef(cfData));
}





//=============================================================================
//		ToCF : Convert CFDateRef to NDate.
//-----------------------------------------------------------------------------
inline NDate ToNN(CFDateRef cfDate)
{
	NCFDate theDate;

	bool wasOK = theDate.SetObject(cfDate);
	NN_REQUIRE(wasOK);

	return theDate.GetDate();
}





//=============================================================================
//		ToCF : Convert CFDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFDictionaryRef cfDictionary)
{
	NCFDictionary theDictionary;

	bool wasOK = theDictionary.SetObject(cfDictionary);
	NN_REQUIRE(wasOK);

	return theDictionary.GetDictionary();
}





//=============================================================================
//		ToCF : Convert CFMutableDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFMutableDictionaryRef cfDictionary)
{
	return ToNN(CFDictionaryRef(cfData));
}





//=============================================================================
//		ToCF : Convert CFNumberRef to NNumber.
//-----------------------------------------------------------------------------
inline NNumber ToNN(CFNumberRef cfNumber)
{
	NCFNumber theNumber;

	bool wasOK = theNumber.SetObject(cfNumber);
	NN_REQUIRE(wasOK);

	return theNumber.GetNumber();
}





//=============================================================================
//		ToCF : Convert CFRange to NRange.
//-----------------------------------------------------------------------------
inline NRange ToNN(const CFRange& cfRange)
{
	return NRange(size_t(cfRange.location), size_t(cfRange.length));
}





//=============================================================================
//		ToCF : Convert CFStringRef to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(CFStringRef cfString)
{
	NCFString theString;

	bool wasOK = theString.SetObject(cfString);
	NN_REQUIRE(wasOK);

	return theString.GetString();
}





//=============================================================================
//		ToCF : Convert CFMutableStringRef to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(CFMutableStringRef cfString)
{
	return ToNN(CFStringRef(cfData));
}





//=============================================================================
//		ToCF : Convert CFURLRef to NURL.
//-----------------------------------------------------------------------------
inline NURL ToNN(CFURLRef cfURL)
{
	NCFURL theURL;

	bool wasOK = theURL.SetObject(cfURL);
	NN_REQUIRE(wasOK);

	return theURL.GetURL();
}
