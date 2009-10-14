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
#include "NCFBundle.h"
#include "NCursor.h"
#include "NAboutBox.h"





//============================================================================
//		NAboutBox::NAboutBox : Constructor.
//----------------------------------------------------------------------------
// Layout
static const HIRect kBoundsWindow								= { {   0,   0 }, { 410, 360 } };
static const HIRect kBoundsIcon									= { { 140,  20 }, { 128, 128 } };
static const HIRect kBoundsProduct								= { {  20, 156 }, { 370,  20 } };
static const HIRect kBoundsCopyright							= { {  20, 180 }, { 370,  40 } };
static const HIRect kBoundsDescription							= { {  20, 220 }, { 370,  77 } };
static const HIRect kBoundsButton3_1							= { {  20, 320 }, { 100,  20 } };
static const HIRect kBoundsButton3_2							= { { 155, 320 }, { 100,  20 } };
static const HIRect kBoundsButton3_3							= { { 290, 320 }, { 100,  20 } };
static const HIRect kBoundsButton2_1							= { {  43, 320 }, { 100,  20 } };
static const HIRect kBoundsButton2_2							= { { 185, 320 }, { 100,  20 } };
static const HIRect kBoundsButton1_1							= { { 115, 320 }, { 100,  20 } };

static const HISize kDeltaDescription							= {  0, 95 };
static const HISize kDeltaButton								= { 80, 48 };


// Misc
static const char *kFormatProductVersion						= "%s %s";

static const WindowAttributes kAboutWindowModelessAttributes	= kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute;
static const WindowAttributes kAboutWindowAttributes			= kAboutWindowModelessAttributes  |
																  kWindowDoesNotCycleAttribute    |
																  kWindowCompositingAttribute     |
																  kWindowAsyncDragAttribute       |
																  kWindowStandardHandlerAttribute |
																  kWindowApplicationScaledAttribute;

static const ControlFontStyleRec kStyleCenterBold = { kControlUseThemeFontIDMask | kControlUseJustMask, kThemeEmphasizedSystemFont, 0, 0, 0, teCenter };
static const ControlFontStyleRec kStyleCenter     = { kControlUseThemeFontIDMask | kControlUseJustMask, kThemeApplicationFont,      0, 0, 0, teCenter };





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
//		NAboutBox::GetInstance : Get the instance.
//----------------------------------------------------------------------------
NAboutBox *NAboutBox::GetInstance(void)
{	static NAboutBox	*sAboutBox = NULL;
	
	NHIWindow			*theWindow;
	HIRect				theRect;
	OSStatus			theErr;



	// Instantiate the singleton
	if (sAboutBox == NULL)
		sAboutBox = new NAboutBox;
	
	sAboutBox->mInstance = (NWindowController **) &sAboutBox;



	// Initialize the window
	theWindow = sAboutBox->GetWindow();
	
	if (theWindow == NULL)
		{
		theRect   = NGeometryUtilities::Transform(kBoundsWindow);
		theWindow = new NHIWindow(kDocumentWindowClass, kAboutWindowAttributes, theRect);
		if (theWindow != NULL)
			{
			theErr  = SetThemeWindowBackground(*theWindow, kThemeBrushDialogBackgroundActive, true);
			theErr |= sAboutBox->Initialize(theWindow);
			NN_ASSERT_NOERR(theErr);
			}
		}



	// Get the instance
	return(sAboutBox);
}





