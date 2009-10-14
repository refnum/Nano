/*	NAME:
		NDataBrowser.cpp

	DESCRIPTION:
		DataBrowser control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NMathUtilities.h"
#include "NDataBrowser.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Properties
//
// These values may be archived to disk, and can never change.
static const NString kDBUserStateKey								= "DBUserState";


// Tags
static const OSType kTagDrawColumnDividers							= 'cdiv';
static const OSType kTagDrawRowStripes								= 'rstr';
static const OSType kTagRowHeight									= 'rhgt';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NDataBrowser, kHIDataBrowserClassID, NULL);





//============================================================================
//		NDataBrowser::NDataBrowser : Constructor.
//----------------------------------------------------------------------------
NDataBrowser::NDataBrowser(HIWindowRef				theWindow,
							const HIRect			&theRect,
							DataBrowserViewStyle	theStyle)
{	HIViewRef		theView;
	OSStatus		theErr;



	// Create the view
	theErr = CreateDataBrowserControl(NULL, &kQDRectZero, theStyle, &theView);
	NN_ASSERT_NOERR(theErr);

	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NDataBrowser::~NDataBrowser : Destructor.
//----------------------------------------------------------------------------
NDataBrowser::~NDataBrowser(void)
{


	// Clean up
	if (GetHIObject() != NULL)
		RemoveCallbacks();
}





//============================================================================
//		NDataBrowser::Reset : Reset the browser.
//----------------------------------------------------------------------------
void NDataBrowser::Reset(void)
{


	// Reset the browser
	//
	// By re-applying the current source we will reset the browser contents.
	SetSource(mSource);
}





//============================================================================
//		NDataBrowser::GetSource : Get the source.
//----------------------------------------------------------------------------
NDataBrowserSource *NDataBrowser::GetSource(void) const
{


	// Get the source
	return(mSource);
}





//============================================================================
//		NDataBrowser::SetSource : Set the source.
//----------------------------------------------------------------------------
void NDataBrowser::SetSource(NDataBrowserSource *theSource)
{


	// Detach the existing source
	if (mSource != NULL)
		mSource->SetBrowser(NULL);



	// Set the source
	mSource = theSource;
	
	if (mSource != NULL)
		mSource->SetBrowser(this);
	
	
	
	// Update the browser
	RemoveItems();
	
	if (GetStyle() == kDataBrowserListView)
		ApplyListHeaders();
	
	if (mSource != NULL)
		mSource->PopulateBrowser();
}





//============================================================================
//		NDataBrowser::GetFilter : Get the filter.
//----------------------------------------------------------------------------
NString NDataBrowser::GetFilter(void) const
{


	// Get the filter
	return(mFilter);
}





//============================================================================
//		NDataBrowser::SetFilter : Set the filter.
//----------------------------------------------------------------------------
void NDataBrowser::SetFilter(const NString &theFilter)
{


	// Set the filter
	mFilter = theFilter;

	Reset();
}





//============================================================================
//		NDataBrowser::GetStyle : Get the style.
//----------------------------------------------------------------------------
DataBrowserViewStyle NDataBrowser::GetStyle(void) const
{	DataBrowserViewStyle	theStyle;
	OSStatus				theErr;



	// Get the style
	theStyle = kDataBrowserNoView;
	theErr   = GetDataBrowserViewStyle(*this, &theStyle);
	NN_ASSERT_NOERR(theErr);
	
	return(theStyle);
}





//============================================================================
//		NDataBrowser::SetStyle : Set the style.
//----------------------------------------------------------------------------
void NDataBrowser::SetStyle(DataBrowserViewStyle theStyle)
{	OSStatus	theErr;



	// Set the style
	theErr = SetDataBrowserViewStyle(*this, theStyle);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
OptionBits NDataBrowser::GetAttributes(void) const
{	OptionBits		theAttributes;
	OSStatus		theErr;



	// Get the attributes
	//
	// Support for 10.3 is temporary.
	theAttributes = kDataBrowserAttributeNone;
	
	if (DataBrowserGetAttributes != NULL)
		{
		theErr = DataBrowserGetAttributes(*this, &theAttributes);
		NN_ASSERT_NOERR(theErr);
		}
		
	return(theAttributes);
}





//============================================================================
//		NDataBrowser::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NDataBrowser::SetAttributes(OptionBits setThese, OptionBits clearThese)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(NSystemUtilities::SystemIsAtLeast(kSystemTenFour));



	// Set the attributes
	//
	// Support for 10.3 is temporary.
	if (DataBrowserChangeAttributes != NULL)
		{
		theErr = DataBrowserChangeAttributes(*this, setThese, clearThese);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NDataBrowser::GetSelectionFlags : Get the selection flags.
//----------------------------------------------------------------------------
DataBrowserSelectionFlags NDataBrowser::GetSelectionFlags(void) const
{	DataBrowserSelectionFlags	theFlags;
	OSStatus					theErr;



	// Get the selection flags
	theFlags = kDataBrowserSelectOnlyOne;
	theErr   = GetDataBrowserSelectionFlags(*this, &theFlags);
	NN_ASSERT_NOERR(theErr);
	
	return(theFlags);
}





//============================================================================
//		NDataBrowser::SetSelectionFlags : Set the selection flags.
//----------------------------------------------------------------------------
void NDataBrowser::SetSelectionFlags(DataBrowserSelectionFlags theFlags)
{	OSStatus	theErr;



	// Set the flags
	theErr = SetDataBrowserSelectionFlags(*this, theFlags);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetMetric : Get a metric.
//----------------------------------------------------------------------------
float NDataBrowser::GetMetric(DataBrowserMetric theMetric) const
{	float		theValue;
	OSStatus	theErr;



	// Get the metric
	//
	// Support for 10.3 is temporary.
	theValue = kDataBrowserMetricDefaultValue;
	
	if (DataBrowserGetMetric != NULL)
		{
		theErr = DataBrowserGetMetric(*this, theMetric, NULL, &theValue);
		NN_ASSERT_NOERR(theErr);
		}
		
	return(theValue);
}





//============================================================================
//		NDataBrowser::SetMetric : Set a metric.
//----------------------------------------------------------------------------
void NDataBrowser::SetMetric(DataBrowserMetric theMetric, float theValue)
{	bool		isDefault;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(NSystemUtilities::SystemIsAtLeast(kSystemTenFour));



	// Set the metric
	//
	// Support for 10.3 is temporary.
	if (DataBrowserSetMetric != NULL)
		{
		isDefault = NMathUtilities::AreEqual(theValue, kDataBrowserMetricDefaultValue); 
		theErr    = DataBrowserSetMetric(*this, theMetric, isDefault, theValue);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NDataBrowser::GetRowHeight : Get the row height.
//----------------------------------------------------------------------------
UInt16 NDataBrowser::GetRowHeight(void) const
{	UInt16		theValue;
	OSStatus	theErr;



	// Get the row height
	theValue = 0;
	theErr   = GetDataBrowserTableViewRowHeight(*this, &theValue);
	NN_ASSERT_NOERR(theErr);

	return(theValue);
}





//============================================================================
//		NDataBrowser::SetRowHeight : Set the row height.
//----------------------------------------------------------------------------
void NDataBrowser::SetRowHeight(UInt16 theValue)
{	OSStatus	theErr;



	// Set the row height
	theErr = SetDataBrowserTableViewRowHeight(*this, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetItemState : Get the state for an item.
//----------------------------------------------------------------------------
DataBrowserItemState NDataBrowser::GetItemState(DataBrowserItemID theItem) const
{	DataBrowserItemState	theState;
	OSStatus				theErr;



	// Get the item count
	theState = kDataBrowserItemNoState;
	theErr   = GetDataBrowserItemState(*this, theItem, &theState);
	NN_ASSERT_NOERR(theErr);
	
	return(theState);
}





//============================================================================
//		NDataBrowser::RevealItem : Reveal an item.
//----------------------------------------------------------------------------
void NDataBrowser::RevealItem(DataBrowserItemID theItem, DataBrowserPropertyID theProperty, DataBrowserRevealOptions theOptions)
{	OSStatus	theErr;



	// Reveal the item
	theErr = RevealDataBrowserItem(*this, theItem, theProperty, theOptions);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::AddItems : Add items.
//----------------------------------------------------------------------------
void NDataBrowser::AddItems(const DBItemList &theItems, DataBrowserItemID theContainer)
{	const DataBrowserItemID		*itemPtr;
	UInt32						numItems;
	OSStatus					theErr;



	// Get the state we need
	numItems = theItems.size();
	itemPtr  = theItems.empty() ? NULL : &theItems[0];



	// Add the items
	theErr = AddDataBrowserItems(*this, theContainer, numItems, itemPtr, kDataBrowserItemNoProperty);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::AddItems : Add items.
//----------------------------------------------------------------------------
void NDataBrowser::AddItems(UInt32 numItems, DataBrowserItemID theContainer)
{	DBItemList		theItems;
	UInt32			n;



	// Add the items
	for (n = 1; n <= numItems; n++)
		theItems.push_back((DataBrowserItemID) n);
	
	AddItems(theItems, theContainer);
}





//============================================================================
//		NDataBrowser::AddItem : Add an item.
//----------------------------------------------------------------------------
void NDataBrowser::AddItem(DataBrowserItemID theItem, DataBrowserItemID theContainer)
{	DBItemList	theList;



	// Add the item
	theList.push_back(theItem);
	
	AddItems(theList, theContainer);
}





//============================================================================
//		NDataBrowser::RemoveItems : Remove items.
//----------------------------------------------------------------------------
void NDataBrowser::RemoveItems(const DBItemList *theItems, DataBrowserItemID theContainer)
{	OSStatus	theErr;



	// Remove all items
	if (theItems == NULL || theItems->empty())
		{
		theErr = RemoveDataBrowserItems(*this, theContainer, 0, NULL, kDataBrowserItemNoProperty);
		NN_ASSERT_NOERR(theErr);
		}


	// Remove the specified items
	else
		{
		theErr = RemoveDataBrowserItems(*this, theContainer, theItems->size(), &theItems->at(0), kDataBrowserItemNoProperty);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NDataBrowser::RemoveItem : Remove an item.
//----------------------------------------------------------------------------
void NDataBrowser::RemoveItem(DataBrowserItemID theItem, DataBrowserItemID theContainer)
{	DBItemList	theList;



	// Remove the item
	theList.push_back(theItem);
	
	RemoveItems(&theList, theContainer);
}





//============================================================================
//		NDataBrowser::UpdateItems : Update items.
//----------------------------------------------------------------------------
void NDataBrowser::UpdateItems(const DBItemList *theItems, DataBrowserItemID theContainer)
{	OSStatus	theErr;



	// Update all items
	if (theItems == NULL || theItems->empty())
		{
		theErr = UpdateDataBrowserItems(*this, theContainer, 0, NULL, kDataBrowserItemNoProperty, kDataBrowserItemNoProperty);
		NN_ASSERT_NOERR(theErr);
		}


	// Update the specified items
	else
		{
		theErr = UpdateDataBrowserItems(*this, theContainer, theItems->size(), &theItems->at(0), kDataBrowserItemNoProperty, kDataBrowserItemNoProperty);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NDataBrowser::UpdateItem : Update an item.
//----------------------------------------------------------------------------
void NDataBrowser::Updateitem(DataBrowserItemID theItem, DataBrowserItemID theContainer)
{	DBItemList	theList;



	// Update the item
	theList.push_back(theItem);
	
	UpdateItems(&theList, theContainer);
}





//============================================================================
//		NDataBrowser::OpenItems : Open items.
//----------------------------------------------------------------------------
void NDataBrowser::OpenItems(const DBItemList *theItems, DataBrowserItemID theContainer)
{	DBItemList					tmpItems;
	DBItemListConstIterator		theIter;



	// Check our state
	if (mSource == NULL)
		return;



	// Open all items if necessary
	if (theItems == NULL || theItems->empty())
		{
		tmpItems = GetItems(kDataBrowserItemAnyState, true, theContainer);
		theItems = &tmpItems;
		}



	// Open the items
	for (theIter = theItems->begin(); theIter != theItems->end(); theIter++)
		mSource->ItemOpened(*theIter);
}





//============================================================================
//		NDataBrowser::OpenItem : Open an item.
//----------------------------------------------------------------------------
void NDataBrowser::OpenItem(DataBrowserItemID theItem, DataBrowserItemID theContainer)
{	DBItemList	theList;



	// Open the item
	theList.push_back(theItem);
	
	OpenItems(&theList, theContainer);
}





//============================================================================
//		NDataBrowser::OpenContainer : Open a container.
//----------------------------------------------------------------------------
void NDataBrowser::OpenContainer(DataBrowserItemID theContainer)
{	OSStatus	theErr;



	// Open the container
	theErr = OpenDataBrowserContainer(*this, theContainer);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::CloseContainer : Close a container.
//----------------------------------------------------------------------------
void NDataBrowser::CloseContainer(DataBrowserItemID theContainer)
{	OSStatus	theErr;



	// Close the container
	theErr = CloseDataBrowserContainer(*this, theContainer);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::SortContainer : Sort a container.
//----------------------------------------------------------------------------
void NDataBrowser::SortContainer(DataBrowserItemID theContainer, bool sortChildren)
{	OSStatus	theErr;



	// Sort the container
	theErr = SortDataBrowserContainer(*this, theContainer, sortChildren);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetItemCount : Get the number of items in a given state.
//----------------------------------------------------------------------------
UInt32 NDataBrowser::GetItemCount(DataBrowserItemState theState, bool canRecurse, DataBrowserItemID theContainer) const
{	UInt32		numItems;
	OSStatus	theErr;



	// Get the item count
	numItems = 0;
	theErr   = GetDataBrowserItemCount(*this, theContainer, canRecurse, theState, &numItems);
	NN_ASSERT_NOERR(theErr);
	
	return(numItems);
}





//============================================================================
//		NDataBrowser::GetItems : Get the items in a given state.
//----------------------------------------------------------------------------
DBItemList NDataBrowser::GetItems(DataBrowserItemState theState, bool canRecurse, DataBrowserItemID theContainer) const
{	UInt32				n, numItems;
	DataBrowserItemID	*itemPtr;
	DBItemList			theItems;
	Handle				theHnd;
	OSStatus			theErr;



	// Allocate the handle
	theHnd = NewHandle(0);
	if (theHnd == NULL)
		return(theItems);



	// Get the items
	theErr = GetDataBrowserItems(*this, theContainer, canRecurse, theState, theHnd);
	NN_ASSERT_NOERR(theErr);



	// Build the list
	numItems = GetHandleSize(theHnd) / sizeof(DataBrowserItemID);
	itemPtr  = (DataBrowserItemID *) *theHnd;
	
	for (n = 0; n < numItems; n++)
		theItems.push_back(itemPtr[n]);



	// Clean up
	DisposeHandle(theHnd);
	
	return(theItems);
}





//============================================================================
//		NDataBrowser::IsItemSelected : Is an item selected?
//----------------------------------------------------------------------------
bool NDataBrowser::IsItemSelected(DataBrowserItemID theItem) const
{


	// Get the item state
	return(IsDataBrowserItemSelected(*this, theItem));
}





//============================================================================
//		NDataBrowser::AreItemsSelected : Are any items selected?
//----------------------------------------------------------------------------
bool NDataBrowser::AreItemsSelected(bool canRecurse, DataBrowserItemID theContainer) const
{


	// Check the item state
	return(GetItemCount(kDataBrowserItemIsSelected, canRecurse, theContainer) != 0);
}





//============================================================================
//		NDataBrowser::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NDataBrowser::SetSelection(const DBItemList *theItems, DataBrowserSetOption selectOp)
{	OSStatus		theErr;



	// Clear the selection
	if (theItems == NULL || theItems->empty())
		{
		theErr = SetDataBrowserSelectedItems(*this, 0, NULL, kDataBrowserItemsRemove);
		NN_ASSERT_NOERR(theErr);
		}
	
	
	// Set the selection
	else
		{
		theErr = SetDataBrowserSelectedItems(*this, theItems->size(), &theItems->at(0), selectOp);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NDataBrowser::GetScrollBarInset : Get the scroll bar inset.
//----------------------------------------------------------------------------
void NDataBrowser::GetScrollBarInset(HIPoint &insetHoriz, HIPoint &insetVert) const
{	QDRect		qdRect;
	OSStatus	theErr;



	// Get the scroll bar inset
	qdRect = kQDRectZero;
	theErr = GetDataBrowserScrollBarInset(*this, &qdRect);
	NN_ASSERT_NOERR(theErr);



	// Convert the inset
	insetHoriz.x = (float) qdRect.left;
	insetHoriz.y = (float) qdRect.right;

	insetVert.x = (float) qdRect.top;
	insetVert.y = (float) qdRect.bottom;
}





//============================================================================
//		NDataBrowser::SetScrollBarInset : Set the scroll bar inset.
//----------------------------------------------------------------------------
void NDataBrowser::SetScrollBarInset(const HIPoint &insetHoriz, const HIPoint &insetVert)
{	QDRect		qdRect;
	OSStatus	theErr;



	// Convert the inset
	qdRect.left  = (SInt16) insetHoriz.x;
	qdRect.right = (SInt16) insetHoriz.y;

	qdRect.top    = (SInt16) insetVert.x;
	qdRect.bottom = (SInt16) insetVert.y;



	// Set the scroll bar inset
	theErr = SetDataBrowserScrollBarInset(*this, &qdRect);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetTargetItem : Get the target item.
//----------------------------------------------------------------------------
DataBrowserItemID NDataBrowser::GetTargetItem(void) const
{	DataBrowserItemID	theItem;
	OSStatus			theErr;



	// Get the target item
	theItem = kDataBrowserNoItem;
	theErr  = GetDataBrowserTarget(*this, &theItem);
	NN_ASSERT_NOERR(theErr);

	return(theItem);
}





//============================================================================
//		NDataBrowser::SetTargetItem : Set the target item.
//----------------------------------------------------------------------------
void NDataBrowser::SetTargetItem(DataBrowserItemID theItem)
{	OSStatus	theErr;



	// Set the target item
	theErr = SetDataBrowserTarget(*this, theItem);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetSortOrder : Get the sort order.
//----------------------------------------------------------------------------
DataBrowserSortOrder NDataBrowser::GetSortOrder(void) const
{	DataBrowserSortOrder	theOrder;
	OSStatus				theErr;



	// Get the sort order
	theOrder = kDataBrowserOrderUndefined;
	theErr   = GetDataBrowserSortOrder(*this, &theOrder);
	NN_ASSERT_NOERR(theErr);

	return(theOrder);
}





//============================================================================
//		NDataBrowser::SetSortOrder : Set the sort order.
//----------------------------------------------------------------------------
void NDataBrowser::SetSortOrder(DataBrowserSortOrder theOrder)
{	OSStatus	theErr;



	// Set the sort order
	theErr = SetDataBrowserSortOrder(*this, theOrder);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetSortProperty : Get the sort property.
//----------------------------------------------------------------------------
DataBrowserPropertyID NDataBrowser::GetSortProperty(void) const
{	DataBrowserPropertyID	theProperty;
	OSStatus				theErr;



	// Get the sort property
	theProperty = kDataBrowserItemNoProperty;
	theErr      = GetDataBrowserSortProperty(*this, &theProperty);
	NN_ASSERT_NOERR(theErr);

	return(theProperty);
}





//============================================================================
//		NDataBrowser::SetSortProperty : Set the sort property.
//----------------------------------------------------------------------------
void NDataBrowser::SetSortProperty(DataBrowserPropertyID theProperty)
{	OSStatus	theErr;



	// Set the sort property
	theErr = SetDataBrowserSortProperty(*this, theProperty);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetOrigin : Get the origin.
//----------------------------------------------------------------------------
HIPoint NDataBrowser::GetOrigin(void) const
{	UInt32		offsetTop, offsetLeft;
	HIPoint		theOrigin;
	OSStatus	theErr;



	// Get the sort property
	theOrigin = kHIPointZero;
	theErr     = GetDataBrowserScrollPosition(*this, &offsetTop, &offsetLeft);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		{
		theOrigin.x = (float) offsetLeft;
		theOrigin.y = (float) offsetTop;
		}
		
	return(theOrigin);
}





//============================================================================
//		NDataBrowser::SetOrigin : Set the origin.
//----------------------------------------------------------------------------
void NDataBrowser::SetOrigin(const HIPoint &theOrigin)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theOrigin.x >= 0.0f && theOrigin.y >= 0.0f);



	// Set the origin
	theErr = SetDataBrowserScrollPosition(*this, (UInt32) theOrigin.y, (UInt32) theOrigin.x);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetScrollBars : Get the scroll bar state.
//----------------------------------------------------------------------------
void NDataBrowser::GetScrollBars(bool &hasHoriz, bool &hasVert) const
{	Boolean		valueH, valueV;
	OSStatus	theErr;



	// Get the scroll bar state
	hasHoriz = hasVert = false;
	theErr   = GetDataBrowserHasScrollBars(*this, &valueH, &valueV);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		{
		hasHoriz = valueH;
		hasVert  = valueV;
		}
}





//============================================================================
//		NDataBrowser::SetScrollBars : Set the scroll bar state.
//----------------------------------------------------------------------------
void NDataBrowser::SetScrollBars(bool hasHoriz, bool hasVert)
{	OSStatus	theErr;



	// Set the scroll bar state
	theErr = SetDataBrowserHasScrollBars(*this, hasHoriz, hasVert);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetPropertyFlags : Get the flags for a property.
//----------------------------------------------------------------------------
DataBrowserPropertyFlags NDataBrowser::GetPropertyFlags(DataBrowserPropertyID theProperty) const
{	DataBrowserPropertyFlags	theFlags;
	OSStatus					theErr;



	// Get the flags
	theFlags = kDataBrowserDefaultPropertyFlags;
	theErr   = GetDataBrowserPropertyFlags(*this, theProperty, &theFlags);
	NN_ASSERT_NOERR(theErr);
	
	return(theFlags);
}





//============================================================================
//		NDataBrowser::SetPropertyFlags : Set the flags for a property.
//----------------------------------------------------------------------------
void NDataBrowser::SetPropertyFlags(DataBrowserPropertyID theProperty, DataBrowserPropertyFlags theFlags)
{	OSStatus	theErr;



	// Set the flags
	theErr = SetDataBrowserPropertyFlags(*this, theProperty, theFlags);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetEditText : Get the edit text.
//----------------------------------------------------------------------------
NString NDataBrowser::GetEditText(void) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the edit text
	theErr = CopyDataBrowserEditText(*this, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NDataBrowser::SetEditText : Set the edit text.
//----------------------------------------------------------------------------
void NDataBrowser::SetEditText(const NString &theText)
{	OSStatus	theErr;



	// Set the edit text
	theErr = SetDataBrowserEditText(*this, theText);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetEditItem : Get the edit item.
//----------------------------------------------------------------------------
void NDataBrowser::GetEditItem(DataBrowserItemID &theItem, DataBrowserPropertyID &theProperty) const
{	OSStatus	theErr;



	// Get the edit item
	theItem     = kDataBrowserNoItem;
	theProperty = kDataBrowserItemNoProperty;
	
	theErr = GetDataBrowserEditItem(*this, &theItem, &theProperty);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::SetEditItem : Set the edit item.
//----------------------------------------------------------------------------
void NDataBrowser::SetEditItem(DataBrowserItemID theItem, DataBrowserPropertyID theProperty)
{	OSStatus	theErr;



	// Set the edit item
	theErr = SetDataBrowserEditItem(*this, theItem, theProperty);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetTableHiliteStyle : Get the table hilite style.
//----------------------------------------------------------------------------
DataBrowserTableViewHiliteStyle NDataBrowser::GetTableHiliteStyle(void) const
{	DataBrowserTableViewHiliteStyle		theStyle;
	OSStatus							theErr;



	// Get the style
	theStyle = kDataBrowserTableViewMinimalHilite;
	theErr   = GetDataBrowserTableViewHiliteStyle(*this, &theStyle);
	NN_ASSERT_NOERR(theErr);
	
	return(theStyle);
}





//============================================================================
//		NDataBrowser::SetTableHiliteStyle : Set the table hilite style.
//----------------------------------------------------------------------------
void NDataBrowser::SetTableHiliteStyle(DataBrowserTableViewHiliteStyle theStyle)
{	OSStatus	theErr;



	// Set the style
	theErr = SetDataBrowserTableViewHiliteStyle(*this, theStyle);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::EncodeProperties : Encode the properties.
//----------------------------------------------------------------------------
OSStatus NDataBrowser::EncodeProperties(NDictionary &theProperties)
{	CFDictionaryRef		cfDictionary;
	OSStatus			theErr;



	// Encode the base class
	theErr = NHIView::EncodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Get the user state
	theErr = GetDataBrowserUserState(*this, &cfDictionary);
	NN_ASSERT_NOERR(theErr);



	// Encode the properties
	if (theErr == noErr)
		{
		theProperties.SetValueDictionary(kDBUserStateKey, cfDictionary);
		CFSafeRelease(cfDictionary);
		}
	
	return(theErr);
}





//============================================================================
//		NDataBrowser::DecodeProperties : Decode the properties.
//----------------------------------------------------------------------------
OSStatus NDataBrowser::DecodeProperties(const NDictionary &theProperties)
{	NDictionary		theDictionary;
	OSStatus		theErr;



	// Decode the base class
	theErr = NHIView::DecodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Decode the properties
	theDictionary = theProperties.GetValueDictionary(kDBUserStateKey);



	// Set the user state
	theErr = SetDataBrowserUserState(*this, theDictionary);
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		NDataBrowser::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NDataBrowser::InitializeView(void)
{	EventTypeSpec	theEvents[] = { { kEventClassKeyboard,	kEventRawKeyDown },
									{ kEventClassNone,		kEventKindNone	 } };



	// Initialize ourselves
	mSource = NULL;



	// Apply our properties
	//
	// Although IB exposes the row height for the browser, using that to
	// adjust the row height has no effect (rdar://4234674) and so we let
	// the row height be set with a property.
	if (GetPropertyBoolean(kPropertyNano, kTagDrawColumnDividers))
		SetAttributes(kDataBrowserAttributeListViewDrawColumnDividers);

	if (GetPropertyBoolean(kPropertyNano, kTagDrawRowStripes))
		SetAttributes(kDataBrowserAttributeListViewAlternatingRowColors);

	if (HasProperty(                   kPropertyNano, kTagRowHeight))
		SetRowHeight(GetPropertyUInt16(kPropertyNano, kTagRowHeight));



	// Register for events
	AddEvents(theEvents);
	
	InstallCallbacks();
}





//============================================================================
//		NDataBrowser::DoEventRawKeyDown : Handle kEventRawKeyDown.
//----------------------------------------------------------------------------
OSStatus NDataBrowser::DoEventRawKeyDown(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Forward the key stroke
	if (IsActive())
		theErr = mSource->BrowserRawKeyDown(theEvent);
	else
		theErr = eventNotHandledErr;
	
	return(theErr);
}





//============================================================================
//		NDataBrowser::ApplyListHeaders : Apply the list headers.
//----------------------------------------------------------------------------
#pragma mark -
void NDataBrowser::ApplyListHeaders(void)
{	DBHeaderList						theHeaders;
	DataBrowserTableViewHiliteStyle		theStyle;
	DataBrowserListViewColumnDesc		theDesc;
	DBHeaderListConstIterator			theIter;
	OSStatus							theErr;



	// Check we need headers
	//
	// Only the list view has headers.
	if (GetStyle() != kDataBrowserListView || mSource == NULL)
		return;



	// Get the headers
	theHeaders = mSource->GetListHeaders();
	if (theHeaders.empty())
		return;



	// Prepare to set the headers
	//
	// To switch headers, we need to switch out of/back to list view; in doing
	// so, some list view state is lost which we then need to re-apply.
	RemoveItems();

	theStyle = GetTableHiliteStyle();
	
	SetStyle(kDataBrowserColumnView);
	SetStyle(kDataBrowserListView);



	// Set the headers
	for (theIter = theHeaders.begin(); theIter != theHeaders.end(); theIter++)
		{
		theDesc = GetListHeaderDesc(*theIter);
		theErr  = AddDataBrowserListViewColumn(*this, &theDesc, kDataBrowserListViewAppendColumn);
		NN_ASSERT_NOERR(theErr);
		}



	// Restore the state
	SetTableHiliteStyle(theStyle);
}





//============================================================================
//		NDataBrowser::GetListHeaderDesc : Get a list header descriptor.
//----------------------------------------------------------------------------
DataBrowserListViewColumnDesc NDataBrowser::GetListHeaderDesc(const DBHeader &theHeader)
{	DataBrowserListViewColumnDesc	theDesc;



	// Get the header descriptor
	theDesc.propertyDesc.propertyID      = theHeader.propertyID;
	theDesc.propertyDesc.propertyType    = theHeader.propertyType;
	theDesc.propertyDesc.propertyFlags   = theHeader.propertyFlags;
	theDesc.headerBtnDesc.version        = kDataBrowserListViewLatestHeaderDesc;
	theDesc.headerBtnDesc.minimumWidth   = theHeader.widthMin;
	theDesc.headerBtnDesc.maximumWidth   = theHeader.widthMax;
	theDesc.headerBtnDesc.titleOffset    = theHeader.titleOffset;
	theDesc.headerBtnDesc.titleString    = theHeader.titleString;
	theDesc.headerBtnDesc.initialOrder   = theHeader.initialOrder;
	theDesc.headerBtnDesc.btnFontStyle   = theHeader.titleStyle;
	theDesc.headerBtnDesc.btnContentInfo = theHeader.titleContent;
	
	return(theDesc);
}





//============================================================================
//		NDataBrowser::InstallCallbacks : Install our callbacks.
//----------------------------------------------------------------------------
void NDataBrowser::InstallCallbacks(void)
{	static DataBrowserCallbacks		sCallbacks = { kDataBrowserLatestCallbacks, 0 };
	static bool						sCallbacksValid;

	OSStatus	theErr;



	// Initialize the callbacks
	if (!sCallbacksValid)
		{
		theErr = InitDataBrowserCallbacks(&sCallbacks);
		NN_ASSERT_NOERR(theErr);

		sCallbacks.u.v1.itemDataCallback			 = NewDataBrowserItemDataUPP(            DBItemData);
		sCallbacks.u.v1.itemCompareCallback			 = NewDataBrowserItemCompareUPP(         DBItemCompare);
		sCallbacks.u.v1.itemNotificationCallback	 = (DataBrowserItemNotificationUPP)      NewDataBrowserItemNotificationWithItemUPP(DBItemNotification);
		sCallbacks.u.v1.addDragItemCallback			 = NewDataBrowserAddDragItemUPP(         DBAddDragItem);
		sCallbacks.u.v1.acceptDragCallback			 = NewDataBrowserAcceptDragUPP(          DBAcceptDrag);
		sCallbacks.u.v1.receiveDragCallback			 = NewDataBrowserReceiveDragUPP(         DBReceiveDrag);
		sCallbacks.u.v1.postProcessDragCallback		 = NewDataBrowserPostProcessDragUPP(     DBPostProcessDrag);
		sCallbacks.u.v1.itemHelpContentCallback		 = NewDataBrowserItemHelpContentUPP(     DBItemHelpContent);
		sCallbacks.u.v1.getContextualMenuCallback	 = NewDataBrowserGetContextualMenuUPP(   DBGetContextualMenu);
		sCallbacks.u.v1.selectContextualMenuCallback = NewDataBrowserSelectContextualMenuUPP(DBSelectContextualMenu);

		sCallbacksValid = true;
		}



	// Install the callbacks
	theErr = SetDataBrowserCallbacks(*this, &sCallbacks);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::RemoveCallbacks : Remove our callbacks.
//----------------------------------------------------------------------------
void NDataBrowser::RemoveCallbacks(void)
{	DataBrowserCallbacks		theCallbacks = { kDataBrowserLatestCallbacks, 0 };
	OSStatus					theErr;



	// Remove the callbacks
	theErr = InitDataBrowserCallbacks(&theCallbacks);
	NN_ASSERT_NOERR(theErr);
	
	theErr = SetDataBrowserCallbacks(*this, &theCallbacks);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDataBrowser::GetSource : Get the source for a browser.
//----------------------------------------------------------------------------
NDataBrowserSource *NDataBrowser::GetSource(ControlRef theControl, bool allowNULL)
{	NDataBrowser	*thisPtr;



	// Get the browser
	thisPtr = dynamic_cast<NDataBrowser*>(NHIObject::GetNHIObject((HIObjectRef) theControl));
	if (thisPtr == NULL)
		{
		NN_ASSERT(allowNULL);
		return(NULL);
		}



	// Get the source
	if (!allowNULL)
		NN_ASSERT(thisPtr->mSource != NULL);
	
	return(thisPtr->mSource);
}





//============================================================================
//		NDataBrowser::DBItemData : Item get/set.
//----------------------------------------------------------------------------
OSStatus NDataBrowser::DBItemData(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, DataBrowserItemDataRef itemData, Boolean setValue)
{	NDataBrowserSource		*theSource = GetSource(browser);
	OSStatus				theErr;



	// Ignore the root container
	//
	// We may want to make this a per-source behaviour, but by default we assume
	// that browser sources don't support any properties for the root container.
	if (item == kDataBrowserNoItem)
		return(errDataBrowserPropertyNotSupported);



	// Invoke the source
	if (setValue)
		theErr = theSource->SetItemData(item, itemData, property);
	else
		theErr = theSource->GetItemData(item, itemData, property);
	
	return(theErr);
}





//============================================================================
//		NDataBrowser::DBItemCompare : Item compare.
//----------------------------------------------------------------------------
Boolean NDataBrowser::DBItemCompare(ControlRef browser, DataBrowserItemID itemOne, DataBrowserItemID itemTwo, DataBrowserPropertyID sortProperty)
{	NDataBrowserSource		*theSource = GetSource(browser);
	CFComparisonResult		theResult;



	// Invoke the source
	theResult = theSource->CompareItems(itemOne, itemTwo, sortProperty);
	
	return(theResult == kCFCompareLessThan);
}





//============================================================================
//		NDataBrowser::DBItemNotification : Item notification.
//----------------------------------------------------------------------------
void NDataBrowser::DBItemNotification(ControlRef						browser,
										DataBrowserItemID				item,
										DataBrowserItemNotification		message,
										DataBrowserItemDataRef			/*itemData*/)
{	NDataBrowserSource		*theSource = GetSource(browser, true);



	// Check our state
	if (theSource == NULL)
		return;



	// Invoke the source
	switch (message) {
		// Item notifications
		case kDataBrowserItemAdded:
			theSource->ItemAdded(item);
			break;
		
		case kDataBrowserItemRemoved:
			theSource->ItemRemoved(item);
			break;
		
		case kDataBrowserEditStarted:
			theSource->ItemEditBegin(item);
			break;
		
		case kDataBrowserEditStopped:
			theSource->ItemEditEnd(item);
			break;
		
		case kDataBrowserItemSelected:
			theSource->ItemSelected(item);
			break;
		
		case kDataBrowserItemDeselected:
			theSource->ItemDeselected(item);
			break;
		
		case kDataBrowserItemDoubleClicked:
			theSource->ItemOpened(item);
			break;
		
		
		// Container notifications
		case kDataBrowserContainerOpened:
			theSource->ContainerOpened(item);
			break;
		
		case kDataBrowserContainerClosing:
			theSource->ContainerClosing(item);
			break;
		
		case kDataBrowserContainerClosed:
			theSource->ContainerClosed(item);
			break;
		
		case kDataBrowserContainerSorting:
			theSource->ContainerSorting(item);
			break;
		
		case kDataBrowserContainerSorted:
			theSource->ContainerSorted(item);
			break;
		
		case kDataBrowserUserToggledContainer:
			theSource->ContainerToggled(item);
			break;
		
		
		// Browser notifications
		case kDataBrowserTargetChanged:
			theSource->BrowserTarget();
			break;
		
		case kDataBrowserUserStateChanged:
			theSource->BrowserLayout();
			break;
		
		case kDataBrowserSelectionSetChanged:
			theSource->BrowserSelection();
			break;
		}
}





