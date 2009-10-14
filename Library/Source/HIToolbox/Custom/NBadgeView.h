/*	NAME:
		NBadgeView.h

	DESCRIPTION:
		Badge view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NBADGEVIEW_HDR
#define NBADGEVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NColor.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBadgeView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NBadgeView);

										NBadgeView(HIWindowRef		theWindow,
													const HIRect	&theRect,
													const NColor	&colorBadge = kColorRed,
													const NColor	&colorText  = kColorWhite);

	virtual								~NBadgeView(void);


	// Get/set the badge color
	NColor								GetBadgeColor(void) const;
	void								SetBadgeColor(const NColor &theColor);


	// Get/set the text color
	NColor								GetTextColor(void) const;
	void								SetTextColor(const NColor &theColor);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	void								AddCorner(CGPointList &thePoints, NPosition theCorner, const CGRect &theRect, float theScale);
	void								AddEdge(  CGPointList &thePoints, NPosition theSide,   const CGRect &theRect, float theScale);


private:
	NColor								mColorBadge;
	NColor								mColorText;
	
	NCGShading							mShading;
};





#endif // NBADGEVIEW_HDR


