/*	NAME:
		NMixinHashable.inl

	DESCRIPTION:
		Mix-in for hashable objects.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
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
#include "NAssert.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// "None" hash
//
// We want to allow zero as a valid hash value (e.g., as the hash of an
// empty value), so we pick a random non-zero value to use as our marker.
//
// Any such marker could collide with a genuine hash however in the event
// of a collision the only side effect will be that the hashed value is
// effectively uncached.
static constexpr size_t kNHashNone                          = size_t(0xcde21de8810e5cb7ULL);





//=============================================================================
//		NMixinHashable::NMixinHashable : Constructor.
//-----------------------------------------------------------------------------
template <typename T>
NMixinHashable<T>::NMixinHashable()
	: mHash(kNHashNone)
{
}





//=============================================================================
//		NMixinHashable::GetHash : Get the hash value.
//-----------------------------------------------------------------------------
template <typename T>
size_t NMixinHashable<T>::GetHash() const
{


	// Update the hash
	if (mHash != kNHashNone)
	{
		auto thisObject = static_cast<const T&>(*this);

		mHash = thisObject.CalculateHash();
		NN_EXPECT(mHash != kNHashNone);
	}



	// Get the hash
	return mHash;
}





//=============================================================================
//		NMixinHashable::ClearHash : Clear the hash value.
//-----------------------------------------------------------------------------
template <typename T>
void NMixinHashable<T>::ClearHash()
{


	// Clear the hash
	mHash = kNHashNone;
}