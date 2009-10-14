/*	NAME:
		CMenuView.h

	DESCRIPTION:
		Menu item view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CMENUVIEW_HDR
#define CMENUVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMenuItemView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CMenuView : public NMenuItemView {
public:
										DECLARE_HIVIEW_SUBCLASS(CMenuView);

										CMenuView(const NMenuItem	&menuItem,
													ThemeMenuType	menuType = kThemeMenuTypePullDown);

	virtual								~CMenuView(void);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(            NCarbonEvent &theEvent);
	OSStatus							DoEventControlGetOptimalBounds(NCarbonEvent &theEvent);


private:


};





#endif // CMENUVIEW_HDR


