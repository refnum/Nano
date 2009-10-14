/*	NAME:
		CToolbarDocument.cpp

	DESCRIPTION:
		NanoViewer document toolbar.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NanoViewer Build.h"
#include "CApplication.h"
#include "CToolbarDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Toolbar IDs
#define kToolbarDocumentID										"com.refnum.nano.nanoviewer.toolbar.document"
#define kToolbarItemRotateLeftID								kToolbarDocumentID ".rotate.left"
#define kToolbarItemRotateRightID								kToolbarDocumentID ".rotate.right"
#define kToolbarItemActualSizeID								kToolbarDocumentID ".actualsize"
#define kToolbarItemZoomToFitID									kToolbarDocumentID ".zoom.fit"
#define kToolbarItemZoomInID									kToolbarDocumentID ".zoom.in"
#define kToolbarItemZoomOutID									kToolbarDocumentID ".zoom.out"


// Toolbar items
static const HIToolbarItemInfo kItemRotateLeft					= { kToolbarItemRotateLeftID,         kCmdDocumentRotateLeft,  kImageToolbarDocRotateLeft,  kStringToolbarDocRotateLeft  };
static const HIToolbarItemInfo kItemRotateRight					= { kToolbarItemRotateRightID,        kCmdDocumentRotateRight, kImageToolbarDocRotateRight, kStringToolbarDocRotateRight };
static const HIToolbarItemInfo kItemActualSize					= { kToolbarItemActualSizeID,         kCmdDocumentActualSize,  kImageToolbarDocActualSize,  kStringToolbarDocActualSize  };
static const HIToolbarItemInfo kItemZoomToFit					= { kToolbarItemZoomToFitID,          kCmdDocumentZoomToFit,   kImageToolbarDocZoomToFit,   kStringToolbarDocZoomToFit   };
static const HIToolbarItemInfo kItemZoomIn						= { kToolbarItemZoomInID,             kCmdDocumentZoomIn,      kImageToolbarDocZoomIn,      kStringToolbarDocZoomIn      };
static const HIToolbarItemInfo kItemZoomOut						= { kToolbarItemZoomOutID,            kCmdDocumentZoomOut,     kImageToolbarDocZoomOut,     kStringToolbarDocZoomOut     };
static const HIToolbarItemInfo kItemSeparator					= { kHIToolbarSeparatorIdentifier     };
static const HIToolbarItemInfo kItemSpace						= { kHIToolbarSpaceIdentifier         };
static const HIToolbarItemInfo kItemFlexibleSpace				= { kHIToolbarFlexibleSpaceIdentifier };
static const HIToolbarItemInfo kItemPrint						= { kHIToolbarPrintItemIdentifier,    kHICommandNone, "", "", "", "", kHIToolbarItemBehaviourInConfig };
static const HIToolbarItemInfo kItemCustomize					= { kHIToolbarCustomizeIdentifier,    kHICommandNone, "", "", "", "", kHIToolbarItemBehaviourInConfig };
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
	theList.push_back(kItemRotateLeft);
	theList.push_back(kItemRotateRight);
	theList.push_back(kItemSeparator);
	theList.push_back(kItemActualSize);
	theList.push_back(kItemZoomToFit);
	theList.push_back(kItemZoomIn);
	theList.push_back(kItemZoomOut);

	theList.push_back(kItemFlexibleSpace);
	theList.push_back(kItemSpace);

	theList.push_back(kItemPrint);
	theList.push_back(kItemCustomize);
	theList.push_back(kItemUnifiedInset);

	return(theList);
}

