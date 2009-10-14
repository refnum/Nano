/*	NAME:
		NIBWindow.h

	DESCRIPTION:
		Nib window loader.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NIBWINDOW_HDR
#define NIBWINDOW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NHIWindow.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NIBView;

typedef std::vector<NIBView*>									NIBViewList;
typedef NIBViewList::iterator									NIBViewListIterator;
typedef NIBViewList::const_iterator								NIBViewListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NIBWindow {
public:
										NIBWindow(void);
	virtual								~NIBWindow(void);


	// Get the views
	const NIBViewList					*GetViews(void);


	// Find the views
	void								FindViews(NHIWindow *theWindow);


	// Add a view to the list
	static void							AddView(NIBView *theView);


	// Reset the current view list
	static void							ResetViews(void);
	

private:
	NIBViewList							mViews;

	static NIBViewList					mCurrentViews;
};





#endif // NIBWINDOW_HDR


