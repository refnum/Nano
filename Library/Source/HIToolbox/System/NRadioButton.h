/*	NAME:
		NRadioButton.h

	DESCRIPTION:
		Radio button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NRADIOBUTTON_HDR
#define NRADIOBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRadioButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NRadioButton);

										NRadioButton(HIWindowRef			theWindow,
														const HIRect		&theRect,
														const NString		&theTitle,
														SInt32				theValue   = kControlRadioButtonUncheckedValue,
														bool				autoToggle = true);

	virtual								~NRadioButton(void);


	// Get/set the selected state
	bool								IsSelected(void) const;
	void								SetSelected(bool isSelected);


	// Set the mixed state
	void								SetMixed(void);


private:


};





#endif // NRADIOBUTTON_HDR


