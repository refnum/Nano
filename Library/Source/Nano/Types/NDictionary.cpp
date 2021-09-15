/*	NAME:
		NDictionary.cpp

	DESCRIPTION:
		Dictionary object.

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
#include "NDictionary.h"

// Nano
#include "NArray.h"
#include "NData.h"
#include "NFormat.h"
#include "NNumber.h"
#include "NRange.h"
#include "NStdAlgorithm.h"
#include "NTime.h"





//=============================================================================
//		NDictionary::HasKey : Is a key present?
//-----------------------------------------------------------------------------
bool NDictionary::HasKey(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());


	// Check the key
	return nstd::contains(*this, theKey);
}





//=============================================================================
//		NDictionary::RemoveKey : Remove a key.
//-----------------------------------------------------------------------------
void NDictionary::RemoveKey(const NString& theKey)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Remove the key
	erase(theKey);
}





//=============================================================================
//		NDictionary::GetKeys : Get the keys.
//-----------------------------------------------------------------------------
NVectorString NDictionary::GetKeys() const
{


	// Get the keys
	NVectorString theKeys;

	for (const auto& [theKey, theValue] : *this)
	{
		NN_UNUSED(theValue);
		theKeys.emplace_back(theKey);
	}

	return theKeys;
}





//=============================================================================
//		NDictionary::GetValues : Get the values.
//-----------------------------------------------------------------------------
NVectorAny NDictionary::GetValues() const
{


	// Get the keys
	NVectorAny theValues;

	for (const auto& [theKey, theValue] : *this)
	{
		NN_UNUSED(theKey);
		theValues.emplace_back(theValue);
	}

	return theValues;
}





//=============================================================================
//		NDictionary::Invert : Invert the dictinary.
//-----------------------------------------------------------------------------
bool NDictionary::Invert()
{


	// Get the state we need
	NDictionary theResult;
	bool        canInvert = true;



	// Invert the keys / values
	for (const auto& [theKey, theValue] : *this)
	{
		canInvert = theValue.IsString();
		if (canInvert)
		{
			NString newKey = theValue.GetString();
			canInvert      = !newKey.IsEmpty() && !theResult.HasKey(newKey);

			if (canInvert)
			{
				theResult[newKey] = theKey;
			}
		}

		if (!canInvert)
		{
			break;
		}
	}



	// Replace our content
	if (canInvert)
	{
		*this = theResult;
	}

	return canInvert;
}





//=============================================================================
//		NDictionary::GetArray : Get an NArray value.
//-----------------------------------------------------------------------------
NArray NDictionary::GetArray(const NString& theKey) const
{


	// Get the value
	return GetValue<NArray>(theKey);
}





//=============================================================================
//		NDictionary::GetData : Get an NData value.
//-----------------------------------------------------------------------------
NData NDictionary::GetData(const NString& theKey) const
{


	// Get the value
	return GetValue<NData>(theKey);
}





//=============================================================================
//		NDictionary::GetDictionary : Get an NDictionary value.
//-----------------------------------------------------------------------------
NDictionary NDictionary::GetDictionary(const NString& theKey) const
{


	// Get the value
	return GetValue<NDictionary>(theKey);
}





//=============================================================================
//		NDictionary::GetNumber : Get an NNumber value.
//-----------------------------------------------------------------------------
NNumber NDictionary::GetNumber(const NString& theKey) const
{


	// Get the value
	return GetValue<NNumber>(theKey);
}





//=============================================================================
//		NDictionary::GetString : Get an NString value.
//-----------------------------------------------------------------------------
NString NDictionary::GetString(const NString& theKey) const
{


	// Get the value
	return GetValue<NString>(theKey);
}





//=============================================================================
//		NDictionary::GetTime : Get an NTime value.
//-----------------------------------------------------------------------------
NTime NDictionary::GetTime(const NString& theKey) const
{


	// Get the value
	return GetValue<NTime>(theKey);
}





#pragma mark NMixinComparable
//=============================================================================
//		NDictionary::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NDictionary::CompareEqual(const NDictionary& theDictionary) const
{


	// Compare the size
	//
	// A different size means no equality.
	if (GetSize() != theDictionary.GetSize())
	{
		return false;
	}



	// Compare the values
	return CompareOrder(theDictionary) == NComparison::EqualTo;
}





//=============================================================================
//		NDictionary::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NDictionary::CompareOrder(const NDictionary& theDictionary) const
{


	// Compare by size
	NVectorString keysA = GetKeys();
	NVectorString keysB = theDictionary.GetKeys();

	NComparison theResult = NCompare(keysA.size(), keysB.size());



	// Compare by keys
	if (theResult == NComparison::EqualTo)
	{
		nstd::sort(keysA);
		nstd::sort(keysB);

		theResult = NCompare(keysA, keysB);
	}



	// Compare by values
	if (theResult == NComparison::EqualTo)
	{
		for (const auto& theKey : keysA)
		{
			const auto& valueA = at(theKey);
			const auto& valueB = theDictionary.at(theKey);

			theResult = NCompare(valueA, valueB);
			if (theResult != NComparison::EqualTo)
			{
				break;
			}
		}
	}

	return theResult;
}
