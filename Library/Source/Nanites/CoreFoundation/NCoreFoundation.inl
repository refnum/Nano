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
inline CFArrayRef ToCF(const NArray& theArray)
{
	return CFArrayRef(ToNS(theArray));
}





//=============================================================================
//		ToCF : Convert NData to CFDataRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
inline CFDataRef ToCF(const NData& theData)
{
	return CFDataRef(ToNS(theData));
}





//=============================================================================
//		ToCF : Convert NDate to CFDateRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
inline CFDateRef ToCF(const NDate& theDate)
{
	return CFDateRef(ToNS(theDate));
}





//=============================================================================
//		ToCF : Convert NDictionary to CFDictionaryRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
inline CFDictionaryRef ToCF(const NDictionary& theDictionary)
{
	return CFDictionaryRef(ToNS(theDictionary));
}





//=============================================================================
//		ToCF : Convert NNumber to CFNumberRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
inline CFNumberRef ToCF(const NNumber& theNumber)
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
inline CFStringRef ToCF(const NString& theString)
{
	return CFStringRef(ToNS(theString));
}





//=============================================================================
//		ToCF : Convert a Nano object to a CF object.
//-----------------------------------------------------------------------------
NCFType ToCF(const NAny& theValue)
{


	// Convert the object
	NNumber theNumber;
	NCFType cfObject;

	if (theValue.HasBool())
	{
		cfObject.Set(theValue.GetBool() ? kCFBooleanTrue : kCFBooleanFalse);
	}
	else if (theValue.HasData())
	{
		cfObject.Set(ToCF(theValue.GetData()));
	}
	else if (theValue.HasString())
	{
		cfObject.Set(ToCF(theValue.GetString()));
	}
	else if (theNumber.SetValue(theValue))
	{
		cfObject.Set(ToCF(theNumber));
	}
	else if (theValue.Has<NArray>())
	{
		cfObject.Set(ToCF(theValue.Get<NArray>()));
	}
	else if (theValue.Has<NDate>())
	{
		cfObject.Set(ToCF(theValue.Get<NDate>()));
	}
	else if (theValue.Has<NDictionary>())
	{
		cfObject.Set(ToCF(theValue.Get<NDictionary>()));
	}
	else if (theValue.Has<NURL>())
	{
		cfObject.Set(ToCF(theValue.Get<NURL>()));
	}
	else
	{
		NN_LOG_ERROR("Unable to convert Nano object to CF!");
	}

	return cfObject;
}





//=============================================================================
//		ToCF : Convert NURL to CFURLRef.
//-----------------------------------------------------------------------------
//		Note :	As CF objects are toll-free bridged to NS objects, converting
//				to NS then casting ensures the CF object is auto-released.
//----------------------------------------------------------------------------
inline CFURLRef ToCF(const NURL& theURL)
{
	return CFURLRef(ToNS(theURL));
}





//=============================================================================
//		ToCF : Convert a CF object to a Nano object.
//-----------------------------------------------------------------------------
NAny ToNN(CFTypeRef cfObject)
{


	// Convert the object
	NAny theObject;

	if (cfObject != nullptr)
	{
		CFTypeID cfType = CFGetTypeID(cfOnject);

		if (cfType == CFBooleanGetTypeID())
		{
			theObject = CFBooleanGetValue(cfObject) ? true : false;
		}
		else if (cfType == CFNumberGetTypeID())
		{
			theObject = NCFNumber(CFNumberRef(cfObject)).GetNumber();
		}
		else if (cfType == CFStringGetTypeID())
		{
			theObject = NCFString(CFStringRef(cfObject)).GetString();
		}
		else if (cfType == CFDataGetTypeID())
		{
			theObject = NCFData(CFDataRef(cfObject)).GetData();
		}
		else if (cfType == CFDateGetTypeID())
		{
			theObject = NCFDate(CFDateRef(cfObject)).GetDate();
		}
		else if (cfType == CFArrayGetTypeID())
		{
			theObject = NCFArray(CFArrayRef(cfObject)).GetArray();
		}
		else if (cfType == CFDictionaryGetTypeID())
		{
			theObject = NCFDictionary(CFDictionaryRef(cfObject)).GetDictionary();
		}
		else if (cfType == CFURLGetTypeID())
		{
			theObject = NCFURL(CFURLRef(cfObject)).GetURL();
		}
		else
		{
			NN_LOG_ERROR("Unable to convert CF object to Nano!");
		}
	}

	return theObject;
}





//=============================================================================
//		ToNN : Convert CFArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFArrayRef cfArray)
{
	NCFArray theArray;

	bool wasOK = theArray.SetObject(cfArray);
	NN_REQUIRE(wasOK);

	return theArray.GetArray();
}





//=============================================================================
//		ToNN : Convert CFMutableArrayRef to NArray.
//-----------------------------------------------------------------------------
inline NArray ToNN(CFMutableArrayRef cfArray)
{
	return ToNN(CFArrayRef(cfData));
}





//=============================================================================
//		ToNN : Convert CFDataRef to NData.
//-----------------------------------------------------------------------------
inline NData ToNN(CFDataRef cfData)
{
	NCFData theData;

	bool wasOK = theData.SetObject(cfData);
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

	bool wasOK = theDate.SetObject(cfDate);
	NN_REQUIRE(wasOK);

	return theDate.GetDate();
}





//=============================================================================
//		ToNN : Convert CFDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFDictionaryRef cfDictionary)
{
	NCFDictionary theDictionary;

	bool wasOK = theDictionary.SetObject(cfDictionary);
	NN_REQUIRE(wasOK);

	return theDictionary.GetDictionary();
}





//=============================================================================
//		ToNN : Convert CFMutableDictionaryRef to NDictionary.
//-----------------------------------------------------------------------------
inline NDictionary ToNN(CFMutableDictionaryRef cfDictionary)
{
	return ToNN(CFDictionaryRef(cfData));
}





//=============================================================================
//		ToNN : Convert CFNumberRef to NNumber.
//-----------------------------------------------------------------------------
inline NNumber ToNN(CFNumberRef cfNumber)
{
	NCFNumber theNumber;

	bool wasOK = theNumber.SetObject(cfNumber);
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

	bool wasOK = theString.SetObject(cfString);
	NN_REQUIRE(wasOK);

	return theString.GetString();
}





//=============================================================================
//		ToNN : Convert CFMutableStringRef to NString.
//-----------------------------------------------------------------------------
inline NString ToNN(CFMutableStringRef cfString)
{
	return ToNN(CFStringRef(cfData));
}





//=============================================================================
//		ToNN : Convert CFURLRef to NURL.
//-----------------------------------------------------------------------------
inline NURL ToNN(CFURLRef cfURL)
{
	NCFURL theURL;

	bool wasOK = theURL.SetObject(cfURL);
	NN_REQUIRE(wasOK);

	return theURL.GetURL();
}
