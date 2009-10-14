/*	NAME:
		NWindowPanels.h

	DESCRIPTION:
		Multi-panel window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWINDOWPANELS_HDR
#define NWINDOWPANELS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NHIToolbar.h"
#include "NWindowController.h"
#include "NPanelController.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Panel info
typedef struct {
	NString				resPanel;					// Nib file   resource for panel
	NString				resImage;					// App bundle resource for image
	NString				keyLabel;					// Localized.strings key for label
} PanelInfo;


// Lists
typedef std::vector<PanelInfo>									PanelInfoList;
typedef PanelInfoList::iterator									PanelInfoListIterator;
typedef PanelInfoList::const_iterator							PanelInfoListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWindowPanels : public NWindowController, public NPanelController {
public:
										NWindowPanels(void);
	virtual								~NWindowPanels(void);


	// Set the panels
	//
	// The panel nib must be assigned before the panels can be set.
	void								SetPanels(const PanelInfoList &thePanels);


protected:
	// Handle events
	virtual OSStatus					DoEventCommandProcess(NCarbonEvent &theEvent);


	// Handle panel changes
	virtual void						DoPanelChanged(void);


private:
	PanelInfoList						mPanels;
	NHIToolbar							*mToolbar;
};





#endif // NWINDOWPANELS_HDR


