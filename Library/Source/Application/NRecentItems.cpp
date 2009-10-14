/*	NAME:
		NRecentItems.cpp

	DESCRIPTION:
		Recent Items menu.
		
		THe metrics used to display parent items were derived from David
		Catmull's ACCELA library implementation - see ARecentItemsMenu.
		
		These metrics match the appearance of Cocoa's NDocumentController.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <map>

#include "Nano Build.h"
#include "NThemeUtilities.h"
#include "NMenuItem.h"
#include "NCFBundle.h"
#include "NCGContext.h"
#include "NAppleEvent.h"
#include "NCFPreferences.h"
#include "NRecentItems.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
//
// These values are archived to disk, and can never change.
static const NString kRecentItemsStateKey						= "NRecentItems";
static const NString kRecentItemsGroupIDKey						= "GroupID";
static const NString kRecentItemsGroupFilesKey					= "GroupFiles";


// Commands
static const MenuCommand kRecentItemsSelected					= 'rise';
static const MenuCommand kRecentItemsClearMenu					= 'ricl';


// Misc
static const NString kTextSeparator								= NString(" \xE2\x80\x94 ", kStringLength, kCFStringEncodingUTF8);
static const NString kTextCommandKey							= NString("\x23\x18 ",      kStringLength, kCFStringEncodingUTF8);
static const float kIconSize									= 16.0f;





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
typedef std::map<NString, RecentItemFile*>						NamedItemMap;
typedef NamedItemMap::iterator									NamedItemMapIterator;
typedef NamedItemMap::const_iterator							NamedItemMapConstIterator;





//============================================================================
//		NRecentItems::NRecentItems : Constructor.
//----------------------------------------------------------------------------
NRecentItems::NRecentItems(void)
{	EventTypeSpec			theEvents[] = {	{ kEventClassCommand,	kEventCommandProcess		},
											{ kEventClassMenu,		kEventMenuDrawItemContent	},
											{ kEventClassMenu,		kEventMenuMeasureItemWidth	},
											{ kEventClassMenu,		kEventMenuPopulate			},
											{ kEventClassNone,		kEventKindNone				} };
	RecentItemGroupList		theGroups;
	MenuRef					theMenu;
	OSStatus				theErr;



	// Create the menu
	theErr = CreateNewMenu(0, 0, &theMenu);
	NN_ASSERT_NOERR(theErr);



	// Initialize ourselves
	//
	// Since our menu retains the menu, we must release it.
	mMenuDirty = true;
	mMenu      = NMenu(theMenu);

	ReleaseMenu(theMenu);



	// Register for events
	InstallHandler(mMenu, theEvents);



	// Load the menu
	theGroups.push_back(RecentItemGroup());

	SetGroups(theGroups);
}





//============================================================================
//		NRecentItems::~NRecentItems : Destructor.
//----------------------------------------------------------------------------
NRecentItems::~NRecentItems(void)
{


	// Save our state
	SaveMenu();
}





//============================================================================
//		NRecentItems::IsEmpty : Is the menu empty?
//----------------------------------------------------------------------------
bool NRecentItems::IsEmpty(void) const
{


	// Check our state
	return(GetSize() == 0);
}





//============================================================================
//		NRecentItems::IsNotEmpty : Is the menu not empty?
//----------------------------------------------------------------------------
bool NRecentItems::IsNotEmpty(void) const
{


	// Check our state
	return(GetSize() != 0);
}





//============================================================================
//		NRecentItems::GetSize : Get the number of items.
//----------------------------------------------------------------------------
UInt32 NRecentItems::GetSize(void) const
{	UInt32								numItems;
	RecentItemGroupListConstIterator	theIter;



	// Count the items
	numItems = 0;
	
	for (theIter = mGroups.begin(); theIter != mGroups.end(); theIter++)
		numItems += theIter->fileList.size();
	
	return(numItems);
}





//============================================================================
//		NRecentItems::Clear : Clear the menu.
//----------------------------------------------------------------------------
void NRecentItems::Clear(void)
{	RecentItemGroupListIterator		theIter;



	// Clear the menu
	for (theIter = mGroups.begin(); theIter != mGroups.end(); theIter++)
		theIter->fileList.clear();

	mMenuDirty = true;
}





//============================================================================
//		NRecentItems::AttachMenu : Attach the menu.
//----------------------------------------------------------------------------
void NRecentItems::AttachMenu(UInt32 theCmd)
{	NMenuItem		menuItem(theCmd);



	// Attach our menu
	if (menuItem.IsValid())
		menuItem.SetSubMenu(mMenu);
}





//============================================================================
//		NRecentItems::AddItem : Add an item.
//----------------------------------------------------------------------------
void NRecentItems::AddItem(const NFile &theFile, const NString &groupID)
{	RecentItemGroup					*theGroup;
	RecentItemFileList				*theFiles;
	RecentItemFile					theItem;
	RecentItemFileListIterator		theIter;



	// Find the group
	//
	// The group must exist if any items are to be be added to it.
	theGroup = GetGroup(groupID);
	NN_ASSERT(theGroup != NULL);
	
	if (theGroup == NULL)
		return;



	// Purge the group if necessary
	theFiles = &theGroup->fileList;

	if (theGroup->fileLimit != 0)
		{
		if (theFiles->size() == theGroup->fileLimit)
			theFiles->pop_back();
		}



	// Purge any existing item
	for (theIter = theFiles->begin(); theIter != theFiles->end(); )
		{
		if (theIter->theItem.GetFile() == theFile)
			theIter = theFiles->erase(theIter);
		else
			theIter++;
		}



	// Add the item to the front
	//
	// Parent state is calculated on demand, since most items don't need it.
	theItem.theItem = NAlias(theFile);
	
	theFiles->insert(theFiles->begin(), 1, theItem);

	mMenuDirty = true;
}





//============================================================================
//		NRecentItems::GetGroups : Get the groups.
//----------------------------------------------------------------------------
RecentItemGroupList NRecentItems::GetGroups(void) const
{	RecentItemGroup						theGroup;
	RecentItemGroupList					theList;
	RecentItemGroupListConstIterator	theIter;



	// Get the groups
	//
	// The file list is not exposed to the caller. Strictly speaking we could
	// just return mGroups, but since SetGroups doesn't accept files we strip
	// out the files to give a consistent interface.
	for (theIter = mGroups.begin(); theIter != mGroups.end(); theIter++)
		{
		theGroup = *theIter;
		theGroup.fileList.clear();
		
		theList.push_back(theGroup);
		}
	
	return(theList);
}





//============================================================================
//		NRecentItems::SetGroups : Set the groups.
//----------------------------------------------------------------------------
void NRecentItems::SetGroups(const RecentItemGroupList &theGroups)
{	RecentItemGroupListConstIterator	theIter;



	// Validate our parameters
	for (theIter = theGroups.begin(); theIter != theGroups.end(); theIter++)
		NN_ASSERT(theIter->fileList.empty());



	// Set the groups
	//
	// When the groups are changed, we to reload the menu to see what files we
	// can fit into the new group list.
	//
	// This allows us to provide a default set of groups in our constructor, but
	// still let apps override this default group list at runtime (we will ignore
	// most of their saved state when we load for the first time, but will put
	// them into the correct groups when we re-load the menu).
	mGroups = theGroups;
	
	LoadMenu();
}





//============================================================================
//		NRecentItems::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NRecentItems::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd  = theEvent.GetHICommand();
	theErr = noErr;
	
	switch (hiCmd.commandID) {
		case kRecentItemsSelected:
			NN_ASSERT(hiCmd.attributes & kHICommandFromMenu);
			CommandSelect(hiCmd.source.menu.menuItemIndex);
			break;
			
		case kRecentItemsClearMenu:
			Clear();
			break;

		default:
			theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NRecentItems::DoEventMenuDrawItemContent : Handle kEventMenuDrawItemContent.
//----------------------------------------------------------------------------
OSStatus NRecentItems::DoEventMenuDrawItemContent(NCarbonEvent &theEvent)
{	NCGContext				cgContext(theEvent, true, kHIThemeOrientationInverted);
	NMenuItem				menuItem( theEvent);
	HIRect					theBounds;
	float					widthIcon;
	const RecentItemFile	*theItem;
	HIThemeTextInfo			textInfo;
	OSStatus				theErr;
	Rect					qdRect;
	CGrafPtr				qdPort;



	// Get the item
	theItem = GetItem(menuItem.GetIndex());

	if (theItem->parentName.IsEmpty())
		return(eventNotHandledErr);



	// Draw the standard content
	theErr = CallNextHandler(theEvent);
	if (theErr != noErr)
		return(theErr);



	// Get the state we need
	widthIcon      = kIconSize + NThemeUtilities::GetMetric(kThemeMetricMenuIconTrailingEdgeMargin);
	textInfo       = kThemeTextMenuItem;
	textInfo.state = kThemeStateInactive;

	if (menuItem.IsEnabled())
		{
		if (menuItem.GetMenu().GetTracking().itemSelected == menuItem.GetIndex())
			textInfo.state = kThemeStatePressed;
		else
			textInfo.state = kThemeStateActive;
		}



	// Get the text bounds
	//
	// The standard handler will return the area it used for the text, and we place our
	// content to the right. Since this rectangle is in the QD coordinate system, we
	// need to translate it to obtain the correct location in the CG coordinate system.
	GetPort(     &qdPort);
	GetPortBounds(qdPort, &qdRect);

	theBounds           = theEvent.GetParameterHIRect(kEventParamMenuTextBounds);
	theBounds.origin.y  = (qdRect.bottom - qdRect.top) - theBounds.origin.y - theBounds.size.height;



	// Draw the separator
	theBounds.origin.x  += theBounds.size.width;
	theBounds.size.width = NThemeUtilities::GetTextDimensions(kTextSeparator, textInfo).width;
	
	cgContext.DrawThemeText(kTextSeparator, theBounds, textInfo);



	// Draw the parent icon
	theBounds.origin.x  += theBounds.size.width;
	theBounds.size.width = kIconSize;
	
	cgContext.DrawIcon(theItem->parentIcon, theBounds);

	theBounds.size.width = widthIcon;



	// Draw the parent name
	theBounds.origin.x  += theBounds.size.width;
	theBounds.size.width = NThemeUtilities::GetTextDimensions(theItem->parentName, textInfo).width;
	
	cgContext.DrawThemeText(theItem->parentName, theBounds, textInfo);

	return(noErr);
}





//============================================================================
//		NRecentItems::DoEventMenuPopulate : Handle kEventMenuPopulate.
//----------------------------------------------------------------------------
OSStatus NRecentItems::DoEventMenuPopulate(NCarbonEvent &/*theEvent*/)
{


	// Rebuild the menu
	if (mMenuDirty)
		RebuildMenu();

	return(eventNotHandledErr);
}





