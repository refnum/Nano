/*	NAME:
		NCGShading.inl

	DESCRIPTION:
		CoreGraphics shading object.

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
//		NCGShading::GetMode : Get the shading mode.
//-----------------------------------------------------------------------------
inline NShadingMode NCGShading::GetMode() const
{


	// Get the mode
	return mMode;
}





//=============================================================================
//		NCGShading::GetSamples : Get the shading samples.
//-----------------------------------------------------------------------------
inline NVectorShadingSample NCGShading::GetSamples() const
{


	// Get the samples
	return mSamples;
}





//=============================================================================
//		NCGShading::GetStartPoint : Get the start point.
//-----------------------------------------------------------------------------
inline NPoint NCGShading::GetStartPoint() const
{


	// Get the start point
	return mStartPoint;
}





//=============================================================================
//		NCGShading::GetStartExtend : Get the start extend.
//-----------------------------------------------------------------------------
inline bool NCGShading::GetStartExtend() const
{


	// Get the start extend
	return mStartExtend;
}





//=============================================================================
//		NCGShading::GetStartRadius : Get the start radius.
//-----------------------------------------------------------------------------
inline CGFloat NCGShading::GetStartRadius() const
{


	// Validate our state
	NN_REQUIRE(mMode == NShadingMode::Radial);



	// Get the start radius
	return mStartRadius;
}





//=============================================================================
//		NCGShading::GetEndPoint : Get the end point.
//-----------------------------------------------------------------------------
inline NPoint NCGShading::GetEndPoint() const
{


	// Get the end point
	return mEndPoint;
}





//=============================================================================
//		NCGShading::GetEndExtend : Get the end extend.
//-----------------------------------------------------------------------------
inline bool NCGShading::GetEndExtend() const
{


	// Get the end extend
	return mEndExtend;
}





//=============================================================================
//		NCGShading::GetEndRadius : Get the end radius.
//-----------------------------------------------------------------------------
inline CGFloat NCGShading::GetEndRadius() const
{


	// Validate our state
	NN_REQUIRE(mMode == NShadingMode::Radial);



	// Get the end radius
	return mEndRadius;
}





//=============================================================================
//		operator== : NCGShadingSample equality.
//-----------------------------------------------------------------------------
inline bool operator==(const NShadingSample& value1, const NShadingSample& value2)
{
	return value1.theValue == value2.theValue && value1.theColor == value2.theColor;
}





//=============================================================================
//		operator== : NCGShadingSample inequality.
//-----------------------------------------------------------------------------
inline bool operator!=(const NShadingSample& value1, const NShadingSample& value2)
{
	return value1.theValue != value2.theValue || value1.theColor != value2.theColor;
}
