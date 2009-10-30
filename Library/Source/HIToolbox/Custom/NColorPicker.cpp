/*	NAME:
		NColorPicker.cpp

	DESCRIPTION:
		Color picker view.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPasteboard.h"
#include "NMathUtilities.h"
#include "NCGContext.h"
#include "NColorPicker.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagAlphaVisible								= 'alph';
static const OSType kTagColor										= 'colr';


// Misc
static const float  kChipInset										= 5.0f;
static const UInt32 kCommandPickColor								= 'CPik';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NColorPicker, "com.nano.ncolorpicker", kHIBevelButtonClassID);





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NColorPicker *NColorPicker::mActivePicker;





//============================================================================
//		NColorPicker::NColorPicker : Constructor.
//----------------------------------------------------------------------------
NColorPicker::NColorPicker(HIWindowRef			theWindow,
							const HIRect		&theRect,
							const NColor		&theColor)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetColor(theColor);
}





//============================================================================
//		NColorPicker::~NColorPicker : Destructor.
//----------------------------------------------------------------------------
NColorPicker::~NColorPicker(void)
{


	// Reset the picker
	if (IsActivePicker())
		SetActivePicker(NULL);
}





//============================================================================
//		NColorPicker::IsAlphaVisible : Get the alpha visibility.
//----------------------------------------------------------------------------
bool NColorPicker::IsAlphaVisible(void) const
{


	// Get the state
	return(mAlphaVisible);
}





//============================================================================
//		NColorPicker::SetAlphaVisible : Set the alpha visibility.
//----------------------------------------------------------------------------
void NColorPicker::SetAlphaVisible(bool isVisible)
{


	// Set the state
	mAlphaVisible = isVisible;
}





//============================================================================
//		NColorPicker::GetColor : Get the color.
//----------------------------------------------------------------------------
NColor NColorPicker::GetColor(void) const
{


	// Get the color
	return(mColor);
}





//============================================================================
//		NColorPicker::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColorPicker::SetColor(const NColor &theColor)
{	bool				didChange;
	StAutoReleasePool	thePool;



	// Set the color
	didChange = (theColor != mColor);
	mColor    = theColor;
	
	if (!didChange)
		return;



	// Update the color panel
	SetNeedsDisplay();

	if (IsActivePicker())
		NColorPanel::SetColor(mColor);
}





//============================================================================
//		NColorPicker::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NColorPicker::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassCommand, kEventCommandProcess           },
										{ kEventClassControl, kEventControlDraw              },
										{ kEventClassControl, kEventControlVisibilityChanged },
										{ kEventClassNone,    kEventKindNone                 } };



	// Initialize ourselves
	mAlphaVisible = true;
	mColor        = kColorBlack;
	
	SetCommandID(kCommandPickColor);



	// Register for events
	AddEvents(theEvents);

	SetDraggedTypes(NArray(kUTNTypeColor.GetValue(), NULL));



	// Apply our properties
	if (HasProperty(               kPropertyNano, kTagColor))
		SetColor(GetPropertyString(kPropertyNano, kTagColor));

	if (HasProperty(                       kPropertyNano, kTagAlphaVisible))
		SetAlphaVisible(GetPropertyBoolean(kPropertyNano, kTagAlphaVisible));
}





//============================================================================
//		NColorPicker::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void NColorPicker::DoMessage(BroadcastMsg theMsg, void */*msgData*/)
{


	// Handle the message
	switch (theMsg) {
		case kMsgColorPanelTrackBegin:
		case kMsgColorPanelTrackEnd:
		case kMsgColorPanelChanged:
		case kMsgColorPanelClosed:
			DoPanelChange(theMsg);
			break;

		default:
			break;
		}
}





