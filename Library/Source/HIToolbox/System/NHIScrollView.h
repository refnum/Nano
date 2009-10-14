/*	NAME:
		NHIScrollView.h

	DESCRIPTION:
		Scroll view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHISCROLLVIEW_HDR
#define NHISCROLLVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NColor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const OptionBits kHIScrollViewOptionsDefault			= kHIScrollViewOptionsVertScroll  |
															  kHIScrollViewOptionsHorizScroll |
															  kHIScrollViewOptionsAllowGrow;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIScrollView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NHIScrollView);

										NHIScrollView(HIWindowRef		theWindow,
														const HIRect	&theRect,
														OptionBits		theOptions = kHIScrollViewOptionsDefault);
	virtual								~NHIScrollView(void);


	// Is a navigation action possible?
	bool								CanNavigate(HIScrollViewAction theAction) const;


	// Navigate the view
	void								Navigate(HIScrollViewAction theAction);
	

	// Get/set the auto-hide state
	bool								IsScrollBarAutoHide(void) const;
	void								SetScrollBarAutoHide(bool autoHide);


	// Get/set the horizontal indent
	UInt32								GetHorizontalIndent(void) const;
	void								SetHorizontalIndent(UInt32 theValue);


	// Get/set the vertical indent
	UInt32								GetVerticalIndent(void) const;
	void								SetVerticalIndent(UInt32 theValue);


private:


};





#endif // NHISCROLLVIEW_HDR


