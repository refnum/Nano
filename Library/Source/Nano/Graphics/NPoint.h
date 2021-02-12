/*	NAME:
		NPoint.h

	DESCRIPTION:
		Point object.

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
#ifndef NPOINT_H
#define NPOINT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NanoMacros.h"
#include "NanoTypes.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NVector;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NPoint final : public NMixinComparable<NPoint>
{
public:
	constexpr                           NPoint(float64_t x, float64_t y);
	constexpr                           NPoint();


	// Clear the point
	constexpr void                      Clear();


	// Is the point zero?
	constexpr bool                      IsZero() const;


	// Get the distance to a point
	float64_t                           GetDistance( const NPoint& thePoint) const;
	constexpr float64_t                 GetDistance2(const NPoint& thePoint) const;


	// Get an integral point
	//
	// An integral point is rounded to the nearest integer coordinate.
	NPoint                              GetIntegral() const;
	void                                MakeIntegral();


	// Get an offset point
	constexpr NPoint                    GetOffset(float64_t deltaX, float64_t deltaY) const;
	constexpr void                      Offset(   float64_t deltaX, float64_t deltaY);


	// Operators
	constexpr NPoint                    operator+(const NVector& theVector) const;
	constexpr NPoint                    operator-(const NVector& theVector) const;
	constexpr NVector                   operator-(const NPoint& thePoint)   const;

	constexpr const NPoint&             operator+=(const NVector& theVector);
	constexpr const NPoint&             operator-=(const NVector& theVector);


public:
	// NMixinComparable
	constexpr bool                      CompareEqual(const NPoint& thePoint) const;
	constexpr NComparison               CompareOrder(const NPoint& thePoint) const;


public:
	float64_t                           x;
	float64_t                           y;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NPoint.inl"



#endif // NPOINT_H
