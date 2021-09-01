/*	NAME:
		NPreference.inl

	DESCRIPTION:
		Preference value.

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
#include "NAny.h"
#include "NBroadcast.h"
#include "NBroadcaster.h"
#include "NPreferences.h"





//=============================================================================
//		NPreference::NPreference : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NPreference<T>::NPreference(const NString& theKey)
	: mValue()
{


	// Register for changes
	StartReceiving(NPreferences::GetChangeBroadcast(theKey),
	[=](const NBroadcast& theBroadcast)
	{
		ValueChanged(theBroadcast.GetValue());
	});

	ValueChanged(NPreferences::GetValue(theKey));
}





//=============================================================================
//		NPreference::GetKey : Get the key.
//-----------------------------------------------------------------------------
template<typename T>
NString NPreference<T>::GetKey() const
{


	// Get the key
	//
	// To avoid having to store the key with each preference object
	// we recover it from our change broadcast.
	auto theBroadcasts = GetBroadcasts();
	NN_REQUIRE(theBroadcasts.size() == 1);

	return NPreferences::GetChangeKey(theBroadcasts[0]);
}





//=============================================================================
//		NPreference::Exists : Does the preference exist?
//-----------------------------------------------------------------------------
template<typename T>
bool NPreference<T>::Exists() const
{


	// Check the preferences
	return NPreferences::HasKey(GetKey());
}





//=============================================================================
//		NPreference::Get : Get the value.
//-----------------------------------------------------------------------------
template<typename T>
T NPreference<T>::Get() const
{


	// Get the value
	return mValue;
}





//=============================================================================
//		NPreference::Set : Set the value.
//-----------------------------------------------------------------------------
template<typename T>
void NPreference<T>::Set(const T& theValue) const
{


	// Set the value
	NPreferences::SetValue(GetKey(), theValue);
}





#pragma mark private
//=============================================================================
//		NPreference::ValueChanged : Handle updates.
//-----------------------------------------------------------------------------
template<typename T>
void NPreference<T>::ValueChanged(const NAny& theValue)
{


	// Validate our parameters
	NN_REQUIRE(theValue.IsEmpty() || theValue.IsNumber() || theValue.Is<T>());



	// Update the value
	if constexpr (std::is_arithmetic_v<T>)
	{
		NNumber theNumber = theValue.GetNumber();

		if constexpr (std::is_integral_v<T>)
		{
			if constexpr (std::is_unsigned_v<T>)
			{
				mValue = T(theNumber.GetUInt64());
			}
			else
			{
				mValue = T(theNumber.GetInt64());
			}
		}
		else
		{
			mValue = T(theNumber.GetFloat64());
		}
	}
	else if (theValue.IsEmpty())
	{
		mValue = T{};
	}
	else
	{
		mValue = theValue.Get<T>();
	}
}
