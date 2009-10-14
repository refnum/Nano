/*	NAME:
		NFlatButton.h

	DESCRIPTION:
		Flat button.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NFLATBUTTON_HDR
#define NFLATBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NFlatButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NFlatButton);

										NFlatButton(HIWindowRef theWindow, const HIRect &theRect);

	virtual								~NFlatButton(void);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:


};





#endif // NFLATBUTTON_HDR


