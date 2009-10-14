/*	NAME:
		NLabelView.cpp

	DESCRIPTION:
		Finder-style "label color" view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NSystemUtilities.h"
#include "NThemeUtilities.h"
#include "NMathUtilities.h"
#include "Nano Build.h"
#include "NCFBundle.h"
#include "NLabelView.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef struct {
	NColor		theTop;
	NColor		theBottom;
} LabelGradient;





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const UInt32 kLabelPartCount									= 8;


// Colors
static const NColor kSelectionColor									= kColorWhite;
static const NColor kSelectionFrameColor							= kColorGray;
static const NColor kSelectionHiliteColor							= NColor(0xD8D8D8FF);

static const NColor kCrossColor										= NColor(0x7A7A7AFF);
static const NColor kCrossShadowColor								= NColor(0x00000011);
static const float  kCrossShadowBlur								= 1.0f;
static const CGSize kCrossShadowOffset								= CGSizeMake(0.0f, -1.0f);

static const NColor kCircleShadowColor								= NColor(0x00000088);
static const float  kCircleShadowBlur								= 1.75f;
static const CGSize kCircleShadowOffset								= CGSizeMake(0.0f, -1.9f);

static const LabelGradient kPartGradients[kLabelPartCount]			= { { kColorWhite,        kColorBlack        },			// X
																		{ NColor(0xFF9EA4FF), NColor(0xFF1543FF) },			// Red
																		{ NColor(0xFFDF79FF), NColor(0xFFA218FF) },			// Orange
																		{ NColor(0xFBF785FF), NColor(0xFADA00FF) },			// Yellow
																		{ NColor(0xC9F087FF), NColor(0x85DD00FF) },			// Green
																		{ NColor(0x8FD4F1FF), NColor(0x009BDFFF) },			// Blue
																		{ NColor(0xFFB6DAFF), NColor(0xFC3EACFF) },			// Purple
																		{ NColor(0xD0D0D0FF), NColor(0x939393FF) } };		// Gray


// Times
static const NTimerID  kTimerRollOver								= 1000;
static const EventTime kDelayRollOver								= 0.100f;
static const EventTime kDelaySelection								= 0.066f;


// Geometry
static const float kLabelPartGapAbove								=  5.0f;
static const float kLabelPartGapLeft								=  3.0f;
static const float kLabelPartGapAfterNone							=  5.0f;
static const float kLabelPartNameHeight								= 15.0f;
static const float kLabelPartWidth									= 17.0f;
static const float kLabelPartHeight									= 16.0f;

static const UInt32 kGlyphWidth										= 11;
static const UInt32 kGlyphHeight									= 11;
static const float  kGlyphInset										= 1.0f;

static const float kCrossWidth										= 2.0f;
static const float kCrossInset										= 1.5f;

static const float kCircleShadingStart								= 0.25f;
static const float kCircleShadingEnd								= 0.95f;
static const float kCircleInset										= 0.5f;


// Text
static const HIThemeTextInfo kThemeTextMenuItemLabel = {	kHIThemeTextInfoVersionZero,
															kThemeStateActive,
															kThemeSmallEmphasizedSystemFont,
															kHIThemeTextHorizontalFlushCenter,
															kHIThemeTextVerticalFlushTop,
															kHIThemeTextBoxOptionNone,
															kHIThemeTextTruncationEnd,
															0,
															false };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NLabelView, "com.nano.nlabelview", kHIViewClassID);





//============================================================================
//		NLabelView::NLabelView : Constructor.
//----------------------------------------------------------------------------
NLabelView::NLabelView(const NMenuItem &menuItem, ThemeMenuType menuType)
{	OSStatus	theErr;



	// Create the view
	ConstructView(HIViewCreate(kClassID, this));



	// Configure the view
	theErr = SetMenuItem(menuItem, menuType);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NLabelView::~NLabelView : Destructor.
//----------------------------------------------------------------------------
NLabelView::~NLabelView(void)
{
}





//============================================================================
//		NLabelView::GetSelection : Get the selection.
//----------------------------------------------------------------------------
OptionBits NLabelView::GetSelection(void) const
{


	// Get the selection
	return(mSelection);
}





//============================================================================
//		NLabelView::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NLabelView::SetSelection(OptionBits theSelection)
{	OptionBits		oldSelection;



	// Set the selection
	oldSelection = mSelection;
	mSelection   = theSelection;



	// Handle changes
	if (mSelection != oldSelection)
		{
		BroadcastMessage(kMsgLabelSelectionChanged, this);
		SetNeedsDisplay();
		}
}





//============================================================================
//		NLabelView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NLabelView::InitializeView(void)
{


	// Invoke the base class
	NMenuItemView::InitializeView();



	// Initialize ourselves
	mSelection = 0;

	mTitleBaseline = 0.0f;
	mTitleHeight   = 0.0f;
	mTitleLeft     = 0.0;

	mRollOver     = kHIViewNoPart;
	mRollOverNew  = kHIViewNoPart;
	mRollOverText = kHIViewNoPart;
	mRollOverPart = kHIViewNoPart;
}





//============================================================================
//		NLabelView::DoMenuItemChanged : Handle changes to the menu item.
//----------------------------------------------------------------------------
void NLabelView::DoMenuItemChanged(void)
{


	// Reset our state
	ResetMetrics();
}





//============================================================================
//		NLabelView::DoMenuItemPopulate : Populate the menu item.
//----------------------------------------------------------------------------
void NLabelView::DoMenuItemPopulate(void)
{


	// Reset our state
	ResetMetrics();
}





//============================================================================
//		NLabelView::DoMenuItemHitTest : Hit-test the menu item.
//----------------------------------------------------------------------------
HIViewPartCode NLabelView::DoMenuItemHitTest(const HIPoint &thePoint)
{	HIRect		theRect;
	UInt32		n;



	// Hit-test our parts
	for (n = 1; n <= kLabelPartCount; n++)
		{
		theRect = GetPartBounds(n);

		if (CGRectContainsPoint(theRect, thePoint))
			return((HIViewPartCode) n);
		}
	
	return(kHIViewNoPart);
}





//============================================================================
//		NLabelView::DoTimer : Handle a timer.
//----------------------------------------------------------------------------
void NLabelView::DoTimer(NTimerID theTimer, TimerMsg theMsg)
{


	// Handle the timer
	switch (theTimer) {
		case kTimerRollOver:
			UpdateRollOver();
			break;
		
		default:
			NTimerTask::DoTimer(theTimer, theMsg);
			break;
		}
}





//============================================================================
//		NLabelView::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	//
	// If the user is using non-sticky menu tracking and releases the mouse over the
	// view, kEventCommandProcess will be dispatched for the menu item.
	//
	// Since we broadcast our own message on selection changes, we consume this event.
	hiCmd  = theEvent.GetHICommand();
	theErr = eventNotHandledErr;
			
	if (hiCmd.attributes & kHICommandFromMenu)
		{
		NMenuItem	theItem(hiCmd.source.menu.menuRef, hiCmd.source.menu.menuItemIndex, false);

		if (theItem == GetMenuItem())
			theErr = noErr;
		}
	
	return(theErr);
}





//============================================================================
//		NLabelView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventControlDraw(NCarbonEvent &theEvent)
{	bool				isHilighted, isSelected;
	NCGContext			cgContext(theEvent);
	HIPoint				thePoint;
	HIRect				theRect;
	HIViewPartCode		thePart;
	NString				theText;
	UInt32				n;



	// Draw the title
	theText  = GetMenuItemText();
	theRect  = GetTitleBounds();

	if (!IsMenuItemEnabled())
		cgContext.SetTextColor(kThemeTextColorMenuItemDisabled);

	cgContext.DrawThemeText(theText, theRect, kThemeTextMenuItem);



	// Draw the label
	thePart = (mRollOverText != kHIViewNoPart) ? mRollOverText : mRollOver;
	
	if (thePart > 1)
		{
		theText = GetPartName(thePart);
		theRect = GetLabelBounds();

		cgContext.SetTextColor(kThemeTextColorMenuItemDisabled);
		cgContext.DrawThemeText(theText, theRect, kThemeTextMenuItemLabel);
		}



	// Draw the parts
	thePart = (mRollOverPart != kHIViewNoPart) ? mRollOverPart : mRollOver;

	if (!IsMenuItemEnabled())
		cgContext.SetAlpha(kHIViewDisabledAlpha);

	for (n = 1; n <= kLabelPartCount; n++)
		{
		isHilighted = (n == GetControlHilite(*this) || n == (UInt32) thePart);
		isSelected  = ((mSelection & (1 << (n - 1))) != 0);

		theRect  = GetPartBounds(n);
		thePoint = NGeometryUtilities::GetCenter(theRect);

		if (isHilighted || isSelected)
			DrawPartSelection(cgContext, thePoint, isHilighted);

		DrawPartGlyph(cgContext, thePoint, n);
		}

	return(noErr);
}





//============================================================================
//		NLabelView::DoEventControlGetOptimalBounds : Handle kEventControlGetOptimalBounds.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventControlGetOptimalBounds(NCarbonEvent &theEvent)
{	HIRect		theBounds, rectFirst, rectLast;
	float		baseLine;



	// Get the state we need
	rectFirst = GetPartBounds(1);
	rectLast  = GetPartBounds(kLabelPartCount);

	GetTitleHeight(&baseLine);



	// Calculate the optimal bounds
	theBounds.origin      = kHIPointZero;
	theBounds.size.width  = CGRectGetMaxX(rectLast) + rectFirst.origin.x;
	theBounds.size.height = CGRectGetMaxY(rectLast) + kLabelPartNameHeight;

	theEvent.SetParameterHIRect(kEventParamControlOptimalBounds,         theBounds);
	theEvent.SetParameterSInt32(kEventParamControlOptimalBaselineOffset, (SInt32) baseLine);
	
	return(noErr);
}





//============================================================================
//		NLabelView::DoEventControlGetPartRegion : Handle kEventControlGetPartRegion.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventControlGetPartRegion(NCarbonEvent &theEvent)
{	RgnHandle			qdRgnOut, qdRgnTmp;
	HIViewPartCode		thePart;
	HIRect				theRect;
	OSStatus			theErr;
	UInt32				n;



	// Get the state we need
	thePart  = theEvent.GetViewPartCode();
	qdRgnOut = NULL;

	theErr = theEvent.GetParameter(kEventParamControlRegion, typeQDRgnHandle, sizeof(qdRgnOut), &qdRgnOut);
	if (theErr != noErr || qdRgnOut == NULL)
		return(theErr);



	// Return the part region
	//
	// We provide the clickable meta-region so that if the user clicks in portions
	// of the view that are not trackable, the click can pass through the control
	// and initiate async dragging.
	theErr = eventNotHandledErr;
	
	if (thePart == kControlClickableMetaPart)
		{
		qdRgnTmp = NewRgn();
		if (qdRgnTmp != NULL)
			{
			for (n = 1; n <= kLabelPartCount; n++ )
				{
				theRect = GetPartBounds(n);
				SetRectRgn(qdRgnTmp, (SInt16) theRect.origin.x,
									 (SInt16) theRect.origin.y,
									 (SInt16) CGRectGetMaxX(theRect),
									 (SInt16) CGRectGetMaxY(theRect));

				UnionRgn(qdRgnTmp, qdRgnOut, qdRgnOut);
				}
			
			DisposeRgn(qdRgnTmp);
			theErr = noErr;
			}
		}
	
	return(theErr);
}





//============================================================================
//		NLabelView::DoEventControlHit : Handle kEventControlHit.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventControlHit(NCarbonEvent &theEvent)
{	HIViewPartCode		thePart;



	// Get the state we need
	thePart = theEvent.GetViewPartCode();



	// Update the selection
	if (thePart == 1)
		SetSelection(0);
	else
		SetSelection(1 << (thePart - 1));
	
	return(noErr);
}





//============================================================================
//		NLabelView::DoEventControlSimulateHit : Handle kEventControlSimulateHit.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventControlSimulateHit(NCarbonEvent &theEvent)
{	NCarbonEvent		hitEvent(kEventClassControl, kEventControlHit);
	HIViewPartCode		thePart, selectedPart;



	// Check this is our menu item
	thePart = theEvent.GetViewPartCode();
	if (thePart != GetMenuItem().GetIndex())
		return(eventNotHandledErr);



	// Save the current state
	selectedPart = mRollOver;



	// Simulate the hit
	if (selectedPart != kHIViewNoPart)
		selectedPart = mRollOverNew;
	
	if (selectedPart != kHIViewNoPart)
		{
		// Prepare to update
		ResetRollOver();
		mRollOverText = selectedPart;


		// Blink the part
		mRollOverPart = 0;
		Render(kDelaySelection);

		mRollOverPart = selectedPart;
		Render(kDelaySelection);

		mRollOverPart = 0;
		Render(kDelaySelection);


		// Hit the part
		hitEvent.SetViewPartCode(selectedPart);
		DoEventControlHit(hitEvent);

		Render(kDelaySelection);
		}
	
	return(noErr);
}





//============================================================================
//		NLabelView::DoEventControlTrack : Handle kEventControlTrack.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventControlTrack(NCarbonEvent &/*theEvent*/)
{


	// Clear the rollover state
	//
	// We clear our state so that it doesn't interfere with the proper part
	// drawing during tracking.
	//
	// This avoids the situation where moving the mouse out of a part that
	// was clicked on would continue to draw the part in its rollover state
	// (because mRollOver still contains that part).
	ResetRollOver();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NLabelView::DoEventControlVisibilityChanged : Handle kEventControlVisibilityChanged.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventControlVisibilityChanged(NCarbonEvent &theEvent)
{


	// Reset our state
	if (IsVisible())
		ResetRollOver();



	// Invoke the base class
	return(NMenuItemView::DoEventControlVisibilityChanged(theEvent));
}





//============================================================================
//		NLabelView::DoEventMouseDragged : Handle kEventMouseDragged.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventMouseDragged(NCarbonEvent &theEvent)
{


	// Update the mouse
	return(UpdateMouse(theEvent));
}





//============================================================================
//		NLabelView::DoEventMouseMoved : Handle kEventMouseMoved.
//----------------------------------------------------------------------------
OSStatus NLabelView::DoEventMouseMoved(NCarbonEvent &theEvent)
{


	// Update the mouse
	return(UpdateMouse(theEvent));
}





//============================================================================
//		NLabelView::GetTitleBounds : Get the title bounds.
//----------------------------------------------------------------------------
#pragma mark -
HIRect NLabelView::GetTitleBounds(void)
{	float		leftEdge;
	HIRect		theRect;



	// Get the state we need
	leftEdge = GetTitleLeft();



	// Get the bounds
	theRect = GetBounds();

	theRect.origin.x    = leftEdge;
	theRect.size.width -= leftEdge;
	theRect.size.height = GetTitleHeight();
	
	return(theRect);
}





//============================================================================
//		NLabelView::GetTitleHeight : Get the height of the title.
//----------------------------------------------------------------------------
float NLabelView::GetTitleHeight(float *baseLine)
{	NString		theText;
	HISize		theSize;



	// Calculate the height
	//
	// We use a fixed string if no title was supplied, as per Apple's example code.
	if (NMathUtilities::IsZero(mTitleHeight))
		{
		theText = GetMenuItemText();
		if (theText.IsEmpty())
			theText = "Sample";

		theSize        = kHISizeZero;
		mTitleBaseline = NThemeUtilities::GetTextDimensions(theText, theSize, kThemeTextMenuItem);
		mTitleHeight   = theSize.height + kLabelPartGapAbove;
		}



	// Get the title height
	if (baseLine != NULL)
		*baseLine = mTitleBaseline;
		
	return(mTitleHeight);
}





//============================================================================
//		NLabelView::GetTitleLeft : Get the left edge of the title.
//----------------------------------------------------------------------------
float NLabelView::GetTitleLeft(void)
{	MenuAttributes		theAttributes;



	// Calculate the left edge
	if (NMathUtilities::IsZero(mTitleLeft))
		{
		theAttributes = GetMenuItem().GetMenu().GetAttributes();
		
		if (theAttributes & kMenuAttrExcludesMarkColumn)
			mTitleLeft = NThemeUtilities::GetMetric(kThemeMetricMenuExcludedMarkColumnWidth);
		else
			mTitleLeft = NThemeUtilities::GetMetric(kThemeMetricMenuMarkColumnWidth);
		}



	// Get the left edge
	return(mTitleLeft);
}





//============================================================================
//		NLabelView::GetPartBounds : Get the bounds for a part.
//----------------------------------------------------------------------------
HIRect NLabelView::GetPartBounds(HIViewPartCode thePart)
{	HIRect		theRect;



	// Calculate the "none" part bounds
	//
	// The first part is left-aligned with the menu item title, with a small
	// inset as per the Finder (but unlike the LabelItemView example).
	theRect = GetBounds();

	theRect.origin.x += GetTitleLeft()   + kLabelPartGapLeft;
	theRect.origin.x -= (kLabelPartWidth - kGlyphWidth) / 2.0f;
	theRect.origin.y  = theRect.origin.y + GetTitleHeight();

	theRect.size.width  = kLabelPartWidth;
	theRect.size.height = kLabelPartHeight;



	// Offset the remaining parts
	if (thePart > 1)
		theRect.origin.x += kLabelPartGapAfterNone;

	theRect.origin.x = theRect.origin.x + (kLabelPartWidth + 1) * (thePart - 1);

	return(theRect);
}





//============================================================================
//		NLabelView::GetPartName : Get the name of a part.
//----------------------------------------------------------------------------
NString NLabelView::GetPartName(HIViewPartCode thePart)
{	NString		theFormat;
	UInt32		n;



	// Validate our parameters
	NN_ASSERT(thePart >= 1 && thePart <= (HIViewPartCode) kLabelPartCount);



	// Fetch the names
	if (mPartNames.empty())
		{
		theFormat  = NBundleString(kStringLabelColor, "", kNanoStrings).GetUTF8();
		mPartNames = NSystemUtilities::GetLabelNames();

		mPartNames.insert(mPartNames.begin(), "");
		
		for (n = 1; n < kLabelPartCount; n++)
			{
			NN_ASSERT(mPartNames[n].IsNotEmpty());
			mPartNames[n].Format(theFormat.GetUTF8(), mPartNames[n].GetUTF8());
			}
		}



	// Get the name
	return(mPartNames[thePart - 1]);
}





//============================================================================
//		NLabelView::GetLabelBounds : Get the label bounds.
//----------------------------------------------------------------------------
HIRect NLabelView::GetLabelBounds(void)
{	HIRect		theRect, partBounds;



	// Get the state we need
	partBounds = GetPartBounds(1);



	// Get the bounds
	theRect = GetBounds();

	theRect.origin.y    = CGRectGetMaxY(partBounds);
	theRect.size.height = CGRectGetMaxY(GetBounds()) - theRect.origin.y;
	
	return(theRect);
}





//============================================================================
//		NLabelView::ResetMetrics : Reset our metrics.
//----------------------------------------------------------------------------
void NLabelView::ResetMetrics(void)
{


	// Reset the metrics
	mTitleBaseline = 0.0f;
	mTitleHeight   = 0.0f;
	mTitleLeft     = 0.0f;

	mPartNames.clear();
}





//============================================================================
//		NLabelView::ResetRollOver : Reset the roll-over state.
//----------------------------------------------------------------------------
void NLabelView::ResetRollOver(void)
{


	// Reset our state
	mRollOver     = kHIViewNoPart;
	mRollOverNew  = kHIViewNoPart;
	mRollOverText = kHIViewNoPart;
	mRollOverPart = kHIViewNoPart;
}





//============================================================================
//		NLabelView::UpdateRollOver : Update the roll-over state.
//----------------------------------------------------------------------------
void NLabelView::UpdateRollOver(void)
{	HIRect		theRect;



	// Update the label
	if (mRollOver > 1 || mRollOverNew > 1)
		{
		theRect = GetLabelBounds();
		SetNeedsDisplay(true, &theRect);
		}



	// Update the parts
	if (mRollOver != 0)
		{
		theRect = GetPartBounds(mRollOver);
		SetNeedsDisplay(true, &theRect);
		}

	if (mRollOverNew != 0)
		{
		theRect = GetPartBounds(mRollOverNew);
		SetNeedsDisplay(true, &theRect);
		}



	// Update our state
	mRollOver = mRollOverNew;
	
	if (HasTimer(kTimerRollOver))
		RemoveTimer(kTimerRollOver);
}





//============================================================================
//		NLabelView::UpdateMouse : Update the mouse.
//----------------------------------------------------------------------------
OSStatus NLabelView::UpdateMouse(NCarbonEvent &theEvent)
{	NCarbonEvent	hitTestEvent(kEventClassControl, kEventControlHitTest);
	HIRect			rectFirst, rectLast;
	HIPoint			thePoint;



	// Validate our parameters
	NN_ASSERT(theEvent.IsKind(kEventMouseMoved) || theEvent.IsKind(kEventMouseDragged) );



	// Check our state
	if (GetSuperView() == NULL || !IsWindowVisible(GetHIWindow()))
		return(eventNotHandledErr);



	// Get the state we need
	thePoint = theEvent.GetMouseLocation();
	thePoint = NGeometryUtilities::Transform(thePoint, kHICoordSpace72DPIGlobal, NULL, kHICoordSpaceView, *this);

	hitTestEvent.SetMouseLocation(thePoint);
	DoEventControlHitTest(hitTestEvent);



	// Update the roll-over state
	//
	// If we are over a label part, or the mouse passed outside of the area used
	// by the label parts, we immediately invalidate the label name for redraw.
	//
	// Otherwise, we delay the invalidation for a short time in case the mouse is
	// about to move over another label part. If so, we'll get back here again to
	// invalidate the old and new parts, but will have avoided redrawing ourselves
	// with an empty label name.
	//
	// This helps avoid flicker - even with double-buffering, the screen typically
	// updates soon enough that the user can see flicker if they move back and forth
	// between two label parts. 
	mRollOverNew = hitTestEvent.GetViewPartCode();
	
	if (mRollOverNew != mRollOver)
		{
		rectFirst = GetPartBounds(2);
		rectLast  = GetPartBounds(kLabelPartCount);

		if (mRollOverNew != 0 || thePoint.x < CGRectGetMinX(rectFirst) || thePoint.x > CGRectGetMaxX(rectLast))
			UpdateRollOver();

		else if (!HasTimer(kTimerRollOver))
			AddTimer(kDelayRollOver, 0, kTimerRollOver);

		else
			SetNextFireTime(kDelayRollOver, kTimerRollOver);
		}

	return(eventNotHandledErr);
}





//============================================================================
//		NLabelView::DrawPartSelection : Draw a part selection.
//----------------------------------------------------------------------------
void NLabelView::DrawPartSelection(NCGContext &cgContext, const HIPoint &thePoint, bool isHilighted)
{	HIRect		theRect;



	// Get the state we need
	theRect.size.width  = kLabelPartWidth  - 2;
	theRect.size.height = kLabelPartHeight - 2;

	theRect.origin.x = floor(thePoint.x - (theRect.size.width  / 2.0f));
	theRect.origin.y = floor(thePoint.y - (theRect.size.height / 2.0f));



	// Draw the selection
	cgContext.SetLineWidth(0.5f);
	cgContext.SetStrokeColor(kSelectionFrameColor);
	cgContext.SetFillColor(isHilighted ? kSelectionHiliteColor : kSelectionColor);

	cgContext.AddRect(theRect);
	cgContext.DrawPath();
}





//============================================================================
//		NLabelView::DrawPartGlyph : Draw a part glyph.
//----------------------------------------------------------------------------
void NLabelView::DrawPartGlyph(NCGContext &cgContext, const HIPoint &thePoint, HIViewPartCode thePart)
{	NShadingSample		theSamples[] = { { 0.0f, kColorRed }, { 1.0f, kColorBlue } };
	NCGShading			theShading(kShadingRadial);
	HIRect				theRect;



	// Validate our parameters
	NN_ASSERT(thePart >= 1 && thePart <= (HIViewPartCode) kLabelPartCount);
	


	// Get the state we need
	theRect.origin.x    = floor(thePoint.x - (kGlyphWidth  / 2.0f));
	theRect.origin.y    = floor(thePoint.y - (kGlyphHeight / 2.0f));
	theRect.size.width  = kGlyphWidth;
	theRect.size.height = kGlyphHeight;

	theRect = CGRectInset(theRect, kGlyphInset, kGlyphInset);



	// Prepare to draw
	cgContext.SaveState();



	// Draw the 'x'
	if (thePart == 1)
		{
		// Prepare to draw
		theRect = CGRectInset(theRect, kCrossInset, kCrossInset);

		cgContext.SetLineWidth(  kCrossWidth);
		cgContext.SetShadowColor(kCrossShadowColor, kCrossShadowBlur, kCrossShadowOffset);
		cgContext.SetStrokeColor(kCrossColor);


		// Draw the cross
		cgContext.MoveTo(   CGPointMake(CGRectGetMinX(theRect), CGRectGetMinY(theRect)));
		cgContext.AddLineTo(CGPointMake(CGRectGetMaxX(theRect), CGRectGetMaxY(theRect)));
		cgContext.MoveTo(   CGPointMake(CGRectGetMaxX(theRect), CGRectGetMinY(theRect)));
		cgContext.AddLineTo(CGPointMake(CGRectGetMinX(theRect), CGRectGetMaxY(theRect)));
		cgContext.StrokePath();
		}


	// Draw a circle
	else
		{
		// Prepare to draw
		theSamples[0].theColor = kPartGradients[thePart - 1].theTop;
		theSamples[1].theColor = kPartGradients[thePart - 1].theBottom;
		
		theShading.SetSamples(theSamples);
		theShading.SetStartPoint(CGPointMake(CGRectGetMidX(theRect), CGRectGetMinY(theRect)));
		theShading.SetEndPoint(  CGPointMake(CGRectGetMidX(theRect), CGRectGetMinY(theRect)));
		theShading.SetStartRadius(theRect.size.height * kCircleShadingStart);
		theShading.SetEndRadius(  theRect.size.height * kCircleShadingEnd);

		cgContext.SetShadowColor(kCircleShadowColor, kCircleShadowBlur, kCircleShadowOffset);
		cgContext.SetFillColor(  kColorWhite);


		// Draw the circle
		theRect = CGRectInset(theRect, kCircleInset, kCircleInset);
		
		cgContext.AddEllipse(theRect);
		cgContext.FillPath();

		cgContext.AddEllipse(theRect);
		cgContext.ClipToPath();
		cgContext.DrawShading(theShading);
		}



	// Clean up
	cgContext.RestoreState();
}




