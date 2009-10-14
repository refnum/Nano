/*	NAME:
		CApplication.cpp

	DESCRIPTION:
		NanoDemo application.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"

#include "NanoDemo Build.h"
#include "CShapeView.h"
#include "CWindowPreferences.h"
#include "CWindowDocument.h"
#include "CApplication.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const EdgeWidth kDefaultEdgeWidth						= kEdgeWidth1;
static const EdgeStyle kDefaultEdgeStyle						= kEdgeStyleSolid;
static const NColor    kDefaultWindowBackground					= kColorWhite;





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
	theDefaults.SetValueUInt32(kPrefEdgeWidthKey,        kDefaultEdgeWidth);
	theDefaults.SetValueUInt32(kPrefEdgeStyleKey,        kDefaultEdgeStyle);
	theDefaults.SetValueColor( kPrefWindowBackgroundKey, kDefaultWindowBackground);



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

