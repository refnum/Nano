/*	NAME:
		CApplication.cpp

	DESCRIPTION:
		ÇPROJECTNAMEÈ application.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ÇPROJECTNAMEÈ Build.h"
#include "CWindowPreferences.h"
#include "CWindowExample.h"
#include "CApplication.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NColor  kDefaultWindowBackground					= kColorWhite;
static const NString kWindowExample								= "Example";





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
	theDefaults.SetValueColor(kPrefWindowBackgroundKey, kDefaultWindowBackground);



	// Initialize the properties
	theProperties->SetValueString(		kAppNibFileKey,			kAppNibFile);
	theProperties->SetValueString(		kAppNameKey,			kAppName);
	theProperties->SetValueString(		kAppHomePageKey,		kAppHomePage);
	theProperties->SetValueDictionary(	kAppDefaultPrefsKey,	theDefaults);

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
//		CApplication::DoAppleEventOpenApp : Handle kAEOpenApplication.
//----------------------------------------------------------------------------
OSStatus CApplication::DoAppleEventOpenApp(const AppleEvent &/*theEvent*/)
{	OSStatus	theErr;
	


	// Create the window
	//
	// kAEOpenApplication is sent when an app is launched without documents.
	//
	// The recommended behaviour is to create a new empty document, but as
	// we do not have any documents we create our example window instead.
	theErr = CreateWindow();
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
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
//		CApplication::CreateWindow : Create the example window.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CApplication::CreateWindow(void)
{	CWindowExample	*theWindow;



	// Create the window
	theWindow = CreateController<CWindowExample>(kAppNibFile, kWindowExample);
	if (theWindow == NULL)
		return(memFullErr);



	// And show it
	theWindow->GetWindow()->Select();

	return(noErr);
}


