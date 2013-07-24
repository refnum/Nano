/*	NAME:
		NCGContext.h

	DESCRIPTION:
		CoreGraphics context.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGCONTEXT_HDR
#define NCGCONTEXT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphics.h"
#include "NCGShading.h"
#include "NCFObject.h"
#include "NImage.h"
#include "NCache.h"
#include "NMutex.h"
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Shadow
typedef struct {
	NColor		color;
	float32_t	blur;
	NPoint		offset;
} NShadowInfo;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Shadows
static const NShadowInfo kNShadowNone								= { NColor(0.0f, 0.0f, 0.0f, 0.00f), 0.0f, NPoint() };
static const NShadowInfo kNShadowLight								= { NColor(0.0f, 0.0f, 0.0f, 0.25f), 2.0f, NPoint() };
static const NShadowInfo kNShadowDefault							= { NColor(0.0f, 0.0f, 0.0f, 0.33f), 2.5f, NPoint() };
static const NShadowInfo kNShadowMedium								= { NColor(0.0f, 0.0f, 0.0f, 0.50f), 3.0f, NPoint() };
static const NShadowInfo kNShadowHeavy								= { NColor(0.0f, 0.0f, 0.0f, 1.00f), 3.0f, NPoint() };





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGContext :	public NCFObject,
					public NUncopyable {
public:
										NCGContext(NImage &theImage);
										NCGContext(CGContextRef cgContext, bool takeOwnership);

										NCGContext(void);
	virtual							   ~NCGContext(void);


	// Set the target
	void								SetTarget(NImage &theImage);
	void								SetTarget(CGContextRef cgContext, bool takeOwnership);


	// Save/restore the state
	void								SaveState(   void);
	void								RestoreState(void);


	// Adjust the transform
	void								Scale(     float32_t x, float32_t y);
	void								Translate( float32_t x, float32_t y);
	void								Rotate(    NRadians    theAngle);


	// Adjust the flip state
	bool								IsFlipped(void) const;
	void								Flip(const NRectangle &theRect);


	// Adjust the clip
	void								ClipToPath(bool evenOddClip=false);
	void								ClipToRect(const NRectangle &theRect);


	// Set the line width
	void								SetLineWidth(float32_t theWidth);


	// Set the line cap
	void								SetLineCap(CGLineCap theCap);


	// Set the line join
	void								SetLineJoin(CGLineJoin theJoin);


	// Set the line dash
	void								SetLineDash(void);
	void								SetLineDash(const Float32List &theDash, float thePhase=0.0f);
	

	// Set the alpha
	void								SetAlpha(float32_t theAlpha);


	// Set the anti-aliasing
	void								SetAntiAlias(bool antiAlias);


	// Get/set the interpolation quality
	CGInterpolationQuality				GetInterpolation(void) const;
	void								SetInterpolation(CGInterpolationQuality theValue);


	// Set the blend mode
	void								SetBlendMode(CGBlendMode theMode);


	// Set the shadow
	//
	// Shadows can be disabled by setting the shadow alpha to 0.0.
	void								SetShadow(const NShadowInfo &theInfo=kNShadowDefault);


	// Set the fill color
	void								SetFillColor(const NColor &theColor);
	

	// Set the stroke color
	void								SetStrokeColor(const NColor &theColor);


	// Begin/close a path
	void								BeginPath(void);
	void								ClosePath(void);


	// Move to a point
	void								MoveTo(float32_t x, float32_t y);
	void								MoveTo(const NPoint &thePoint);


	// Add a line
	void								AddLineTo(float32_t x, float32_t y);
	void								AddLineTo(const NPoint &thePoint);


	// Add a rectangle
	void								AddRect(const NRectangle &theRect, float32_t cornerRadius=0.0f);


	// Add a circle
	void								AddCircle(const NPoint &theCenter, float32_t theRadius);


	// Add an ellipse
	void								AddEllipse(const NRectangle &theRect);


	// Add an arc
	void								AddArc(       const NPoint &theCenter, float32_t theRadius, float32_t angleStart, float32_t angleEnd, bool clockWise);
	void								AddArcToPoint(const NPoint &point1, const NPoint &point2, float32_t theRadius);


	// Add a polyline
	void								AddPolyline(const NPointList &thePoints);
	void								AddPolyline(NIndex numPoints, const NPoint *thePoints);


	// Add a polygon
	void								AddPolygon(const NPointList &thePoints);


	// Draw a rectangle
	void								FillRect(  const NRectangle &theRect);
	void								StrokeRect(const NRectangle &theRect);
	void								DrawRect(  const NRectangle &theRect, CGPathDrawingMode theMode=kCGPathFillStroke);
	void								ClearRect( const NRectangle &theRect);


	// Draw the current path
	void								FillPath(  void);
	void								StrokePath(void);
	void								DrawPath(CGPathDrawingMode theMode=kCGPathFillStroke);


	// Draw an image
	void								DrawImage(const NImage &theImage, const NRectangle &theRect);


	// Draw a shading
	void								DrawShading(const NCGShading &theShading);


	// Draw text
	void								DrawText(const NString			   &theText,
												 const NRectangle		   &theRect,
													   NPosition			alignTo  = kNPositionLeft,
													   CTFontUIFontType		fontID   = kCTFontUIFontUser,
													   float32_t			fontSize = 0.0);


	// Measure text
	NRectangle							GetTextBounds(const NString			   &theText,
															CTFontUIFontType	fontID   = kCTFontUIFontUser,
															float32_t			fontSize = 0.0);


private:
	void								InitialiseSelf(void);


private:
	NCFObject							PrepareTextLine(	const NString		&theText,
															CTFontUIFontType	fontID,
															float32_t			fontSize,
															NRectangle			&textRect);

	NCFObject							GetCTLine(const NString &theText);
	NCFObject							GetCGFont(CTFontUIFontType fontID, float32_t fontSize);


private:
	bool								mIsFlipped;
	
	static NMutex						mTextLock;
	static NCache						mTextCache;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StCGContextState {
public:
										StCGContextState(NCGContext &theContext);
	virtual							   ~StCGContextState(void);


private:
	NCGContext						   *mContext;
};





#endif // NCGCONTEXT_HDR


