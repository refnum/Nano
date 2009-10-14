/*	NAME:
		CApplication.h

	DESCRIPTION:
		NanoViewer application.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
static const OSType kCmdDocumentRotateLeft							= 'rotl';
static const OSType kCmdDocumentRotateRight							= 'rotr';
static const OSType kCmdDocumentFlipHorizontal						= 'flih';
static const OSType kCmdDocumentFlipVertical						= 'fliv';
static const OSType kCmdDocumentActualSize							= 'acts';
static const OSType kCmdDocumentAutoSize							= 'auts';
static const OSType kCmdDocumentZoomToFit							= 'zfit';
static const OSType kCmdDocumentZoomIn								= 'zoin';
static const OSType kCmdDocumentZoomIn_Hidden						= 'ZOIN';
static const OSType kCmdDocumentZoomOut								= 'zout';


// Preferences
//
// These values are archived to disk, and should never change.
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


	// Get the status of a command
	CommandStatus						GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle commands
	OSStatus							CommandPreferences(void);


	// Create a new document
	NDocument							*CreateDocument(void);


private:


};





#endif // CAPPLICATION_HDR


