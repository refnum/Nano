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
//		NDictionary::RemoveKeys : Remove keys.
//-----------------------------------------------------------------------------
void NDictionary::RemoveKeys(const NVectorString& theKeys)
{


	// Validate our parameters
	NN_REQUIRE(!theKeys.empty());



	// Remove the keys
	for (const auto& theKey : theKeys)
	{
		NN_REQUIRE(!theKey.IsEmpty());
		erase(theKey);
	}
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
		canInvert = theValue.Has<NString>();
		if (canInvert)
		{
			NString newKey = theValue.Get<NString>();
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
//		NDictionary::GetBool : Get a bool value.
//-----------------------------------------------------------------------------
bool NDictionary::GetBool(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	NNumber theResult;

	auto theIter = find(theKey);
	if (theIter != end())
	{
		if (!theResult.SetValue(theIter->second))
		{
			NN_LOG_WARNING("Unable to convert [{}] to bool", theKey);
		}
	}

	return theResult.GetBool();
}





//=============================================================================
//		NDictionary::GetInt32 : Get an int32_t value.
//-----------------------------------------------------------------------------
int32_t NDictionary::GetInt32(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	NNumber theResult;

	auto theIter = find(theKey);
	if (theIter != end())
	{
		if (!theResult.SetValue(theIter->second))
		{
			NN_LOG_WARNING("Unable to convert [{}] to int32_t", theKey);
		}
	}

	return theResult.GetInt32();
}





//=============================================================================
//		NDictionary::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
int64_t NDictionary::GetInt64(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	NNumber theResult;

	auto theIter = find(theKey);
	if (theIter != end())
	{
		if (!theResult.SetValue(theIter->second))
		{
			NN_LOG_WARNING("Unable to convert [{}] to int64_t", theKey);
		}
	}

	return theResult.GetInt64();
}





//=============================================================================
//		NDictionary::GetFloat32 : Get a float32_t value.
//-----------------------------------------------------------------------------
float32_t NDictionary::GetFloat32(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	NNumber theResult;

	auto theIter = find(theKey);
	if (theIter != end())
	{
		if (!theResult.SetValue(theIter->second))
		{
			NN_LOG_WARNING("Unable to convert [{}] to float32_t", theKey);
		}
	}

	return theResult.GetFloat32();
}





//=============================================================================
//		NDictionary::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
float64_t NDictionary::GetFloat64(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	NNumber theResult;

	auto theIter = find(theKey);
	if (theIter != end())
	{
		if (!theResult.SetValue(theIter->second))
		{
			NN_LOG_WARNING("Unable to convert [{}] to float64_t", theKey);
		}
	}

	return theResult.GetFloat64();
}





//=============================================================================
//		NDictionary::GetData : Get an NData value.
//-----------------------------------------------------------------------------
NData NDictionary::GetData(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	auto theIter = find(theKey);
	if (theIter != end())
	{
		const auto& theValue = theIter->second;

		if (theValue.HasData())
		{
			return theValue.GetData();
		}
	}

	NN_LOG_WARNING("Unable to convert [{}] to NData", theKey);
	return {};
}





//=============================================================================
//		NDictionary::GetDictionary : Get an NDictionary value.
//-----------------------------------------------------------------------------
NDictionary NDictionary::GetDictionary(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	auto theIter = find(theKey);
	if (theIter != end())
	{
		const auto& theValue = theIter->second;

		if (theValue.Has<NDictionary>())
		{
			return theValue.Get<NDictionary>();
		}
	}

	NN_LOG_WARNING("Unable to convert [{}] to NDictionary", theKey);
	return {};
}





//=============================================================================
//		NDictionary::GetString : Get an NString value.
//-----------------------------------------------------------------------------
NString NDictionary::GetString(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	auto theIter = find(theKey);
	if (theIter != end())
	{
		const auto& theValue = theIter->second;

		if (theValue.HasString())
		{
			return theValue.GetString();
		}
	}

	NN_LOG_WARNING("Unable to convert [{}] to NString", theKey);
	return {};
}





//=============================================================================
//		NDictionary::GetTime : Get an NTime value.
//-----------------------------------------------------------------------------
NTime NDictionary::GetTime(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	auto theIter = find(theKey);
	if (theIter != end())
	{
		const auto& theValue = theIter->second;

		if (theValue.HasTime())
		{
			return theValue.GetTime();
		}
	}

	NN_LOG_WARNING("Unable to convert [{}] to NTime", theKey);
	return {};
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
