/*	NAME:
		NSourceListView.cpp

	DESCRIPTION:
		Source list view.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NKeyboardUtilities.h"
#include "NSystemUtilities.h"
#include "NHIViewUtilities.h"
#include "NMathUtilities.h"
#include "NSourceListView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Geometry
static const float kIconSize										= 16.0f;
static const float kRowHeight										= 20.0f;
static const float kRowIndent										= 13.0f;
static const float kRowGapFrame										= 9.0f;
static const float kRowGapIconTitle									= 4.0f;
static const float kHeaderOffsetY									= 2.0f;
static const float kHeaderGap										= 5.0f;


// Colors
static const NColor kColorBackgroundFill							= NColor(0.82f, 0.84f, 0.89f);
static const NColor kColorDragStroke								= NColor(0.03f, 0.32f, 0.84f);
static const NColor kColorDragFill									= NColor(0.03f, 0.32f, 0.84f, 0.25f);
static const NColor kColorTextHeaderNormal							= NColor(0.31f, 0.36f, 0.43f);
static const NColor kColorTextHeaderShadow							= NColor(0.95f, 0.96f, 0.97f);
static const NColor kColorTextRowNormal								= NColor(0.02f, 0.02f, 0.02f);
static const NColor kColorTextRowSelected							= NColor(1.00f, 1.00f, 1.00f);

static const CGShadowInfo kTextRowShadow							= { NColor(0.00f, 0.00f, 0.00f, 0.5f), 1.0f, CGSizeMake(0.0f, -1.0f) };

const NShadingSample kShadingActive[] =								{ { 0.00f, NColor(0.23f, 0.47f, 0.70f) },
																	  { 0.05f, NColor(0.36f, 0.58f, 0.84f) },
																	  { 1.00f, NColor(0.09f, 0.34f, 0.68f) } };

const NShadingSample kShadingInactive[] =							{ { 0.00f, NColor(0.51f, 0.57f, 0.70f) },
																	  { 0.05f, NColor(0.63f, 0.69f, 0.81f) },
																	  { 1.00f, NColor(0.45f, 0.52f, 0.67f) } };


// Misc
static const UInt32 kInvalidRow										= (UInt32) -1;

static const float kEditTextFrameInset								= 3.0f;
static const float kDragFrameWidth									= 3.0f;
static const float kDragCornerSize									= 5.0f;


// Text info
static const HIThemeTextInfo kThemeTextHeader = {		kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeSmallEmphasizedSystemFont,
														kHIThemeTextHorizontalFlushLeft,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationEnd,
														1,
														false };

static const HIThemeTextInfo kThemeTextRowNormal = {	kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeSmallSystemFont,
														kHIThemeTextHorizontalFlushLeft,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationEnd,
														1,
														false };

static const HIThemeTextInfo kThemeTextRowSelected = {	kHIThemeTextInfoVersionZero,
														kThemeStateActive,
														kThemeSmallEmphasizedSystemFont,
														kHIThemeTextHorizontalFlushLeft,
														kHIThemeTextVerticalFlushCenter,
														kHIThemeTextBoxOptionNone,
														kHIThemeTextTruncationEnd,
														1,
														false };

static const ControlFontStyleRec kControlFontRowEditor = {	kControlUseThemeFontIDMask,
															kThemeSmallEmphasizedSystemFont,
															0 };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NSourceListView, "com.nano.nsourcelistview", kHIViewClassID);





//============================================================================
//		NSourceListView::NSourceListView : Constructor.
//----------------------------------------------------------------------------
NSourceListView::NSourceListView(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NSourceListView::~NSourceListView : Destructor.
//----------------------------------------------------------------------------
NSourceListView::~NSourceListView(void)
{
}





//============================================================================
//		NSourceListView::GetItem : Get an item.
//----------------------------------------------------------------------------
const NSourceListItem *NSourceListView::GetItem(const NString &theID) const
{


	// Get the item
	return(GetItemForID(theID, &mItems));
}





//============================================================================
//		NSourceListView::GetItems : Get the items.
//----------------------------------------------------------------------------
NSourceListItemList NSourceListView::GetItems(void) const
{


	// Get the items
	return(mItems);
}





//============================================================================
//		NSourceListView::SetItems : Set the items.
//----------------------------------------------------------------------------
void NSourceListView::SetItems(const NSourceListItemList &theItems)
{


	// Stop any edit
	EditEnd(false);



	// Set the items
	mItems     = theItems;
	mSelection = kInvalidRow;

	BuildRows();
}





//============================================================================
//		NSourceListView::GetSelection : Get the selection.
//----------------------------------------------------------------------------
NString NSourceListView::GetSelection(void) const
{	NString		theID;



	// Get the selection
	if (mSelection != kInvalidRow)
		theID = mRows[mSelection].theItem->GetID();
	
	return(theID);
}





//============================================================================
//		NSourceListView::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NSourceListView::SetSelection(const NString &theID)
{	UInt32		n, numItems;



	// Validate our parameters
	NN_ASSERT(theID.IsNotEmpty());



	// Stop any edit
	EditEnd(false);



	// Set the selection
	mSelection = kInvalidRow;
	numItems   = mRows.size();

	for (n = 0; n < numItems && mSelection == kInvalidRow; n++)
		{
		if (mRows[n].theItem->GetID() == theID)
			mSelection = n;
		}
	
	NN_ASSERT(mSelection != kInvalidRow);
	NN_ASSERT(CanSelect(mSelection));



	// Update our state
	BroadcastMessage(kMsgSourceListChangedSelection, mRows[mSelection].theItem);
	
	RevealRow(mSelection);
	SetNeedsDisplay();
}





//============================================================================
//		NSourceListView::RemoveSelection : Remove the selection.
//----------------------------------------------------------------------------
void NSourceListView::RemoveSelection(void)
{


	// Check our state
	if (!CanRemove(mSelection))
		return;
	
	
	
	// Remove the selection
	BroadcastMessage(kMsgSourceItemRemoved, mRows[mSelection].theItem);
}





//============================================================================
//		NSourceListView::EditSelection : Edit the selection.
//----------------------------------------------------------------------------
void NSourceListView::EditSelection(void)
{


	// Check our state
	if (!CanRename(mSelection))
		return;



	// Open the editor
	EditBegin();
}





//============================================================================
//		NSourceListView::GetDragDelegate : Get the drag delegate.
//----------------------------------------------------------------------------
NSourceListDragDelegate *NSourceListView::GetDragDelegate(void) const
{


	// Get the delegate
	return(mDragDelegate);
}





//============================================================================
//		NSourceListView::SetDragDelegate : Set the drag delegate.
//----------------------------------------------------------------------------
void NSourceListView::SetDragDelegate(NSourceListDragDelegate *theDelegate)
{


	// Set the delegate
	mDragDelegate = theDelegate;
}





//============================================================================
//		NSourceListView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NSourceListView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassCommand,  kEventCommandProcess       },
										{ kEventClassControl,  kEventControlBoundsChanged },
										{ kEventClassControl,  kEventControlGetFocusPart  },
										{ kEventClassControl,  kEventControlHitTest       },
										{ kEventClassControl,  kEventControlSetFocusPart  },
										{ kEventClassControl,  kEventControlTrack         },
										{ kEventClassKeyboard, kEventRawKeyDown           },
										{ kEventClassNone,     kEventKindNone             } };



	// Initialize the base class
	NScrollableView::InitializeView();



	// Initialize ourselves
	mEditor    = NULL;
	mHasFocus  = false;
	mSelection = kInvalidRow;
	
	mDragDelegate = NULL;
	mDragHilite   = kDragHiliteNone; 
	mDragTarget   = kInvalidRow;
	
	mShadingActive.SetSamples(  kShadingActive);
	mShadingInactive.SetSamples(kShadingInactive);



	// Configure the view
	SetViewLineSize(CGSizeMake(kRowHeight, kRowHeight));

	SetFeatures(kHIViewFeatureAllowsSubviews   |
				kHIViewFeatureGetsFocusOnClick |
				kHIViewFeatureIsOpaque);



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NSourceListView::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NSourceListView::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd  = theEvent.GetHICommand();
	theErr = noErr;
	
	switch (hiCmd.commandID) {
		case kHICommandNEditAccept:
		case kHICommandNEditCancel:
			EditEnd(hiCmd.commandID == kHICommandNEditCancel);
			break;
		
		default:
			theErr = NHIView::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}






//============================================================================
//		NSourceListView::DoEventControlBoundsChanged : Handle kEventControlBoundsChanged.
//----------------------------------------------------------------------------
OSStatus NSourceListView::DoEventControlBoundsChanged(NCarbonEvent &/*theEvent*/)
{


	// Update the rows
	UpdateRows();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NSourceListView::DoEventControlGetFocusPart : Handle kEventControlGetFocusPart.
//----------------------------------------------------------------------------
OSStatus NSourceListView::DoEventControlGetFocusPart(NCarbonEvent &theEvent)
{


	// Return the focus
	theEvent.SetViewPartCode(mHasFocus ? (HIViewPartCode) kHIViewIndicatorPart :
										 (HIViewPartCode) kHIViewFocusNoPart);

	return(noErr);
}





//============================================================================
//		NSourceListView::DoEventControlSetFocusPart : Handle kEventControlSetFocusPart.
//----------------------------------------------------------------------------
OSStatus NSourceListView::DoEventControlSetFocusPart(NCarbonEvent &theEvent)
{	HIViewPartCode		thePart;
	OSStatus			theErr;



	// Get the state we need
	thePart = theEvent.GetViewPartCode();
	theErr  = noErr;



	// Handle the event
	switch (thePart) {
		case kHIViewFocusNoPart:
			// Lose the focus
			mHasFocus = false;
			break;
		
		case kHIViewIndicatorPart:
			// Acquire the focus
			mHasFocus = true;
			break;

		case kHIViewFocusNextPart:
		case kHIViewFocusPrevPart:
			// Advance the focus
			mHasFocus = !mHasFocus;
			theEvent.SetViewPartCode(mHasFocus ? (HIViewPartCode) kHIViewIndicatorPart :
												 (HIViewPartCode) kHIViewFocusNoPart);
			break;
		
		default:
			theErr = eventNotHandledErr;
			break;
		}



	// Update our state
	SetNeedsDisplay();
	
	return(theErr);
}





//============================================================================
//		NSourceListView::DoEventRawKeyDown : Handle kEventRawKeyDown.
//----------------------------------------------------------------------------
OSStatus NSourceListView::DoEventRawKeyDown(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Check our state
	if (!mHasFocus)
		return(eventNotHandledErr);



	// Handle the key stroke
	theErr = noErr;
	
	switch (theEvent.GetKeyCode()) {
		case kKeyCodeBackspace:
		case kKeyCodeDelete:
			RemoveSelection();
			break;
		
		case kKeyCodeEnter:
		case kKeyCodeReturn:
		case kKeyCodeNumPadEnter:
			EditSelection();
			break;
		
		case kKeyCodeArrowUp:
			SelectRow(-1);
			break;

		case kKeyCodeArrowDown:
			SelectRow(+1);
			break;

		default:
			theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NSourceListView::DoEventScrollableScrollTo : Handle kEventScrollableScrollTo.
//----------------------------------------------------------------------------
OSStatus NSourceListView::DoEventScrollableScrollTo(NCarbonEvent &theEvent)
{


	// Stop any edit
	EditEnd(false);



	// Handle the event
	return(NScrollableView::DoEventScrollableScrollTo(theEvent));
}





//============================================================================
//		NSourceListView::DoDragUpdate : Update a drag.
//----------------------------------------------------------------------------
ThemeCursor NSourceListView::DoDragUpdate(const NDrag &theDrag, bool isWithin)
{	ThemeCursor					theResult;
	SourceListDragHilite		theHilite;
	const NSourceListItem		*theItem;
	UInt32						theIndex;
	bool						isDirty;



	// Validate our state
	NN_ASSERT(mDragDelegate != NULL);



	// Get the state we need
	theIndex = GetRowUnderDrag(theDrag);
	theItem  = (theIndex == kInvalidRow) ? NULL : mRows[theIndex].theItem;



	// Update the delegate
	if (isWithin)
		{
		theHilite = mDragDelegate->SLDragUpdate(theDrag, theItem);
		isWithin  = (theHilite != kDragHiliteNone);
		}
	else
		{
		theHilite = kDragHiliteNone;
		theIndex  = kInvalidRow;
		theItem   = NULL;
		}
	
	isDirty   = (theHilite != mDragHilite || theIndex != mDragTarget);
	theResult = isWithin ? kThemeCopyArrowCursor : kThemeNotAllowedCursor;



	// Update our state
	mDragHilite = theHilite;
	mDragTarget = theIndex;

	SetDragTarget(isWithin);

	if (isDirty)
		SetNeedsDisplay();

	return(theResult);
}





//============================================================================
//		NSourceListView::DoDragReceive : Receive a drag.
//----------------------------------------------------------------------------
DragActions NSourceListView::DoDragReceive(const NDrag &theDrag)
{	DragActions					theResult;
	const NSourceListItem		*theItem;
	UInt32						theIndex;



	// Validate our state
	NN_ASSERT(mDragDelegate != NULL);



	// Get the state we need
	theIndex = GetRowUnderDrag(theDrag);
	theItem  = (theIndex == kInvalidRow) ? NULL : mRows[theIndex].theItem;



	// Update the delegate
	theResult = mDragDelegate->SLDragReceive(theDrag, theItem);



	// Update our state
	mDragHilite = kDragHiliteNone;
	mDragTarget = kInvalidRow;

	SetNeedsDisplay();
	
	return(theResult);
}





//============================================================================
//		NSourceListView::DoTrackMouse : Track the mouse.
//----------------------------------------------------------------------------
bool NSourceListView::DoTrackMouse(NCarbonEvent &theEvent, const HIPoint &theMouse, MouseTrackingResult theTrack)
{


	// Track the mouse
	switch (theTrack) {
		case kMouseTrackingMouseUp:
			SelectRow(theMouse);
			
			if (theEvent.GetMouseClickCount() == 2)
				EditSelection();
			break;
		}

	return(NHIView::DoTrackMouse(theEvent, theMouse, theTrack));
}





//============================================================================
//		NSourceListView::DrawBackground : Draw the background.
//----------------------------------------------------------------------------
void NSourceListView::DrawBackground(NCarbonEvent &theEvent, const HIRect &/*theContent*/)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);
	HIRect				theRect;



	// Get the state we need
	theRect = GetBounds();



	// Draw the background
	cgContext.SetFillColor(kColorBackgroundFill);
	cgContext.FillRect(theRect);
}





//============================================================================
//		NSourceListView::DrawContent : Draw the content.
//----------------------------------------------------------------------------
void NSourceListView::DrawContent(NCarbonEvent &theEvent)
{	bool				isSelected, isEditing;
	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);
	UInt32				n, numRows;



	// Get the state we need
	numRows = mRows.size();



	// Draw the underlay
	DrawDragHilite(cgContext, true);



	// Draw the content
	for (n = 0; n < numRows; n++)
		{
		isSelected = (mSelection == n);
		isEditing  = (isSelected && mEditor != NULL);

		if (mRows[n].isHeader)
			DrawHeader(cgContext, mRows[n]);
		else
			DrawRow(   cgContext, mRows[n], isSelected, isEditing);
		}



	// Draw the overlay
	DrawDragHilite(cgContext, false);
}





