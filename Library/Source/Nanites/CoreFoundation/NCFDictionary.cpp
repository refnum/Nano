/*	NAME:
		NCFDictionary.cpp

	DESCRIPTION:
		CFDictionaryRef wrapper.

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
#include "NCFDictionary.h"

// Nano
#include "NCoreFoundation.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		InsertValue : Insert a value.
//----------------------------------------------------------------------------
static void InsertValue(const void* keyPtr, const void* valuePtr, void* userData)
{


	// Get the state we need
	NCFDictionary* thisPtr = reinterpret_cast<NCFDictionary*>(userData);
	CFTypeRef      cfValue = reinterpret_cast<CFTypeRef>(valuePtr);
	CFStringRef    cfKey   = reinterpret_cast<CFStringRef>(keyPtr);



	// Insert the value


	NVariant  theValue;
	NCFString theKey;



	// Insert the value
	NString theKey   = ToNN(cfKey);
	NAny    theValue = ToNN(cfValue);

	thisPtr->insert({theKey, theValue});
}





//=============================================================================
//		NCFDictionary::GetDictionary : Get the dictionary.
//-----------------------------------------------------------------------------
NDictionary NCFDictionary::GetDictionary() const
{


	// Get the dictionary
	CFDictionaryRef cfDictionary = *this;
	NDictionary     theDictionary;

	if (cfDictionary != nullptr)
	{
		CFDictionaryApplyFunction(cfDictionary, InsertValue, &theDictionary);

		NN_REQUIRE(size_t(CFDictionaryGetCount(cfDictionary)) == theDictionary.size());
	}

	return theDictionary;
}





//=============================================================================
//		NCFDictionary::SetDictionary : Set the dictionary.
//-----------------------------------------------------------------------------
bool NCFDictionary::SetDictionary(const NDictionary& theDictionary)
{


	// Set the dictionary
	CFIndex numItems = CFIndex(theDictionary.size());
	bool    wasOK    = Set(CFDictionaryCreateMutable(kCFAllocatorDefault,
											   numItems,
											   &kCFTypeDictionaryKeyCallBacks,
											   &kCFTypeDictionaryValueCallBacks));

	if (wasOK)
	{
		CFDictionaryRef cfDictionary = *this;

		for (const auto& keyValue : theDictionary)
		{
			CFStringRef cfKey   = ToCF(keyValue->first);
			NCFType     cfValue = ToCF(keyValue->second);

			CFDictionarySetValue(cfDictionary, cfKey, cfValue);
		}
	}

	return wasOK;
}
