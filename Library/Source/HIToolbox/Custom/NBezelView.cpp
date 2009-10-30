/*	NAME:
		NBezelView.cpp

	DESCRIPTION:
		Bezel view.
		
		A "bezel view" provides a translucent rounded rectangle, which can
		be placed in a transparent window to provide the background.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGContext.h"
#include "NBezelView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagBezelAlpha									= 'alph';
static const OSType kTagBezelCorner									= 'corn';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NBezelView, "com.nano.nbezelview", kHIViewClassID);





//============================================================================
//		NBezelView::NBezelView : Constructor.
//----------------------------------------------------------------------------
NBezelView::NBezelView(HIWindowRef		theWindow,
						const HIRect	&theRect,
						float			theAlpha,
						float			theCorner)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetAlpha(theAlpha);
	SetCorner(theCorner);
}





//============================================================================
//		NBezelView::~NBezelView : Destructor.
//----------------------------------------------------------------------------
NBezelView::~NBezelView(void)
{
}





//============================================================================
//		NBezelView::GetAlpha : Get the alpha factor.
//----------------------------------------------------------------------------
float NBezelView::GetAlpha(void) const
{


	// Get the alpha
	return(mAlpha);
}





//============================================================================
//		NBezelView::SetAlpha : Set the alpha factor.
//----------------------------------------------------------------------------
void NBezelView::SetAlpha(float theValue)
{


	// Set the alpha
	mAlpha = theValue;
	
	SetNeedsDisplay();
}





//============================================================================
//		NBezelView::GetCorner : Get the corner radius.
//----------------------------------------------------------------------------
float NBezelView::GetCorner(void) const
{


	// Get the corner
	return(mCorner);
}





//============================================================================
//		NBezelView::SetCorner : Set the corner radius.
//----------------------------------------------------------------------------
void NBezelView::SetCorner(float theValue)
{


	// Set the corner
	mCorner = theValue;
	
	SetNeedsDisplay();
}





//============================================================================
//		NBezelView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NBezelView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw	},
										{ kEventClassNone,    kEventKindNone	} };



	// Initialize ourselves
	mAlpha  = kBezelDefaultAlpha;
	mCorner = kBezelDefaultCorner;



	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	if (HasProperty(                 kPropertyNano, kTagBezelAlpha))
		SetAlpha( GetPropertyFloat32(kPropertyNano, kTagBezelAlpha));

	if (HasProperty(                 kPropertyNano, kTagBezelCorner))
		SetCorner(GetPropertyFloat32(kPropertyNano, kTagBezelCorner));
}





//============================================================================
//		NBezelView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NBezelView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	StContextState	cgState(cgContext);



	// Draw the bezel
	cgContext.SetFillColor(NColor(0.0f, 0.0f, 0.0f, mAlpha));
	cgContext.AddRect(GetBounds(), mCorner);
	cgContext.FillPath();

	return(noErr);
}


