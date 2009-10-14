/*	NAME:
		NDocument.cpp

	DESCRIPTION:
		Document object.
	
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
#include "NGeometryUtilities.h"
#include "NFileUtilities.h"
#include "NPropertyFile.h"
#include "NApplication.h"
#include "NCGContext.h"
#include "NCFBundle.h"
#include "NIcon.h"
#include "NDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Properties
//
// These values may be archived to disk, and can never change.
static const NString kDocWindowFrameKey					= "DocWindowFrame";





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NDocumentList	NDocument::mDocuments;
UInt32			NDocument::mUntitledCount;
NDocument		*NDocument::mCurrentDocument;





//============================================================================
//		NDocument::NDocument : Constructor.
//----------------------------------------------------------------------------
NDocument::NDocument(void)
{


	// Initialize ourselves
	mIsModified      = false;
	mCloseOnSave     = false;
	mContinueOnClose = false;



	// Add the document to the list
	mDocuments.push_back(this);
}





//============================================================================
//		NDocument::~NDocument : Destructor.
//----------------------------------------------------------------------------
NDocument::~NDocument(void)
{


	// Remove the document from the list
	mDocuments.remove(this);



	// Update the state
	if (mCurrentDocument == this)
		mCurrentDocument = NULL;
}





//============================================================================
//		NDocument::InitializeDocument : Initialize the document.
//----------------------------------------------------------------------------
OSStatus NDocument::InitializeDocument(const NFile *theFile)
{	EventTypeSpec		theEvents[] = {	{ kEventClassWindow, kEventWindowActivated },
										{ kEventClassNone,	 kEventKindNone        } };
	NString				nibName, resName;
	NHIWindow			*theWindow;
	OSStatus			theErr;



	// Set the file
	if (theFile != NULL)
		SetFile(*theFile);



	// Initialize the properties
	theErr = InitializeProperties(NApplication::GetApp()->GetProperties());
	if (theErr != noErr)
		goto cleanup;



	// Initialize the controller
	nibName = mProperties.GetValueString(kDocNibFileKey);
	resName = mProperties.GetValueString(kDocNibWindowKey);

	theErr = NWindowController::Initialize(nibName, resName);
	if (theErr != noErr)
		goto cleanup;

	if (mProperties.GetValueBoolean(kDocWindowToolbarUnified))
		GetWindow()->SetAttributes(kWindowUnifiedTitleAndToolbarAttribute);



	// Register for events
	AddEvents(theEvents);

	GetUndoManager()->AddListener(this);
	NCFPreferences::GetPrefs()->AddListener(this);
	mFileDialog.AddListener(this);
	mPrintDialog.AddListener(this);
	


	// Prepare the window
	theWindow = GetWindow();
	theWindow->SetPosition(kWindowCascadeOnMainScreen);

	mFileDialog.SetParent( theWindow);
	mPrintDialog.SetParent(theWindow);

	UpdateWindow();



	// Prepare the document
	DoPrefChanged(NCFPreferences::GetPrefs(), kPrefChangedAllKey);
	
	if (HasFile())
		theErr |= LoadDocument();
	
	if (theErr == noErr)
		theWindow->Select();



	// Handle failure
cleanup:
	if (theErr != noErr)
		delete this;
	
	return(theErr);
}





//============================================================================
//		NDocument::HasFile : Does the document have a file?
//----------------------------------------------------------------------------
bool NDocument::HasFile(void) const
{


	// Check our state
	return(mAlias.IsValid());
}





//============================================================================
//		NDocument::IsModified : Is the document modified?
//----------------------------------------------------------------------------
bool NDocument::IsModified(void) const
{


	// Get our state
	//
	// A document's modified state is in two parts; the presence of actions on
	// the undo stack, and an additional flag indicating non-undable changes.
	//
	// Once a non-undoable change has been made, the document remains dirty
	// until that state is explicitly cleared with SetModified (which also
	// clears the undo stacks, so ensures the document is non-dirty).
	return(mIsModified || GetUndoManager()->CanUndo());
}





//============================================================================
//		NDocument::SetModified : Set the modified state.
//----------------------------------------------------------------------------
void NDocument::SetModified(bool isModified)
{


	// Set our state
	mIsModified = isModified;



	// Update the related objects
	//
	// Manipulating the modified state by hand clears the undo stacks,
	// since we should only be used for changes that are not undable.
	GetUndoManager()->Clear();

	UpdateWindowModified();
}





//============================================================================
//		NDocument::GetFile : Get the document file.
//----------------------------------------------------------------------------
NFile NDocument::GetFile(void) const
{


	// Get the file
	return(mAlias.GetFile());
}





//============================================================================
//		NDocument::SetFile : Set the document file.
//----------------------------------------------------------------------------
void NDocument::SetFile(const NFile &theFile)
{


	// Set the file
	mAlias.SetFile(theFile);



	// Update the window
	//
	// During file-based initialisation, we will not have a window yet.
	if (GetWindow() != NULL)
		UpdateWindow();
}





//============================================================================
//		NDocument::GetProperties : Get the properties dictionary.
//----------------------------------------------------------------------------
NDictionary *NDocument::GetProperties(void)
{


	// Get the properties
	return(&mProperties);
}





//============================================================================
//		NDocument::GetDocumentName : Get the document name.
//----------------------------------------------------------------------------
//		Note :	The document name is a user-visible string which identifies
//				the document, before and after it has been saved.
//
//				Unspecified documents are titled as per the HIG, switching to
//				the display name of the file once saved.
//----------------------------------------------------------------------------
NString NDocument::GetDocumentName(void)
{	NString							theName, theExtension;
	NDocumentListConstIterator		theIter;
	bool							isReset;
	NUTI							fileUTI;



	// Name untitled documents
	//
	// The HIG guidelines are for file-less documents to be named "untitled",
	// "untitled 2", "untitled 3", etc. The "u" is always lower-case, and
	// there is no "untitled 1".
	//
	// If all untitled documents are closed, the HIG requires that the count
	// be reset for the next untitled document created by the user.
	if (!HasFile() && mUntitledName.IsEmpty())
		{
		// Check for a reset
		//
		// If everything has a file, the untitled count resets to the start.
		isReset = true;
		
		for (theIter = mDocuments.begin(); theIter != mDocuments.end(); theIter++)
			{
			if (!(*theIter)->HasFile())
				{
				isReset = false;
				break;
				}
			}
		
		if (isReset || mDocuments.size() == 1)
			mUntitledCount = 0;
		
		
		
		// Select a name
		mUntitledCount++;
		
		mUntitledName = NBundleString(kStringDocumentUntitled, "", kNanoStrings);
		fileUTI       = mProperties.GetValueString(kDocFileUTIKey);
		theExtension  = fileUTI.GetFileExtension();

		if (mUntitledCount > 1)
			mUntitledName.Format("%s %d", mUntitledName.GetUTF8(), mUntitledCount);
		
		if (theExtension.IsNotEmpty())
			mUntitledName.Format("%s.%s", mUntitledName.GetUTF8(), theExtension.GetUTF8());
		}



	// Get the document name
	if (HasFile())
		theName = GetFile().GetDisplayName();
	else
		theName = mUntitledName;

	return(theName);
}





//============================================================================
//		NDocument::GetDocuments : Get the document list.
//----------------------------------------------------------------------------
NDocumentList NDocument::GetDocuments(bool onlyModified)
{	NDocumentListConstIterator		theIter;
	NDocumentList					theList;



	// Get all the documents
	if (!onlyModified)
		return(mDocuments);



	// Get the modified documents
	for (theIter = mDocuments.begin(); theIter != mDocuments.end(); theIter++)
		{
		if ((*theIter)->IsModified())
			theList.push_back(*theIter);
		}
	
	return(theList);
}





//============================================================================
//		NDocument::GetCurrentDocument : Get the current document.
//----------------------------------------------------------------------------
NDocument *NDocument::GetCurrentDocument(void)
{


	// Get the current document
	return(mCurrentDocument);
}





//============================================================================
//		NDocument::InitializeProperties : Initialize the properties.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NDocument::InitializeProperties(const NDictionary *appProperties)
{	NCFBundle	appBundle;
	NArray		theUTIs;
	NString		theUTI;



	// Validate our parameters
	NN_ASSERT(appProperties != NULL);



	// Get the state we need
	theUTIs = appBundle.GetInfoUTIs(kLSRolesEditor);
	theUTI  = kUTNTypeItem;

	if (theUTIs.IsNotEmpty())
		theUTI = theUTIs.GetValueString(0);



	// Initialize our properties
	//
	// By default documents can be saved to any of the formats listed in our Info.plist's
	// CFBundleDocumentTypes key, and the initial format for documents created without a
	// file is assumed to be the first item in this array.
	//
	// Sub-classes can customise this behaviour by overriding these properties, but this
	// provides a reasonable default for most applications.
	mProperties.SetValueString( kDocNibFileKey,				appProperties->GetValueString(kAppNibFileKey));
	mProperties.SetValueString( kDocNibWindowKey,			"Document");
	mProperties.SetValueBoolean(kDocWindowToolbarUnified,	true);
	mProperties.SetValueString( kDocRecentItemsGroupKey,    "");
	mProperties.SetValueString( kDocFileIconKey,			"FileDocument");
	mProperties.SetValueString( kDocFileUTIKey,				theUTI);
	mProperties.SetValueArray(  kDocFormatUTIsKey,			theUTIs);

	return(noErr);
}





//============================================================================
//		NDocument::InitializeSelf : Initialize ourselves
//----------------------------------------------------------------------------
OSStatus NDocument::InitializeSelf(void)
{


	// Initialize ourselves
	//
	// Sub-classes should override to initialize themselves, then call through.
	return(noErr);
}





//============================================================================
//		NDocument::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void NDocument::DoMessage(BroadcastMsg theMsg, void *msgData)
{	const NString		*prefsKey    = (const NString     *) msgData;
	FileDialogResult	*fileResult  = (FileDialogResult  *) msgData;
	PrintDialogResult	*printResult = (PrintDialogResult *) msgData;



	// Handle the message
	//
	// Sub-classes must call through to pass unknown messages to us.
	switch (theMsg) {
		case kMsgPreferenceValueChanged:
			DoPrefChanged(NCFPreferences::GetPrefs(), *prefsKey);
			break;

		case kMsgFileDialogResult:
			CompleteFileDialog(*fileResult);
			break;
		
		case kMsgPrintDialogResult:
			CompletePrintDialog(*printResult);
			break;
		
		case kMsgUndoManagerUpdated:
			UpdateWindowModified();
			break;
		
		default:
			NWindowController::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		NDocument::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NDocument::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kHICommandCancel:
			theErr = CommandCancel();
			break;
		
		case kHICommandSave:
			theErr = CommandSave();
			break;

		case kHICommandSaveAs:
			theErr = CommandSaveAs();
			break;
		
		case kHICommandRevert:
			theErr = CommandRevert();
			break;
		
		case kHICommandPageSetup:
			theErr = CommandPageSetup();
			break;
		
		case kHICommandPrint:
			theErr = CommandPrint();
			break;
			
		default:
			theErr = NWindowController::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NDocument::DoEventAppActivated : Handle kEventAppActivated.
//----------------------------------------------------------------------------
//		Note : Forwarded to us by NApplication.
//----------------------------------------------------------------------------
OSStatus NDocument::DoEventAppActivated(NCarbonEvent &/*theEvent*/)
{


	// Update the alias
	(void) mAlias.GetFile();



	// Update the window
	UpdateWindow();

	return(eventNotHandledErr);
}





