/*	NAME:
		NShadingView.h

	DESCRIPTION:
		Shading view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NSHADINGVIEW_HDR
#define NSHADINGVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGShading.h"
#include "NCGContext.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NShadingView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NShadingView);

										NShadingView(HIWindowRef theWindow, const HIRect &theRect);

	virtual								~NShadingView(void);


	// Get/set the shading
	NCGShading							GetShading(void) const;
	void								SetShading(const NCGShading &theShading);


	// Get/set the orientation
	NLinearOrientation					GetOrientation(void) const;
	void								SetOrientation(NLinearOrientation theOrientation);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	NLinearOrientation					mOrientation;
	NCGShading							mShading;
};





#endif // NSHADINGVIEW_HDR