//============================================================================
//		NSourceListView::GetContentBounds : Get the content bounds.
//----------------------------------------------------------------------------
HIRect NSourceListView::GetContentBounds(void) const
{


	// Get the content bounds
	//
	// The source list content is always anchored at the top left of the view,
	// rather than floating in the centre.
	return(ConvertImageToView(GetImageBounds()));
}





//============================================================================
//		NSourceListView::BuildRows : Build the rows.
//----------------------------------------------------------------------------
#pragma mark -
void NSourceListView::BuildRows(void)
{	NSourceListItemListIterator		theIter;
	HIRect							theRect;
	NString							theID;



	// Validate our state
	NN_ASSERT(mSelection  == kInvalidRow);
	NN_ASSERT(mDragTarget == kInvalidRow);



	// Get the state we need
	theRect             = GetBounds();
	theRect.size.height = kRowHeight;



	// Build the rows
	mRows.clear();

	for (theIter = mItems.begin(); theIter != mItems.end(); theIter++)
		{
		// Add the row
		AddRow(&(*theIter), theRect, 0);
		theRect.origin.y += kHeaderGap;
		
		
		// Select the first non-header item
		if (theID.IsEmpty() && !theIter->GetChildren()->empty())
			theID = theIter->GetChildren()->front().GetID();
		}



	// Update our state
	theRect.size.height = CGRectGetMaxY(theRect) - kRowHeight;

	SetImageSize(theRect.size);
	SetSelection(theID);
}





