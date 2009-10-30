/*	NAME:
		CWindowPreferences.cpp

	DESCRIPTION:
		NanoWeb preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NApplication.h"
#include "NCFBundle.h"

#include "NanoWeb Build.h"
#include "CApplication.h"
#include "CWindowPreferences.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const PanelInfo kPanelGeneral			= { "PrefsGeneral",  kImageToolbarPrefsGeneral,  kStringToolbarPrefsGeneral  };
static const PanelInfo kPanelUpdate				= { "PrefsUpdate",   kBundleIDSoftwareUpdate,    kStringToolbarPrefsUpdate   };





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
{	PanelInfoList	theList;



	// Build the panels
	theList.push_back(kPanelGeneral);
	theList.push_back(kPanelUpdate);



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



	// Configure the panel
	if (mHomePage != NULL)
		{
		mHomePage->AddEvents(theEvents);
		mHomePage->SetEventDelegate(this);
	
		GetWindow()->SetKeyboardFocus(mHomePage);
		}
}





//============================================================================
//		CWindowPreferences::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowPreferences::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	EventTime		theInterval;
	bool			changedAll;



	// Update the UI
	changedAll = (theKey == kPrefChangedAllKey);

	if (mHomePage != NULL)
		{
		if (changedAll || theKey == kPrefHomePageKey)
			mHomePage->SetTextValue(thePrefs->GetValueString(kPrefHomePageKey));
		}
	
	if (mSoftwareUpdateInterval != NULL)
		{
		if (changedAll || theKey == kSUCheckIntervalKey)
			{
			theInterval = thePrefs->GetValueFloat64(kSUCheckIntervalKey);

			if		(theInterval >= kEventDurationMonth)	mSoftwareUpdateInterval->SetCurrentCommand(kCmdSoftwareUpdateIntervalMonthly);
			else if (theInterval >= kEventDurationWeek)		mSoftwareUpdateInterval->SetCurrentCommand(kCmdSoftwareUpdateIntervalWeekly);
			else if (theInterval >= kEventDurationDay)		mSoftwareUpdateInterval->SetCurrentCommand(kCmdSoftwareUpdateIntervalDaily);
			else											mSoftwareUpdateInterval->SetCurrentCommand(kCmdSoftwareUpdateIntervalNever);
			}
		}

	if (mSoftwareUpdateLastCheck != NULL)
		{
		if ((changedAll || theKey == kSULastCheckedKey) && thePrefs->HasKey(kSULastCheckedKey, false))
			mSoftwareUpdateLastCheck->SetTextValue(thePrefs->GetValueDate(kSULastCheckedKey).GetAsString(kCFDateFormatterFullStyle));
		}
}



