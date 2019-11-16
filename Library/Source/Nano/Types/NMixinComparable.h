/*	NAME:
		NMixinComparable.h

	DESCRIPTION:
		Mix-in for comparable objects.

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
#ifndef NMIXIN_COMPARABLE_H
#define NMIXIN_COMPARABLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTypes.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
enum class NComparison
{
	LessThan                                                = -1,
	EqualTo                                                 = 0,
	GreaterThan                                             = 1
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template <class T>
class NMixinComparable
{
public:
	// Operators
	bool                                operator==(const T& theValue) const;
	bool                                operator!=(const T& theValue) const;
	bool                                operator<=(const T& theValue) const;
	bool                                operator<( const T& theValue) const;
	bool                                operator>=(const T& theValue) const;
	bool                                operator>( const T& theValue) const;


	// Compare an object
	//
	// Must be implemented by derived classes.
	NComparison                         Compare(const T& theValue) const;


private:
	NComparison                         CompareTo(const T& theValue) const;
};





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
// Compare two types
template <typename A, typename B>
inline NComparison NCompare(const A& a, const B& b)
{
	if (a < b)
	{
		return NComparison::LessThan;
	}
	else if (a > b)
	{
		return NComparison::GreaterThan;
	}
	else
	{
		return NComparison::EqualTo;
	}
}


// Convert a memcmp-style comparison result
inline NComparison NCompare(int32_t x)
{
	return NCompare(x, 0);
}





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.inl"



#endif // NMIXIN_COMPARABLE_H
