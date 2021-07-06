/*	NAME:
		NCoreFoundation.cpp

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
#include "NCoreFoundation.h"





//=============================================================================
//		ToCF : Convert a Nano object to a CF object.
//-----------------------------------------------------------------------------
NCFType ToCF(const NAny& theValue)
{


	// Convert the object
	NNumber theNumber;
	NCFType cfObject;

	if (theValue.IsBool())
	{
		cfObject.Set(theValue.GetBool() ? kCFBooleanTrue : kCFBooleanFalse);
	}

	else if (theValue.Is<NArray>())
	{
		cfObject.Set(ToCF(theValue.Get<NArray>()));
	}

	else if (theValue.IsData())
	{
		cfObject.Set(ToCF(theValue.Get<NData>()));
	}

	else if (theValue.Is<NDate>())
	{
		cfObject.Set(ToCF(theValue.Get<NDate>()));
	}

	else if (theValue.Is<NDictionary>())
	{
		cfObject.Set(ToCF(theValue.Get<NDictionary>()));
	}

	else if (theValue.IsString())
	{
		cfObject.Set(ToCF(theValue.GetString()));
	}

	else if (theValue.Is<NURL>())
	{
		cfObject.Set(ToCF(theValue.Get<NURL>()));
	}

	else if (theNumber.SetValue(theValue))
	{
		cfObject.Set(ToCF(theNumber));
	}

	else
	{
		NN_LOG_ERROR("Unable to convert Nano object to CF!");
	}

	return cfObject;
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
		CFTypeID cfType = CFGetTypeID(cfObject);

		if (cfType == CFBooleanGetTypeID())
		{
			theObject = CFBooleanGetValue(CFBooleanRef(cfObject)) ? true : false;
		}

		else if (cfType == CFArrayGetTypeID())
		{
			NCFArray cfArray;

			cfArray.Set(CFArrayRef(cfObject));
			theObject = cfArray.GetArray();
		}

		else if (cfType == CFDataGetTypeID())
		{
			NCFData cfData;

			cfData.Set(CFDataRef(cfObject));
			theObject = cfData.GetData();
		}

		else if (cfType == CFDateGetTypeID())
		{
			NCFDate cfDate;

			cfDate.Set(CFDateRef(cfObject));
			theObject = cfDate.GetDate();
		}

		else if (cfType == CFDictionaryGetTypeID())
		{
			NCFDictionary cfDictionary;

			cfDictionary.Set(CFDictionaryRef(cfObject));
			theObject = cfDictionary.GetDictionary();
		}

		else if (cfType == CFNumberGetTypeID())
		{
			NCFNumber cfNumber;

			cfNumber.Set(CFNumberRef(cfObject));
			theObject = cfNumber.GetNumber();
		}

		else if (cfType == CFStringGetTypeID())
		{
			NCFString cfString;

			cfString.Set(CFStringRef(cfObject));
			theObject = cfString.GetString();
		}

		else if (cfType == CFURLGetTypeID())
		{
			NCFURL cfURL;

			cfURL.Set(CFURLRef(cfObject));
			theObject = cfURL.GetURL();
		}
		else
		{
			NN_LOG_ERROR("Unable to convert CF object to Nano!");
		}
	}

	return theObject;
}
