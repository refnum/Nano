/*	NAME:
		NEditText.cpp

	DESCRIPTION:
		Edit text control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NKeyboardUtilities.h"
#include "NThemeUtilities.h"
#include "NMenuItem.h"
#include "NEditText.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagCommandEditor								= 'cmde';
static const OSType kTagSpelling									= 'spel';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NEditText, kHITextFieldClassID, NULL);





//============================================================================
//		NEditText::NEditText : Constructor.
//----------------------------------------------------------------------------
NEditText::NEditText(HIWindowRef					theWindow,
						const HIRect				&theRect,
						const NString				&theValue,
						bool						isPassword,
						const ControlFontStyleRec	*theStyle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateEditUnicodeTextControl(NULL, &kQDRectZero, theValue, isPassword, theStyle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NEditText::~NEditText : Destructor.
//----------------------------------------------------------------------------
NEditText::~NEditText(void)
{
}





//============================================================================
//		NEditText::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NEditText::GetSize(void) const
{


	// Get the size
	return(GetControlUInt32(kControlEntireControl, kControlEditTextCharCount));
}





//============================================================================
//		NEditText::IsReadOnly : Get the read-only state.
//----------------------------------------------------------------------------
bool NEditText::IsReadOnly(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlEditTextLockedTag));
}





//============================================================================
//		NEditText::SetReadOnly : Set the read-only state.
//----------------------------------------------------------------------------
void NEditText::SetReadOnly(bool readOnly)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlEditTextLockedTag, readOnly);
}





//============================================================================
//		NEditText::IsMultiLine : Is this a mult-line text field?
//----------------------------------------------------------------------------
bool NEditText::IsMultiLine(void) const
{


	// Get the flag
	//
	// The edit text tag has the opposite meaning to the equivalent static
	// text tag, so we negate it to give the same interface as NStaticText.
	return(!GetControlBoolean(kControlEntireControl, kControlEditTextSingleLineTag));
}





//============================================================================
//		NEditText::SetMultiLine : Set the multi-line state.
//----------------------------------------------------------------------------
void NEditText::SetMultiLine(bool multiLine)
{


	// Set the flag
	//
	// The edit text tag has the opposite meaning to the equivalent static
	// text tag, so we negate it to give the same interface as NStaticText.
	SetControlBoolean(kControlEntireControl, kControlEditTextSingleLineTag, !multiLine);
}





//============================================================================
//      NEditText::IsCommandEditor : Is this a command-editor field?
//----------------------------------------------------------------------------
bool NEditText::IsCommandEditor(void) const
{


	// Get the state
	return(mCommandEditor != kHICommandNone);
}





//============================================================================
//      NEditText::SetCommandEditor : Set the command-editor state.
//----------------------------------------------------------------------------
void NEditText::SetCommandEditor(UInt32 theCmd)
{


    // Set the state
	mCommandEditor = theCmd;
}





//============================================================================
//      NEditText::GetCommandOnFocus : Get the focus behaviour.
//----------------------------------------------------------------------------
bool NEditText::GetCommandOnFocus(void) const
{


	// Get the state
	return(mCommandOnFocus);
}





//============================================================================
//      NEditText::SetCommandOnFocus : Set the focus behaviour.
//----------------------------------------------------------------------------
void NEditText::SetCommandOnFocus(bool theValue)
{


    // Set the state
	mCommandOnFocus = theValue;
}





//============================================================================
//		NEditText::GetPasswordValue : Get the password string.
//----------------------------------------------------------------------------
NString NEditText::GetPasswordValue(void) const
{


	// Get the password text
	return(GetControlString(kControlEntireControl, kControlEditTextPasswordCFStringTag));
}





//============================================================================
//		NEditText::GetSelection : Get the selection.
//----------------------------------------------------------------------------
CFRange NEditText::GetSelection(void) const
{	CFRange							theSelection;
	ControlEditTextSelectionRec		theValue;



	// Get the selection
	GetControlData(kControlEntireControl, kControlEditTextSelectionTag, sizeof(theValue), &theValue);
	theSelection = CFRangeMake(theValue.selStart, theValue.selEnd - theValue.selStart);

	return(theSelection);
}





//============================================================================
//		NEditText::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NEditText::SetSelection(const CFRange &theSelection)
{	ControlEditTextSelectionRec		theValue;



	// Set the selection
	theValue.selStart = theSelection.location;
	theValue.selEnd   = theSelection.location + theSelection.length;
	
	if (theSelection.length == -1)
		theValue.selEnd = GetSize() - theSelection.location;
		
	SetControlData(kControlEntireControl, kControlEditTextSelectionTag, sizeof(theValue), &theValue);
}





//============================================================================
//      NEditText::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NEditText::InitializeView(void)
{   const EventTypeSpec theEvents[] = { { kEventClassControl,			kEventControlSetFocusPart				},
										{ kEventClassKeyboard,			kEventRawKeyDown						},
										{ kEventClassTextInput,			kEventTextInputOffsetToPos				},
										{ kEventClassTextInput,			kEventTextInputPosToOffset				},
										{ kEventClassTextField,			kEventTextAccepted						},
										{ kEventClassTextField,			kEventTextShouldChangeInRange			},
										{ kEventClassTextField,			kEventTextDidChange						},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetCharacters	},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetFont			},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetLength		},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetSelectedRange	},
										{ kEventClassNone,				kEventKindNone							} };



	// Initialize the base class
	NSpellableView::InitializeView();



	// Initialize ourselves
	mCommandEditor  = kHICommandNone;
	mCommandOnFocus = false;
	
	mLastSelection = kCFRangeNone;

	SetOptions(kHIViewOptionSetsCursor);
	


    // Register for events
	//
	// Although the edit text control implements the kEventClassTSMDocumentAccess events,
	// it does not provide a working implementation of the kEventClassTextInput offset/pos
	// events that are needed to support Command-Control-D dictionary lookup (rdar://4854061).
	//
	// To enable this feature, we register for both sets of events and override the system
	// kEventClassTextInput handlers with working versions.
	//
	// In addition this control does not support the spelling kHICommands (rdar://5135601),
	// so we inherit from NSpellableView to enable this.
	AddEvents(theEvents);



    // Apply our properties
    if (HasProperty(                                kPropertyNano, kTagCommandEditor))
		SetCommandEditor((UInt32) GetPropertyOSType(kPropertyNano, kTagCommandEditor));

    if (HasProperty(                          kPropertyNano, kTagSpelling))
		{
		SetSpellingEnabled(         GetPropertyBoolean(kPropertyNano, kTagSpelling));
		SetSpellingAsYouTypeEnabled(GetPropertyBoolean(kPropertyNano, kTagSpelling));
		}
}





//============================================================================
//      NEditText::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventControlDraw(NCarbonEvent &theEvent)
{	CFRange		theSelection;



	// Check for selection changes
	//
	// Unfortunately we can't detect selection changes until we draw, which
	// means any attempt to invalidate ourselves will clip to the area that
	// is currently being drawn.
	//
	// Since we may be drawing due to the insertion cursor blinking, which
	// has a tiny clip, we add a deferred invalidation to draw everything.
	theSelection = GetSelection();
	if (theSelection != mLastSelection)
		{
		if (DoSpellSelectionChanged(theSelection))
			SetNeedsDisplay(true, NULL, kEventDurationMillisecond);

		mLastSelection = theSelection;
		}



	// Draw the view
	return(NSpellableView::DoEventControlDraw(theEvent));
}





//============================================================================
//		NEditText::DoEventControlSetFocusPart : Handle kEventControlSetFocusPart.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventControlSetFocusPart(NCarbonEvent &theEvent)
{	HIViewPartCode		oldFocus, theFocus;
	OSStatus			theErr;



	// Get the state we need
	oldFocus = GetFocus();



	// Handle the event
	theErr = CallNextHandler(theEvent);
	if (theErr != noErr)
		return(theErr);



	// Send the command
	if (mCommandOnFocus)
		{
		theFocus = theEvent.GetViewPartCode();
		if (oldFocus != kHIViewFocusNoPart && theFocus == kHIViewFocusNoPart)
			DispatchCommand(mCommandEditor);
		}

	return(theErr);
}





//============================================================================
//      NEditText::DoEventRawKeyDown : Handle kEventRawKeyDown.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventRawKeyDown(NCarbonEvent &theEvent)
{	OSStatus	theErr;



    // Handle the key stroke
	theErr = eventNotHandledErr;
	
	switch (theEvent.GetKeyCode()) {
		case kKeyCodeEnter:
		case kKeyCodeReturn:
		case kKeyCodeNumPadEnter:
			// Accept the edit
			//
			// Single-line edit fields should never contain newlines, so we
			// as per NSTextField we consume them and select our contents.
			DispatchCommand(mCommandEditor);

			if (!IsMultiLine())
				{
				SetSelection();
				theErr = noErr;
				}
			break;


		case kKeyCodeEscape:
			// Cancel the edit
			DispatchCommand(kHICommandNEditCancel);
			break;


		default:
			break;
		}

	return(theErr);
}





//============================================================================
//      NEditText::DoEventTextInputOffsetToPos : Handle kEventTextInputOffsetToPos.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventTextInputOffsetToPos(NCarbonEvent &theEvent)
{	CFRange			theSelection;
	bool			leadingEdge;
	SInt32			theOffset;
	HIPoint			thePoint;
	OSStatus		theErr;



	// Get the state we need
	theSelection = GetSelection();
	leadingEdge  = true;

	if (theEvent.HasParameter(kEventParamTextInputSendLeadingEdge))
		leadingEdge = theEvent.GetParameterBoolean(kEventParamTextInputSendLeadingEdge);



	// Check the offset
	//
	// The offset is an UniChar-sized index, in bytes, relative to the selection.
	theOffset = theEvent.GetParameterSInt32( kEventParamTextInputSendTextOffset);
	theOffset = theSelection.location + (theOffset / 2);

	if (theOffset < 0 || theOffset >= (SInt32) GetSize())
		return(errOffsetInvalid);



	// Dispatch the event
	//
	// The original event handler only works for points to the right of the selection,
	// and so we need to temporarily move the selection to the start of the text.
	SetSelection(CFRangeMake(0, 0));

	theEvent.SetParameterSInt32(kEventParamTextInputSendTextOffset, theOffset * 2);
	theErr = CallNextHandler(theEvent);
	NN_ASSERT(theErr == noErr || theErr == eventNotHandledErr);

	SetSelection(theSelection);



	// Handle trailing edge requests
	//
	// The original event handler also ignores the trailing edge flag.
	if (theErr == noErr && !leadingEdge)
		{
		UpdateLayout();

		thePoint    = theEvent.GetParameterHIPoint(kEventParamTextInputReplyPoint);
		thePoint.x += mLayout[theOffset].size.width;

		theEvent.SetParameterHIPoint(kEventParamTextInputReplyPoint, thePoint);
		}

	return(theErr);
}





//============================================================================
//      NEditText::DoEventTextInputPosToOffset : Handle kEventTextInputPosToOffset.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventTextInputPosToOffset(NCarbonEvent &theEvent)
{	CFRange			theSelection;
	UInt32			n, numChars;
	SInt32			theOffset;
	HIPoint			thePoint;
	bool			isInside;



	// Get the state we need
	UpdateLayout();

	theSelection = GetSelection();
	thePoint     = theEvent.GetParameterHIPoint(kEventParamTextInputSendCurrentPoint);
	numChars     = mLayout.size();



	// Find the offset
	//
	// The original event handler always returns 0, and so we perform our own
	// layout in order to determine which offset contains the point.
	isInside  = false;
	theOffset = 0;

	for (n = 0; n < numChars; n++)
		{
		if (CGRectContainsPoint(mLayout[n], thePoint))
			{
			theOffset = n * 2;
			isInside  = true;
			break;
			}
		}



	// Update the event
	//
	// The offset is an UniChar-sized index, in bytes, relative to the start.
	theEvent.SetParameterBoolean(kEventParamTextInputReplyLeadingEdge, false);
	theEvent.SetParameterSInt32( kEventParamTextInputReplyTextOffset,  theOffset);
	theEvent.SetParameterSInt32( kEventParamTextInputReplyRegionClass, isInside ? kTSMInsideOfBody : kTSMOutsideOfBody);

	return(noErr);
}





//============================================================================
//      NEditText::DoEventTextAccepted : Handle kEventTextAccepted.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventTextAccepted(NCarbonEvent &/*theEvent*/)
{


	// Inform our listeners
	//
	// Ths event is only sent to the view and its immediate parent, so we
	// broadcast our own message to allow arbitrary objects to receive it.
	BroadcastMessage(kMsgEditTextAccepted, this);

	return(eventNotHandledErr);
}





