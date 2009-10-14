/*	NAME:
		CMenuView.cpp

	DESCRIPTION:
		Menu item view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "CMenuView.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(CMenuView, "com.refnum.nano.nanodemo.cmenuview", kHIViewClassID);





//============================================================================
//		CMenuView::CMenuView : Constructor.
//----------------------------------------------------------------------------
CMenuView::CMenuView(const NMenuItem &menuItem, ThemeMenuType menuType)
{	OSStatus	theErr;



	// Create the view
	ConstructView(HIViewCreate(kClassID, this));



	// Configure the view
	theErr = SetMenuItem(menuItem, menuType);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		CMenuView::~CMenuView : Destructor.
//----------------------------------------------------------------------------
CMenuView::~CMenuView(void)
{
}





//============================================================================
//		CMenuView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void CMenuView::InitializeView(void)
{


	// Initialize the base class
	NMenuItemView::InitializeView();
	
	
	
	// Initialize ourselves
}





//============================================================================
//		CMenuView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus CMenuView::DoEventControlDraw(NCarbonEvent &theEvent)
{


	// Draw the view
	//
	// An NMenuItemView sub-class must override this method in order to
	// draw their content, but for this example we simply call through.
	return(NMenuItemView::DoEventControlDraw(theEvent));
}





//============================================================================
//		CMenuView::DoEventControlGetOptimalBounds : Handle kEventControlGetOptimalBounds.
//----------------------------------------------------------------------------
OSStatus CMenuView::DoEventControlGetOptimalBounds(NCarbonEvent &theEvent)
{


	// Draw the view
	//
	// An NMenuItemView sub-class must override this method in order to
	// measure their content, but for this example we simply call through.
	return(NMenuItemView::DoEventControlGetOptimalBounds(theEvent));
}
