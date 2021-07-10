/*	NAME:
		NTransform.h

	DESCRIPTION:
		Affine transform object.

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
#ifndef NTRANSFORM_H
#define NTRANSFORM_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NPoint.h"
#include "NRectangle.h"
#include "NSize.h"

// System
#include <array>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
using NMatrix_3x3                                           = std::array<std::array<float64_t, 3>, 3>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NTransform
{
public:
	constexpr                           NTransform(const NMatrix_3x3& theMatrix);
	constexpr                           NTransform();


	// Clear the transform
	//
	// Resets to the identity transform.
	constexpr void                      Clear();


	// Is the transform the identity transform?
	constexpr bool                      IsIdentity() const;


	// Invert the transform
	//
	// Returns as the transform could be inverted.
	constexpr bool                      Invert();


	// Concatenate a transform
	constexpr void                      Concatenate(const NTransform& theTransform);


	// Concatenate a translate / scale / rotate transform
	constexpr void                      Translate(float64_t x, float64_t y);
	constexpr void                      Scale(    float64_t x, float64_t y);
	constexpr void                      Rotate(NRadians angle);


	// Apply the transform
	//
	// Transforming a rectangle returns the smallest rectangle
	// that contains the transformed corner points.
	constexpr NPoint                    Apply(const NPoint& thePoint);
	constexpr NSize                     Apply(const NSize& theSize);
	constexpr NRectangle                Apply(const NRectangle& theRectangle);



public:
	// NMixinComparable
	constexpr bool                      CompareEqual(const NTransform& theTransform) const;
	constexpr NComparison               CompareOrder(const NTransform& theTransform) const;


private:
	NMatrix_3x3                         mMatrix;
};





//=============================================================================
//		Function Prototypes
//-----------------------------------------------------------------------------
// Create a transform
constexpr NTransform NTransformTranslate(float64_t x, float64_t y);
constexpr NTransform NTransformScale(    float64_t x, float64_t y);
NTransform           NTransformRotate(NRadians angle);





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NTransform.inl"



#endif // NTRANSFORM_H
