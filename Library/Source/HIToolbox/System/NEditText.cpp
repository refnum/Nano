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
#include "NEditText.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagCommandEditor								= 'cmde';





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
//		NEditText::IsEmpty : Is the edit field empty?
//----------------------------------------------------------------------------
bool NEditText::IsEmpty(void) const
{


	// Check our contents
	return(GetTextValue().IsEmpty());
}





//============================================================================
//		NEditText::IsLocked : Is the edit field locked?
//----------------------------------------------------------------------------
bool NEditText::IsLocked(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlEditTextLockedTag));
}





//============================================================================
//		NEditText::SetLocked : Set the locked state.
//----------------------------------------------------------------------------
void NEditText::SetLocked(bool isLocked)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlEditTextLockedTag, isLocked);
}





//============================================================================
//		NEditText::IsMultiLine : Is this a mult-line text field?
//----------------------------------------------------------------------------
bool NEditText::IsMultiLine(void) const
{


	// Get the flag
	//
	// Note that the tag meaning is inverted from the static text control,
	// and so we negate it to expose the same interface as NStaticText.
	return(!GetControlBoolean(kControlEntireControl, kControlEditTextSingleLineTag));
}





//============================================================================
//		NEditText::SetMultiLine : Set the multi-line state.
//----------------------------------------------------------------------------
void NEditText::SetMultiLine(bool multiLine)
{


	// Set the flag
	//
	// Note that the tag meaning is inverted from the static text control,
	// and so we negate it to expose the same interface as NStaticText.
	SetControlBoolean(kControlEntireControl, kControlEditTextSingleLineTag, !multiLine);
}





//============================================================================
//      NEditText::IsCommandEditor : Is this a command-editor field?
//----------------------------------------------------------------------------
bool NEditText::IsCommandEditor(void) const
{


	// Get the state
	return(mCommandEditor != 0);
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
{	ControlEditTextSelectionRec		theValue = { theSelection.location, theSelection.length };



	// Set the selection
	if (theSelection.length == -1)
		theValue.selEnd = GetTextValue().GetSize() - theSelection.location;
		
	SetControlData(kControlEntireControl, kControlEditTextSelectionTag, sizeof(theValue), &theValue);
}





//============================================================================
//      NEditText::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NEditText::InitializeView(void)
{   const EventTypeSpec theEvents[] = { { kEventClassKeyboard,			kEventRawKeyDown						},
										{ kEventClassTextInput,			kEventTextInputOffsetToPos				},
										{ kEventClassTextInput,			kEventTextInputPosToOffset				},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetCharacters	},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetFont			},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetLength		},
										{ kEventClassTSMDocumentAccess, kEventTSMDocumentAccessGetSelectedRange	},
										{ kEventClassNone,				kEventKindNone							} };



	// Initialize ourselves
	mCommandEditor   = false;
	mMeasureBaseline = 0.0f;



    // Register for events
	//
	// Although the edit text control implements the kEventClassTSMDocumentAccess events, it
	// does not provide a working implementation of the kEventClassTextInput offset/pos events
	// that are also required to support Command-Control-D dictionary lookup (rdar://4854061).
	//
	// To enable this feature, we register for both sets of events and provide our own
	// kEventClassTextInput handlers to override the control's own handlers.
	AddEvents(theEvents);



    // Apply our properties
    if (HasProperty(                                kPropertyNano, kTagCommandEditor))
		SetCommandEditor((UInt32) GetPropertyOSType(kPropertyNano, kTagCommandEditor));
}





