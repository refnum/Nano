/*	NAME:
		NBevelButton.h

	DESCRIPTION:
		Bevel button control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NBEVELBUTTON_HDR
#define NBEVELBUTTON_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBevelButton : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NBevelButton);

										NBevelButton(HIWindowRef							theWindow,
														const HIRect						&theRect,
														const NString						&theTitle,
														ControlBevelThickness				theThickness  = kControlBevelButtonNormalBevel,
														ControlBevelButtonBehavior			theBehaviour  = kControlBehaviorPushbutton,
														const ControlButtonContentInfo		*theContent   = NULL,
														ControlBevelButtonMenuBehavior		menuBehaviour = kControlBehaviorCommandMenu,
														ControlBevelButtonMenuPlacement		menuPlacement = kControlBevelButtonMenuOnBottom);

	virtual								~NBevelButton(void);


	// Get/set the kind
	ThemeButtonKind						GetKind(void) const;
	void								SetKind(ThemeButtonKind theKind);


	// Get/set the icon
	NIcon								GetIcon(void) const;
	void								SetIcon(const NIcon &theIcon);


	// Get/set the image
	//
	// Image support requires Mac OS X 10.4 or later.
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theImage);


	// Get/set the transform
	IconTransformType					GetTransform(void) const;
	void								SetTransform(IconTransformType theTransform);


	// Get/set the menu
	//
	// The button will retain any assigned menu.
	MenuRef								GetMenu(void) const;
	void								SetMenu(MenuRef theMenu);


	// Get/set the menu value
	//
	// If the menu can have multiple values, the last selected value can be retrieved.
	MenuItemIndex						GetMenuValue(bool getLast=false) const;
	void								SetMenuValue(MenuItemIndex theIndex);


	// Get/set the menu delay
	EventTime							GetMenuDelay(void) const;
	void								SetMenuDelay(EventTime theDelay);


	// Get/set the menu glyph center state
	bool								IsMenuGlyphCentered(void) const;
	void								SetMenuGlyphCentered(bool isCentered);


	// Get/set the menu multi-value state
	bool								IsMenuMultiValue(void) const;
	void								SetMenuMultiValue(bool multiValue);


	// Get/set the text placement
	ControlButtonTextPlacement			GetTextPlacement(void) const;
	void								SetTextPlacement(ControlButtonTextPlacement thePlacement);


	// Get/set the text alignment
	ControlButtonTextAlignment			GetTextAlignment(void) const;
	void								SetTextAlignment(ControlButtonTextAlignment theAlignment);


	// Get/set the text offset
	SInt16								GetTextOffset(void) const;
	void								SetTextOffset(SInt16 theOffset);


	// Get/set the graphic alignment
	ControlButtonGraphicAlignment		GetGraphicAlignment(void) const;
	void								SetGraphicAlignment(ControlButtonGraphicAlignment theAlignment);


	// Get/set the graphic offset
	HIPoint								GetGraphicOffset(void) const;
	void								SetGraphicOffset(const HIPoint &theOffset);


	// Get/set if the graphic can be scaled
	bool								CanScaleGraphic(void) const;
	void								SetScaleGraphic(bool canScale);


protected:
	// Initialize the view
	void								InitializeView(void);


private:


};





#endif // NBEVELBUTTON_HDR


