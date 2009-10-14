/*	NAME:
		CShapeView.cpp

	DESCRIPTION:
		Shape view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
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
static const float kShapeInset										= 1.0f;
static const float kLineStyleDot[]									= { 2, 2 };
static const float kLineStyleDash[]									= { 4, 4 };





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
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	NHIObjectFactory::SetNextInstance(this);
	
	theErr = HIObjectCreate(kClassID, NULL, (HIObjectRef *) &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



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
//		CShapeView::EncodeProperties : Encode the properties.
//----------------------------------------------------------------------------
OSStatus CShapeView::EncodeProperties(NDictionary &theProperties)
{	OSStatus	theErr;



	// Encode the base class
	theErr = NHIView::EncodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Encode the properties
	theProperties.SetValueOSType(kSVShape, (OSType) mShape);
	theProperties.SetValueColor( kSVColor, mColorFill);
	
	return(theErr);
}





//============================================================================
//		CShapeView::DecodeProperties : Decode the properties.
//----------------------------------------------------------------------------
OSStatus CShapeView::DecodeProperties(const NDictionary &theProperties)
{	NDictionary		theDictionary;
	OSStatus		theErr;



	// Decode the base class
	theErr = NHIView::DecodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Decode the properties
	SetShape((ShapeType) theProperties.GetValueOSType(kSVShape));
	SetFillColor(        theProperties.GetValueColor( kSVColor));

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
			cgContext.SetLineDash(0.0f, NULL, 0);
			break;
		
		case kEdgeStyleDash:
			cgContext.SetLineDash(0.0f, kLineStyleDash, GET_ARRAY_SIZE(kLineStyleDash));
			break;
		
		case kEdgeStyleDot:
			cgContext.SetLineDash(0.0f, kLineStyleDot, GET_ARRAY_SIZE(kLineStyleDot));
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
			NN_ASSERT_MSG("Unknown shape type: %d", mShape);
			break;
		}

	cgContext.DrawPath(kCGPathFillStroke);

	return(noErr);
}
