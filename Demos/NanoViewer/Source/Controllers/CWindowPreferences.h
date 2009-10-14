/*	NAME:
		CWindowPreferences.h

	DESCRIPTION:
		NanoViewer preferences window.
	
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


	// Handle preference changes
	void								DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


private:
	NIB_VIEW('bcol', NColorPicker,		WindowBackground);
};





#endif // CWINDOWPREFERENCES_HDR


