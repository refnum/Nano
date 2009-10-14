/*	NAME:
		CWindowPreferences.h

	DESCRIPTION:
		NanoDemo preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CWINDOWPREFERENCES_HDR
#define CWINDOWPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowPreferences.h"
#include "NColorPicker.h"
#include "NPopupButton.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CWindowPreferences : public NWindowPreferences {
public:
										DECLARE_WINDOW_CONTROLLER(CWindowPreferences);

										CWindowPreferences(void);
	virtual								~CWindowPreferences(void);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle events
	OSStatus							DoEventCommandProcess(NCarbonEvent &theEvent);


	// Handle preference changes
	void								DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


private:
	NIB_VIEW('widt', NPopupButton,		GeneralEdgeWidth);
	NIB_VIEW('styl', NPopupButton,		GeneralEdgeStyle);
	NIB_VIEW('bcol', NColorPicker,		AdvancedWindowBackground);
};





#endif // CWINDOWPREFERENCES_HDR


