/*	NAME:
		NDialView.cpp

	DESCRIPTION:
		Dial view.

		A "dial view" is equivalent to Cocoa's "round slider" control, but
		can also track distance from the center as well as the angle.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDialView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagDialAngle									= 'dang';
static const OSType kTagDialDistance								= 'ddis';
static const OSType kTagDialTrackDistance							= 'dtra';


// Geometry
static const float kFaceBorderWidth									= 1.5f;
static const float kFaceHiliteRadius								= 0.5f;
static const float kFaceHiliteInset									= 0.6f;
static const float kFaceHiliteOffset								= 0.5f;
static const float kThumbBorderGap									= 6.5f;
static const float kThumbOuterRadius								= 2.55f;
static const float kThumbInnerRadius								= 1.75f;
static const float kThumbDotRadius									= 1.5f;


// Colors
static const NColor kFaceBorder										= NColor(0x999999FF);
static const NColor kFaceHilite										= NColor(0xFFFFFFCC);
static const NColor kThumbOuter										= NColor(0xC2C2C2DD);
static const NColor kThumbInner										= NColor(0xAAAAAAEE);
static const NColor kThumbDot										= NColor(0x777777FF);

static const NShadingSample kSamplesFace[]							= {	{ 0.0f, NColor(0xE4E4E4FF) },
																		{ 1.0f, NColor(0xF1F1F1FF) } };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NDialView, "com.nano.ndialview", kHIViewClassID);





//============================================================================
//		NDialView::NDialView : Constructor.
//----------------------------------------------------------------------------
NDialView::NDialView(HIWindowRef			theWindow,
						const HIRect		&theRect,
						const NDialValue	&theValue)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetDialValue(theValue);
}





//============================================================================
//		NDialView::~NDialView : Destructor.
//----------------------------------------------------------------------------
NDialView::~NDialView(void)
{
}





//============================================================================
//		NDialView::CanTrackDistance : Can we track the distance?
//----------------------------------------------------------------------------
bool NDialView::CanTrackDistance(void) const
{


	// Get our state
	return(mTrackDistance);
}





//============================================================================
//		NDialView::SetTrackDistance : Set our distance tracking state.
//----------------------------------------------------------------------------
void NDialView::SetTrackDistance(bool canTrack)
{


	// Set our state
	mTrackDistance = canTrack;

	SyncValueDistance();
}





//============================================================================
//		NDialView::GetDialValue : Get the dial value.
//----------------------------------------------------------------------------
NDialValue NDialView::GetDialValue(void) const
{


	// Get the value
	return(mValue);
}





//============================================================================
//		NDialView::SetDialValue : Set the dial value.
//----------------------------------------------------------------------------
void NDialView::SetDialValue(const NDialValue &theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue.theDistance >= 0.0f && theValue.theDistance <= 1.0f);
	


	// Set the value
	mValue = theValue;

	SyncValueDistance();



	// Update our state
	SetNeedsDisplay();
	BroadcastMessage(kMsgDialChanged, this);
}





//============================================================================
//		NDialView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NDialView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlDraw		},
										{ kEventClassControl, kEventControlHitTest	},
										{ kEventClassControl, kEventControlTrack	},
										{ kEventClassNone,    kEventKindNone		} };



	// Initialize ourselves
	mValue         = kDialValueDefault;
	mTrackStart    = kDialValueDefault;
	mTrackDistance = true;

	mShadingFace.SetSamples(kSamplesFace);
	


	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	if (HasProperty(                            kPropertyNano, kTagDialAngle))
		mValue.theAngle    = GetPropertyFloat32(kPropertyNano, kTagDialAngle);

	if (HasProperty(                            kPropertyNano, kTagDialDistance))
		mValue.theDistance = GetPropertyFloat32(kPropertyNano, kTagDialDistance);

	if (HasProperty(                        kPropertyNano, kTagDialTrackDistance))
		mTrackDistance = GetPropertyBoolean(kPropertyNano, kTagDialTrackDistance);

	SyncValueDistance();
}





//============================================================================
//		NDialView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NDialView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext	cgContext(theEvent);



	// Prepare to draw
	if (!IsEnabled())
		cgContext.SetAlpha(kHIViewDisabledAlpha);



	// Draw the dial view
	DrawFace( cgContext);
	DrawThumb(cgContext);

	return(noErr);
}





//============================================================================
//		NDialView::DoTrackMouse : Track the mouse.
//----------------------------------------------------------------------------
bool NDialView::DoTrackMouse(NCarbonEvent &theEvent, const HIPoint &theMouse, MouseTrackingResult theTrack)
{


	// Track the mouse
	switch (theTrack) {
		case kMouseTrackingMouseDown:
			mTrackStart = mValue;
			break;

		case kMouseTrackingMouseDragged:
			SetDialValue(GetValueForPoint(theMouse));
			break;

		case kMouseTrackingUserCancelled:
			SetDialValue(mTrackStart);
			break;
		}



	// Inherit default behaviour
	return(NHIView::DoTrackMouse(theEvent, theMouse, theTrack));
}





//============================================================================
//		NDialView::DrawFace : Draw the face.
//----------------------------------------------------------------------------
#pragma mark -
void NDialView::DrawFace(NCGContext &cgContext)
{	float		minX, midX, maxX, minY, midY, maxY;
	HIRect		faceRect, hiliteRect;
	float		theRadius, theInset;



	// Get the state we need
	faceRect   = CGRectInset(GetBounds(), kFaceBorderWidth, kFaceBorderWidth);
	hiliteRect = faceRect;

	hiliteRect.origin.y -= (hiliteRect.size.height * kFaceHiliteOffset);

	minX = CGRectGetMinX(faceRect);
	midX = CGRectGetMidX(faceRect);
	maxX = CGRectGetMaxX(faceRect);

	minY = CGRectGetMinY(faceRect);
	midY = CGRectGetMidY(faceRect);
	maxY = CGRectGetMaxY(faceRect);
	
	mShadingFace.ApplyLinearShading(kLinearTopBottom, minY, maxY);



	// Draw the face
	cgContext.SaveState();

		cgContext.AddEllipse(faceRect);
		cgContext.ClipToPath();

		cgContext.DrawShading(mShadingFace);
	
	cgContext.RestoreState();



	// Draw the hilite
	cgContext.SaveState();

		cgContext.AddEllipse(faceRect);
		cgContext.ClipToPath();

		theRadius = (faceRect.size.width / 2.0f) * kFaceHiliteRadius;
		theInset  = theRadius * kFaceHiliteInset;
	
		minX += theInset;
		maxX -= theInset;

		cgContext.MoveTo(   midX, midY);
		cgContext.AddArcToPoint(CGPointMake(minX, midY), CGPointMake(minX, minY), theRadius);
		cgContext.AddLineTo(minX, minY);
		cgContext.AddLineTo(maxX, minY);
		cgContext.AddArcToPoint(CGPointMake(maxX, midY), CGPointMake(midX, midY), theRadius);
		cgContext.AddLineTo(midX, midY);

		cgContext.SetFillColor(kFaceHilite);
		cgContext.FillPath();

	cgContext.RestoreState();



	// Draw the border
	cgContext.SetLineWidth(  kFaceBorderWidth);
	cgContext.SetStrokeColor(kFaceBorder);

	cgContext.AddEllipse(faceRect);
	cgContext.StrokePath();
}





//============================================================================
//		NDialView::DrawThumb : Draw the thumb.
//----------------------------------------------------------------------------
void NDialView::DrawThumb(NCGContext &cgContext)
{	HIPoint		thePoint;



	// Get the state we need
	thePoint = GetPointForValue(mValue);



	// Draw the thumb
	cgContext.SetFillColor(NColor(kThumbOuter));
	cgContext.AddCircle(thePoint, kThumbOuterRadius);
	cgContext.FillPath();

	thePoint.y -= (kThumbOuterRadius - kThumbInnerRadius) / 2.0f;
	cgContext.SetFillColor(NColor(kThumbInner));
	cgContext.AddCircle(thePoint, kThumbInnerRadius);
	cgContext.FillPath();

	thePoint.y -= (kThumbInnerRadius - kThumbDotRadius) / 2.0f;
	cgContext.SetFillColor(NColor(kThumbDot));
	cgContext.AddCircle(thePoint, kThumbDotRadius);
	cgContext.FillPath();
}





//============================================================================
//		NDialView::GetValueForPoint : Get the value for a point.
//----------------------------------------------------------------------------
NDialValue NDialView::GetValueForPoint(const HIPoint &thePoint)
{	float			maxDistance;
	HIPoint			theOffset;
	NDialValue		theValue;
	HIRect			theRect;



	// Get the state we need
	theRect     = GetBounds();
	maxDistance = GetMaxDistance(theRect);

	theOffset.x = thePoint.x - CGRectGetMidX(theRect);
	theOffset.y = thePoint.y - CGRectGetMidY(theRect);



	// Calculate the value
	//
	// Values are always normalized from radians/pixels to 0.0-1.0;
	theValue.theAngle    = atan2(-theOffset.y, theOffset.x) - M_PI_2;
	theValue.theDistance = sqrtf((theOffset.x * theOffset.x) + (theOffset.y * theOffset.y));

	theValue.theAngle    = (1.0f - (theValue.theAngle    / (M_PI * 2.0f)));
	theValue.theDistance = std::min(theValue.theDistance / maxDistance, 1.0f);
	
	return(theValue);
}





//============================================================================
//		NDialView::GetPointForValue : Get the point for a value.
//----------------------------------------------------------------------------
HIPoint NDialView::GetPointForValue(const NDialValue &theValue)
{	float		radAngle, theDistance;
	HIPoint		thePoint;
	HIRect		theRect;



	// Get the state we need
	theRect = GetBounds();
	
	radAngle    = ((1.0f - theValue.theAngle) * (M_PI * 2.0f)) + M_PI_2;
	theDistance = theValue.theDistance * GetMaxDistance(theRect);
		


	// Calculate the point
	thePoint.x = CGRectGetMidX(theRect) + (cos(radAngle) *  theDistance);
	thePoint.y = CGRectGetMidY(theRect) + (sin(radAngle) * -theDistance);

	return(thePoint);
}





//============================================================================
//		NDialView::GetMaxDistance : Get the maximum allowable distance.
//----------------------------------------------------------------------------
float NDialView::GetMaxDistance(const HIRect &theRect)
{	float	maxDistance;



	// Get the maximum distance
	maxDistance = (std::min(theRect.size.width, theRect.size.height) / 2.0f) - kThumbBorderGap;
	
	return(maxDistance);
}





//============================================================================
//		NDialView::SyncValueDistance : Sync the value distance.
//----------------------------------------------------------------------------
void NDialView::SyncValueDistance(void)
{


	// Sync the distance
	if (!mTrackDistance)
		mValue.theDistance = 1.0f;
}




