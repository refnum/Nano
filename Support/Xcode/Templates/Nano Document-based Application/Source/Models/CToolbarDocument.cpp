/*	NAME:
		CToolbarDocument.cpp

	DESCRIPTION:
		ÇPROJECTNAMEÈ document toolbar.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ÇPROJECTNAMEÈ Build.h"
#include "CApplication.h"
#include "CToolbarDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Toolbar IDs
#define kToolbarDocumentID										"com.mycompany.ÇPROJECTNAMEASIDENTIFIERÈ.toolbar.document"
#define kToolbarItemColorID										kToolbarDocumentID ".color"
#define kToolbarItemInfoID										kToolbarDocumentID ".info"


// Toolbar items
static const HIToolbarItemInfo kItemColor						= { kToolbarItemColorID,              kHICommandNone,   "",                   kStringToolbarDocColor };
static const HIToolbarItemInfo kItemInfo						= { kToolbarItemInfoID,               kCmdDocumentInfo, kImageToolbarDocInfo, kStringToolbarDocInfo  };
static const HIToolbarItemInfo kItemFlexibleSpace				= { kHIToolbarFlexibleSpaceIdentifier };
static const HIToolbarItemInfo kItemPrint						= { kHIToolbarPrintItemIdentifier     };
static const HIToolbarItemInfo kItemCustomize					= { kHIToolbarCustomizeIdentifier     };
static const HIToolbarItemInfo kItemUnifiedInset				= { kHIToolbarUnifiedInsetIdentifier  };


// Misc
static const HIRect kBoundsColor								= { kHIPointZero, {  30, 24 } };





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
//		CToolbarDocument::GetColorPicker : Get the color picker.
//----------------------------------------------------------------------------
NColorPicker *CToolbarDocument::GetColorPicker(void)
{	NHIToolbarItem		*theItem;
	NHIView				*theView;



	// Get the color picker
	theItem = GetToolbarItem(kToolbarItemColorID);
	theView = (theItem == NULL) ? NULL : theItem->GetView();
	
	return(dynamic_cast<NColorPicker*>(theView));
}





//============================================================================
//		CToolbarDocument::GetToolbarContents : Get the toolbar contents.
//----------------------------------------------------------------------------
#pragma mark -
HIToolbarItemInfoList CToolbarDocument::GetToolbarContents(void)
{	HIToolbarItemInfoList	theList;



	// Get the items
	theList.push_back(kItemUnifiedInset);
	theList.push_back(kItemColor);
	theList.push_back(kItemInfo);
	theList.push_back(kItemFlexibleSpace);
	theList.push_back(kItemPrint);
	theList.push_back(kItemCustomize);
	theList.push_back(kItemUnifiedInset);

	return(theList);
}





//============================================================================
//		CToolbarDocument::CreateItemView : Create a toolbar item view.
//----------------------------------------------------------------------------
NHIView *CToolbarDocument::CreateItemView(const NHIToolbarItem *theItem)
{	NColorPicker	*colorPicker;



	// Create the color picker
	if (theItem->IsIdentifier(kToolbarItemColorID))
		{
		colorPicker = new NColorPicker(NULL, kBoundsColor);
		colorPicker->SetSizeConstraints(&kBoundsColor.size, &kBoundsColor.size);

		return(colorPicker);
		}



	// Or use the default
	return(NHIToolbar::CreateItemView(theItem));
}
