/*	NAME:
		NMembraneButton.h

	DESCRIPTION:
		Membrane button.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NMEMBRANEBUTTON_HDR
#define NMEMBRANEBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGShading.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMembraneButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NMembraneButton);

										NMembraneButton(HIWindowRef theWindow, const HIRect &theRect);

	virtual								~NMembraneButton(void);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	NCGShading							mShadingFace;
};





#endif // NMEMBRANEBUTTON_HDR


