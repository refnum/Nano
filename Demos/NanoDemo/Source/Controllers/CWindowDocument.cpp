/*	NAME:
		CWindowDocument.cpp

	DESCRIPTION:
		NanoDemo document window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NanoDemo Build.h"
#include "CApplication.h"
#include "CWindowDocument.h"





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
		case kMsgColorPickerTrackBegin:
		case kMsgColorPickerTrackEnd:
			TrackUndo(theMsg == kMsgColorPickerTrackBegin);
			break;

		case kMsgColorPickerChanged:
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
		case kCmdDocumentSetShapeTriangle:
		case kCmdDocumentSetShapeRectangle:
		case kCmdDocumentSetShapeCircle:
			theErr = CommandShape(hiCmd.commandID);
			break;

		default:
			theErr = NDocument::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		CWindowDocument::DoPrefChanged : Handle preference changes.
//----------------------------------------------------------------------------
void CWindowDocument::DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey)
{	bool	changedAll;



	// Update our state
	changedAll = (theKey == kPrefChangedAllKey);
	
	if (changedAll || theKey == kPrefEdgeWidthKey)
		mShapeView->SetEdgeWidth((EdgeWidth) thePrefs->GetValueUInt32(kPrefEdgeWidthKey));

	if (changedAll || theKey == kPrefEdgeStyleKey)
		mShapeView->SetEdgeStyle((EdgeStyle) thePrefs->GetValueUInt32(kPrefEdgeStyleKey));

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
	theErr = mShapeView->EncodeProperties(theProperties);
	
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
	theErr = mShapeView->DecodeProperties(theProperties);

	SyncToolbar();

	return(theErr);
}





//============================================================================
//		CWindowDocument::GetPrintView : Get the view to print.
//----------------------------------------------------------------------------
NHIView *CWindowDocument::GetPrintView(void)
{


	// Get the view to print
	return(mShapeView);
}





//============================================================================
//		CWindowDocument::CommandShape : Handle a shape command.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowDocument::CommandShape(UInt32 theCmd)
{


	// Set the shape
	//
	// We allow the event to propogate, so that the menu can update its state.
	switch (theCmd) {
		case kCmdDocumentSetShapeTriangle:
			SetShape(kShapeTriangle);
			break;

		case kCmdDocumentSetShapeRectangle:
			SetShape(kShapeRectangle);
			break;

		case kCmdDocumentSetShapeCircle:
			SetShape(kShapeCircle);
			break;
		
		default:
			NN_LOG("Unknown command: %d", theCmd);
			break;
		}

	return(eventNotHandledErr);
}





//============================================================================
//		CWindowDocument::SyncToolbar : Synchronize the toolbar state.
//----------------------------------------------------------------------------
void CWindowDocument::SyncToolbar(void)
{	NColorPicker		*colorPicker;
	NPopupButton		*shapeMenu;
	UInt32				theCmd;



	// Get the views
	shapeMenu   = mToolbar->GetShapeMenu();
	colorPicker = mToolbar->GetColorPicker();



	// Synchronize the toolbar
	switch (mShapeView->GetShape()) {
		case kShapeTriangle:
			theCmd = kCmdDocumentSetShapeTriangle;
			break;

		case kShapeRectangle:
			theCmd = kCmdDocumentSetShapeRectangle;
			break;

		case kShapeCircle:
			theCmd = kCmdDocumentSetShapeCircle;
			break;
		
		default:
			theCmd = kCmdDocumentSetShapeTriangle;
			NN_LOG("Unknown shape: %d", mShapeView->GetShape());
			break;
		}
	
	if (shapeMenu != NULL)
		shapeMenu->SetCurrentValueByCommand(theCmd);

	if (colorPicker != NULL)
		colorPicker->SetColor(mShapeView->GetFillColor());



	// Register for events
	//
	// Since the color picker can be replaced in the toolbar configuration
	// sheet, whenever we sync the toolbar we may need to re-listen to a
	// new color picker.
	if (colorPicker != NULL && !IsListeningTo(colorPicker))
		colorPicker->AddListener(this);
}





//============================================================================
//		CWindowDocument::SetShape : Set the shape.
//----------------------------------------------------------------------------
void CWindowDocument::SetShape(ShapeType theShape)
{


	// Record the undo
	RecordUndo(BindSelf(CWindowDocument::SetShape, mShapeView->GetShape()), kStringUndoShape);



	// Set the shape
	mShapeView->SetShape(theShape);
	SyncToolbar();
}





//============================================================================
//		CWindowDocument::SetColor : Set the color.
//----------------------------------------------------------------------------
void CWindowDocument::SetColor(const NColor &theColor)
{


	// Record the undo
	RecordUndo(BindSelf(CWindowDocument::SetColor, mShapeView->GetFillColor()), kStringUndoColor);
	


	// Set the color
	mShapeView->SetFillColor(theColor);
	SyncToolbar();
}

