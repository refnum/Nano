/*	NAME:
		CShapeView.cpp

	DESCRIPTION:
		Shape view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NSystemUtilities.h"
#include "NCGContext.h"

#include "CShapeView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Properties
//
// These values may be archived to disk, and can never change.
static const NString kSVShape										= "SVShape";
static const NString kSVColor										= "SVColor";


// Tags
static const OSType kTagShape										= 'shap';
static const OSType kTagEdgeWidth									= 'widt';
static const OSType kTagEdgeStyle									= 'styl';
static const OSType kTagColorStroke									= 'cstr';
static const OSType kTagColorFill									= 'cfil';


// Misc
static const float       kShapeInset								= 1.0f;
static const Float32List kLineStyleDot								= vector(2.0f, 2.0f);
static const Float32List kLineStyleDash								= vector(4.0f, 4.0f);





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(CShapeView, "com.nano.cshapeview", kHIViewClassID);





//============================================================================
//		CShapeView::CShapeView : Constructor.
//----------------------------------------------------------------------------
CShapeView::CShapeView(HIWindowRef		theWindow,
						const HIRect	&theRect,
						ShapeType		theShape,
						EdgeWidth		edgeWidth,
						EdgeStyle		edgeStyle,
						const NColor	&colorStroke,
						const NColor	&colorFill)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetShape(theShape);

	SetEdgeWidth(edgeWidth);
	SetEdgeStyle(edgeStyle);

	SetStrokeColor(colorStroke);
	SetFillColor(  colorFill);
}





//============================================================================
//		CShapeView::~CShapeView : Destructor.
//----------------------------------------------------------------------------
CShapeView::~CShapeView(void)
{
}





//============================================================================
//		CShapeView::GetShape : Get the shape.
//----------------------------------------------------------------------------
ShapeType CShapeView::GetShape(void) const
{


	// Get the shape
	return(mShape);
}





//============================================================================
//		CShapeView::SetShape : Set the shape.
//----------------------------------------------------------------------------
void CShapeView::SetShape(ShapeType theShape)
{


	// Set the shape
	mShape = theShape;
	
	SetNeedsDisplay();
}





//============================================================================
//		CShapeView::GetEdgeWidth : Get the edge width.
//----------------------------------------------------------------------------
EdgeWidth CShapeView::GetEdgeWidth(void) const
{


	// Get the edge width
	return(mEdgeWidth);
}





//============================================================================
//		CShapeView::SetEdgeWidth : Set the edge width.
//----------------------------------------------------------------------------
void CShapeView::SetEdgeWidth(EdgeWidth theWidth)
{


	// Set the edge width
	mEdgeWidth = theWidth;
	
	SetNeedsDisplay();
}





//============================================================================
//		CShapeView::GetEdgeStyle : Get the edge style.
//----------------------------------------------------------------------------
EdgeStyle CShapeView::GetEdgeStyle(void) const
{


	// Get the edge style
	return(mEdgeStyle);
}





//============================================================================
//		CShapeView::SetEdgeStyle : Set the edge style.
//----------------------------------------------------------------------------
void CShapeView::SetEdgeStyle(EdgeStyle theStyle)
{


	// Set the edge style
	mEdgeStyle = theStyle;
	
	SetNeedsDisplay();
}





//============================================================================
//		CShapeView::GetStrokeColor : Get the stroke color.
//----------------------------------------------------------------------------
NColor CShapeView::GetStrokeColor(void) const
{


	// Get the color
	return(mColorStroke);
}





//============================================================================
//		CShapeView::SetStrokeColor : Set the stroke color.
//----------------------------------------------------------------------------
void CShapeView::SetStrokeColor(const NColor &theColor)
{


	// Set the color
	mColorStroke = theColor;
	
	SetNeedsDisplay();
}





//============================================================================
//		CShapeView::GetFillColor : Get the fill color.
//----------------------------------------------------------------------------
NColor CShapeView::GetFillColor(void) const
{


	// Get the color
	return(mColorFill);
}





//============================================================================
//		CShapeView::SetFillColor : Set the fill color.
//----------------------------------------------------------------------------
void CShapeView::SetFillColor(const NColor &theColor)
{


	// Set the color
	mColorFill = theColor;
	
	SetNeedsDisplay();
}





//============================================================================
//		CShapeView::EncodeState : Encode the view state.
//----------------------------------------------------------------------------
OSStatus CShapeView::EncodeState(NDictionary &theState) const
{	OSStatus	theErr;



	// Encode the base class
	theErr = NHIView::EncodeState(theState);
	if (theErr != noErr)
		return(theErr);



	// Encode our state
	theState.SetValueOSType(kSVShape, (OSType) mShape);
	theState.SetValueColor( kSVColor, mColorFill);
	
	return(theErr);
}





//============================================================================
//		CShapeView::DecodeState : Decode the view state.
//----------------------------------------------------------------------------
OSStatus CShapeView::DecodeState(const NDictionary &theState)
{	NDictionary		theDictionary;
	OSStatus		theErr;



	// Decode the base class
	theErr = NHIView::DecodeState(theState);
	if (theErr != noErr)
		return(theErr);



	// Decode our state
	SetShape((ShapeType) theState.GetValueOSType(kSVShape));
	SetFillColor(        theState.GetValueColor( kSVColor));

	return(theErr);
}





//============================================================================
//		CShapeView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void CShapeView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw },
										{ kEventClassNone,    kEventKindNone    } };



	// Register for events
	AddEvents(theEvents);



	// Initialize ourselves
	SetShape(kShapeTriangle);

	SetEdgeWidth(kEdgeWidth1);
	SetEdgeStyle(kEdgeStyleSolid);
	
	SetStrokeColor(kColorBlack);
	SetFillColor(  kColorWhite);



	// Apply our properties
	if (HasProperty(                           kPropertyNano, kTagShape))
		SetShape((ShapeType) GetPropertyOSType(kPropertyNano, kTagShape));

	if (HasProperty(                               kPropertyNano, kTagEdgeWidth))
		SetEdgeWidth((EdgeWidth) GetPropertyOSType(kPropertyNano, kTagEdgeWidth));

	if (HasProperty(                               kPropertyNano, kTagEdgeStyle))
		SetEdgeStyle((EdgeStyle) GetPropertyOSType(kPropertyNano, kTagEdgeStyle));

	if (HasProperty(                     kPropertyNano, kTagColorStroke))
		SetStrokeColor(GetPropertyString(kPropertyNano, kTagColorStroke));

	if (HasProperty(                     kPropertyNano, kTagColorFill))
		SetFillColor(  GetPropertyString(kPropertyNano, kTagColorFill));
}





//============================================================================
//		CShapeView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus CShapeView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	CGPointList		thePoints;
	HIRect			theRect;



	// Prepare to draw
	//
	// We inset the rect so that the shape border lies within our bounds.
	theRect = CGRectInset(GetBounds(), kShapeInset, kShapeInset);
	
	switch (mEdgeStyle) {
		case kEdgeStyleSolid:
			cgContext.SetLineDash();
			break;
		
		case kEdgeStyleDash:
			cgContext.SetLineDash(kLineStyleDash);
			break;
		
		case kEdgeStyleDot:
			cgContext.SetLineDash(kLineStyleDot);
			break;
		}

	cgContext.SetLineWidth(  mEdgeWidth);
	cgContext.SetStrokeColor(mColorStroke);
	cgContext.SetFillColor(  mColorFill);



	// Draw the shape
	switch (mShape) {
		case kShapeTriangle:
			thePoints.push_back(CGPointMake(theRect.origin.x + (theRect.size.width / 2.0f), theRect.origin.y));
			thePoints.push_back(CGPointMake(theRect.origin.x +  theRect.size.width,         theRect.origin.y + theRect.size.height));
			thePoints.push_back(CGPointMake(theRect.origin.x,                               theRect.origin.y + theRect.size.height));
			cgContext.AddPolygon(thePoints);
			break;

		case kShapeRectangle:
			cgContext.AddRect(theRect);
			break;

		case kShapeCircle:
			cgContext.AddEllipse(theRect);
			break;

		default:
			NN_LOG("Unknown shape type: %d", mShape);
			break;
		}

	cgContext.DrawPath();

	return(noErr);
}
