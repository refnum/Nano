/*	NAME:
		NWindowPreferences.h

	DESCRIPTION:
		Preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWINDOWPREFERENCES_HDR
#define NWINDOWPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFPreferences.h"
#include "NWindowPanels.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWindowPreferences : public NWindowPanels {
public:
										NWindowPreferences(void);
	virtual								~NWindowPreferences(void);


protected:
	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle panel changes
	virtual void						DoPanelChanged(void);


	// Handle preference changes
	//
	// Sub-classes should implement to update their UI when a preference changes.
	virtual void						DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);
};





#endif // NWINDOWPREFERENCES_HDR


