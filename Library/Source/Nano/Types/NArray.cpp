/*	NAME:
		NArray.cpp

	DESCRIPTION:
		Array object.

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
#include "NArray.h"

// Nano
#include "NData.h"
#include "NDictionary.h"
#include "NFormat.h"
#include "NNumber.h"
#include "NRange.h"
#include "NStdAlgorithm.h"
#include "NString.h"
#include "NTime.h"





//=============================================================================
//		NArray::HasValue : Is a value present?
//-----------------------------------------------------------------------------
bool NArray::HasValue(const NAny& theValue) const
{


	// Check the value
	return nstd::contains(*this, theValue);
}





//=============================================================================
//		NArray::RemoveValue : Remove a value.
//-----------------------------------------------------------------------------
void NArray::RemoveValue(size_t theIndex)
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Remove the value
	nstd::erase(*this, theIndex, 1);
}





//=============================================================================
//		NArray::RemoveValues : Remove values.
//-----------------------------------------------------------------------------
void NArray::RemoveValues(const NRange& theRange)
{


	// Remove the values
	NRange finalRange = theRange.GetNormalized(GetSize());
	NN_REQUIRE(finalRange.GetFirst() < GetSize());
	NN_REQUIRE(finalRange.GetLast() < GetSize());

	nstd::erase(*this, finalRange.GetFirst(), finalRange.GetSize());
}





//=============================================================================
//		NArray::GetBool : Get a bool value.
//-----------------------------------------------------------------------------
bool NArray::GetBool(size_t theIndex) const
{


	// Get the value
	return GetNumber(theIndex, "bool").GetBool();
}





//=============================================================================
//		NArray::GetUInt32 : Get a uint32_t value.
//-----------------------------------------------------------------------------
uint32_t NArray::GetUInt32(size_t theIndex) const
{


	// Get the value
	return GetNumber(theIndex, "uint32_t").GetUInt32();
}





//=============================================================================
//		NArray::GetUInt64 : Get a uint64_t value.
//-----------------------------------------------------------------------------
uint64_t NArray::GetUInt64(size_t theIndex) const
{


	// Get the value
	return GetNumber(theIndex, "uint64_t").GetUInt64();
}





//=============================================================================
//		NArray::GetInt32 : Get an int32_t value.
//-----------------------------------------------------------------------------
int32_t NArray::GetInt32(size_t theIndex) const
{


	// Get the value
	return GetNumber(theIndex, "int32_t").GetInt32();
}





//=============================================================================
//		NArray::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
int64_t NArray::GetInt64(size_t theIndex) const
{


	// Get the value
	return GetNumber(theIndex, "int64_t").GetInt64();
}





//=============================================================================
//		NArray::GetFloat32 : Get a float32_t value.
//-----------------------------------------------------------------------------
float32_t NArray::GetFloat32(size_t theIndex) const
{


	// Get the value
	return GetNumber(theIndex, "float32_t").GetFloat32();
}





//=============================================================================
//		NArray::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
float64_t NArray::GetFloat64(size_t theIndex) const
{


	// Get the value
	return GetNumber(theIndex, "float64_t").GetFloat64();
}





//=============================================================================
//		NArray::GetArray : Get an NArray value.
//-----------------------------------------------------------------------------
NArray NArray::GetArray(size_t theIndex) const
{


	// Get the value
	return GetValue<NArray>(theIndex, "NArray");
}





//=============================================================================
//		NArray::GetData : Get an NData value.
//-----------------------------------------------------------------------------
NData NArray::GetData(size_t theIndex) const
{


	// Get the value
	return GetValue<NData>(theIndex, "NData");
}





//=============================================================================
//		NArray::GetDictionary : Get an NDictionary value.
//-----------------------------------------------------------------------------
NDictionary NArray::GetDictionary(size_t theIndex) const
{


	// Get the value
	return GetValue<NDictionary>(theIndex, "NDictionary");
}





//=============================================================================
//		NArray::GetString : Get an NString value.
//-----------------------------------------------------------------------------
NString NArray::GetString(size_t theIndex) const
{


	// Get the value
	return GetValue<NString>(theIndex, "NString");
}





//=============================================================================
//		NArray::GetTime : Get an NTime value.
//-----------------------------------------------------------------------------
NTime NArray::GetTime(size_t theIndex) const
{


	// Get the value
	return GetValue<NTime>(theIndex, "NTime");
}





#pragma mark NMixinComparable
//=============================================================================
//		NArray::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NArray::CompareEqual(const NArray& theArray) const
{


	// Compare the size
	//
	// A different size means no equality.
	if (GetSize() != theArray.GetSize())
	{
		return false;
	}



	// Compare the values
	return CompareOrder(theArray) == NComparison::EqualTo;
}





//=============================================================================
//		NArray::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NArray::CompareOrder(const NArray& theArray) const
{


	// Get the state we need
	NComparison theResult = NComparison::EqualTo;
	size_t      sizeA     = GetSize();
	size_t      sizeB     = theArray.GetSize();
	size_t      n         = 0;



	// Compare each value
	while (n < sizeA && n < sizeB)
	{
		const auto& valueA = at(n);
		const auto& valueB = theArray.at(n);

		theResult = NCompare(valueA, valueB);
		if (theResult != NComparison::EqualTo)
		{
			break;
		}

		n++;
	}



	// Compare by size
	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(sizeA, sizeB);
	}

	return theResult;
}





#pragma mark private
//=============================================================================
//		NArray::GetNumber : Get a number value.
//-----------------------------------------------------------------------------
NNumber NArray::GetNumber(size_t theIndex, const NString& theType) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Get the number
	NNumber theResult;

	if (!theResult.SetValue(at(theIndex)))
	{
		NN_LOG_WARNING("Unable to convert [{}] to {}", theIndex, theType);
	}

	return theResult;
}