//============================================================================
//		NSourceListView::UpdateRows : Update the rows.
//----------------------------------------------------------------------------
void NSourceListView::UpdateRows(void)
{	CGSize				imageSize, superSize;
	NHIView				*superView;
	SLRowListIterator	theIter;



	// Get the state we need
	superView = GetSuperView();
	imageSize = GetImageSize();



	// Update the image size
	//
	// Source list views embedded in a scroll view need to auto-expand to track
	// the bounds of the scroll view.
	if (superView != NULL && superView->GetClassID().EqualTo(kHIScrollViewClassID))
		{
		superSize       = superView->GetBounds().size;
		imageSize.width = superSize.width;

		if (imageSize.height > superSize.height)
			imageSize.width -= NThemeUtilities::GetMetric(kThemeMetricScrollBarWidth);

		SetImageSize(imageSize);
		}



	// Update the rows
	for (theIter = mRows.begin(); theIter != mRows.end(); theIter++)
		theIter->theRect.size.width = imageSize.width;
}





//============================================================================
//		NSourceListView::AddRow : Add a row.
//----------------------------------------------------------------------------
void NSourceListView::AddRow(NSourceListItem *theItem, HIRect &theRect, UInt32 theIndent)
{	NSourceListItemList				*theChildren;
	NSourceListItemListIterator		theIter;
	SLRow							theRow;



	// Add the row
	theRow.theRect   = theRect;
	theRow.isHeader  = (theIndent == 0);
	theRow.theIndent = theIndent;
	theRow.theItem   = theItem;

	mRows.push_back(theRow);
	theRect.origin.y += kRowHeight;



	// Add the children
	//
	// We can cast away the const, since the children belong to our internal items
	// and need to be mutable for us to update their state given a row reference.
	theChildren = (NSourceListItemList *) theItem->GetChildren();

	if (!theChildren->empty())
		{
		for (theIter = theChildren->begin(); theIter != theChildren->end(); theIter++)
			AddRow(&(*theIter), theRect, theIndent + 1);
		}
}





