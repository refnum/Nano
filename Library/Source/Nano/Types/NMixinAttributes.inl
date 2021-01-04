/*	NAME:
		NMixinAttributes.inl

	DESCRIPTION:
		Mix-in for objects with attributes.

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
//		NMixinAttributes::NMixinAttributes : Constructor.
//-----------------------------------------------------------------------------
template<class T, typename A>
NMixinAttributes<T, A>::NMixinAttributes()
	: mAttributes(0)
{
}





//=============================================================================
//		NMixinAttributes::HasAttribute : Is an attribute set?
//-----------------------------------------------------------------------------
template<class T, typename A>
bool NMixinAttributes<T, A>::HasAttribute(A theAttribute) const
{


	// Check our state
	return (mAttributes & theAttribute) != 0;
}





//=============================================================================
//		NMixinAttributes::ClearAttributes : Clear attributes.
//-----------------------------------------------------------------------------
template<class T, typename A>
void NMixinAttributes<T, A>::ClearAttributes(A theAttributes)
{


	// Clear the attributes
	SetAttributes(0, theAttributes);
}





//=============================================================================
//		NMixinAttributes::GetAttributes : Get the attributes.
//-----------------------------------------------------------------------------
template<class T, typename A>
A NMixinAttributes<T, A>::GetAttributes() const
{


	// Get the attributes
	return mAttributes;
}





//=============================================================================
//		NMixinAttributes::SetAttributes : Set the attributes.
//-----------------------------------------------------------------------------
template<class T, typename A>
void NMixinAttributes<T, A>::SetAttributes(A setThese, A clearThese)
{


	// Set the attributes
	mAttributes &= ~clearThese;
	mAttributes |= setThese;
}
