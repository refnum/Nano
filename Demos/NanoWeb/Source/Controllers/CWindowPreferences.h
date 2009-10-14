/*	NAME:
		CWindowPreferences.h

	DESCRIPTION:
		NanoWeb preferences window.
	
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
#include "NEditText.h"





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


	// Handle events
	OSStatus							DoEventTextAccepted(NCarbonEvent &theEvent);


	// Handle panel changes
	void								DoPanelChanged(void);


	// Handle preference changes
	void								DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


private:
	NIB_VIEW('home', NEditText,			HomePage);
};





#endif // CWINDOWPREFERENCES_HDR


