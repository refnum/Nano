/*	NAME:
		CApplication.h

	DESCRIPTION:
		NanoDemo application.
	
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
static const OSType kCmdDocumentSetShapeTriangle					= 'trig';
static const OSType kCmdDocumentSetShapeRectangle					= 'rect';
static const OSType kCmdDocumentSetShapeCircle						= 'circ';

static const OSType kCmdPreferencesEdgeWidth						= 'widt';
static const OSType kCmdPreferencesEdgeStyle						= 'styl';


// Preferences
//
// These values are archived to disk, and should never change.
static const NString kPrefEdgeWidthKey								= "EdgeWidth";
static const NString kPrefEdgeStyleKey								= "EdgeStyle";
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


