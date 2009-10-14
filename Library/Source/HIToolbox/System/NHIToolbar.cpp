/*	NAME:
		NHIToolbar.cpp

	DESCRIPTION:
		Toolbar object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIViewUtilities.h"
#include "NHIToolbar.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kHIToolbarSystemItemPrefix					= "com.apple";

static const HIRect kBoundsUnifiedInset							= { kHIPointZero, { 1, 10 } };





//============================================================================
//		NHIToolbar::NHIToolbar : Constructor.
//----------------------------------------------------------------------------
NHIToolbar::NHIToolbar(const NString &theID, OptionBits theAttributes)
{	const EventTypeSpec	theEvents[] = { { kEventClassToolbar, kEventToolbarCreateItemWithIdentifier },
										{ kEventClassToolbar, kEventToolbarGetAllowedIdentifiers    },
										{ kEventClassToolbar, kEventToolbarGetDefaultIdentifiers    },
										{ kEventClassToolbar, kEventToolbarGetSelectableIdentifiers },
										{ kEventClassToolbar, kEventToolbarItemAdded				},
										{ kEventClassToolbar, kEventToolbarItemRemoved				},
										{ kEventClassToolbar, kEventToolbarLayoutChanged			},
										{ kEventClassNone,    kEventKindNone						} };
	HIToolbarRef	theToolbar;
	OSStatus		theErr;
	


	// Create the toolbar
	theErr = HIToolbarCreate(theID, theAttributes, &theToolbar);
	NN_ASSERT_NOERR(theErr);
	
	SetHIObject(theToolbar);



	// Register for events
	AddEvents(theEvents);
}





//============================================================================
//		NHIToolbar::~NHIToolbar : Destructor.
//----------------------------------------------------------------------------
NHIToolbar::~NHIToolbar(void)
{
}





//============================================================================
//		NHIToolbar::GetIdentifier : Get the identifier.
//----------------------------------------------------------------------------
NString NHIToolbar::GetIdentifier(void) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Get the identifier
	cfString = NULL;
	theErr   = HIToolbarCopyIdentifier(*this, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NHIToolbar::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
OptionBits NHIToolbar::GetAttributes(void) const
{	OptionBits		theValue;
	OSStatus		theErr;



	// Get the attributes
	theValue = kHIToolbarNoAttributes;
	theErr   = HIToolbarGetAttributes(*this, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NHIToolbar::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NHIToolbar::SetAttributes(OptionBits setThese, OptionBits clearThese)
{	OSStatus	theErr;



	// Set the attributes
	theErr = HIToolbarChangeAttributes(*this, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbar::GetDisplayMode : Get the display mode.
//----------------------------------------------------------------------------
HIToolbarDisplayMode NHIToolbar::GetDisplayMode(void) const
{	HIToolbarDisplayMode	theValue;
	OSStatus				theErr;



	// Get the display mode
	theValue = kHIToolbarDisplayModeDefault;
	theErr   = HIToolbarGetDisplayMode(*this, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NHIToolbar::SetDisplayMode : Set the display mode.
//----------------------------------------------------------------------------
void NHIToolbar::SetDisplayMode(HIToolbarDisplayMode theValue)
{	OSStatus	theErr;



	// Set the display mode
	theErr = HIToolbarSetDisplayMode(*this, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbar::GetDisplaySize : Get the display size.
//----------------------------------------------------------------------------
HIToolbarDisplaySize NHIToolbar::GetDisplaySize(void) const
{	HIToolbarDisplaySize	theValue;
	OSStatus				theErr;



	// Get the display size
	theValue = kHIToolbarDisplaySizeDefault;
	theErr   = HIToolbarGetDisplaySize(*this, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NHIToolbar::SetDisplaySize : Set the display size.
//----------------------------------------------------------------------------
void NHIToolbar::SetDisplaySize(HIToolbarDisplaySize theValue)
{	OSStatus	theErr;



	// Set the display size
	theErr = HIToolbarSetDisplaySize(*this, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIToolbar::GetToolbarItems : Get the toolbar items.
//----------------------------------------------------------------------------
NHIToolbarItemList NHIToolbar::GetToolbarItems(void) const
{	HIToolbarItemRef		hiToolbarItem;
	UInt32					n, numItems;
	NHIToolbarItemList		theItems;
	NHIToolbarItem			*theItem;
	CFArrayRef				cfArray;
	OSStatus				theErr;



	// Get the items
	theErr = HIToolbarCopyItems(*this, &cfArray);
	if (theErr != noErr)
		return(theItems);



	// Build the list
	numItems = CFArrayGetCount(cfArray);
	
	for (n = 0; n < numItems; n++)
		{
		// Get the item
		hiToolbarItem = (HIToolbarItemRef) CFArrayGetValueAtIndex(cfArray, n);
		theItem       = dynamic_cast<NHIToolbarItem*>(NHIObject::GetNHIObject(hiToolbarItem));


		// Create a wrapper if necessary
		//
		// System items are created by the HIToolbar object rather than our CreateItem,
		// however we create a wrapper NHIToolbarItem object for these objects as needed
		// so that they can be manipulated as with any other item.
		//
		// Since the wrapper is an NHIObject, it will only be created once and will be
		// destroyed when the corresponding HIObjectRef is released.
		if (theItem == NULL)
			theItem = new NHIToolbarItem(hiToolbarItem);
		
		
		// Update the list
		if (theItem != NULL)
			theItems.push_back(theItem);
		}



	// Clean up
	CFSafeRelease(cfArray);
	
	return(theItems);
}





//============================================================================
//		NHIToolbar::GetToolbarItem : Get a toolbar item.
//----------------------------------------------------------------------------
NHIToolbarItem *NHIToolbar::GetToolbarItem(const NString &theIdentifier)
{	NHIToolbarItemList					theItems;
	NHIToolbarItem						*theItem;
	NHIToolbarItemListConstIterator		theIter;



	// Validate our parameters
	NN_ASSERT(theIdentifier.IsNotEmpty());



	// Find the item
	theItems = GetToolbarItems();
	
	for (theIter = theItems.begin(); theIter != theItems.end(); theIter++)
		{
		theItem = *theIter;
		NN_ASSERT(theItem != NULL);

		if (theItem->IsIdentifier(theIdentifier))
			return(theItem);
		}
	
	return(NULL);
}





//============================================================================
//		NHIToolbar::Attach : Attach the toolbar.
//----------------------------------------------------------------------------
void NHIToolbar::Attach(NHIWindow *theWindow)
{	HIToolbarItemInfoListIterator	theIter;



	// Get the contents
	//
	// We can not fetch the contents in our constructor while creating the toolbar,
	// since we can't use a pure virtual method until our sub-class is constructed.
	//
	// Since we don't receive any events until the toolbar is attached, we can
	// defer the initialisation to this point.
	mItems = GetToolbarContents();



	// Configure the built-in items
	//
	// Nano provides some additional built-in items, which can be added simply by
	// specifying their identifier in the list of contents.
	//
	// The exact configuration of those items is determined by the base-class.
	for (theIter = mItems.begin(); theIter != mItems.end(); theIter++)
		{
		if (theIter->theIdentifier == kHIToolbarUnifiedInsetIdentifier)
			{
			theIter->theBehaviour  = kHIToolbarItemBehaviourIsDefault;
			theIter->theAttributes = kHIToolbarItemAllowDuplicates | kHIToolbarItemCantBeRemoved | kHIToolbarItemDisabled;
			
			if (theIter == mItems.begin())
				theIter->theAttributes |= kHIToolbarItemAnchoredLeft;
			}
		}



	// Attach the toolbar
	theWindow->SetToolbar(this, true);
}





//============================================================================
//		NHIToolbar::operator HIToolbarRef : HIToolbarRef-cast operator.
//----------------------------------------------------------------------------
NHIToolbar::operator HIToolbarRef(void) const
{


	// Get the toolbar
	return((HIToolbarRef) GetHIObject());
}





//============================================================================
//		NHIToolbar::CreateItem : Create a toolbar item.
//----------------------------------------------------------------------------
#pragma mark -
NHIToolbarItem *NHIToolbar::CreateItem(const HIToolbarItemInfo &theInfo)
{	NHIToolbarItem		*theItem;



	// Create the item
	//
	// The default implementation is to create a new NHIToolbarItem, however
	// sub-classes can override this method to customize the item after it has
	// been created - or to instantiate their own NHIToolbarItem subclass.
	theItem = new NHIToolbarItem(this, theInfo);
	NN_ASSERT(theItem);
	
	return(theItem);
}





//============================================================================
//		NHIToolbar::CreateItemView : Create a toolbar item view.
//----------------------------------------------------------------------------
NHIView *NHIToolbar::CreateItemView(const NHIToolbarItem *theItem)
{	NHIView		*theView;



	// Create a unified inset
	//
	// The "unified inset" view is a special view to address rdar://4429919,
	// where the Mac OS X 10.4 Unified Toolbar appearance places items too
	// close to the window edge.
	//
	// A screenshot of this effect can be seen at:
	//
	//		http://lists.apple.com/archives/carbon-dev/2006/Feb/msg00001.html
	//
	// This affect is particularly visible when selected items are placed in
	// the toolbar, since the selection effect ends up being drawn flush with
	// the window edge (rdar://4300873).
	//
	//
	// This item is configured to not appear in the configuration sheet for
	// the toolbar, and can not be removed by the user.
	//
	// If this item is placed at the start of a toolbar, it will also have the
	// kHIToolbarItemAnchoredLeft attribute turned on to prevent the user from
	// moving it around.
	//
	// Unfortunately there is no kHIToolbarItemAnchoredRight (rdar://4417751),
	// so if it is placed elsewhere in the toolbar the user will be able to
	// drag it around (although being invisible, this is seldom a problem).
	if (theItem->IsIdentifier(kHIToolbarUnifiedInsetIdentifier))
		{
		theView = NHIViewUtilities::CreateHIView(NULL, kBoundsUnifiedInset);
		theView->SetSizeConstraints(&kBoundsUnifiedInset.size, &kBoundsUnifiedInset.size);
		
		return(theView);
		}



	// Or use the default
	//
	// By returning NULL, the toolbar will use the standard item view.
	//
	// To insert a custom view (e.g., an NHISegmentedView) into a toolbar,
	// sub-classes should override this method to create the view for their
	// item and call through for unknown items.
	return(NULL);
}





//============================================================================
//		NHIToolbar::DoEventToolbarCreateItemWithIdentifier : Handle kEventToolbarCreateItemWithIdentifier.
//----------------------------------------------------------------------------
OSStatus NHIToolbar::DoEventToolbarCreateItemWithIdentifier(NCarbonEvent &theEvent)
{	NString									theIdentifier;
	NHIToolbarItem							*theItem;
	HIToolbarItemInfoListConstIterator		theIter;
	HIToolbarItemRef						itemRef;
	OSStatus								theErr;



	// Get the state we need
	theIdentifier = theEvent.GetParameterString(kEventParamToolbarItemIdentifier);
	NN_ASSERT(theIdentifier.IsNotEmpty());



	// Check for system items
	//
	// Note that this event is invoked for any item, even standard system
	// items, however we are only required to create an item for custom items.
	//
	// Standard system items must be left for the default handler to process.
	if (theIdentifier.StartsWith(kHIToolbarSystemItemPrefix))
		return(eventNotHandledErr);



	// Create a custom item
	for (theIter = mItems.begin(); theIter != mItems.end(); theIter++)
		{
		if (theIter->theIdentifier == theIdentifier)
			{
			theItem = CreateItem(*theIter);
			NN_ASSERT(theItem != NULL);
			
			if (theItem != NULL)
				{
				itemRef = *theItem;
				theErr  = theEvent.SetParameter(kEventParamToolbarItem, typeHIToolbarItemRef, sizeof(itemRef), &itemRef);
				NN_ASSERT_NOERR(theErr);
			
				return(theErr);
				}
			}
		}



	// Handle failure
	NN_LOG("Unknown item: %s", theIdentifier.GetUTF8());
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIToolbar::DoEventToolbarGetAllowedIdentifiers : Handle kEventToolbarGetAllowedIdentifiers.
//----------------------------------------------------------------------------
OSStatus NHIToolbar::DoEventToolbarGetAllowedIdentifiers(NCarbonEvent &theEvent)
{


	// Add the items
	AddItems(theEvent, kHIToolbarItemBehaviourInConfig);
	
	return(noErr);
}





//============================================================================
//		NHIToolbar::DoEventToolbarGetDefaultIdentifiers : Handle kEventToolbarGetDefaultIdentifiers.
//----------------------------------------------------------------------------
OSStatus NHIToolbar::DoEventToolbarGetDefaultIdentifiers(NCarbonEvent &theEvent)
{


	// Add the items
	AddItems(theEvent, kHIToolbarItemBehaviourIsDefault);
	
	return(noErr);
}





//============================================================================
//		NHIToolbar::DoEventToolbarGetSelectableIdentifiers : Handle kEventToolbarGetSelectableIdentifiers.
//----------------------------------------------------------------------------
OSStatus NHIToolbar::DoEventToolbarGetSelectableIdentifiers(NCarbonEvent &theEvent)
{


	// Add the items
	AddItems(theEvent, kHIToolbarItemBehaviourIsSelectable);
	
	return(noErr);
}





//============================================================================
//		NHIToolbar::DoEventToolbarItemAdded : Handle kEventToolbarItemAdded.
//----------------------------------------------------------------------------
OSStatus NHIToolbar::DoEventToolbarItemAdded(NCarbonEvent &/*theEvent*/)
{


	// Let everyone know
	BroadcastMessage(kMsgToolbarItemAdded, this);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIToolbar::DoEventToolbarItemRemoved : Handle kEventToolbarItemRemoved.
//----------------------------------------------------------------------------
OSStatus NHIToolbar::DoEventToolbarItemRemoved(NCarbonEvent &/*theEvent*/)
{


	// Let everyone know
	BroadcastMessage(kMsgToolbarItemRemoved, this);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIToolbar::DoEventToolbarLayoutChanged : Handle kEventToolbarLayoutChanged.
//----------------------------------------------------------------------------
OSStatus NHIToolbar::DoEventToolbarLayoutChanged(NCarbonEvent &/*theEvent*/)
{


	// Let everyone know
	BroadcastMessage(kMsgToolbarLayoutChanged, this);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NHIToolbar::AddItems : Add the toolbar items to an event.
//----------------------------------------------------------------------------
#pragma mark -
void NHIToolbar::AddItems(NCarbonEvent &theEvent, OptionBits theMask)
{	OptionBits								theFlags;
	CFMutableArrayRef						cfArray;
	HIToolbarItemInfoListConstIterator		theIter;
	OSStatus								theErr;



	// Get the array
	theErr = theEvent.GetParameter(kEventParamMutableArray, typeCFMutableArrayRef, sizeof(cfArray), &cfArray);
	NN_ASSERT_NOERR(theErr);



	// Add the items
	for (theIter = mItems.begin(); theIter != mItems.end(); theIter++)
		{
		// Get the behaviour flags
		//
		// To allow items to obtain the default behaviour from a minimal HIToolbarItemInfo
		// structure, the "default value" is 0 rather than the more normal "default values
		// or'd together" approach.
		//
		// This allows apps to obtain the default behaviour without initializing that field
		// in the structure, but means we must explicitly set the default bits now.
		theFlags = theIter->theBehaviour;

		if (theFlags == kHIToolbarItemBehaviourDefault)
			theFlags = kHIToolbarItemBehaviourInConfig | kHIToolbarItemBehaviourIsDefault;



		// Add matching items
		if ((theMask & theFlags) == theMask)
			CFArrayAppendValue(cfArray, theIter->theIdentifier);
		}
}



