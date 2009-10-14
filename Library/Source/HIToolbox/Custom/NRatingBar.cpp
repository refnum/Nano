/*	NAME:
		NRatingBar.cpp

	DESCRIPTION:
		iTunes-style rating bar.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NRatingBar.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Geometry
static const float kDotRadius										= 1.5f;

static const float   kStarSize										= 13.0f;
static const UInt32  kStarCount										= 6;
static const HIPoint kStarPoints[kStarCount]						= { { 0.5f, 0.0f },
																		{ 0.2f, 1.0f },
																		{ 1.0f, 0.4f },
																		{ 0.0f, 0.4f },
																		{ 0.8f, 1.0f },
																		{ 0.5f, 0.0f } };
																		

// Colors
static const NColor kColorStar										= NColor(0.57f, 0.57f, 0.57f);
static const NColor kColorDot										= NColor(0.72f, 0.72f, 0.72f);





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NRatingBar, "com.nano.nratingbar", kHIViewClassID);





//============================================================================
//		NRatingBar::NRatingBar : Constructor.
//----------------------------------------------------------------------------
NRatingBar::NRatingBar(HIWindowRef		theWindow,
						const HIRect	&theRect,
						SInt32			valueInitial,
						SInt32			valueMinimum,
						SInt32			valueMaximum)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetMinimumValue(valueMinimum);
	SetMaximumValue(valueMaximum);
	SetCurrentValue(valueInitial);
}





//============================================================================
//		NRatingBar::~NRatingBar : Destructor.
//----------------------------------------------------------------------------
NRatingBar::~NRatingBar(void)
{
}





//============================================================================
//		NRatingBar::GetImage : Get the image.
//----------------------------------------------------------------------------
NCGImage NRatingBar::GetImage(void) const
{


	// Get the image
	return(mImage);
}





//============================================================================
//		NRatingBar::SetImage : Set the image.
//----------------------------------------------------------------------------
void NRatingBar::SetImage(const NCGImage &theImage)
{


	// Set the image
	mImage = theImage;
	
	SetNeedsDisplay();
}





//============================================================================
//		NRatingBar::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NRatingBar::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw		},
										{ kEventClassControl, kEventControlHitTest	},
										{ kEventClassControl, kEventControlTrack	},
										{ kEventClassNone,    kEventKindNone		} };



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NRatingBar::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NRatingBar::DoEventControlDraw(NCarbonEvent &theEvent)
{	UInt32			n, numItems, itemValue;
	NCGContext		cgContext(theEvent);
	HIRect			theRect, rectItem;



	// Get the state we need
	theRect   = GetBounds();
	numItems  = GetMaximumValue() - GetMinimumValue();
	itemValue = GetCurrentValue() - GetMinimumValue();

	rectItem           = CGRectMake(0, 0, kStarSize, kStarSize);
	rectItem.origin.y += floor((CGRectGetHeight(theRect) - kStarSize) / 2.0f);



	// Prepare to draw
	if (!IsEnabled())
		cgContext.SetAlpha(kHIViewDisabledAlpha);



	// Draw the rating bar
	for (n = 0; n < numItems; n++)
		{
		if (n < itemValue)
			DrawItem( cgContext, rectItem);
		else
			DrawSpace(cgContext, rectItem);

		rectItem.origin.x += kStarSize;
		}

	return(noErr);
}





//============================================================================
//		NRatingBar::DoTrackMouse : Track the mouse.
//----------------------------------------------------------------------------
bool NRatingBar::DoTrackMouse(NCarbonEvent &theEvent, const HIPoint &theMouse, MouseTrackingResult theTrack)
{	float		theValue, maxWidth;
	HIRect		theRect;



	// Track the mouse
	switch (theTrack) {
		case kMouseTrackingMouseUp:
		case kMouseTrackingUserCancelled:
			// Remove the dots
			SetNeedsDisplay();
			break;


		case kMouseTrackingMouseDown:
		case kMouseTrackingMouseDragged:
		case kMouseTrackingTimedOut:
			// Update our value
			//
			// We always redraw, to show our dots on mouse down.
			theRect  = GetBounds();
			maxWidth = (GetMaximumValue() - GetMinimumValue()) * kStarSize;
			theValue = (theMouse.x - theRect.origin.x) / maxWidth;

			SetNeedsDisplay();
			SetPercentValue(GET_CLAMPED_VALUE(theValue, 0.0f, 1.0f));
			break;
		}



	// Inherit default behaviour
	return(NHIView::DoTrackMouse(theEvent, theMouse, theTrack));
}





//============================================================================
//		NRatingBar::DrawItem : Draw an item.
//----------------------------------------------------------------------------
#pragma mark -
void NRatingBar::DrawItem(NCGContext &cgContext, const HIRect &theRect)
{	UInt32		n;



	// Draw the item
	//
	// If we weren't given an image, we provide a default star.
	if (mImage.IsValid())
		cgContext.DrawImage(mImage, theRect);

	else
		{
		cgContext.MoveTo(theRect.origin.x + kStarPoints[0].x, theRect.origin.y + kStarPoints[0].y);
		
		for (n = 0; n < kStarCount; n++)
			cgContext.AddLineTo(theRect.origin.x + (kStarPoints[n].x * theRect.size.width),
								theRect.origin.y + (kStarPoints[n].y * theRect.size.height));

		cgContext.SetFillColor(kColorStar);
		cgContext.FillPath();
		}
}





//============================================================================
//		NRatingBar::DrawSpace : Draw a space.
//----------------------------------------------------------------------------
void NRatingBar::DrawSpace(NCGContext &cgContext, const HIRect &theRect)
{


	// Draw a dot when tracking
	if (IsTrackingMouse())
		{
		cgContext.SetFillColor(kColorDot);
		cgContext.AddCircle(NGeometryUtilities::GetCenter(theRect), kDotRadius);
		cgContext.FillPath();
		}
}

