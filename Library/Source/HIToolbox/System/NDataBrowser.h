/*	NAME:
		NDataBrowser.h

	DESCRIPTION:
		DataBrowser control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATABROWSER_HDR
#define NDATABROWSER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NHIView.h"
#include "NDictionary.h"
#include "NDataBrowserSource.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
static const float kDataBrowserMetricDefaultValue					= -42.0f;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists
typedef std::vector<DataBrowserItemID>							DBItemList;
typedef DBItemList::iterator									DBItemListIterator;
typedef DBItemList::const_iterator								DBItemListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataBrowser : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NDataBrowser);

										NDataBrowser(HIWindowRef				theWindow,
														const HIRect			&theRect,
														DataBrowserViewStyle	theStyle);

	virtual								~NDataBrowser(void);


	// Reset the browser
	void								Reset(void);
	

	// Get/set the source
	NDataBrowserSource					*GetSource(void) const;
	void								SetSource(NDataBrowserSource *theSource);


	// Get/set the filter
	NString								GetFilter(void) const;
	void								SetFilter(const NString &theFilter);
	

	// Get/set the style
	DataBrowserViewStyle				GetStyle(void) const;
	void								SetStyle(DataBrowserViewStyle theStyle);


	// Get/set the attributes
	OptionBits							GetAttributes(void) const;
	void								SetAttributes(OptionBits setThese, OptionBits clearThese=kDataBrowserAttributeNone);


	// Get/set the selection flags
	DataBrowserSelectionFlags			GetSelectionFlags(void) const;
	void								SetSelectionFlags(DataBrowserSelectionFlags theFlags);


	// Get/set a metric
	//
	// A metric can be set to kDataBrowserMetricDefaultValue to select the default value. 
	float								GetMetric(DataBrowserMetric theMetric) const;
	void								SetMetric(DataBrowserMetric theMetric, float theValue);


	// Get/set the row height
	UInt16								GetRowHeight(void) const;
	void								SetRowHeight(UInt16 theValue);
	

	// Get the state of an item
	DataBrowserItemState				GetItemState(DataBrowserItemID theItem) const;


	// Reveal an item
	void								RevealItem(DataBrowserItemID theItem, DataBrowserPropertyID theProperty, DataBrowserRevealOptions theOptions=kDataBrowserRevealAndCenterInView);


	// Add items
	//
	// When adding numItems items, a DBItemList will be added that contains integer
	// values from 1 to numItems inclusive.
	void								AddItems(const DBItemList &theItems, DataBrowserItemID theContainer=kDataBrowserNoItem);
	void								AddItems(      UInt32      numItems, DataBrowserItemID theContainer=kDataBrowserNoItem);
	void								Additem(DataBrowserItemID  theItem,  DataBrowserItemID theContainer=kDataBrowserNoItem);


	// Remove items
	//
	// A NULL or empty list can be supplied to remove all items.
	void								RemoveItems(const DBItemList *theItems=NULL, DataBrowserItemID theContainer=kDataBrowserNoItem);
	void								RemoveItem(DataBrowserItemID  theItem,       DataBrowserItemID theContainer=kDataBrowserNoItem);


	// Update items
	//
	// A NULL or empty list can be supplied to update all items.
	void								UpdateItems(const DBItemList *theItems=NULL, DataBrowserItemID theContainer=kDataBrowserNoItem);
	void								Updateitem(DataBrowserItemID  theItem,       DataBrowserItemID theContainer=kDataBrowserNoItem);


	// Open items
	//
	// A NULL or empty list can be supplied to open all items.
	void								OpenItems(const DBItemList *theItems=NULL, DataBrowserItemID theContainer=kDataBrowserNoItem);
	void								OpenItem(DataBrowserItemID  theItem,       DataBrowserItemID theContainer=kDataBrowserNoItem);


	// Open/close a container
	void								OpenContainer( DataBrowserItemID theContainer);
	void								CloseContainer(DataBrowserItemID theContainer);


	// Sort a container
	void								SortContainer(DataBrowserItemID theContainer, bool sortChildren);


	// Get the number of items in a given state
	UInt32								GetItemCount(DataBrowserItemState theState, bool canRecurse=true, DataBrowserItemID theContainer=kDataBrowserNoItem) const;


	// Get the items in a given state
	DBItemList							GetItems(DataBrowserItemState theState, bool canRecurse=true, DataBrowserItemID theContainer=kDataBrowserNoItem) const;


	// Is an item selected?
	bool								IsItemSelected(DataBrowserItemID theItem) const;


	// Are any items selected?
	bool								AreItemsSelected(bool canRecurse=true, DataBrowserItemID theContainer=kDataBrowserNoItem) const;


	// Set the selection
	//
	// A NULL or empty list can be supplied to clear the current selection.
	void								SetSelection(const DBItemList *theItems, DataBrowserSetOption selectOp=kDataBrowserItemsAssign);


	// Get/set the scroll bar inset
	void								GetScrollBarInset(      HIPoint &insetHoriz,       HIPoint &insetVert) const;
	void								SetScrollBarInset(const HIPoint &insetHoriz, const HIPoint &insetVert);


	// Get/set the target item
	DataBrowserItemID					GetTargetItem(void) const;
	void								SetTargetItem(DataBrowserItemID theItem);


	// Get/set the sort order
	DataBrowserSortOrder				GetSortOrder(void) const;
	void								SetSortOrder(DataBrowserSortOrder theOrder);


	// Get/set the sort property
	DataBrowserPropertyID				GetSortProperty(void) const;
	void								SetSortProperty(DataBrowserPropertyID theProperty);


	// Get/set the origin
	HIPoint								GetOrigin(void) const;
	void								SetOrigin(const HIPoint &theOrigin);


	// Get/set the scroll bar state
	void								GetScrollBars(bool &hasHoriz, bool &hasVert) const;
	void								SetScrollBars(bool  hasHoriz, bool  hasVert);
	
	
	// Get/set the flags for a property
	DataBrowserPropertyFlags			GetPropertyFlags(DataBrowserPropertyID theProperty) const;
	void								SetPropertyFlags(DataBrowserPropertyID theProperty, DataBrowserPropertyFlags theFlags);


	// Get/set the edit text
	NString								GetEditText(void) const;
	void								SetEditText(const NString &theText);
	
	
	// Get/set the edited item
	void								GetEditItem(DataBrowserItemID &theItem, DataBrowserPropertyID &theProperty) const;
	void								SetEditItem(DataBrowserItemID  theItem, DataBrowserPropertyID  theProperty);


	// Get/set the table view hilite style
	DataBrowserTableViewHiliteStyle		GetTableHiliteStyle(void) const;
	void								SetTableHiliteStyle(DataBrowserTableViewHiliteStyle theStyle);


	// Encode/decode the properties
	OSStatus							EncodeProperties(      NDictionary &theProperties);
	OSStatus							DecodeProperties(const NDictionary &theProperties);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventRawKeyDown(NCarbonEvent &theEvent);


private:
	void								ApplyListHeaders(void);
	DataBrowserListViewColumnDesc		GetListHeaderDesc(const DBHeader &theHeader);

	void								InstallCallbacks(void);
	void								RemoveCallbacks( void);

	static NDataBrowserSource			*GetSource(ControlRef theControl, bool allowNULL=false);

	static OSStatus						DBItemData(            ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, DataBrowserItemDataRef itemData, Boolean setValue);
	static Boolean						DBItemCompare(         ControlRef browser, DataBrowserItemID itemOne, DataBrowserItemID itemTwo, DataBrowserPropertyID sortProperty);
	static void							DBItemNotification(    ControlRef browser, DataBrowserItemID item, DataBrowserItemNotification message, DataBrowserItemDataRef itemData);
	static Boolean						DBAddDragItem(         ControlRef browser, DragReference theDrag, DataBrowserItemID item, ItemReference *itemRef);
	static Boolean						DBAcceptDrag(          ControlRef browser, DragReference theDrag, DataBrowserItemID item);
	static Boolean						DBReceiveDrag(         ControlRef browser, DragReference theDrag, DataBrowserItemID item);
	static void							DBPostProcessDrag(     ControlRef browser, DragReference theDrag, OSStatus trackDragResult);
	static void							DBItemHelpContent(     ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentPtr ioHelpContent);
	static void							DBGetContextualMenu(   ControlRef browser, MenuRef *menu, UInt32 *helpType, CFStringRef *helpItemString, AEDesc *selection);
	static void							DBSelectContextualMenu(ControlRef browser, MenuRef menu, UInt32 selectionType, SInt16 menuID, MenuItemIndex menuItem);


private:
	NDataBrowserSource					*mSource;
	NString								mFilter;
};





#endif // NDATABROWSER_HDR


