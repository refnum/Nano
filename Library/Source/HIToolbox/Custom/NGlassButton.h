/*	NAME:
		NGlassButton.h

	DESCRIPTION:
		Glass button.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NGLASSBUTTON_HDR
#define NGLASSBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGShading.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NGlassButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NGlassButton);

										NGlassButton(HIWindowRef theWindow, const HIRect &theRect);

	virtual								~NGlassButton(void);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	NCGShading							mShadingTop;
	NCGShading							mShadingBottom;
};





#endif // NGLASSBUTTON_HDR


