/*	NAME:
		CWindowPreferences.cpp

	DESCRIPTION:
		NanoDemo preferences window.
	
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

#include "NanoDemo Build.h"
#include "CApplication.h"
#include "CWindowPreferences.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const PanelInfo kPanelGeneral			= { "PrefsGeneral",  kImageToolbarPrefsGeneral,  kStringToolbarPrefsGeneral  };
static const PanelInfo kPanelAdvanced			= { "PrefsAdvanced", kImageToolbarPrefsAdvanced, kStringToolbarPrefsAdvanced };
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



	// Build the panel list
	theList.push_back(kPanelGeneral);
	theList.push_back(kPanelAdvanced);
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
		case kMsgColorPickerChanged:
			NCFPreferences::GetPrefs()->SetValueColor(kPrefWindowBackgroundKey, mAdvancedWindowBackground->GetColor());
			break;
		
		default:
			NWindowPreferences::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		CWindowPreferences::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus CWindowPreferences::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd  = theEvent.GetHICommand();
	theErr = noErr;
	
	switch (hiCmd.commandID) {
		case kCmdPreferencesEdgeWidth:
			NCFPreferences::GetPrefs()->SetValueUInt32(kPrefEdgeWidthKey, mGeneralEdgeWidth->GetCurrentValue());
			break;
			
		case kCmdPreferencesEdgeStyle:
			NCFPreferences::GetPrefs()->SetValueUInt32(kPrefEdgeStyleKey, mGeneralEdgeStyle->GetCurrentValue());
			break;
			
		default:
			theErr = NWindowPreferences::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		CWindowPreferences::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowPreferences::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	bool	changedAll;



	// Update the UI
	changedAll = (theKey == kPrefChangedAllKey);

	if (mGeneralEdgeWidth != NULL)
		{
		if (changedAll || theKey == kPrefEdgeWidthKey)
			mGeneralEdgeWidth->SetCurrentValue(thePrefs->GetValueUInt32(kPrefEdgeWidthKey));
		}

	if (mGeneralEdgeStyle != NULL)
		{
		if (changedAll || theKey == kPrefEdgeStyleKey)
			mGeneralEdgeStyle->SetCurrentValue(thePrefs->GetValueUInt32(kPrefEdgeStyleKey));
		}

	if (mAdvancedWindowBackground != NULL)
		{
		if (changedAll || theKey == kPrefWindowBackgroundKey)
			mAdvancedWindowBackground->SetColor(thePrefs->GetValueColor(kPrefWindowBackgroundKey));
		}
	
	if (mSoftwareUpdateLastCheck != NULL)
		{
		if ((changedAll || theKey == kSULastCheckedKey) && thePrefs->HasKey(kSULastCheckedKey, false))
			mSoftwareUpdateLastCheck->SetTextValue(thePrefs->GetValueDate(kSULastCheckedKey).GetAsString(kCFDateFormatterFullStyle));
		}
}



