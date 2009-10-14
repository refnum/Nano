/*	NAME:
		CApplication.h

	DESCRIPTION:
		NanoWeb application.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
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


// Preferences
//
// These values are archived to disk, and should never change.
static const NString kPrefHomePageKey							= "HomePage";





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


