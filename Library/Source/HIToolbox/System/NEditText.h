/*	NAME:
		NEditText.h

	DESCRIPTION:
		Edit text control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NEDITTEXT_HDR
#define NEDITTEXT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NEditText : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NEditText);

										NEditText(HIWindowRef						theWindow,
														const HIRect				&theRect,
														const NString				&theValue  = "",
														bool						isPassword = false,
														const ControlFontStyleRec	*theStyle  = NULL);
	virtual								~NEditText(void);


	// Is the edit field empty?
	bool								IsEmpty(void) const;


	// Get/set the locked state
	bool								IsLocked(void) const;
	void								SetLocked(bool isLocked);


	// Get/set the multi-line state
	bool								IsMultiLine(void) const;
	void								SetMultiLine(bool multiLine);


	// Get the password text
	NString								GetPasswordValue(void) const;


	// Get/set the selection
	ControlEditTextSelectionRec			GetSelection(void) const;
	void								SetSelection(const ControlEditTextSelectionRec &theSelection);
	void								SetSelection(SInt16 selStart=0, SInt16 selEnd=-1);


private:


};





#endif // NEDITTEXT_HDR


