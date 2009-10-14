/*	NAME:
		CApplication.cpp

	DESCRIPTION:
		NanoDemo application.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NSystemUtilities.h"
#include "NThemeUtilities.h"
#include "NTextUtilities.h"
#include "NLabelView.h"

#include "NanoDemo Build.h"
#include "CMenuView.h"
#include "CShapeView.h"
#include "CWindowNanoViews.h"
#include "CWindowPreferences.h"
#include "CWindowDocument.h"
#include "CApplication.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Appearance
static const EdgeWidth kDefaultEdgeWidth						= kEdgeWidth1;
static const EdgeStyle kDefaultEdgeStyle						= kEdgeStyleSolid;
static const NColor    kDefaultWindowBackground					= kColorWhite;


// Misc
static const EventTime kDockAnimateTime							= 0.03f;





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


	// Clean up
	delete mDockView;
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
	theDefaults.SetValueUInt32( kPrefEdgeWidthKey,        kDefaultEdgeWidth);
	theDefaults.SetValueUInt32( kPrefEdgeStyleKey,        kDefaultEdgeStyle);
	theDefaults.SetValueColor(  kPrefWindowBackgroundKey, kDefaultWindowBackground);



	// Initialize the properties
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
	new CMenuView( kCmdDemoMenuItem);
	new NLabelView(kCmdDemoLabelView);

	mDockView = new NProgressPie(NULL, kDockBadgeRectTR);

	return(theErr);
}





//============================================================================
//		CApplication::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus CApplication::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;



	// Get the command status
	switch (hiCmd.commandID) {
		case kCmdDemoSplash:
		case kCmdDemoCursor:
		case kCmdDemoPoof:
		case kCmdDemoViews:
		case kCmdDemoMenuItem:
		case kCmdDemoLabelView:
		case kCmdDemoDockDefault:
		case kCmdDemoDockImage:
		case kCmdDemoDockIcon:
		case kCmdDemoDockView:
			theStatus = kCommandEnable;
			break;
		
		default:
			theStatus = NApplication::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		CApplication::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus CApplication::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kCmdDemoSplash:
			theErr = CommandDemoSplash();
			break;
		
		case kCmdDemoCursor:
			theErr = CommandDemoCursor();
			break;
		
		case kCmdDemoPoof:
			theErr = CommandDemoPoof();
			break;
		
		case kCmdDemoViews:
			theErr = CommandDemoViews();
			break;

		case kCmdDemoDockDefault:
		case kCmdDemoDockImage:
		case kCmdDemoDockIcon:
		case kCmdDemoDockView:
			theErr = CommandDemoDock(hiCmd.commandID);
			break;
		
		default:
			theErr = NApplication::DoEventCommandProcess(theEvent);
			break;
		}
	
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





//============================================================================
//		CApplication::CommandDemoSplash : Display the splash screen.
//----------------------------------------------------------------------------
//		Note :	NApplication has built-in support for displaying a splash
//				screen during launch, simply by placing a "Splash" window in
//				the application .nib file.
//
//				For the purposes of this demo, we create one by hand.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CApplication::CommandDemoSplash(void)
{	NString					theStatus, theFormat;
	NWindowSplashScreen		*theWindow;
	float					n;



	// Create the window
	theWindow = CreateController<NWindowSplashScreen>(kAppNibFile, "Demo Splash");
	if (theWindow == NULL)
		return(memFullErr);



	// Perform some work
	theWindow->Show();
	theFormat = theWindow->GetStatus();
	
	for (n = 0.0f; n <= 100.0f; n += 5.0f)
		{
		theStatus.Format(theFormat, (UInt32) n);
		
		theWindow->SetStatus(theStatus);
		theWindow->SetProgress(n / 100.0f);

		usleep(50000);
		}



	// Clean up
	delete theWindow;

	return(noErr);
}





//============================================================================
//		CApplication::CommandDemoCursor : Display a large cursor.
//----------------------------------------------------------------------------
OSStatus CApplication::CommandDemoCursor(void)
{	NCGImage		theImage;



	// Initialize the cursor
	if (!mCursor.GetImage().IsValid())
		{
		theImage = NCGImage(kImageDemoCursor);
		
		mCursor.SetImage(theImage);
		mCursor.SetHotSpot(NGeometryUtilities::GetCenter(theImage.GetBounds()));
		}
		


	// Display a large cursor
	mCursor.SetCursor(kThemeImageCursor);

	return(noErr);
}





//============================================================================
//		CApplication::CommandDemoPoof : Display the poof effect.
//----------------------------------------------------------------------------
OSStatus CApplication::CommandDemoPoof(void)
{


	// Show the effect
	NThemeUtilities::ShowEffect(kThemeEffectDisappearingItem);
	
	return(noErr);
}





//============================================================================
//		CApplication::CommandDemoViews : Display the custom views.
//----------------------------------------------------------------------------
OSStatus CApplication::CommandDemoViews(void)
{	CWindowNanoViews	*theWindow;



	// Show the window
	theWindow = CWindowNanoViews::GetInstance();
	theWindow->GetWindow()->Select();

	return(noErr);
}





//============================================================================
//		CApplication::CommandDemoDock : Display the dock icons.
//----------------------------------------------------------------------------
OSStatus CApplication::CommandDemoDock(UInt32 theCmd)
{	NDockTile	theTile;



	// Reset our state
	mTimer.RemoveTimer();
	


	// Update the dock icon
	theTile = GetDockTile();
	
	switch (theCmd) {
		case kCmdDemoDockDefault:
			theTile.RefreshImage();
			break;
			
		case kCmdDemoDockImage:
			theTile.SetImage(NCGImage(kImageDemoCursor));
			break;
		
		case kCmdDemoDockIcon:
			theTile.DrawBadge(NIcon(kImageDemoIcon));
			break;

		case kCmdDemoDockView:
			mTimer.AddTimer(BindSelf(CApplication::AnimateDockView), 0.0f, kDockAnimateTime);
			break;
		
		default:
			NN_LOG("Unknown command: %d", theCmd);
			break;
		}
	
	return(noErr);
}





//============================================================================
//		CApplication::AnimateDockView : Animate the dock view.
//----------------------------------------------------------------------------
void CApplication::AnimateDockView(void)
{	SInt32		theValue;



	// Loop the progress
	theValue = mDockView->GetCurrentValue() + 1;

	if (theValue > 100)
		theValue = 0;



	// Update the dock tile
	mDockView->SetCurrentValue(theValue);
	GetDockTile().DrawBadge(mDockView);
}




