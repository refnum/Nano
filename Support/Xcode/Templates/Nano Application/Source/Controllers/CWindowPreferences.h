/*	NAME:
		CWindowPreferences.h

	DESCRIPTION:
		ÇPROJECTNAMEÈ preferences window.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef CWINDOWPREFERENCES_HDR
#define CWINDOWPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowPreferences.h"
#include "NColorPicker.h"
#include "NSlider.h"





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
	NIB_VIEW('sint', NSlider,			GeneralExample);
	NIB_VIEW('bcol', NColorPicker,		AdvancedWindowBackground);
};





#endif // CWINDOWPREFERENCES_HDR


