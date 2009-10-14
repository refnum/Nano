/*	NAME:
		NTabbedView.cpp

	DESCRIPTION:
		Tabbed view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTabbedView.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NTabbedView, kHITabbedViewClassID, NULL);





//============================================================================
//		NTabbedView::NTabbedView : Constructor.
//----------------------------------------------------------------------------
NTabbedView::NTabbedView(HIWindowRef				theWindow,
							const HIRect			&theRect,
							const TabInfoList		&theTabs,
							ControlTabSize			theSize,
							ControlTabDirection		theDirection)
{	UInt32									n, numItems;
	std::vector<ControlButtonContentInfo>	tabContent;
	std::vector<ControlTabEntry>			tabInfo;
	HIViewRef								theView;
	OSStatus								theErr;



	// Prepare the tabs
	numItems = theTabs.size();
	
	tabContent.resize(numItems);
	tabInfo.resize(   numItems);
	
	for (n = 0; n < numItems; n++)
		{
		// Prepare the content
		tabContent[n] = kControlContentNone;
		
		if (theTabs[n].theIcon.IsValid())
			{
			tabContent[n].contentType = kControlContentIconRef;
			tabContent[n].u.iconRef   = theTabs[n].theIcon;
			}



		// Initialize the tab entry
		tabInfo[n].icon    = &tabContent[n];
		tabInfo[n].name    = theTabs[n].theName;
		tabInfo[n].enabled = theTabs[n].isEnabled;
		}



	// Create the view
	theErr = CreateTabsControl(NULL, &kQDRectZero, theSize, theDirection,
								tabInfo.size(), &tabInfo[0], &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NTabbedView::~NTabbedView : Destructor.
//----------------------------------------------------------------------------
NTabbedView::~NTabbedView(void)
{
}





//============================================================================
//		NTabbedView::GetTabCount : Get the number of tabs.
//----------------------------------------------------------------------------
UInt32 NTabbedView::GetTabCount(void) const
{


	// Get the tab count
	return(GetMaximumValue());
}





//============================================================================
//		NTabbedView::GetTabInfo : Get a tab's info.
//----------------------------------------------------------------------------
TabInfo NTabbedView::GetTabInfo(UInt32 theIndex) const
{	TabInfo	theInfo;



	// Get the info
	theInfo.isEnabled = IsTabEnabled(theIndex);
	theInfo.theName   = GetTabName(  theIndex);
	theInfo.theIcon   = GetTabIcon(  theIndex);
	
	return(theInfo);
}





//============================================================================
//		NTabbedView::SetTabInfo : Set a tab's info.
//----------------------------------------------------------------------------
void NTabbedView::SetTabInfo(UInt32 theIndex, const TabInfo &theInfo)
{


	// Set the info
	SetTabEnabled(theIndex, theInfo.isEnabled);
	SetTabName(   theIndex, theInfo.theName);
	SetTabIcon(   theIndex, theInfo.theIcon);
}





//============================================================================
//		NTabbedView::IsTabEnabled : Is a tab enabled?
//----------------------------------------------------------------------------
bool NTabbedView::IsTabEnabled(UInt32 theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 1 && theIndex <= GetTabCount());



	// Get the tab state
	return(GetControlBoolean(theIndex, kControlTabEnabledFlagTag));
}





//============================================================================
//		NTabbedView::SetTabEnabled : Set a tab's enabled state.
//----------------------------------------------------------------------------
void NTabbedView::SetTabEnabled(UInt32 theIndex, bool isEnabled)
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 1 && theIndex <= GetTabCount());



	// Set the tab state
	SetControlBoolean(theIndex, kControlTabEnabledFlagTag, isEnabled);
}





//============================================================================
//		NTabbedView::GetTabName : Get a tab's name.
//----------------------------------------------------------------------------
NString NTabbedView::GetTabName(UInt32 theIndex) const
{	ControlTabInfoRecV1		theInfo = { kControlTabInfoVersionOne, 0, NULL };
	NString					theValue;



	// Validate our parameters
	NN_ASSERT(theIndex >= 1 && theIndex <= GetTabCount());



	// Get the tab state
	//
	// kControlTabInfoTag does not follow the CF model of Get/Copy/Create,
	// and we are assumed to take ownership of the returned string. 
	GetControlData(theIndex, kControlTabInfoTag, sizeof(theInfo), &theInfo);

	if (theInfo.name != NULL)
		theValue.Set(theInfo.name);
	
	return(theValue);
}





//============================================================================
//		NTabbedView::SetTabName : Set a tab's name.
//----------------------------------------------------------------------------
void NTabbedView::SetTabName(UInt32 theIndex, const NString &theValue)
{	ControlTabInfoRecV1		theInfo = { kControlTabInfoVersionOne, 0, NULL };



	// Validate our parameters
	NN_ASSERT(theIndex >= 1 && theIndex <= GetTabCount());



	// Set the tab state
	theInfo.name = theValue;
	
	SetControlData(theIndex, kControlTabInfoTag, sizeof(theInfo), &theInfo);
}





//============================================================================
//		NTabbedView::GetTabIcon : Get a tab's icon.
//----------------------------------------------------------------------------
NIcon NTabbedView::GetTabIcon(UInt32 theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 1 && theIndex <= GetTabCount());



	// Get the tab icon
	return(GetContentIcon(theIndex, kControlTabImageContentTag));
}





//============================================================================
//		NTabbedView::SetTabIcon : Set a tab's icon.
//----------------------------------------------------------------------------
void NTabbedView::SetTabIcon(UInt32 theIndex, const NIcon &theIcon)
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 1 && theIndex <= GetTabCount());



	// Set the tab icon
	SetContentIcon(theIndex, kControlTabImageContentTag, theIcon);
}


