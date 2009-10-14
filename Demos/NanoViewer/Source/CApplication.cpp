/*	NAME:
		CApplication.cpp

	DESCRIPTION:
		NanoViewer application.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NPasteboard.h"

#include "NanoViewer Build.h"
#include "CWindowPreferences.h"
#include "CWindowDocument.h"
#include "CApplication.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NColor kDefaultWindowBackground					= kColorWhite;





//============================================================================
//		main : Program entry point.
//----------------------------------------------------------------------------
CREATE_NAPPLICATION(CApplication)





//============================================================================
//		CApplication::CApplication : Constructor.
//----------------------------------------------------------------------------
CApplication::CApplication(void)
{
}





//============================================================================
//		CApplication::~CApplication : Destructor.
//----------------------------------------------------------------------------
CApplication::~CApplication(void)
{
}





//============================================================================
//		CApplication::InitializeProperties : Initialize the properties.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CApplication::InitializeProperties(void)
{	NDictionary		*theProperties;
	NDictionary		theDefaults;
	OSStatus		theErr;



	// Initialize the base class
	theErr = NApplication::InitializeProperties();
	if (theErr != noErr)
		return(theErr);



	// Get the state we need
	theProperties = GetProperties();
	theDefaults   = theProperties->GetValueDictionary(kAppDefaultPrefsKey);



	// Initialize the default preferences
	theDefaults.SetValueFloat64(kSUCheckIntervalKey,      kEventDurationNever);
	theDefaults.SetValueColor(  kPrefWindowBackgroundKey, kDefaultWindowBackground);



	// Initialize the properties
	theProperties->SetValueString(		kAppNibFileKey,				kAppNibFile);
	theProperties->SetValueString(		kAppNameKey,				kAppName);
	theProperties->SetValueString(		kAppHomePageKey,			kAppHomePage);
	theProperties->SetValueBoolean(		kAppCreateDocOnReopenKey,	false);
	theProperties->SetValueDictionary(	kAppDefaultPrefsKey,		theDefaults);

	return(theErr);
}





//============================================================================
//		CApplication::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
OSStatus CApplication::InitializeSelf(void)
{	OSStatus	theErr;



	// Initialize the base class
	theErr = NApplication::InitializeSelf();
	if (theErr != noErr)
		return(theErr);



	// Initialize ourselves

	return(theErr);
}





//============================================================================
//		CApplication::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus CApplication::GetCommandStatus(const HICommandExtended &hiCmd)
{	NPasteboard		thePasteboard;
	CommandStatus	theStatus;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandNew:
			theStatus = EnableIf(thePasteboard.HasFlavor(kUTNTypeImage));
			break;
		
		default:
			theStatus = NApplication::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		CApplication::CommandPreferences : Display the preferences window.
//----------------------------------------------------------------------------
OSStatus CApplication::CommandPreferences(void)
{	CWindowPreferences		*theWindow;



	// Show the preferences
	theWindow = CWindowPreferences::GetInstance();
	theWindow->GetWindow()->Select();

	return(noErr);
}





//============================================================================
//		CApplication::CreateDocument : Create a new document.
//----------------------------------------------------------------------------
NDocument *CApplication::CreateDocument(void)
{


	// Create the document
	return(new CWindowDocument);
}


