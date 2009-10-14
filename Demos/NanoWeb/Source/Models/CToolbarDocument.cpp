/*	NAME:
		CToolbarDocument.cpp

	DESCRIPTION:
		NanoWeb document toolbar.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NanoWeb Build.h"
#include "CApplication.h"
#include "CToolbarDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Toolbar IDs
#define kToolbarDocumentID										"com.refnum.nano.nanoweb.toolbar.document"
#define kToolbarItemBackID										kToolbarDocumentID ".back"
#define kToolbarItemForwardID									kToolbarDocumentID ".forward"
#define kToolbarItemAddressID									kToolbarDocumentID ".address"
#define kToolbarItemProgressID									kToolbarDocumentID ".progress"


// Toolbar items
static const HIToolbarItemInfo kItemBack						= { kToolbarItemBackID,     kHICommandNBack,    kImageToolbarDocBack,    kStringToolbarDocBack,    "", "", kHIToolbarItemBehaviourDefault, kHIToolbarItemSendCmdToUserFocus};
static const HIToolbarItemInfo kItemForward						= { kToolbarItemForwardID,  kHICommandNForward, kImageToolbarDocForward, kStringToolbarDocForward, "", "", kHIToolbarItemBehaviourDefault, kHIToolbarItemSendCmdToUserFocus};
static const HIToolbarItemInfo kItemAddress						= { kToolbarItemAddressID,  kHICommandNone,     "",                      kStringToolbarDocAddress };
static const HIToolbarItemInfo kItemProgress					= { kToolbarItemProgressID, kHICommandNone,     "",                      kStringToolbarDocStatus  };
static const HIToolbarItemInfo kItemUnifiedInset				= { kHIToolbarUnifiedInsetIdentifier };


// Misc
static const HISize kMinSizeAddress								= { 100, 16 };
static const HIRect kBoundsAddress								= { kHIPointZero, { 6000, 16 } };
static const HIRect kBoundsProgress								= { kHIPointZero, {   32, 32 } };





//============================================================================
//		CToolbarDocument::CToolbarDocument : Constructor.
//----------------------------------------------------------------------------
CToolbarDocument::CToolbarDocument(void)
				: NHIToolbar(kToolbarDocumentID)
{
}





//============================================================================
//		CToolbarDocument::~CToolbarDocument : Destructor.
//----------------------------------------------------------------------------
CToolbarDocument::~CToolbarDocument(void)
{
}





//============================================================================
//		CToolbarDocument::GetAddress : Get the address field.
//----------------------------------------------------------------------------
NEditText *CToolbarDocument::GetAddress(void)
{	NHIToolbarItem		*theItem;
	NHIView				*theView;



	// Get the address field
	theItem = GetToolbarItem(kToolbarItemAddressID);
	theView = (theItem == NULL) ? NULL : theItem->GetView();
	
	return(dynamic_cast<NEditText*>(theView));
}





//============================================================================
//		CToolbarDocument::GetProgress : Get the progress arrows.
//----------------------------------------------------------------------------
NChasingArrows *CToolbarDocument::GetProgress(void)
{	NHIToolbarItem		*theItem;
	NHIView				*theView;



	// Get the progress field
	theItem = GetToolbarItem(kToolbarItemProgressID);
	theView = (theItem == NULL) ? NULL : theItem->GetView();
	
	return(dynamic_cast<NChasingArrows*>(theView));
}





//============================================================================
//		CToolbarDocument::GetToolbarContents : Get the toolbar contents.
//----------------------------------------------------------------------------
#pragma mark -
HIToolbarItemInfoList CToolbarDocument::GetToolbarContents(void)
{	HIToolbarItemInfoList	theList;



	// Get the items
	theList.push_back(kItemUnifiedInset);
	theList.push_back(kItemBack);
	theList.push_back(kItemForward);
	theList.push_back(kItemAddress);
	theList.push_back(kItemProgress);
	theList.push_back(kItemUnifiedInset);

	return(theList);
}





//============================================================================
//		CToolbarDocument::CreateItemView : Create a toolbar item view.
//----------------------------------------------------------------------------
NHIView *CToolbarDocument::CreateItemView(const NHIToolbarItem *theItem)
{	NEditText			*theAddress;
	NChasingArrows		*theArrows;



	// Create the address field
	if (theItem->IsIdentifier(kToolbarItemAddressID))
		{
		theAddress = new NEditText(NULL, kBoundsAddress);
		theAddress->SetSizeConstraints(&kMinSizeAddress, &kBoundsAddress.size);
		theAddress->SetMultiLine(false);
		
		return(theAddress);
		}



	// Create the progress arrows
	else if (theItem->IsIdentifier(kToolbarItemProgressID))
		{
		theArrows = new NChasingArrows(NULL, kBoundsProgress);

		return(theArrows);
		}



	// Or use the default
	return(NHIToolbar::CreateItemView(theItem));
}
