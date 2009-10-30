/*	NAME:
		NSpellableView.h

	DESCRIPTION:
		Spellable view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSPELLABLEVIEW_HDR
#define NSPELLABLEVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSpellChecker.h"
#include "NSpellSession.h"
#include "NTimer.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSpellableView : public NHIView {
public:
										NSpellableView(void);
	virtual								~NSpellableView(void);


	// Get/set the spelling state
	bool								IsSpellingEnabled(void) const;
	void								SetSpellingEnabled(bool theFlag);


	// Get/set the as-you-type state
	bool								IsSpellingAsYouTypeEnabled(void) const;
	void								SetSpellingAsYouTypeEnabled(bool theFlag);


	// Get/set the selection
	virtual CFRange						GetSelection(void) const                              = 0;
	virtual void						SetSelection(const CFRange &theSelection=kCFRangeAll) = 0;


protected:
	// Initialize the view
	//
	// Sub-classes must call through to the base class.
	virtual void						InitializeView(void);


	// Get the status of a command
	//
	// Sub-classes must call through to the base class.
	virtual CommandStatus				GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle events
	//
	// Sub-classes must call through to the base class.
	virtual OSStatus					DoEventCommandProcess(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDraw(   NCarbonEvent &theEvent);


	// Handle spelling state changes
	//
	// Sub-classes must call through when their state changes.
	virtual bool						DoSpellTextChanged(     const CFRange &theRange, const NString &newText);
	virtual bool						DoSpellSelectionChanged(const CFRange &theRange);


private:
	OSStatus							CommandShowSpellingPanel(     void);
	OSStatus							CommandCheckSpelling(         void);
	OSStatus							CommandCheckSpellingAsYouType(void);
	OSStatus							CommandChangeSpelling(NCarbonEvent &theEvent);
	OSStatus							CommandIgnoreSpelling(NCarbonEvent &theEvent);

	NString								SpellNextMistake(void);
	void								SpellAsYouType(void);

	void								DrawMistakes(NCarbonEvent &theEvent);

	CGPoint								GetTextPosition(UInt32 theOffset, bool leadingEdge);


private:
	NSpellChecker						mSpellChecker;
	NSpellSessionID						mSpellSession;
	NTimer								mTimer;

	bool								mCheckSpelling;
	bool								mCheckAsYouType;

	CFRange								mNextMistake;
	CFRangeList							mAsYouTypeMistakes;
};





#endif // NSPELLABLEVIEW_HDR


