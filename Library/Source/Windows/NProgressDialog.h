/*  NAME:
        NProgressDialog.h

    DESCRIPTION:
        Progress dialog.

    COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
    ___________________________________________________________________________
*/
#ifndef NPROGESSDIALOG_HDR
#define NPROGESSDIALOG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowController.h"
#include "NProgressBar.h"
#include "NPushButton.h"
#include "NStaticText.h"
#include "NFunctor.h"





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NProgressDialog : public NWindowController {
public:
										DECLARE_WINDOW_CONTROLLER(NProgressDialog);

										NProgressDialog(void);
	virtual								~NProgressDialog(void);


	// Get/set the cancel action
	NFunctor							GetCancelAction(void) const;
	void								SetCancelAction(const NFunctor &theAction);


	// Get/set the cancel visibility
	bool								IsCancelVisible(void) const;
	void								SetCancelVisible(bool isVisible);


	// Get/set the cancel availability
	bool								IsCancelEnabled(void) const;
	void								SetCancelEnabled(bool isEnabled);


	// Get/set the caption
	NString								GetCaption(void) const;
	void								SetCaption(const NString &theValue);


	// Get/set the progress
	float								GetProgress(void) const;
	void								SetProgress(float theValue);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


	// Handle commands
	OSStatus							CommandSheet(UInt32 theCmd);


private:
	NFunctor							mCancelAction;
	bool								mCancelVisible;
	bool								mCancelEnabled;
	
	CGRect								mProgressCompact;
	CGRect								mProgressExpanded;
	
	NIB_VIEW('text', NStaticText,		Caption);
	NIB_VIEW('canb', NPushButton,		Cancel);
	NIB_VIEW('prog', NProgressBar,		Progress);
};



#endif // NPROGESSDIALOG_HDR