//============================================================================
//		NSourceListView::RevealRow : Reveal a row.
//----------------------------------------------------------------------------
void NSourceListView::RevealRow(UInt32 theIndex)
{	CGRect		theRect, theBounds;
	CGPoint		rowTL, rowBR;



	// Validate our parameters
	NN_ASSERT(theIndex < mRows.size());



	// Get the state we need
	theBounds = GetVisibleBounds();
	theRect   = mRows[theIndex].theRect;

	rowTL = theRect.origin;
	rowBR = CGPointMake(rowTL.x, CGRectGetMaxY(theRect));



	// Scroll the selection into view
	if (!CGRectContainsPoint(theBounds, rowTL) || !CGRectContainsPoint(theBounds, rowBR))
		SetImageOrigin(rowTL);
}





//============================================================================
//		NSourceListView::GetRowAtPoint : Get the row at a point.
//----------------------------------------------------------------------------
UInt32 NSourceListView::GetRowAtPoint(const HIPoint &thePoint)
{	UInt32		n, numRows;
	HIPoint		imagePoint;



	// Get the state we need
	imagePoint = ConvertViewToImage(thePoint);
	numRows    = mRows.size();



	// Find the row
	for (n = 0; n < numRows; n++)
		{
		if (CGRectContainsPoint(mRows[n].theRect, imagePoint))
			return(n);
		}
	
	return(kInvalidRow);
}





