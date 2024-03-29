/*	NAME:
		NCFArray.cpp

	DESCRIPTION:
		CFArrayRef wrapper.

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
#include "NCFArray.h"

// Nano
#include "NCoreFoundation.h"





//=============================================================================
//		NCFArray::GetArray : Get the array.
//-----------------------------------------------------------------------------
NArray NCFArray::GetArray() const
{


	// Get the array
	CFArrayRef cfArray = *this;
	NArray     theArray;

	if (cfArray != nullptr)
	{
		CFIndex numItems = CFArrayGetCount(cfArray);

		for (CFIndex n = 0; n < numItems; n++)
		{
			theArray.push_back(ToNN(CFTypeRef(CFArrayGetValueAtIndex(cfArray, n))));
		}
	}

	return theArray;
}





//=============================================================================
//		NCFArray::SetArray : Set the array.
//-----------------------------------------------------------------------------
bool NCFArray::SetArray(const NArray& theArray)
{


	// Set the array
	CFIndex numItems = CFIndex(theArray.size());
	bool    wasOK =
		Assign(CFArrayCreateMutable(kCFAllocatorDefault, numItems, &kCFTypeArrayCallBacks));

	if (wasOK)
	{
		CFArrayRef        cfArray        = *this;
		CFMutableArrayRef cfMutableArray = const_cast<CFMutableArrayRef>(cfArray);

		for (const auto& theItem : theArray)
		{
			CFArrayAppendValue(cfMutableArray, CFTypeRef(ToCF(theItem)));
		}
	}

	return wasOK;
}