//============================================================================
//		NColorPicker::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NColorPicker::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kCommandPickColor:
			theErr = SetActivePicker(this);
			break;
		
		default:
			theErr = NHIView::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NColorPicker::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NColorPicker::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);
	HIRect				theBounds, theRect;
	OSStatus			theErr;



	// Draw the button
	theErr = CallNextHandler(theEvent);
	if (theErr != noErr)
		return(theErr);



	// Prepare to draw
	theBounds = GetBounds();
	theRect   = CGRectInset(theBounds, kChipInset, kChipInset);



	// Draw the chip
	if (IsEnabled())
		{
		if (NMathUtilities::NotEqual(mColor.GetAlpha(), 1.0f))
			{
			cgContext.SetFillColor(kColorBlack);
			cgContext.MoveTo(   theRect.origin.x,                      theRect.origin.y);
			cgContext.AddLineTo(theRect.origin.x + theRect.size.width, theRect.origin.y);
			cgContext.AddLineTo(theRect.origin.x,                      theRect.origin.y + theRect.size.height);
			cgContext.FillPath();

			cgContext.SetFillColor(kColorWhite);
			cgContext.MoveTo(   theRect.origin.x,                      theRect.origin.y + theRect.size.height);
			cgContext.AddLineTo(theRect.origin.x + theRect.size.width, theRect.origin.y + theRect.size.height);
			cgContext.AddLineTo(theRect.origin.x + theRect.size.width, theRect.origin.y);
			cgContext.FillPath();
			}
		}
	else
		cgContext.SetAlpha(kHIViewDisabledAlpha);
	
	cgContext.SetFillColor(mColor);
	cgContext.FillRect(theRect);



	// Draw the drag hilite
	if (IsDragTarget())
		cgContext.DrawDragHilite(theBounds);

	return(noErr);
}





//============================================================================
//		NColorPicker::DoEventControlVisibilityChanged : Handle kEventControlVisibilityChanged.
//----------------------------------------------------------------------------
OSStatus NColorPicker::DoEventControlVisibilityChanged(NCarbonEvent &/*theEvent*/)
{


	// Reset the picker
	if (IsActivePicker())
		SetActivePicker(NULL);

	return(eventNotHandledErr);
}





//============================================================================
//		NColorPicker::DoDragReceive : Receive a drag.
//----------------------------------------------------------------------------
DragActions NColorPicker::DoDragReceive(const NDrag &theDrag)
{


	// Set the color
	SetColor(theDrag.GetPasteboard().GetValueColor());



	// Update our listeners
	BroadcastMessage(kMsgColorPanelChanged, this);

	return(kDragActionGeneric);
}





//============================================================================
//		NColorPicker::IsActivePicker : Is this the active picker?
//----------------------------------------------------------------------------
#pragma mark -
bool NColorPicker::IsActivePicker(void)
{


	// Check our state
	return(this == mActivePicker);
}





//============================================================================
//		NColorPicker::SetActivePicker : Set the active picker.
//----------------------------------------------------------------------------
OSStatus NColorPicker::SetActivePicker(NColorPicker *thePicker)
{


	// Reset the existing picker
	if (mActivePicker != NULL)
		{
		mActivePicker->SetCurrentValue(0);
		NColorPanel::GetBroadcaster()->RemoveListener(mActivePicker);
		}



	// Update the active picker
	mActivePicker = thePicker;

	if (mActivePicker != NULL)
		{
		mActivePicker->SetCurrentValue(1);
		NColorPanel::GetBroadcaster()->AddListener(mActivePicker);
		}



	// Update the panel
	if (mActivePicker != NULL)
		{
		NColorPanel::SetAlphaVisible(mAlphaVisible);
		NColorPanel::SetColor(       mColor);
		}

	NColorPanel::SetVisible(mActivePicker != NULL);

	return(noErr);
}





//============================================================================
//		NColorPicker::DoPanelChange : Handle a color panel change.
//----------------------------------------------------------------------------
void NColorPicker::DoPanelChange(BroadcastMsg theMsg)
{


	// Update our state
	//
	// The color panel is always live, so we can just update on every message.
	SetColor(NColorPanel::GetColor());



	// Inform our listeners
	//
	// The active color picker receives messages from the color panel, and forwards
	// them on to its listeners to allow them to update their state when the picker
	// is updated as a result of a color panel change.
	//
	// We forward all messages, including track begin/end, to allow objects to use
	// those messages to drive their own actions (e.g., begining an undo group).
	BroadcastMessage(theMsg, this);
}


