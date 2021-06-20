/*	NAME:
		NCoreFoundation.h

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
#ifndef NCORE_FOUNDATION_H
#define NCORE_FOUNDATION_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCFArray.h"
#include "NCFData.h"
#include "NCFDate.h"
#include "NCFDictionary.h"
#include "NCFNumber.h"
#include "NCFString.h"
#include "NCFURL.h"
#include "NRange.h"





//=============================================================================
//		Inline functions
//-----------------------------------------------------------------------------
// Nano to CoreFoundation
CFArrayRef      ToCF(const NArray& theArray);
CFDataRef       ToCF(const NData& theData);
CFDateRef       ToCF(const NDate& theDate);
CFDictionaryRef ToCF(const NDictionary& theDictionary);
CFNumberRef     ToCF(const NNumber& theNumber);
CFRange         ToCF(const NRange& theRange);
CFStringRef     ToCF(const NString& theString);
CFURLRef        ToCF(const NURL& theURL);



// CoreFoundation to Nano
NArray      ToNN(CFArrayRef cfArray);
NArray      ToNN(CFMutableArrayRef cfArray);
NData       ToNN(CFDataRef cfData);
NData       ToNN(CFMutableDataRef cfData);
NDate       ToNN(CFDateRef cfDate);
NDictionary ToNN(CFDictionaryRef cfDictionary);
NDictionary ToNN(CFMutableDictionaryRef cfDictionary);
NNumber     ToNN(CFNumberRef cfNumber);
NRange      ToNN(const CFRange& cfRange);
NString     ToNN(CFMutableStringRef cfString);
NString     ToNN(CFStringRef cfString);
NURL        ToNN(CFURLRef cfURL);





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCoreFoundation.inl"

#endif // NCORE_FOUNDATION_H