//============================================================================
//      NEditText::DoEventRawKeyDown : Handle kEventRawKeyDown.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventRawKeyDown(NCarbonEvent &theEvent)
{	UInt32		saveCmd;
	OSStatus    theErr;



    // Handle the key stroke
	theErr = eventNotHandledErr;

	switch (theEvent.GetKeyCode()) {
		case kKeyCodeEnter:
		case kKeyCodeNumPad_Enter:
		case kKeyCodeReturn:
			// Dispatch our command
			//
			// Assigning a command to an edit field causes it to dispatch the command when clicked.
			//
			// Since this is not typically useful when used in a command editor situation, we have
			// a parallel command that we temporarily swap into place when we need to dispatch.
			if (mCommandEditor != kHICommandNone)
				{
				saveCmd = GetCommandID();
				SetCommandID(mCommandEditor);

				theErr = SendCommand();
				SetCommandID(saveCmd);
				}
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
{	CFRange		theSelection;
	SInt32		theOffset;
	HIPoint		thePoint;
	NString		theText;



	// Update our state
	MeasureText();



	// Get the state we need
	theOffset    = theEvent.GetParameterSInt32(kEventParamTextInputSendTextOffset);
	theSelection = GetSelection();
	theText      = GetTextValue();



	// Calculate the UniChar offset
	//
	// The offset is provided in bytes (of UniChars), relative to the insertion point.
	theOffset = theSelection.location + (theOffset / 2);

	if (theOffset < 0 || theOffset >= (SInt32) theText.GetSize())
		return(errOffsetInvalid);



	// Calculate the position
	if (theOffset == 0)
		{
		thePoint   = mMeasureSizes[0];
		thePoint.x = 0.0f;
		}
	else
		thePoint = mMeasureSizes[theOffset - 1];

	thePoint.y -= NThemeUtilities::GetMetric(kThemeMetricEditTextWhitespace);
	thePoint.y -= NThemeUtilities::GetMetric(kThemeMetricEditTextFrameOutset);
	thePoint    = NGeometryUtilities::Transform(thePoint, kHICoordSpaceView, (HIViewRef) *this, kHICoordSpace72DPIGlobal);



	// Update the event
	theEvent.SetParameterHIPoint(kEventParamTextInputReplyPoint,               thePoint);
	theEvent.SetParameterSInt32( kEventParamTextInputReplyLineHeight, (SInt32) mMeasureBaseline);
	
	return(noErr);
}





//============================================================================
//      NEditText::DoEventTextInputPosToOffset : Handle kEventTextInputPosToOffset.
//----------------------------------------------------------------------------
OSStatus NEditText::DoEventTextInputPosToOffset(NCarbonEvent &theEvent)
{	UInt32		n, numItems;
	HIRect		theBounds;
	SInt32		theOffset;
	HIPoint		thePoint;
	bool		isInside;



	// Update our state
	MeasureText();



	// Get the state we need
	thePoint = theEvent.GetParameterHIPoint(kEventParamTextInputSendCurrentPoint);
	thePoint = NGeometryUtilities::Transform(thePoint, kHICoordSpace72DPIGlobal, NULL, kHICoordSpaceView, (HIViewRef) *this);

	theBounds            = GetBounds();
	theBounds.size.width = mMeasureSizes.back().x;



	// Locate the offset
	//
	// The offset is returned as bytes (of UniChars).
	isInside  = CGRectContainsPoint(theBounds, thePoint);
	theOffset = 0;

	if (isInside)
		{
		numItems = mMeasureSizes.size();

		for (n = 0; n < numItems; n++)
			{
			if (mMeasureSizes[n].x > thePoint.x)
				{
				theOffset = n;
				break;
				}
			}
		
		theOffset *= 2;
		}



	// Update the event
	theEvent.SetParameterBoolean(kEventParamTextInputReplyLeadingEdge, false);
	theEvent.SetParameterSInt32( kEventParamTextInputReplyTextOffset,  theOffset);
	theEvent.SetParameterSInt32( kEventParamTextInputReplyRegionClass, isInside ? kTSMInsideOfBody : kTSMOutsideOfBody);
	
	return(noErr);
}





//============================================================================
//      NEditText::MeasureText : Measure the text.
//----------------------------------------------------------------------------
#pragma mark -
void NEditText::MeasureText(void)
{	NString				theText, theSegment;
	UInt32				n, theLength;
	HIThemeTextInfo		theInfo;
	HISize				theSize;



	// Get the state we need
	theText = GetTextValue();



	// Rebuild the measurement state
	if (theText != mMeasureText)
		{
		// Update our state
		mMeasureText     = theText;
		mMeasureBaseline = 0.0f;
		mMeasureSizes.clear();



		// Measure the text
		//
		// Strictly speaking we should select an HIThemeTextInfo which matches the control font
		// style - this will produce incorrect measurements for edit fields that do not use the
		// standard system font.
		theLength = theText.GetSize();
		theInfo   = kThemeTextSystemFontLeft;

		for (n = 0; n < theLength; n++)
			{
			theSegment = theText.GetLeft(n + 1);
			theSize    = CGSizeZero;

			mMeasureBaseline = NThemeUtilities::GetTextDimensions(theSegment, theSize, theInfo);
			mMeasureSizes.push_back(CGPointMake(theSize.width, theSize.height));
			}
		}
}



