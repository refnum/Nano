/*	NAME:
		NGroupBox.h

	DESCRIPTION:
		Group box control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NGROUPBOX_HDR
#define NGROUPBOX_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NGroupBox : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NGroupBox);

										NGroupBox(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NString	&theTitle,
													bool			isPrimary);

										NGroupBox(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NString	&theTitle,
													bool			isPrimary,
													SInt32			theValue,
													bool			autoToggle = true);

										NGroupBox(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NString	&theTitle,
													bool			isPrimary,
													bool			variableWidth,
													SInt16			titleWidth = 0,
													SInt16			titleJust  = teFlushDefault);

	virtual								~NGroupBox(void);


	// Get the title box
	HIRect								GetTitleBox(void) const;


	// Get/set the menu
	//
	// Only available for pop-up group boxes. The group box will retain any assigned menu.
	MenuRef								GetMenu(void) const;
	void								SetMenu(MenuRef theMenu);


private:


};





#endif // NGROUPBOX_HDR


