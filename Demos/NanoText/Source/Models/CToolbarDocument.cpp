/*	NAME:
		CToolbarDocument.cpp

	DESCRIPTION:
		NanoText document toolbar.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NanoText Build.h"
#include "CApplication.h"
#include "CToolbarDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Toolbar IDs
#define kToolbarDocumentID										"com.refnum.nano.nanotext.toolbar.document"
#define kToolbarItemColorsID									kToolbarDocumentID ".colors"
#define kToolbarItemFontsID										kToolbarDocumentID ".fonts"
#define kToolbarItemSpellingID									kToolbarDocumentID ".spelling"


// Toolbar items
static const HIToolbarItemInfo kItemColors						= { kToolbarItemColorsID,   kHICommandNToggleColorPanel, kImageToolbarPanelColors,   kStringToolbarPanelColors   };
static const HIToolbarItemInfo kItemFonts						= { kToolbarItemFontsID,    kHICommandShowHideFontPanel, kImageToolbarPanelFonts,    kStringToolbarPanelFonts    };
static const HIToolbarItemInfo kItemSpelling					= { kToolbarItemSpellingID, kHICommandShowSpellingPanel, kImageToolbarPanelSpelling, kStringToolbarPanelSpelling, "", "", 0, kHIToolbarItemSendCmdToUserFocus };
static const HIToolbarItemInfo kItemFlexibleSpace				= { kHIToolbarFlexibleSpaceIdentifier };
static const HIToolbarItemInfo kItemPrint						= { kHIToolbarPrintItemIdentifier     };
static const HIToolbarItemInfo kItemCustomize					= { kHIToolbarCustomizeIdentifier     };
static const HIToolbarItemInfo kItemUnifiedInset				= { kHIToolbarUnifiedInsetIdentifier  };





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
//		CToolbarDocument::GetToolbarContents : Get the toolbar contents.
//----------------------------------------------------------------------------
#pragma mark -
HIToolbarItemInfoList CToolbarDocument::GetToolbarContents(void)
{	HIToolbarItemInfoList	theList;



	// Get the items
	theList.push_back(kItemUnifiedInset);
	theList.push_back(kItemColors);
	theList.push_back(kItemFonts);
	theList.push_back(kItemSpelling);
	theList.push_back(kItemFlexibleSpace);
	theList.push_back(kItemPrint);
	theList.push_back(kItemCustomize);
	theList.push_back(kItemUnifiedInset);

	return(theList);
}




