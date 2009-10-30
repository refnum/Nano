/*	NAME:
		NCommandDialog.h

	DESCRIPTION:
		Command dialog.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOMMANDDIALOG_HDR
#define NCOMMANDDIALOG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowController.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCommandDialog : public NWindowController {
public:
										NCommandDialog(void);
	virtual								~NCommandDialog(void);


	// Get/set the close command
	//
	// The "close command" is returned if the user attempts to close the window.
	//
	// The window will not be closed, to allow the caller to hide or destroy it
	// as appropriate when the dialog returns.
	UInt32								GetCloseCommand(void) const;
	void								SetCloseCommand(UInt32 theCmd);


	// Wait for a command
	//
	// Runs the dialog in a modal event loop until a command is received.
	UInt32								WaitForCommand(void);


protected:
	// Handle events
	virtual OSStatus					DoEventCommandProcess(NCarbonEvent &theEvent);


	// Handle commands
	virtual OSStatus					CommandClose(void);


private:
	void								FinishDialog(UInt32 theCmd);
	

private:
	UInt32								mCloseCommand;
	UInt32								mLastCommand;
};





#endif // NCOMMANDDIALOG_HDR


