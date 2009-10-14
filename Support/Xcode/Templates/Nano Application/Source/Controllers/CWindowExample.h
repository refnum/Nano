/*	NAME:
		CWindowExample.h

	DESCRIPTION:
		ÇPROJECTNAMEÈ example window.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef CWINDOWEXAMPLE_HDR
#define CWINDOWEXAMPLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NProgressBar.h"
#include "NHIImageView.h"
#include "NWindowController.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CWindowExample : public NWindowController {
public:
										CWindowExample(void);
	virtual								~CWindowExample(void);


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
	OSStatus							DoExample(void);


private:
	NIB_VIEW('prog', NProgressBar,		ProgressBar);
	NIB_VIEW('imag', NHIImageView,		Image);
};





#endif // CWINDOWEXAMPLE_HDR