//============================================================================
//		NDocument::DoEventWindowActivated : Handle kEventWindowActivated.
//----------------------------------------------------------------------------
OSStatus NDocument::DoEventWindowActivated(NCarbonEvent &/*theEvent*/)
{


	// Close if necessary
	//
	// The HIG requires that when a user has elected to review changes during
	// a quit operation, any dirty windows display their "save changes" sheet
	// only when they are activated.
	if (IsModified() && NApplication::GetApp()->GetState() == kAppStateQuitReviewMultiple)
		{
		if (!mFileDialog.IsOpen())
			CommandClose();
		}



	// Update our state
	mCurrentDocument = this;

	return(eventNotHandledErr);
}





//============================================================================
//		NDocument::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NDocument::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;
	bool			hasSheet;



	// Get the command status
	hasSheet = GetWindow()->HasSheet();
	
	switch (hiCmd.commandID) {
		case kHICommandSaveAs:
		case kHICommandPageSetup:
		case kHICommandPrint:
			theStatus = EnableIf(!hasSheet);
			break;

		case kHICommandSave:
			theStatus = EnableIf((IsModified() || !HasFile()) && !hasSheet);
			break;
		
		case kHICommandRevert:
			theStatus = EnableIf((IsModified() &&  HasFile()) && !hasSheet);
			break;
		
		default:
			theStatus = NWindowController::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		NDocument::CommandCancel : Handle a cancel command.
//----------------------------------------------------------------------------
OSStatus NDocument::CommandCancel(void)
{	bool		hadSheet;
	OSStatus	theErr;



	// Cancel our sheets
	//
	// Our file and print dialog sheets are managed by those objects, but we
	// may also receive a cancel command from some other sheet that has been
	// attached to this controller.
	//
	// To allow sub-classes to process the command, or NWindowController close
	// the sheet, we must invoke our CommandSheet method as NWindowController
	// would have done if we hadn't handled kHICommandCancel.
	hadSheet = (mFileDialog.IsOpen() || mPrintDialog.IsOpen());
	
	if (mFileDialog.IsOpen())
		mFileDialog.Cancel();

	if (mPrintDialog.IsOpen())
		mPrintDialog.Cancel();
		
	theErr = CommandSheet(kHICommandCancel);
	if (theErr == eventNotHandledErr && hadSheet)
		theErr = noErr;



	// Reset our state
	//
	// Although a file dialog can report its own cancel event, we may also
	// be sent a generic cancel by the application if a review sequence is
	// cancelled.
	//
	// We reset our "foo-on-bar" flags in both cases, since they're now stale.
	mCloseOnSave     = false;
	mContinueOnClose = false;

	return(theErr);
}





//============================================================================
//		NDocument::CommandClose : Handle a close command.
//----------------------------------------------------------------------------
OSStatus NDocument::CommandClose(void)
{	bool			closeDoc, continueClose;
	AppState		theState;
	NApplication	*theApp;



	// Get the state we need
	theApp   = NApplication::GetApp();
	theState = theApp->GetState();
	
	closeDoc      = true;
	continueClose = mContinueOnClose;



	// Prompt the user
	if (IsModified())
		{
		// Reset our state
		//
		// If the document needs to prompt the user, most paths through do not
		// involve either closing ourselves or continuing the close; when the
		// sheet completes it will clear our modified state and re-close.
		closeDoc      = false;
		continueClose = false;



		// Activate the sheet
		//
		// If we have a sheet open, we just bring ourselves to the front; since the
		// user can move between documents during a review, they can potentially
		// have "save changes" sheets visible on multiple documents.
		//
		// When one of this is accepted, and the review continues, the next
		// document in the review may already have its sheet open - in which
		// case it should not open a second.
		if (mFileDialog.IsOpen() || mPrintDialog.IsOpen())
			GetWindow()->Select();
		
		
		// Or show the sheet
		else
			{
			switch (theState) {
				case kAppStateRunning:
					// Prompt to save
					mFileDialog.AskSaveChanges(kNavSaveChangesClosingDocument);
					break;

				case kAppStateQuitReviewSingle:
				case kAppStateQuitReviewMultiple:
					// Review changes
					//
					// The most suitable sheet type for a review of a single document during
					// a quit is kNavSaveChangesOther.
					//
					// kNavSaveChangesQuittingApplication refers to "Documents" (plural) and is
					// missing a "Discard Changes" button, and kNavSaveChangesClosingDocument
					// is intended for closing a document outside a quit review.
					//
					// Since the app is reviewing, we set our flag to continue the review if
					// this document is actually closed.
					mContinueOnClose = true;
					mFileDialog.AskSaveChanges(kNavSaveChangesOther);
					break;

				case kAppStateQuitDiscardChanges:
					// Discard changes
					closeDoc = true;
					break;

				default:
					NN_LOG("Unknown app state: %d", theState);
					break;
				}
			}
		}



	// Close the document
	if (closeDoc)
		delete this;



	// Continue closing
	//
	// When modified documents are reviewed during a quit process, the user can
	// move between documents until they actually close one of the modified
	// documents.
	//
	// At this point, the document being closed then informs the app so that the
	// review can continue and any further modified documents can be closed.
	if (continueClose)
		{
		if (theState == kAppStateQuitReviewSingle || theState == kAppStateQuitReviewMultiple)
			theApp->ContinueReview();
		}

	return(noErr);
}





//============================================================================
//		NDocument::CommandSave : Handle a save command.
//----------------------------------------------------------------------------
OSStatus NDocument::CommandSave(void)
{	OSStatus	theErr;
	bool		canSave;
	NString		theUTI;



	// Get the state we need
	//
	// Only documents with a file can be saved in place, unless the file's UTI is
	// not one we can save to (i.e., we opened them as a viewer, not an editor).
	//
	// In that case we force the user to save as, to ensure they pick one of the
	// UTIs we can act as an editor for.
	canSave = HasFile();
	
	if (canSave)
		{
		theUTI  = GetFile().GetUTI();
		canSave = mProperties.GetValueArray(kDocFormatUTIsKey).HasValue(theUTI);
		}



	// Save the document
	if (canSave)
		theErr = SaveDocument();
	else
		theErr = CommandSaveAs();
	
	return(theErr);
}





//============================================================================
//		NDocument::CommandSaveAs : Handle a save as command.
//----------------------------------------------------------------------------
OSStatus NDocument::CommandSaveAs(void)
{	NString				fileName, fileUTI, textButton;
	NArray				formatUTIs;
	NavUserAction		navAction;



	// Get the state we need
	//
	// kDocFileUTIKey provides the default UTI for documents without a file,
	// but if we are re-saving an existing file then we can use the actual UTI.
	fileName   = GetDocumentName();
	fileUTI    = mProperties.GetValueString(kDocFileUTIKey);
	formatUTIs = mProperties.GetValueArray( kDocFormatUTIsKey);

	if (HasFile())
		fileUTI = GetFile().GetUTI();



	// Choose the file
	textButton = NBundleString(kStringFileDialogSave, "", kNanoStrings);
	navAction  = mFileDialog.ChooseSave("", textButton, fileName, fileUTI, NULL, formatUTIs);

	if (navAction == kNavUserActionCancel)
		return(userCanceledErr);
	
	return(noErr);
}





//============================================================================
//		NDocument::CommandRevert : Handle a revert command.
//----------------------------------------------------------------------------
OSStatus NDocument::CommandRevert(void)
{	OSStatus	theErr;



	// Confirm the revert if necessary
	if (IsModified())
		{
		mFileDialog.AskDiscardChanges(GetDocumentName());
		return(noErr);
		}



	// Revert the document
	theErr = LoadDocument();
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NDocument::CommandPageSetup : Handle a page setup command.
//----------------------------------------------------------------------------
OSStatus NDocument::CommandPageSetup(void)
{	OSStatus	theErr;



	// Prepare the dialog
	theErr = mPrintDialog.CreateSession();
	
	if (theErr == noErr)
		theErr = UpdatePrintDialog(kPrintDialogPageSetup, &mPrintDialog);
	
	
	
	// Display the dialog
	if (theErr == noErr)
		theErr = mPrintDialog.DoPageSetup();
	
	return(theErr);
}





//============================================================================
//		NDocument::CommandPrint : Handle a print command.
//----------------------------------------------------------------------------
OSStatus NDocument::CommandPrint(void)
{	OSStatus	theErr;



	// Prepare the dialog
	theErr = mPrintDialog.CreateSession();
	
	if (theErr == noErr)
		theErr = UpdatePrintDialog(kPrintDialogPrint, &mPrintDialog);
	
	
	
	// Display the dialog
	if (theErr == noErr)
		theErr = mPrintDialog.DoPrint();
	
	return(theErr);
}





//============================================================================
//		NDocument::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void NDocument::DoPrefChanged(NCFPreferences */*thePrefs*/, const NString &/*theKey*/)
{


	// Base class has no preferences
	//
	// Sub-classes whose state depends on the preferences should override this
	// method to detect when a preference is changed.
	//
	// This method is invoked during initialisation, with kPrefChangedAllKey,
	// to allow sub-classes to configure their state from the preferences.
}





//============================================================================
//		NDocument::LoadSelf : Load ourselves.
//----------------------------------------------------------------------------
//		Note :	NDocument provides a default file format for documents using
//				NPropertyFile, which provides a key-value based archive.
//
//				Documents that can represent their state in this kind of file
//				should implement EncodeProperties/DecodeProperties, and their
//				state will be encoded/decoded automatically.
//
//				Documents which require a custom file format should override
//				LoadSelf/SaveSelf to perform their own archiving.
//----------------------------------------------------------------------------
OSStatus NDocument::LoadSelf(void)
{	NPropertyFile	theFile;
	OSStatus		theErr;



	// Load the file
	theErr = theFile.Load(GetFile());
	NN_ASSERT_NOERR(theErr);

	if (theErr != noErr)
		return(theErr);



	// Decode our properties
	theErr = DecodeProperties(theFile.GetProperties());

	return(theErr);
}





//============================================================================
//		NDocument::SaveSelf : Save ourselves.
//----------------------------------------------------------------------------
//		Note : See LoadSelf for the default document file format.
//----------------------------------------------------------------------------
OSStatus NDocument::SaveSelf(void)
{	NPropertyFile	theFile;
	OSStatus		theErr;



	// Encode our properties
	theErr = EncodeProperties(theFile.GetProperties());
	NN_ASSERT_NOERR(theErr);

	if (theErr != noErr)
		return(theErr);



	// Save the file
	theErr = theFile.Save(GetFile());
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		NDocument::PrintSelf : Print ourselves.
//----------------------------------------------------------------------------
//		Note :	The base class provides a default PrintSelf/UpdatePrintDialog
//				that will center and scale to fit the view from GetPrintView.
//
//				Sub-classes can override GetPrintView if their content fits
//				on a single page, or PrintSelf/UpdatePrintDialog to manage
//				their own printing.
//----------------------------------------------------------------------------
OSStatus NDocument::PrintSelf(const NPrintDialog *theDialog, UInt32 pageNum)
{	HIRect				pageRect, viewRect;
	NCGContext			cgContext;
	NHIView				*theView;
	NCarbonEvent		theEvent;
	HISize				theSize;
	OSStatus			theErr;



	// Validate our parameters
	NN_ASSERT(theDialog != NULL);
	NN_ASSERT(pageNum   == 1);

	(void) pageNum;



	// Get the state we need
	theEvent  = theDialog->CreateEventControlDraw();
	cgContext = theEvent.GetCGContext();
	theView   = GetPrintView();
	
	if (theView == NULL)
		{
		SysBeep(30);
		return(noErr);
		}



	// Position the view
	//
	// The view is scaled to fit and centered on the page.
	pageRect = theEvent.GetParameterHIRect(kEventNParamPageRect);
	viewRect = theView->GetBounds();
	theSize  = viewRect.size;

	NGeometryUtilities::ScaleToFit(  viewRect, pageRect);
	NGeometryUtilities::PositionRect(viewRect, pageRect, kPositionCenter);



	// Draw the view
	cgContext.ClipToRect(viewRect);
	
	cgContext.TranslateCTM(viewRect.origin.x, viewRect.origin.y);
	cgContext.ScaleCTM(    viewRect.size.width / theSize.width, viewRect.size.height / theSize.height);

	theErr = theEvent.SendToHIView(*theView);
	NN_ASSERT_NOERR(theErr);

	return(noErr);
}





//============================================================================
//		NDocument::EncodeProperties : Encode the properties.
//----------------------------------------------------------------------------
OSStatus NDocument::EncodeProperties(NDictionary &theProperties)
{


	// Encode our properties
	//
	// Sub-classes should override to encode their properties, then call through.
	theProperties.SetValueRect(kDocWindowFrameKey, GetWindow()->GetFrame());
	
	return(noErr);
}





//============================================================================
//		NDocument::DecodeProperties : Decode the properties.
//----------------------------------------------------------------------------
OSStatus NDocument::DecodeProperties(const NDictionary &theProperties)
{


	// Decode our properties
	//
	// Sub-classes should override to encode their properties, then call through.
	if (theProperties.HasKey(kDocWindowFrameKey))
		GetWindow()->SetFrame(theProperties.GetValueRect(kDocWindowFrameKey));
	
	return(noErr);
}





//============================================================================
//		NDocument::UpdateWindowTitle : Update the window title.
//----------------------------------------------------------------------------
void NDocument::UpdateWindowTitle(void)
{


	// Update the title
	//
	// The base class sets the title to the document name (e.g., "My Picture.jpg"),
	// however sub-classes may wish to override this method to augment the basic
	// name (e.g., "My Picture.jpg (200x200)".
	GetWindow()->SetTitle(GetDocumentName());
}





//============================================================================
//		NDocument::UpdateWindowProxy : Update the window proxy.
//----------------------------------------------------------------------------
void NDocument::UpdateWindowProxy(void)
{	OSType			fileType, fileCreator;
	NString			fileIcon;
	NFile			theFile;
	NIcon			theIcon;
	NUTI			fileUTI;



	// Set the window proxy
	//
	// If we have a file, the proxy provides the icon and the pop-up path.
	if (HasFile())
		{
		GetWindow()->SetProxyIcon(mAlias);
		return;
		}



	// Get the state we need
	fileIcon = mProperties.GetValueString(kDocFileIconKey);
	fileUTI  = mProperties.GetValueString(kDocFileUTIKey);

	fileType    = fileUTI.GetFileType();
	fileCreator = NApplication::GetApp()->GetProperties()->GetValueUInt32(kAppCreatorKey);



	// Prepare the icon
	//
	// If we have an icon within the app bundle then we use it, otherwise we fall
	// back to type/creator. Launch Services will not return icons for apps that
	// have not yet been registered, so looking for the icon "by hand" ensures
	// that the icon is always seen.
	if (fileIcon.IsNotEmpty())
		{
		theFile = NBundleResource(fileIcon, "icns");
		if (theFile.IsFile())
			theIcon = NIcon(theFile);
		}
	
	if (!theIcon.IsValid() && fileType != kFileTypeNone && fileCreator != kFileCreatorNone)
		theIcon = NIcon(fileType, fileCreator);

	GetWindow()->SetProxyIcon(theIcon);
}





//============================================================================
//		NDocument::UpdateWindowModified : Update the window modified state.
//----------------------------------------------------------------------------
void NDocument::UpdateWindowModified(void)
{


	// Update the window modified state
	GetWindow()->SetModified(IsModified());
}





//============================================================================
//		NDocument::UpdatePrintDialog : Update the print dialog.
//----------------------------------------------------------------------------
OSStatus NDocument::UpdatePrintDialog(PrintDialogKind theKind, NPrintDialog *theDialog)
{


	// Update the dialog
	//
	// The base class assigns default settings for a single-page document
	// named after the document.
	//
	// Sub-classes should override to adjust the print dialog as necessary.
	switch (theKind) {
		case kPrintDialogPageSetup:
			// Do nothing
			break;

		case kPrintDialogPrint:
			theDialog->SetJobName(GetDocumentName());
			theDialog->SetPageRange(1, 1);
			break;
		
		default:
			NN_LOG("Unknown dialog: %d", theKind);
			break;
		}
	
	return(noErr);
}





//============================================================================
//		NDocument::GetPrintView : Get the print view.
//----------------------------------------------------------------------------
NHIView *NDocument::GetPrintView(void)
{


	// Get the view to print
	//
	// Sub-classes should override to print the view to print, if they are
	// using the default UpdatePrintDIalog/PrintSelf implementation.
	return(NULL);
}





//============================================================================
//		NDocument::LoadDocument : Load the document.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NDocument::LoadDocument(void)
{	OSStatus	theErr;



	// Load the document
	theErr = LoadSelf();
	if (theErr != noErr)
		return(theErr);



	// Update our state
	SetModified(false);
	UpdateWindow();
	
	return(theErr);
}





//============================================================================
//		NDocument::SaveDocument : Save the document.
//----------------------------------------------------------------------------
OSStatus NDocument::SaveDocument(void)
{	OSType			fileType, fileCreator;
	NString			groupID;
	NFile			theFile;
	NUTI			theUTI;
	OSStatus		theErr;



	// Save the document
	theErr = SaveSelf();
	if (theErr != noErr)
		return(theErr);



	// Update the file
	//
	// Once the file has been saved, we assign a type/creator based on our UTI.
	theFile = GetFile();
	theUTI  = theFile.GetUTI();
	
	if (theUTI.GetFileType() == kFileTypeNone)
		theUTI = mProperties.GetValueString(kDocFileUTIKey);
		
	fileType = theUTI.GetFileType();
	if (fileType != kFileTypeNone)
		{
		fileCreator = NApplication::GetApp()->GetProperties()->GetValueUInt32(kAppCreatorKey);
		theFile.SetFileCreatorAndType(fileCreator, fileType);
		}



	// Update our state
	SetModified(false);
	UpdateWindow();



	// Update the application
	groupID = mProperties.GetValueString(kDocRecentItemsGroupKey);
	
	NApplication::GetApp()->GetRecentItems()->AddItem(theFile, groupID);

	return(theErr);
}





//============================================================================
//		NDocument::PrintDocument : Print the document.
//----------------------------------------------------------------------------
OSStatus NDocument::PrintDocument(void)
{	UInt32				n, firstPage, lastPage;
	PMPrintSettings		printSettings;
	PMPrintSession		printSession;
	PMPageFormat		pageFormat;
	OSStatus			theErr;



	// Get the state we need
	pageFormat    = mPrintDialog.GetPageFormat();
	printSession  = mPrintDialog.GetPrintSession();
	printSettings = mPrintDialog.GetPrintSettings();

	NN_ASSERT(pageFormat    != NULL);
	NN_ASSERT(printSession  != NULL);
	NN_ASSERT(printSettings != NULL);

	mPrintDialog.GetPageRange(firstPage, lastPage);
	NN_ASSERT(lastPage != (UInt32) kPMPrintAllPages);



	// Begin the document
	theErr = PMSessionBeginDocument(printSession, printSettings, pageFormat);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		return(theErr);
	
	
	
	// Print each page
	for (n = firstPage; n <= lastPage && theErr == noErr; n++)
		{
		theErr = PMSessionBeginPage(printSession, pageFormat, NULL);
		if (theErr == noErr)
			{
			theErr  = PrintSelf(&mPrintDialog, n);
			theErr |= PMSessionEndPage(printSession);
			}
		
		NN_ASSERT(theErr == noErr || theErr == kDTPAbortJobErr);
		}



	// End the document
	theErr |= PMSessionEndDocument(printSession);
	NN_ASSERT(theErr == noErr || theErr == kDTPAbortJobErr);



	// Clean up
	mPrintDialog.DestroySession();

	return(theErr);
}





//============================================================================
//		NDocument::UpdateWindow : Update the window state.
//----------------------------------------------------------------------------
void NDocument::UpdateWindow(void)
{


	// Update the window state
	UpdateWindowTitle();
	UpdateWindowProxy();
	UpdateWindowModified();
}





//============================================================================
//		NDocument::CompleteFileDialog : Complete a file dialog.
//----------------------------------------------------------------------------
void NDocument::CompleteFileDialog(const FileDialogResult &theResult)
{	AppState		theState;
	bool			closeDoc;
	NApplication	*theApp;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theResult.theFiles.size() <= 1);



	// Get the state we need
	theApp   = NApplication::GetApp();
	theState = theApp->GetState();

	closeDoc = false;



	// Complete the dialog
	switch (theResult.navAction) {
		case kNavUserActionCancel:
			// Cancel the action
			//
			// As well as resetting our own state, if the app is performing a review
			// during quit then we are responsible for cancelling the review.
			mCloseOnSave     = false;
			mContinueOnClose = false;
			
			if (theState == kAppStateQuitReviewSingle || theState == kAppStateQuitReviewMultiple)
				theApp->CancelReview();
			break;


		case kNavUserActionSaveAs:
			// Save the document
			SetFile(theResult.theFiles[0]);
			theErr = SaveDocument();


			// Close if necessary
			//
			// If we save-as'd due to closing an unspecified document, the document
			// should be closed when the save is completed.
			closeDoc = mCloseOnSave;
			break;


		case kNavUserActionSaveChanges:
			// Save the document
			//
			// If we are saving an unspecified document, the save command will need
			// to invoke a save-as dialog in order to save the document - so we need
			// to defer the close-on-save behaviour to the conclusion of that sheet.
			mCloseOnSave = !HasFile();
			theErr       = CommandSave();
			
			
			// Close if necessary
			//
			// If we were specified, CommandSave will have saved the document and
			// so we can close ourselves right now.
			if (theErr == noErr && !mCloseOnSave)
				closeDoc = true;
			break;


		case kNavUserActionDontSaveChanges:
			// Don't save
			//
			// By clearing the modified state, we ensure that when we close we will
			// just delete ourselves silently without any prompting.
			SetModified(false);
			closeDoc = true;
			break;


		case kNavUserActionDiscardChanges:
			// Revert changes
			theErr = LoadDocument();
			break;


		default:
			NN_LOG("Unknown action: %d", theResult.navAction);
			break;
		}



	// Complete a save
	if (theResult.navAction == kNavUserActionSaveAs)
		{
		// Complete the save
		if (theResult.navReply != NULL)
			{
			// Update Nav services
			//
			// Nav Services may need to update the file state once it has been
			// saved, e.g., to adjust the extension visibility of the file.
			theErr = NavCompleteSave(theResult.navReply, kNavTranslateInPlace);
			NN_ASSERT_NOERR(theErr);
			
			
			// Update our state
			//
			// If a format was selected in the Nav dialog, we adjust our UTI to match.
			if (theResult.formatUTI.IsValid())
				mProperties.SetValueString(kDocFileUTIKey, theResult.formatUTI);
			}
		
		

		// Update the window
		UpdateWindow();
		}



	// Close the document
	if (closeDoc)
		{
		theErr = CommandClose();
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NDocument::CompletePrintDialog : Complete a print dialog.
//----------------------------------------------------------------------------
void NDocument::CompletePrintDialog(const PrintDialogResult &theResult)
{	OSStatus	theErr;



	// Complete the dialog
	switch (theResult.dialogKind) {
		case kPrintDialogPageSetup:
			// Do nothing
			break;


		case kPrintDialogPrint:
			if (theResult.didAccept)
				theErr = PrintDocument();
			break;


		default:
			NN_LOG("Unknown dialog: %d", theResult.dialogKind);
			break;
		}
}


