/*	NAME:
		NSplitterView.cpp

	DESCRIPTION:
		Splitter view.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCursor.h"
#include "NCGContext.h"
#include "NSplitterView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const UInt32 kDefaultSize										= 10;
static const float  kGrabberSize										= 1.5f;


// Colors
static const NColor kColorGrabber										= NColor(0.60f, 0.60f, 0.60f);
static const NColor kColorBevelFrame									= NColor(0.33f, 0.33f, 0.33f);
static const NColor kColorHairlineFrame									= NColor(0.25f, 0.25f, 0.25f);

const NShadingSample kShadingSplitter[]									= { { 0.0f, kColorWhite },
																			{ 1.0f, NColor(0.86f, 0.86f, 0.86f) } };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NSplitterView, "com.nano.nsplitterview", kHIViewClassID);





//============================================================================
//		NSplitterView::NSplitterView : Constructor.
//----------------------------------------------------------------------------
NSplitterView::NSplitterView(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NSplitterView::~NSplitterView : Destructor.
//----------------------------------------------------------------------------
NSplitterView::~NSplitterView(void)
{
}





//============================================================================
//		NSplitterView::GetKind : Get the kind.
//----------------------------------------------------------------------------
SplitterKind NSplitterView::GetKind(void) const
{


	// Get the kind
	return(mKind);
}





//============================================================================
//		NSplitterView::SetKind : Set the kind.
//----------------------------------------------------------------------------
void NSplitterView::SetKind(SplitterKind theKind)
{


	// Set the kind
	mKind = theKind;
	
	UpdateSplitter();
}





//============================================================================
//		NSplitterView::IsVertical : Get the orientation.
//----------------------------------------------------------------------------
bool NSplitterView::IsVertical(void) const
{


	// Get the orientation
	return(mVertical);
}





//============================================================================
//		NSplitterView::SetVertical : Set the orientation.
//----------------------------------------------------------------------------
void NSplitterView::SetVertical(bool isVertical)
{


	// Set the orientation
	mVertical = isVertical;
	
	UpdateSplitter();
}





//============================================================================
//		NSplitterView::GetRatio : Get the ratio.
//----------------------------------------------------------------------------
float NSplitterView::GetRatio(void) const
{


	// Get the ratio
	return(mRatio);
}





//============================================================================
//		NSplitterView::SetRatio : Set the ratio.
//----------------------------------------------------------------------------
void NSplitterView::SetRatio(float theRatio)
{	float	limitMin, limitMax;



	// Validate our parameters
	NN_ASSERT(theRatio >= 0.0f && theRatio <= 1.0f);



	// Get the state we need
	limitMin = mRatioLimit;
	limitMax = 1.0f - mRatioLimit;



	// Set the ratio
	mRatio = GET_CLAMPED_VALUE(theRatio, limitMin, limitMax);
	
	UpdateSplitter();
}





//============================================================================
//		NSplitterView::GetRatioLimit : Get the ratio limit.
//----------------------------------------------------------------------------
float NSplitterView::GetRatioLimit(void) const
{


	// Get the ratio limit
	return(mRatioLimit);
}





//============================================================================
//		NSplitterView::SetRatioLimit : Set the ratio limit.
//----------------------------------------------------------------------------
void NSplitterView::SetRatioLimit(float theRatio)
{


	// Validate our parameters
	NN_ASSERT(theRatio >= 0.0f && theRatio < 1.0f);
	


	// Set the ratio limit
	mRatioLimit = theRatio;
	
	UpdateSplitter();
}





//============================================================================
//		NSplitterView::GetDividerSize : Get the divider size.
//----------------------------------------------------------------------------
UInt32 NSplitterView::GetDividerSize(void) const
{


	// Get the divider size
	return(mDividerSize);
}





//============================================================================
//		NSplitterView::SetDividerSize : Set the divider size.
//----------------------------------------------------------------------------
void NSplitterView::SetDividerSize(UInt32 theSize)
{


	// Set the divider size
	mDividerSize = theSize;
	
	UpdateSplitter();
}





//============================================================================
//		NSplitterView::GetViewA : Get view "A".
//----------------------------------------------------------------------------
NHIView *NSplitterView::GetViewA(void) const
{


	// Get the view
	return(mViewA);
}





//============================================================================
//		NSplitterView::GetViewB : Get view "B".
//----------------------------------------------------------------------------
NHIView *NSplitterView::GetViewB(void) const
{


	// Get the view
	return(mViewB);
}





//============================================================================
//		NSplitterView::SetViewA : Set view "A".
//----------------------------------------------------------------------------
void NSplitterView::SetViewA(NHIView *theView)
{


	// Remove the old view
	if (mViewA != NULL)
		delete mViewA;



	// Set the view
	mViewA = theView;

	if (mViewA != NULL)
		AddSubView(mViewA);



	// Update our state
	UpdateSplitter();
}





//============================================================================
//		NSplitterView::SetViewB : Set view "B".
//----------------------------------------------------------------------------
void NSplitterView::SetViewB(NHIView *theView)
{


	// Remove the old view
	if (mViewB != NULL)
		delete mViewB;



	// Set the view
	mViewB = theView;

	if (mViewB != NULL)
		AddSubView(mViewB);



	// Update our state
	UpdateSplitter();
}





//============================================================================
//		NSplitterView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NSplitterView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl,  kEventControlBoundsChanged         },
										{ kEventClassControl,  kEventControlDraw                  },
										{ kEventClassControl,  kEventControlHitTest               },
										{ kEventClassControl,  kEventControlInterceptSubviewClick },
										{ kEventClassControl,  kEventControlTrack                 },
										{ kEventClassControl,  kEventControlTrackingAreaEntered   },
										{ kEventClassNone,     kEventKindNone                     } };



	// Initialize ourselves
	mKind        = kSplitterStandard;
	mVertical    = true;
	mRatio       = 0.5f;
	mRatioLimit  = 0.0f;
	mDividerSize = kDefaultSize;
	
	mViewA = NULL;
	mViewB = NULL;
	
	mRectA = kHIRectZero;
	mRectB = kHIRectZero;
	mRectD = kHIRectZero;
	
	mDragLast      = kHIPointZero;
	mDragSize      = 0.0f;
	mDragOffsetMin = 0.0f;
	mDragOffsetMax = 0.0f;
	mDragCursor    = false;

	mShading.SetSamples(kShadingSplitter);

	SetFeatures(kHIViewFeatureAllowsSubviews);
	SetOptions( kHIViewOptionSetsCursor);



	// Register for events
	CreateTrackingArea();

	AddEvents(theEvents);
}





//============================================================================
//		NSplitterView::DoEventControlBoundsChanged : Handle kEventControlBoundsChanged.
//----------------------------------------------------------------------------
OSStatus NSplitterView::DoEventControlBoundsChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	UpdateSplitter();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NSplitterView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NSplitterView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);



	// Validate our state
	NN_ASSERT(mViewA != NULL);
	NN_ASSERT(mViewB != NULL);



	// Draw the background
	switch (mKind) {
		case kSplitterStandard:
			cgContext.DrawShading(mShading);
			cgContext.SetStrokeColor(kColorBevelFrame);
			cgContext.StrokeRect(mRectD);
			break;

		case kSplitterHairline:
			cgContext.SetFillColor(kColorHairlineFrame);
			cgContext.FillRect(mRectD);
			break;
		
		case kSplitterThumb:
		case kSplitterTransparent:
			// No background
			break;
		
		default:
			NN_LOG("Unknown kind: %d", mKind);
			break;
		}



	// Draw the thumb
	switch (mKind) {
		case kSplitterStandard:
		case kSplitterThumb:
			cgContext.SetStrokeColor(kColorGrabber);
			cgContext.AddCircle(NGeometryUtilities::GetCenter(mRectD), kGrabberSize);
			cgContext.StrokePath();
			break;

		case kSplitterHairline:
		case kSplitterTransparent:
			// No thumb
			break;
		
		default:
			NN_LOG("Unknown kind: %d", mKind);
			break;
		}

	return(noErr);
}





//============================================================================
//		NSplitterView::DoEventControlInterceptSubviewClick : Handle kEventControlInterceptSubviewClick.
//----------------------------------------------------------------------------
OSStatus NSplitterView::DoEventControlInterceptSubviewClick(NCarbonEvent &theEvent)
{	NCarbonEvent		mouseEvent;
	bool				inDivider;
	HIPoint				thePoint;
	OSStatus			theErr;



	// Get the state we need
	mouseEvent = theEvent.GetParameterEvent(kEventParamEventRef);
	theErr     = eventNotHandledErr;



	// Handle the event
	if (mouseEvent.IsValid())
		{
		thePoint  = mouseEvent.GetMouseLocation();
		thePoint  = NGeometryUtilities::Transform(thePoint, kHICoordSpace72DPIGlobal, NULL, kHICoordSpaceView, (HIViewRef) *this);

		inDivider = CGRectContainsPoint(mRectD, thePoint);
		UpdateCursor(inDivider);

		if (inDivider)
			theErr = noErr;
		}
	
	return(theErr);
}





//============================================================================
//		NSplitterView::DoEventControlTrackingAreaEntered : Handle kEventControlTrackingAreaEntered.
//----------------------------------------------------------------------------
OSStatus NSplitterView::DoEventControlTrackingAreaEntered(NCarbonEvent &theEvent)
{


	// Update the cursor
	UpdateCursor(CGRectContainsPoint(mRectD, theEvent.GetMouseLocation()));
	
	return(eventNotHandledErr);
}





//============================================================================
//		NSplitterView::DoTrackMouse : Handle mouse tracking.
//----------------------------------------------------------------------------
bool NSplitterView::DoTrackMouse(NCarbonEvent &theEvent, const HIPoint &theMouse, MouseTrackingResult theTrack)
{	HIPoint		thePoint;
	float		theRatio;
	HIRect		theRect;



	// Validate our state
	NN_ASSERT(mViewA != NULL);
	NN_ASSERT(mViewB != NULL);



	// Get the state we need
	theRect  = GetBounds();
	thePoint = theMouse;



	// Handle the event
	switch (theTrack) {
		case kMouseTrackingMouseDown:
			// Drag if we're in the splitter
			if (!CGRectContainsPoint(mRectD, theMouse))
				return(false);


			// Prepare the size
			if (IsVertical())
				mDragSize = theRect.size.width  - mDividerSize;
			else
				mDragSize = theRect.size.height - mDividerSize;


			// Prepare the offsets
			//
			// To prevent the splitter from being dragged beyond our bounds,
			// we need the offset of the mouse down within the divider.
			mDragLast = theMouse;
			
			if (IsVertical())
				{
				mDragOffsetMin = theMouse.x - mRectD.origin.x;
				mDragOffsetMax = mRectD.size.width - mDragOffsetMin;
				}
			else
				{
				mDragOffsetMin = theMouse.y - mRectD.origin.y;
				mDragOffsetMax = mRectD.size.height - mDragOffsetMin;
				}
			break;



		case kMouseTrackingMouseDragged:
			// Clamp the mouse
			if (IsVertical())
				{
				thePoint.x = std::max(thePoint.x, theRect.origin.x + mDragOffsetMin);
				thePoint.x = std::min(thePoint.x, theRect.origin.x + theRect.size.width - mDragOffsetMax);
				}
			else
				{
				thePoint.y = std::max(thePoint.y, theRect.origin.y + mDragOffsetMin);
				thePoint.y = std::min(thePoint.y, theRect.origin.y + theRect.size.height - mDragOffsetMax);
				}


			// Calculate the new ratio
			theRatio = mRatio;
			
			if (IsVertical() && NMathUtilities::NotEqual(mDragLast.x, thePoint.x))
				theRatio = mRatio - ((mDragLast.x - thePoint.x) / mDragSize);

			else if (!IsVertical() && NMathUtilities::NotEqual(mDragLast.y, thePoint.y))
				theRatio = mRatio - ((mDragLast.y - thePoint.y) / mDragSize);
			
			theRatio = GET_CLAMPED_VALUE(theRatio, 0.0f, 1.0f);


			// Update the splitter
			if (NMathUtilities::NotEqual(theRatio, mRatio))
				SetRatio(theRatio);
				
			mDragLast = thePoint;
			break;


		default:
			break;
		}



	// Call through for default behaviour
	return(NHIView::DoTrackMouse(theEvent, theMouse, theTrack));
}





//============================================================================
//		NSplitterView::UpdateCursor : Update the cursor.
//----------------------------------------------------------------------------
#pragma mark -
void NSplitterView::UpdateCursor(bool inDivider)
{	NCursor		theCursor;



	// Check our state
	if (GetOptions() & kHIViewOptionIsLocked)
		return;



	// Set the drag cursor
	if (inDivider)
		{
		mDragCursor = true;
		theCursor.SetCursor(IsVertical() ? kThemeResizeLeftRightCursor : kThemeResizeUpDownCursor);
		}


	// Reset the cursor
	//
	// To avoid flicker from our sub-views changing the cursor, we only reset the cursor
	// once after it's been set to the drag cursor. If our sub-views also want to set the
	// cursor, this allows them to change it without being overriden by us.
	else if (mDragCursor)
		{
		mDragCursor = false;
		theCursor.SetCursor(kThemeArrowCursor);
		}
}





//============================================================================
//		NSplitterView::UpdateSplitter : Update the splitter state.
//----------------------------------------------------------------------------
void NSplitterView::UpdateSplitter(void)
{


	// Check our state
	if (GetOptions() & kHIViewOptionIsLocked)
		return;



	// Update the layout
	UpdateLayout();



	// Update the shading
	if (IsVertical())
		mShading.ApplyLinearShading(kLinearLeftRight, CGRectGetMinX(mRectD), CGRectGetMaxX(mRectD));
	else
		mShading.ApplyLinearShading(kLinearTopBottom, CGRectGetMinY(mRectD), CGRectGetMaxY(mRectD));



	// Update the views
	if (mViewA != NULL)
		mViewA->SetFrame(mRectA);

	if (mViewB != NULL)
		mViewB->SetFrame(mRectB);



	// Request a redraw
	SetNeedsDisplay();
}





//============================================================================
//		NSplitterView::UpdateLayout : Update the layout.
//----------------------------------------------------------------------------
void NSplitterView::UpdateLayout(void)
{	float		theSpan, spanA, spanB, spanD;
	bool		overlappingD;
	HIRect		theRect;



	// Get the state we need
	theRect = GetBounds();
	theSpan = (SInt32) (IsVertical() ? theRect.size.width : theRect.size.height);

	switch (mKind) {
		case kSplitterTransparent:
			overlappingD = true;
			spanD        = 0.0f;
			break;
		
		case kSplitterHairline:
			overlappingD = true;
			spanD        = 1.0f;
			break;
		
		default:
			overlappingD = false;
			spanD        = mDividerSize;
			break;
		}
		
	spanA = floorf((theSpan - spanD) * mRatio);
	spanB = theSpan - spanD - spanA;



	// Calculate view A
	mRectA = theRect;
	
	if (IsVertical())
		mRectA.size.width  = spanA;
	else
		mRectA.size.height = spanA;



	// Calculate view B
	mRectB = theRect;
	
	if (IsVertical())
		{
		mRectB.size.width = spanB;
		mRectB.origin.x   = CGRectGetMaxX(theRect) - spanB;
		}
	else
		{
		mRectB.size.height = spanB;
		mRectB.origin.y    = CGRectGetMaxY(theRect) - spanB;
		}



	// Calculate the divider
	//
	// Some splitters (transparent and hairline) allow the divider to overhang the
	// A and B views, letting the user grab the divider while still in those views.
	mRectD = theRect;

	if (IsVertical())
		{
		if (overlappingD)
			{
			mRectD.origin.x   = CGRectGetMaxX(mRectA) - floorf(mDividerSize / 2.0f);
			mRectD.size.width = mDividerSize;
			}
		else
			{
			mRectD.origin.x   = CGRectGetMaxX(mRectA);
			mRectD.size.width = spanD;
			}
		}
	else
		{
		if (overlappingD)
			{
			mRectD.origin.y    = CGRectGetMaxY(mRectA) - floorf(mDividerSize / 2.0f);
			mRectD.size.height = mDividerSize;
			}
		else
			{
			mRectD.origin.y    = CGRectGetMaxY(mRectA);
			mRectD.size.height = spanD;
			}
		}
}