//============================================================================
//		NRecentItems::DoEventMenuMeasureItemWidth : Handle kEventMenuMeasureItemWidth.
//----------------------------------------------------------------------------
OSStatus NRecentItems::DoEventMenuMeasureItemWidth(NCarbonEvent &theEvent)
{	float					theWidth, widthIcon;
	NMenuItem				menuItem(theEvent);
	const RecentItemFile	*theItem;



	// Get the item
	theItem = GetItem(menuItem.GetIndex());

	if (theItem->parentName.IsEmpty())
		return(eventNotHandledErr);



	// Get the state we need
	widthIcon = kIconSize + NThemeUtilities::GetMetric(kThemeMetricMenuIconTrailingEdgeMargin);



	// Calculate the initial width
	//
	// Menu items start with a mark column, an indent, and an icon.
	if (menuItem.GetAttributes() & kMenuAttrExcludesMarkColumn)
		theWidth = NThemeUtilities::GetMetric(kThemeMetricMenuExcludedMarkColumnWidth);
	else
		theWidth = NThemeUtilities::GetMetric(        kThemeMetricMenuMarkColumnWidth);

	theWidth += NThemeUtilities::GetMetric(kThemeMetricMenuIndentWidth) * menuItem.GetIndent();
	theWidth += widthIcon;



	// Add the item name
	theWidth += NThemeUtilities::GetMetric(kThemeMetricMenuTextLeadingEdgeMargin);
	theWidth += NThemeUtilities::GetTextDimensions(menuItem.GetText(), kThemeTextMenuItem).width;



	// Add the parent
	//
	// The parent adds a separator, another icon, and the parent name.
	theWidth += NThemeUtilities::GetTextDimensions(kTextSeparator, kThemeTextMenuItem).width;
	theWidth += widthIcon;
	theWidth += NThemeUtilities::GetTextDimensions(theItem->parentName, kThemeTextMenuItem).width;
	theWidth += NThemeUtilities::GetMetric(kThemeMetricMenuTextTrailingEdgeMargin);



	// Add the trailing space
	//
	// Menu items finish with the command key column, and some theme-specific "extra space".
	theWidth += NThemeUtilities::GetTextDimensions(kTextCommandKey, kThemeTextMenuItemCmdKey).width;
	theWidth += NThemeUtilities::GetMenuItemExtra(kThemeMenuItemHierBackground | kThemeMenuItemHasIcon).width;



	// Return the width
	theEvent.SetParameterSInt32(kEventParamMenuItemWidth, (SInt32) theWidth);

	return(noErr);
}





