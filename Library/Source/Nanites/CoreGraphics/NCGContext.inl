/*	NAME:
		NCGContext.inl

	DESCRIPTION:
		CoreGraphics context.

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
//		NCGContext::NCGContext : Constructor.
//-----------------------------------------------------------------------------
inline NCGContext::NCGContext(NImage& theImage)
	: mIsFlipped(false)
{


	// Set the output
	SetOutput(theImage);
}





//=============================================================================
//		NCGContext::NCGContext : Constructor.
//-----------------------------------------------------------------------------
inline NCGContext::NCGContext()
	: mIsFlipped(false)
{
}





//=============================================================================
//		NCGContext::SaveState : Save the state.
//-----------------------------------------------------------------------------
inline void NCGContext::SaveState()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Save the state
	CGContextSaveGState(*this);
}





//=============================================================================
//		NCGContext::RestoreState : Restore the state.
//-----------------------------------------------------------------------------
inline void NCGContext::RestoreState()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Restore the state
	CGContextRestoreGState(*this);
}





//=============================================================================
//		NCGContext::Scale : Apply a scale transform.
//-----------------------------------------------------------------------------
inline void NCGContext::Scale(CGFloat x, CGFloat y)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Scale the transform
	CGContextScaleCTM(*this, x, y);
}





//=============================================================================
//		NCGContext::Translate : Apply a translation transform.
//-----------------------------------------------------------------------------
inline void NCGContext::Translate(CGFloat x, CGFloat y)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Translate the transform
	CGContextTranslateCTM(*this, x, y);
}





//=============================================================================
//		NCGContext::Rotate : Apply a rotation transform.
//-----------------------------------------------------------------------------
inline void NCGContext::Rotate(NRadians theAngle)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Rotate the transform
	CGContextRotateCTM(*this, CGFloat(theAngle));
}





//=============================================================================
//		NCGContext::IsFlipped : Is the context flipped?
//-----------------------------------------------------------------------------
inline bool NCGContext::IsFlipped() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	return mIsFlipped;
}





//=============================================================================
//		NCGContext::ClipToRect : Clip to a rectangle.
//-----------------------------------------------------------------------------
inline void NCGContext::ClipToRect(const NRectangle& theRect)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Clip to the rectangle
	CGContextClipToRect(*this, ToCG(theRect));
}





//=============================================================================
//		NCGContext::SetLineWidth : Set the line width.
//-----------------------------------------------------------------------------
inline void NCGContext::SetLineWidth(CGFloat theWidth)
{


	// Validate our parameters and state
	NN_REQUIRE(theWidth > 0.0);
	NN_REQUIRE(IsValid());



	// Set the line width
	CGContextSetLineWidth(*this, theWidth);
}





//=============================================================================
//		NCGContext::SetLineCap : Set the line cap.
//-----------------------------------------------------------------------------
inline void NCGContext::SetLineCap(CGLineCap theCap)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the line cap
	CGContextSetLineCap(*this, theCap);
}





//=============================================================================
//		NCGContext::SetLineJoin : Set the line join.
//-----------------------------------------------------------------------------
inline void NCGContext::SetLineJoin(CGLineJoin theJoin)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the line join
	CGContextSetLineJoin(*this, theJoin);
}





//=============================================================================
//		NCGContext::SetLineDash : Set the line dash.
//-----------------------------------------------------------------------------
inline void NCGContext::SetLineDash()
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Set the line dash
	CGContextSetLineDash(*this, 0.0, nullptr, 0);
}





//=============================================================================
//		NCGContext::SetAlpha : Set the alpha.
//-----------------------------------------------------------------------------
inline void NCGContext::SetAlpha(CGFloat theAlpha)
{


	// Validate our parameters and state
	NN_REQUIRE(theAlpha > 0.0);
	NN_REQUIRE(IsValid());



	// Set the alpha
	CGContextSetAlpha(*this, theAlpha);
}





//=============================================================================
//		NCGContext::SetAntiAlias : Set the anti-aliasing.
//-----------------------------------------------------------------------------
inline void NCGContext::SetAntiAlias(bool antiAlias)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the anti-aliasing
	CGContextSetShouldAntialias(*this, antiAlias);
}





//=============================================================================
//		NCGContext::GetInterpolation : Get the interpolation quality.
//-----------------------------------------------------------------------------
inline CGInterpolationQuality NCGContext::GetInterpolation() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the anti-aliasing
	return CGContextGetInterpolationQuality(*this);
}





//=============================================================================
//		NCGContext::SetInterpolation : Set the interpolation quality.
//-----------------------------------------------------------------------------
inline void NCGContext::SetInterpolation(CGInterpolationQuality theValue)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the anti-aliasing
	CGContextSetInterpolationQuality(*this, theValue);
}





//=============================================================================
//		NCGContext::SetBlendMode : Set the blend mode.
//-----------------------------------------------------------------------------
inline void NCGContext::SetBlendMode(CGBlendMode theMode)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the blend mode
	CGContextSetBlendMode(*this, theMode);
}





//=============================================================================
//		NCGContext::SetFillColor : Set the fill color.
//-----------------------------------------------------------------------------
inline void NCGContext::SetFillColor(const NColor& theColor)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the fill color
	CGContextSetRGBFillColor(*this,
							 CGFloat(theColor.GetRed()),
							 CGFloat(theColor.GetGreen()),
							 CGFloat(theColor.GetBlue()),
							 CGFloat(theColor.GetAlpha()));
}





//=============================================================================
//		NCGContext::SetStrokeColor : Set the stroke color.
//-----------------------------------------------------------------------------
inline void NCGContext::SetStrokeColor(const NColor& theColor)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Set the stroke color
	CGContextSetRGBStrokeColor(*this,
							   CGFloat(theColor.GetRed()),
							   CGFloat(theColor.GetGreen()),
							   CGFloat(theColor.GetBlue()),
							   CGFloat(theColor.GetAlpha()));
}





//=============================================================================
//		NCGContext::BeginPath : Begin a sub-path.
//-----------------------------------------------------------------------------
inline void NCGContext::BeginPath()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Begin the path
	CGContextBeginPath(*this);
}





//=============================================================================
//		NCGContext::ClosePath : Close a sub-path.
//-----------------------------------------------------------------------------
inline void NCGContext::ClosePath()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Close the path
	CGContextClosePath(*this);
}





//=============================================================================
//		NCGContext::MoveTo : Move to a point.
//-----------------------------------------------------------------------------
inline void NCGContext::MoveTo(CGFloat x, CGFloat y)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Move to the point
	CGContextMoveToPoint(*this, x, y);
}





//=============================================================================
//		NCGContext::MoveTo : Move to a point.
//-----------------------------------------------------------------------------
inline void NCGContext::MoveTo(const NPoint& thePoint)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Move to the point
	CGContextMoveToPoint(*this, thePoint.x, thePoint.y);
}





//=============================================================================
//		NCGContext::AddLineTo : Add a line to a point.
//-----------------------------------------------------------------------------
inline void NCGContext::AddLineTo(CGFloat x, CGFloat y)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Add a line to the point
	CGContextAddLineToPoint(*this, x, y);
}





//=============================================================================
//		NCGContext::AddLineTo : Add a line to a point.
//-----------------------------------------------------------------------------
inline void NCGContext::AddLineTo(const NPoint& thePoint)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Add a line to the point
	CGContextAddLineToPoint(*this, thePoint.x, thePoint.y);
}





//=============================================================================
//		NCGContext::AddArc : Add an arc.
//-----------------------------------------------------------------------------
inline void NCGContext::AddArc(const NPoint& theCenter,
							   CGFloat       theRadius,
							   CGFloat       angleStart,
							   CGFloat       angleEnd,
							   NOrientation  theOrientation)
{


	// Validate our parameters and state
	NN_REQUIRE(theRadius > 0.0);
	NN_REQUIRE(IsValid());



	// Add an arc
	CGContextAddArc(*this,
					theCenter.x,
					theCenter.y,
					theRadius,
					angleStart,
					angleEnd,
					theOrientation == NOrientation::Clockwise);
}





//=============================================================================
//		NCGContext::AddArcToPoint : Add an arc.
//-----------------------------------------------------------------------------
inline void NCGContext::AddArcToPoint(const NPoint& point1, const NPoint& point2, CGFloat theRadius)
{


	// Validate our parameters and state
	NN_REQUIRE(theRadius > 0.0);
	NN_REQUIRE(IsValid());



	// Add an arc
	CGContextAddArcToPoint(*this, point1.x, point1.y, point2.x, point2.y, theRadius);
}





//=============================================================================
//		NCGContext::FillRect : Fill a rectangle.
//-----------------------------------------------------------------------------
inline void NCGContext::FillRect(const NRectangle& theRect)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Fill the rectangle
	CGContextFillRect(*this, ToCG(theRect));
}





//=============================================================================
//		NCGContext::StrokeRect : Stroke a rectangle.
//-----------------------------------------------------------------------------
inline void NCGContext::StrokeRect(const NRectangle& theRect)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Stroke the rectangle
	CGContextStrokeRect(*this, ToCG(theRect));
}





//=============================================================================
//		NCGContext::ClearRect : Clear a rectangle.
//-----------------------------------------------------------------------------
inline void NCGContext::ClearRect(const NRectangle& theRect)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Clear the rectangle
	CGContextClearRect(*this, ToCG(theRect));
}





//=============================================================================
//		NCGContext::FillPath : Fill the current path.
//-----------------------------------------------------------------------------
inline void NCGContext::FillPath()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Fill the path
	CGContextFillPath(*this);
}





//=============================================================================
//		NCGContext::StrokePath : Stroke the current path.
//-----------------------------------------------------------------------------
inline void NCGContext::StrokePath()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Stroke the path
	CGContextStrokePath(*this);
}





//=============================================================================
//		NCGContext::DrawPath : Draw the current path.
//-----------------------------------------------------------------------------
inline void NCGContext::DrawPath(CGPathDrawingMode theMode)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Draw the path
	CGContextDrawPath(*this, theMode);
}





#pragma mark NScopedCGState
//=============================================================================
//		NScopedCGState::NScopedCGState : Constructor.
//-----------------------------------------------------------------------------
inline NScopedCGState::NScopedCGState(NCGContext& theContext)
	: mContext(theContext)
{


	// Save the state
	mContext.SaveState();
}





//=============================================================================
//		NScopedCGState::~NScopedCGState : Destructor.
//-----------------------------------------------------------------------------
inline NScopedCGState::~NScopedCGState()
{


	// Restore the state
	mContext.RestoreState();
}
