/*	NAME:
		NLittleArrows.h

	DESCRIPTION:
		Little arrows control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLITTLEARROWS_HDR
#define NLITTLEARROWS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLittleArrows : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NLittleArrows);

										NLittleArrows(HIWindowRef		theWindow,
														const HIRect	&theRect,
														SInt32			valueInitial   = 0,
														SInt32			valueMinimum   = 0,
														SInt32			valueMaximum   = 100,
														SInt32			valueIncrement = 1);
	virtual								~NLittleArrows(void);


	// Get/set the increment value
	SInt32								GetIncrementValue(void) const;
	void								SetIncrementValue(SInt32 theValue);


protected:
	// Initialize the view
	void								InitializeView(void);


private:
	static void							ControlAction(ControlRef theControl, ControlPartCode partCode);


private:
	static ControlActionUPP				mControlAction;
};





#endif // NLittleARROWS_HDR


