/*	NAME:
		NCheckBox.h

	DESCRIPTION:
		Check box control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCHECKBOX_HDR
#define NCHECKBOX_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCheckBox : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NCheckBox);

										NCheckBox(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NString	&theTitle,
													SInt32			theValue   = kControlCheckBoxUncheckedValue,
													bool			autoToggle = true);

	virtual								~NCheckBox(void);


	// Get/set the checked state
	bool								IsChecked(void) const;
	void								SetChecked(bool isChecked);


	// Set the mixed state
	void								SetMixed(void);
	

private:


};





#endif // NCHECKBOX_HDR


