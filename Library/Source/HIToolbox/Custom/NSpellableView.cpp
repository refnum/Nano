/*	NAME:
		NSpellableView.cpp

	DESCRIPTION:
		Spellable view.
		
		A spellable view has an associated spell checker and spell session, and
		is sub-classed by views with text content that can be spell checked.
		
		NSpellableView will handle spelling-related commands, and provide the
		expected "red underline" behaviour as an overlay to the text.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMenuItem.h"
#include "NSpellingPanel.h"
#include "NSpellableView.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const EventTime kSpellCheckTime								= 1.0f;

static const float  kHiliteRadius									= 1.25f;
static const NColor kHiliteColor									= NColor(1.00f, 0.00f, 0.00f, 0.7f);





//============================================================================
//		NSpellableView::NSpellableView : Constructor.
//----------------------------------------------------------------------------
NSpellableView::NSpellableView(void)
{
}





//============================================================================
//		NSpellableView::~NSpellableView : Destructor.
//----------------------------------------------------------------------------
NSpellableView::~NSpellableView(void)
{
}





//============================================================================
//		NSpellableView::IsSpellingEnabled : Get the spelling state.
//----------------------------------------------------------------------------
bool NSpellableView::IsSpellingEnabled(void) const
{

	// Get the state
	return(mCheckSpelling);
}





//============================================================================
//		NSpellableView::SetSpellingEnabled : Set the spelling state.
//----------------------------------------------------------------------------
void NSpellableView::SetSpellingEnabled(bool theFlag)
{

	// Set the state
	mCheckSpelling = theFlag;
}





//============================================================================
//		NSpellableView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NSpellableView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassCommand,    kEventCommandProcess      },
										{ kEventClassCommand,    kEventCommandUpdateStatus },
										{ kEventClassControl,    kEventControlDraw         },
										{ kEventClassNone,		 kEventKindNone            } };



	// Initialize ourselves
	mCheckSpelling  = true;
	mCheckAsYouType = false;
	
	mNextMistake = kCFRangeNone;



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NSpellableView::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NSpellableView::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandShowSpellingPanel:
		case kHICommandCheckSpelling:
		case kHICommandCheckSpellingAsYouType:
			theStatus = EnableIf(mCheckSpelling);
			break;

		default:
			theStatus = NHIView::GetCommandStatus(hiCmd);
			break;
		}



	// Update the menu item
	if (hiCmd.commandID == kHICommandCheckSpellingAsYouType)
		{
		NMenuItem	menuItem(kHICommandCheckSpellingAsYouType);
		
		menuItem.SetChecked(mCheckAsYouType);
		}

	return(theStatus);
}





//============================================================================
//		NSpellableView::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NSpellableView::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kHICommandShowSpellingPanel:
			theErr = CommandShowSpellingPanel();
			break;
		
		case kHICommandCheckSpelling:
			theErr = CommandCheckSpelling();
			break;
		
		case kHICommandCheckSpellingAsYouType:
			theErr = CommandCheckSpellingAsYouType();
			break;
		
		case kHICommandChangeSpelling:
			theErr = CommandChangeSpelling(theEvent);
			break;
		
		case kHICommandIgnoreSpelling:
			theErr = CommandIgnoreSpelling(theEvent);
			break;
		
		default:
			theErr = NHIView::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NSpellableView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NSpellableView::DoEventControlDraw(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Draw the view
	theErr = CallNextHandler(theEvent);
	if (theErr == noErr)
		DrawMistakes(theEvent);

	return(theErr);
}





//============================================================================
//		NSpellableView::DoSpellTextChanged : Handle text changes.
//----------------------------------------------------------------------------
bool NSpellableView::DoSpellTextChanged(const CFRange &theRange, const NString &newText)
{	CFIndex					rangeLast, theDelta;
	bool					isDirty;
	CFRangeListIterator		theIter;



	// Get the state we need
	rangeLast = CFRangeGetLast(theRange);
	theDelta  = newText.GetSize() - theRange.length;
	isDirty   = false;



	// Update our state
	mNextMistake = kCFRangeNone;
	theIter      = mAsYouTypeMistakes.begin();
	
	while (theIter != mAsYouTypeMistakes.end())
		{
		// Drop mistakes that are being edited
		if (CFRangeOverlaps(*theIter, theRange))
			{
			theIter = mAsYouTypeMistakes.erase(theIter);
			isDirty = true;
			}
		else
			{
			// And shift mistakes after the edit up/down as needed
			if (theIter->location > rangeLast)
				{
				theIter->location += theDelta;
				isDirty            = true;
				}
			
			theIter++;
			}
		}
	
	return(isDirty);
}





//============================================================================
//		NSpellableView::DoSpellSelectionChanged : Handle selection changes.
//----------------------------------------------------------------------------
bool NSpellableView::DoSpellSelectionChanged(const CFRange &theRange)
{


	// Update our state
	if (mNextMistake != theRange)
		mNextMistake = kCFRangeNone;

	return(!mAsYouTypeMistakes.empty());
}





//============================================================================
//      NSpellableView::CommandShowSpellingPanel : Handle kHICommandShowSpellingPanel.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NSpellableView::CommandShowSpellingPanel(void)
{


	// Validate our state
	NN_ASSERT(mCheckSpelling);



	// Show the spelling panel
	NSpellingPanel::SetMistake(SpellNextMistake(), true);
	
	return(noErr);
}





//============================================================================
//      NSpellableView::CommandCheckSpelling : Handle kHICommandCheckSpelling.
//----------------------------------------------------------------------------
OSStatus NSpellableView::CommandCheckSpelling(void)
{


	// Validate our state
	NN_ASSERT(mCheckSpelling);



	// Update the spelling panel
	NSpellingPanel::SetMistake(SpellNextMistake(), false);

	return(noErr);
}





//============================================================================
//      NSpellableView::CommandCheckSpellingAsYouType : Handle kHICommandCheckSpellingAsYouType.
//----------------------------------------------------------------------------
OSStatus NSpellableView::CommandCheckSpellingAsYouType(void)
{


	// Validate our state
	NN_ASSERT(mCheckSpelling);



	// Update our state
	mCheckAsYouType = !mCheckAsYouType;
	mAsYouTypeMistakes.clear();
	
	SetNeedsDisplay();



	// Start/stop checking
	if (mCheckAsYouType)
		mTimer.AddTimer(BindSelf(NSpellableView::SpellAsYouType), kSpellCheckTime, kSpellCheckTime, true);
	else
		mTimer.RemoveTimer();
	
	return(noErr);
}





//============================================================================
//      NSpellableView::CommandChangeSpelling : Handle kHICommandChangeSpelling.
//----------------------------------------------------------------------------
OSStatus NSpellableView::CommandChangeSpelling(NCarbonEvent &theEvent)
{	NString		theWord, theText;
	CFRange		theSelection;



	// Validate our state
	NN_ASSERT(mCheckSpelling);



	// Get the state we need
	theWord      = theEvent.GetParameterString(kEventNParamSpellReplacement);
	theText      = GetTextValue();
	theSelection = GetSelection();



	// Replace the mistake
	ReplaceTextValue(theSelection, theWord);
	SetSelection(CFRangeMake(theSelection.location, theWord.GetSize()));

	return(noErr);
}





//============================================================================
//      NSpellableView::CommandIgnoreSpelling : Handle kHICommandIgnoreSpelling.
//----------------------------------------------------------------------------
OSStatus NSpellableView::CommandIgnoreSpelling(NCarbonEvent &theEvent)
{


	// Validate our state
	NN_ASSERT(mCheckSpelling);



	// Get the session ID
	theEvent.SetParameterSInt32(kEventNParamSpellSessionID, mSpellSession);
	return(noErr);
}





//============================================================================
//      NSpellableView::SpellNextMistake : Update the next mistake.
//----------------------------------------------------------------------------
NString NSpellableView::SpellNextMistake(void)
{	NString			theText, theWord;
	CFRange			theSelection;



	// Get the state we need
	theText      = GetTextValue();
	theSelection = GetSelection();



	// Find the next mistake
	mNextMistake = mSpellChecker.GetMistake(theText, CFRangeGetNext(theSelection), true, mSpellSession);
	if (mNextMistake != kCFRangeNone)
		{
		theWord = theText.GetString(mNextMistake);
		SetSelection(mNextMistake);
		}

	return(theWord);
}





//============================================================================
//      NSpellableView::SpellAsYouType : Update the as-you-type mistakes.
//----------------------------------------------------------------------------
void NSpellableView::SpellAsYouType(void)
{


	// Check our state
	if (!mCheckAsYouType)
		return;
	
	
	
	// Find the as-you-type mistakes
	mAsYouTypeMistakes = mSpellChecker.GetMistakes(GetTextValue(), mSpellSession);

	SetNeedsDisplay();
}





//============================================================================
//		NSpellableView::DrawMistakes : Draw the mistakes.
//----------------------------------------------------------------------------
void NSpellableView::DrawMistakes(NCarbonEvent &theEvent)
{	CGPoint						startPoint, endPoint;
	NCGContext					cgContext(theEvent);
	CFRange						theSelection;
	CFRangeList					theMistakes;
	CFRangeListConstIterator	theIter;



	// Get the state we need
	theSelection = GetSelection();



	// Build the visible mistakes
	//
	// Which mistakes are visible depends on the current spelling mode and
	// the selection. These rules were determined by observing Cocoa.
	if (mNextMistake != kCFRangeNone && mNextMistake == theSelection)
		theMistakes.push_back(mNextMistake);

	if (mCheckAsYouType)
		{
		for (theIter = mAsYouTypeMistakes.begin(); theIter != mAsYouTypeMistakes.end(); theIter++)
			{
			if (!CFRangeContains(*theIter, theSelection.location) && theSelection.location != CFRangeGetNext(*theIter))
				theMistakes.push_back(*theIter);
			}
		}



	// Draw the mistakes
	cgContext.SaveState();

	cgContext.SetFillColor(kHiliteColor);

	for (theIter = theMistakes.begin(); theIter != theMistakes.end(); theIter++)
		{
		startPoint = GetTextPosition(CFRangeGetFirst(*theIter), true);
		  endPoint = GetTextPosition(CFRangeGetLast( *theIter), false);

		startPoint.x += kHiliteRadius;
		startPoint.y -= kHiliteRadius;

		cgContext.BeginPath();
		
		 while (startPoint.x < endPoint.x)
			{
			cgContext.AddCircle(startPoint, kHiliteRadius);
			startPoint.x += (kHiliteRadius * 3.0f);
			}

		cgContext.FillPath();
		}

	cgContext.RestoreState();
}





//============================================================================
//		NSpellableView::GetTextPosition : Get the position of a character.
//----------------------------------------------------------------------------
CGPoint NSpellableView::GetTextPosition(UInt32 theOffset, bool leadingEdge)
{	NCarbonEvent		theEvent(kEventClassTextInput, kEventTextInputOffsetToPos);
	CFRange				theSelection;
	CGPoint				thePoint;
	OSStatus			theErr;



	// Prepare the event
	//
	// The offset is an UniChar-sized index, in bytes, relative to the selection.
	thePoint     = CGPointZero;
	theSelection = GetSelection();

	SetSelection(CFRangeMake(0, 0));

	theEvent.SetParameterSInt32( kEventParamTextInputSendTextOffset,  theOffset * 2);
	theEvent.SetParameterBoolean(kEventParamTextInputSendLeadingEdge, leadingEdge);



	// Get the position
	theErr = theEvent.SendToHIView(*this, kEventTargetDontPropagate);
	if (theErr == noErr)
		{
		thePoint = theEvent.GetParameterHIPoint(kEventParamTextInputReplyPoint);
		thePoint = NGeometryUtilities::Transform(thePoint, kHICoordSpace72DPIGlobal, NULL, kHICoordSpaceView, (HIViewRef) *this);
		}



	// Clean up
	SetSelection(theSelection);

	return(thePoint);
}




