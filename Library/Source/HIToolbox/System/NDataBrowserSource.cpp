/*	NAME:
		NDataBrowserSource.cpp

	DESCRIPTION:
		DataBrowser source.
		
		The DataBrowser "source" object is responsible for providing the model
		for the DataBrowser view - it provides the content, and the view takes
		care of displaying that content to the user.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDataBrowser.h"
#include "NDataBrowserSource.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32 kKeyCodeBackscape								= 0x33;
static const UInt32 kKeyCodeDelete									= 0x75;
static const UInt32 kKeyCodeEnter									= 0x34;
static const UInt32 kKeyCodeNumPad_Enter							= 0x4C;
static const UInt32 kKeyCodeReturn									= 0x24;





//============================================================================
//		NDataBrowserSource::NDataBrowserSource : Constructor.
//----------------------------------------------------------------------------
NDataBrowserSource::NDataBrowserSource(void)
{


	// Initialize ourselves
	mBrowser = NULL;
}





//============================================================================
//		NDataBrowserSource::~NDataBrowserSource : Destructor.
//----------------------------------------------------------------------------
NDataBrowserSource::~NDataBrowserSource(void)
{


	// Inform our browser
	if (mBrowser != NULL)
		mBrowser->SetSource(NULL);
}





//============================================================================
//		NDataBrowserSource::GetBrowser : Get the browser.
//----------------------------------------------------------------------------
NDataBrowser *NDataBrowserSource::GetBrowser(void) const
{


	// Get the browser
	return(mBrowser);
}





//============================================================================
//		NDataBrowserSource::SetBrowser : Set the browser.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetBrowser(NDataBrowser *theBrowser)
{


	// Set the browser
	mBrowser = theBrowser;
}





//============================================================================
//		NDataBrowserSource::PopulateBrowser : Populate the browser.
//----------------------------------------------------------------------------
void NDataBrowserSource::PopulateBrowser(void)
{


	// Populate the browser
	//
	// Sub-classes should override to insert their DataBrowserItemIDs.
	//
	// If the source supports filtering, the browser's filter string
	// should be used to filter the candidates for the browser.
}





//============================================================================
//		NDataBrowserSource::GetListHeaders : Get the list headers.
//----------------------------------------------------------------------------
DBHeaderList NDataBrowserSource::GetListHeaders(void)
{	DBHeaderList	theHeaders;



	// Get the list headers
	//
	// Sub-classes should override to return the column headers for list view.
	return(theHeaders);
}





//============================================================================
//		NDataBrowserSource::SetItemData : Set the data for an item.
//----------------------------------------------------------------------------
OSStatus NDataBrowserSource::SetItemData(DataBrowserItemID			/*itemID*/,
											DataBrowserItemDataRef	/*itemData*/,
											DataBrowserPropertyID	/*propertyID*/)
{


	// Set the data for an item
	//
	// Sub-classes should override if their data is mutable.
	return(errDataBrowserPropertyNotSupported);
}





//============================================================================
//		NDataBrowserSource::GetItemHelp : Set the help for an item.
//----------------------------------------------------------------------------
void NDataBrowserSource::GetItemHelp(DataBrowserItemID			/*itemID*/,
										DataBrowserPropertyID	/*propertyID*/,
										HMContentRequest		/*theRequest*/,
										HMContentProvidedType	&helpType,
										HMHelpContentRec		&helpContent)
{


	// Get the help for an item
	//
	// Sub-classes should overide to provide help for the item.
	memset(&helpContent, 0x00, sizeof(helpContent));
	
	helpType            = kHMContentNotProvided;
	helpContent.version = kMacHelpVersion;
	helpContent.content[0].contentType = kHMNoContent;
	helpContent.content[1].contentType = kHMNoContent;
}





//============================================================================
//		NDataBrowserSource::CompareItems : Compare two items.
//----------------------------------------------------------------------------
CFComparisonResult NDataBrowserSource::CompareItems(DataBrowserItemID		/*itemOne*/,
													DataBrowserItemID		/*itemTwo*/,
													DataBrowserPropertyID	/*propertyID*/)
{


	// Compare the items
	//
	// Sub-classes should override if their content is sortable.
	return(kCFCompareEqualTo);
}





