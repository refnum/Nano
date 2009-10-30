/*	NAME:
		NRecentItems.h

	DESCRIPTION:
		Recent Items menu.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NRECENTITEMS_HDR
#define NRECENTITEMS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NCarbonEventHandler.h"
#include "NContainer.h"
#include "NString.h"
#include "NAlias.h"
#include "NMenu.h"
#include "NIcon.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Commands
static const OSType kCmdRecentItems									= 'recn';


// Misc
static const UInt32 kDefaultRecentItemsLimit						= 10;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Menu item
typedef struct {
	NAlias		theItem;
	NIcon		parentIcon;
	NString		parentName;
} RecentItemFile;

typedef std::vector<RecentItemFile>					RecentItemFileList;
typedef RecentItemFileList::iterator				RecentItemFileListIterator;
typedef RecentItemFileList::const_iterator			RecentItemFileListConstIterator;


// Menu group
typedef struct RecentItemGroup {
	// Fields
	NString					groupID;
	NString					groupName;
	UInt32					fileLimit;
	RecentItemFileList		fileList;


	// Methods
	RecentItemGroup(const NString &theID="", const NString &theName="", UInt32 theLimit=kDefaultRecentItemsLimit)
	{
		groupID   = theID;
		groupName = theName;
		fileLimit = theLimit;
	}
	
} RecentItemGroup;

typedef std::vector<RecentItemGroup>				RecentItemGroupList;
typedef RecentItemGroupList::iterator				RecentItemGroupListIterator;
typedef RecentItemGroupList::const_iterator			RecentItemGroupListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRecentItems :	public NCarbonEventHandler,
						public NContainer {
public:
										NRecentItems( void);
	virtual								~NRecentItems(void);


	// Get the size
	//
	// Meta-items such as separators or group titles are ignored.
	UInt32								GetSize(void) const;


	// Clear the menu
	//
	// Meta-items such as separators or group titles are not removed.
	void								Clear(void);


	// Attach the menu
	//
	// Attaches the menu as a hierarchical menu under the specified menu item.
	void								AttachMenu(UInt32 theCmd=kCmdRecentItems);


	// Add an item
	//
	// The specified group must exist, or the item will not be added.
	void								AddItem(const NFile &theFile, const NString &groupID="");


	// Get/set the groups
	//
	// By default the menu contains a single group with an empty ID, which produces
	// the standard File menu "Open Recent" appearance.
	//
	// If a list of groups are assigned, the menu appearance will be that of the
	// "Recent Items" menu within the Apple menu.
	//
	// Each group should contain an identifier for archiving the group contents to
	// the preferences, a localized name for display in the menu, and the maximum
	// number of items for the group (or 0, to remove any limit).
	//
	// The fileList field is always ignored - files should be added with AddItem.
	RecentItemGroupList					GetGroups(void) const;
	void								SetGroups(const RecentItemGroupList &theGroups);


protected:
	// Handle events
	OSStatus							DoEventCommandProcess(      NCarbonEvent &theEvent);
	OSStatus							DoEventMenuDrawItemContent( NCarbonEvent &theEvent);
	OSStatus							DoEventMenuPopulate(        NCarbonEvent &theEvent);
	OSStatus							DoEventMenuMeasureItemWidth(NCarbonEvent &theEvent);


private:
	void								CommandSelect(MenuItemIndex theIndex);

	void								LoadMenu(void);
	void								SaveMenu(void);

	void								RebuildMenu(void);
	
	void								UpdateParents(void);
	void								UpdateParent( RecentItemFile &theItem);
	
	RecentItemGroup						*GetGroup(const NString &groupID);
	const RecentItemFile				*GetItem(MenuItemIndex theIndex);


private:
	NMenu								mMenu;
	RecentItemGroupList					mGroups;

	bool								mMenuDirty;
	RecentItemFileList					mMenuItems;
};





#endif // NRecentItems_HDR
