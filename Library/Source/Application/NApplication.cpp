/*	NAME:
		NApplication.cpp

	DESCRIPTION:
		Application object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "Nano Build.h"
#include "NFileUtilities.h"
#include "NIBUtilities.h"
#include "NCFBundle.h"
#include "NAboutBox.h"
#include "NApplication.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const EventTime kSplashTimeMinimum								= 2.0f;





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


	// Validate our state
	NN_ASSERT(mApp == this);



	// Clean up
	DisposeNibReference(mNib);



	// Reset our state
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
{


	// Quit the app
	NCarbonEvent::CreateCommand(kHICommandQuit).PostToQueue();
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
{	NCFBundle	appBundle;



	// Initialize the properties
	mProperties.SetValueString( kAppNibFileKey,				"Application");
	mProperties.SetValueString( kAppNibMenuBarKey,			"MenuBar");
	mProperties.SetValueString( kAppNibHelpMenuKey,			"Help");
	mProperties.SetValueString( kAppNibSplashScreenKey,		"Splash");
	mProperties.SetValueString( kAppNameKey,				"Nano App");
	mProperties.SetValueString( kAppHomePageKey,			"http://www.refnum.com/products/nano/");
	mProperties.SetValueUInt32( kAppCreatorKey,				kFileCreatorNone);
	mProperties.SetValueArray(  kAppFormatUTIsKey,			appBundle.GetInfoUTIs());
	mProperties.SetValueBoolean(kAppCreateDocOnOpenKey,		true);
	mProperties.SetValueBoolean(kAppCreateDocOnReopenKey,	true);

	return(noErr);
}





//============================================================================
//		NApplication::InitializeBegin : Begin initialisation.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeBegin(void)
{	EventTypeSpec		carbonEvents[] = {	{ kEventClassCommand,		kEventCommandProcess		},
											{ kEventClassCommand,		kEventCommandUpdateStatus	},
											{ kEventClassApplication,	kEventAppActivated			},
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
	theFSRef = appBundle.GetFile().GetFSRef();
	theErr   = AHRegisterHelpBook(&theFSRef);
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		NApplication::InitializeMenuBar : Initialize the menu bar.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeMenuBar(void)
{	NString				nibName, resMenu, resHelp;
	MenuRef				menuHelp, menuApp;
	MenuItemIndex		theIndex;
	SInt32				errorFD;
	IBNibRef			nibRef;
	OSStatus			theErr;
	


	// Get the state we need
	nibName = mProperties.GetValueString(kAppNibFileKey);
	resMenu = mProperties.GetValueString(kAppNibMenuBarKey);
	resHelp = mProperties.GetValueString(kAppNibHelpMenuKey);



	// Load the menu bar
	//
	// Carbon applications automatically gain a Help menu, which holds
	// default commands such as opening the application help book.
	//
	// To allow applications to insert custom items in the Help menu,
	// we look for an optional menu in the app nib file.
	nibRef = NIBUtilities::CreateNib(nibName);
	if (nibRef != NULL)
		{
		NIBUtilities::SetMenuBar(nibRef, resMenu);

		if (resHelp.IsNotEmpty())
			{
			// Load the resource
			//
			// Since the resource is optional, we suppress errors.
			errorFD = NIBUtilities::SetErrorState(-1);
			menuApp = NIBUtilities::CreateMenu(nibRef, resHelp);
			(void)    NIBUtilities::SetErrorState(errorFD);
		
		
			// Insert the menu items
			if (menuApp != NULL)
				{
				theErr = HMGetHelpMenu(&menuHelp, &theIndex);
				NN_ASSERT_NOERR(theErr);
				
				NN_ASSERT(theIndex >= 1);
				theIndex--;
				
				theErr  = CopyMenuItems(menuApp, 1, CountMenuItems(menuApp), menuHelp, theIndex);
				theErr |= ReleaseMenu(  menuApp);
				NN_ASSERT_NOERR(theErr);
				}
			}

		DisposeNibReference(nibRef);
		}



	// Attach the recent items menu
	mRecentItems.AttachMenu();

	return(noErr);
}





//============================================================================
//		NApplication::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeSelf(void)
{


	// Initialize ourselves
	//
	// Sub-classes would typically override this method for their initialisation.
	return(noErr);
}





//============================================================================
//		NApplication::InitializeEnd : Finish initialisation.
//----------------------------------------------------------------------------
OSStatus NApplication::InitializeEnd(void)
{


	// Conclude initialisation
	//
	// Sub-classes can override this method if they need control just before RAEL.
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
//		NApplication::DoEventCommandProcess : Hande kEventCommandProcess.
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
		
		case kHICommandQuit:
			theErr = CommandQuit();
			break;
		
		case kHICommandAbout:
			theErr = CommandAbout();
			break;
		
		case kHICommandPreferences:
			theErr = CommandPreferences();
			break;
		
		default:
			theErr = eventNotHandledErr;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NApplication::DoEventAppActivated : Hande kEventAppActivated.
//----------------------------------------------------------------------------
OSStatus NApplication::DoEventAppActivated(NCarbonEvent &theEvent)
{


	// Inform our documents
	SendEventToDocuments(theEvent, false);
	
	return(eventNotHandledErr);
}





//============================================================================
//		NApplication::DoAppleEventOpenApp : Handle kAEOpenApplication.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventOpenApp(const AppleEvent &/*theEvent*/)
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
OSStatus NApplication::DoAppleEventOpenDocs(const AppleEvent &theEvent)
{	NFileList		theFiles;
	AEDescList		aeFiles;
	OSStatus		theErr;



	// Get the documents
	theErr = AEGetParamDesc(&theEvent, keyDirectObject, typeAEList, &aeFiles);
	if (theErr != noErr)
		return(theErr);
	
	theFiles = NFileUtilities::GetAEFiles(&aeFiles);
	theErr   = AEDisposeDesc(&aeFiles);



	// Create the documents
	return(CreateDocuments(theFiles, NULL));
}





