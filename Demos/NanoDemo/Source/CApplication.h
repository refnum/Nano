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
#include "NProgressPie.h"
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

static const OSType kCmdDemoCursor									= 'dcur';
static const OSType kCmdDemoPoof									= 'dpof';
static const OSType kCmdDemoSplash									= 'dspl';
static const OSType kCmdDemoNotify									= 'dnot';
static const OSType kCmdDemoViews									= 'dvie';
static const OSType kCmdDemoMenuItem								= 'dmev';
static const OSType kCmdDemoLabelView								= 'dlab';
static const OSType kCmdDemoDockDefault								= 'dtap';
static const OSType kCmdDemoDockImage								= 'dtim';
static const OSType kCmdDemoDockIcon								= 'dtic';
static const OSType kCmdDemoDockView								= 'dtvw';


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
	OSStatus							CommandDemoCursor(void);
	OSStatus							CommandDemoPoof(  void);
	OSStatus							CommandDemoSplash(void);
	OSStatus							CommandDemoNotify(void);
	OSStatus							CommandDemoViews( void);
	OSStatus							CommandDemoDock(UInt32 theCmd);
	
	void								AnimateDockView(void);


private:
	NCursor								mCursor;
	NTimer								mTimer;

	NProgressPie						*mDockView;
};





#endif // CAPPLICATION_HDR


