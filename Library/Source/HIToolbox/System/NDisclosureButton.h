/*	NAME:
		NDisclosureButton.h

	DESCRIPTION:
		Disclosure button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDISCLOSUREBUTTON_HDR
#define NDISCLOSUREBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDisclosureButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NDisclosureButton);

										NDisclosureButton(HIWindowRef		theWindow,
															const HIRect	&theRect,
															UInt32			theValue   = kControlDisclosureButtonClosed,
															bool			autoToggle = true);

	virtual								~NDisclosureButton(void);


	// Get/set the open state
	bool								IsOpen(void) const;
	void								SetOpen(bool isOpen);


private:


};





#endif // NDISCLOSUREBUTTON_HDR


