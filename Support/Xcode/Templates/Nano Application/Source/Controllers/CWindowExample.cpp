/*	NAME:
		CWindowExample.cpp

	DESCRIPTION:
		ÇPROJECTNAMEÈ example window.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ÇPROJECTNAMEÈ Build.h"
#include "CApplication.h"
#include "CWindowExample.h"





//============================================================================
//		CWindowExample::CWindowExample : Constructor.
//----------------------------------------------------------------------------
CWindowExample::CWindowExample(void)
{
}





//============================================================================
//		CWindowExample::~CWindowExample : Destructor.
//----------------------------------------------------------------------------
CWindowExample::~CWindowExample(void)
{
}





//============================================================================
//		CWindowExample::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
OSStatus CWindowExample::InitializeSelf(void)
{


	// Register for events
	NCFPreferences::GetPrefs()->AddListener(this);



	// Prepare the window
	DoPrefChanged(NCFPreferences::GetPrefs(), kPrefChangedAllKey);

	return(noErr);
}





//============================================================================
//		CWindowExample::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CWindowExample::DoMessage(BroadcastMsg theMsg, void *msgData)
{	const NString		*prefsKey = (const NString *) msgData;



	// Handle the message
	switch (theMsg) {
		case kMsgPreferenceValueChanged:
			DoPrefChanged(NCFPreferences::GetPrefs(), *prefsKey);
			break;
		
		default:
			NWindowController::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		CWindowExample::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus CWindowExample::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kCmdExampleButton:
			theErr = DoExample();
			break;

		default:
			theErr = NWindowController::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		CWindowExample::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowExample::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	bool	changedAll;



	// Update our state
	changedAll = (theKey == kPrefChangedAllKey);
	
	if (changedAll || theKey == kPrefWindowBackgroundKey)
		GetWindow()->SetContentColor(thePrefs->GetValueColor(kPrefWindowBackgroundKey));
}





//============================================================================
//		CWindowExample::DoExample : Handle kCmdExampleButton.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowExample::DoExample(void)
{


	// Handle the command
	//
	// Windows typically need to respond to commands, which they receive
	// by overriding DoEventCommandProcess.
	//
	//
	// Note that since we don't need access to the push button that sent
	// this command, we do not need to declare it with NIB_VIEW.
	//
	// Only views that we need to manipulate, or which have properties to be
	// applied when the window is created, need to be declared with NIB_VIEW.
	mProgressBar->SetIndeterminate(!mProgressBar->IsIndeterminate());
	
	return(noErr);
}



