/*	NAME:
		NCGContext.h

	DESCRIPTION:
		CoreGraphics context object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGCONTEXT_HDR
#define NCGCONTEXT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NGeometryUtilities.h"
#include "NThemeUtilities.h"
#include "NCarbonEvent.h"
#include "NCFObject.h"
#include "NCGShading.h"
#include "NCGImage.h"
#include "NColor.h"
#include "NIcon.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGContext : public NCFObject {
public:
										NCGContext(const NCarbonEvent &theEvent);

										NCGContext(CGContextRef cgContext=NULL);

										NCGContext(NData				&theData,
													UInt32				theWidth,
													UInt32				theHeight,
													CGBitmapInfo		theInfo        = kCGImageAlphaPremultipliedFirst,
													UInt32				rowBytes       = 0,
													HIThemeOrientation	theOrientation = kHIThemeOrientationInverted);
										
	virtual								~NCGContext(void);


	// Is the context inverted?
	//
	// A context is "inverted" when its orientation is kHIThemeOrientationInverted.
	bool								IsInverted(void) const;


	// Get/set the context orientation
	HIThemeOrientation					GetOrientation(void) const;
	void								SetOrientation(HIThemeOrientation theOrientation);


	// Save/restore the graphics state
	void								SaveState(   void);
	void								RestoreState(void);


	// Flush the context
	void								Flush(void);
	

	// Get/set the transform
	CGAffineTransform					GetCTM(void);
	void								ConcatCTM(const CGAffineTransform &theTransform);


	// Adjust the transform
	void								ScaleCTM(     float x, float y);
	void								TranslateCTM( float x, float y);
	void								RotateCTM(    float   theAngle);
	void								FlipCTM(const CGRect &theRect);


	// Adjust the clip
	void								ClipToPath(bool evenOddClip=false);
	void								ClipToRect(const CGRect &theRect);
	void								ClipToMask(const CGRect &theRect, const NCGImage &theMask);


	// Set the line width
	void								SetLineWidth(float theWidth);


	// Set the line cap
	void								SetLineCap(CGLineCap theCap);


	// Set the line join
	void								SetLineJoin(CGLineJoin theJoin);


	// Set the line dash
	void								SetLineDash(void);
	void								SetLineDash(const Float32List &theDash, float thePhase=0.0f);
	

	// Set the alpha
	void								SetAlpha(float theAlpha);


	// Set the anti-aliasing
	void								SetAntiAlias(bool antiAlias);


	// Set the blend mode
	void								SetBlendMode(CGBlendMode theMode);


	// Get/set the interpolation quality
	CGInterpolationQuality				GetInterpolation(void) const;
	void								SetInterpolation(CGInterpolationQuality theValue);
	

	// Set the stroke color
	void								SetStrokeColor(const NColor &theColor);
	void								SetStrokeColor(ThemeBrush    theBrush);


	// Set the fill color
	void								SetFillColor(const NColor &theColor);
	void								SetFillColor(ThemeBrush    theBrush);


	// Set the shadow
	//
	// Shadows can be disabled by setting the shadow alpha to 0.0.
	void								SetShadow(const CGShadowInfo &theInfo=kCGShadowDefault);


	// Set the text color
	void								SetTextColor(ThemeTextColor theColor);


    // Set the text mode
    void                                SetTextMode(CGTextDrawingMode theMode);


	// Begin/close a path
	void								BeginPath(void);
	void								ClosePath(void);


	// Move to a point
	void								MoveTo(float x, float y);
	void								MoveTo(const CGPoint &thePoint);


	// Add a line
	void								AddLineTo(float x, float y);
	void								AddLineTo(const CGPoint &thePoint);


	// Add a rectangle
	//
	// If a corner radius is specified, the rectangle is a "round rect".
	void								AddRect(const CGRect &theRect, float cornerRadius=0.0f);


	// Add a circle
	void								AddCircle(const CGPoint &theCenter, float theRadius);


	// Add an ellipse
	void								AddEllipse(const CGRect &theRect);


	// Add an arc
	void								AddArc(       const CGPoint &theCenter, float theRadius, float angleStart, float angleEnd, bool clockWise);
	void								AddArcToPoint(const CGPoint &point1, const CGPoint &point2, float theRadius);


	// Add a polyline
	void								AddPolyline(const CGPointList &thePoints);


	// Add a polygon
	void								AddPolygon(const CGPointList &thePoints);


	// Draw a rectangle
	void								ClearRect( const CGRect &theRect);
	void								FillRect(  const CGRect &theRect);
	void								StrokeRect(const CGRect &theRect);


	// Draw the current path
	void								DrawPath(CGPathDrawingMode theMode=kCGPathFillStroke);
	void								FillPath(  void);
	void								StrokePath(void);


	// Draw an image
	void								DrawImage(const NCGImage &theImage, const CGRect &theRect);


	// Draw an icon
	void								DrawIcon(const NIcon		&theIcon,
												 const CGRect		&theRect,
												 IconAlignmentType	theAlignment = kAlignAbsoluteCenter,
												 IconTransformType	theTransform = kTransformNone,
												 const NColor		&theColor    = kColorBlack,
												 PlotIconRefFlags	theFlags     = kPlotIconRefNormalFlags);


	// Draw a shading
	void								DrawShading(const NCGShading &theShading);


	// Draw a drag hilite
	void								DrawDragHilite(const CGRect &theRect);


	// Draw an HITheme text box
	bool								DrawThemeText(const NString				&theText,
														const HIRect			&theRect,
														const HIThemeTextInfo	&theInfo = kThemeTextAppFontCenter);


private:
	void								InitializeSelf(CGContextRef			cgContext,
														bool				takeOwnership,
														HIThemeOrientation	theOrientation);


private:
	HIThemeOrientation					mOrientation;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StContextState {
public:
										StContextState(NCGContext &theContext);
	virtual								~StContextState(void);


private:
	NCGContext							&mContext;
};





#endif // NCGCONTEXT_HDR


