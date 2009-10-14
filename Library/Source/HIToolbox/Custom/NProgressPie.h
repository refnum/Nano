/*	NAME:
		NProgressPie.h

	DESCRIPTION:
		Xcode-style "progress pie" view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NPROGRESSPIE_HDR
#define NPROGRESSPIE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NProgressPie : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NProgressPie);

										NProgressPie(HIWindowRef		theWindow,
														const HIRect	&theRect,
														SInt32			valueInitial    = 0,
														SInt32			valueMinimum    = 0,
														SInt32			valueMaximum    = 100);

	virtual								~NProgressPie(void);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:


};





#endif // NPROGRESSPIE_HDR