//============================================================================
//		NSourceListView::GetRowUnderDrag : Get the row under a drag.
//----------------------------------------------------------------------------
UInt32 NSourceListView::GetRowUnderDrag(const NDrag &theDrag)
{	CGPoint		thePoint;
	UInt32		theIndex;



	// Get the row
	thePoint = theDrag.GetMouseLocation(*this);
	theIndex = GetRowAtPoint(thePoint);
	
	return(theIndex);
}





//============================================================================
//		NSourceListView::SelectRow : Select a row.
//----------------------------------------------------------------------------
void NSourceListView::SelectRow(const HIPoint &thePoint)
{	UInt32		theIndex;



	// Select the row
	theIndex = GetRowAtPoint(thePoint);
	if (theIndex != kInvalidRow && CanSelect(theIndex))
		SetSelection(mRows[theIndex].theItem->GetID());
}





//============================================================================
//		NSourceListView::SelectRow : Select a row.
//----------------------------------------------------------------------------
void NSourceListView::SelectRow(SInt32 theDelta)
{	SInt32		theIndex;



	// Calculate the new selection
	theIndex = mSelection;

	do
		{
		theIndex = theIndex + theDelta;
		if (theIndex < 0 || theIndex >= (SInt32) mRows.size())
			return;
		}
	while (!CanSelect(theIndex));



	// Update the selection
	NN_ASSERT(theIndex >= 0 && theIndex < (SInt32) mRows.size());
	SetSelection(mRows[theIndex].theItem->GetID());
}





