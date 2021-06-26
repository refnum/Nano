/*	NAME:
		NCGContext.h

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
#ifndef NCGCONTEXT_H
#define NCGCONTEXT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NCFObject.h"
#include "NColor.h"
#include "NCoreGraphics.h"
#include "NPoint.h"
#include "NRectangle.h"

// System
#include <CoreGraphics/CGContext.h>
#include <CoreText/CTFont.h>
#include <CoreText/CTLine.h>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NImage;
class NCGShading;
class NString;


// NCFObject helpers
using NCTLine = NCFObject<CTLineRef>;


// Shadow info
struct NShadowInfo
{
	NColor  color;
	CGFloat blur;
	NPoint  offset;
};





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Orientations
enum class NOrientation
{
	Clockwise,
	Counterclockwise
};


// Clipping
enum class NClipping
{
	NonZero,
	EvenOdd
};


// Shadows
inline constexpr NShadowInfo kNShadowNone                   = {NColor(0.0f, 0.0f, 0.0f, 0.00f), 0.0, NPoint()};
inline constexpr NShadowInfo kNShadowLight                  = {NColor(0.0f, 0.0f, 0.0f, 0.25f), 2.0, NPoint()};
inline constexpr NShadowInfo kNShadowDefault                = {NColor(0.0f, 0.0f, 0.0f, 0.33f), 2.5, NPoint()};
inline constexpr NShadowInfo kNShadowMedium                 = {NColor(0.0f, 0.0f, 0.0f, 0.50f), 3.0, NPoint()};
inline constexpr NShadowInfo kNShadowHeavy                  = {NColor(0.0f, 0.0f, 0.0f, 1.00f), 3.0, NPoint()};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCGContext : public NCFObject<CGContextRef>
{
public:
										NCGContext(NImage& theImage);
										NCGContext();


	// Save/restore the state
	void                                SaveState();
	void                                RestoreState();


	// Adjust the transform
	void                                Scale(    CGFloat x, CGFloat y);
	void                                Translate(CGFloat x, CGFloat y);
	void                                Rotate(NRadians theAngle);


	// Adjust the flip state
	bool                                IsFlipped() const;
	void                                Flip(       const NRectangle& theRect);


	// Adjust the clip
	void                                ClipToPath(NClipping theClip = NClipping::NonZero);
	void                                ClipToRect(const NRectangle& theRect);


	// Set the output
	void                                SetOutput(NImage& theImage);


	// Set the line width
	void                                SetLineWidth(CGFloat theWidth);


	// Set the line cap
	void                                SetLineCap(CGLineCap theCap);


	// Set the line join
	void                                SetLineJoin(CGLineJoin theJoin);


	// Set the line dash
	void                                SetLineDash();
	void                                SetLineDash(const NVectorCGFloat& theDash, CGFloat thePhase = 0.0);


	// Set the alpha
	void                                SetAlpha(CGFloat theAlpha);


	// Set the anti-aliasing
	void                                SetAntiAlias(bool antiAlias);


	// Get/set the interpolation quality
	CGInterpolationQuality              GetInterpolation() const;
	void                                SetInterpolation(CGInterpolationQuality theValue);


	// Set the blend mode
	void                                SetBlendMode(CGBlendMode theMode);


	// Set the shadow
	//
	// Shadows can be disabled by setting the shadow alpha to 0.0.
	void                                SetShadow(const NShadowInfo& theShadow = kNShadowDefault);


	// Set the fill color
	void                                SetFillColor(const NColor& theColor);


	// Set the stroke color
	void                                SetStrokeColor(const NColor& theColor);


	// Begin/close a path
	void                                BeginPath();
	void                                ClosePath();


	// Move to a point
	void                                MoveTo(CGFloat x, CGFloat y);
	void                                MoveTo(const NPoint& thePoint);


	// Add a line
	void                                AddLineTo(CGFloat x, CGFloat y);
	void                                AddLineTo(const NPoint& thePoint);


	// Add a rectangle
	void                                AddRect(const NRectangle& theRect, CGFloat cornerRadius = 0.0);


	// Add a circle
	void                                AddCircle(const NPoint& theCenter, CGFloat theRadius);


	// Add an ellipse
	void                                AddEllipse(const NRectangle& theRect);


	// Add an arc
	void                                AddArc(const NPoint& theCenter,
											   CGFloat       theRadius,
											   CGFloat       angleStart,
											   CGFloat       angleEnd,
											   NOrientation  theOrientation);
	void                                AddArcToPoint(const NPoint& point1, const NPoint& point2, CGFloat theRadius);


	// Add a polyline
	void                                AddPolyline(const NVectorPoint& thePoints);


	// Add a polygon
	void                                AddPolygon(const NVectorPoint& thePoints);


	// Draw a rectangle
	void                                FillRect(  const NRectangle& theRect);
	void                                StrokeRect(const NRectangle& theRect);
	void                                DrawRect(  const NRectangle& theRect, CGPathDrawingMode theMode = kCGPathFillStroke);
	void                                ClearRect( const NRectangle& theRect);


	// Draw the current path
	void                                FillPath();
	void                                StrokePath();
	void                                DrawPath(CGPathDrawingMode theMode = kCGPathFillStroke);


	// Draw an image
	void                                DrawImage(const NImage& theImage, const NRectangle& theRect);


	// Draw a shading
	void                                DrawShading(const NCGShading& theShading);


	// Draw text
	void                                DrawText(const NString&    theText,
												 const NRectangle& theRect,
												 NPosition         alignTo  = NPosition::Left,
												 CTFontUIFontType  fontID   = kCTFontUIFontUser,
												 CGFloat           fontSize = 0.0);


	// Measure text
	NRectangle                          GetTextBounds(const NString&   theText,
													  CTFontUIFontType fontID   = kCTFontUIFontUser,
													  CGFloat          fontSize = 0.0);


private:
	NCTLine                             PrepareTextLine(const NString&   theText,
														CTFontUIFontType fontID,
														CGFloat          fontSize,
														NRectangle&      textRect);

	NCTLine                             GetCTLine(const NString& theText);
	NCGFont                             GetCGFont(CTFontUIFontType fontID, CGFloat fontSize);

private:
	bool                                mIsFlipped;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NScopedCGState final
{
public:
										NScopedCGState(NCGContext& theContext);
									   ~NScopedCGState();

										NScopedCGState(const NScopedCGState& otherState) = delete;
	NScopedCGState&                     operator=(     const NScopedCGState& otherState) = delete;

										NScopedCGState(NScopedCGState&& otherState) = delete;
	NScopedCGState&                     operator=(     NScopedCGState&& otherState) = delete;


private:
	NCGContext&                         mContext;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCGContext.inl"



#endif // NCGCONTEXT_H
