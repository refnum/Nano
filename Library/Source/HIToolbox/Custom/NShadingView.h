/*	NAME:
		NShadingView.h

	DESCRIPTION:
		Shading view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NSHADINGVIEW_HDR
#define NSHADINGVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NScrollableView.h"
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


	// Get the shading
	NCGShading							*GetShading(void);


	// Get/set the frame style
	FrameStyle							GetFrameStyle(void) const;
	void								SetFrameStyle(FrameStyle theStyle);


	// Get/set the orientation
	NLinearOrientation					GetOrientation(void) const;
	void								SetOrientation(NLinearOrientation theOrientation);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	NCGShading							mShading;

	FrameStyle							mFrameStyle;
	NLinearOrientation					mOrientation;
};





#endif // NSHADINGVIEW_HDR