//============================================================================
//		NAboutBox::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NAboutBox::InitializeSelf(void)
{	NString			theValue, thePrefix;
	NCFBundle		appBundle;
	NIcon			theIcon;
	


	// Initialize the properties
	//
	// The kCFBundleGetInfoStringKey key typically starts with a "MyApp 1.0, " prefix,
	// since it is visible in the Finder's Get Info window.
	//
	// To improve the appearance of the copyright string for apps that use this format,
	// we automatically strip off the redundant information.
	mProperties.SetValueString(kAboutTitleKey,     appBundle.GetInfoString(kCFBundleNameKey));
	mProperties.SetValueString(kAboutProductKey,   appBundle.GetInfoString(kCFBundleNameKey));
	mProperties.SetValueString(kAboutVersionKey,   appBundle.GetInfoString(kCFBundleVersionKey));

	theValue  = appBundle.GetInfoString(kCFBundleGetInfoStringKey);
	thePrefix = GetProductText(", ");

	if (theValue.StartsWith(thePrefix))
		theValue = theValue.GetString(thePrefix.GetSize());

	mProperties.SetValueString(kAboutCopyrightKey, theValue);



	// Get the state we need
	theIcon = appBundle.GetResource(appBundle.GetInfoString(kCFBundleIconFileKey));



	// Create the views
	mAppIcon          = new NIconView(  *GetWindow(), NGeometryUtilities::Transform(kBoundsIcon), theIcon);
	mTextProduct      = new NStaticText(*GetWindow(), NGeometryUtilities::Transform(kBoundsProduct),     "", &kStyleCenterBold);
	mTextCopyright    = new NStaticText(*GetWindow(), NGeometryUtilities::Transform(kBoundsCopyright),   "", &kStyleCenter);
	mTextDescription  = new NStaticText(*GetWindow(), NGeometryUtilities::Transform(kBoundsDescription), "", &kStyleCenter);
	mGroupDescription = new NGroupBox(  *GetWindow(), NGeometryUtilities::Transform(kBoundsDescription), "", true);
	mButton1          = new NPushButton(*GetWindow(), NGeometryUtilities::Transform(kBoundsButton3_1),   "", kAboutCommandButton1);
	mButton2          = new NPushButton(*GetWindow(), NGeometryUtilities::Transform(kBoundsButton3_2),   "", kAboutCommandButton2);
	mButton3          = new NPushButton(*GetWindow(), NGeometryUtilities::Transform(kBoundsButton3_3),   "", kAboutCommandButton3);

	mTextProduct->SetLayoutInfo(     NHIViewUtilities::GetLayoutInfo(kBindLeftRight));
	mTextCopyright->SetLayoutInfo(   NHIViewUtilities::GetLayoutInfo(kBindLeftRight));
	mTextDescription->SetLayoutInfo( NHIViewUtilities::GetLayoutInfo(kBindLeftRight));
	mGroupDescription->SetLayoutInfo(NHIViewUtilities::GetLayoutInfo(kBindLeftRight));
	mButton1->SetLayoutInfo(         NHIViewUtilities::GetLayoutInfo(kBindBottom));
	mButton2->SetLayoutInfo(         NHIViewUtilities::GetLayoutInfo(kBindBottom));
	mButton3->SetLayoutInfo(         NHIViewUtilities::GetLayoutInfo(kBindBottom));

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
	bool			hasButtons, hasDescription;
	HISize			deltaDesc, deltaButton;
	float			textHeight;
	NHIWindow		*theWindow;
	HIRect			theRect;



	// Get the state we need
	theWindow = GetWindow();

	theTitle       = mProperties.GetValueString(kAboutTitleKey);
	theDescription = mProperties.GetValueString(kAboutDescriptionKey);
	buttonOne      = mProperties.GetValueString(kAboutButton1TitleKey);
	buttonTwo      = mProperties.GetValueString(kAboutButton2TitleKey);
	buttonThree    = mProperties.GetValueString(kAboutButton3TitleKey);
	
	theValue       = buttonOne + buttonTwo + buttonThree;
	hasButtons     = theValue.IsNotEmpty();
	hasDescription = theDescription.IsNotEmpty();



	// Reset the bounds
	theWindow->SetFrame(		NGeometryUtilities::Transform(kBoundsWindow));
	mAppIcon->SetFrame(			NGeometryUtilities::Transform(kBoundsIcon));
	mTextProduct->SetFrame(		NGeometryUtilities::Transform(kBoundsProduct));
	mTextCopyright->SetFrame(	NGeometryUtilities::Transform(kBoundsCopyright));
	mTextDescription->SetFrame(	NGeometryUtilities::Transform(kBoundsDescription));
	mGroupDescription->SetFrame(NGeometryUtilities::Transform(kBoundsDescription));
	mButton1->SetFrame(			NGeometryUtilities::Transform(kBoundsButton3_1));
	mButton2->SetFrame(			NGeometryUtilities::Transform(kBoundsButton3_2));
	mButton3->SetFrame(			NGeometryUtilities::Transform(kBoundsButton3_3));



	// Apply the properties
	theWindow->SetTitle(theTitle);
	mTextProduct->SetTextValue(GetProductText());
	mTextCopyright->SetTextValue( mProperties.GetValueString(kAboutCopyrightKey));
	mTextDescription->SetTextValue(theDescription);
	mButton1->SetTextValue(buttonOne);
	mButton2->SetTextValue(buttonTwo);
	mButton3->SetTextValue(buttonThree);



	// Adjust the description
	mTextDescription->SetVisible( hasDescription);
	mGroupDescription->SetVisible(hasDescription);

	theRect    = mGroupDescription->GetFrame();
	textHeight = (float) mTextDescription->GetTextHeight();

	theRect.origin.y   += (theRect.size.height - textHeight) / 2.0f;
	theRect.size.height = textHeight;
	
	mTextDescription->SetFrame(theRect);



	// Adjust the buttons
	mButton1->SetVisible(buttonOne.IsNotEmpty());
	mButton2->SetVisible(buttonTwo.IsNotEmpty());
	mButton3->SetVisible(buttonThree.IsNotEmpty());
	
	if (buttonOne.IsNotEmpty() && buttonTwo.IsNotEmpty() && buttonThree.IsNotEmpty())
		; // Default position

	else if (buttonOne.IsNotEmpty() && buttonTwo.IsNotEmpty())
		{
		mButton1->SetFrame(NGeometryUtilities::Transform(kBoundsButton2_1));
		mButton2->SetFrame(NGeometryUtilities::Transform(kBoundsButton2_2));
		}

	else if (buttonOne.IsNotEmpty())
		mButton1->SetFrame(NGeometryUtilities::Transform(kBoundsButton1_1));



	// Adjust the window
	theRect     = NGeometryUtilities::Transform(kBoundsWindow);
	deltaDesc   = NGeometryUtilities::Transform(kDeltaDescription);
	deltaButton = NGeometryUtilities::Transform(kDeltaButton);

	if (!hasDescription)
		theRect.size.height -= deltaDesc.height;

	if (!hasButtons)
		theRect.size.height -= deltaButton.height;

	if (buttonThree.IsEmpty())
		theRect.size.width  -= deltaButton.width;

	theWindow->SetFrame(theRect);



	// Adjust the icon
	theRect          = mAppIcon->GetFrame();
	theRect.origin.x = (theWindow->GetFrame().size.width / 2.0f) - (theRect.size.width / 2.0f);

	mAppIcon->SetFrame(theRect);



	// Display the window
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

	theValue.Format(kFormatProductVersion, theProduct.GetUTF8(), theVersion.GetUTF8());
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



