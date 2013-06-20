/*	NAME:
		NCGContext.cpp

	DESCRIPTION:
		CoreGraphics context.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreFoundation.h"
#include "NMathUtilities.h"
#include "NCoreGraphics.h"
#include "NCGImage.h"
#include "NCGColor.h"
#include "NCGContext.h"





//============================================================================
//		Build constants
//----------------------------------------------------------------------------
#define DEBUG_LAYOUT												0





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		ToCG : Convert to CG.
//----------------------------------------------------------------------------
static const CGFloat *ToCG(NIndex theSize, const Float32 *theValues, CGFloatList &cgValues)
{
#if NN_TARGET_ARCH_64
	NIndex	n;


	cgValues.resize(theSize);

	for (n = 0; n < theSize; n++)
		cgValues[n] = (CGFloat) theValues[n];
	
	return(&cgValues[0]);

#else
	NN_ASSERT(sizeof(CGFloat) == sizeof(Float32));
	NN_UNUSED(theSize);
	NN_UNUSED(cgValues);

	return((const CGFloat *) theValues);
#endif
}





//============================================================================
//		ToCG : Convert to CG.
//----------------------------------------------------------------------------
static const CGPoint *ToCG(NIndex theSize, const NPoint *theValues, CGPointList &cgValues)
{
#if NN_TARGET_ARCH_64
	NIndex	n;


	cgValues.resize(theSize);

	for (n = 0; n < theSize; n++)
		cgValues[n] = CGPointMake(theValues[n].x, theValues[n].y);
	
	return(&cgValues[0]);

#else
	NN_ASSERT(sizeof(CGPoint) == sizeof(NPoint));
	NN_UNUSED(theSize);
	NN_UNUSED(cgValues);

	return((const CGPoint *) theValues);
#endif
}





#pragma mark NCGContext
//============================================================================
//		NCGContext::NCGContext : Constructor.
//----------------------------------------------------------------------------
NCGContext::NCGContext(NImage &theImage, bool flipContext)
{	CGBitmapInfo	bitmapInfo;
	CGContextRef	cgContext;



	// Get the state we need
	bitmapInfo = NCGImage::GetBitmapInfo(theImage.GetFormat());

	cgContext = CGBitmapContextCreate(	theImage.GetPixels(),
										theImage.GetWidth(),
										theImage.GetHeight(),
										theImage.GetBitsPerComponent(),
										theImage.GetBytesPerRow(),
										NCGColor::GetDeviceRGB(),
										bitmapInfo);



	// Initialize ourselves
	InitialiseSelf(cgContext, true);

	if (flipContext)
		Flip(theImage.GetBounds());
}





//============================================================================
//		NCGContext::NCGContext : Constructor.
//----------------------------------------------------------------------------
NCGContext::NCGContext(CGContextRef cgContext, bool takeOwnership)
{


	// Initialize ourselves
	InitialiseSelf(cgContext, takeOwnership);
}





//============================================================================
//		NCGContext::NCGContext : Constructor.
//----------------------------------------------------------------------------
NCGContext::NCGContext(void)
{


	// Initialize ourselves
	InitialiseSelf();
}





//============================================================================
//		NCGContext::~NCGContext : Destructor.
//----------------------------------------------------------------------------
NCGContext::~NCGContext(void)
{
}





//============================================================================
//		NCGContext::SaveState : Save the state.
//----------------------------------------------------------------------------
void NCGContext::SaveState(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Save the state
	CGContextSaveGState(*this);
}





//============================================================================
//		NCGContext::RestoreState : Restore the state.
//----------------------------------------------------------------------------
void NCGContext::RestoreState(void)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Restore the state
	CGContextRestoreGState(*this);
}





//============================================================================
//		NCGContext::Scale : Apply a scale transform.
//----------------------------------------------------------------------------
void NCGContext::Scale(Float32 x, Float32 y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Scale the transform
	CGContextScaleCTM(*this, x, y);
}





//============================================================================
//		NCGContext::Translate : Apply a translation transform.
//----------------------------------------------------------------------------
void NCGContext::Translate(Float32 x, Float32 y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Translate the transform
	CGContextTranslateCTM(*this, x, y);
}





//============================================================================
//		NCGContext::Rotate : Apply a rotation transform.
//----------------------------------------------------------------------------
void NCGContext::Rotate(NRadians theAngle)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Rotate the transform
	CGContextRotateCTM(*this, (CGFloat) theAngle);
}





//============================================================================
//		NCGContext::IsFlipped : Is the context flipped?
//----------------------------------------------------------------------------
bool NCGContext::IsFlipped(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the state
	return(mIsFlipped);
}





//============================================================================
//		NCGContext::Flip : Flip the context.
//----------------------------------------------------------------------------
void NCGContext::Flip(const NRectangle &theRect)
{


	// Flip the context
	mIsFlipped = !mIsFlipped;

	Translate(0.0f, theRect.origin.y + theRect.GetMaxY());
	Scale(1.0f, -1.0f);
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
void NCGContext::ClipToRect(const NRectangle &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Clip to the rectangle
	CGContextClipToRect(*this, ToCG(theRect));
}





//============================================================================
//		NCGContext::SetLineWidth : Set the line width.
//----------------------------------------------------------------------------
void NCGContext::SetLineWidth(Float32 theWidth)
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
void NCGContext::SetLineDash(const Float32List &theDash, Float32 thePhase)
{	NIndex			theSize;
	CGFloatList		cgDash;



	// Validate our parameters and state
	NN_ASSERT(thePhase >= 0.0f);
	NN_ASSERT(IsValid());



	// Set the line dash
	if (theDash.empty())
		CGContextSetLineDash(*this, 0.0f, NULL, 0);
	else
		{
		theSize = theDash.size();
		CGContextSetLineDash(*this, thePhase, ToCG(theSize, &theDash[0], cgDash), theSize);
		}
}





//============================================================================
//		NCGContext::SetAlpha : Set the alpha.
//----------------------------------------------------------------------------
void NCGContext::SetAlpha(Float32 theAlpha)
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
//		NCGContext::SetShadow : Set the shadow.
//----------------------------------------------------------------------------
void NCGContext::SetShadow(const NShadowInfo &theInfo)
{	NCGColor	cgColor(theInfo.color);



	// Set the shadow
	CGContextSetShadowWithColor(	*this,
									CGSizeMake(theInfo.offset.x, theInfo.offset.y),
									theInfo.blur,
									cgColor.GetObject());
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
void NCGContext::MoveTo(Float32 x, Float32 y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Move to the point
	CGContextMoveToPoint(*this, x, y);
}





//============================================================================
//		NCGContext::MoveTo : Move to a point.
//----------------------------------------------------------------------------
void NCGContext::MoveTo(const NPoint &thePoint)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Move to the point
	CGContextMoveToPoint(*this, thePoint.x, thePoint.y);
}





//============================================================================
//		NCGContext::AddLineTo : Add a line to a point.
//----------------------------------------------------------------------------
void NCGContext::AddLineTo(Float32 x, Float32 y)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Add a line to the point
	CGContextAddLineToPoint(*this, x, y);
}





//============================================================================
//		NCGContext::AddLineTo : Add a line to a point.
//----------------------------------------------------------------------------
void NCGContext::AddLineTo(const NPoint &thePoint)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Add a line to the point
	CGContextAddLineToPoint(*this, thePoint.x, thePoint.y);
}





//============================================================================
//		NCGContext::AddRect : Add a rectangle.
//----------------------------------------------------------------------------
void NCGContext::AddRect(const NRectangle &theRect, Float32 cornerRadius)
{	NPoint		leftMiddle, topLeft, topRight, bottomRight;



	// Validate our parameters and state
	NN_ASSERT(cornerRadius >= 0.0f);
	NN_ASSERT(IsValid());



	// Add a rectangle
	if (NMathUtilities::IsZero(cornerRadius))
		CGContextAddRect(*this, ToCG(theRect));
	
	
	// Add a round-rect
	else
		{
		// Get the state we need
		leftMiddle.x = theRect.origin.x;
		leftMiddle.y = theRect.origin.y + (theRect.size.height / 2.0f);
		
		topLeft.x = theRect.origin.x;
		topLeft.y = theRect.origin.y + theRect.size.height;
		
		topRight.x = theRect.origin.x + theRect.size.width;
		topRight.y = theRect.origin.y + theRect.size.height;
		
		bottomRight.x = theRect.origin.x + theRect.size.width;
		bottomRight.y = theRect.origin.y;


		// Add the rect
		CGContextMoveToPoint(   *this,     leftMiddle.x,     leftMiddle.y);
		CGContextAddArcToPoint( *this, theRect.origin.x, theRect.origin.y,    bottomRight.x,    bottomRight.y, cornerRadius);
		CGContextAddArcToPoint( *this,    bottomRight.x,    bottomRight.y,       topRight.x,       topRight.y, cornerRadius);
		CGContextAddArcToPoint( *this,       topRight.x,       topRight.y,        topLeft.x,        topLeft.y, cornerRadius);
		CGContextAddArcToPoint( *this,        topLeft.x,        topLeft.y, theRect.origin.x, theRect.origin.y, cornerRadius);
		CGContextAddLineToPoint(*this,      leftMiddle.x,      leftMiddle.y);
		}
}





//============================================================================
//		NCGContext::AddCircle : Add a circle.
//----------------------------------------------------------------------------
void NCGContext::AddCircle(const NPoint &theCenter, Float32 theRadius)
{


	// Validate our parameters and state
	NN_ASSERT(theRadius > 0.0f);
	NN_ASSERT(IsValid());



	// Add a circle
	CGContextMoveToPoint(*this, theCenter.x + theRadius, theCenter.y);
	CGContextAddArc(     *this, theCenter.x,             theCenter.y, theRadius, 0.0f, (Float32) (M_PI * 2.0), true);
}





//============================================================================
//		NCGContext::AddEllipse : Add an ellipse.
//----------------------------------------------------------------------------
void NCGContext::AddEllipse(const NRectangle &theRect)
{	Float32		halfWidth, halfHeight;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the state we need
	halfWidth  = theRect.size.width  / 2.0f;
	halfHeight = theRect.size.height / 2.0f;


	// Add the ellipse
	CGContextSaveGState(   *this);
	CGContextTranslateCTM( *this, theRect.origin.x + halfWidth, theRect.origin.y + halfHeight);
	CGContextScaleCTM(     *this, halfWidth, halfHeight);
	CGContextAddArc(       *this, 0.0f, 0.0f, 1.0f, 0.0f, (Float32) (M_PI * 2.0), true);
	CGContextRestoreGState(*this);
}





//============================================================================
//		NCGContext::AddArc : Add an arc.
//----------------------------------------------------------------------------
void NCGContext::AddArc(const NPoint &theCenter, Float32 theRadius, Float32 angleStart, Float32 angleEnd, bool clockWise)
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
void NCGContext::AddArcToPoint(const NPoint &point1, const NPoint &point2, Float32 theRadius)
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
void NCGContext::AddPolyline(const NPointList &thePoints)
{


	// Add the polyline
	AddPolyline(thePoints.size(), &thePoints[0]);
}





//============================================================================
//		NCGContext::AddPolyline : Add a polyline.
//----------------------------------------------------------------------------
void NCGContext::AddPolyline(NIndex numPoints, const NPoint *thePoints)
{	CGPointList		cgPoints;



	// Validate our parameters and state
	NN_ASSERT(numPoints >= 2);
	NN_ASSERT(IsValid());



	// Add the polyline
	CGContextAddLines(*this, ToCG(numPoints, thePoints, cgPoints), numPoints);
}





//============================================================================
//		NCGContext::AddPolygon : Add a polygon.
//----------------------------------------------------------------------------
void NCGContext::AddPolygon(const NPointList &thePoints)
{	CGPointList		cgPoints;
	NIndex			theSize;



	// Validate our parameters and state
	NN_ASSERT(thePoints.size() >= 3);
	NN_ASSERT(IsValid());



	// Add the polygon
	theSize = thePoints.size();
	
	CGContextAddLines(*this, ToCG(theSize, &thePoints[0], cgPoints), theSize);
	CGContextAddLineToPoint(*this, thePoints[0].x, thePoints[0].y);
}





//============================================================================
//		NCGContext::FillRect : Fill a rectangle.
//----------------------------------------------------------------------------
void NCGContext::FillRect(const NRectangle &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Fill the rectangle
	CGContextFillRect(*this, ToCG(theRect));
}





//============================================================================
//		NCGContext::StrokeRect : Stroke a rectangle.
//----------------------------------------------------------------------------
void NCGContext::StrokeRect(const NRectangle &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Stroke the rectangle
	CGContextStrokeRect(*this, ToCG(theRect));
}





//============================================================================
//		NCGContext::DrawRect : Draw a rectangle.
//----------------------------------------------------------------------------
void NCGContext::DrawRect(const NRectangle &theRect, CGPathDrawingMode theMode)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Draw the rectangle
	AddRect(theRect);
	DrawPath(theMode);
}





//============================================================================
//		NCGContext::ClearRect : Clear a rectangle.
//----------------------------------------------------------------------------
void NCGContext::ClearRect(const NRectangle &theRect)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Clear the rectangle
	CGContextClearRect(*this, ToCG(theRect));
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
//		NCGContext::DrawImage : Draw an image.
//----------------------------------------------------------------------------
void NCGContext::DrawImage(const NImage &theImage, const NRectangle &theRect)
{	NCGImage	cgImage(theImage);



	// Validate our parameters and state
	NN_ASSERT(theImage.IsValid());
	NN_ASSERT(!theRect.IsEmpty());
	NN_ASSERT(IsValid());



	// Draw the image
	CGContextDrawImage(*this, ToCG(theRect), cgImage.GetObject());
}





//============================================================================
//		NCGContext::DrawShading : Draw a shading.
//----------------------------------------------------------------------------
void NCGContext::DrawShading(const NCGShading &theShading)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Draw the shading
	CGContextDrawShading(*this, theShading);
}





//============================================================================
//		NCGContext::DrawImage : Draw an image.
//----------------------------------------------------------------------------
void NCGContext::DrawText(const NString &theText, const NRectangle &theRect, NPosition alignTo, CTFontUIFontType fontID, Float32 fontSize)
{	StCGContextState	saveState(*this);
	NCFObject			ctLine, cgFont;
	NRectangle			textRect;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the state we need
	ctLine = GetCTLine(theText);
	cgFont = GetCGFont(fontID, fontSize);

	if (!ctLine.IsValid() || !cgFont.IsValid())
		return;



	// Perform the layout
	CGContextSetFont(*this, cgFont);

	if (IsFlipped())
		CGContextSetTextMatrix(*this, CGAffineTransformMakeScale(1.0f, -1.0f));
	else
		CGContextSetTextMatrix(*this, CGAffineTransformIdentity);

	textRect = ToNN(CTLineGetImageBounds(ctLine, *this));
	textRect.SetPosition(theRect, alignTo);



	// Draw the layout
	if (DEBUG_LAYOUT)
		{	StCGContextState	debugState(*this);

		SetAlpha(0.5f);
		SetLineWidth(1.0f);
		SetStrokeColor(kNColorGreen);

		SetFillColor(kNColorBlue);
		DrawRect(theRect);

		SetFillColor(kNColorRed);
		DrawRect(textRect);
		}



	// Draw the text
	CGContextSetTextPosition(*this, textRect.GetMinX(), textRect.GetMaxY());
	CTLineDraw(ctLine,       *this);
}





//============================================================================
//		NCGContext::GetTextBounds : Get the text bounds.
//----------------------------------------------------------------------------
NRectangle NCGContext::GetTextBounds(const NString &theText, CTFontUIFontType fontID, Float32 fontSize)
{	NCFObject		ctLine, cgFont;
	NRectangle		theRect;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the state we need
	ctLine = GetCTLine(theText);
	cgFont = GetCGFont(fontID, fontSize);

	if (!ctLine.IsValid() || !cgFont.IsValid())
		return(theRect);



	// Measure the text
	CGContextSetFont(*this, cgFont);
	theRect = ToNN(CTLineGetImageBounds(ctLine, *this));

	return(theRect);
}





#pragma mark private
//============================================================================
//		NCGContext::InitialiseSelf : Initialise ourselves.
//----------------------------------------------------------------------------
void NCGContext::InitialiseSelf(CGContextRef cgContext, bool takeOwnership)
{	NCFDictionary		textAttributes;



	// Get the state we need
	textAttributes.SetValue(ToNN(kCTForegroundColorFromContextAttributeName), true);



	// Initialise ourselves
	if (cgContext != NULL)
		SetObject(cgContext, takeOwnership);

	mIsFlipped      = false;
	mTextAttributes = textAttributes.GetObject();
}





#pragma mark private
//============================================================================
//		NCGContext::GetCTLine : Get a CTLine.
//----------------------------------------------------------------------------
NCFObject NCGContext::GetCTLine(const NString &theText)
{	NCFObject		cfString, ctLine;



	// Create the line
	if (cfString.SetObject(CFAttributedStringCreate(kCFAllocatorNano, ToCF(theText), mTextAttributes)))
		ctLine.SetObject(CTLineCreateWithAttributedString(cfString));

	return(ctLine);
}





//============================================================================
//		NCGContext::GetCGFont : Get a CGFont.
//----------------------------------------------------------------------------
NCFObject NCGContext::GetCGFont(CTFontUIFontType fontID, Float32 fontSize)
{	NCFObject	ctFont, cgFont;



	// Create the font
	if (ctFont.SetObject(CTFontCreateUIFontForLanguage(fontID, fontSize, NULL)))
		cgFont.SetObject(CTFontCopyGraphicsFont(ctFont, NULL));
	
	return(cgFont);
}





#pragma mark StCGContextState
//============================================================================
//		StCGContextState::StCGContextState : Constructor.
//----------------------------------------------------------------------------
StCGContextState::StCGContextState(NCGContext &theContext)
{


	// Initialize ourselves
	mContext = &theContext;
	mContext->SaveState();
}





//============================================================================
//		StCGContextState::~StCGContextState : Destructor.
//----------------------------------------------------------------------------
StCGContextState::~StCGContextState(void)
{


	// Clean up
	mContext->RestoreState();
}


