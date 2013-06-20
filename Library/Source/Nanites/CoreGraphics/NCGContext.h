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
#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Shadow
typedef struct {
	NColor		color;
	Float32		blur;
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
class NCGContext : public NCFObject {
public:
										NCGContext(NImage &theImage, bool flipContext=true);
										NCGContext(CGContextRef cgContext, bool takeOwnership);

										NCGContext(void);
	virtual							   ~NCGContext(void);


	// Save/restore the state
	void								SaveState(   void);
	void								RestoreState(void);


	// Adjust the transform
	void								Scale(     Float32 x, Float32 y);
	void								Translate( Float32 x, Float32 y);
	void								Rotate(    NRadians    theAngle);


	// Adjust the flip state
	bool								IsFlipped(void) const;
	void								Flip(const NRectangle &theRect);


	// Adjust the clip
	void								ClipToPath(bool evenOddClip=false);
	void								ClipToRect(const NRectangle &theRect);


	// Set the line width
	void								SetLineWidth(Float32 theWidth);


	// Set the line cap
	void								SetLineCap(CGLineCap theCap);


	// Set the line join
	void								SetLineJoin(CGLineJoin theJoin);


	// Set the line dash
	void								SetLineDash(void);
	void								SetLineDash(const Float32List &theDash, float thePhase=0.0f);
	

	// Set the alpha
	void								SetAlpha(Float32 theAlpha);


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
	void								MoveTo(Float32 x, Float32 y);
	void								MoveTo(const NPoint &thePoint);


	// Add a line
	void								AddLineTo(Float32 x, Float32 y);
	void								AddLineTo(const NPoint &thePoint);


	// Add a rectangle
	void								AddRect(const NRectangle &theRect, Float32 cornerRadius=0.0f);


	// Add a circle
	void								AddCircle(const NPoint &theCenter, Float32 theRadius);


	// Add an ellipse
	void								AddEllipse(const NRectangle &theRect);


	// Add an arc
	void								AddArc(       const NPoint &theCenter, Float32 theRadius, Float32 angleStart, Float32 angleEnd, bool clockWise);
	void								AddArcToPoint(const NPoint &point1, const NPoint &point2, Float32 theRadius);


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
													   Float32				fontSize = 0.0);


	// Measure text
	NRectangle							GetTextBounds(const NString			   &theText,
															CTFontUIFontType	fontID   = kCTFontUIFontUser,
															Float32				fontSize = 0.0);


private:
	void								InitialiseSelf(	CGContextRef	cgContext     = NULL,
														bool			takeOwnership = false);


private:
	NCFObject							GetCTLine(const NString &theText);
	NCFObject							GetCGFont(CTFontUIFontType fontID, Float32 fontSize);


private:
	bool								mIsFlipped;
	NCFObject							mTextAttributes;
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


