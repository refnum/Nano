/*	NAME:
		NVector.h

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
#ifndef NVECTOR_H
#define NVECTOR_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NanoMacros.h"
#include "NanoTypes.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NVector final : public NMixinComparable<NVector>
{
public:
	constexpr                           NVector(float64_t x, float64_t y);
	constexpr                           NVector();


	// Clear the vector
	constexpr void                      Clear();


	// Is the vector zero?
	constexpr bool                      IsZero() const;


	// Is the vector normalized?
	bool                                IsNormalized() const;


	// Get the length
	float64_t                           GetLength()  const;
	constexpr float64_t                 GetLength2() const;


	// Get the dot product
	constexpr float64_t                 GetDot(const NVector& theVector) const;


	// Get the cross product
	constexpr float64_t                 GetCross(const NVector& theVector) const;


	// Get the angle to another vector
	NDegrees                            GetAngle(const NVector& theVector) const;


	// Get a normalized vector
	NVector                             GetNormalized() const;
	void                                Normalize();


	// Get a negated vector
	constexpr NVector                   GetNegated() const;
	constexpr void                      Negate();


	// Get a scaled vector
	constexpr NVector                   GetScaled(float64_t scaleBy) const;
	constexpr void                      Scale(    float64_t scaleBy);


	// Operators
	constexpr NVector                   operator+(const NVector& theVector) const;
	constexpr NVector                   operator-(const NVector& theVector) const;

	constexpr const NVector&            operator+=(const NVector& theVector);
	constexpr const NVector&            operator-=(const NVector& theVector);


public:
	// NMixinComparable
	constexpr bool                      CompareEqual(const NVector& theVector) const;
	constexpr NComparison               CompareOrder(const NVector& theVector) const;


public:
	float64_t                           x;
	float64_t                           y;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NVector.inl"



#endif // NVECTOR_H
