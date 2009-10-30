/*	NAME:
		NRatingBar.h

	DESCRIPTION:
		iTunes-style rating bar.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NRATINGBAR_HDR
#define NRATINGBAR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGContext.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRatingBar : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NRatingBar);

										NRatingBar(HIWindowRef		theWindow,
													const HIRect	&theRect,
													SInt32			valueInitial = 0,
													SInt32			valueMinimum = 0,
													SInt32			valueMaximum = 100);

	virtual								~NRatingBar(void);


	// Get/set the image
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theImage);
	

protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


	// Handle mouse tracking
	bool								DoTrackMouse(NCarbonEvent				&theEvent,
														const HIPoint			&theMouse,
														MouseTrackingResult		theTrack);


private:
	void								DrawItem( NCGContext &cgContext, const HIRect &theRect);
	void								DrawSpace(NCGContext &cgContext, const HIRect &theRect);


private:
	NCGImage							mImage;
};





#endif // NRATINGBAR_HDR


