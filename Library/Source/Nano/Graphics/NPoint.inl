/*	NAME:
		NPoint.inl

	DESCRIPTION:
		Point object.

		NEncodable support uses a helper class to avoid a v-table.

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
#include "NVector.h"





//=============================================================================
//		NPoint::NPoint : Constructor.
//-----------------------------------------------------------------------------
constexpr NPoint::NPoint(float64_t valX, float64_t valY)
	: x(valX)
	, y(valY)
{
}





//=============================================================================
//		NPoint::NPoint : Constructor.
//-----------------------------------------------------------------------------
constexpr NPoint::NPoint()
	: x(0.0)
	, y(0.0)
{
}





//=============================================================================
//		NPoint::Clear : Clear the point.
//-----------------------------------------------------------------------------
constexpr void NPoint::Clear()
{


	// Clear the point
	x = 0.0;
	y = 0.0;
}





//=============================================================================
//		NPoint::IsZero : Is the point zero?
//-----------------------------------------------------------------------------
constexpr bool NPoint::IsZero() const
{
	return x == 0.0 && y == 0.0;
}





//=============================================================================
//		NPoint::GetDistance : Get the distance to a point.
//-----------------------------------------------------------------------------
inline float64_t NPoint::GetDistance(const NPoint& thePoint) const
{
	return sqrt(GetDistance2(thePoint));
}





//=============================================================================
//		NPoint::GetDistance2 : Get the distance^2 to a point.
//-----------------------------------------------------------------------------
constexpr float64_t NPoint::GetDistance2(const NPoint& thePoint) const
{


	// Get the distance^2
	float64_t deltaX = x - thePoint.x;
	float64_t deltaY = y - thePoint.y;

	return (deltaX * deltaX) + (deltaY * deltaY);
}





//=============================================================================
//		NPoint::GetIntegral : Get an integral point.
//-----------------------------------------------------------------------------
inline NPoint NPoint::GetIntegral() const
{


	// Get an integral point
	//
	// We round with round() to ensure consistent behaviour regardless of
	// the current rounding direction.
	NPoint theResult;

	theResult.x = round(x);
	theResult.y = round(y);

	return theResult;
}





//=============================================================================
//		NPoint::Make : Make the point integral.
//-----------------------------------------------------------------------------
inline void NPoint::MakeIntegral()
{
	*this = GetIntegral();
}





//=============================================================================
//		NPoint::GetOffset : Get an offset point.
//-----------------------------------------------------------------------------
constexpr NPoint NPoint::GetOffset(float64_t deltaX, float64_t deltaY) const
{


	// Offset the point
	NPoint theResult;

	theResult.x = x + deltaX;
	theResult.y = y + deltaY;

	return theResult;
}





//=============================================================================
//		NPoint::Offset : Offset the point.
//-----------------------------------------------------------------------------
constexpr void NPoint::Offset(float64_t deltaX, float64_t deltaY)
{
	*this = GetOffset(deltaX, deltaY);
}





//=============================================================================
//		NPoint::+ : Addition operator.
//-----------------------------------------------------------------------------
constexpr NPoint NPoint::operator+(const NVector& theVector) const
{


	// Add the vector
	return NPoint(x + theVector.x, y + theVector.y);
}





//=============================================================================
//		NPoint::- : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr NPoint NPoint::operator-(const NVector& theVector) const
{


	// Subtract the vector
	return NPoint(x - theVector.x, y - theVector.y);
}





//=============================================================================
//		NPoint::- : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr NVector NPoint::operator-(const NPoint& thePoint) const
{


	// Subtract the point
	return NVector(x - thePoint.x, y - thePoint.y);
}





//=============================================================================
//		NPoint::+= : Addition operator.
//-----------------------------------------------------------------------------
constexpr const NPoint& NPoint::operator+=(const NVector& theVector)
{
	*this = *this + theVector;

	return *this;
}





//=============================================================================
//		NPoint::-= : Subtraction operator.
//-----------------------------------------------------------------------------
constexpr const NPoint& NPoint::operator-=(const NVector& theVector)
{
	*this = *this - theVector;

	return *this;
}





#pragma mark NMixinComparable
//=============================================================================
//		NPoint::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
constexpr bool NPoint::CompareEqual(const NPoint& thePoint) const
{


	// Compare the point
	return x == thePoint.x && y == thePoint.y;
}





//=============================================================================
//		NPoint::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
constexpr NComparison NPoint::CompareOrder(const NPoint& thePoint) const
{


	// Order the point
	//
	// A point has no real ordering so we sort by x then y.
	NComparison theResult = NCompare(x, thePoint.x);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(y, thePoint.y);
	}

	return theResult;
}
