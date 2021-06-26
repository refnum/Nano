/*	NAME:
		NCGContext.cpp

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
#include "NCGContext.h"

// Nano
#include "NCGColor.h"
#include "NCGImage.h"
#include "NCGShading.h"
#include "NImage.h"
#include "NMathUtils.h"

// System
#include <CoreText/CTLine.h>





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
#define NN_SHOW_TEXT_LAYOUT                                 0





//=============================================================================
//		NCGContext::Flip : Flip the context.
//-----------------------------------------------------------------------------
void NCGContext::Flip(const NRectangle& theRect)
{


	// Flip the context
	Translate(0.0, theRect.origin.y + theRect.GetMaxY());
	Scale(1.0, -1.0);



	// Update our state
	mIsFlipped = !mIsFlipped;
}





//=============================================================================
//		NCGContext::ClipToPath : Clip to the current path.
//-----------------------------------------------------------------------------
void NCGContext::ClipToPath(NClipping theClip)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Clip to the current path
	switch (theClip)
	{
		case NClipping::NonZero:
			CGContextClip(*this);
			break;

		case NClipping::EvenOdd:
			CGContextEOClip(*this);
			break;
	}
}





//=============================================================================
//		NCGContext::SetOutput : Set the output to an image.
//-----------------------------------------------------------------------------
void NCGContext::SetOutput(NImage& theImage)
{


	// Validate our parameters
	NN_REQUIRE(theImage.IsValid());


	// Get the state we need
	CGBitmapInfo bitmapInfo = NCGImage::GetBitmapInfo(theImage.GetFormat());

	bool wasOK = Set(CGBitmapContextCreate(theImage.GetMutablePixels(),
										   theImage.GetWidth(),
										   theImage.GetHeight(),
										   theImage.GetBitsPerComponent(),
										   theImage.GetBytesPerRow(),
										   NCGColor::GetDeviceRGB(),
										   bitmapInfo));
	NN_EXPECT(wasOK);

	if (wasOK)
	{
		Flip(theImage.GetBounds());
	}
}





//=============================================================================
//		NCGContext::SetLineDash : Set the line dash.
//-----------------------------------------------------------------------------
void NCGContext::SetLineDash(const NVectorCGFloat& theDash, CGFloat thePhase)
{


	// Validate our parameters and state
	NN_REQUIRE(thePhase >= 0.0);
	NN_REQUIRE(IsValid());



	// Set the line dash
	if (theDash.empty())
	{
		CGContextSetLineDash(*this, 0.0, nullptr, 0);
	}
	else
	{
		CGContextSetLineDash(*this, thePhase, &theDash[0], theDash.size());
	}
}





//=============================================================================
//		NCGContext::SetShadow : Set the shadow.
//-----------------------------------------------------------------------------
void NCGContext::SetShadow(const NShadowInfo& theShadow)
{


	// Set the shadow
	NCGColor cgColor;

	cgColor.SetColor(theShadow.color);
	CGContextSetShadowWithColor(*this,
								CGSizeMake(theShadow.offset.x, theShadow.offset.y),
								theShadow.blur,
								cgColor);
}





//=============================================================================
//		NCGContext::AddRect : Add a rectangle.
//-----------------------------------------------------------------------------
void NCGContext::AddRect(const NRectangle& theRect, CGFloat cornerRadius)
{


	// Validate our parameters and state
	NN_REQUIRE(cornerRadius >= 0.0);
	NN_REQUIRE(IsValid());



	// Add a rectangle
	if (cornerRadius == 0.0)
	{
		CGContextAddRect(*this, ToCG(theRect));
	}


	// Add a round-rect
	else
	{
		// Get the state we need
		CGFloat leftMiddleX = theRect.origin.x;
		CGFloat leftMiddleY = theRect.origin.y + (theRect.size.height / 2.0);

		CGFloat topLeftX = theRect.origin.x;
		CGFloat topLeftY = theRect.origin.y + theRect.size.height;

		CGFloat topRightX = theRect.origin.x + theRect.size.width;
		CGFloat topRightY = theRect.origin.y + theRect.size.height;

		CGFloat bottomRightX = theRect.origin.x + theRect.size.width;
		CGFloat bottomRightY = theRect.origin.y;


		// Add the rect
		CGContextMoveToPoint(*this, leftMiddleX, leftMiddleY);
		CGContextAddArcToPoint(*this,
							   theRect.origin.x,
							   theRect.origin.y,
							   bottomRightX,
							   bottomRightY,
							   cornerRadius);
		CGContextAddArcToPoint(*this,
							   bottomRightX,
							   bottomRightY,
							   topRightX,
							   topRightY,
							   cornerRadius);
		CGContextAddArcToPoint(*this, topRightX, topRightY, topLeftX, topLeftY, cornerRadius);
		CGContextAddArcToPoint(*this,
							   topLeftX,
							   topLeftY,
							   theRect.origin.x,
							   theRect.origin.y,
							   cornerRadius);
		CGContextAddLineToPoint(*this, leftMiddleX, leftMiddleY);
	}
}





//=============================================================================
//		NCGContext::AddCircle : Add a circle.
//-----------------------------------------------------------------------------
void NCGContext::AddCircle(const NPoint& theCenter, CGFloat theRadius)
{


	// Validate our parameters and state
	NN_REQUIRE(theRadius > 0.0);
	NN_REQUIRE(IsValid());



	// Add a circle
	CGContextMoveToPoint(*this, theCenter.x + theRadius, theCenter.y);
	CGContextAddArc(*this, theCenter.x, theCenter.y, theRadius, 0.0, kNPi * 2.0, true);
}





//=============================================================================
//		NCGContext::AddEllipse : Add an ellipse.
//-----------------------------------------------------------------------------
void NCGContext::AddEllipse(const NRectangle& theRect)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state we need
	CGFloat halfWidth  = theRect.size.width / 2.0;
	CGFloat halfHeight = theRect.size.height / 2.0;


	// Add the ellipse
	CGContextSaveGState(*this);
	CGContextTranslateCTM(*this, theRect.origin.x + halfWidth, theRect.origin.y + halfHeight);
	CGContextScaleCTM(*this, halfWidth, halfHeight);
	CGContextAddArc(*this, 0.0, 0.0, 1.0, 0.0, kNPi * 2.0, true);
	CGContextRestoreGState(*this);
}





//=============================================================================
//		NCGContext::AddPolyline : Add a polyline.
//-----------------------------------------------------------------------------
void NCGContext::AddPolyline(const NVectorPoint& thePoints)
{


	// Validate our parameters and state
	NN_REQUIRE(thePoints.size() >= 2);
	NN_REQUIRE(IsValid());



	// Add the polyline
	static_assert(sizeof(NPoint) == sizeof(CGPoint));
	static_assert(offsetof(NPoint, x) == offsetof(CGPoint, x));
	static_assert(offsetof(NPoint, y) == offsetof(CGPoint, y));

	const CGPoint* cgPoints = reinterpret_cast<const CGPoint*>(&thePoints[0]);
	CGContextAddLines(*this, cgPoints, thePoints.size());
}





//=============================================================================
//		NCGContext::AddPolygon : Add a polygon.
//-----------------------------------------------------------------------------
void NCGContext::AddPolygon(const NVectorPoint& thePoints)
{


	// Validate our parameters and state
	NN_REQUIRE(thePoints.size() >= 3);
	NN_REQUIRE(IsValid());



	// Add the polygon
	static_assert(sizeof(NPoint) == sizeof(CGPoint));
	static_assert(offsetof(NPoint, x) == offsetof(CGPoint, x));
	static_assert(offsetof(NPoint, y) == offsetof(CGPoint, y));

	const CGPoint* cgPoints = reinterpret_cast<const CGPoint*>(&thePoints[0]);
	CGContextAddLines(*this, cgPoints, thePoints.size());
	CGContextAddLineToPoint(*this, thePoints[0].x, thePoints[0].y);
}





//=============================================================================
//		NCGContext::DrawRect : Draw a rectangle.
//-----------------------------------------------------------------------------
void NCGContext::DrawRect(const NRectangle& theRect, CGPathDrawingMode theMode)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Draw the rectangle
	AddRect(theRect);
	DrawPath(theMode);
}





//=============================================================================
//		NCGContext::DrawImage : Draw an image.
//-----------------------------------------------------------------------------
void NCGContext::DrawImage(const NImage& theImage, const NRectangle& theRect)
{


	// Validate our parameters and state
	NN_REQUIRE(theImage.IsValid());
	NN_REQUIRE(!theRect.IsEmpty());
	NN_REQUIRE(IsValid());



	// Draw the image
	NCGImage cgImage;

	cgImage.SetImage(theImage, NCFSource::View);

	CGContextDrawImage(*this, ToCG(theRect), cgImage);
}





//=============================================================================
//		NCGContext::DrawShading : Draw a shading.
//-----------------------------------------------------------------------------
void NCGContext::DrawShading(const NCGShading& theShading)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Draw the shading
	CGContextDrawShading(*this, theShading);
}





//=============================================================================
//		NCGContext::DrawImage : Draw an image.
//-----------------------------------------------------------------------------
void NCGContext::DrawText(const NString&    theText,
						  const NRectangle& theRect,
						  NPosition         alignTo,
						  CTFontUIFontType  fontID,
						  CGFloat           fontSize)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state we need
	NScopedCGState saveState(*this);
	NRectangle     textRect;

	// dair, todo
	NN_UNUSED(theText);
	NN_UNUSED(fontID);
	NN_UNUSED(fontSize);
	//	NCFObject ctLine = PrepareTextLine(theText, fontID, fontSize, textRect);
	textRect.SetPosition(alignTo, theRect);



	// Draw the layout
	if (NN_SHOW_TEXT_LAYOUT)
	{
		NScopedCGState saveDebugState(*this);

		SetAlpha(0.5);
		SetLineWidth(1.0);
		SetStrokeColor(kNColorGreen);

		SetFillColor(kNColorBlue);
		DrawRect(theRect);

		SetFillColor(kNColorRed);
		DrawRect(textRect);
	}



	// Draw the text
	CGContextSetTextPosition(*this, textRect.GetMinX(), textRect.GetMaxY());
	// dair, todo
	//	CTLineDraw(ctLine,       *this);
}





//=============================================================================
//		NCGContext::GetTextBounds : Get the text bounds.
//-----------------------------------------------------------------------------
NRectangle NCGContext::GetTextBounds(const NString&   theText,
									 CTFontUIFontType fontID,
									 CGFloat          fontSize)
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Measure the text
	NScopedCGState saveState(*this);
	NRectangle     textRect;

	// dair, todo
	NN_UNUSED(theText);
	NN_UNUSED(fontID);
	NN_UNUSED(fontSize);
	//	PrepareTextLine(theText, fontID, fontSize, textRect);

	return textRect;
}