//============================================================================
//		NSourceListView::EditBegin : Begin editing a row.
//----------------------------------------------------------------------------
void NSourceListView::EditBegin(void)
{	NHIView		*editFrame;
	NString		theValue;
	HIRect		theRect;



	// Validate our parameters and state
	NN_ASSERT(CanRename(mSelection));
	NN_ASSERT(mEditor  == NULL);



	// Scroll the selection into view
	RevealRow(mSelection);



	// Get the state we need
	theRect  = ConvertImageToView(GetTextRect(mRows[mSelection]));
	theValue = mRows[mSelection].theItem->GetTitle();



	// Create our views
	mEditor   = new NEditText(NULL, CGRectZero, theValue, false, &kControlFontRowEditor);
	editFrame = NHIViewUtilities::CreateHIView();
	
	if (mEditor == NULL || editFrame == NULL)
		{
		SysBeep(30);
		delete mEditor;
		delete editFrame;
		
		mEditor = NULL;
		return;
		}



	// Position the frame
	//
	// We implement renaming by embedding an edit text control on top of the item that is being
	// renamed, and wait for that edit text to lose focus/accept the text to complete the edit.
	//
	// Unfortunately edit text controls always draw a focus ring (rdar://5609068), and so we need
	// to place the control in a framing view that lets us clip out the focus ring/border. There
	// are no metrics for the amount of space we need to leave, so these sizes are hard-coded.
	editFrame->SetFeatures(kHIViewFeatureAllowsSubviews);
	editFrame->AddSubView(mEditor);
	AddSubView(editFrame);

	theRect.origin.x   -= kEditTextFrameInset;
	theRect.size.width += kEditTextFrameInset;
	editFrame->SetFrame(theRect);

	theRect.origin.x     = kEditTextFrameInset;
	theRect.origin.y     = kEditTextFrameInset;
	theRect.size.width  -= (kEditTextFrameInset * 2.0f);
	theRect.size.height -= (kEditTextFrameInset * 2.0f);
	mEditor->SetFrame(theRect);



	// Configure the editor
	mEditor->SetCommandEditor(kHICommandNEditAccept);
	mEditor->SetCommandOnFocus(true);
	mEditor->SetMultiLine(false);
	
	NHIWindow(GetHIWindow()).SetKeyboardFocus(mEditor);
}





//============================================================================
//		NSourceListView::EditEnd : Finish editing a row.
//----------------------------------------------------------------------------
void NSourceListView::EditEnd(bool cancelEdit)
{	NHIView				*editFrame;
	NSourceListItem		*theItem;
	NString				theValue;



	// Check our state
	if (mEditor == NULL)
		return;



	// Get the state we need
	//
	// Extra assignment is to silence compiler warning in release builds.
	theItem = NULL;
	
	if (!cancelEdit)
		{
		// Get the state we need
		NN_ASSERT(CanRename(mSelection));

		theItem  = mRows[mSelection].theItem;
		theValue = mEditor->GetTextValue();


		// Cancel any edits that leave an empty name
		if (theValue.IsEmpty())
			cancelEdit = true;
		}



	// Destroy the editor
	//
	// The editor is placed inside a framing view, and will be destroyed by
	// that view when the framing view is destroyed.
	//
	// Destroying a view forces it to lose the focus, and will crash on 10.5
	// if the view being destroyed is currently losing the focus (i.e., we
	// destroy the editor when it loses the focus, which causes it to attempt
	// to lose the focus a second time).
	//
	// To allow the editor to be destroyed cleanly, we turn off its command
	// support and remove it from the view hierarchy so that we have no further
	// interaction with it. We can then use a delayed functor to do the actual
	// cleanup, after the editor has finished losing focus.
	mEditor->SetCommandEditor(kHICommandNone);

	editFrame = mEditor->GetSuperView();
	mEditor   = NULL;

	editFrame->RemoveFromSuperView();
	NSystemUtilities::DelayFunctor(BindFunction(NSourceListView::DeleteView, editFrame), 0.1f);



	// Finish the edit
	if (!cancelEdit)
		{
		theItem->SetTitle(theValue);
		BroadcastMessage(kMsgSourceItemRenamed, theItem);
		}
}





