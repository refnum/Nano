/*	NAME:
		NWindowPreferences.cpp

	DESCRIPTION:
		Preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowPreferences.h"





//============================================================================
//		NWindowPreferences::NWindowPreferences : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NWindowPreferences::NWindowPreferences(void)
{


	// Register for events
	NCFPreferences::GetPrefs()->AddListener(this);
}





//============================================================================
//		NWindowPreferences::~NWindowPreferences : Destructor.
//----------------------------------------------------------------------------
NWindowPreferences::~NWindowPreferences(void)
{
}





//============================================================================
//		NWindowPreferences::DoMessage : Handle messages.
//----------------------------------------------------------------------------
#pragma mark -
void NWindowPreferences::DoMessage(BroadcastMsg theMsg, void *msgData)
{	const NString	*msgString = (const NString *) msgData;



	// Handle the message
	switch (theMsg) {
		case kMsgPreferenceValueChanged:
			DoPrefChanged(NCFPreferences::GetPrefs(), *msgString);
			break;

		default:
			NWindowPanels::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		NWindowPreferences::DoPanelChanged : The panel has changed.
//----------------------------------------------------------------------------
void NWindowPreferences::DoPanelChanged(void)
{


	// Update the base class
	NWindowPanels::DoPanelChanged();



	// Update our state
	//
	// The base class will resolve any NIBViews in the new panel, so we then
	// perform a forced update of the preferences to bring the UI in sync.
	DoPrefChanged(NCFPreferences::GetPrefs(), kPrefChangedAllKey);
}





//============================================================================
//		NWindowPreferences::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void NWindowPreferences::DoPrefChanged(NCFPreferences */*thePrefs*/, const NString &/*theKey*/)
{
}



