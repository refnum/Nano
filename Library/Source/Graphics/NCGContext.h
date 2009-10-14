/*	NAME:
		NCGContext.h

	DESCRIPTION:
		CoreGraphics context object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCGCONTEXT_HDR
#define NCGCONTEXT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"
#include "NGeometryUtilities.h"
#include "NCarbonEvent.h"
#include "NCGShading.h"
#include "NCGImage.h"
#include "NColor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
extern const HIThemeTextInfo kThemeTextSystemFontLeft;
extern const HIThemeTextInfo kThemeTextSystemFontCenter;
extern const HIThemeTextInfo kThemeTextSystemFontRight;

extern const HIThemeTextInfo kThemeTextAppFontLeft;
extern const HIThemeTextInfo kThemeTextAppFontCenter;
extern const HIThemeTextInfo kThemeTextAppFontRight;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCGContext : public NCFObject {
public:
										NCGContext(const NCarbonEvent	&theEvent,
													bool				saveState = true);

										NCGContext(CGContextRef			cgContext,
													bool				saveState      = true,
													HIThemeOrientation	theOrientation = kHIThemeOrientationInverted);

										NCGContext(NData			&theData,
													UInt32			theWidth,
													UInt32			theHeight,
													CGBitmapInfo	theInfo  = kCGImageAlphaPremultipliedFirst,
													UInt32			rowBytes = 0);

										NCGContext(void);
	virtual								~NCGContext(void);


	// Get the context orientation
	HIThemeOrientation					GetOrientation(void) const;
	
	
	// Is the context inverted?
	//
	// A context is "inverted" when its orientation is kHIThemeOrientationInverted.
	bool								IsInverted(void) const;
	

	// Save/restore the graphics state
	void								SaveState(   void);
	void								RestoreState(void);


	// Get/set the transform
	CGAffineTransform					GetCTM(void);
	void								ConcatCTM(const CGAffineTransform &theTransform);


	// Adjust the transform
	void								ScaleCTM(    float x, float y);
	void								TranslateCTM(float x, float y);
	void								RotateCTM(   float theAngle);


	// Clip to a rectangle
	void								ClipToRect(const CGRect &theRect);


	// Set the line width
	void								SetLineWidth(float theWidth);


	// Set the line dash
	void								SetLineDash(float thePhase, const float *dashLengths, UInt32 numLengths);


	// Set the alpha
	void								SetAlpha(float theAlpha);


	// Set the stroke color
	void								SetStrokeColor(const NColor &theColor);
	void								SetStrokeColor(ThemeBrush    theBrush);


	// Set the fill color
	void								SetFillColor(const NColor &theColor);
	void								SetFillColor(ThemeBrush    theBrush);


	// Set the shadow color
	//
	// Shadows can be disabled by setting the shadow color to kColorTransparent.
	void								SetShadowColor(const NColor		&theColor  = kColorBlack,
														float			theBlur    = 3.0f,
														const CGSize	&theOffset = CGSizeZero);


	// Move to a point
	void								MoveToPoint(float x, float y);
	void								MoveToPoint(const CGPoint &thePoint);


	// Add a line
	void								AddLineToPoint(float x, float y);
	void								AddLineToPoint(const CGPoint &thePoint);


	// Add a rectangle
	//
	// If a corner radius is specified, the rectangle is a "round rect".
	void								AddRect(const CGRect &theRect, float cornerRadius=0.0f);


	// Add a circle
	void								AddCircle(const CGPoint &theCentre, float theRadius);


	// Add an ellipse
	void								AddEllipse(const CGRect &theRect);


	// Add a polygon
	void								AddPolygon(const CGPointList &thePoints);


	// Draw a rectangle
	void								ClearRect( const CGRect &theRect);
	void								FillRect(  const CGRect &theRect);
	void								StrokeRect(const CGRect &theRect);


	// Draw the current path
	void								DrawPath(CGPathDrawingMode theMode);
	void								FillPath(  void);
	void								StrokePath(void);


	// Draw an image
	void								DrawImage(const NCGImage &theImage, const CGRect &theRect);


	// Draw a shading
	void								DrawShading(const NCGShading &theShading);


	// Draw a drag hilite
	void								DrawDragHilite(const CGRect &theRect);


	// Draw an HITheme text box
	bool								DrawThemeText(const NString				&theString,
														const HIRect			&theRect,
														const HIThemeTextInfo	&theInfo = kThemeTextAppFontCenter);


private:
	void								InitializeSelf(CGContextRef			cgContext,
														bool				saveState,
														HIThemeOrientation	theOrientation);


private:
	bool								mRestoreState;
	HIThemeOrientation					mOrientation;
};





#endif // NCGCONTEXT_HDR