//============================================================================
//		NRecentItems::CommandSelect : Handle a select command.
//----------------------------------------------------------------------------
void NRecentItems::CommandSelect(MenuItemIndex theIndex)
{	NAppleEvent		theEvent(kCoreEventClass, kAEOpenDocuments);
	FSRef			theFSRef;
	NFile			theFile;
	OSStatus		theErr;



	// Get the file
	theFile  = GetItem(theIndex)->theItem.GetFile();
	theFSRef = theFile.GetFSRef();
	
	if (!theFile.IsValid() || !theFile.Exists())
		{
		SysBeep(30);
		return;
		}



	// Open it
	theErr  = theEvent.SetParameter(keyDirectObject, typeFSRef, sizeof(theFSRef), &theFSRef);
	theErr |= theEvent.Send();
	
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NRecentItems::LoadMenu : Load the menu.
//----------------------------------------------------------------------------
void NRecentItems::LoadMenu(void)
{	UInt32				n, m, numGroups, numFiles;
	NArray				theState, fileList;
	NDictionary			groupDict;
	RecentItemGroup		*theGroup;
	NString				groupID;
	RecentItemFile		theFile;



	// Validate our state
	NN_ASSERT(!mGroups.empty());



	// Load our state
	theState  = NCFPreferences::GetPrefs()->GetValueArray(kRecentItemsStateKey);
	numGroups = theState.GetSize();
	
	for (n = 0; n < numGroups; n++)
		{
		// Get the group state
		groupDict = theState.GetValueDictionary(n);
		groupID   = groupDict.GetValueString(kRecentItemsGroupIDKey);
		fileList  = groupDict.GetValueArray( kRecentItemsGroupFilesKey);



		// Find the group
		//
		// If this group isn't in our list, it must be discarded. If custom groups
		// are assigned at run-time, the menu will be reloaded and these items may
		// then find a position within the active groups.
		theGroup = GetGroup(groupID);
		if (theGroup == NULL)
			continue;



		// Add the files
		NN_ASSERT(theGroup->fileList.empty());
		numFiles = fileList.GetSize();
		
		for (m = 0; m < numFiles; m++)
			{
			theFile.theItem.SetData(fileList.GetValueData(m));

			if (theFile.theItem.IsValid())
				theGroup->fileList.push_back(theFile);
			}
		}

	mMenuDirty = true;
}





//============================================================================
//		NRecentItems::SaveMenu : Save the menu.
//----------------------------------------------------------------------------
void NRecentItems::SaveMenu(void)
{	NArray								theState, theFiles;
	RecentItemGroupListConstIterator	groupIter;
	RecentItemFileListConstIterator		fileIter;
	NDictionary							theGroup;



	// Validate our state
	NN_ASSERT(!mGroups.empty());



	// Save our state
	for (groupIter = mGroups.begin(); groupIter != mGroups.end(); groupIter++)
		{
		// Collect the files
		theFiles = NArray();
		for (fileIter = groupIter->fileList.begin(); fileIter != groupIter->fileList.end(); fileIter++)
			{
			if (fileIter->theItem.IsValid())
				theFiles.AppendValueData(fileIter->theItem.GetData());
			}



		// Save the group
		theGroup = NDictionary();
		theGroup.SetValueString(kRecentItemsGroupIDKey,    groupIter->groupID);
		theGroup.SetValueArray( kRecentItemsGroupFilesKey, theFiles);

		theState.AppendValueDictionary(theGroup);
		}

	NCFPreferences::GetPrefs()->SetValueArray(kRecentItemsStateKey, theState);
}





//============================================================================
//		NRecentItems::RebuildMenu : Rebuild the menu.
//----------------------------------------------------------------------------
void NRecentItems::RebuildMenu(void)
{	UInt32								groupIndent;
	MenuItemAttributes					clearAttr;
	NString								clearName;
	RecentItemFile						dummyItem;
	RecentItemGroupListConstIterator	groupIter;
	RecentItemFileListConstIterator		fileIter;
	NMenuItem							menuItem;
	NFile								theFile;



	// Validate our state
	NN_ASSERT(mMenuDirty);
	NN_ASSERT(!mGroups.empty());



	// Get the state we need
	//
	// Groups are either present or not, based on the ID state.
	clearAttr   = IsEmpty() ? kMenuItemAttrDisabled : 0;
	clearName   = NBundleString(kStringRecentItemsClear, "", kNanoStrings);
	groupIndent = mGroups.front().groupID.IsEmpty() ? 0 : 1;
	
	if (groupIndent == 0)
		NN_ASSERT(mGroups.size() == 1);



	// Reset our state
	UpdateParents();

	mMenu.Clear();
	mMenuItems.clear();



	// Build the menu
	for (groupIter = mGroups.begin(); groupIter != mGroups.end(); groupIter++)
		{
		// Begin the group
		if (groupIndent != 0)
			{
			NN_ASSERT(groupIter->groupID.IsNotEmpty());
			NN_ASSERT(groupIter->groupName.IsNotEmpty());
			
			mMenu.AppendItem(groupIter->groupName, 0, kMenuItemAttrSectionHeader);
			mMenuItems.push_back(dummyItem);
			}
		
		
		
		// Add the files
		for (fileIter = groupIter->fileList.begin(); fileIter != groupIter->fileList.end(); fileIter++)
			{
			theFile  = fileIter->theItem.GetFile();
			menuItem = mMenu.AppendItem(theFile.GetDisplayName(), kRecentItemsSelected);

			menuItem.SetIcon(NIcon(theFile));
			menuItem.SetHelpTag(theFile.GetPath());
			menuItem.SetIndent(groupIndent);
			
			if (fileIter->parentName.IsNotEmpty())
				menuItem.SetAttributes(kMenuItemAttrCustomDraw);

			mMenuItems.push_back(*fileIter);
			}



		// Finish the group
		if (groupIndent != 0 || !groupIter->fileList.empty())
			{
			mMenu.AppendSeparator();
			mMenuItems.push_back(dummyItem);
			}
		}
	
	mMenu.AppendItem(clearName, kRecentItemsClearMenu, clearAttr);



	// Update our state
	mMenuDirty = false;
}





//============================================================================
//		NRecentItems::UpdateParents : Update the parent state.
//----------------------------------------------------------------------------
void NRecentItems::UpdateParents(void)
{	NFile							theFile, theParent;
	RecentItemGroupListIterator		groupIter;
	RecentItemFileListIterator		fileIter;
	NamedItemMapIterator			itemIter;
	NamedItemMap					theItems;
	NString							theName;



	// Reset the parent state
	for (groupIter = mGroups.begin(); groupIter != mGroups.end(); groupIter++)
		{
		for (fileIter = groupIter->fileList.begin(); fileIter != groupIter->fileList.end(); fileIter++)
			fileIter->parentName.Clear();
		}



	// Identify common names
	//
	// Items that share the same name need to display their parent folder, to
	// allow the user to easily distinguish between them.
	//
	// We keep a map that maps item names to the first item that contained that
	// name; whenever we see a duplicate, we know that both the first item and
	// the current match need to show their parent.
	for (groupIter = mGroups.begin(); groupIter != mGroups.end(); groupIter++)
		{
		for (fileIter = groupIter->fileList.begin(); fileIter != groupIter->fileList.end(); fileIter++)
			{
			// Get the state we need
			theFile  = fileIter->theItem.GetFile();
			theName  = theFile.GetDisplayName();
			itemIter = theItems.find(theName);



			// Save unique items
			if (itemIter == theItems.end())
				theItems[theName] = &(*fileIter);
			
			
			// Until we have a clash
			else
				{
				UpdateParent(*itemIter->second);
				UpdateParent(*fileIter);
				}
			}
		}
}





//============================================================================
//		NRecentItems::UpdateParent : Update the parent state of an item.
//----------------------------------------------------------------------------
void NRecentItems::UpdateParent(RecentItemFile &theItem)
{	NFile	theParent;



	// Update the parent state
	if (theItem.parentName.IsEmpty())
		{
		theParent = theItem.theItem.GetFile().GetParent();
		
		theItem.parentIcon = NIcon(theParent);
		theItem.parentName = theParent.GetDisplayName();
		}
}





//============================================================================
//		NRecentItems::GetGroup : Get a group.
//----------------------------------------------------------------------------
RecentItemGroup *NRecentItems::GetGroup(const NString &groupID)
{	RecentItemGroupListIterator		theIter;



	// Find the group
	for (theIter = mGroups.begin(); theIter != mGroups.end(); theIter++)
		{
		if (theIter->groupID == groupID)
			return(&(*theIter));
		}
	
	return(NULL);
}





//============================================================================
//		NRecentItems::GetItem : Get an item.
//----------------------------------------------------------------------------
const RecentItemFile *NRecentItems::GetItem(MenuItemIndex theIndex)
{	RecentItemFile		*theItem;



	// Validate our parameters
	NN_ASSERT(theIndex >= 1 && theIndex <= mMenuItems.size());



	// Get the item
	//
	// We return a const pointer, since mMenuItems is a flattened representation
	// of the menu contents.
	//
	// This list is rebuilt whenever the menu is rebuilt, so is immutable.
	theItem = &mMenuItems[theIndex - 1];
	NN_ASSERT(theItem->theItem.IsValid());
	
	return(theItem);
}





