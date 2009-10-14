/*	NAME:
		NAboutBox.cpp

	DESCRIPTION:
		Nano about box.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLaunchServicesUtilities.h"
#include "NHIViewUtilities.h"
#include "Nano Build.h"
#include "NCFBundle.h"
#include "NCursor.h"
#include "NAboutBox.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const WindowAttributes kAboutWindowModelessAttributes	= kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute;
static const WindowAttributes kAboutWindowAttributes			= kAboutWindowModelessAttributes  |
																  kWindowDoesNotCycleAttribute    |
																  kWindowCompositingAttribute     |
																  kWindowAsyncDragAttribute       |
																  kWindowStandardHandlerAttribute |
																  kWindowApplicationScaledAttribute;





//============================================================================
//		Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(NAboutBox, kNanoNibFile, "About");





//============================================================================
//		NAboutBox::NAboutBox : Constructor.
//----------------------------------------------------------------------------
NAboutBox::NAboutBox(void)
{
}





//============================================================================
//		NAboutBox::~NAboutBox : Destructor.
//----------------------------------------------------------------------------
NAboutBox::~NAboutBox(void)
{
}





//============================================================================
//		NAboutBox::DoDialog : Do the dialog.
//----------------------------------------------------------------------------
void NAboutBox::DoDialog(void)
{	NCursor		theCursor;



	// Prepare the window
	if (!GetWindow()->IsVisible())
		PrepareWindow();



	// Bring it to the front
	theCursor.SetCursor(kThemeArrowCursor);

	GetWindow()->Select();



	// Run it as a modal dialog if necessary
	if (mProperties.GetValueBoolean(kAboutIsModalKey))
		{
		GetWindow()->BeginModalEventLoop();
		Close();
		}
}





//============================================================================
//		NAboutBox::EndDialog : End the dialog.
//----------------------------------------------------------------------------
void NAboutBox::EndDialog(void)
{


	// Validate our state
	NN_ASSERT(mProperties.GetValueBoolean(kAboutIsModalKey));



	// End the dialog
	GetWindow()->EndModalEventLoop();
}





//============================================================================
//		NAboutBox::GetProperties : Get the properties dictionary.
//----------------------------------------------------------------------------
NDictionary *NAboutBox::GetProperties(void)
{


	// Get the properties
	return(&mProperties);
}





//============================================================================
//		NAboutBox::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NAboutBox::InitializeSelf(void)
{	NString			theCopyright, thePrefix;
	NCFBundle		appBundle;



	// Initialize the properties
	mProperties.SetValueString(kAboutTitleKey,     appBundle.GetInfoString(kCFBundleNameKey));
	mProperties.SetValueString(kAboutProductKey,   appBundle.GetInfoString(kCFBundleNameKey));
	mProperties.SetValueString(kAboutVersionKey,   appBundle.GetInfoString(kCFBundleVersionKey));
	mProperties.SetValueString(kAboutCopyrightKey, appBundle.GetInfoString(kCFBundleGetInfoStringKey));



	// Adjust the copyright
	//
	// The kCFBundleGetInfoStringKey key often starts with a "MyApp 1.0, " prefix,
	// since it is visible in the Finder's Get Info window.
	//
	// Stripping off this redundant prefix provides a better appearance.
	theCopyright = mProperties.GetValueString(kAboutCopyrightKey);
	thePrefix    = GetProductText(", ");

	if (theCopyright.StartsWith(thePrefix))
		mProperties.SetValueString(kAboutCopyrightKey, theCopyright.GetString(thePrefix.GetSize()));

	return(noErr);
}





//============================================================================
//		NAboutBox::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NAboutBox::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kAboutCommandButton1:
			theErr = DoAboutButton(mProperties.GetValueString(kAboutButton1URLKey));
			break;

		case kAboutCommandButton2:
			theErr = DoAboutButton(mProperties.GetValueString(kAboutButton2URLKey));
			break;

		case kAboutCommandButton3:
			theErr = DoAboutButton(mProperties.GetValueString(kAboutButton3URLKey));
			break;

		default:
			theErr = NWindowController::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NAboutBox::PrepareWindow : Prepare the window.
//----------------------------------------------------------------------------
#pragma mark -
void NAboutBox::PrepareWindow(void)
{	NString			theTitle, theDescription, buttonOne, buttonTwo, buttonThree, theValue;
	NHIWindow		*theWindow;
	NCFBundle		appBundle;



	// Get the state we need
	theWindow = GetWindow();

	theTitle       = mProperties.GetValueString(kAboutTitleKey);
	theDescription = mProperties.GetValueString(kAboutDescriptionKey);
	buttonOne      = mProperties.GetValueString(kAboutButton1TitleKey);
	buttonTwo      = mProperties.GetValueString(kAboutButton2TitleKey);
	buttonThree    = mProperties.GetValueString(kAboutButton3TitleKey);



	// Apply the properties
	theWindow->SetTitle(theTitle);
	mTextProduct->SetTextValue(GetProductText());
	mTextCopyright->SetTextValue(mProperties.GetValueString(kAboutCopyrightKey));
	mTextDescription->SetTextValue(theDescription);
	mButton1->SetTextValue(buttonOne);
	mButton2->SetTextValue(buttonTwo);
	mButton3->SetTextValue(buttonThree);



	// Configure the views
	mAppIcon->SetIcon(appBundle.GetInfoString(kCFBundleIconFileKey));
	mButton1->SetVisible(buttonOne.IsNotEmpty());
	mButton2->SetVisible(buttonTwo.IsNotEmpty());
	mButton3->SetVisible(buttonThree.IsNotEmpty());



	// Configure the window
	theWindow->SetAttributes( kAboutWindowModelessAttributes, kWindowInWindowMenuAttribute);
	theWindow->SetFeatures(   kWindowNoFeatures,              kWindowIsModal);
	theWindow->SetWindowGroup(kDocumentWindowClass);
	
	if (mProperties.GetValueBoolean(kAboutIsModalKey))
		{
		theWindow->SetAttributes( kWindowNoAttributes, kAboutWindowModelessAttributes);
		theWindow->SetFeatures(   kWindowIsModal,      kWindowNoFeatures);
		theWindow->SetWindowGroup(kMovableModalWindowClass);
		}

	theWindow->SetPosition(kWindowAlertPositionOnMainScreen);
}





//============================================================================
//		NAboutBox::GetProductText : Get the "product version" string.
//----------------------------------------------------------------------------
NString NAboutBox::GetProductText(const NString &theSuffix)
{	NString		theProduct, theVersion, theValue;



	// Get the "product version" string
	theProduct = mProperties.GetValueString(kAboutProductKey);
	theVersion = mProperties.GetValueString(kAboutVersionKey);

	theValue.Format("%@ %@", theProduct, theVersion);
	theValue += theSuffix;

	return(theValue);
}





//============================================================================
//		NAboutBox::DoAboutButton : Handle an about button.
//----------------------------------------------------------------------------
OSStatus NAboutBox::DoAboutButton(const NString &theURL)
{	OSStatus	theErr;



	// Handle the button
	//
	// If a URL has been assigned to the button then we handle it, otherwise
	// we allow the command to propogate back up to the application.
	theErr = eventNotHandledErr;
	
	if (theURL.IsNotEmpty())
		theErr = NLaunchServicesUtilities::OpenURL(theURL);
	
	return(theErr);
}