//============================================================================
//		NApplication::DoAppleEventPrintDocs : Handle kAEPrintDocuments.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventPrintDocs(const AppleEvent &theEvent)
{	NCarbonEvent	printEvent;
	NFileList		theFiles;
	AEDescList		aeFiles;
	OSStatus		theErr;



	// Get the documents
	theErr = AEGetParamDesc(&theEvent, keyDirectObject, typeAEList, &aeFiles);
	if (theErr != noErr)
		return(theErr);
	
	theFiles = NFileUtilities::GetAEFiles(&aeFiles);
	theErr   = AEDisposeDesc(&aeFiles);



	// Print the documents
	printEvent = NCarbonEvent::CreateCommand(kHICommandPrint);
	
	return(CreateDocuments(theFiles, &printEvent));
}





//============================================================================
//		NApplication::DoAppleEventQuitApp : Handle kAEQuitApplication.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventQuitApp(const AppleEvent &/*theEvent*/)
{	OSStatus	theErr;



	// Quit the app
	//
	// kAEQuitApplication is sent when the application is asked to quit.
	//
	// Note that CommandQuit returns eventNotHandled when the quit is to
	// continue, since this means the event should proceed.
	theErr = CommandQuit();

	if (theErr == eventNotHandledErr)
		QuitApplicationEventLoop();

	return(noErr);
}





//============================================================================
//		NApplication::DoAppleEventReopenApp : Handle kAEReopenApplication.
//----------------------------------------------------------------------------
OSStatus NApplication::DoAppleEventReopenApp(const AppleEvent &/*theEvent*/)
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
		return(memFullErr);



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



	// Choose the documents
	textButton = NBundleString(kStringFileDialogOpen, "", kNanoStrings);
	navAction  = theDialog.ChooseFile("", textButton, theResult, formatUTIs);

	if (navAction == kNavUserActionCancel)
		return(userCanceledErr);



	// Create the documents
	return(CreateDocuments(theResult.theFiles, NULL));
}





//============================================================================
//		NApplication::CommandQuit : Quit the application.
//----------------------------------------------------------------------------
OSStatus NApplication::CommandQuit(void)
{	OSStatus	theErr;



	// Close the documents
	//
	// Closing documents may need to cancel the quit, to review changes.
	theErr = CloseDocuments();
	if (theErr != eventNotHandledErr)
		return(theErr);



	// Close the windows
	CloseWindows();
	
	return(eventNotHandledErr);
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

	aboutTitle.Format(aboutTitle.GetUTF8(), mProperties.GetValueString(kAppNameKey).GetUTF8());



	// Prepare the window properties
	//
	// The base class provides a standard Nano about box with "home page" button,
	// however sub-classes can override this method to customise their about box.
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
//		NApplication::CreateDocument : Create a new document.
//----------------------------------------------------------------------------
NDocument *NApplication::CreateDocument(void)
{


	// Create the document
	//
	// Sub-classes should override create a new instance of their document
	// class. The base class will take care of initialising the document
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
OSStatus NApplication::CloseDocuments(void)
{	NDocument			*theDocument;
	NDocumentList		theDocuments;
	NFileDialog			fileDialog;
	NavUserAction		navAction;
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
		return(eventNotHandledErr);
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
		
		return(theErr);
		}



	// Handle multiple modified documents
	//
	// Each modified document is closed in turn, until no docs remain.
	navAction = fileDialog.AskReviewDocuments(theDocuments.size());
	theErr    = noErr;
	
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
			mState = kAppStateQuitDiscardChanges;
			theErr = eventNotHandledErr;
			
			SendEventToDocuments(theEvent, false);
			break;
		
		
		default:
			NN_LOG("Unknown nav action: %d", navAction);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NApplication::CloseWindows : Close the windows.
//----------------------------------------------------------------------------
void NApplication::CloseWindows(void)
{	NHIWindowList				theWindows;
	NCarbonEvent				theEvent;
	HIWindowRef					hiWindow;
	NHIWindowListConstIterator	theIter;



	// Close the windows
	theWindows = NHIWindow::GetWindows();
	theEvent   = NCarbonEvent::CreateCommand(kHICommandClose);
	
	for (theIter = theWindows.begin(); theIter != theWindows.end(); theIter++)
		{
		hiWindow = (*theIter)->GetHIWindow();
		theEvent.SendToHIWindow(hiWindow);
		}
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
		errorFD = NIBUtilities::SetErrorState(-1);
		mSplashScreen = CreateController<NWindowSplashScreen>(nibName, resName);
		(void)    NIBUtilities::SetErrorState(errorFD);
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
