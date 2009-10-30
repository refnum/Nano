/*	NAME:
		NCommandDialog.cpp

	DESCRIPTION:
		Command dialog.
		
		A command dialog runs a window in a modal event loop, returning control
		to the caller as commands are received from user interface controls.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCommandDialog.h"





//============================================================================
//		NCommandDialog::NCommandDialog : Constructor.
//----------------------------------------------------------------------------
NCommandDialog::NCommandDialog(void)
{


	// Initialize ourselves
	mCloseCommand = kHICommandClose;
	mLastCommand  = kHICommandNone;
}





//============================================================================
//		NCommandDialog::~NCommandDialog : Destructor.
//----------------------------------------------------------------------------
NCommandDialog::~NCommandDialog(void)
{
}





//============================================================================
//		NCommandDialog::GetCloseCommand : Get the close command.
//----------------------------------------------------------------------------
UInt32 NCommandDialog::GetCloseCommand(void) const
{


	// Get the command
	return(mCloseCommand);
}





//============================================================================
//		NCommandDialog::SetCloseCommand : Set the close command.
//----------------------------------------------------------------------------
void NCommandDialog::SetCloseCommand(UInt32 theCmd)
{


	// Set the command
	mCloseCommand = theCmd;
}





//============================================================================
//		NCommandDialog::WaitForCommand : Wait for a command from the dialog.
//----------------------------------------------------------------------------
UInt32 NCommandDialog::WaitForCommand(void)
{


	// Reset our state
	mLastCommand = kHICommandNone;



	// Run the dialog
	GetWindow()->Select();
	GetWindow()->BeginModalEventLoop();
	
	return(mLastCommand);
}





//============================================================================
//		NCommandDialog::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NCommandDialog::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();

	switch (hiCmd.commandID) {
		case kHICommandSelectWindow:
			// Ignore meta commands
			theErr = eventNotHandledErr;
			break;

		default:
			FinishDialog(hiCmd.commandID);
			theErr = NWindowController::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NCommandDialog::CommandClose : Handle a close command.
//----------------------------------------------------------------------------
OSStatus NCommandDialog::CommandClose(void)
{


	// Handle the command
	//
	// We override NWindowController's default behaviour (to delete itself), to
	// allow our caller to take the appropriate action when RunDialog returns.
	FinishDialog(mCloseCommand);
	
	return(noErr);
}





//============================================================================
//		NCommandDialog::FinishDialog : Finish the dialog.
//----------------------------------------------------------------------------
#pragma mark -
void NCommandDialog::FinishDialog(UInt32 theCmd)
{


	// Finish the dialog
	mLastCommand = theCmd;
	GetWindow()->EndModalEventLoop();
}



