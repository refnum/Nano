/*	NAME:
		NBadgeView.cpp

	DESCRIPTION:
		Badge view.
		
		A "badge view" provides a starburst-style badge effect, suitable for
		inserting into an NDockTile to obtain a Mail-style status badge.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NCGContext.h"
#include "NBadgeView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagColorBadge									= 'cbad';
static const OSType kTagColorText									= 'ctxt';


// Geometry
static const UInt32  kCornerCount									= 14;
static const CGPoint kCornerPoints[kCornerCount]					= { { 0.155f, 1.000f },
																		{ 0.000f, 0.885f },
																		{ 0.190f, 0.805f },
																		{ 0.060f, 0.650f },
																		{ 0.260f, 0.620f },
																		{ 0.170f, 0.435f },
																		{ 0.375f, 0.455f },
																		{ 0.330f, 0.250f },
																		{ 0.525f, 0.320f },
																		{ 0.530f, 0.115f },
																		{ 0.705f, 0.230f },
																		{ 0.760f, 0.030f },
																		{ 0.900f, 0.180f },
																		{ 1.000f, 0.000f } };


// Appearance
static const float kBadgeInset										= 60.0f;
static const float kBadgeShadowOffset								= 10.0f;
static const float kBadgeShadowBlur									= 38.0f;

static const NColor kColorShadow									= NColor(0.0f, 0.0f, 0.0f, 0.40f);
static const NColor kColorShadingTL									= NColor(0.0f, 0.0f, 0.0f, 0.00f);
static const NColor kColorShadingBR									= NColor(0.0f, 0.0f, 0.0f, 0.15f);

const NShadingSample kShadingBadge[] =								{ { 0.0f, kColorShadingTL },
																	  { 0.6f, kColorShadingTL },
																	  { 1.0f, kColorShadingBR } };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NBadgeView, "com.nano.nbadgeview", kHIViewClassID);





//============================================================================
//		NBadgeView::NBadgeView : Constructor.
//----------------------------------------------------------------------------
NBadgeView::NBadgeView(HIWindowRef		theWindow,
						const HIRect	&theRect,
						const NColor	&colorBadge,
						const NColor	&colorText)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetBadgeColor(colorBadge);
	SetTextColor( colorText);
}





//============================================================================
//		NBadgeView::~NBadgeView : Destructor.
//----------------------------------------------------------------------------
NBadgeView::~NBadgeView(void)
{
}





//============================================================================
//		NBadgeView::GetBadgeColor : Get the badge color.
//----------------------------------------------------------------------------
NColor NBadgeView::GetBadgeColor(void) const
{


	// Get the color
	return(mColorBadge);
}





//============================================================================
//		NBadgeView::SetBadgeColor : Set the badge color.
//----------------------------------------------------------------------------
void NBadgeView::SetBadgeColor(const NColor &theColor)
{


	// Set the color
	mColorBadge = theColor;
	
	SetNeedsDisplay();
}





//============================================================================
//		NBadgeView::GetTextColor : Get the text color.
//----------------------------------------------------------------------------
NColor NBadgeView::GetTextColor(void) const
{


	// Get the color
	return(mColorText);
}





//============================================================================
//		NBadgeView::SetTextColor : Set the text color.
//----------------------------------------------------------------------------
void NBadgeView::SetTextColor(const NColor &theColor)
{


	// Set the color
	mColorText = theColor;
	
	SetNeedsDisplay();
}





//============================================================================
//		NBadgeView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NBadgeView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw	},
										{ kEventClassNone,    kEventKindNone	} };



	// Initialize ourselves
	mColorBadge = kColorRed;
	mColorText  = kColorWhite;
	
	mShading.SetSamples(kShadingBadge);



	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	if (HasProperty(                    kPropertyNano, kTagColorBadge))
		SetBadgeColor(GetPropertyString(kPropertyNano, kTagColorBadge));

	if (HasProperty(                   kPropertyNano, kTagColorText))
		SetTextColor(GetPropertyString(kPropertyNano, kTagColorText));
}





//============================================================================
//		NBadgeView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NBadgeView::DoEventControlDraw(NCarbonEvent &theEvent)
{	float				theScale, theInset, shadowBlur;
	NCGContext			cgContext(theEvent);
	CGSize				shadowSize;
	CGPointList			thePoints;
	HIThemeTextInfo		textInfo;
	CGRect				theRect;



	// Get the state we need
	theRect  = GetBounds();
	theInset = theRect.size.height / kBadgeInset;
	theRect  = CGRectInset(theRect, theInset, theInset);
	theRect.origin.y -= theInset;

	theScale   = theRect.size.height * 0.5f;
	shadowBlur =                  theScale / kBadgeShadowBlur;
	shadowSize = CGSizeMake(0.0, -theScale / kBadgeShadowOffset);

	textInfo        = kThemeTextSystemFontCenter;
	textInfo.fontID = kThemeEmphasizedSystemFont;



	// Update the shading
	mShading.SetStartPoint(theRect.origin);
	mShading.SetEndPoint(CGPointMake(CGRectGetMaxX(theRect), CGRectGetMaxY(theRect)));



	// Draw the badge
	AddCorner(thePoints, kPositionTopLeft,     theRect, theScale);
	AddEdge(  thePoints, kPositionTopLeft,     theRect, theScale);
	AddCorner(thePoints, kPositionTopRight,    theRect, theScale);
	AddCorner(thePoints, kPositionBottomRight, theRect, theScale);
	AddEdge(  thePoints, kPositionBottomRight, theRect, theScale);
	AddCorner(thePoints, kPositionBottomLeft,  theRect, theScale);

	cgContext.SetShadowColor(kColorShadow, shadowBlur, shadowSize);
	cgContext.SetFillColor(mColorBadge);
	cgContext.AddPolygon(thePoints);
	cgContext.FillPath();

	cgContext.SetShadowColor(kColorClear);
	cgContext.AddPolygon(thePoints);
	cgContext.ClipToPath();
	cgContext.DrawShading(mShading);



	// Draw the text
	cgContext.SetFillColor(mColorText);
	cgContext.DrawThemeText(GetTextValue(), theRect, textInfo);

	return(noErr);
}





//============================================================================
//		NBadgeView::AddCorner : Add a corner.
//----------------------------------------------------------------------------
#pragma mark -
void NBadgeView::AddCorner(CGPointList &thePoints, NPosition theCorner, const CGRect &theRect, float theScale)
{	CGPointList		newPoints;
	CGPoint32		thePoint;
	UInt32			n;



	// Build the corner
	for (n = 0; n < kCornerCount; n++)
		{
		thePoint.x = kCornerPoints[n].x * theScale;
		thePoint.y = kCornerPoints[n].y * theScale;

		switch (theCorner) {
			case kPositionTopLeft:
				// Default
				break;
		
			case kPositionTopRight:
				thePoint.x = theRect.size.width  - thePoint.x;
				break;

			case kPositionBottomLeft:
				thePoint.y = theRect.size.height - thePoint.y;
				break;
		
			case kPositionBottomRight:
				thePoint.x = theRect.size.width  - thePoint.x;
				thePoint.y = theRect.size.height - thePoint.y;
				break;
		
			default:
				NN_LOG("Unknown position: %d", theCorner);
				break;
			}
		
		thePoint.x += theRect.origin.x;
		thePoint.y += theRect.origin.y;

		newPoints.push_back(thePoint);
		}



	// Update the points
	if (theCorner == kPositionTopRight || theCorner == kPositionBottomLeft)
		reverse(newPoints);

	append(thePoints, newPoints);
}





//============================================================================
//		NBadgeView::AddEdge : Add an edge.
//----------------------------------------------------------------------------
void NBadgeView::AddEdge(CGPointList &thePoints, NPosition theCorner, const CGRect &theRect, float theScale)
{	float			edgeWidth, spikeWidth, spikeHeight;
	UInt32			n, numSpikes;
	CGPoint32		thePoint;



	// Get the state we need
	spikeWidth  = fabs(kCornerPoints[kCornerCount-1].x - kCornerPoints[kCornerCount-2].x) * theScale * 2.0f;
	spikeHeight = fabs(kCornerPoints[kCornerCount-1].y - kCornerPoints[kCornerCount-2].y) * theScale;
	edgeWidth   = theRect.size.width - (theScale * 2.0f);



	// Check we have an edge
	//
	// Each corner provides half of the first/last spike of the edge; if the edge
	// isn't wide enough, it doesn't contribute any spikes to the polygon.
	//
	// The size of the edge spikes are adjusted slightly to ensure we always have
	// an integral number of spikes along the edge (even if this makes them a bit
	// larger/smaller than the corner spikes).
	if (edgeWidth <= 0.0f)
		return;

	numSpikes  = (UInt32) (edgeWidth / spikeWidth);
	spikeWidth =          (edgeWidth / (numSpikes * 2));



	// Build the edge
	switch (theCorner) {
		 case kPositionTopLeft:
			thePoint.x = theRect.origin.x + theScale;
			thePoint.y = theRect.origin.y;

			for (n = 0; n < numSpikes; n++)
				{
				thePoint.x += spikeWidth;
				thePoint.y += spikeHeight;
				thePoints.push_back(thePoint);

				thePoint.x += spikeWidth;
				thePoint.y  = theRect.origin.y;
				thePoints.push_back(thePoint);
				}
			break;
		
		case kPositionBottomRight:
			thePoint.x = theRect.origin.x + theRect.size.width  - theScale;
			thePoint.y = theRect.origin.y + theRect.size.height;

			for (n = 0; n < numSpikes; n++)
				{
				thePoint.x -= spikeWidth;
				thePoint.y -= spikeHeight;
				thePoints.push_back(thePoint);

				thePoint.x -= spikeWidth;
				thePoint.y  = theRect.origin.y + theRect.size.height;
				thePoints.push_back(thePoint);
				}
			break;

		default:
			NN_LOG("Unknown position: %d", theCorner);
			break;
		}
}

