/*	NAME:
		CShapeView.h

	DESCRIPTION:
		Shape view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CSHAPEVIEW_HDR
#define CSHAPEVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NColor.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Shape types
//
// These values are used for kTagShape, and can never change.
typedef enum {
	kShapeTriangle						= 'trig',
	kShapeRectangle						= 'rect',
	kShapeCircle						= 'circ'
} ShapeType;


// Edge widths
//
// These values are archived to disk, and should never change.
typedef enum {
	kEdgeWidth1							= 1,
	kEdgeWidth2							= 2,
	kEdgeWidth4							= 4,
	kEdgeWidth8							= 8
} EdgeWidth;


// Edge styles
//
// These values are archived to disk, and should never change.
typedef enum {
	kEdgeStyleSolid						= 1,
	kEdgeStyleDot						= 2,
	kEdgeStyleDash						= 3
} EdgeStyle;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CShapeView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(CShapeView);

										CShapeView(HIWindowRef		theWindow,
													const HIRect	&theRect,
													ShapeType		theShape     = kShapeRectangle,
													EdgeWidth		edgeWidth    = kEdgeWidth1,
													EdgeStyle		edgeStyle    = kEdgeStyleSolid,
													const NColor	&colorStroke = kColorBlack,
													const NColor	&colorFill   = kColorWhite);

	virtual								~CShapeView(void);


	// Get/set the shape
	ShapeType							GetShape(void) const;
	void								SetShape(ShapeType theShape);


	// Get/set the edge width
	EdgeWidth							GetEdgeWidth(void) const;
	void								SetEdgeWidth(EdgeWidth theWidth);


	// Get/set the edge style
	EdgeStyle							GetEdgeStyle(void) const;
	void								SetEdgeStyle(EdgeStyle theStyle);


	// Get/set the stroke color
	NColor								GetStrokeColor(void) const;
	void								SetStrokeColor(const NColor &theColor);


	// Get/set the fill color
	NColor								GetFillColor(void) const;
	void								SetFillColor(const NColor &theColor);


	// Encode/decode the view state
	OSStatus							EncodeState(      NDictionary &theState) const;
	OSStatus							DecodeState(const NDictionary &theState);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	ShapeType							mShape;
	EdgeWidth							mEdgeWidth;
	EdgeStyle							mEdgeStyle;
	
	NColor								mColorStroke;
	NColor								mColorFill;
};





#endif // CSHAPEVIEW_HDR


