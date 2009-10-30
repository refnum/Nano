/*	NAME:
		NApplication.cpp

	DESCRIPTION:
		Application object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "Nano Build.h"
#include "NFileUtilities.h"
#include "NSystemUtilities.h"
#include "NTextUtilities.h"
#include "NHelpUtilities.h"
#include "NIBUtilities.h"
#include "NColorPanel.h"
#include "NFontPanel.h"
#include "NCFBundle.h"
#include "NAboutBox.h"
#include "NApplication.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Times
static const EventTime kSplashTimeMinimum								= 2.0f;
static const EventTime kSoftwareUpdateCheckTime							= kEventDurationHour;





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NApplication *NApplication::mApp;





//============================================================================
//		NApplication::NApplication : Constructor.
//----------------------------------------------------------------------------
NApplication::NApplication(void)
{


	// Validate our state
	NN_ASSERT(mApp == NULL);



	// Initialize ourselves
	mState = kAppStateStarting;
	
	mSplashStart  = 0.0f;
	mSplashScreen = NULL;
	
	mApp = this;
}





//============================================================================
//		NApplication::~NApplication : Destructor.
//----------------------------------------------------------------------------
NApplication::~NApplication(void)
{


	// Validate the global state
	NN_ASSERT(mApp == this);



	// Clean up
	DisposeNibReference(mNib);



	// Reset the global state
	mApp = NULL;
}





//============================================================================
//		NApplication::GetApp : Get the application.
//----------------------------------------------------------------------------
NApplication *NApplication::GetApp(void)
{


	// Validate our state
	NN_ASSERT(mApp != NULL);



	// Get the app
	return(mApp);
}





//============================================================================
//		NApplication::Run : Run the application.
//----------------------------------------------------------------------------
void NApplication::Run(void)
{	OSStatus	theErr;



	// Initialize the application
	theErr = InitializeProperties();
	
	if (theErr == noErr)
		theErr = InitializeBegin();

	if (theErr == noErr)
		theErr = InitializeMenuBar();

	if (theErr == noErr)
		theErr = InitializeSelf();

	if (theErr == noErr)
		theErr = InitializeEnd();



	// Run the application
	mState = kAppStateRunning;

	if (theErr == noErr)
		RunApplicationEventLoop();



	// Quit the application
	mState = kAppStateQuitting;

	ShutDown();
}





//============================================================================
//		NApplication::Quit : Quit the application.
//----------------------------------------------------------------------------
void NApplication::Quit(void)
{	NAppleEvent		theEvent(kCoreEventClass, kAEQuitApplication);
	OSStatus		theErr;



	// Quit the app
	theErr = theEvent.Send();
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NApplication::GetProperties : Get the properties dictionary.
//----------------------------------------------------------------------------
NDictionary *NApplication::GetProperties(void)
{


	// Get the properties
	return(&mProperties);
}





//============================================================================
//		NApplication::GetState : Get the application state.
//----------------------------------------------------------------------------
AppState NApplication::GetState(void) const
{


	// Get the state
	return(mState);
}





//============================================================================
//		NApplication::GetNib : Get the application nib.
//----------------------------------------------------------------------------
IBNibRef NApplication::GetNib(void) const
{


	// Get the nib
	return(mNib);
}





//============================================================================
//		NApplication::GetRecentItems : Get the recent items menu.
//----------------------------------------------------------------------------
NRecentItems *NApplication::GetRecentItems(void)
{


	// Get the recent items menu
	return(&mRecentItems);
}





//============================================================================
//		NApplication::GetDockTile : Get the Dock tile.
//----------------------------------------------------------------------------
NDockTile NApplication::GetDockTile(void) const
{	NDockTile		theTile;



	// Get the tile
	return(theTile);
}





//============================================================================
//		NApplication::ContinueReview : Continue a review of modified docs.
//----------------------------------------------------------------------------
void NApplication::ContinueReview(void)
{	NDocumentList	theDocuments;
	NDocument		*theDocument;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(mState == kAppStateQuitReviewSingle || mState == kAppStateQuitReviewMultiple);



	// Continue a single review
	//
	// If a single document was reviewed, the app can now quit.
	if (mState == kAppStateQuitReviewSingle)
		Quit();



	// Continue a multiple review
	//
	// If multiple documents are being reviewed, the first document is asked to
	// close. Once it completes, it will ask us to continue the review - until
	// eventually all will be closed, and we can quit.
	else
		{
		// Close the first modified document
		theDocuments = NDocument::GetDocuments(true);
		if (!theDocuments.empty())
			{
			// Get the document
			theDocument = theDocuments.front();
			NN_ASSERT(theDocument->IsModified());


			// Close it
			theEvent = NCarbonEvent::CreateCommand(kHICommandClose);
			theErr   = theDocument->DispatchEvent(theEvent);
			NN_ASSERT_NOERR(theErr);
			}
		
		
		// Or quit the app
		else
			Quit();
		}
}





//============================================================================
//		NApplication::CancelReview : Cancel the review of modified docs.
//----------------------------------------------------------------------------
void NApplication::CancelReview(void)
{	NCarbonEvent	theEvent;



	// Validate our state
	NN_ASSERT(mState == kAppStateQuitReviewSingle || mState == kAppStateQuitReviewMultiple);



	// Cancel the review
	//
	// If we were reviewing multiple documents, then multiple windows may
	// have a sheet attached to them.
	//
	// This could arise because the user switched between documents during
	// a review (triggering a sheet on each newly activated document), or
	// because they asked to quit/continued working/asked to quit.
	if (mState == kAppStateQuitReviewMultiple)
		{
		theEvent = NCarbonEvent::CreateCommand(kHICommandCancel);
		SendEventToDocuments(theEvent, true);
		}



	// Reset our state
	mState = kAppStateRunning;
}





//============================================================================
//		NApplication::InitializeProperties : Initialize the properties.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NApplication::InitializeProperties(void)
{	NDictionary		theDefaults;
	NCFBundle		appBundle;



	// Initialize the default preferences
	theDefaults.SetValueBoolean(kSUCheckEnabledKey,  true);
	theDefaults.SetValueFloat64(kSUCheckIntervalKey, kEventDurationDay);



	// Initialize the properties
	mProperties.SetValueString(		kAppNibFileKey,				"Application");
	mProperties.SetValueString(		kAppNibMenuBarKey,			"MenuBar");
	mProperties.SetValueString(		kAppNibHelpMenuKey,			"Help");
	mProperties.SetValueString(		kAppNibSplashScreenKey,		"Splash");
	mProperties.SetValueString(		kAppNameKey,				"Nano App");
	mProperties.SetValueString(		kAppHomePageKey,			"http://www.refnum.com/products/nano/");
	mProperties.SetValueUInt32(		kAppCreatorKey,				NTextUtilities::GetOSType(appBundle.GetInfoString(kCFBundleSignatureKey)));
	mProperties.SetValueArray(		kAppFormatUTIsKey,			appBundle.GetInfoUTIs());
	mProperties.SetValueBoolean(	kAppCreateDocOnOpenKey,		true);
	mProperties.SetValueBoolean(	kAppCreateDocOnReopenKey,	true);
	mProperties.SetValueDictionary(	kAppDefaultPrefsKey,		theDefaults);

	return(noErr);
}





//============================================================================
//		NApplication::InitializeBegin : Begin initialization.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeBegin(void)
{	EventTypeSpec		carbonEvents[] = {	{ kEventClassApplication,	kEventAppActivated			},
											{ kEventClassCommand,		kEventCommandProcess		},
											{ kEventClassCommand,		kEventCommandUpdateStatus	},
											{ kEventClassNApplication,	kEventNAppGetUndoManager	},
											{ kEventClassNone,			kEventKindNone				} };
	EventTypeSpec		appleEvents[] = {	{ kCoreEventClass,	kAEOpenApplication		},
											{ kCoreEventClass,	kAEOpenDocuments		},
											{ kCoreEventClass,	kAEPrintDocuments		},
											{ kCoreEventClass,	kAEQuitApplication		},
											{ kCoreEventClass,	kAEReopenApplication	},
											{ kEventClassNone,	kEventKindNone			} };
	NDictionary			theDefaults;
	NCFBundle			appBundle;
	FSRef				theFSRef;
	NString				nibName;
	OSStatus			theErr;
	bool				wasOK;



	// Initialize AppKit
	wasOK = NSApplicationLoad();
	NN_ASSERT(wasOK);



	// Initialize QuickTime
	theErr = EnterMovies();
	NN_ASSERT_NOERR(theErr);



	// Initialize the preferences
	theDefaults = mProperties.GetValueDictionary(kAppDefaultPrefsKey);
	if (theDefaults.IsValid())
		NCFPreferences::GetPrefs()->SetDefaults(theDefaults);



	// Create the nib reference
	nibName = mProperties.GetValueString(kAppNibFileKey);
	mNib    = NIBUtilities::CreateNib(nibName);



	// Show the splash screen
	ShowSplashScreen();



	// Register for events
	theErr  = NCarbonEventHandler::InstallHandler(carbonEvents);
	theErr |=  NAppleEventHandler::InstallHandler( appleEvents);
	NN_ASSERT_NOERR(theErr);
	
	SetEventDelegate(&mUndoManager);



	// Register the help book
	if (appBundle.GetInfoString(kCFBundleHelpBookNameKey).IsNotEmpty())
		{
		theFSRef = appBundle.GetFile().GetFSRef();
		theErr   = AHRegisterHelpBook(&theFSRef);
		NN_ASSERT_NOERR(theErr);
		}

	return(theErr);
}





//============================================================================
//		NApplication::InitializeMenuBar : Initialize the menu bar.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeMenuBar(void)
{	NString				resMenu, resHelp;
	NMenuItem			helpMenu;
	NMenu				theMenu;



	// Get the state we need
	resMenu = mProperties.GetValueString(kAppNibMenuBarKey);
	resHelp = mProperties.GetValueString(kAppNibHelpMenuKey);



	// Load the menu bar
	NIBUtilities::SetMenuBar(mNib, resMenu);
	mRecentItems.AttachMenu();



	// Load the Help menu
	//
	// Carbon applications automatically gain a Help menu that holds default commands,
	// and we expand this using an optional menu defined in the application nib file.
	if (resHelp.IsNotEmpty())
		{
		theMenu  = NIBUtilities::CreateMenu(mNib, resHelp, true);
		helpMenu = NHelpUtilities::GetHelpMenu();
		
		if (theMenu.IsValid())
			{
			NN_ASSERT(helpMenu.GetIndex() >= 1);
			helpMenu.GetMenu().CopyItems(theMenu, 1, theMenu.GetSize(), helpMenu.GetIndex() - 1);
			}
		}

	return(noErr);
}





//============================================================================
//		NApplication::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeSelf(void)
{


	// Initialize ourselves
	//
	// Sub-classes would typically override this method for their initialization.
	return(noErr);
}





//============================================================================
//		NApplication::InitializeEnd : Finish initialization.
//----------------------------------------------------------------------------
//		Note : Sub-classes can override for a hook immediately before RAEL.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeEnd(void)
{


	// Finish initialization
	mTimer.AddTimer(BindSelf(NApplication::CheckForUpdates), 0.0f, kSoftwareUpdateCheckTime);

	HideSplashScreen();

	return(noErr);
}





//============================================================================
//		NApplication::ShutDown : Shut down the app.
//----------------------------------------------------------------------------
void NApplication::ShutDown(void)
{


	// Shut down the app
	CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
}





//============================================================================
//		NApplication::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void NApplication::DoMessage(BroadcastMsg /*theMsg*/, void */*msgData*/)
{


	// Ignore the message
	//
	// We derive from NMsgListener so that we can listen to objects such as
	// our file dialog, but since sub-classes may not use this feature (e.g.,
	// if they don't need to process filter messages) we need to provide an
	// implementation of the otherwise pure virtual method.
	//
	// Sub-classes that override this method should call through with unknown
	// messages, in case we need to listen to our own messages in the future.
}





