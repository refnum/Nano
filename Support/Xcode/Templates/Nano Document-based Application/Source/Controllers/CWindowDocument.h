/*	NAME:
		CWindowDocument.h

	DESCRIPTION:
		ÇPROJECTNAMEÈ document window.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef CWINDOWDOCUMENT_HDR
#define CWINDOWDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDocument.h"

#include "CExampleView.h"
#include "CToolbarDocument.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CWindowDocument : public NDocument {
public:
										CWindowDocument(void);
	virtual								~CWindowDocument(void);


	// Get/set the author
	NString								GetAuthor(void) const;
	void								SetAuthor(const NString &theAuthor);


	// Get/set the description
	NString								GetDescription(void) const;
	void								SetDescription(const NString &theDescription);


protected:
	// Initialize ourselves
	OSStatus							InitializeProperties(const NDictionary *appProperties);
	OSStatus							InitializeSelf(void);


	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle events
	OSStatus							DoEventCommandProcess(NCarbonEvent &theEvent);


	// Get the status of a command
	CommandStatus						GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle preference changes
	void								DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


	// Encode/decode the properties
	OSStatus							EncodeProperties(      NDictionary &theProperties);
	OSStatus							DecodeProperties(const NDictionary &theProperties);


	// Get the view to print
	NHIView								*GetPrintView(void);


private:
	void								SetColor(void);
	OSStatus							DoInfo(  void);
	
	void								SyncToolbar(void);


private:
	NIB_VIEW('exam', CExampleView,		ExampleView);
	CToolbarDocument					*mToolbar;
	
	NString								mAuthor;
	NString								mDescription;
};





#endif // CWINDOWDOCUMENT_HDR


