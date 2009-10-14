/*	NAME:
		NBezelView.h

	DESCRIPTION:
		Bezel view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NBEZELVIEW_HDR
#define NBEZELVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
static const float kBezelDefaultAlpha								= 0.5f;
static const float kBezelDefaultCorner								= 12.0f;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBezelView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NBezelView);

										NBezelView(HIWindowRef		theWindow,
													const HIRect	&theRect,
													float			theAlpha  = kBezelDefaultAlpha,
													float			theCorner = kBezelDefaultCorner);

	virtual								~NBezelView(void);


	// Get/set the alpha factor
	float								GetAlpha(void) const;
	void								SetAlpha(float theValue);


	// Get/set the corner radius
	float								GetCorner(void) const;
	void								SetCorner(float theValue);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	float								mAlpha;
	float								mCorner;
};





#endif // NBezelView_HDR


