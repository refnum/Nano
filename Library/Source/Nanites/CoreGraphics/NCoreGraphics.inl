/*	NAME:
		NCoreGraphics.inl

	DESCRIPTION:
		CoreGraphics support.

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
//		Public Functions
//-----------------------------------------------------------------------------
//		ToCG : Convert NPoint to CGPoint.
//----------------------------------------------------------------------------
inline CGPoint ToCG(const NPoint& thePoint)
{
	return CGPointMake(thePoint.x, thePoint.y);
}





//=============================================================================
//		ToCG : Convert NSize to CGSize.
//-----------------------------------------------------------------------------
inline CGSize ToCG(const NSize& theSize)
{
	return CGSizeMake(theSize.width, theSize.height);
}





//=============================================================================
//		ToCG : Convert NRectangle to CGRect.
//-----------------------------------------------------------------------------
inline CGRect ToCG(const NRectangle& theRect)
{
	return CGRectMake(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height);
}





//=============================================================================
//		ToNN : Convert CGPoint to NPoint.
//-----------------------------------------------------------------------------
inline NPoint ToNN(const CGPoint& thePoint)
{
	return NPoint(float64_t(thePoint.x), float64_t(thePoint.y));
}





//=============================================================================
//		ToNN : Convert CGSize to NSize.
//-----------------------------------------------------------------------------
inline NSize ToNN(const CGSize& theSize)
{
	return NSize(float64_t(theSize.width), float64_t(theSize.height));
}





//=============================================================================
//		ToNN : Convert CGRect to NRectangle.
//-----------------------------------------------------------------------------
inline NRectangle ToNN(const CGRect& theRect)
{
	return NRectangle(float64_t(theRect.origin.x),
					  float64_t(theRect.origin.y),
					  float64_t(theRect.size.width),
					  float64_t(theRect.size.height));
}





//=============================================================================
//		operator== : CGPoint equality.
//-----------------------------------------------------------------------------
inline bool operator==(const CGPoint& value1, const CGPoint& value2)
{
	return CGPointEqualToPoint(value1, value2);
}





//=============================================================================
//		operator== : CGSize equality.
//-----------------------------------------------------------------------------
inline bool operator==(const CGSize& value1, const CGSize& value2)
{
	return CGSizeEqualToSize(value1, value2);
}





//=============================================================================
//		operator== : CGRect equality.
//-----------------------------------------------------------------------------
inline bool operator==(const CGRect& value1, const CGRect& value2)
{
	return CGRectEqualToRect(value1, value2);
}





//=============================================================================
//		operator!= : CGPoint inequality.
//-----------------------------------------------------------------------------
inline bool operator!=(const CGPoint& value1, const CGPoint& value2)
{
	return !CGPointEqualToPoint(value1, value2);
}





//=============================================================================
//		operator!= : CGSize inequality.
//-----------------------------------------------------------------------------
inline bool operator!=(const CGSize& value1, const CGSize& value2)
{
	return !CGSizeEqualToSize(value1, value2);
}





//=============================================================================
//		operator!= : CGRect inequality.
//-----------------------------------------------------------------------------
inline bool operator!=(const CGRect& value1, const CGRect& value2)
{
	return !CGRectEqualToRect(value1, value2);
}





//=============================================================================
//		CGRectMake : Construct a CGRect.
//-----------------------------------------------------------------------------
inline CGRect CGRectMake(CGFloat theWidth, CGFloat theHeight)
{
	return CGRectMake(0.0, 0.0, theWidth, theHeight);
}





//=============================================================================
//		CGRectMake : Construct a CGRect.
//-----------------------------------------------------------------------------
inline CGRect CGRectMake(const CGSize& theSize)
{
	return CGRectMake(0.0, 0.0, theSize.width, theSize.height);
}
