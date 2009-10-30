/*	NAME:
		NShadingView.cpp

	DESCRIPTION:
		Shading view.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NShadingView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagFrameStyle									= 'fram';
static const OSType kTagShadingMode									= 'mode';
static const OSType kTagShadingOrientation							= 'orin';
static const OSType kTagShadingColor0								= 'col0';
static const OSType kTagShadingColor1								= 'col1';
static const OSType kTagShadingColor2								= 'col2';
static const OSType kTagShadingColor3								= 'col3';
static const OSType kTagShadingColor4								= 'col4';
static const OSType kTagShadingColor5								= 'col5';
static const OSType kTagShadingColor6								= 'col6';
static const OSType kTagShadingColor7								= 'col7';
static const OSType kTagShadingColor8								= 'col8';
static const OSType kTagShadingColor9								= 'col9';





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
NCGShading *NShadingView::GetShading(void)
{


	// Get the shading
	return(&mShading);
}





//============================================================================
//		NShadingView::GetFrameStyle : Get the frame style.
//----------------------------------------------------------------------------
FrameStyle NShadingView::GetFrameStyle(void) const
{


	// Get the frame style
	return(mFrameStyle);
}





//============================================================================
//		NShadingView::SetFrameStyle : Set the frame style.
//----------------------------------------------------------------------------
void NShadingView::SetFrameStyle(FrameStyle theStyle)
{


	// Set the frame style
	mFrameStyle = theStyle;
	
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
{	const EventTypeSpec		theEvents[] = { { kEventClassControl, kEventControlDraw },
											{ kEventClassNone,    kEventKindNone	} };
	NShadingSampleList		theSamples;
	NShadingSample			theSample;
	UInt32					n;



	// Initialize ourselves
	mFrameStyle  = kFrameNone;
	mOrientation = kLinearTopBottom;

	mShading.SetSamples(kShadingFrontRow);



	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	if (HasProperty(                                          kPropertyNano, kTagFrameStyle))
		SetFrameStyle(   (FrameStyle)       GetPropertyOSType(kPropertyNano, kTagFrameStyle));

	if (HasProperty(                                          kPropertyNano, kTagShadingMode))
		mShading.SetMode((NShadingMode)     GetPropertyOSType(kPropertyNano, kTagShadingMode));

	if (HasProperty(                                          kPropertyNano, kTagShadingOrientation))
		SetOrientation((NLinearOrientation) GetPropertyOSType(kPropertyNano, kTagShadingOrientation));

	if (HasProperty(                             kPropertyNano, kTagShadingColor0))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor0); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor1))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor1); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor2))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor2); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor3))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor3); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor4))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor4); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor5))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor5); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor6))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor6); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor7))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor7); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor8))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor8); theSamples.push_back(theSample); }

	if (HasProperty(                             kPropertyNano, kTagShadingColor9))
		{ theSample.theColor = GetPropertyString(kPropertyNano, kTagShadingColor9); theSamples.push_back(theSample); }

	if (!theSamples.empty())
		{
		for (n = 0; n < theSamples.size(); n++)
			theSamples[n].theValue = (float) n / (float) theSamples.size();
		
		mShading.SetSamples(theSamples);
		}
}





//============================================================================
//		NShadingView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NShadingView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);
	HIRect				theRect;



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



	// Draw the frame
	switch (mFrameStyle) {
		case kFrameNone:
			// Nothing to draw
			break;
		
		case kFrameThin:
			cgContext.SetLineWidth(1.0f);
			cgContext.SetStrokeColor(kColorBlack);
			cgContext.StrokeRect(theRect);
			break;
		
		default:
			NN_LOG("Unknown frame: %d", mFrameStyle);
			break;
		}

	return(noErr);
}




