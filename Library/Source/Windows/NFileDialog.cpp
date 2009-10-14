/*	NAME:
		NFileDialog.cpp

	DESCRIPTION:
		Standard file dialog.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NFileDialog.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Application signature
//
// Documented at http://lists.apple.com/archives/carbon-dev/2006/Jul/msg00248.html
static const OSType kNavApplicationFormat								= 'extn';





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NavObjectFilterUPP	NFileDialog::mNavFilter = NewNavObjectFilterUPP(NFileDialog::NavFilter);
NavEventUPP			NFileDialog::mNavEvent  = NewNavEventUPP(       NFileDialog::NavEvent);





//============================================================================
//		NFileDialog::NFileDialog : Constructor.
//----------------------------------------------------------------------------
NFileDialog::NFileDialog(void)
{


	// Initialize ourselves
	mParent = NULL;
	mDialog = NULL;
	mKind   = kNavNone;
}





//============================================================================
//		NFileDialog::~NFileDialog : Destructor.
//----------------------------------------------------------------------------
NFileDialog::~NFileDialog(void)
{


	// Validate our state
	NN_ASSERT(!IsOpen());
}





//============================================================================
//		NFileDialog::IsOpen : Is the dialog open?
//----------------------------------------------------------------------------
bool NFileDialog::IsOpen(void) const
{


	// Check our state
	return(mDialog != NULL);
}





//============================================================================
//		NFileDialog::Cancel : Cancel the dialog.
//----------------------------------------------------------------------------
void NFileDialog::Cancel(void)
{


	// Validate our state
	NN_ASSERT(IsOpen());



	// Cancel the dialog
	NavDialogDispose(mDialog);

	mDialog = NULL;
	mKind   = kNavNone;
}





//============================================================================
//		NFileDialog::GetParent : Get the parent.
//----------------------------------------------------------------------------
NHIWindow *NFileDialog::GetParent(void) const
{


	// Get the parent
	return(mParent);
}





//============================================================================
//		NFileDialog::SetParent : Set the parent.
//----------------------------------------------------------------------------
void NFileDialog::SetParent(NHIWindow *theParent)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Set the parent
	mParent = theParent;
}





//============================================================================
//		NFileDialog::AskReviewDocuments : Prompt to review the modified docs.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::AskReviewDocuments(UInt32 numDocs)
{	NavDialogCreationOptions	theOptions;
	OSStatus					theErr;



	// Validate our parameters and state
	NN_ASSERT_MSG(numDocs != 1, "Use AskSaveChanges for 1 document");
	NN_ASSERT(!IsOpen());



	// Create the dialog
	theOptions = GetOptions();
	theErr     = NavCreateAskReviewDocumentsDialog(&theOptions, numDocs, mNavEvent, this, &mDialog);

	if (theErr != noErr)
		return(kNavUserActionCancel);



	// Run the dialog
	return(DoDialog(kNavReviewDocuments));
}





//============================================================================
//		NFileDialog::AskSaveChanges : Prompt to save changes.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::AskSaveChanges(NavAskSaveChangesAction theCause)
{	NavDialogCreationOptions	theOptions;
	OSStatus					theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Create the dialog
	theOptions = GetOptions();
	theErr     = NavCreateAskSaveChangesDialog(&theOptions, theCause, mNavEvent, this, &mDialog);

	if (theErr != noErr)
		return(kNavUserActionCancel);



	// Run the dialog
	return(DoDialog(kNavSaveChanges));
}





//============================================================================
//		NFileDialog::AskDiscardChanges : Prompt to discard changes.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::AskDiscardChanges(const NString &fileName)
{	NavDialogCreationOptions	theOptions;
	OSStatus					theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the options
	theOptions = GetOptions();
	theOptions.saveFileName = GetString(fileName);



	// Create the dialog
	theErr = NavCreateAskDiscardChangesDialog(&theOptions, mNavEvent, this, &mDialog);

	if (theErr != noErr)
		return(kNavUserActionCancel);



	// Run the dialog
	return(DoDialog(kNavDiscardChanges));
}





//============================================================================
//		NFileDialog::ChooseSave : Choose a file for saving.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::ChooseSave(const NString			&textDialog,
										const NString		&textButton,
										const NString		&fileName,
										const NUTI			&fileUTI,
										FileDialogResult	*theResult,
										const NArray		&formatUTIs)
{	NavDialogCreationOptions	theOptions;
	NArray						theFormats;
	OSStatus					theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the options
	theOptions = GetOptions();
	theFormats = GetFormatList(formatUTIs);

	theOptions.saveFileName      = GetString(fileName);
	theOptions.message           = GetString(textDialog);
	theOptions.actionButtonLabel = GetString(textButton);
	
	if (theFormats.GetSize() >= 2)
		theOptions.popupExtension = theFormats;



	// Create the dialog
	theErr  = NavCreatePutFileDialog(&theOptions, kFileTypeNone, kFileCreatorNone, mNavEvent, this, &mDialog);
	theErr |= NavDialogSetSaveFileExtensionHidden(mDialog, false);

	if (theErr != noErr)
		return(kNavUserActionCancel);



	// Run the dialog
	return(DoDialog(kNavChooseSave, theResult, &fileUTI, &formatUTIs));
}





//============================================================================
//		NFileDialog::ChooseFile : Choose a file.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::ChooseFile(const NString			&textDialog,
										const NString		&textButton,
										FileDialogResult	&theResult,
										const NArray		&formatUTIs)
{	NavDialogCreationOptions	theOptions;
	NArray						theFormats;
	OSStatus					theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the options
	//
	// We use NavDialogSetFilterTypeIdentifiers to provide UTI-based filtering
	// when we can, so only supply a format list if that API is not available.
	theOptions = GetOptions();
	theFormats = GetFormatList(formatUTIs);

	theOptions.message           = GetString(textDialog);
	theOptions.actionButtonLabel = GetString(textButton);

	if (theFormats.IsNotEmpty() && NavDialogSetFilterTypeIdentifiers == NULL)
		{
		theOptions.optionFlags   |= kNavAllFilesInPopup;
		theOptions.popupExtension = theFormats;
		}



	// Create the dialog
	theErr = NavCreateGetFileDialog(&theOptions, NULL, mNavEvent, NULL, mNavFilter, this, &mDialog);

	if (theErr != noErr)
		return(kNavUserActionCancel);



	// Run the dialog
	return(DoDialog(kNavChooseFile, &theResult, NULL, &formatUTIs));
}





//============================================================================
//		NFileDialog::ChooseFolder : Choose a folder.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::ChooseFolder(const NString		&textDialog,
										const NString		&textButton,
										FileDialogResult	&theResult)
{	NavDialogCreationOptions	theOptions;
	OSStatus					theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the options
	theOptions = GetOptions();

	theOptions.message           = GetString(textDialog);
	theOptions.actionButtonLabel = GetString(textButton);



	// Create the dialog
	theErr = NavCreateChooseFolderDialog(&theOptions, mNavEvent, mNavFilter, this, &mDialog);

	if (theErr != noErr)
		return(kNavUserActionCancel);



	// Run the dialog
	return(DoDialog(kNavChooseFolder, &theResult));
}





//============================================================================
//		NFileDialog::ChooseVolume : Choose a volume.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::ChooseVolume(const NString		&textDialog,
										const NString		&textButton,
										FileDialogResult	&theResult)
{	NavDialogCreationOptions	theOptions;
	OSStatus					theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the options
	theOptions = GetOptions();

	theOptions.message           = GetString(textDialog);
	theOptions.actionButtonLabel = GetString(textButton);



	// Create the dialog
	theErr = NavCreateChooseVolumeDialog(&theOptions, mNavEvent, mNavFilter, this, &mDialog);

	if (theErr != noErr)
		return(kNavUserActionCancel);



	// Run the dialog
	return(DoDialog(kNavChooseVolume, &theResult));
}





//============================================================================
//		NFileDialog::GetOptions : Get the default options.
//----------------------------------------------------------------------------
#pragma mark -
NavDialogCreationOptions NFileDialog::GetOptions(void)
{	NavDialogCreationOptions	theOptions;



	// Prepare the options
	NavGetDefaultDialogCreationOptions(&theOptions);

	theOptions.optionFlags |= kNavAllowPreviews;
	theOptions.optionFlags |= kNavAllowMultipleFiles;
	theOptions.optionFlags |= kNavSelectAllReadableItem;
	theOptions.optionFlags |= kNavPreserveSaveFileExtension;

	if (mParent != NULL)
		{
		theOptions.parentWindow = mParent->GetHIWindow();
		theOptions.modality     = kWindowModalityWindowModal;
		}
		
	return(theOptions);
}





//============================================================================
//		NFileDialog::GetString : Get a string.
//----------------------------------------------------------------------------
CFStringRef NFileDialog::GetString(const NString &theString)
{


	// Get the string
	//
	// Nav dialogs use NULL to indicate the default choice; an NString always
	// has an empty string associated with it, so we need to convert.
	if (theString.IsNotEmpty())
		return(theString);
	
	return(NULL);
}





//============================================================================
//		NFileDialog::DoDialog : Do the dialog.
//----------------------------------------------------------------------------
NavUserAction NFileDialog::DoDialog(FileDialogKind		theKind,
									FileDialogResult	*theResult,
									const NUTI			*fileUTI,
									const NArray		*formatUTIs)
{	NavMenuItemSpec		firstItem = { kNavMenuItemSpecVersion, kNavApplicationFormat, 0 };
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(mDialog != NULL);



	// Select the parent
	//
	// If we're displaying a sheet, the parent should start off active.
	if (mParent != NULL)
		mParent->Select();



	// Prepare our state
	mKind = theKind;

	mResult.navReply  = NULL;
	mResult.navAction = kNavUserActionNone;
	mResult.theFiles.clear();

	mFormatUTI.Clear();
	mFormatUTIs.Set(NULL);



	// Set the UTIs
	if (fileUTI != NULL)
		mFormatUTI = *fileUTI;

	if (formatUTIs != NULL)
		mFormatUTIs = *formatUTIs;



	// Set the filter
	//
	// Choosing a file may require a filter; we use NavDialogSetFilterTypeIdentifiers
	// to provide UTI-based filtering when we can.
	if (theKind == kNavChooseFile && mFormatUTIs.IsNotEmpty())
		{
		if (NavDialogSetFilterTypeIdentifiers != NULL)
			{
			theErr = NavDialogSetFilterTypeIdentifiers(mDialog, mFormatUTIs);
			NN_ASSERT_NOERR(theErr);
			}
		}



	// Set the UTI
	//
	// When a file is saved and there is a list of formats it can be saved as, we
	// validate the current format is part of this list.
	//
	// If not, we force a fake menu selection through to ensure the initial file
	// name reflects a valid format.
	//
	// This allows callers that act as viewers for a UTI to invoke us with the
	// current UTI of the file and a list of formats they can save to, and assume
	// that we will always return a UTI from one of the list of saveable formats.
	if (theKind == kNavChooseSave && mFormatUTIs.IsNotEmpty())
		{
		if (mFormatUTI.IsValid() && !mFormatUTIs.HasValue(mFormatUTI.GetValue()))
			DoPopupMenuSelect(&firstItem);
		}



	// Do the dialog
	//
	// If we're displaying a sheet, NavDialogRun will return immediately
	// and the final result will be broadcast when the sheet is dismissed.
	theErr = NavDialogRun(mDialog);
	if (theErr != noErr)
		mResult.navAction = kNavUserActionCancel;



	// Return the result
	if (theResult != NULL)
		*theResult = mResult;
	
	return(mResult.navAction);
}





//============================================================================
//		NFileDialog::GetFormatList : Get the format list.
//----------------------------------------------------------------------------
NArray NFileDialog::GetFormatList(const NArray &formatUTIs)
{	UInt32			n, numFormats;
	NArray			theFormats;
	NUTI			theUTI;



	// Build the format list
	numFormats = formatUTIs.GetSize();
	
	for (n = 0; n < numFormats; n++)
		{
		theUTI = formatUTIs.GetValueString(n);
		theFormats.AppendValueString(theUTI.GetDescription());
		}
	
	return(theFormats);
}





//============================================================================
//		NFileDialog::FilterUTI : Filter by UTI.
//----------------------------------------------------------------------------
bool NFileDialog::FilterUTI(const NFile &theFile, bool &showFile)
{	bool	didFilter;
	NUTI	theUTI;



	// Check if we can filter or not
	//
	// We use NavDialogSetFilterTypeIdentifiers to provide automatic UTI-based
	// filtering when we can, or our own UTI-based filter for older systems.
	didFilter = (mFormatUTIs.IsNotEmpty() && NavDialogSetFilterTypeIdentifiers == NULL);
	showFile  = false;

	if (didFilter)
		{
		// Apply the filter
		//
		// If the format pop-up is not set to one of our UTIs then it must be
		// kNavAllFilesInPopup, in which case a UTI match is enough to show it.
		//
		// If the format pop-up is set for a specific UTI, we only show the file
		// if it's UTI conforms to the selected format.
		theUTI = theFile.GetUTI();

		if (mFormatUTIs.HasValue(theUTI.GetValue()))
			{
			if (mFormatUTI.IsValid())
				showFile = theUTI.ConformsTo(mFormatUTI);
			else
				showFile = true;
			}
		}
	
	return(didFilter);
}





//============================================================================
//		NFileDialog::DoPopupMenuSelect : Handle kNavCBPopupMenuSelect.
//----------------------------------------------------------------------------
void NFileDialog::DoPopupMenuSelect(const NavMenuItemSpec *navMenu)
{	NString		theName;
	CFRange		dotPos;
	OSStatus	theErr;



	// Update the format
	//
	// The application-defined format items can be distinguished from 
	// any Nav-inserted meta items by examining the menu creator.
	if (navMenu->menuCreator == kNavApplicationFormat)
		mFormatUTI = mFormatUTIs.GetValueString(navMenu->menuType);
	else
		mFormatUTI.Clear();



	// Adjust the extension
	//
	// Since we enable kNavPreserveSaveFileExtension, the file extension can not
	// be changed by the user when saving. To change the extension they must select
	// a different format, at which point we update the extension to match.
	if (mKind == kNavChooseSave && mFormatUTI.IsValid())
		{
		theName = NavDialogGetSaveFileName(mDialog);
		dotPos  = theName.Find(".", kCFCompareBackwards);

		if (dotPos.length == 1)
			{
			theName  = theName.GetLeft(dotPos.location + 1);
			theName += mFormatUTI.GetFileExtension();
			
			theErr = NavDialogSetSaveFileName(mDialog, theName);
			NN_ASSERT_NOERR(theErr);
			}
		}
}





//============================================================================
//		NFileDialog::DoUserAction : Handle kNavCBUserAction.
//----------------------------------------------------------------------------
void NFileDialog::DoUserAction(const NavCBRec *navInfo)
{	NavDialogRef		theDialog;
	bool				haveReply;
	NavReplyRecord		navReply;
	OSStatus			theErr;



	// Get the reply
	haveReply = false;

	mResult.formatUTI = mFormatUTI;
	mResult.navReply  = NULL;
	mResult.navAction = navInfo->userAction;

	if (navInfo->userAction == kNavUserActionOpen   ||
		navInfo->userAction == kNavUserActionSaveAs ||
		navInfo->userAction == kNavUserActionChoose ||
		navInfo->userAction == kNavUserActionNewFolder)
		{
		theErr    = NavDialogGetReply(navInfo->context, &navReply);
		haveReply = (theErr == noErr);
		}



	// Get the files
	if (haveReply)
		{
		mResult.navReply = &navReply;
		mResult.theFiles = NFileUtilities::GetNavReplyFiles(navReply);
		}



	// Broadcast the message
	//
	// We clear our dialog reference before broadcasting, since
	// we may be destroyed as a result of the message.
	theDialog = mDialog;
	mDialog   = NULL;
	mKind     = kNavNone;

	BroadcastMessage(kMsgFileDialogResult, &mResult);



	// Clean up
	if (haveReply)
		NavDisposeReply(&navReply);
		
	NavDialogDispose(theDialog);
}





//============================================================================
//		NFileDialog::NavFilter : Navigation filter callback.
//----------------------------------------------------------------------------
Boolean NFileDialog::NavFilter(AEDesc *theItem, void */*info*/, void *callBackUD, NavFilterModes filterMode)
{	NFileDialog			*thisPtr = (NFileDialog *) callBackUD;
	FileDialogFilter	theMsg;



	// Check for files
	//
	// Nav Services may send non-file objects through the filter, which we discard.
	theMsg.theFile = NFileUtilities::GetAEFile(theItem);
	if (!theMsg.theFile.Exists())
		return(false);



	// Filter by UTI
	//
	// We use NavDialogSetFilterTypeIdentifiers to provide UTI-based filtering when we can.
	//
	// This filters out files before our callback is invoked, and so to provide consistent
	// behaviour when NavDialogSetFilterTypeIdentifiers is not available we perform any UTI
	// filtering before letting the app examine the file.
	if (thisPtr->FilterUTI(theMsg.theFile, theMsg.showFile))
		return(theMsg.showFile);



	// Broadcast the message
	theMsg.fileDialog = thisPtr;
	theMsg.filterMode = filterMode;
	theMsg.showFile   = true;
	
	thisPtr->BroadcastMessage(kMsgFileDialogFilter, &theMsg);
	
	return(theMsg.showFile);
}





