/*	NAME:
		CToolbarDocument.cpp

	DESCRIPTION:
		NanoDemo document toolbar.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NIBUtilities.h"

#include "NanoDemo Build.h"
#include "CApplication.h"
#include "CToolbarDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Toolbar IDs
#define kToolbarDocumentID										"com.refnum.nano.nanodemo.toolbar.document"
#define kToolbarItemAboutID										kToolbarDocumentID ".about"
#define kToolbarItemShapeID										kToolbarDocumentID ".shape"
#define kToolbarItemColorID										kToolbarDocumentID ".color"


// Toolbar items
static const HIToolbarItemInfo kItemAbout						= { kToolbarItemAboutID,              kHICommandAbout, kImageToolbarDocAbout, kStringToolbarDocAbout };
static const HIToolbarItemInfo kItemShape						= { kToolbarItemShapeID,              kHICommandNone,  "",                    kStringToolbarDocShape };
static const HIToolbarItemInfo kItemColor						= { kToolbarItemColorID,              kHICommandNone,  "",                    kStringToolbarDocColor };
static const HIToolbarItemInfo kItemFlexibleSpace				= { kHIToolbarFlexibleSpaceIdentifier };
static const HIToolbarItemInfo kItemPrint						= { kHIToolbarPrintItemIdentifier     };
static const HIToolbarItemInfo kItemCustomize					= { kHIToolbarCustomizeIdentifier     };
static const HIToolbarItemInfo kItemUnifiedInset				= { kHIToolbarUnifiedInsetIdentifier  };


// Misc
static const NString kShapeMenu									= "Shape";
static const HIRect  kBoundsShape								= { kHIPointZero, { 110, 20 } };
static const HIRect  kBoundsColor								= { kHIPointZero, {  30, 24 } };





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
//		CToolbarDocument::GetShapeMenu : Get the shape menu.
//----------------------------------------------------------------------------
NPopupButton *CToolbarDocument::GetShapeMenu(void)
{	NHIToolbarItem		*theItem;
	NHIView				*theView;



	// Get the shape menu
	theItem = GetToolbarItem(kToolbarItemShapeID);
	theView = (theItem == NULL) ? NULL : theItem->GetView();
	
	return(dynamic_cast<NPopupButton*>(theView));
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
	theList.push_back(kItemAbout);
	theList.push_back(kItemShape);
	theList.push_back(kItemColor);
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
	NPopupButton	*shapeMenu;
	MenuRef			theMenu;



	// Create the shape menu
	if (theItem->IsIdentifier(kToolbarItemShapeID))
		{
		theMenu   = NIBUtilities::CreateMenu(NApplication::GetApp()->GetNib(), kShapeMenu);
		shapeMenu = new NPopupButton(NULL, kBoundsShape, theMenu);
		
		return(shapeMenu);
		}



	// Create the color picker
	else if (theItem->IsIdentifier(kToolbarItemColorID))
		{
		colorPicker = new NColorPicker(NULL, kBoundsColor);
		colorPicker->SetSizeConstraints(&kBoundsColor.size, &kBoundsColor.size);

		return(colorPicker);
		}



	// Or use the default
	return(NHIToolbar::CreateItemView(theItem));
}
