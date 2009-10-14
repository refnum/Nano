/*	NAME:
		NWindowPreferences.h

	DESCRIPTION:
		Preferences window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWINDOWPREFERENCES_HDR
#define NWINDOWPREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NCFPreferences.h"
#include "NWindowController.h"
#include "NPanelController.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Preference panel info
typedef struct {
	NString				resPanel;					// Nib file resource for panel
	NString				resImage;					// App bundle resource for image
	NString				keyLabel;					// Localized.strings key for label
} PrefPanelInfo;


// Lists
typedef std::vector<PrefPanelInfo>							PrefPanelInfoList;
typedef PrefPanelInfoList::iterator							PrefPanelInfoListIterator;
typedef PrefPanelInfoList::const_iterator					PrefPanelInfoListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWindowPreferences : public NWindowController, public NPanelController {
public:
										NWindowPreferences(void);
	virtual								~NWindowPreferences(void);


	// Set the panels
	//
	// The panel nib must be assigned before the panels can be set.
	void								SetPanels(const PrefPanelInfoList &thePanel);


protected:
	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle events
	virtual OSStatus					DoEventCommandProcess(NCarbonEvent &theEvent);


	// Handle panel changes
	virtual void						DoPanelChanged(void);


	// Handle preference changes
	//
	// Sub-classes should implement to update their UI when a preference changes.
	virtual void						DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


private:
	PrefPanelInfoList					mPanels;
};





#endif // NWINDOWPREFERENCES_HDR