//============================================================================
//      NEditText::DoEventTextShouldChangeInRange : Handle kEventTextShouldChangeInRange.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventTextShouldChangeInRange(NCarbonEvent &theEvent)
{	CFRange		theRange;
	NString		theText;



	// Get the state we need
	theRange = theEvent.GetParameterCFRange(kEventParamTextSelection);
	theText  = theEvent.GetParameterString( kEventParamCandidateText);



	// Reset our state
	mLayout.clear();



	// Update the spelling
	if (DoSpellTextChanged(theRange, theText))
		SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//      NEditText::DoEventTextDidChange : Handle kEventTextDidChange.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventTextDidChange(NCarbonEvent &/*theEvent*/)
{


	// Inform our listeners
	//
	// Ths event is only sent to the view and its immediate parent, so we
	// broadcast our own message to allow arbitrary objects to receive it.
	BroadcastMessage(kMsgEditTextChanged, this);

	return(eventNotHandledErr);
}





//============================================================================
//      NEditText::UpdateLayout : Update the layout.
//----------------------------------------------------------------------------
void NEditText::UpdateLayout(void)
{	NCarbonEvent		theEvent(kEventClassTextInput, kEventTextInputOffsetToPos);
	SInt32				n, theSize, lineHeight;
	CFRange				theSelection;
	HIPoint				thePoint;
	HIRect				theRect;
	OSStatus			theErr;



	// Check our state
	if (!mLayout.empty())
		return;



	// Get the state we need
	theSelection = GetSelection();
	theSize      = GetSize();



	// Measure the text
	//
	// To perform layout for the dictionary pop-up/spelling features, we need to obtain the
	// rectangle used by each character.
	//
	// We can calculate these rectangles by requesting the offset for each character in turn,
	// and measuring the last character directly (since the edit text field doesn't support
	// access to the trailing edge of characters).
	//
	//
	// This approach doesn't handle edit text fields which have scrolled their content, but
	// there is no way to determine the visible origin used by the edit field when it draws.
	//
	// Since this means layout-based features will not function correctly when the text is
	// scrolled, we may eventually just replace the edit text control with an HITextView and
	// take over all aspects of this view.
	SetSelection(CFRangeMake(0, 0));

	for (n = 0; n < theSize; n++)
		{
		// Send the event
		//
		// The offset is an UniChar-sized index, in bytes, relative to the selection.
		theEvent.SetParameterSInt32(kEventParamTextInputSendTextOffset, n * 2);

		theErr = theEvent.SendToHIView(*this, kEventTargetDontPropagate);
		NN_ASSERT_NOERR(theErr);

		thePoint   = theEvent.GetParameterHIPoint(kEventParamTextInputReplyPoint);
		lineHeight = theEvent.GetParameterSInt32( kEventParamTextInputReplyLineHeight);


		// Save the layout
		theRect.origin.x    = thePoint.x;
		theRect.origin.y    = thePoint.y - lineHeight;
		theRect.size.width  = 0.0f;
		theRect.size.height = lineHeight;

		if (!mLayout.empty())
			mLayout.back().size.width = thePoint.x - mLayout.back().origin.x;

		mLayout.push_back(theRect);
		}

	if (!mLayout.empty())
		mLayout.back().size.width = GetLastWidth();



	// Clean up
	SetSelection(theSelection);
}





//============================================================================
//      NEditText::GetLastWidth : Get the last character's width.
//----------------------------------------------------------------------------
float NEditText::GetLastWidth(void) const
{	HISize		theSize;



	// Validate our state
	NN_ASSERT(IsNotEmpty());



	// Measure the last character
	theSize = kHISizeZero;
	NThemeUtilities::GetTextDimensions(GetTextValue().GetRight(1), theSize, GetLayoutInfo());
	
	return(theSize.width);
}





//============================================================================
//      NEditText::GetLayoutInfo : Get the text info for layout.
//----------------------------------------------------------------------------
HIThemeTextInfo NEditText::GetLayoutInfo(void) const
{	ControlFontStyleRec		fontStyle;
	HIThemeTextInfo			theInfo;



	// Get the text info
	//
	// Accurate layout is only provided when we're drawing with a theme font.
	fontStyle = GetFontStyle();
	theInfo   = kThemeTextSystemFontLeft;

	if (fontStyle.flags & kControlUseThemeFontIDMask)
		theInfo.fontID = (ThemeFontID) fontStyle.font;
	
	return(theInfo);
}





//============================================================================
//      NEditText::DispatchCommand : Dispatch a command.
//----------------------------------------------------------------------------
void NEditText::DispatchCommand(UInt32 theCmd)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Check our state
	if (mCommandEditor == kHICommandNone)
		return;



	// Build the command info
	memset(&hiCmd, 0x00, sizeof(hiCmd));
	
	hiCmd.attributes     = kHICommandFromControl;
	hiCmd.commandID      = theCmd;
	hiCmd.source.control = *this;



	// Dispatch the command
	theErr = ProcessHICommand((HICommand *) &hiCmd);
	NN_ASSERT(theErr == noErr || theErr == eventNotHandledErr);
}