//============================================================================
//		NDataBrowser::DBAddDragItem : Begin a drag.
//----------------------------------------------------------------------------
Boolean NDataBrowser::DBAddDragItem(ControlRef browser, DragReference theDrag, DataBrowserItemID item, ItemReference */*itemRef*/)
{	NDataBrowserSource		*theSource = GetSource(browser);



	// Invoke the source
	return(theSource->CanDrag(item, theDrag));
}





//============================================================================
//		NDataBrowser::DBAcceptDrag : Accept a drag.
//----------------------------------------------------------------------------
Boolean NDataBrowser::DBAcceptDrag(ControlRef browser, DragReference theDrag, DataBrowserItemID item)
{	NDataBrowserSource		*theSource = GetSource(browser);



	// Invoke the source
	return(theSource->AcceptDrag(item, theDrag));
}





//============================================================================
//		NDataBrowser::DBReceiveDrag : Receive a drag.
//----------------------------------------------------------------------------
Boolean NDataBrowser::DBReceiveDrag(ControlRef browser, DragReference theDrag, DataBrowserItemID item)
{	NDataBrowserSource		*theSource = GetSource(browser);



	// Invoke the source
	return(theSource->ReceiveDrag(item, theDrag));
}





//============================================================================
//		NDataBrowser::DBPostProcessDrag : Post-process a drag.
//----------------------------------------------------------------------------
void NDataBrowser::DBPostProcessDrag(ControlRef browser, DragReference theDrag, OSStatus trackDragResult)
{	NDataBrowserSource		*theSource = GetSource(browser);



	// Invoke the source
	return(theSource->FinishDrag(trackDragResult, theDrag));
}





