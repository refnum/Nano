/*	NAME:
		NTransform.inl

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include <math.h>





//=============================================================================
//		NTransform::NTransform : Constructor.
//-----------------------------------------------------------------------------
constexpr NTransform::NTransform(const NMatrix_3x3& theMatrix)
	: mMatrix(theMatrix)
{
}





//=============================================================================
//		NTransform::NTransform : Constructor.
//-----------------------------------------------------------------------------
constexpr NTransform::NTransform()
	: mMatrix{{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0}}}
{
}





//=============================================================================
//		NTransform::Clear : Clear the transform.
//-----------------------------------------------------------------------------
constexpr void NTransform::Clear()
{


	// Clear the transform
	mMatrix[0][0] = 1.0;
	mMatrix[0][1] = 0.0;
	mMatrix[0][2] = 0.0;

	mMatrix[1][0] = 0.0;
	mMatrix[1][1] = 1.0;
	mMatrix[1][2] = 0.0;

	mMatrix[2][0] = 0.0;
	mMatrix[2][1] = 0.0;
	mMatrix[2][2] = 1.0;
}





//=============================================================================
//		NTransform::IsIdentity : Is this the identity transform?
//-----------------------------------------------------------------------------
constexpr bool NTransform::IsIdentity() const
{


	// Check our state
	return (mMatrix[0][0] == 1.0 && mMatrix[0][1] == 0.0 && mMatrix[0][2] == 0.0) &&
		   (mMatrix[1][0] == 0.0 && mMatrix[1][1] == 1.0 && mMatrix[1][2] == 0.0) &&
		   (mMatrix[2][0] == 0.0 && mMatrix[2][1] == 0.0 && mMatrix[2][2] == 1.0);
}





//=============================================================================
//		NTransform::Invert : Invert the transform.
//-----------------------------------------------------------------------------
constexpr bool NTransform::Invert()
{


	// Calculate the determinant
	float64_t d = 0.0;

	d += mMatrix[0][0] * ((mMatrix[1][1] * mMatrix[2][2]) - (mMatrix[1][2] * mMatrix[2][1]));
	d -= mMatrix[0][1] * ((mMatrix[1][0] * mMatrix[2][2]) - (mMatrix[1][2] * mMatrix[2][0]));
	d += mMatrix[0][2] * ((mMatrix[1][0] * mMatrix[2][1]) - (mMatrix[1][1] * mMatrix[2][0]));

	if (d == 0.0)
	{
		return false;
	}



	// Invert the matrix
	float64_t s = 1.0 / d;

	NMatrix_3x3 theResult{
		{{s * ((mMatrix[1][1] * mMatrix[2][2]) - (mMatrix[1][2] * mMatrix[2][1])),
		  s * ((mMatrix[1][2] * mMatrix[2][0]) - (mMatrix[1][0] * mMatrix[2][2])),
		  s * ((mMatrix[1][0] * mMatrix[2][1]) - (mMatrix[1][1] * mMatrix[2][0]))},

		 {s * ((mMatrix[0][2] * mMatrix[2][1]) - (mMatrix[0][1] * mMatrix[2][2])),
		  s * ((mMatrix[0][0] * mMatrix[2][2]) - (mMatrix[0][2] * mMatrix[2][0])),
		  s * ((mMatrix[0][1] * mMatrix[2][0]) - (mMatrix[0][0] * mMatrix[2][1]))},

		 {s * ((mMatrix[0][1] * mMatrix[1][2]) - (mMatrix[0][2] * mMatrix[1][1])),
		  s * ((mMatrix[0][2] * mMatrix[1][0]) - (mMatrix[0][0] * mMatrix[1][2])),
		  s * ((mMatrix[0][0] * mMatrix[1][1]) - (mMatrix[0][1] * mMatrix[1][0]))}}};

	mMatrix = theResult;

	return true;
}





//=============================================================================
//		NTransform::Concatenate : Concatenate a transform.
//-----------------------------------------------------------------------------
constexpr void NTransform::Concatenate(const NTransform& theTransform)
{


	// Concatenate the transform
	const auto& a = mMatrix;
	const auto& b = theTransform.mMatrix;

	NMatrix_3x3 theResult{{{(a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]),
							(a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]),
							(a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2])},

						   {(a[1][0] * b[0][0]) + (a[1][1] * b[1][0]) + (a[1][2] * b[2][0]),
							(a[1][0] * b[0][1]) + (a[1][1] * b[1][1]) + (a[1][2] * b[2][1]),
							(a[1][0] * b[0][2]) + (a[1][1] * b[1][2]) + (a[1][2] * b[2][2])},

						   {(a[2][0] * b[0][0]) + (a[2][1] * b[1][0]) + (a[2][2] * b[2][0]),
							(a[2][0] * b[0][1]) + (a[2][1] * b[1][1]) + (a[2][2] * b[2][1]),
							(a[2][0] * b[0][2]) + (a[2][1] * b[1][2]) + (a[2][2] * b[2][2])}}};

	mMatrix = theResult;
}





//=============================================================================
//		NTransform::Translate : Concatenate a translate transform.
//-----------------------------------------------------------------------------
constexpr void NTransform::Translate(float64_t x, float64_t y)
{


	// Concatenate the transform
	Concatenate(NTransformTranslate(x, y));
}





//=============================================================================
//		NTransform::Scale : Concatenate a scale transform.
//-----------------------------------------------------------------------------
constexpr void NTransform::Scale(float64_t x, float64_t y)
{


	// Concatenate the transform
	Concatenate(NTransformScale(x, y));
}





//=============================================================================
//		NTransform::Rotate : Concatenate a rotation transform.
//-----------------------------------------------------------------------------
void NTransform::Rotate(NRadians angle)
{


	// Concatenate the transform
	Concatenate(NTransformRotate(angle));
}





//=============================================================================
//		NTransform::Apply : Transform an NPoint.
//-----------------------------------------------------------------------------
constexpr NPoint NTransform::Apply(const NPoint& thePoint)
{


	// Apply the transform
	NPoint theResult;

	theResult.x = (mMatrix[0][0] * thePoint.x) + (mMatrix[1][0] * thePoint.y) + mMatrix[2][0];
	theResult.y = (mMatrix[0][1] * thePoint.x) + (mMatrix[1][1] * thePoint.y) + mMatrix[2][1];

	return theResult;
}





//=============================================================================
//		NTransform::Apply : Transform an NSize.
//-----------------------------------------------------------------------------
constexpr NSize NTransform::Apply(const NSize& theSize)
{


	// Apply the transform
	NSize theResult;

	theResult.width  = (mMatrix[0][0] * theSize.width) + (mMatrix[1][0] * theSize.height);
	theResult.height = (mMatrix[0][1] * theSize.width) + (mMatrix[0][1] * theSize.height);

	return theResult;
}





//=============================================================================
//		NTransform::Apply : Transform an NRectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NTransform::Apply(const NRectangle& theRectangle)
{


	// Apply the transform
	NPoint pTL = Apply(theRectangle.GetPoint(NPosition::TopLeft));
	NPoint pTR = Apply(theRectangle.GetPoint(NPosition::TopRight));
	NPoint pBL = Apply(theRectangle.GetPoint(NPosition::BottomLeft));
	NPoint pBR = Apply(theRectangle.GetPoint(NPosition::BottomRight));

	float64_t minX = std::min(pTL.x, std::min(pTR.x, std::min(pBL.x, pBR.x)));
	float64_t maxX = std::max(pTL.x, std::max(pTR.x, std::max(pBL.x, pBR.x)));

	float64_t minY = std::min(pTL.y, std::min(pTR.y, std::min(pBL.y, pBR.y)));
	float64_t maxY = std::max(pTL.y, std::max(pTR.y, std::max(pBL.y, pBR.y)));

	return NRectangle(minX, minY, maxX - minX, maxY - minY);
}





#pragma mark NMixinComparable
//=============================================================================
//		NTransform::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
constexpr bool NTransform::CompareEqual(const NTransform& theTransform) const
{


	// Compare the transform
	const auto& a = mMatrix;
	const auto& b = theTransform.mMatrix;

	return (a[0][0] == b[0][0] && a[0][1] == b[0][1] && a[0][2] == b[0][2]) &&
		   (a[1][0] == b[1][0] && a[1][1] == b[1][1] && a[1][2] == b[1][2]) &&
		   (a[2][0] == b[2][0] && a[2][1] == b[2][1] && a[2][2] == b[2][2]);
}





//=============================================================================
//		NTransform::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
constexpr NComparison NTransform::CompareOrder(const NTransform& theTransform) const
{


	// Order the transform
	//
	// A transform has no real ordering.
	for (size_t y = 0; y < 3; y++)
	{
		for (size_t x = 0; x < 3; x++)
		{
			NComparison theResult = NCompare(mMatrix[y][x], theTransform.mMatrix[y][x]);
			if (theResult != NComparison::EqualTo)
			{
				return theResult;
			}
		}
	}

	return NComparison::EqualTo;
}





//=============================================================================
//		NTransformTranslate : Create a translation transform.
//-----------------------------------------------------------------------------
constexpr NTransform NTransformTranslate(float64_t x, float64_t y)
{


	// Get the transform
	return NTransform(NMatrix_3x3{{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {x, y, 1.0}}});
}





//=============================================================================
//		NTransformScale : Create a scale transform.
//-----------------------------------------------------------------------------
constexpr NTransform NTransformScale(float64_t x, float64_t y)
{


	// Get the transform
	return NTransform(NMatrix_3x3{{{x, 0.0, 0.0}, {0.0, y, 0.0}, {0.0, 0.0, 1.0}}});
}





//=============================================================================
//		NTransformRotate : Create a rotation transform.
//-----------------------------------------------------------------------------
inline NTransform NTransformRotate(NRadians angle)
{


	// Get the transform
	NRadians cosA = cos(angle);
	NRadians sinA = sin(angle);

	return NTransform(NMatrix_3x3{{{cosA, sinA, 0.0}, {-sinA, -cosA, 0.0}, {0.0, 0.0, 1.0}}});
}
