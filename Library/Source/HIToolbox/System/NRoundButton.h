/*	NAME:
		NRoundButton.h

	DESCRIPTION:
		Round button control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NROUNDBUTTON_HDR
#define NROUNDBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRoundButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NRoundButton);

										NRoundButton(HIWindowRef				theWindow,
														const HIRect			&theRect,
														ControlRoundButtonSize	theSize = kControlRoundButtonNormalSize,
														bool					isHelp  = false);

	virtual								~NRoundButton(void);


	// Get/set the icon
	NIcon								GetIcon(void) const;
	void								SetIcon(const NIcon &theIcon);


	// Get/set the size
	//
	// A round button always has a fixed size, centered within its frame.
	ControlRoundButtonSize				GetSize(void) const;
	void								SetSize(ControlRoundButtonSize theSize);


private:


};





#endif // NROUNDBUTTON_HDR