//============================================================================
//		NDataBrowser::DBItemHelpContent : Get the help content.
//----------------------------------------------------------------------------
void NDataBrowser::DBItemHelpContent(ControlRef browser, DataBrowserItemID item, DataBrowserPropertyID property, HMContentRequest inRequest, HMContentProvidedType *outContentProvided, HMHelpContentPtr ioHelpContent)
{	NDataBrowserSource		*theSource = GetSource(browser);



	// Invoke the source
	theSource->GetItemHelp(item, property, inRequest, *outContentProvided, *ioHelpContent);
}





//============================================================================
//		NDataBrowser::DBGetContextualMenu : Get the contextual menu.
//----------------------------------------------------------------------------
void NDataBrowser::DBGetContextualMenu(ControlRef browser, MenuRef *menu, UInt32 *helpType, CFStringRef *helpItemString, AEDesc *selection)
{	NDataBrowserSource		*theSource = GetSource(browser);



	// Invoke the source
	*helpItemString = NULL;
	*menu           = theSource->GetContextualMenu(*helpType, *selection);
}





//============================================================================
//		NDataBrowser::DBSelectContextualMenu : Select a contextual menu.
//----------------------------------------------------------------------------
void NDataBrowser::DBSelectContextualMenu(ControlRef browser, MenuRef menu, UInt32 selectionType, SInt16 /*menuID*/, MenuItemIndex menuItem)
{	NDataBrowserSource		*theSource = GetSource(browser);



	// Invoke the source
	theSource->HandleContextualMenu(selectionType, menu, menuItem);
}

