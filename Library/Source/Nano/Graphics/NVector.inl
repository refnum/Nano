/*	NAME:
		NVector.inl

	DESCRIPTION:
		Vector object.

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
#include "NMathUtils.h"





//=============================================================================
//		NVector::NVector : Constructor.
//-----------------------------------------------------------------------------
constexpr NVector::NVector(float64_t valX, float64_t valY)
	: x(valX)
	, y(valY)
{
}





//=============================================================================
//		NVector::NVector : Constructor.
//-----------------------------------------------------------------------------
constexpr NVector::NVector()
	: x(0.0)
	, y(0.0)
{
}





//=============================================================================
//		NVector::Clear : Clear the vector.
//-----------------------------------------------------------------------------
constexpr void NVector::Clear()
{


	// Clear the vector
	x = 0;
	y = 0;
}





//=============================================================================
//		NVector::IsZero : Is the vector zero?
//-----------------------------------------------------------------------------
constexpr bool NVector::IsZero() const
{
	return x == 0.0 && y == 0.0;
}





//=============================================================================
//		NVector::IsNormalized : Is the vector normalized?
//-----------------------------------------------------------------------------
inline bool NVector::IsNormalized() const
{
	return GetLength() == 1.0;
}





//=============================================================================
//		NVector::GetLength : Get the length.
//-----------------------------------------------------------------------------
inline float64_t NVector::GetLength() const
{
	return sqrt(GetLength2());
}





//=============================================================================
//		NVector::GetLength2 : Get the length^2.
//-----------------------------------------------------------------------------
constexpr float64_t NVector::GetLength2() const
{


	// Get the length^2
	return (x * x) + (y * y);
}





//=============================================================================
//		NVector::GetDot : Get the dot product.
//-----------------------------------------------------------------------------
constexpr float64_t NVector::GetDot(const NVector& theVector) const
{


	// Get the dot product
	return (x * theVector.x) + (y * theVector.y);
}





//=============================================================================
//		NVector::GetCross : Get the cross product.
//-----------------------------------------------------------------------------
constexpr float64_t NVector::GetCross(const NVector& theVector) const
{


	// Get the cross product
	return (x * theVector.y) - (y * theVector.x);
}





//=============================================================================
//		NVector::GetAngle : Get the angle to another vector.
//-----------------------------------------------------------------------------
inline NDegrees NVector::GetAngle(const NVector& theVector) const
{


	// Calculate the angle
	NVector normal1(GetNormalized());
	NVector normal2(theVector.GetNormalized());

	float64_t dotProduct   = normal1.GetDot(normal2);
	float64_t crossProduct = normal1.GetCross(normal2);
	float64_t angleRad     = atan2(crossProduct, dotProduct);

	return NMathUtils::ToDegrees(angleRad);
}





//=============================================================================
//		NVector::GetNormalized : Get the normalized vector.
//-----------------------------------------------------------------------------
inline NVector NVector::GetNormalized() const
{


	// Normalize the vector
	float64_t invLength = 1.0 / GetLength();

	return NVector(x * invLength, y * invLength);
}





//=============================================================================
//		NVector::Normalize : Normalize the vector.
//-----------------------------------------------------------------------------
inline void NVector::Normalize()
{
	*this = GetNormalized();
}





//=============================================================================
//		NVector::GetNegated : Get the negated vector.
//-----------------------------------------------------------------------------
constexpr NVector NVector::GetNegated() const
{


	// Negate the vector
	return NVector(-x, -y);
}





//=============================================================================
//		NVector::Negate : Negate the vector.
//-----------------------------------------------------------------------------
constexpr void NVector::Negate()
{
	*this = GetNegated();
}





//=============================================================================
//		NVector::GetScaled : Get the scaled vector.
//-----------------------------------------------------------------------------
constexpr NVector NVector::GetScaled(float64_t scaleBy) const
{


	// Scale the vector
	return NVector(x * scaleBy, y * scaleBy);
}





//=============================================================================
//		NVector::Scale : Scale the vector.
//-----------------------------------------------------------------------------
constexpr void NVector::Scale(float64_t scaleBy)
{
	*this = GetScaled(scaleBy);
}





//=============================================================================
//		NVector::+ : Addition operator.
//-----------------------------------------------------------------------------
constexpr NVector NVector::operator+(const NVector& theVector) const
{


	// Add the vector
	return NVector(x + theVector.x, y + theVector.y);
}





//=============================================================================
//		NVector::- : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr NVector NVector::operator-(const NVector& theVector) const
{


	// Subtract the vector
	return NVector(x - theVector.x, y - theVector.y);
}





//=============================================================================
//		NVector::+= : Addition operator.
//-----------------------------------------------------------------------------
constexpr const NVector& NVector::operator+=(const NVector& theVector)
{
	*this = *this + theVector;

	return *this;
}





//=============================================================================
//		NVector::-= : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr const NVector& NVector::operator-=(const NVector& theVector)
{
	*this = *this - theVector;

	return *this;
}





#pragma mark NMixinComparable
//=============================================================================
//		NVector::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
constexpr bool NVector::CompareEqual(const NVector& theVector) const
{


	// Compare the vector
	return x == theVector.x && y == theVector.y;
}





//=============================================================================
//		NVector::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
constexpr NComparison NVector::CompareOrder(const NVector& theVector) const
{


	// Order the vector
	//
	// A vector has no real ordering so we sort by length (magnitude)
	// and then by location.
	NComparison theResult = NCompare(GetLength2(), theVector.GetLength2());

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(x, theVector.x);

		if (theResult == NComparison::EqualTo)
		{
			theResult = NCompare(y, theVector.y);
		}
	}

	return theResult;
}
