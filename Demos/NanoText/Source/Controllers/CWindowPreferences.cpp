/*	NAME:
		CWindowPreferences.cpp

	DESCRIPTION:
		NanoText preferences window.
	
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

#include "NanoText Build.h"
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
//		CWindowPreferences::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CWindowPreferences::DoMessage(BroadcastMsg theMsg, void *msgData)
{


	// Handle the message
	switch (theMsg) {
		case kMsgColorPanelChanged:
			NCFPreferences::GetPrefs()->SetValueColor(kPrefWindowBackgroundKey, mGeneralWindowBackground->GetColor());
			break;
		
		default:
			NWindowPreferences::DoMessage(theMsg, msgData);
			break;
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

	if (mGeneralWindowBackground != NULL)
		{
		if (changedAll || theKey == kPrefWindowBackgroundKey)
			mGeneralWindowBackground->SetColor(thePrefs->GetValueColor(kPrefWindowBackgroundKey));
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



