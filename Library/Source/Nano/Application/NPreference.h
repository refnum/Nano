/*	NAME:
		NPreference.h

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
#ifndef NPREFERENCE_H
#define NPREFERENCE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NAtomic.h"
#include "NReceiver.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NString;
class NAny;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename T>
class NPreference : public NReceiver
{
public:
										NPreference(const NString& theKey);


	// Get the key
	NString                             GetKey() const;


	// Does the preference exist?
	bool                                Exists() const;


	// Get / set the value
	T                                   Get() const;
	void                                Set(  const T& theValue) const;


private:
	void                                ValueChanged(const NAny& theValue);

private:
	NAtomic<T>                          mValue;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NPreference.inl"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
using NPreferenceBool       = NPreference<bool>;
using NPreferenceUInt32     = NPreference<uint32_t>;
using NPreferenceUInt64     = NPreference<uint64_t>;
using NPreferenceInt32      = NPreference<int32_t>;
using NPreferenceInt64      = NPreference<int64_t>;
using NPreferenceFloat32    = NPreference<float32_t>;
using NPreferenceFloat64    = NPreference<float64_t>;
using NPreferenceArray      = NPreference<NArray>;
using NPreferenceData       = NPreference<NData>;
using NPreferenceDictionary = NPreference<NDictionary>;
using NPreferenceNumber     = NPreference<NNumber>;
using NPreferenceString     = NPreference<NString>;
using NPreferenceTime       = NPreference<NTime>;



#endif // NPREFERENCE_H
