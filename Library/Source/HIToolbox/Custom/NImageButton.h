/*	NAME:
		NImageButton.h

	DESCRIPTION:
		Image button.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NIMAGEBUTTON_HDR
#define NIMAGEBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NImageButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NImageButton);

										NImageButton(HIWindowRef theWindow, const HIRect &theRect);

	virtual								~NImageButton(void);


	// Get/set the active image
	NCGImage							GetActiveImage(void) const;
	void								SetActiveImage(const NCGImage &theImage);


	// Get/set the inactive image
	NCGImage							GetInactiveImage(void) const;
	void								SetInactiveImage(const NCGImage &theImage);


	// Get/set the pressed image
	NCGImage							GetPressedImage(void) const;
	void								SetPressedImage(const NCGImage &theImage);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	NCGImage							mImageActive;
	NCGImage							mImageInactive;
	NCGImage							mImagePressed;
};





#endif // NIMAGEBUTTON_HDR


