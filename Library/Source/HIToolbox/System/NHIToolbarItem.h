/*	NAME:
		NHIToolbarItem.h

	DESCRIPTION:
		Toolbar item.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHITOOLBARITEM_HDR
#define NHITOOLBARITEM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NHIObject.h"
#include "NCGImage.h"
#include "NIcon.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Behaviours
static const OptionBits kHIToolbarItemBehaviourDefault					= 0;
static const OptionBits kHIToolbarItemBehaviourInConfig					= (1 << 0);
static const OptionBits kHIToolbarItemBehaviourIsDefault				= (1 << 1);
static const OptionBits kHIToolbarItemBehaviourIsSelectable				= (1 << 2);





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NHIToolbar;
class NHIToolbarItem;


// Toolbar item info
//
// The HIToolbarItemInfo structure holds the default state used to instantiate
// a new NHIToolbarItem, based on the most common usage of toolbars (i.e., a
// fixed set of images/strings that are loaded from the application bundle).
//
// If you wish to create an item with more complex state, you should override
// NHIToolbar::CreateItem to adjust the properties of newly created items, or
// instantiate your own sub-class of NHIToolbarItem.
typedef struct {
	NString				theIdentifier;				// Item identifier
	UInt32				theCommand;					// Item command
	NString				resImage;					// App bundle resource for image
	NString				keyLabel;					// Localized.strings key for label
	NString				keyHelpTagStd;				// Localized.strings key for standard help
	NString				keyHelpTagExt;				// Localized.strings key for extended help
	OptionBits			theBehaviour;				// Item behaviour
	OptionBits			theAttributes;				// Item attributes
} HIToolbarItemInfo;


// Lists
typedef std::vector<HIToolbarItemInfo>						HIToolbarItemInfoList;
typedef HIToolbarItemInfoList::iterator						HIToolbarItemInfoListIterator;
typedef HIToolbarItemInfoList::const_iterator				HIToolbarItemInfoListConstIterator;

typedef std::vector<NHIToolbarItem*>						NHIToolbarItemList;
typedef NHIToolbarItemList::iterator						NHIToolbarItemListIterator;
typedef NHIToolbarItemList::const_iterator					NHIToolbarItemListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIToolbarItem : public NHIObject {
public:
										NHIToolbarItem(NHIToolbar *theToolbar, const HIToolbarItemInfo &theInfo);
										NHIToolbarItem(HIToolbarItemRef hiToolbarItem);
	virtual								~NHIToolbarItem(void);


	// Is this item using this identifier?
	bool								IsIdentifier(const NString &theIdentifier) const;


	// Get the identifier
	NString								GetIdentifier(void) const;
	

	// Get the toolbar
	NHIToolbar							*GetToolbar(void) const;


	// Get/set the enabled state
	bool								IsEnabled(void) const;
	void								SetEnabled(bool isEnabled);


	// Get/set the attributes
	OptionBits							GetAttributes(void) const;
	void								SetAttributes(OptionBits setThese, OptionBits clearThese=kHIToolbarItemNoAttributes);


	// Get/set the label
	NString								GetLabel(void) const;
	void								SetLabel(const NString &theValue);


	// Get/set the help tag
	NString								GetHelpTag(bool isExtended) const;
	void								SetHelpTag(const NString &theValue, bool isExtended=false);


	// Get/set the command ID
	UInt32								GetCommandID(void) const;
	void								SetCommandID(UInt32 theCmd);


	// Get/set the menu
	MenuRef								CopyMenu(void) const;
	void								SetMenu(MenuRef theMenu);


	// Get/Set the image
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theImage);


	// Set the icon
	void								SetIcon(const NIcon &theIcon);


	// Get the custom view
	//
	// Returns NULL if the item does not have a custom view.
	NHIView								*GetView(void) const;


	// Operators
	operator							HIToolbarItemRef(void) const;


protected:
	// Handle events
	virtual OSStatus					DoEventToolbarItemCreateCustomView(NCarbonEvent &theEvent);
	

private:
	NHIToolbar							*mToolbar;
	NHIView								*mView;
};





#endif // NHITOOLBARITEM_HDR


