/*	NAME:
		NMenuItemView.h

	DESCRIPTION:
		Custom menu item view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NMENUITEMVIEW_HDR
#define NMENUITEMVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMenuItem.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMenuItemView : public NHIView {
public:
										NMenuItemView(void);
	virtual								~NMenuItemView(void);


	// Get/set the menu item
	NMenuItem							GetMenuItem(void) const;
	OSStatus							SetMenuItem(const NMenuItem	&menuItem,
													ThemeMenuType	menuType=kThemeMenuTypePullDown);


	// Is the menu item enabled?
	bool								IsMenuItemEnabled(void) const;


	// Is the menu item selected?
	bool								IsMenuItemSelected(void) const;


	// Get the menu item text
	NString								GetMenuItemText(void) const;
	

protected:
	// Initialize the view
	virtual void						InitializeView(void);


	// Handle menu item notifications
	virtual void						DoMenuItemChanged( void);
	virtual void						DoMenuItemPopulate(void);
	virtual HIViewPartCode				DoMenuItemHitTest(const HIPoint &thePoint);


	// Handle events
	virtual OSStatus					DoEventControlDraw(             NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetOptimalBounds( NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlHitTest(          NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlVisibilityChanged(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuCalculateSize(       NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuDrawItem(            NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuMeasureItemHeight(   NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuMeasureItemWidth(    NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuPopulate(            NCarbonEvent &theEvent);


private:
	NMenuItem							mMenuItem;

	NCarbonEventHandler					mDelegateMenu;
	NCarbonEventHandler					mDelegateMenuView;
	NCarbonEventHandler					mDelegateMenuWindow;
};





#endif // NMENUITEMVIEW_HDR


