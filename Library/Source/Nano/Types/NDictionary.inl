/*	NAME:
		NDictionary.inl

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





//=============================================================================
//		NDictionary::Clear : Clear the dictionary.
//-----------------------------------------------------------------------------
inline void NDictionary::Clear()
{


	// Clear the dictionary
	clear();
}





//=============================================================================
//		NDictionary::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline size_t NDictionary::GetSize() const
{


	// Get the size
	return size();
}





//=============================================================================
//		NDictionary::GetBool : Get a bool value.
//-----------------------------------------------------------------------------
inline bool NDictionary::GetBool(const NString& theKey) const
{


	// Get the value
	return GetValue<bool>(theKey);
}





//=============================================================================
//		NDictionary::GetUInt32 : Get a uint32_t value.
//-----------------------------------------------------------------------------
inline uint32_t NDictionary::GetUInt32(const NString& theKey) const
{


	// Get the value
	return GetValue<uint32_t>(theKey);
}





//=============================================================================
//		NDictionary::GetUInt64 : Get a uint64_t value.
//-----------------------------------------------------------------------------
inline uint64_t NDictionary::GetUInt64(const NString& theKey) const
{


	// Get the value
	return GetValue<uint64_t>(theKey);
}





//=============================================================================
//		NDictionary::GetInt32 : Get an int32_t value.
//-----------------------------------------------------------------------------
inline int32_t NDictionary::GetInt32(const NString& theKey) const
{


	// Get the value
	return GetValue<int32_t>(theKey);
}





//=============================================================================
//		NDictionary::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
inline int64_t NDictionary::GetInt64(const NString& theKey) const
{


	// Get the value
	return GetValue<int64_t>(theKey);
}





//=============================================================================
//		NDictionary::GetFloat32 : Get a float32_t value.
//-----------------------------------------------------------------------------
inline float32_t NDictionary::GetFloat32(const NString& theKey) const
{


	// Get the value
	return GetValue<float32_t>(theKey);
}





//=============================================================================
//		NDictionary::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
inline float64_t NDictionary::GetFloat64(const NString& theKey) const
{


	// Get the value
	return GetValue<float64_t>(theKey);
}





#pragma mark private
//=============================================================================
//		NDictionary::GetValue : Get a typed value.
//-----------------------------------------------------------------------------
template<typename T>
T NDictionary::GetValue(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	auto theIter = find(theKey);
	if (theIter != end())
	{
		const auto& theValue = theIter->second;
		NN_REQUIRE(theValue.Is<T>());

		return theValue.Get<T>();
	}

	return {};
}
