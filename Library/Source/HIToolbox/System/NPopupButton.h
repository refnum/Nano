/*	NAME:
		NPopupButton.h

	DESCRIPTION:
		Popup button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPOPUPBUTTON_HDR
#define NPOPUPBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGImage.h"
#include "NHIView.h"
#include "NMenu.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPopupButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NPopupButton);

										NPopupButton(HIWindowRef							theWindow,
														const HIRect						&theRect,
														MenuRef								theMenu,
														const NString						&theTitle  = "",
														bool								fixedWidth = true,
														SInt16								titleWidth = 0,
														SInt16								titleJust  = teFlushDefault);

	virtual								~NPopupButton(void);


	// Get/set the menu
	//
	// The button will retain any assigned menu.
	NMenu								GetMenu(void) const;
	void								SetMenu(const NMenu &theMenu);


	// Get/set the extra height
	//
	// The "extra height" adds additional whitespace to the button.
	UInt16								GetExtraHeight(void) const;
	void								SetExtraHeight(UInt16 theValue);


	// Get/set the selection mark
	//
	// When set, the menu will display a check mark next to the selected item.
	bool								IsSelectionMarked(void) const;
	void								SetSelectionMarked(bool isMarked);


	// Set the current value
	//
	// Sets the current value to the menu index of the specified command.
	void								SetCurrentValueByCommand(UInt32 theCmd);


	// Get/set the command for a menu item
	MenuCommand							GetItemCommand(MenuItemIndex theIndex);
	void								SetItemCommand(MenuItemIndex theIndex, MenuCommand theValue);


	// Get/set the text for a menu item
	NString								GetItemText(MenuItemIndex theIndex);
	void								SetItemText(MenuItemIndex theIndex, const NString &theValue);


	// Get/set the image for a menu item
	NCGImage							GetItemImage(MenuItemIndex theIndex);
	void								SetItemImage(MenuItemIndex theIndex, const NCGImage &theValue);


protected:
	// Initialize the view
	void								InitializeView(void);


private:


};





#endif // NPOPUPBUTTON_HDR


