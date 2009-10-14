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
#include "NTextUtilities.h"

#include "ÇPROJECTNAMEÈ Build.h"
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



	// Initialize the default preferences
	theDefaults.SetValueColor(kPrefWindowBackgroundKey, kDefaultWindowBackground);



	// Initialize the properties
	theProperties = GetProperties();

	theProperties->SetValueString(		kAppNibFileKey,			kAppNibFile);
	theProperties->SetValueString(		kAppNameKey,			kAppName);
	theProperties->SetValueString(		kAppHomePageKey,		kAppHomePage);
	theProperties->SetValueUInt32(		kAppCreatorKey,			NTextUtilities::GetOSType(kAppCreator));
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

