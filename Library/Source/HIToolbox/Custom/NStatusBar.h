/*	NAME:
		NStatusBar.h

	DESCRIPTION:
		Status bar.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NSTATUSBAR_HDR
#define NSTATUSBAR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGShading.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStatusBar : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NStatusBar);

										NStatusBar(HIWindowRef theWindow, const HIRect &theRect);
	virtual								~NStatusBar(void);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	NCGShading							mShading;
};





#endif // NSTATUSBAR_HDR