//============================================================================
//		NDataBrowserSource::ItemAdded : An item has been added.
//----------------------------------------------------------------------------
void NDataBrowserSource::ItemAdded(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserItemAdded, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ItemRemoved : An item has been removed.
//----------------------------------------------------------------------------
void NDataBrowserSource::ItemRemoved(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserItemRemoved, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ItemEditBegin : An item is being edited.
//----------------------------------------------------------------------------
void NDataBrowserSource::ItemEditBegin(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserItemEditBegin, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ItemEditEnd : An item has been edited.
//----------------------------------------------------------------------------
void NDataBrowserSource::ItemEditEnd(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserItemEditEnd, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ItemSelected : An item has been selected.
//----------------------------------------------------------------------------
void NDataBrowserSource::ItemSelected(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserItemSelected, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ItemDeselected : An item has been deselected.
//----------------------------------------------------------------------------
void NDataBrowserSource::ItemDeselected(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserItemDeselected, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ItemOpened : An item has been opened.
//----------------------------------------------------------------------------
void NDataBrowserSource::ItemOpened(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserItemOpened, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ContainerOpened : A container has been opened.
//----------------------------------------------------------------------------
void NDataBrowserSource::ContainerOpened(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserContainerOpened, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ContainerClosing : A container is closing.
//----------------------------------------------------------------------------
void NDataBrowserSource::ContainerClosing(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserContainerClosing, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ContainerClosed : A container has been closed.
//----------------------------------------------------------------------------
void NDataBrowserSource::ContainerClosed(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserContainerClosed, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ContainerSorting : A container is being sorted.
//----------------------------------------------------------------------------
void NDataBrowserSource::ContainerSorting(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserContainerSorting, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ContainerSorted : A container has been sorted.
//----------------------------------------------------------------------------
void NDataBrowserSource::ContainerSorted(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserContainerSorted, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::ContainerToggled : A container has been toggled.
//----------------------------------------------------------------------------
void NDataBrowserSource::ContainerToggled(DataBrowserItemID itemID)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserContainerToggled, (void *) itemID);
}





//============================================================================
//		NDataBrowserSource::SelectionRemove : Remove the selection.
//----------------------------------------------------------------------------
void NDataBrowserSource::SelectionRemove(void)
{


	// Remove the selection
	//
	// Sub-classes should override to implement removal of selected items,
	// either by removing items from the browser or simply repopulating.
}





//============================================================================
//		NDataBrowserSource::SelectionAccept : Accept the selection.
//----------------------------------------------------------------------------
void NDataBrowserSource::SelectionAccept(void)
{


	// Accept the selection
	//
	// Sub-classes should override to implement "acceptance" of selected items,
	// where acceptance is triggered by the user pressing enter while the browser
	// contains a selection.
}





//============================================================================
//		NDataBrowserSource::BrowserTarget : The browser target has changed.
//----------------------------------------------------------------------------
void NDataBrowserSource::BrowserTarget(void)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserChangedTarget, this);
}





//============================================================================
//		NDataBrowserSource::BrowserLayout : The browser layout has changed.
//----------------------------------------------------------------------------
void NDataBrowserSource::BrowserLayout(void)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserChangedLayout, this);
}





//============================================================================
//		NDataBrowserSource::BrowserSelection : The browser selection has changed.
//----------------------------------------------------------------------------
void NDataBrowserSource::BrowserSelection(void)
{


	// Inform our listeners
	BroadcastMessage(kMsgBrowserChangedSelection, this);
}





//============================================================================
//		NDataBrowserSource::BrowserRawKeyDown : Handle kEventRawKeyDown.
//----------------------------------------------------------------------------
OSStatus NDataBrowserSource::BrowserRawKeyDown(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Handle the key stroke
	theErr = noErr;
	
	switch (theEvent.GetKeyCode()) {
		case kKeyCodeBackscape:
		case kKeyCodeDelete:
			if (mBrowser->AreItemsSelected())
				SelectionRemove();
			break;
		
		case kKeyCodeEnter:
		case kKeyCodeNumPad_Enter:
		case kKeyCodeReturn:
			if (mBrowser->AreItemsSelected())
				SelectionAccept();
			break;
		
		default:
			theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NDataBrowserSource::CanDrag : Can an item be dragged?
//----------------------------------------------------------------------------
bool NDataBrowserSource::CanDrag(DataBrowserItemID /*itemID*/, DragReference /*theDrag*/)
{


	// Can an item be dragged?
	//
	// Sub-classes should override if their items can be dragged.
	return(false);
}





//============================================================================
//		NDataBrowserSource::AcceptDrag : Accept a drag.
//----------------------------------------------------------------------------
bool NDataBrowserSource::AcceptDrag(DataBrowserItemID /*itemID*/, DragReference /*theDrag*/)
{


	// Accept a drag
	//
	// Sub-classes should override if their items can be dragged.
	return(false);
}





//============================================================================
//		NDataBrowserSource::ReceiveDrag : Receive a drag.
//----------------------------------------------------------------------------
bool NDataBrowserSource::ReceiveDrag(DataBrowserItemID /*itemID*/, DragReference /*theDrag*/)
{


	// Receive a drag
	//
	// Sub-classes should override if their items can be dragged.
	return(false);
}





//============================================================================
//		NDataBrowserSource::FinishDrag : Finish a drag.
//----------------------------------------------------------------------------
void NDataBrowserSource::FinishDrag(OSStatus /*trackResult*/, DragReference /*theDrag*/)
{


	// Finish a drag
	//
	// Sub-classes should override if their items can be dragged.
}





//============================================================================
//		NDataBrowserSource::GetContextualMenu : Get the contextual menu for an item.
//----------------------------------------------------------------------------
MenuRef NDataBrowserSource::GetContextualMenu(UInt32 &helpType, AEDesc &/*theSelection*/)
{


	// Get the contextual menu
	//
	// Sub-classes should override if their items have a contextual menu.
	helpType = kCMHelpItemRemoveHelp;
	
	return(NULL);
}





//============================================================================
//		NDataBrowserSource::HandleContextualMenu : Handle the contextual menu for an item.
//----------------------------------------------------------------------------
void NDataBrowserSource::HandleContextualMenu(UInt32 /*selectionType*/, MenuRef /*theMenu*/, MenuItemIndex /*theIndex*/)
{


	// Handle the contextual menu
	//
	// Sub-classes should override if their items have a contextual menu.
}





//============================================================================
//		NDataBrowserSource::GetItemValue : Get an item's value.
//----------------------------------------------------------------------------
#pragma mark -
SInt32 NDataBrowserSource::GetItemValue(DataBrowserItemDataRef itemData) const
{	SInt32		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = 0;
	theErr   = GetDataBrowserItemDataValue(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemMinimum : Get an item's minimum value.
//----------------------------------------------------------------------------
SInt32 NDataBrowserSource::GetItemMinimum(DataBrowserItemDataRef itemData) const
{	SInt32		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = 0;
	theErr   = GetDataBrowserItemDataMinimum(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemMaximum : Get an item's maximum value.
//----------------------------------------------------------------------------
SInt32 NDataBrowserSource::GetItemMaximum(DataBrowserItemDataRef itemData) const
{	SInt32		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = 0;
	theErr   = GetDataBrowserItemDataMaximum(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemBool : Get an item's boolean state.
//----------------------------------------------------------------------------
bool NDataBrowserSource::GetItemBool(DataBrowserItemDataRef itemData) const
{	Boolean		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = false;
	theErr   = GetDataBrowserItemDataBooleanValue(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemText : Get an item's text.
//----------------------------------------------------------------------------
NString NDataBrowserSource::GetItemText(DataBrowserItemDataRef itemData) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the value
	//
	// Note that GetDataBrowserItemDataText does not follow the CF model of
	// Get/Copy/Create, and we take ownership of the returned string.
	theErr = GetDataBrowserItemDataText(itemData, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemIcon : Get an item's icon.
//----------------------------------------------------------------------------
NIcon NDataBrowserSource::GetItemIcon(DataBrowserItemDataRef itemData) const
{	NIcon			theValue;
	IconRef			theIcon;
	OSStatus		theErr;



	// Get the value
	theIcon = NULL;
	theErr  = GetDataBrowserItemDataIcon(itemData, &theIcon);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr && theIcon != NULL)
		{
		theValue = theIcon;
		theErr   = ReleaseIconRef(theIcon);
		NN_ASSERT_NOERR(theErr);
		}
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemDate : Get an item's date.
//----------------------------------------------------------------------------
NCFDate NDataBrowserSource::GetItemDate(DataBrowserItemDataRef itemData) const
{	NCFDate			theValue;
	LongDateTime	theTime;
	OSStatus		theErr;



	// Get the value
	theErr = GetDataBrowserItemDataLongDateTime(itemData, &theTime);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theValue = NCFDate::GetGregorianDate(theTime);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemMenu : Get an item's menu.
//----------------------------------------------------------------------------
MenuRef NDataBrowserSource::GetItemMenu(DataBrowserItemDataRef itemData) const
{	MenuRef		theValue;
	OSStatus	theErr;



	// Get the value
	theValue = NULL;
	theErr   = GetDataBrowserItemDataMenuRef(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemState : Get an item's draw state.
//----------------------------------------------------------------------------
ThemeDrawState NDataBrowserSource::GetItemState(DataBrowserItemDataRef itemData) const
{	ThemeDrawState		theValue;
	OSStatus			theErr;



	// Get the value
	theValue = kThemeStateInactive;
	theErr   = GetDataBrowserItemDataDrawState(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemButton : Get an item's button value.
//----------------------------------------------------------------------------
ThemeButtonValue NDataBrowserSource::GetItemButton(DataBrowserItemDataRef itemData) const
{	ThemeButtonValue	theValue;
	OSStatus			theErr;



	// Get the value
	theValue = kThemeButtonOff;
	theErr   = GetDataBrowserItemDataButtonValue(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemTransform : Get an item's icon transform.
//----------------------------------------------------------------------------
IconTransformType NDataBrowserSource::GetItemTransform(DataBrowserItemDataRef itemData) const
{	IconTransformType	theValue;
	OSStatus			theErr;



	// Get the value
	theValue = kTransformNone;
	theErr   = GetDataBrowserItemDataIconTransform(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemID : Get an item's ID.
//----------------------------------------------------------------------------
DataBrowserItemID NDataBrowserSource::GetItemID(DataBrowserItemDataRef itemData) const
{	DataBrowserItemID	theValue;
	OSStatus			theErr;



	// Get the value
	theValue = kDataBrowserNoItem;
	theErr   = GetDataBrowserItemDataItemID(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::GetItemPropertyID : Get an item's property ID.
//----------------------------------------------------------------------------
DataBrowserPropertyID NDataBrowserSource::GetItemPropertyID(DataBrowserItemDataRef itemData) const
{	DataBrowserPropertyID	theValue;
	OSStatus				theErr;



	// Get the value
	theValue = kDataBrowserItemNoProperty;
	theErr   = GetDataBrowserItemDataProperty(itemData, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NDataBrowserSource::SetItemValue : Set an item's value.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemValue(DataBrowserItemDataRef itemData, SInt32 theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataValue(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemMinimum : Set an item's minimum value.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemMinimum(DataBrowserItemDataRef itemData, SInt32 theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataMinimum(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemMaximum : Set an item's maximum value.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemMaximum(DataBrowserItemDataRef itemData, SInt32 theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataMaximum(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemBool : Set an item's boolean state.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemBool(DataBrowserItemDataRef itemData, bool theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataBooleanValue(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemText : Set an item's text.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemText(DataBrowserItemDataRef itemData, const NString &theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataText(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemIcon : Set an item's icon.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemIcon(DataBrowserItemDataRef itemData, const NIcon &theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataIcon(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemDate : Set an item's date.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemDate(DataBrowserItemDataRef itemData, const NCFDate &theValue)
{	LongDateTime	theTime;
	OSStatus		theErr;



	// Get the value
	theTime = NCFDate::GetLongTime(theValue.GetDate());
	theErr  = SetDataBrowserItemDataLongDateTime(itemData, &theTime);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemMenu : Set an item's menu.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemMenu(DataBrowserItemDataRef itemData, MenuRef theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataMenuRef(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemState : Set an item's draw state.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemState(DataBrowserItemDataRef itemData, ThemeDrawState theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataDrawState(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemButton : Set an item's button value.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemButton(DataBrowserItemDataRef itemData, ThemeButtonValue theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataButtonValue(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemTransform : Set an item's icon transform.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemTransform(DataBrowserItemDataRef itemData, IconTransformType theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataIconTransform(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::SetItemID : Set an item's ID.
//----------------------------------------------------------------------------
void NDataBrowserSource::SetItemID(DataBrowserItemDataRef itemData, DataBrowserItemID theValue)
{	OSStatus	theErr;



	// Set the value
	theErr = SetDataBrowserItemDataItemID(itemData, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowserSource::CreateHeader : Create a list header.
//----------------------------------------------------------------------------
DBHeader NDataBrowserSource::CreateHeader(DataBrowserPropertyID		propertyID,
										  DataBrowserPropertyType	propertyType,
										  const NString				&theTitle,
										  UInt32					widthMin,
										  UInt32					widthMax,
										  DataBrowserPropertyFlags	propertyFlags)
{	DBHeader	theHeader;



	// Adjust our parameters
	if (widthMax == 0)
		widthMax = widthMin;



	// Create the header
	memset(&theHeader.titleStyle,   0x00, sizeof(theHeader.titleStyle));
	memset(&theHeader.titleContent, 0x00, sizeof(theHeader.titleContent));

	theHeader.propertyID    = propertyID;
	theHeader.propertyType  = propertyType;
	theHeader.propertyFlags = propertyFlags;
	theHeader.initialOrder  = kDataBrowserOrderDecreasing;

	theHeader.widthMin         = widthMin;
	theHeader.widthMax         = widthMax;
	theHeader.titleOffset      = 0;
	theHeader.titleString      = theTitle;
	theHeader.titleStyle.flags = kControlUseFontMask;
	theHeader.titleStyle.font  = kControlFontViewSystemFont;
	
	return(theHeader);
}





//============================================================================
//		NDataBrowserSource::ResetBrowser : Reset the browser.
//----------------------------------------------------------------------------
void NDataBrowserSource::ResetBrowser(void)
{


	// Reset the browser
	if (mBrowser != NULL)
		mBrowser->Reset();
}




