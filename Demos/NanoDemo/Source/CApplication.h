/*	NAME:
		CApplication.h

	DESCRIPTION:
		NanoDemo application.
	
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
#include "NCursor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Commands
static const OSType kCmdDocumentSetShapeTriangle					= 'trig';
static const OSType kCmdDocumentSetShapeRectangle					= 'rect';
static const OSType kCmdDocumentSetShapeCircle						= 'circ';

static const OSType kCmdPreferencesEdgeWidth						= 'widt';
static const OSType kCmdPreferencesEdgeStyle						= 'styl';

static const OSType kCmdDemoDemoSplash								= 'dspl';
static const OSType kCmdDemoDemoCursor								= 'dcur';
static const OSType kCmdDemoDemoPoof								= 'dpof';
static const OSType kCmdDemoDemoViews								= 'dvie';
static const OSType kCmdDemoMenuItem								= 'dmev';
static const OSType kCmdDemoLabelView								= 'dlab';


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


	// Get the status of a command
	CommandStatus						GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle events
	OSStatus							DoEventCommandProcess(NCarbonEvent &theEvent);


	// Handle commands
	OSStatus							CommandPreferences(void);


	// Create a new document
	NDocument							*CreateDocument(void);


private:
	OSStatus							CommandDemoSplash(void);
	OSStatus							CommandDemoCursor(void);
	OSStatus							CommandDemoPoof(  void);
	OSStatus							CommandDemoViews( void);


private:
	NCursor								mCursor;
};





#endif // CAPPLICATION_HDR


