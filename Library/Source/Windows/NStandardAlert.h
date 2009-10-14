/*	NAME:
		NStandardAlert.h

	DESCRIPTION:
		Standard alert dialog.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTANDARDALERT_HDR
#define NSTANDARDALERT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NHIWindow.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
static const NString kDefaultAlertText							= "**default**";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStandardAlert {
public:
										NStandardAlert(void);
	virtual								~NStandardAlert(void);


	// Is the dialog open?
	bool								IsOpen(void) const;


	// Open the dialog
	//
	// If a parent window is assigned, opening the dialog will return immediately
	// and the final dialog result will be dispatched as a kEventCommandProcess
	// to the parent window.
	//
	// The command ID will be one of kHICommandOK, kHICommandCancel, kHICommandOther,
	// or kHICommandAppHelp.
	DialogItemIndex						Open(void);
	

	// Close the dialog
	//
	// When run as a sheet, the dialog will close itself when a button is pressed.
	//
	// If the parent needs to be destroyed earlier (e.g., if the window is closed),
	// the sheet can be closed programatically.
	void								Close(UInt32 theCmd=kHICommandCancel);


	// Get/set the parent
	//
	// When a parent window is assigned, the dialog will appear as a window-modal
	// sheet rather than an application-modal dialog.
	NHIWindow							*GetParent(void) const;
	void								SetParent(NHIWindow *theParent);


	// Get/set the dialog type
	AlertType							GetType(void) const;
	void								SetType(AlertType theType);


	// Get/set if the dialog has a help button
	bool								HasHelp(void) const;
	void								SetHasHelp(bool hasHelp);


	// Get/set the dialog buttons
	DialogItemIndex						GetButtonDefault(void) const;
	DialogItemIndex						GetButtonCancel( void) const;

	void								SetButtonDefault(DialogItemIndex theButton);
	void								SetButtonCancel( DialogItemIndex theButton);


	// Get/set the dialog text
	NString								GetTextMajor(  void) const;
	NString								GetTextMinor(  void) const;
	NString								GetTextDefault(void) const;
	NString								GetTextCancel( void) const;
	NString								GetTextOther(  void) const;

	void								SetTextMajor(  const NString &theText);
	void								SetTextMinor(  const NString &theText);
	void								SetTextDefault(const NString &theText);
	void								SetTextCancel( const NString &theText);
	void								SetTextOther(  const NString &theText);


	// Display a standard alert
	//
	// Displays a simple alert dialog/sheet (by default, a note alert with
	// a single major string and an OK button).
	static DialogItemIndex				DoAlert(const NString &textMajor,
												const NString &textMinor   = "",
												const NString &textDefault = kDefaultAlertText,
												const NString &textCancel  = "",
												const NString &textOther   = "");


private:
	CFStringRef							GetButtonString(const NString &theText);


private:
	NHIWindow							*mParent;
	DialogRef							mDialog;

	AlertType							mType;
	AlertStdCFStringAlertParamRec		mState;
	
	NString								mTextMajor;
	NString								mTextMinor;
	NString								mTextDefault;
	NString								mTextCancel;
	NString								mTextOther;
};








#endif // NSTANDARDALERT_HDR