//============================================================================
//		NSourceListView::DrawDragHilite : Draw the drag hilite.
//----------------------------------------------------------------------------
void NSourceListView::DrawDragHilite(NCGContext &cgContext, bool isUnderlay)
{


	// Draw the hilite
	switch (mDragHilite) {
		case kDragHiliteNone:
			break;
		
		case kDragHiliteList:
			if (!isUnderlay)
				{
				cgContext.SetStrokeColor(kThemeBrushDragHilite);
				cgContext.SetLineWidth(kDragFrameWidth);
				cgContext.StrokeRect(CGRectInset(GetVisibleBounds(), kDragFrameWidth / 2.0f, kDragFrameWidth / 2.0f));
				}
			break;
		
		case kDragHiliteItem:
			cgContext.SetStrokeColor(kColorDragStroke);
			cgContext.SetFillColor(  kColorDragFill);
			cgContext.SetLineWidth(kDragFrameWidth);

			cgContext.AddRect(CGRectInset(mRows[mDragTarget].theRect, kDragFrameWidth, 0.0f), kDragCornerSize);
			cgContext.DrawPath(isUnderlay ? kCGPathFill : kCGPathStroke);
			break;
		}
}





//============================================================================
//		NSourceListView::DrawHeader : Draw a header.
//----------------------------------------------------------------------------
void NSourceListView::DrawHeader(NCGContext &cgContext, const SLRow &theRow)
{	HIRect		rowRect, textRect;
	NString		theText;



	// Get the state we need
	rowRect = theRow.theRect;
	theText = theRow.theItem->GetTitle().GetUpper();



	// Calculate the layout
	textRect.origin.x    = rowRect.origin.x + kRowGapFrame;
	textRect.origin.y    = rowRect.origin.y + kHeaderOffsetY;
	textRect.size.width  = CGRectGetMaxX(rowRect) - textRect.origin.x;
	textRect.size.height = rowRect.size.height;



	// Draw the row
	cgContext.SetFillColor(kColorTextHeaderShadow);
	cgContext.DrawThemeText(theText, CGRectOffset(textRect, 0.0f, 1.0f), kThemeTextHeader);

	cgContext.SetFillColor(kColorTextHeaderNormal);
	cgContext.DrawThemeText(theText, textRect, kThemeTextHeader);
}





//============================================================================
//		NSourceListView::DrawRow : Draw a row.
//----------------------------------------------------------------------------
void NSourceListView::DrawRow(NCGContext &cgContext, const SLRow &theRow, bool isSelected, bool isEditing)
{	HIRect		rowRect, iconRect, textRect;
	NString		theText;
	NIcon		theIcon;



	// Get the state we need
	rowRect = theRow.theRect;
	iconRect = GetIconRect(theRow);
	textRect = GetTextRect(theRow);

	theText = theRow.theItem->GetTitle();
	theIcon = theRow.theItem->GetIcon();

	if (isEditing)
		theText.Clear();



	// Draw a selected row
	if (isSelected)
		{
		cgContext.SaveState();
		mShadingActive.ApplyLinearShading(  kLinearTopBottom, CGRectGetMinY(rowRect), CGRectGetMaxY(rowRect));
		mShadingInactive.ApplyLinearShading(kLinearTopBottom, CGRectGetMinY(rowRect), CGRectGetMaxY(rowRect));
			
		cgContext.SaveState();
		cgContext.ClipToRect(rowRect);
		cgContext.DrawShading((mHasFocus || isEditing) ? mShadingActive : mShadingInactive);
		cgContext.RestoreState();

		cgContext.SetFillColor(kColorTextRowSelected);

		cgContext.DrawIcon(theIcon, iconRect);
		cgContext.SetShadow(kTextRowShadow);
		cgContext.DrawThemeText(theText, textRect, kThemeTextRowSelected);
		
		cgContext.RestoreState();
		}



	// Draw a normal row
	else
		{
		cgContext.SetFillColor(kColorTextRowNormal);

		cgContext.DrawIcon(     theIcon, iconRect);
		cgContext.DrawThemeText(theText, textRect, kThemeTextRowNormal);
		}
}





