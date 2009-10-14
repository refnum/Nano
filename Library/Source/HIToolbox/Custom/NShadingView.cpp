/*	NAME:
		NShadingView.cpp

	DESCRIPTION:
		Shading view.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NShadingView.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NShadingView, "com.nano.nshadingview", kHIViewClassID);





//============================================================================
//		NShadingView::NShadingView : Constructor.
//----------------------------------------------------------------------------
NShadingView::NShadingView(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NShadingView::~NShadingView : Destructor.
//----------------------------------------------------------------------------
NShadingView::~NShadingView(void)
{
}





//============================================================================
//		NShadingView::GetShading : Get the shading.
//----------------------------------------------------------------------------
NCGShading NShadingView::GetShading(void) const
{


	// Get the shading
	return(mShading);
}





//============================================================================
//		NShadingView::SetShading : Set the shading.
//----------------------------------------------------------------------------
void NShadingView::SetShading(const NCGShading &theShading)
{


	// Set the shading
	mShading = theShading;
	
	SetNeedsDisplay();
}





//============================================================================
//		NShadingView::GetOrientation : Get the shading orientation.
//----------------------------------------------------------------------------
NLinearOrientation NShadingView::GetOrientation(void) const
{


	// Get the orientation
	return(mOrientation);
}





//============================================================================
//		NShadingView::SetOrientation : Set the orientation.
//----------------------------------------------------------------------------
void NShadingView::SetOrientation(NLinearOrientation theOrientation)
{


	// Set the orientation
	mOrientation = theOrientation;
	
	SetNeedsDisplay();
}





//============================================================================
//		NShadingView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NShadingView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw },
										{ kEventClassNone,    kEventKindNone	} };



	// Initialize ourselves
	mOrientation = kLinearTopBottom;

	mShading.SetSamples(kShadingFrontRow);



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NShadingView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NShadingView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	HIRect			theRect;



	// Get the state we need
	theRect = GetBounds();
	
	switch (mOrientation) {
		case kLinearTopBottom:
		case kLinearBottomTop:
			mShading.ApplyLinearShading(mOrientation, CGRectGetMinY(theRect), CGRectGetMaxY(theRect)); 
			break;
			
		case kLinearLeftRight:
		case kLinearRightLeft:
			mShading.ApplyLinearShading(mOrientation, CGRectGetMinX(theRect), CGRectGetMaxX(theRect)); 
			break;
		
		default:
			NN_LOG("Unknown orientation: %d", mOrientation);
			break;
		}



	// Draw the shading
	cgContext.ClipToRect(theRect);
	cgContext.DrawShading(mShading);

	return(noErr);
}




