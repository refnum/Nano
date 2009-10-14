/*	NAME:
		NSpellingPanel.cpp

	DESCRIPTION:
		NSpellingPanel provides access to the system spelling panel. See NSpellChecker
		for programmatic access to the system spell check library.

		To support spell checking, objects should respond to the standard commands:

			Spelling...						Cmd-:		kHICommandShowSpellingPanel
			Check Spelling					Cmd-;		kHICommandCheckSpelling
			Check Spelling as You Type					kHICommandCheckSpellingAsYouType
		
		NSpellingPanel will dispatch additional commands as the user interacts
		with the AppKit spelling panel:
		
			(Find Next)				kHICommandCheckSpelling
			(Correct)				kHICommandChangeSpelling
			(Ignore)				kHICommandIgnoreSpelling

		The expected behaviour for each command is as follows:

			kHICommandShowSpellingPanel
			---------------------------
			Dispatched by a "Spelling..." menu item, the object should find the next
			mistake after the current selection (wrapping at the end) and select it.
			
			UpdatePanel should then be called with the mistake, or an empty string
			if there were no mistakes, requesting that the panel be shown.


			kHICommandCheckSpelling
			-----------------------
			Dispatched by a "Check Spelling" menu item, or by NSpellingPanel if the
			user selects the "Find Next" button in the spelling panel, the object
			should find the next mistake after the current selection (wrapping at
			the end) and select it.
			
			UpdatePanel should then be called with the mistake, or an empty string
			if there were no mistakes, without explicitly showing the panel.


			kHICommandCheckSpellingAsYouType
			--------------------------------
			Dispatched by a "Check Spelling as You Type" menu item, this command
			should set an internal "check as you type" flag in the object.
			
			The object should check this flag as the user modifies the text, and
			periodically (e.g., when a new word is started) perform a spell check.
			
			This spell check should only hilite mistakes in words that do not contain
			the insertion point/selection, and should not change the selection or
			update the spelling panel.


			kHICommandChangeSpelling
			------------------------
			This command is dispatched by NSpellingPanel when the user selects the
			"Correct" button in the spelling panel.
			
			The recipient should replace their selection with the corrected string,
			which is contained in the kEventNParamSpellReplacement parameter.
			
			The replacement text should then be selected.

		
			kHICommandIgnoreSpelling
			------------------------
			This command is dispatched by NSpellingPanel when the user selects the
			"Ignore" button in the spelling panel.
			
			The recipient should update the kEventNParamSpellSessionID parameter,
			to return the NSpellSessionID whose ignore list is to be updated.

		An example implementation of spelling support can be found in NEditText,
		which derives from NSpellableView to obtain the standard behaviour.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEventHandler.h"
#include "NSpellSession.h"
#include "NSpellingPanel.h"





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
@interface NSpellingDelegate : NSWindow

+ (NSpellingDelegate *) sharedDelegate;
- (id) initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)styleMask backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferCreation;
- (BOOL) canBecomeMainWindow;
- (void)  checkSpelling:(id) sender;
- (void) changeSpelling:(id) sender;
- (void) ignoreSpelling:(id) sender;

@end





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NSSpellChecker *NSpellingPanel::mSpellChecker;
NSPanel        *NSpellingPanel::mPanel;





//============================================================================
//		NSpellingDelegate::sharedDelegate : Get the shared delegate.
//----------------------------------------------------------------------------
@implementation NSpellingDelegate

+ (NSpellingDelegate *) sharedDelegate;
{	static NSpellingDelegate	*sSharedDelegate = NULL;



	// Get the delegate
	if (sSharedDelegate == NULL)
		sSharedDelegate = [[NSpellingDelegate alloc] initWithContentRect:NSMakeRect(0, 0, 10, 10)
															styleMask:NSBorderlessWindowMask
															  backing:NSBackingStoreBuffered
																defer:NO];

	return(sSharedDelegate);
}





//============================================================================
//		NSpellingDelegate::initWithContentRect : Initialize the window.
//----------------------------------------------------------------------------
- (id)initWithContentRect:(NSRect) contentRect styleMask:(unsigned int) styleMask backing:(NSBackingStoreType) bufferingType defer:(BOOL) deferCreation
{


	// Initialize ourselves
	self = [super initWithContentRect:contentRect
							styleMask:styleMask
							  backing:bufferingType
							    defer:deferCreation];

	return(self);
}





//============================================================================
//		NSpellingDelegate::canBecomeMainWindow : Can we become the main window?
//----------------------------------------------------------------------------
- (BOOL) canBecomeMainWindow
{


	// We can always become the main window
	return(YES);
}





//============================================================================
//		NSpellingDelegate::checkSpelling : Check the spelling.
//----------------------------------------------------------------------------
- (void) checkSpelling:(id) sender
{	NCarbonEvent		theEvent;



	// Suppress compiler warning
	(void) sender;



	// Check the spelling
	theEvent = NCarbonEvent::CreateCommand(kHICommandCheckSpelling);
	theEvent.SendToFocus();
}





//============================================================================
//		NSpellingDelegate::changeSpelling : Change a mistake.
//----------------------------------------------------------------------------
- (void) changeSpelling:(id) sender
{	NSString			*theWord;
	NCarbonEvent		theEvent;



	// Change the mistake
	//
	// The recipient should replace the selection with the corrected word.
	theWord  = [[sender selectedCell] stringValue];
	theEvent = NCarbonEvent::CreateCommand(kHICommandChangeSpelling);

	theEvent.SetParameter(kEventNParamSpellReplacement, typeCFStringRef, sizeof(CFStringRef), &theWord);
	theEvent.SendToFocus();
}





//============================================================================
//		NSpellingDelegate::ignoreSpelling : Ignore a mistake.
//----------------------------------------------------------------------------
- (void) ignoreSpelling:(id) sender
{	NSpellSessionID		sessionID;
	NSString			*theWord;
	NCarbonEvent		theEvent;



	// Get the session ID
	//
	// The recipient should return the session ID whose ignore list is to be updated.
	sessionID = kNewSpellSession;
	theEvent  = NCarbonEvent::CreateCommand(kHICommandIgnoreSpelling);

	if (theEvent.SendToFocus() == noErr)
		sessionID = (NSpellSessionID) theEvent.GetParameterSInt32(kEventNParamSpellSessionID);



	// Ignore the mistake
	theWord = [[sender selectedCell] stringValue];

	[[NSSpellChecker sharedSpellChecker] ignoreWord:theWord inSpellDocumentWithTag:sessionID];
}

@end // NSpellingDelegate





//============================================================================
//		NSpellingPanel::IsVisible : Is the panel visible?
//----------------------------------------------------------------------------
bool NSpellingPanel::IsVisible(void)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Get the visibility
	return([mPanel isVisible]);
}





//============================================================================
//		NSpellingPanel::SetVisible : Set the panel visibility.
//----------------------------------------------------------------------------
void NSpellingPanel::SetVisible(bool isVisible)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();



	// Set the visibility
	if (isVisible)
		[mPanel orderFront:nil];
	else
		[mPanel orderOut:nil];
}





//============================================================================
//		NSpellingPanel::SetMistake : Set the current mistake.
//----------------------------------------------------------------------------
void NSpellingPanel::SetMistake(const NString &theWord, bool showPanel)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	InitializePanel();
	


	// Update the spelling panel
	[mSpellChecker updateSpellingPanelWithMisspelledWord:theWord];
	
	if (showPanel)
		SetVisible();
}





//============================================================================
//		NSpellingPanel::InitializePanel : Initialize the panel.
//----------------------------------------------------------------------------
#pragma mark -
void NSpellingPanel::InitializePanel(void)
{	StAutoReleasePool	thePool;



	// Initialize the panel
	if (mSpellChecker == NULL)
		{
		// Initialize our state
		mSpellChecker = [NSSpellChecker sharedSpellChecker];
		mPanel        = [mSpellChecker spellingPanel];



		// Initialize the delegate
		//
		// Unfortunately NSSpellChecker does not provide a proper delegate API, and
		// uses two separate protocols (and one implicit message that must also be
		// provided by any object that can be spell checked) to communicate with
		// the target object.
		//
		// These messages are all sent to the first responder, rather than a proper
		// delegate (rdar://5132353).
		//
		// Since the responder chain is a Cocoa-specific concept, we create a hidden
		// window in order to provide a pseudo-delegate to catch these messages.
		[[NSpellingDelegate sharedDelegate] makeMainWindow];
		}
}


