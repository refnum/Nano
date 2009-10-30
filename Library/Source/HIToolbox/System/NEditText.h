/*	NAME:
		NEditText.h

	DESCRIPTION:
		Edit text control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NEDITTEXT_HDR
#define NEDITTEXT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NSystemUtilities.h"
#include "NMsgBroadcaster.h"
#include "NSpellableView.h"
#include "NContainer.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgEditTextAccepted							= 'etok';
static const BroadcastMsg kMsgEditTextChanged							= 'etch';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NEditText :	public NSpellableView,
					public NMsgBroadcaster,
					public NContainer {
public:
										DECLARE_HIVIEW_SUBCLASS(NEditText);

										NEditText(HIWindowRef						theWindow,
														const HIRect				&theRect,
														const NString				&theValue  = "",
														bool						isPassword = false,
														const ControlFontStyleRec	*theStyle  = NULL);
	virtual								~NEditText(void);


	// Get the size
	UInt32								GetSize(void) const;


	// Get/set the read-only state
	bool								IsReadOnly(void) const;
	void								SetReadOnly(bool readOnly);


	// Get/set the multi-line state
	bool								IsMultiLine(void) const;
	void								SetMultiLine(bool multiLine);


	// Get/set the command-editor state
	//
	// A "command editor" will dispatch the specified command (often kHICommandNEditAccept)
	// when the user confirms the edit by moving the focus elsewhere or pressing return/enter.
	//
	// If the user presses escape, a command editor will dispatch kHICommandNEditCancel.
	bool								IsCommandEditor(void) const;
	void								SetCommandEditor(UInt32 theCmd);


	// Get/set the focus behaviour.
	//
	// When enabled, a command editor will dispatch its command if the user confirms the
	// edit by moving the focus out of the edit field.
	bool								GetCommandOnFocus(void) const;
	void								SetCommandOnFocus(bool theValue);


	// Get the password text
	NString								GetPasswordValue(void) const;


	// Get/set the selection
	CFRange								GetSelection(void) const;
	void								SetSelection(const CFRange &theSelection=kCFRangeAll);


protected:
    // Initialize the view
    void                                InitializeView(void);


	// Handle events
	virtual OSStatus					DoEventControlDraw(            NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlSetFocusPart(    NCarbonEvent &theEvent);
	virtual OSStatus					DoEventRawKeyDown(             NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputOffsetToPos(   NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputPosToOffset(   NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextAccepted(           NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextShouldChangeInRange(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextDidChange(          NCarbonEvent &theEvent);


private:
	void								UpdateLayout(void);

	float								GetLastWidth( void) const;
	HIThemeTextInfo						GetLayoutInfo(void) const;

	void								DispatchCommand(UInt32 theCmd);


private:
	UInt32								mCommandEditor;
	bool								mCommandOnFocus;

	CFRange								mLastSelection;
	CGRectList							mLayout;
};





#endif // NEDITTEXT_HDR


