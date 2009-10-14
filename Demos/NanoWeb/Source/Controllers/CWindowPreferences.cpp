/*	NAME:
		CWindowPreferences.cpp

	DESCRIPTION:
		NanoWeb preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NApplication.h"

#include "NanoWeb Build.h"
#include "CApplication.h"
#include "CWindowPreferences.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const PrefPanelInfo kPanelGeneral			= { "PrefsGeneral",  kImageToolbarPrefsGeneral,  kStringToolbarPrefsGeneral  };





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(CWindowPreferences, kAppNibFile, "Preferences");





//============================================================================
//		CWindowPreferences::CWindowPreferences : Constructor.
//----------------------------------------------------------------------------
CWindowPreferences::CWindowPreferences(void)
{
}





//============================================================================
//		CWindowPreferences::~CWindowPreferences : Destructor.
//----------------------------------------------------------------------------
CWindowPreferences::~CWindowPreferences(void)
{
}





//============================================================================
//		CWindowPreferences::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowPreferences::InitializeSelf(void)
{	PrefPanelInfoList	theList;



	// Build the panels
	theList.push_back(kPanelGeneral);



	// Initialize ourselves
	SetPanelNib(NApplication::GetApp()->GetNib());
	SetPanels(theList);
	
	return(noErr);
}





//============================================================================
//		CWindowPreferences::DoEventTextAccepted : Handle kEventTextAccepted.
//----------------------------------------------------------------------------
OSStatus CWindowPreferences::DoEventTextAccepted(NCarbonEvent &/*theEvent*/)
{


	// Update the preferences
	NCFPreferences::GetPrefs()->SetValueString(kPrefHomePageKey, mHomePage->GetTextValue());
	
	return(eventNotHandledErr);
}





//============================================================================
//		CWindowPreferences::DoPanelChanged : The panel has changed.
//----------------------------------------------------------------------------
void CWindowPreferences::DoPanelChanged(void)
{	EventTypeSpec		theEvents[] = {	{ kEventClassTextField, kEventTextAccepted	},
										{ kEventClassNone,      kEventKindNone		} };



	// Update the base class
	NWindowPreferences::DoPanelChanged();



	// Register for events
	mHomePage->AddEvents(theEvents);
	mHomePage->SetEventDelegate(this);
}





//============================================================================
//		CWindowPreferences::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowPreferences::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	bool	changedAll;



	// Update the UI
	changedAll = (theKey == kPrefChangedAllKey);

	if (mHomePage != NULL)
		{
		if (changedAll || theKey == kPrefHomePageKey)
			mHomePage->SetTextValue(thePrefs->GetValueString(kPrefHomePageKey));
		}
}