//============================================================================
//		NFileDialog::NavEvent : Navigation event callback.
//----------------------------------------------------------------------------
void NFileDialog::NavEvent(NavEventCallbackMessage callBackSelector, NavCBRecPtr callBackParms, void *callBackUD)
{	NFileDialog			*thisPtr = (NFileDialog *) callBackUD;
	FileDialogEvent		theMsg;



	// Ignore the terminate message
	//
	// When the dialog is closed, we receive kNavCBUserAction followed by kNavCBTerminate.
	//
	// Since dispatching kNavCBUserAction may cause the file dialog to be destroyed (e.g.,
	// if we're prompting to save changes while quitting, and the user selects discard),
	// we just ignore the final terminate message.
	if (callBackSelector == kNavCBTerminate)
		return;



	// Ignore programmatic closes
	//
	// If the dialog is closed programmatically, we don't broadcast the final result
	// message since code which called Cancel has asked us to go away (so we suppress
	// the redundant result message).
	if (callBackSelector == kNavCBUserAction && callBackParms->userAction == kNavUserActionNone)
		return;



	// Broadcast the message
	theMsg.fileDialog = thisPtr;
	theMsg.navMsg     = callBackSelector;
	theMsg.navData    = callBackParms;
	
	thisPtr->BroadcastMessage(kMsgFileDialogEvent, &theMsg);



	// Process the message
	switch (callBackSelector) {
		case kNavCBPopupMenuSelect:
			thisPtr->DoPopupMenuSelect((NavMenuItemSpec *) callBackParms->eventData.eventDataParms.param);
			break;

		case kNavCBUserAction:
			thisPtr->DoUserAction(callBackParms);
			break;
		}
}






