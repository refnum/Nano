/*	NAME:
		CWindowDocument.cpp

	DESCRIPTION:
		ÇPROJECTNAMEÈ document window.
	
	COPYRIGHT:
		Copyright (c) ÇYEARÈ, ÇORGANIZATIONNAMEÈ

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ÇPROJECTNAMEÈ Build.h"
#include "CApplication.h"
#include "CSheetDocumentInfo.h"
#include "CWindowDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Properties
//
// These values may be archived to disk, and can never change.
static const NString kDocAuthor									= "DocAuthor";
static const NString kDocDescription							= "DocDescription";





//============================================================================
//		CWindowDocument::CWindowDocument : Constructor.
//----------------------------------------------------------------------------
CWindowDocument::CWindowDocument(void)
{
}





//============================================================================
//		CWindowDocument::~CWindowDocument : Destructor.
//----------------------------------------------------------------------------
CWindowDocument::~CWindowDocument(void)
{
}





//============================================================================
//		CWindowDocument::GetInfo : Get the document info.
//----------------------------------------------------------------------------
DocumentInfo CWindowDocument::GetInfo(void) const
{


	// Get the info
	return(mInfo);
}





//============================================================================
//		CWindowDocument::SetInfo : Set the document info.
//----------------------------------------------------------------------------
void CWindowDocument::SetInfo(const DocumentInfo &theInfo)
{


	// Record the undo
	RecordUndo(BindSelf(CWindowDocument::SetInfo, mInfo), kStringUndoInfo);
	


	// Set the info
	mInfo = theInfo;
}





//============================================================================
//		CWindowDocument::InitializeProperties : Initialize the properties.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowDocument::InitializeProperties(const NDictionary *appProperties)
{	NDictionary		*theProperties;
	OSStatus		theErr;



	// Initialize the base class
	theErr = NDocument::InitializeProperties(appProperties);
	if (theErr != noErr)
		return(theErr);



	// Initialize the properties
	theProperties = GetProperties();

	theProperties->SetValueString(kDocFileUTIKey, kDocFileUTI);

	return(theErr);
}





//============================================================================
//		CWindowDocument::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::InitializeSelf(void)
{	OSStatus	theErr;



	// Initialize the base class
	theErr = NDocument::InitializeSelf();
	if (theErr != noErr)
		return(theErr);



	// Initialize ourselves
	mInfo.theAuthor.Set(     CSCopyUserName(false));
	mInfo.theDescription.Set(CSCopyMachineName());



	// Install the toolbar
	mToolbar = new CToolbarDocument;
	mToolbar->Attach(GetWindow());
	
	SyncToolbar();



	// Register for events
	mToolbar->AddListener(this);

	return(theErr);
}





//============================================================================
//		CWindowDocument::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CWindowDocument::DoMessage(BroadcastMsg theMsg, void *msgData)
{


	// Handle the message
	switch (theMsg) {
		case kMsgColorPanelTrackBegin:
		case kMsgColorPanelTrackEnd:
			TrackUndo(theMsg == kMsgColorPanelTrackBegin);
			break;

		case kMsgColorPanelChanged:
			SetColor(mToolbar->GetColorPicker()->GetColor());
			break;
		
		case kMsgToolbarItemAdded:
		case kMsgToolbarItemRemoved:
		case kMsgToolbarLayoutChanged:
			SyncToolbar();
			break;
		
		default:
			NDocument::DoMessage(theMsg, msgData);
			break;
		}
}





//============================================================================
//		CWindowDocument::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd = theEvent.GetHICommand();
	
	switch (hiCmd.commandID) {
		case kCmdDocumentInfo:
			theErr = CommandInfo();
			break;

		default:
			theErr = NDocument::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		CWindowDocument::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus CWindowDocument::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;
	bool			hasSheet;



	// Get the command status
	hasSheet = GetWindow()->HasSheet();
	
	switch (hiCmd.commandID) {
		case kCmdDocumentInfo:
			theStatus = EnableIf(!hasSheet);
			break;
		
		default:
			theStatus = NDocument::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		CWindowDocument::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowDocument::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	bool	changedAll;



	// Update our state
	changedAll = (theKey == kPrefChangedAllKey);
	
	if (changedAll || theKey == kPrefWindowBackgroundKey)
		GetWindow()->SetContentColor(thePrefs->GetValueColor(kPrefWindowBackgroundKey));
}





//============================================================================
//		CWindowDocument::EncodeProperties : Encode the properties.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::EncodeProperties(NDictionary &theProperties)
{	OSStatus	theErr;



	// Encode the base class
	theErr = NDocument::EncodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Encode the properties
	theProperties.SetValueString(kDocAuthor,      mInfo.theAuthor);
	theProperties.SetValueString(kDocDescription, mInfo.theDescription);
	
	theErr = mExampleView->EncodeProperties(theProperties);
	
	return(theErr);
}





//============================================================================
//		CWindowDocument::DecodeProperties : Decode the properties.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::DecodeProperties(const NDictionary &theProperties)
{	OSStatus	theErr;



	// Decode the base class
	theErr = NDocument::DecodeProperties(theProperties);
	if (theErr != noErr)
		return(theErr);



	// Decode the properties
	mInfo.theAuthor      = theProperties.GetValueString(kDocAuthor);
	mInfo.theDescription = theProperties.GetValueString(kDocDescription);

	theErr = mExampleView->DecodeProperties(theProperties);



	// Update the toolbar
	SyncToolbar();

	return(theErr);
}





//============================================================================
//		CWindowDocument::GetPrintView : Get the view to print.
//----------------------------------------------------------------------------
NHIView *CWindowDocument::GetPrintView(void)
{


	// Get the view to print
	return(mExampleView);
}





//============================================================================
//		CWindowDocument::CommandInfo : Handle kCmdDocumentInfo.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowDocument::CommandInfo(void)
{


	// Handle the command
	//
	// Documents typically need to respond to commands, which they receive
	// by overriding DoEventCommandProcess.
	//
	// In this example, we handle kCmdDocumentInfo by invoking our editor
	// sheet. This sheet will retrieve our state on creation, and update us
	// if the user accepts the sheet.
	return(BeginSheet(CSheetDocumentInfo::Create()));
}





//============================================================================
//		CWindowDocument::SyncToolbar : Synchronize the toolbar state.
//----------------------------------------------------------------------------
void CWindowDocument::SyncToolbar(void)
{	NColorPicker	*colorPicker;



	// Get the views
	colorPicker = mToolbar->GetColorPicker();



	// Synchronize the toolbar
	if (colorPicker != NULL)
		colorPicker->SetColor(mExampleView->GetColor());



	// Register for events
	//
	// Since the color picker can be replaced in the toolbar configuration
	// sheet, whenever we sync the toolbar we may need to re-listen to a
	// new color picker.
	if (colorPicker != NULL && !IsListeningTo(colorPicker))
		colorPicker->AddListener(this);
}





//============================================================================
//		CWindowDocument::SetColor : Set the color.
//----------------------------------------------------------------------------
void CWindowDocument::SetColor(const NColor &theColor)
{


	// Record the undo
	RecordUndo(BindSelf(CWindowDocument::SetColor, mExampleView->GetColor()), kStringUndoColor);
	


	// Set the color
	mExampleView->SetColor(theColor);
	SyncToolbar();
}




