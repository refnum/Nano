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
//		Types
//----------------------------------------------------------------------------
typedef struct {
	NString		theAuthor;
	NString		theDescription;
} DocumentInfo;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CWindowDocument : public NDocument {
public:
										CWindowDocument(void);
	virtual								~CWindowDocument(void);


	// Get/set the document info
	DocumentInfo						GetInfo(void) const;
	void								SetInfo(const DocumentInfo &theInfo);


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
	OSStatus							CommandInfo(void);
	void								SyncToolbar(void);

	void								SetColor(const NColor &theColor);


private:
	NIB_VIEW('exam', CExampleView,		ExampleView);
	CToolbarDocument					*mToolbar;
	
	DocumentInfo						mInfo;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline bool operator !=(const DocumentInfo &value1, const DocumentInfo &value2)
{
	return(value1.theAuthor      != value2.theAuthor ||
		   value1.theDescription != value2.theDescription);
}





#endif // CWINDOWDOCUMENT_HDR


