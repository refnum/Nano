/*	NAME:
		NSize.inl

	DESCRIPTION:
		Size object.

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
//		NSize::NSize : Constructor.
//-----------------------------------------------------------------------------
constexpr NSize::NSize(float64_t theWidth, float64_t theHeight)
	: width(theWidth)
	, height(theHeight)
{
}





//=============================================================================
//		NSize::NSize : Constructor.
//-----------------------------------------------------------------------------
constexpr NSize::NSize()
	: width(0.0)
	, height(0.0)
{
}





//=============================================================================
//		NSize::Clear : Clear the size.
//-----------------------------------------------------------------------------
constexpr void NSize::Clear()
{


	// Clear the size
	width  = 0;
	height = 0;
}





//=============================================================================
//		NSize::IsEmpty : Is the size empty?
//-----------------------------------------------------------------------------
constexpr bool NSize::IsEmpty() const
{
	return width == 0.0 && height == 0.0;
}





//=============================================================================
//		NSize::GetIntegral : Get an integral size.
//-----------------------------------------------------------------------------
inline NSize NSize::GetIntegral() const
{


	// Get an integral size
	//
	// We round with ceil() to create the smallest integral size that
	// is equal to or larger than the current size.
	NSize theResult;

	theResult.width  = ceil(width);
	theResult.height = ceil(height);

	return theResult;
}





//=============================================================================
//		NSize::MakeIntegral : Make the size integral.
//-----------------------------------------------------------------------------
inline void NSize::MakeIntegral()
{
	*this = GetIntegral();
}





//=============================================================================
//		NSize::GetNormalized : Get a normalized size.
//-----------------------------------------------------------------------------
constexpr NSize NSize::GetNormalized() const
{


	// Noramlize the size
	//
	// A normalized size has a positive width and height.
	NSize theResult;

	theResult.width  = (width < 0.0) ? -width : width;
	theResult.height = (height < 0.0) ? -height : height;

	return theResult;
}





//=============================================================================
//		NSize::Normalize : Normalize the size.
//-----------------------------------------------------------------------------
constexpr void NSize::Normalize()
{
	*this = GetNormalized();
}





//=============================================================================
//		NSize::GetScaled : Get a scaled size.
//-----------------------------------------------------------------------------
constexpr NSize NSize::GetScaled(float64_t scaleBy) const
{


	// Get a scaled size
	NSize theResult;

	theResult.width  = width * scaleBy;
	theResult.height = height * scaleBy;

	return theResult;
}





//=============================================================================
//		NSize::Scale : Scale the size.
//-----------------------------------------------------------------------------
constexpr void NSize::Scale(float64_t scaleBy)
{
	*this = GetScaled(scaleBy);
}





//=============================================================================
//		NSize::GetResized : Get a resized size.
//-----------------------------------------------------------------------------
constexpr NSize NSize::GetResized(float64_t deltaX, float64_t deltaY) const
{


	// Get a resized size
	NSize theResult;

	theResult.width  = width + deltaX;
	theResult.height = height + deltaY;

	return theResult;
}





//=============================================================================
//		NSize::Resize : Resize the size.
//-----------------------------------------------------------------------------
constexpr void NSize::Resize(float64_t deltaX, float64_t deltaY)
{
	*this = GetResized(deltaX, deltaY);
}





#pragma mark NMixinComparable
//=============================================================================
//		NSize::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
constexpr bool NSize::CompareEqual(const NSize& theSize) const
{


	// Compare the size
	return width == theSize.width && height == theSize.height;
}





//=============================================================================
//		NSize::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
constexpr NComparison NSize::CompareOrder(const NSize& theSize) const
{


	// Order the size
	//
	// A size has no real ordering so we sort by area.
	float64_t areaA = width * height;
	float64_t areaB = theSize.width * theSize.height;

	return NCompare(areaA, areaB);
}
