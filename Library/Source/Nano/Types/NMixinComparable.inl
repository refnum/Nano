/*	NAME:
		NMixinComparable.inl

	DESCRIPTION:
		Mix-in for comparable objects.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
//		NMixinComparable::operator == : Equality operator.
//-----------------------------------------------------------------------------
template <class T>
bool NMixinComparable<T>::operator==(const T& theValue) const
{
	return IsEqual(theValue);
}





//=============================================================================
//		NMixinComparable::operator != : Inequality operator.
//-----------------------------------------------------------------------------
template <class T>
bool NMixinComparable<T>::operator!=(const T& theValue) const
{
	return !IsEqual(theValue);
}





//=============================================================================
//		NMixinComparable::operator <= : Less-than-or-equals operator.
//-----------------------------------------------------------------------------
template <class T>
bool NMixinComparable<T>::operator<=(const T& theValue) const
{
	return GetOrder(theValue) != NComparison::GreaterThan;
}





//=============================================================================
//		NMixinComparable::operator < : Less-than operator.
//-----------------------------------------------------------------------------
template <class T>
bool NMixinComparable<T>::operator<(const T& theValue) const
{
	return GetOrder(theValue) == NComparison::LessThan;
}





//=============================================================================
//		NMixinComparable::operator >= : Greater-than-or-equals operator.
//-----------------------------------------------------------------------------
template <class T>
bool NMixinComparable<T>::operator>=(const T& theValue) const
{
	return GetOrder(theValue) != NComparison::LessThan;
}





//=============================================================================
//		NMixinComparable::operator > : Greater-than operator.
//-----------------------------------------------------------------------------
template <class T>
bool NMixinComparable<T>::operator>(const T& theValue) const
{
	return GetOrder(theValue) == NComparison::GreaterThan;
}





#pragma mark private
//=============================================================================
//		NMixinComparable::IsEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
template <typename T>
bool NMixinComparable<T>::IsEqual(const T& theValue) const
{
	const T& thisObject = static_cast<const T&>(*this);

	return thisObject.CompareEqual(theValue);
}





//=============================================================================
//		NMixinComparable::GetOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
template <typename T>
NComparison NMixinComparable<T>::GetOrder(const T& theValue) const
{
	const T& thisObject = static_cast<const T&>(*this);

	return thisObject.CompareOrder(theValue);
}
