/*	NAME:
		NPushButton.h

	DESCRIPTION:
		Push button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPUSHBUTTON_HDR
#define NPUSHBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPushButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NPushButton);

										NPushButton(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NString	&theTitle,
													UInt32			theCmd);
	virtual								~NPushButton(void);


	// Get/set the default state
	bool								IsDefault(void) const;
	void								SetDefault(bool isDefault);


	// Get/set the cancel state
	bool								IsCancel(void) const;
	void								SetCancel(bool isCancel);


private:


};





#endif // NPUSHBUTTON_HDR


