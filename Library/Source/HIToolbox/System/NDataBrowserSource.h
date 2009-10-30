/*	NAME:
		NDataBrowserSource.h

	DESCRIPTION:
		DataBrowser source.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATABROWSERSOURCE_HDR
#define NDATABROWSERSOURCE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NMsgBroadcaster.h"
#include "NCarbonEvent.h"
#include "NString.h"
#include "NDate.h"
#include "NDrag.h"
#include "NIcon.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgBrowserItemAdded							= 'biad';
static const BroadcastMsg kMsgBrowserItemRemoved						= 'birm';
static const BroadcastMsg kMsgBrowserItemEditBegin						= 'bieb';
static const BroadcastMsg kMsgBrowserItemEditEnd						= 'biee';
static const BroadcastMsg kMsgBrowserItemSelected						= 'bise';
static const BroadcastMsg kMsgBrowserItemDeselected						= 'bide';
static const BroadcastMsg kMsgBrowserItemOpened							= 'biop';

static const BroadcastMsg kMsgBrowserContainerOpened					= 'bcop';
static const BroadcastMsg kMsgBrowserContainerClosing					= 'bclg';
static const BroadcastMsg kMsgBrowserContainerClosed					= 'bcld';
static const BroadcastMsg kMsgBrowserContainerSorting					= 'bcsg';
static const BroadcastMsg kMsgBrowserContainerSorted					= 'bcsd';
static const BroadcastMsg kMsgBrowserContainerToggled					= 'bctg';

static const BroadcastMsg kMsgBrowserSelectionRemove					= 'bsrm';
static const BroadcastMsg kMsgBrowserSelectionAccept					= 'bsac';

static const BroadcastMsg kMsgBrowserChangedTarget						= 'bbtg';
static const BroadcastMsg kMsgBrowserChangedLayout						= 'bbla';
static const BroadcastMsg kMsgBrowserChangedSelection					= 'bbsl';
static const BroadcastMsg kMsgBrowserChangedFocus						= 'bbfo';





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NDataBrowser;


// Header
typedef struct {
	// Column
	DataBrowserPropertyID		propertyID;
	DataBrowserPropertyType		propertyType;
	DataBrowserPropertyFlags	propertyFlags;
	DataBrowserSortOrder		initialOrder;
	
	// Style
	UInt16						widthMin;
	UInt16						widthMax;
	SInt16						titleOffset;
	NString						titleString;
	ControlFontStyleRec			titleStyle;
	ControlButtonContentInfo	titleContent;
} DBHeader;


// Lists
typedef std::vector<DBHeader>									DBHeaderList;
typedef DBHeaderList::iterator									DBHeaderListIterator;
typedef DBHeaderList::const_iterator							DBHeaderListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataBrowserSource : public NMsgBroadcaster {
public:
										NDataBrowserSource(void);
	virtual								~NDataBrowserSource(void);


	// Get/set the browser
	NDataBrowser						*GetBrowser(void) const;
	void								SetBrowser(NDataBrowser *theBrowser);


	// Populate the browser
	virtual void						PopulateBrowser(void);


	// Get the list headers
	virtual DBHeaderList				GetListHeaders(void);
	

	// Get the data for an item
	virtual OSStatus					GetItemData(DataBrowserItemID		itemID,
													DataBrowserItemDataRef	itemData,
													DataBrowserPropertyID	propertyID) = 0;


	// Set the data for an item
	virtual OSStatus					SetItemData(DataBrowserItemID		itemID,
													DataBrowserItemDataRef	itemData,
													DataBrowserPropertyID	propertyID);


	// Get the help for an item
	virtual void						GetItemHelp(DataBrowserItemID		itemID,
													DataBrowserPropertyID	propertyID,
													HMContentRequest		theRequest,
													HMContentProvidedType	&helpType,
													HMHelpContentRec		&helpContent);


	// Compare two items
	virtual CFComparisonResult			CompareItems(DataBrowserItemID		itemOne,
													DataBrowserItemID		itemTwo,
													DataBrowserPropertyID	propertyID);


	// Item notifications
	virtual void						ItemAdded(     DataBrowserItemID itemID);
	virtual void						ItemRemoved(   DataBrowserItemID itemID);
	virtual void						ItemEditBegin( DataBrowserItemID itemID);
	virtual void						ItemEditEnd(   DataBrowserItemID itemID);
	virtual void						ItemSelected(  DataBrowserItemID itemID);
	virtual void						ItemDeselected(DataBrowserItemID itemID);
	virtual void						ItemOpened(    DataBrowserItemID itemID);


	// Container notifications
	virtual void						ContainerOpened( DataBrowserItemID itemID);
	virtual void						ContainerClosing(DataBrowserItemID itemID);
	virtual void						ContainerClosed( DataBrowserItemID itemID);
	virtual void						ContainerSorting(DataBrowserItemID itemID);
	virtual void						ContainerSorted( DataBrowserItemID itemID);
	virtual void						ContainerToggled(DataBrowserItemID itemID);


	// Selection notifications
	virtual void						SelectionRemove(void);
	virtual void						SelectionAccept(void);


	// Browser notifications
	virtual void						BrowserTarget(   void);
	virtual void						BrowserLayout(   void);
	virtual void						BrowserSelection(void);
	virtual void						BrowserFocus(     bool hasFocus);
	virtual OSStatus					BrowserRawKeyDown(NCarbonEvent &theEvent);


	// Handle drags
	virtual bool						CanDrag(    DataBrowserItemID itemID, const NDrag &theDrag);
	virtual bool						AcceptDrag( DataBrowserItemID itemID, const NDrag &theDrag);
	virtual bool						ReceiveDrag(DataBrowserItemID itemID, const NDrag &theDrag);
	virtual void						FinishDrag(OSStatus      trackResult, const NDrag &theDrag);


	// Get the contextual menu for an item
	virtual MenuRef						GetContextualMenu(UInt32 &helpType, AEDesc &theSelection);


	// Handle the contextual menu for an item
	virtual void						HandleContextualMenu(UInt32 selectionType, MenuRef theMenu, MenuItemIndex theIndex);
	

protected:
	// Get/set item state
	SInt32								GetItemValue(     DataBrowserItemDataRef itemData) const;
	SInt32								GetItemMinimum(   DataBrowserItemDataRef itemData) const;
	SInt32								GetItemMaximum(   DataBrowserItemDataRef itemData) const;
	bool								GetItemBool(      DataBrowserItemDataRef itemData) const;
	NString								GetItemText(      DataBrowserItemDataRef itemData) const;
	NIcon								GetItemIcon(      DataBrowserItemDataRef itemData) const;
	NDate								GetItemDate(      DataBrowserItemDataRef itemData) const;
	MenuRef								GetItemMenu(      DataBrowserItemDataRef itemData) const;
	ThemeDrawState						GetItemState(     DataBrowserItemDataRef itemData) const;
	ThemeButtonValue					GetItemButton(    DataBrowserItemDataRef itemData) const;
	IconTransformType					GetItemTransform( DataBrowserItemDataRef itemData) const;
	DataBrowserItemID					GetItemID(        DataBrowserItemDataRef itemData) const;
	DataBrowserPropertyID				GetItemPropertyID(DataBrowserItemDataRef itemData) const;

	void								SetItemValue(    DataBrowserItemDataRef itemData, SInt32            theValue);
	void								SetItemMinimum(  DataBrowserItemDataRef itemData, SInt32            theValue);
	void								SetItemMaximum(  DataBrowserItemDataRef itemData, SInt32            theValue);
	void								SetItemBool(     DataBrowserItemDataRef itemData, bool              theValue);
	void								SetItemText(     DataBrowserItemDataRef itemData, const NString    &theValue);
	void								SetItemIcon(     DataBrowserItemDataRef itemData, const NIcon      &theValue);
	void								SetItemDate(     DataBrowserItemDataRef itemData, const NDate      &theValue);
	void								SetItemMenu(     DataBrowserItemDataRef itemData, MenuRef           theValue);
	void								SetItemState(    DataBrowserItemDataRef itemData, ThemeDrawState    theValue);
	void								SetItemButton(   DataBrowserItemDataRef itemData, ThemeButtonValue  theValue);
	void								SetItemTransform(DataBrowserItemDataRef itemData, IconTransformType theValue);
	void								SetItemID(       DataBrowserItemDataRef itemData, DataBrowserItemID theValue);


	// Create a list header
	DBHeader							CreateHeader(DataBrowserPropertyID		propertyID,
													 DataBrowserPropertyType	propertyType,
													 const NString				&theTitle,
													 UInt32						widthMin,
													 UInt32						widthMax      = 0,
													 DataBrowserPropertyFlags	propertyFlags = kDataBrowserDefaultPropertyFlags);


	// Reset the browser
	void								ResetBrowser(void);


	// Set the text for an item
	OSStatus							SetDataBrowserItemDataText(DataBrowserItemDataRef itemData, const NString &theText, bool stripNewline=true);


private:
	NDataBrowser						*mBrowser;
};





#endif // NDATABROWSERSOURCE_HDR


