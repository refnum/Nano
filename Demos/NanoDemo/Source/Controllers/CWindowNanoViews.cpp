/*	NAME:
		CWindowNanoViews.cpp

	DESCRIPTION:
		NanoDemo custom views window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"

#include "NanoDemo Build.h"
#include "CWindowNanoViews.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const EventTime kDelayUpdate									= 0.025f;





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(CWindowNanoViews, kAppNibFile, "Nano Views");





//============================================================================
//		CWindowNanoViews::CWindowNanoViews : Constructor.
//----------------------------------------------------------------------------
CWindowNanoViews::CWindowNanoViews(void)
{
}





//============================================================================
//		CWindowNanoViews::~CWindowNanoViews : Destructor.
//----------------------------------------------------------------------------
CWindowNanoViews::~CWindowNanoViews(void)
{
}





//============================================================================
//		CWindowNanoViews::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowNanoViews::InitializeSelf(void)
{	NSourceListItemList		theItems;
	NSourceListItem			theItem;



	// Initialize ourselves
	mProgress = 0;

	mPDF->SetImageOrigin(CGPointMake(144, 106));



	// Configure the source list
	//
	// This demo creates a simple source list that with "library" and "playlists" sections.
	//
	// Typically all strings/icons would be obtained as resources from the app bundle, with
	// user-created strings (the playlist names) stored in the preferences.
	//
	// Each item in a source list requires a unique ID; these would typically be constants
	// for fixed items, and some unique identifier for user-created items.
	theItem = NSourceListItem("id.library", "NSourceListView");
	theItem.AddChild("library.music",    NBundleString(kStringLibraryMusic),    NIcon(kBundleIDiTunes));
	theItem.AddChild("library.photos",   NBundleString(kStringLibraryPhotos),   NIcon(kBundleIDiPhoto));
	theItem.AddChild("library.mail",     NBundleString(kStringLibraryMail),     NIcon(kBundleIDMail));
	theItem.AddChild("library.internet", NBundleString(kStringLibraryInternet), NIcon(kBundleIDSafari));
	theItems.push_back(theItem);

	theItem = NSourceListItem("id.playlists", "Playlists", NIcon(), kSLItemCanExpand);
	theItem.AddChild("playlists.1", "Books and Spoken", NIcon(kGenericFolderIcon));
	theItem.AddChild("playlists.2", "Recently Added",   NIcon(kGenericFolderIcon));
	theItem.AddChild("playlists.3", "Never Played",     NIcon(kGenericFolderIcon));
	theItem.AddChild("playlists.4", "For Running",      NIcon(kGenericFolderIcon));
	theItem.AddChild("playlists.5", "Ambient",          NIcon(kGenericFolderIcon));
	theItems.push_back(theItem);
	
	mSourceList->SetItems(theItems);
	


	// Start getting time
	mTimer.AddTimer(BindSelf(CWindowNanoViews::UpdateViews), kDelayUpdate, kDelayUpdate);

	return(noErr);
}





//============================================================================
//		CWindowNanoViews::UpdateViews : Update the views.
//----------------------------------------------------------------------------
#pragma mark -
void CWindowNanoViews::UpdateViews(void)
{	float	theValue;



	// Update the progress
	mProgress += 1;

	if (mProgress > 100)
		mProgress = 0;
	
	theValue = (float) mProgress / 100.0f;



	// Update the views
	mProgressPie->SetPercentValue(    theValue);
	mLevelContinuous->SetPercentValue(theValue);
	mLevelDiscrete->SetPercentValue(  theValue);
}





