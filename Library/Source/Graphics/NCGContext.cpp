/*	NAME:
		NCGContext.cpp

	DESCRIPTION:
		CoreGraphics context object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NMathUtilities.h"
#include "NCGContext.h"





//============================================================================
//		NCGContext::NCGContext : Constructor.
//----------------------------------------------------------------------------
NCGContext::NCGContext(const NCarbonEvent &theEvent)
{


	// Initialize ourselves
	InitializeSelf(theEvent.GetCGContext(), false, kHIThemeOrientationNormal);
}





//============================================================================
//		NCGContext::NCGContext : Constructor.
//----------------------------------------------------------------------------
NCGContext::NCGContext(CGContextRef cgContext)
{


	// Initialize ourselves
	InitializeSelf(cgContext, false, kHIThemeOrientationInverted);
}





//============================================================================
//		NCGContext::NCGContext : Constructor.
//----------------------------------------------------------------------------
NCGContext::NCGContext(NData &theData, UInt32 theWidth, UInt32 theHeight, CGBitmapInfo theInfo, UInt32 rowBytes, HIThemeOrientation	theOrientation)
{	CGContextRef	cgContext;



	// Supply default data
	if (rowBytes == 0)
		rowBytes = theWidth * sizeof(UInt32);

	if (theData.IsEmpty())
		theData.SetSize(rowBytes * theHeight);
	else
		NN_ASSERT(theData.GetSize() <= (rowBytes * theHeight));



	// Supply default byte order
	//
	// If the image endian-ness isn't specified, we replace kCGBitmapByteOrderDefault with
	// the native endian-ness (since kCGBitmapByteOrderDefault is always big-endian, even
	// on little endian hosts).
	//
	// This allows little-endian hosts to manipulate context data with the same bit-shifts
	// as big-endian, and avoids the need to swizzle if the context is drawn to the screen.
	if ((theInfo & kCGBitmapByteOrderMask) == kCGBitmapByteOrderDefault)
		theInfo |= kCGBitmapByteOrder32Host;



	// Create the context
	cgContext = CGBitmapContextCreate(theData.GetData(), theWidth, theHeight, 8, rowBytes,
									  NCoreGraphicsUtilities::GetColorSpaceRGB(), theInfo);

	if (theOrientation == kHIThemeOrientationNormal)
		{
		CGContextTranslateCTM(cgContext, 0.0f, theHeight);
		CGContextScaleCTM(    cgContext, 1.0f, -1.0f);
		}



	// Initialize ourselves
	InitializeSelf(cgContext, true, theOrientation);
}





//============================================================================
//		NCGContext::~NCGContext : Destructor.
//----------------------------------------------------------------------------
NCGContext::~NCGContext(void)
{
}





//============================================================================
//		NCGContext::IsInverted : Is the context inverted?
//----------------------------------------------------------------------------
bool NCGContext::IsInverted(void) const
{


	// Check the orientation
	return(mOrientation == kHIThemeOrientationInverted);
}





//============================================================================
//		NCGContext::GetOrientation : Get the orientation.
//----------------------------------------------------------------------------
HIThemeOrientation NCGContext::GetOrientation(void) const
{


	// Get the orientation
	return(mOrientation);
}





//============================================================================
//		NCGContext::SetOrientation : Set the orientation.
//----------------------------------------------------------------------------
void NCGContext::SetOrientation(HIThemeOrientation theOrientation)
{


	// Set the orientation
	mOrientation = theOrientation;
}





//============================================================================
//		NCGContext::SaveState : Save the graphics state.
//----------------------------------------------------------------------------
void NCGContext::SaveState(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Save the graphics state
	CGContextSaveGState(*this);
}





//============================================================================
//		NCGContext::RestoreState : Restore the graphics state.
//----------------------------------------------------------------------------
void NCGContext::RestoreState(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Restore the graphics state
	CGContextRestoreGState(*this);
}





//============================================================================
//		NCGContext::Flush : Flush the context.
//----------------------------------------------------------------------------
void NCGContext::Flush(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Flush the context
	CGContextFlush(*this);
}





//============================================================================
//		NCGContext::GetCTM : Get the transform.
//----------------------------------------------------------------------------
CGAffineTransform NCGContext::GetCTM(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the transform
	return(CGContextGetCTM(*this));
}





//============================================================================
//		NCGContext::ConcatCTM : Concatenate a transform.
//----------------------------------------------------------------------------
void NCGContext::ConcatCTM(const CGAffineTransform &theTransform)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Concatenate the transform
	CGContextConcatCTM(*this, theTransform);
}





//============================================================================
//		NCGContext::ScaleCTM : Scale the context transform.
//----------------------------------------------------------------------------
void NCGContext::ScaleCTM(float x, float y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Scale the transform
	CGContextScaleCTM(*this, x, y);
}





//============================================================================
//		NCGContext::TranslateCTM : Translate the context transform.
//----------------------------------------------------------------------------
void NCGContext::TranslateCTM(float x, float y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Translate the transform
	CGContextTranslateCTM(*this, x, y);
}





//============================================================================
//		NCGContext::RotateCTM : Rotate the context transform.
//----------------------------------------------------------------------------
void NCGContext::RotateCTM(float theAngle)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Rotate the transform
	CGContextRotateCTM(*this, theAngle);
}





//============================================================================
//		NCGContext::FlipCTM : Flip the context tranform.
//----------------------------------------------------------------------------
void NCGContext::FlipCTM(const CGRect &theRect)
{


	// Flip the context
	TranslateCTM(0.0f, theRect.origin.y + CGRectGetMaxY(theRect));
	ScaleCTM(1.0f, -1.0f);
}





//============================================================================
//		NCGContext::ClipToPath : Clip to the current path.
//----------------------------------------------------------------------------
void NCGContext::ClipToPath(bool evenOddClip)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Clip to the current path
	if (evenOddClip)
		CGContextEOClip(*this);
	else
		CGContextClip(  *this);
}





//============================================================================
//		NCGContext::ClipToRect : Clip to a rectangle.
//----------------------------------------------------------------------------
void NCGContext::ClipToRect(const CGRect &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Clip to the rectangle
	CGContextClipToRect(*this, theRect);
}





//============================================================================
//		NCGContext::ClipToMask : Clip to a mask.
//----------------------------------------------------------------------------
void NCGContext::ClipToMask(const CGRect &theRect, const NCGImage &theMask)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Clip to the mask
	//
	// Note that CGContextClipToMask assumes an inverted context; when clipping
	// within a non-inverted context, you must use FlipCTM (or equivalent) to
	// invert the context.
	//
	// Unlike DrawIcon, ClipToMask can not perform this step automatically for
	// callers since the transform needs to be in effect after we return as the
	// clip has no effect until something is drawn.
	CGContextClipToMask(*this, theRect, theMask);
}





//============================================================================
//		NCGContext::SetLineWidth : Set the line width.
//----------------------------------------------------------------------------
void NCGContext::SetLineWidth(float theWidth)
{


	// Validate our parameters and state
	NN_ASSERT(theWidth > 0.0f);
	NN_ASSERT(IsValid());



	// Set the line width
	CGContextSetLineWidth(*this, theWidth);
}





//============================================================================
//		NCGContext::SetLineCap : Set the line cap.
//----------------------------------------------------------------------------
void NCGContext::SetLineCap(CGLineCap theCap)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the line cap
	CGContextSetLineCap(*this, theCap);
}





//============================================================================
//		NCGContext::SetLineJoin : Set the line join.
//----------------------------------------------------------------------------
void NCGContext::SetLineJoin(CGLineJoin theJoin)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the line join
	CGContextSetLineJoin(*this, theJoin);
}





//============================================================================
//		NCGContext::SetLineDash : Set the line dash.
//----------------------------------------------------------------------------
void NCGContext::SetLineDash(void)
{


	// Set the line dash
	CGContextSetLineDash(*this, 0.0f, NULL, 0);
}





//============================================================================
//		NCGContext::SetLineDash : Set the line dash.
//----------------------------------------------------------------------------
void NCGContext::SetLineDash(const Float32List &theDash, float thePhase)
{


	// Validate our parameters and state
	NN_ASSERT(thePhase >= 0.0f);
	NN_ASSERT(IsValid());



	// Set the line dash
	if (theDash.empty())
		CGContextSetLineDash(*this, 0.0f, NULL, 0);
	else
		CGContextSetLineDash(*this, thePhase, &theDash[0], theDash.size());
}





//============================================================================
//		NCGContext::SetAlpha : Set the alpha.
//----------------------------------------------------------------------------
void NCGContext::SetAlpha(float theAlpha)
{


	// Validate our parameters and state
	NN_ASSERT(theAlpha > 0.0f);
	NN_ASSERT(IsValid());



	// Set the alpha
	CGContextSetAlpha(*this, theAlpha);
}





//============================================================================
//		NCGContext::SetAntiAlias : Set the anti-aliasing.
//----------------------------------------------------------------------------
void NCGContext::SetAntiAlias(bool antiAlias)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the anti-aliasing
	CGContextSetShouldAntialias(*this, antiAlias);
}





//============================================================================
//		NCGContext::SetBlendMode : Set the blend mode.
//----------------------------------------------------------------------------
void NCGContext::SetBlendMode(CGBlendMode theMode)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the blend mode
	CGContextSetBlendMode(*this, theMode);
}





//============================================================================
//      NCGContext::GetInterpolation : Get the interpolation quality.
//----------------------------------------------------------------------------
CGInterpolationQuality NCGContext::GetInterpolation(void) const
{


    // Validate our state
    NN_ASSERT(IsValid());



    // Set the anti-aliasing
    return(CGContextGetInterpolationQuality(*this));
}





//============================================================================
//      NCGContext::SetInterpolation : Set the interpolation quality.
//----------------------------------------------------------------------------
void NCGContext::SetInterpolation(CGInterpolationQuality theValue)
{


    // Validate our state
    NN_ASSERT(IsValid());



    // Set the anti-aliasing
    CGContextSetInterpolationQuality(*this, theValue);
}





//============================================================================
//		NCGContext::SetStrokeColor : Set the stroke color.
//----------------------------------------------------------------------------
void NCGContext::SetStrokeColor(const NColor &theColor)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the stroke color
	CGContextSetRGBStrokeColor(*this, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue(), theColor.GetAlpha());
}





//============================================================================
//		NCGContext::SetStrokeColor : Set the stroke color.
//----------------------------------------------------------------------------
void NCGContext::SetStrokeColor(ThemeBrush theBrush)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the stroke color
	HIThemeSetStroke(theBrush, NULL, *this, mOrientation);
}





//============================================================================
//		NCGContext::SetFillColor : Set the fill color.
//----------------------------------------------------------------------------
void NCGContext::SetFillColor(const NColor &theColor)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the fill color
	CGContextSetRGBFillColor(*this, theColor.GetRed(), theColor.GetGreen(), theColor.GetBlue(), theColor.GetAlpha());
}





//============================================================================
//		NCGContext::SetFillColor : Set the fill color.
//----------------------------------------------------------------------------
void NCGContext::SetFillColor(ThemeBrush theBrush)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the fill color
	HIThemeSetFill(theBrush, NULL, *this, mOrientation);
}





//============================================================================
//		NCGContext::SetShadow : Set the shadow.
//----------------------------------------------------------------------------
void NCGContext::SetShadow(const CGShadowInfo &theInfo)
{	NCFObject	cfColor;



	// Set the shadow color
	if (cfColor.Set(theInfo.color.CopyColor()))
		CGContextSetShadowWithColor(*this, theInfo.offset, theInfo.blur, cfColor);
}





//============================================================================
//		NCGContext::SetTextColor : Set the text color.
//----------------------------------------------------------------------------
void NCGContext::SetTextColor(ThemeTextColor theColor)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the text color
	HIThemeSetTextFill(theColor, NULL, *this, mOrientation);
}





//============================================================================
//      NCGContext::SetTextMode : Set the text mode.
//----------------------------------------------------------------------------
void NCGContext::SetTextMode(CGTextDrawingMode theMode)
{


    // Validate our state
    NN_ASSERT(IsValid());



    // Set the text mode
    CGContextSetTextDrawingMode(*this, theMode);
}





//============================================================================
//		NCGContext::BeginPath : Begin a sub-path.
//----------------------------------------------------------------------------
void NCGContext::BeginPath(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Begin the path
	CGContextBeginPath(*this);
}





//============================================================================
//		NCGContext::ClosePath : Close a sub-path.
//----------------------------------------------------------------------------
void NCGContext::ClosePath(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Close the path
	CGContextClosePath(*this);
}





//============================================================================
//		NCGContext::MoveTo : Move to a point.
//----------------------------------------------------------------------------
void NCGContext::MoveTo(float x, float y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Move to the point
	CGContextMoveToPoint(*this, x, y);
}





//============================================================================
//		NCGContext::MoveTo : Move to a point.
//----------------------------------------------------------------------------
void NCGContext::MoveTo(const CGPoint &thePoint)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Move to the point
	CGContextMoveToPoint(*this, thePoint.x, thePoint.y);
}





//============================================================================
//		NCGContext::AddLineTo : Add a line to a point.
//----------------------------------------------------------------------------
void NCGContext::AddLineTo(float x, float y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Add a line to the point
	CGContextAddLineToPoint(*this, x, y);
}





//============================================================================
//		NCGContext::AddLineTo : Add a line to a point.
//----------------------------------------------------------------------------
void NCGContext::AddLineTo(const CGPoint &thePoint)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Add a line to the point
	CGContextAddLineToPoint(*this, thePoint.x, thePoint.y);
}





//============================================================================
//		NCGContext::AddRect : Add a rectangle.
//----------------------------------------------------------------------------
void NCGContext::AddRect(const CGRect &theRect, float cornerRadius)
{	CGPoint		topMiddle, topLeft, topRight, bottomRight;



	// Validate our parameters and state
	NN_ASSERT(cornerRadius >= 0.0f);
	NN_ASSERT(IsValid());



	// Add a rectangle
	if (NMathUtilities::IsZero(cornerRadius))
		CGContextAddRect(*this, theRect);
	
	
	// Add a round-rect
	else
		{
		// Get the state we need
		topMiddle.x = theRect.origin.x;
		topMiddle.y = theRect.origin.y + (theRect.size.height / 2.0f);
		
		topLeft.x = theRect.origin.x;
		topLeft.y = theRect.origin.y + theRect.size.height;
		
		topRight.x = theRect.origin.x + theRect.size.width;
		topRight.y = theRect.origin.y + theRect.size.height;
		
		bottomRight.x = theRect.origin.x + theRect.size.width;
		bottomRight.y = theRect.origin.y;


		// Add the rect
		CGContextMoveToPoint(   *this,      topMiddle.x,      topMiddle.y);
		CGContextAddArcToPoint( *this, theRect.origin.x, theRect.origin.y,    bottomRight.x,    bottomRight.y, cornerRadius);
		CGContextAddArcToPoint( *this,    bottomRight.x,    bottomRight.y,       topRight.x,       topRight.y, cornerRadius);
		CGContextAddArcToPoint( *this,       topRight.x,       topRight.y,        topLeft.x,        topLeft.y, cornerRadius);
		CGContextAddArcToPoint( *this,        topLeft.x,        topLeft.y, theRect.origin.x, theRect.origin.y, cornerRadius);
		CGContextAddLineToPoint(*this,      topMiddle.x,      topMiddle.y);
		}
}





//============================================================================
//		NCGContext::AddCircle : Add a circle.
//----------------------------------------------------------------------------
void NCGContext::AddCircle(const CGPoint &theCenter, float theRadius)
{


	// Validate our parameters and state
	NN_ASSERT(theRadius > 0.0f);
	NN_ASSERT(IsValid());



	// Add a circle
	CGContextMoveToPoint(*this, theCenter.x + theRadius, theCenter.y);
	CGContextAddArc(     *this, theCenter.x,             theCenter.y, theRadius, 0.0f, (float) (M_PI * 2.0), true);
}





//============================================================================
//		NCGContext::AddEllipse : Add an ellipse.
//----------------------------------------------------------------------------
void NCGContext::AddEllipse(const CGRect &theRect)
{	float	halfWidth, halfHeight;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the state we need
	halfWidth  = theRect.size.width  / 2.0f;
	halfHeight = theRect.size.height / 2.0f;


	// Add the ellipse
	CGContextSaveGState(   *this);
	CGContextTranslateCTM( *this, theRect.origin.x + halfWidth, theRect.origin.y + halfHeight);
	CGContextScaleCTM(     *this, halfWidth, halfHeight);
	CGContextAddArc(       *this, 0.0f, 0.0f, 1.0f, 0.0f, (float) (M_PI * 2.0), true);
	CGContextRestoreGState(*this);
}





//============================================================================
//		NCGContext::AddArc : Add an arc.
//----------------------------------------------------------------------------
void NCGContext::AddArc(const CGPoint &theCenter, float theRadius, float angleStart, float angleEnd, bool clockWise)
{


	// Validate our parameters and state
	NN_ASSERT(theRadius > 0.0f);
	NN_ASSERT(IsValid());



	// Add an arc
	CGContextAddArc(*this, theCenter.x, theCenter.y, theRadius, angleStart, angleEnd, clockWise);
}





//============================================================================
//		NCGContext::AddArcToPoint : Add an arc.
//----------------------------------------------------------------------------
void NCGContext::AddArcToPoint(const CGPoint &point1, const CGPoint &point2, float theRadius)
{


	// Validate our parameters and state
	NN_ASSERT(theRadius > 0.0f);
	NN_ASSERT(IsValid());



	// Add an arc
	CGContextAddArcToPoint(*this, point1.x, point1.y, point2.x, point2.y, theRadius);
}





//============================================================================
//		NCGContext::AddPolyline : Add a polyline.
//----------------------------------------------------------------------------
void NCGContext::AddPolyline(const CGPointList &thePoints)
{


	// Validate our parameters and state
	NN_ASSERT(thePoints.size() >= 2);
	NN_ASSERT(IsValid());



	// Add the polyline
	CGContextAddLines(*this, &thePoints[0], thePoints.size());
}





//============================================================================
//		NCGContext::AddPolygon : Add a polygon.
//----------------------------------------------------------------------------
void NCGContext::AddPolygon(const CGPointList &thePoints)
{


	// Validate our parameters and state
	NN_ASSERT(thePoints.size() >= 3);
	NN_ASSERT(IsValid());



	// Add the polygon
	CGContextAddLines(      *this, &thePoints[0],   thePoints.size());
	CGContextAddLineToPoint(*this,  thePoints[0].x, thePoints[0].y);
}





//============================================================================
//		NCGContext::ClearRect : Clear a rectangle.
//----------------------------------------------------------------------------
void NCGContext::ClearRect(const CGRect &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Clear the rectangle
	CGContextClearRect(*this, theRect);
}





//============================================================================
//		NCGContext::FillRect : Fill a rectangle.
//----------------------------------------------------------------------------
void NCGContext::FillRect(const CGRect &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Fill the rectangle
	CGContextFillRect(*this, theRect);
}





//============================================================================
//		NCGContext::StrokeRect : Stroke a rectangle.
//----------------------------------------------------------------------------
void NCGContext::StrokeRect(const CGRect &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Stroke the rectangle
	CGContextStrokeRect(*this, theRect);
}





//============================================================================
//		NCGContext::DrawPath : Draw the current path.
//----------------------------------------------------------------------------
void NCGContext::DrawPath(CGPathDrawingMode theMode)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Draw the path
	CGContextDrawPath(*this, theMode);
}





//============================================================================
//		NCGContext::FillPath : Fill the current path.
//----------------------------------------------------------------------------
void NCGContext::FillPath(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Fill the path
	CGContextFillPath(*this);
}





//============================================================================
//		NCGContext::StrokePath : Stroke the current path.
//----------------------------------------------------------------------------
void NCGContext::StrokePath(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Stroke the path
	CGContextStrokePath(*this);
}





//============================================================================
//		NCGContext::DrawImage : Draw an image.
//----------------------------------------------------------------------------
void NCGContext::DrawImage(const NCGImage &theImage, const CGRect &theRect)
{	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(theImage.IsValid());
	NN_ASSERT(IsValid());



	// Draw the image
	if (IsInverted())
		CGContextDrawImage(*this, theRect, theImage);
	else
		{
		theErr = HIViewDrawCGImage(*this, &theRect, theImage);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NCGContext::DrawIcon : Draw an icon.
//----------------------------------------------------------------------------
void NCGContext::DrawIcon(const NIcon			&theIcon,
						  const CGRect			&theRect,
						  IconAlignmentType		theAlignment,
						  IconTransformType		theTransform,
						  const NColor			&theColor,
						  PlotIconRefFlags		theFlags)
{	RGBColor	qdColor;
	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(theIcon.IsValid());
	NN_ASSERT(IsValid());



	// Get the state we need
	theColor.GetColor(qdColor);



	// Draw the icon
	//
	// PlotIconRefInContext assumes a inverted context.
	if (!IsInverted())
		{
		SaveState();
		FlipCTM(theRect);
		}

	theErr = PlotIconRefInContext(*this, &theRect, theAlignment, theTransform,
									&qdColor, theFlags, theIcon);
	NN_ASSERT_NOERR(theErr);

	if (!IsInverted())
		RestoreState();
}





//============================================================================
//		NCGContext::DrawShading : Draw a shading.
//----------------------------------------------------------------------------
void NCGContext::DrawShading(const NCGShading &theShading)
{


	// Validate our parameters and state
	NN_ASSERT(IsValid());



	// Draw the shading
	if (theShading.GetMode() != kShadingNone)
		CGContextDrawShading(*this, theShading);
}





//============================================================================
//		NCGContext::DrawDragHilite : Draw a drag hilite.
//----------------------------------------------------------------------------
void NCGContext::DrawDragHilite(const CGRect &theRect)
{


	// Draw the drag hilite
	SetLineWidth(1.0f);
	SetStrokeColor(kThemeBrushDragHilite);

	StrokeRect(CGRectInset(theRect, 1.0f, 1.0f));
}





//============================================================================
//		NCGContext::DrawThemeText : Draw a theme text box.
//----------------------------------------------------------------------------
bool NCGContext::DrawThemeText(const NString			&theText,
								const HIRect			&theRect,
								const HIThemeTextInfo	&theInfo)
{	HIThemeTextInfo		mutableInfo;
	OSStatus			theErr;



	// Get the state we need
	mutableInfo = theInfo;



	// Draw the text box
	theErr = HIThemeDrawTextBox(theText, &theRect, &mutableInfo, *this, mOrientation);
	NN_ASSERT_NOERR(theErr);
	
	return(mutableInfo.truncationHappened);
}





//============================================================================
//		NCGContext::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCGContext::InitializeSelf(CGContextRef cgContext, bool takeOwnership, HIThemeOrientation theOrientation)
{


	// Initialize ourselves
	mOrientation = theOrientation;

	Set(cgContext, takeOwnership);

	if (cgContext != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		StContextState::StContextState : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
StContextState::StContextState(NCGContext &theContext)
				: mContext(theContext)
{


	// Initialize ourselves
	mContext.SaveState();
}





//============================================================================
//		StContextState::~StContextState : Destructor.
//----------------------------------------------------------------------------
StContextState::~StContextState(void)
{


	// Clean up
	mContext.RestoreState();
}