//============================================================================
//		NSourceListView::CanRemove : Can an item be removed.
//----------------------------------------------------------------------------
bool NSourceListView::CanRemove(UInt32 theIndex)
{


	// Validate our parameters
	NN_ASSERT(theIndex < mRows.size());

	
	
	// Check the item
	return(mRows[theIndex].theItem->GetOptions() & kSLItemCanRemove);
}





//============================================================================
//		NSourceListView::CanRename : Can an item be renamed.
//----------------------------------------------------------------------------
bool NSourceListView::CanRename(UInt32 theIndex)
{


	// Validate our parameters
	NN_ASSERT(theIndex < mRows.size());

	
	
	// Check the item
	return(mRows[theIndex].theItem->GetOptions() & kSLItemCanRename);
}





//============================================================================
//		NSourceListView::CanSelect : Can an item be selected.
//----------------------------------------------------------------------------
bool NSourceListView::CanSelect(UInt32 theIndex)
{


	// Validate our parameters
	NN_ASSERT(theIndex < mRows.size());

	
	
	// Check the item
	return(mRows[theIndex].theItem->GetOptions() & kSLItemCanSelect);
}





//============================================================================
//		NSourceListView::GetIconRect : Get the icon rect for a row.
//----------------------------------------------------------------------------
CGRect NSourceListView::GetIconRect(const SLRow &theRow)
{	CGRect		rowRect, theRect;



	// Get the state we need
	rowRect = theRow.theRect;



	// Get the icon rect
	theRect.origin.x    = rowRect.origin.x + kRowGapFrame + (theRow.theIndent * kRowIndent);
	theRect.origin.y    = rowRect.origin.y + ((rowRect.size.height - kIconSize) / 2.0f);
	theRect.size.width  = kIconSize;
	theRect.size.height = kIconSize;
	
	return(theRect);
}





//============================================================================
//		NSourceListView::GetTextRect : Get the text rect for a row.
//----------------------------------------------------------------------------
CGRect NSourceListView::GetTextRect(const SLRow &theRow)
{	CGRect		rowRect, iconRect, theRect;



	// Get the state we need
	rowRect  = theRow.theRect;
	iconRect = GetIconRect(theRow);



	// Get the text rect
	theRect.origin.x    = CGRectGetMaxX(iconRect) + kRowGapIconTitle;
	theRect.origin.y    = rowRect.origin.y;
	theRect.size.width  = CGRectGetMaxX(rowRect)  - theRect.origin.x;
	theRect.size.height = rowRect.size.height;

	return(theRect);
}





//============================================================================
//		NSourceListView::GetItemForID : Get an item by ID.
//----------------------------------------------------------------------------
const NSourceListItem *NSourceListView::GetItemForID(const NString &theID, const NSourceListItemList *theItems) const
{	const NSourceListItem					*theItem;
	NSourceListItemListConstIterator		theIter;



	// Find the item
	theItem = NULL;
	
	for (theIter = theItems->begin(); theIter != theItems->end() && theItem == NULL; theIter++)
		{
		if (theIter->GetID() == theID)
			theItem = &(*theIter);
		
		else if (theIter->GetChildren() != NULL)
			theItem = GetItemForID(theID, theIter->GetChildren());
		}
	
	return(theItem);
}





//============================================================================
//		NSourceListView::DeleteView : Delete a view.
//----------------------------------------------------------------------------
void NSourceListView::DeleteView(NHIView *theView)
{


	// Delete the view
	delete theView;
}

