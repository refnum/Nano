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
//		ToCF : Convert NRange to CFRange.
//----------------------------------------------------------------------------
inline CFRange ToCF(const NRange& theRange)
{
	return CFRangeMake(theRange.GetLocation(), theRange.GetSize());
}





//=============================================================================
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
//		ToCF : Convert CFRange to NRange.
//-----------------------------------------------------------------------------
inline NRange ToNN(const CFRange& theRange)
{
	return NRange(size_t(theRange.location), size_t(theRange.length));
}





//=============================================================================
//		ToCF : Convert CFArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFArrayRef theArray)
{
	return NCFArray(theArray, false);
}





//=============================================================================
//		ToCF : Convert CFDataRef to NData.
//-----------------------------------------------------------------------------
inline NData ToNN(CFDataRef theData)
{
	return NCFData(theData, false);
}





//=============================================================================
//		ToCF : Convert CFDateRef to NDate.
//-----------------------------------------------------------------------------
inline NDate ToNN(CFDateRef theDate)
{
	return NCFDate(theDate, false);
}





//=============================================================================
//		ToCF : Convert CFDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFDictionaryRef theDictionary)
{
	return NCFDictionary(theDictionary, false);
}





//=============================================================================
//		ToCF : Convert CFNumberRef to NNumber.
//-----------------------------------------------------------------------------
inline NNumber ToNN(CFNumberRef theNumber)
{
	return NCFNumber(theNumber, false);
}





//=============================================================================
//		ToCF : Convert CFStringRef to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(CFStringRef theString)
{
	return NCFString(theString, false);
}





//=============================================================================
//		ToCF : Convert CFURLRef to NURL.
//-----------------------------------------------------------------------------
inline NURL ToNN(CFURLRef theURL)
{
	return NCFURL(theURL, false);
}





//=============================================================================
//		ToCF : Convert CFMutableArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFMutableArrayRef theArray)
{
	return NCFArray(theArray, false);
}





//=============================================================================
//		ToCF : Convert CFMutableDataRef to NData.
//-----------------------------------------------------------------------------
inline NData ToNN(CFMutableDataRef theData)
{
	return NCFData(theData, false);
}





//=============================================================================
//		ToCF : Convert CFMutableDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFMutableDictionaryRef theDictionary)
{
	return NCFDictionary(theDictionary, false);
}





//=============================================================================
//		ToCF : Convert CFMutableStringRef to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(CFMutableStringRef theString)
{
	return NCFString(theString, false);
}
