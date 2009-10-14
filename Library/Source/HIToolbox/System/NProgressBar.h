/*	NAME:
		NProgressBar.h

	DESCRIPTION:
		Progress bar control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPROGRESSBAR_HDR
#define NPROGRESSBAR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NProgressBar : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NProgressBar);

										NProgressBar(HIWindowRef		theWindow,
														const HIRect	&theRect,
														SInt32			valueInitial    = 0,
														SInt32			valueMinimum    = 0,
														SInt32			valueMaximum    = 100,
														bool			isIndeterminate = false);
	virtual								~NProgressBar(void);


	// Get/set the indeterminate state
	bool								IsIndeterminate(void) const;
	void								SetIndeterminate(bool isIndeterminate);


private:


};





#endif // NProgressBar_HDR