//============================================================================
//		NApplication::DoEventAppActivated : Handle kEventAppActivated.
//----------------------------------------------------------------------------
OSStatus NApplication::DoEventAppActivated(NCarbonEvent &theEvent)
{


	// Inform our documents
	SendEventToDocuments(theEvent, false);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NApplication::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NApplication::DoEventCommandProcess(NCarbonEvent &theEvent)
{	NDocument			*theDocument;
	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kHICommandNew:
			theErr = CommandNew(theDocument);
			break;
		
		case kHICommandOpen:
			theErr = CommandOpen();
			break;
		
		case kHICommandAbout:
			theErr = CommandAbout();
			break;
		
		case kHICommandPreferences:
			theErr = CommandPreferences();
			break;
		
		case kCmdSoftwareUpdateCheckNow:
		case kCmdSoftwareUpdateIntervalNever:
		case kCmdSoftwareUpdateIntervalDaily:
		case kCmdSoftwareUpdateIntervalWeekly:
		case kCmdSoftwareUpdateIntervalMonthly:
			theErr = CommandSoftwareUpdate(hiCmd.commandID);
			break;
		
		case kHICommandNToggleColorPanel:
			theErr = CommandToggleColorPanel();
			break;
		
		default:
			theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NApplication::DoEventNAppGetUndoManager : Handle kEventNAppGetUndoManager.
//----------------------------------------------------------------------------
OSStatus NApplication::DoEventNAppGetUndoManager(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Get the undo manager
	theErr = theEvent.SetParameterPointer(kEventParamDirectObject, &mUndoManager);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NApplication::DoAppleEventOpenApp : Handle kAEOpenApplication.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventOpenApp(const NAppleEvent &/*theEvent*/)
{	NDocument	*theDocument;
	OSStatus	theErr;



	// Create a document
	//
	// kAEOpenApplication is sent when an app is launched without documents.
	//
	// The recommended behaviour is to create a new empty document.
	theErr = eventNotHandledErr;
	
	if (mProperties.GetValueBoolean(kAppCreateDocOnOpenKey))
		theErr = CommandNew(theDocument, NULL);
		
	return(theErr);
}





//============================================================================
//		NApplication::DoAppleEventOpenDocs : Handle kAEOpenDocuments.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventOpenDocs(const NAppleEvent &theEvent)
{	NFileList		theFiles;
	AEDescList		aeFiles;
	OSStatus		theErr;



	// Get the documents
	aeFiles  = theEvent.GetParameterAEDescList(keyDirectObject);
	theFiles = NFileUtilities::GetAEFiles(&aeFiles);
	theErr   = AEDisposeDesc(&aeFiles);



	// Create the documents
	return(CreateDocuments(theFiles, NULL));
}





//============================================================================
//		NApplication::DoAppleEventPrintDocs : Handle kAEPrintDocuments.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventPrintDocs(const NAppleEvent &theEvent)
{	NCarbonEvent	printEvent;
	NFileList		theFiles;
	AEDescList		aeFiles;
	OSStatus		theErr;



	// Get the documents
	aeFiles  = theEvent.GetParameterAEDescList(keyDirectObject);
	theFiles = NFileUtilities::GetAEFiles(&aeFiles);
	theErr   = AEDisposeDesc(&aeFiles);



	// Print the documents
	printEvent = NCarbonEvent::CreateCommand(kHICommandPrint);
	
	return(CreateDocuments(theFiles, &printEvent));
}





//============================================================================
//		NApplication::DoAppleEventQuitApp : Handle kAEQuitApplication.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventQuitApp(const NAppleEvent &/*theEvent*/)
{


	// Close the windows
	//
	// Documents may need to cancel the quit (to review changes), as may normal
	// windows (if they are performing an operation which can't be interrupted).
	if (!CloseDocuments())
		return(noErr);

	if (!CloseWindows())
		return(noErr);



	// Quit the app
	QuitApplicationEventLoop();

	return(noErr);
}





//============================================================================
//		NApplication::DoAppleEventReopenApp : Handle kAEReopenApplication.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventReopenApp(const NAppleEvent &/*theEvent*/)
{	NDocument	*theDocument;
	OSStatus	theErr;



	// Create a document
	//
	// kAEReopenApplication is sent when an app is resumed by re-double-clicking
	// the application icon or selecting the application from the dock.
	//
	// The recommended behaviour is to create a new empty document if there are
	// no open documents.
	theErr = eventNotHandledErr;
	
	if (mProperties.GetValueBoolean(kAppCreateDocOnReopenKey) && NDocument::GetDocuments().empty())
		theErr = CommandNew(theDocument);

	return(theErr);
}





//============================================================================
//		NApplication::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NApplication::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandNToggleColorPanel:
			theStatus = kCommandDefault;
			NMenuItem(hiCmd.source.menu.menuRef, hiCmd.commandID).SetText(GetColorToggle());
			break;

		case kHICommandShowHideFontPanel:
			theStatus = kCommandDefault;
			NMenuItem(hiCmd.source.menu.menuRef, hiCmd.commandID).SetText(GetFontToggle());
			break;

		default:
			theStatus = NCarbonEventHandler::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		NApplication::CommandNew : Create a new document.
//----------------------------------------------------------------------------
OSStatus NApplication::CommandNew(NDocument *&theDocument, const NFile *theFile)
{	NString		groupID;
	OSStatus	theErr;



	// Check the file
	//
	// By default we won't open the same file twice, we simply select the document
	// window and don't re-add it to the Recent Items menu.
	//
	// This is the same behaviour as NSDocumentController, where opening a document
	// that is already open does not move it to the top of the Recent Items.
	if (theFile != NULL)
		{
		theDocument = FindDocument(*theFile);
		if (theDocument != NULL)
			{
			theDocument->GetWindow()->Select();
			return(noErr);
			}
		}



	// Create the document
	theDocument = CreateDocument();
	if (theDocument == NULL)
		return(unimpErr);



	// Initialize it
	//
	// If the document fails to initialize, it will destroy itself.
	theErr = theDocument->InitializeDocument(theFile);
	NN_ASSERT_NOERR(theErr);



	// Update the recent items menu
	if (theErr == noErr && theFile != NULL)
		{
		groupID = theDocument->GetProperties()->GetValueString(kDocRecentItemsGroupKey);
		mRecentItems.AddItem(*theFile, groupID);
		}
	
	return(theErr);
}





//============================================================================
//		NApplication::CommandOpen : Open an existing document.
//----------------------------------------------------------------------------
OSStatus NApplication::CommandOpen(void)
{	NString				textButton;
	NArray				formatUTIs;
	NFileDialog			theDialog;
	FileDialogResult	theResult;
	NavUserAction		navAction;



	// Get the state we need
	formatUTIs = mProperties.GetValueArray(kAppFormatUTIsKey);

	theDialog.AddListener(this);
	


	// Choose the documents
	textButton = NBundleString(kStringFileDialogOpen, "", kNanoStrings);
	navAction  = theDialog.ChooseFile("", textButton, theResult, formatUTIs);

	if (navAction == kNavUserActionCancel)
		return(userCanceledErr);



	// Create the documents
	return(CreateDocuments(theResult.theFiles, NULL));
}





//============================================================================
//		NApplication::CommandAbout : Display the about box.
//----------------------------------------------------------------------------
OSStatus NApplication::CommandAbout(void)
{	NString			aboutTitle, aboutButton;
	NDictionary		*theProperties;
	NAboutBox		*theWindow;



	// Get the window
	theWindow = NAboutBox::GetInstance();
	if (theWindow == NULL)
		return(memFullErr);



	// Get the state we need
	aboutTitle  = NBundleString(kStringAboutTitle,    "", kNanoStrings);
	aboutButton = NBundleString(kStringAboutHomePage, "", kNanoStrings);

	aboutTitle.Format(aboutTitle, mProperties.GetValueString(kAppNameKey));



	// Prepare the window properties
	//
	// The base class provides a standard Nano about box with "home page" button,
	// however sub-classes can override this method to customize their about box.
	theProperties = theWindow->GetProperties();

	theProperties->SetValueString(kAboutTitleKey,        aboutTitle);
	theProperties->SetValueString(kAboutButton1TitleKey, aboutButton);
	theProperties->SetValueString(kAboutButton1URLKey,   mProperties.GetValueString(kAppHomePageKey));



	// Run the window
	theWindow->DoDialog();
	
	return(noErr);
}





//============================================================================
//		NApplication::CommandPreferences : Display the preferences window.
//----------------------------------------------------------------------------
OSStatus NApplication::CommandPreferences(void)
{


	// Display the window
	//
	// Sub-classes should override to display their preferences window.
	//
	// A simple panel-based preferences window can be created by subclassing
	// NWindowPreferences. Assuming the sub-class was CWindowPreferences, a
	// sub-class would probably implement this method with:
	/*
		CWindowPreferences		*theWindow;


		// Show the preferences
		theWindow = CWindowPreferences::GetInstance();
		theWindow->GetWindow()->Select();

		return(noErr);
	*/

	return(eventNotHandledErr);
}





//============================================================================
//		NApplication::CommandSoftwareUpdate : Handle Software Update commands.
//----------------------------------------------------------------------------
OSStatus NApplication::CommandSoftwareUpdate(UInt32 theCmd)
{	NCFPreferences		*thePrefs;
	OSStatus			theErr;
	


	// Get the state we need
	thePrefs = NCFPreferences::GetPrefs();



	// Handle the command
	theErr = eventNotHandledErr;
	
	switch (theCmd) {
		case kCmdSoftwareUpdateCheckNow:
			theErr = mSoftwareUpdate.CheckForUpdates(false);
			break;

		case kCmdSoftwareUpdateIntervalNever:
			thePrefs->SetValueFloat64(kSUCheckIntervalKey, 0.0);
			break;

		case kCmdSoftwareUpdateIntervalDaily:
			thePrefs->SetValueFloat64(kSUCheckIntervalKey, kEventDurationDay);
			break;

		case kCmdSoftwareUpdateIntervalWeekly:
			thePrefs->SetValueFloat64(kSUCheckIntervalKey, kEventDurationWeek);
			break;

		case kCmdSoftwareUpdateIntervalMonthly:
			thePrefs->SetValueFloat64(kSUCheckIntervalKey, kEventDurationMonth);
			break;
		
		default:
			NN_LOG("Unknown command: %d", theCmd);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NApplication::CommandToggleColorPanel : Toggle the color panel.
//----------------------------------------------------------------------------
OSStatus NApplication::CommandToggleColorPanel(void)
{


	// Toggle the color panel
	NColorPanel::SetVisible(!NColorPanel::IsVisible());
	
	return(noErr);
}





//============================================================================
//		NApplication::CreateDocument : Create a new document.
//----------------------------------------------------------------------------
NDocument *NApplication::CreateDocument(void)
{


	// Create the document
	//
	// Sub-classes should override create a new instance of their document
	// class. The base class will take care of initializing the document
	// object, and handling failure.
	return(NULL);
}





//============================================================================
//		NApplication::GetSplashScreen : Get the splash screen.
//----------------------------------------------------------------------------
NWindowSplashScreen *NApplication::GetSplashScreen(void)
{


	// Get the splash screen
	return(mSplashScreen);
}





//============================================================================
//		NApplication::FindDocument : Find a document given a file.
//----------------------------------------------------------------------------
#pragma mark -
NDocument *NApplication::FindDocument(const NFile &theFile)
{	NDocumentList				theList;
	NDocumentListConstIterator	theIter;



	// Find the document
	theList = NDocument::GetDocuments();
	
	for (theIter = theList.begin(); theIter != theList.end(); theIter++)
		{
		if ((*theIter)->GetFile() == theFile)
			return(*theIter);
		}
	
	return(NULL);
}





//============================================================================
//		NApplication::CreateDocuments : Create a set of documents.
//----------------------------------------------------------------------------
OSStatus NApplication::CreateDocuments(const NFileList &theFiles, NCarbonEvent *theEvent)
{	NDocument				*theDocument;
	NFileListConstIterator	theIter;
	OSStatus				theErr;



	// Create the documents
	//
	// Once created, documents can receive an optional event.
	theErr = noErr;
	
	for (theIter = theFiles.begin(); theIter != theFiles.end(); theIter++)
		{
		theErr = CommandNew(theDocument, &(*theIter));
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == noErr && theEvent != NULL)
			{
			theErr = theDocument->DispatchEvent(*theEvent);
			NN_ASSERT_NOERR(theErr);
			}
		}
	
	return(theErr);
}





//============================================================================
//		NApplication::CloseDocuments : Close the documents.
//----------------------------------------------------------------------------
bool NApplication::CloseDocuments(void)
{	NDocument			*theDocument;
	NDocumentList		theDocuments;
	NFileDialog			fileDialog;
	NavUserAction		navAction;
	bool				didClose;
	NCarbonEvent		theEvent;
	OSStatus			theErr;



	// Initialize ourselves
	theEvent     = NCarbonEvent::CreateCommand(kHICommandClose);
	theDocuments = NDocument::GetDocuments(true);



	// Handle no modified documents
	//
	// If there aren't any modified documents, we can just close everything.
	if (theDocuments.empty())
		{
		SendEventToDocuments(theEvent, false);
		return(true);
		}
	
	
	
	// Handle one modified document
	//
	// The quit is deferred until the document continues the review.
	if (theDocuments.size() == 1)
		{
		// Get the document
		theDocument = theDocuments.front();
		NN_ASSERT(theDocument->IsModified());
		
		
		// Close it
		mState = kAppStateQuitReviewSingle;
		theErr = theDocument->DispatchEvent(theEvent);
		
		return(theErr == eventNotHandledErr);
		}



	// Handle multiple modified documents
	//
	// Each modified document is closed in turn, until no docs remain.
	navAction = fileDialog.AskReviewDocuments(theDocuments.size());
	didClose  = false;
	
	switch (navAction) {
		case kNavUserActionReviewDocuments:
			// Review the documents
			mState = kAppStateQuitReviewMultiple;
			
			SelectModifiedDocuments();
			ContinueReview();
			break;


		case kNavUserActionCancel:
			// Cancel the review
			//
			// The HIG requires that if a quit attempt produces review sheets, and
			// the user quits again then cancels, the initial sheets are removed.
			if (mState == kAppStateQuitReviewSingle || mState == kAppStateQuitReviewMultiple)
				CancelReview();
			
			
			// Cancel the quit
			mState = kAppStateRunning;
			break;


		case kNavUserActionDiscardDocuments:
			// Discard the documents
			mState   = kAppStateQuitDiscardChanges;
			didClose = true;
			
			SendEventToDocuments(theEvent, false);
			break;
		
		
		default:
			NN_LOG("Unknown nav action: %d", navAction);
			break;
		}
	
	return(didClose);
}





//============================================================================
//		NApplication::CloseWindows : Close the windows.
//----------------------------------------------------------------------------
bool NApplication::CloseWindows(void)
{	NHIWindowList					theWindows;
	NCarbonEvent					theEvent;
	HIWindowRef						hiWindow;
	NHIWindowListConstIterator		theIter;



	// Get the state we ned
	theWindows = NHIWindow::GetWindows();
	theEvent   = NCarbonEvent::CreateCommand(kHICommandClose);



	// Attempt to close the windows
	//
	// Although windows are not documents, they may need to refuse to close if they
	// can not currently be closed (e.g., if they are performing an uninterruptable
	// operation such as saving a file).
	for (theIter = theWindows.begin(); theIter != theWindows.end(); theIter++)
		{
		hiWindow = (*theIter)->GetHIWindow();
		theEvent.SendToHIWindow(hiWindow);

		if (IsValidWindowRef(hiWindow))
			return(false);
		}
	
	return(true);
}





//============================================================================
//		NApplication::SelectModifiedDocuments : Select the modified documents.
//----------------------------------------------------------------------------
void NApplication::SelectModifiedDocuments(void)
{	NDocumentList				theDocuments;
	NDocumentListConstIterator	theIter;



	// Select the modified documents
	//
	// When reviewing modified documents during a quit, the HIG requires that
	// the document windows are sorted to bring any modified documents to the
	// front.
	//
	// We do so with BringToFront to avoid activating the windows, as the HIG
	// requires that documents show their discard-changes sheet when the window
	// is activated (either as the review proceeds, or explicitly by the user
	// during a review).
	theDocuments = NDocument::GetDocuments(true);
	
	for (theIter = theDocuments.begin(); theIter != theDocuments.end(); theIter++)
		BringToFront((*theIter)->GetWindow()->GetHIWindow());
}





//============================================================================
//		NApplication::SendEventToDocuments : Send an event to the documents.
//----------------------------------------------------------------------------
void NApplication::SendEventToDocuments(NCarbonEvent &theEvent, bool onlyModified)
{	NDocumentList				theDocuments;
	NDocumentListConstIterator	theIter;
	OSStatus					theErr;



	// Send the event
	//
	// To ensure the event is processed without needing a return to the event
	// loop, we dispatch the event by hand rather than posting it.
	//
	// Although this does take us outside the normal Carbon Event dispatch
	// mechanism, this allows us to ensure that state changes (e.g., the app
	// being activated) are processed atomically.
	theDocuments = NDocument::GetDocuments(onlyModified);
	
	for (theIter = theDocuments.begin(); theIter != theDocuments.end(); theIter++)
		{
		theErr = (*theIter)->DispatchEvent(theEvent);
		NN_ASSERT(theErr == noErr || theErr == eventNotHandledErr);
		}
}





//============================================================================
//		NApplication::ShowSplashScreen : Show the splash screen.
//----------------------------------------------------------------------------
void NApplication::ShowSplashScreen(void)
{	NString		nibName, resName;
	SInt32		errorFD;



	// Get the state we need
	nibName = mProperties.GetValueString(kAppNibFileKey);
	resName = mProperties.GetValueString(kAppNibSplashScreenKey);



	// Create the splash screen
	//
	// Since the resource is optional, we suppress errors.
	if (nibName.IsNotEmpty() && resName.IsNotEmpty())
		{
		errorFD = NFileUtilities::RedirectStderr(-1);
		mSplashScreen = CreateController<NWindowSplashScreen>(nibName, resName);
		(void)    NFileUtilities::RedirectStderr(errorFD);
		}



	// And show it
	if (mSplashScreen != NULL)
		{
		mSplashStart = GetCurrentEventTime();
		mSplashScreen->Show();
		}
}





//============================================================================
//		NApplication::HideSplashScreen : Hide the splash screen.
//----------------------------------------------------------------------------
void NApplication::HideSplashScreen(void)
{	EventTime		theTime;



	// Hide the splash screen
	if (mSplashScreen != NULL)
		{
		// Enforce a minimum time
		theTime = GetCurrentEventTime() - mSplashStart;
		if (theTime < kSplashTimeMinimum)
			{
			theTime = kSplashTimeMinimum - theTime;
			usleep((useconds_t) (theTime / kEventDurationMicrosecond));
			}



		// Remove it
		mSplashScreen->Hide();
		
		delete mSplashScreen;
		mSplashScreen = NULL;
		}
}





//============================================================================
//		NApplication::CheckForUpdates : Check for updates.
//----------------------------------------------------------------------------
void NApplication::CheckForUpdates(void)
{


	// Check for updates
	if (mSoftwareUpdate.ShouldCheck())
		mSoftwareUpdate.CheckForUpdates(true);
}





//============================================================================
//		NApplication::GetColorToggle : Get the "Show/Hide Colors" text.
//----------------------------------------------------------------------------
NString NApplication::GetColorToggle(void) const
{	NString		theKey, theValue;



	// Get the text
	theKey   = NColorPanel::IsVisible() ? kStringHideColors : kStringShowColors;
	theValue = NBundleString(theKey, "", kNanoStrings);
	
	NN_ASSERT(theValue.IsNotEmpty());
	
	return(theValue);
}





//============================================================================
//		NApplication::GetFontToggle : Get the "Show/Hide Fonts" text.
//----------------------------------------------------------------------------
NString NApplication::GetFontToggle(void) const
{	NString		theKey, theValue;



	// Get the text
	theKey   = NFontPanel::IsVisible() ? kStringHideFonts : kStringShowFonts;
	theValue = NBundleString(theKey, "", kNanoStrings);
	
	NN_ASSERT(theValue.IsNotEmpty());
	
	return(theValue);
}

