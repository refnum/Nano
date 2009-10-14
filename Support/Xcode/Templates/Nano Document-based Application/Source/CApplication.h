/*	NAME:
		CApplication.h

	DESCRIPTION:
		ÇPROJECTNAMEÈ application.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef CAPPLICATION_HDR
#define CAPPLICATION_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NApplication.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Commands
static const OSType kCmdDocumentInfo								= 'dinf';
static const OSType kCmdPreferencesExample							= 'pexa';


// Preferences
//
// These values are archived to disk, and should never change.
static const NString kPrefExamplePreferenceKey						= "ExamplePreference";
static const NString kPrefWindowBackgroundKey						= "WindowBackground";





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CApplication : public NApplication {
public:
										CApplication(void);
	virtual								~CApplication(void);


protected:
	// Initialize ourselves
	OSStatus							InitializeProperties(void);
	OSStatus							InitializeSelf(void);


	// Handle commands
	OSStatus							CommandPreferences(void);


	// Create a new document
	NDocument							*CreateDocument(void);


private:


};





#endif // CAPPLICATION_HDR


