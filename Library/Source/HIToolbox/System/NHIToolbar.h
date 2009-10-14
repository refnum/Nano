/*	NAME:
		NHIToolbar.h

	DESCRIPTION:
		Toolbar object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHITOOLBAR_HDR
#define NHITOOLBAR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NArray.h"
#include "NHIView.h"
#include "NHIObject.h"
#include "NHIWindow.h"
#include "NHIToolbarItem.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Options
static const OptionBits kHIToolbarAttributesDefault					= kHIToolbarAutoSavesConfig |
																	  kHIToolbarIsConfigurable;


// Identifiers
static const NString kHIToolbarUnifiedInsetIdentifier				= "com.nano.hitoolbar.unifiedinset";


// Messages
static const BroadcastMsg kMsgToolbarItemAdded						= 'tiad';
static const BroadcastMsg kMsgToolbarItemRemoved					= 'tirm';
static const BroadcastMsg kMsgToolbarLayoutChanged					= 'tlay';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIToolbar : public NHIObject, public NMsgBroadcaster {
friend class NHIToolbarItem;
public:
										NHIToolbar(const NString	&theID,
													OptionBits		theAttributes = kHIToolbarAttributesDefault);
	virtual								~NHIToolbar(void);


	// Get the identifier
	NString								GetIdentifier(void) const;


	// Get/set the attributes
	OptionBits							GetAttributes(void) const;
	void								SetAttributes(OptionBits setThese, OptionBits clearThese=kHIToolbarNoAttributes);


	// Get/set the display mode
	HIToolbarDisplayMode				GetDisplayMode(void) const;
	void								SetDisplayMode(HIToolbarDisplayMode theValue);


	// Get/set the display size
	HIToolbarDisplaySize				GetDisplaySize(void) const;
	void								SetDisplaySize(HIToolbarDisplaySize theValue);


	// Get the toolbar items
	NHIToolbarItemList					GetToolbarItems(void) const;


	// Get a toolbar item
	//
	// If the item is not currently present in the toolbar, NULL is returned.
	NHIToolbarItem						*GetToolbarItem(const NString &theIdentifier);
	

	// Attach the toolbar to a window
	void								Attach(NHIWindow *theWindow);


	// Operators
	operator							HIToolbarRef(void) const;


protected:
	// Get the toolbar contents
	//
	// Sub-classes must override to return the contents of the toolbar.
	virtual HIToolbarItemInfoList		GetToolbarContents(void) = 0;


	// Create a toolbar item
	//
	// Sub-classes should override if they wish to customize items after they
	// have been created, or to create their own sub-classes of NHIToolbarItem.
	virtual NHIToolbarItem				*CreateItem(const HIToolbarItemInfo &theInfo);


	// Create a toolbar item view
	//
	// Sub-classes should override if they wish to use a custom view for an item,
	// and call-through for items with unknown identifiers.
	virtual NHIView						*CreateItemView(const NHIToolbarItem *theItem);


	// Handle events
	OSStatus							DoEventToolbarCreateItemWithIdentifier(NCarbonEvent &theEvent);
	OSStatus							DoEventToolbarGetAllowedIdentifiers(   NCarbonEvent &theEvent);
	OSStatus							DoEventToolbarGetDefaultIdentifiers(   NCarbonEvent &theEvent);
	OSStatus							DoEventToolbarGetSelectableIdentifiers(NCarbonEvent &theEvent);
	OSStatus							DoEventToolbarItemAdded(               NCarbonEvent &theEvent);
	OSStatus							DoEventToolbarItemRemoved(             NCarbonEvent &theEvent);
	OSStatus							DoEventToolbarLayoutChanged(           NCarbonEvent &theEvent);


private:
	void								AddItems(NCarbonEvent &theEvent, OptionBits theMask);


private:
	HIToolbarItemInfoList				mItems;
};





#endif // NHITOOLBAR_HDR


