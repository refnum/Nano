/*	NAME:
		NArray.inl

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
// Nano
#include "NDebug.h"





//=============================================================================
//		NArray::NArray : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NArray::NArray(const std::vector<T>& theValues)
{


	// Initialise ourselves
	SetVector(theValues);
}





//=============================================================================
//		NArray::NArray : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NArray::NArray(std::initializer_list<T> theValues)
{


	// Initialise ourselves
	reserve(theValues.size());

	for (const auto& theValue : theValues)
	{
		emplace_back(theValue);
	}
}





//=============================================================================
//		NArray::Clear : Clear the array.
//-----------------------------------------------------------------------------
inline void NArray::Clear()
{


	// Clear the array
	clear();
}





//=============================================================================
//		NArray::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline size_t NArray::GetSize() const
{


	// Get the size
	return size();
}





//=============================================================================
//		NArray::GetVector : Get a std::vector.
//-----------------------------------------------------------------------------
template<typename T>
std::vector<T> NArray::GetVector() const
{


	// Get the values
	std::vector<T> theResult(size());

	for (size_t n = 0; n < size(); n++)
	{
		const auto& theValue = at(n);
		theResult[n]         = theValue.Get<T>();
	}

	return theResult;
}





//=============================================================================
//		NArray::SetVector : Set a std::vector.
//-----------------------------------------------------------------------------
template<typename T>
void NArray::SetVector(const std::vector<T>& theValues)
{


	// Set the values
	resize(theValues.size());

	for (size_t n = 0; n < theValues.size(); n++)
	{
		at(n) = theValues[n];
	}
}





#pragma mark private
//=============================================================================
//		NArray::GetValue : Get a typed value.
//-----------------------------------------------------------------------------
template<typename T>
T NArray::GetValue(size_t theIndex, const NString& theType) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());
	NN_REQUIRE(!theType.IsEmpty());

	NN_UNUSED(theType);



	// Get the value
	const auto& theValue = at(theIndex);

	if (theValue.Has<T>())
	{
		return theValue.Get<T>();
	}

	NN_LOG_WARNING("Unable to convert [{}] to {}", theIndex, theType);
	return {};
}
