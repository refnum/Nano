/*	NAME:
		NStatusBar.cpp

	DESCRIPTION:
		Status bar.

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
#include "NStatusBar.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const float kStatusTextInset								= 4.0f;


// Text
static const ThemeTextColor  kStatusTextColor					= kThemeTextColorPlacardActive;

static const HIThemeTextInfo kStatusTextInfo					= {	kHIThemeTextInfoVersionZero,
																	kThemeStateActive,
																	kThemeMiniSystemFont,
																	kHIThemeTextHorizontalFlushLeft,
																	kHIThemeTextVerticalFlushCenter,
																	kHIThemeTextBoxOptionNone,
																	kHIThemeTextTruncationNone,
																	0,
																	false };


// Colors
static const NColor kColorDivider								= NColor(0xBBBBBBFF);

const NShadingSample kSamplesStatus[] =							{ { 0.0f, NColor(0xDDDDDDFF) },
																  { 1.0f, NColor(0xBBBBBBFF) } };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NStatusBar, "com.nano.nstatusbar", kHIViewClassID);





//============================================================================
//		NStatusBar::NStatusBar : Constructor.
//----------------------------------------------------------------------------
NStatusBar::NStatusBar(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NStatusBar::~NStatusBar : Destructor.
//----------------------------------------------------------------------------
NStatusBar::~NStatusBar(void)
{
}





//============================================================================
//		NStatusBar::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NStatusBar::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw },
										{ kEventClassNone,    kEventKindNone	} };



	// Initialize ourselves
	mShading.SetSamples(kSamplesStatus);



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NStatusBar::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NStatusBar::DoEventControlDraw(NCarbonEvent &theEvent)
{	float			minX, maxX, minY, maxY;
	NCGContext		cgContext(theEvent);
	NString			theText;
	HIRect			theRect;



	// Get the state we need
	theRect = GetBounds();
	theText = GetTextValue();

	minX = CGRectGetMinX(theRect);
	maxX = CGRectGetMaxX(theRect);

	minY = CGRectGetMinY(theRect);
	maxY = CGRectGetMaxY(theRect);



	// Draw the status bar
	mShading.ApplyLinearShading(kLinearTopBottom, minY, maxY);
	cgContext.DrawShading(mShading);



	// Draw the divider
	cgContext.MoveTo(   minX, minY);
	cgContext.AddLineTo(maxX, minY);

	cgContext.SetStrokeColor(kColorDivider);
	cgContext.StrokePath();
	
	
	
	// Draw the text
	if (theText.IsNotEmpty())
		{
		theRect           = CGRectInset(theRect, kStatusTextInset, 0);
		theRect.origin.y += 1.0f;
		
		cgContext.SetTextColor(kStatusTextColor);
		cgContext.DrawThemeText(theText, theRect, kStatusTextInfo);
		}

	return(noErr);
}



