/*	NAME:
		CWindowDocument.h

	DESCRIPTION:
		NanoViewer document window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CWINDOWDOCUMENT_HDR
#define CWINDOWDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIImageView.h"
#include "NDocument.h"

#include "CToolbarDocument.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CWindowDocument : public NDocument {
public:
										CWindowDocument(void);
	virtual								~CWindowDocument(void);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle events
	OSStatus							DoEventCommandProcess(     NCarbonEvent &theEvent);
	OSStatus							DoEventWindowBoundsChanged(NCarbonEvent &theEvent);


	// Get the status of a command
	CommandStatus						GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle preference changes
	void								DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


	// Load/save the document
	OSStatus							LoadSelf(void);
	OSStatus							SaveSelf(void);


	// Get the view to print
	NHIView								*GetPrintView(void);


private:
	OSStatus							CommandRotate(bool rotateLeft);
	OSStatus							CommandFlip(  bool flipHoriz);
	OSStatus							CommandActualSize(void);
	OSStatus							CommandAutoSize(void);
	OSStatus							CommandZoomToFit(void);
	OSStatus							CommandZoomIn(void);
	OSStatus							CommandZoomOut(void);
	
	OSStatus							SetZoomLevel(UInt32 theLevel);
	OSStatus							SetImage(const NCGImage &theImage, bool setModified=true);
	

private:
	NIB_VIEW('imgv', NHIImageView,		ImageView);
	CToolbarDocument					*mToolbar;
};





#endif // CWINDOWDOCUMENT_HDR


