/*	NAME:
		CWindowDocument.cpp

	DESCRIPTION:
		NanoViewer document window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NFileUtilities.h"
#include "NSTLUtilities.h"
#include "NPasteboard.h"
#include "NMenuItem.h"
#include "NCFBundle.h"

#include "NanoViewer Build.h"
#include "CApplication.h"
#include "CWindowDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const float  kZoomLevels[] = { 16.0f, 8.0f, 4.0f, 2.0f, 1.75f, 1.5f, 1.25f, 1.0f, 0.75f, 0.5f, 0.25f, 0.125f, 0.0625f, 0.03125 };
static const UInt32 kZoomDefault  = 7;





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
//		CWindowDocument::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowDocument::InitializeSelf(void)
{	EventTypeSpec		theEvents[] = {	{ kEventClassWindow, kEventWindowBoundsChanged },
										{ kEventClassNone,   kEventKindNone            } };
	NPasteboard			thePasteboard;
	NCGImage			theImage;
	OSStatus			theErr;



	// Initialize the base class
	theErr = NDocument::InitializeSelf();
	if (theErr != noErr)
		return(theErr);



	// Register for events
	AddEvents(theEvents);



	// Install the toolbar
	mToolbar = new CToolbarDocument;
	mToolbar->Attach(GetWindow());



	// Initialize the image
	mImageView->SetZoomLevels(vector_create(kZoomLevels));
	mImageView->SetZoomLevel(kZoomDefault);

	if (!HasFile())
		{
		theImage = thePasteboard.GetValueImage();
		if (theImage.IsValid())
			{
			SetImage(theImage);
			SetModified(true);
			}
		else
			SetFile(NBundleResource(kImageExample));
		}

	CommandAutoSize();

	return(theErr);
}





//============================================================================
//		CWindowDocument::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void CWindowDocument::DoMessage(BroadcastMsg theMsg, void *msgData)
{	const NFile		*theFile = (const NFile *) msgData;



	// Handle the message
	switch (theMsg) {
		case kMsgImageReceivedFile:
			// Switch to the dropped file
			SetFile(*theFile);
			SetImage(mImageView->GetImage());
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
		case kCmdDocumentRotateLeft:
			theErr = CommandRotate(true);
			break;
			
		case kCmdDocumentRotateRight:
			theErr = CommandRotate(false);
			break;

		case kCmdDocumentFlipHorizontal:
			theErr = CommandFlip(true);
			break;
			
		case kCmdDocumentFlipVertical:
			theErr = CommandFlip(false);
			break;
			
		case kCmdDocumentZoomToFit:
			theErr = CommandZoomToFit();
			break;
			
		case kCmdDocumentActualSize:
			theErr = CommandActualSize();
			break;
			
		case kCmdDocumentAutoSize:
			theErr = CommandAutoSize();
			break;
			
		case kCmdDocumentZoomIn:
		case kCmdDocumentZoomIn_Hidden:
			theErr = CommandZoomIn();
			break;
		
		case kCmdDocumentZoomOut:
			theErr = CommandZoomOut();
			break;

		case kHICommandCopy:
			theErr = mImageView->DoEditCommand(hiCmd.commandID);
			break;

		default:
			theErr = NDocument::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		CWindowDocument::DoEventWindowBoundsChanged : Handle kEventWindowBoundsChanged.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::DoEventWindowBoundsChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	UpdateToolbarStatus();
	
	return(eventNotHandledErr);
}





//============================================================================
//		CWindowDocument::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus CWindowDocument::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;



	// Get the command status
	switch (hiCmd.commandID) {
		case kCmdDocumentRotateLeft:
		case kCmdDocumentRotateRight:
			theStatus = kCommandEnable;
			break;

		case kCmdDocumentZoomToFit:
			theStatus = EnableIf(mImageView->GetImageBounds().size != mImageView->GetBounds().size);
			break;

		case kCmdDocumentActualSize:
			theStatus = EnableIf(mImageView->GetScaling() != kScaleNone);
			break;

		case kCmdDocumentAutoSize:
			theStatus = EnableIf(mImageView->GetScaling() != kScaleProportionally);
			break;

		case kCmdDocumentZoomIn:
		case kCmdDocumentZoomIn_Hidden:
			theStatus = EnableIf(mImageView->CanZoomIn());
			break;
		
		case kCmdDocumentZoomOut:
			theStatus = EnableIf(mImageView->CanZoomOut());
			break;

		case kHICommandCopy:
			theStatus = mImageView->GetEditStatus(hiCmd.commandID);
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
//		CWindowDocument::LoadSelf : Load ourselves.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::LoadSelf(void)
{	NCGImage	theImage;



	// Load the image
	if (!theImage.Set(NCoreGraphicsUtilities::CreateImageFromFile(GetFile())))
		return(badFormat);
	
	SetImage(theImage);

	return(noErr);
}





//============================================================================
//		CWindowDocument::SaveSelf : Save ourselves.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::SaveSelf(void)
{	NData		theData;
	NFile		theFile;
	NUTI		theUTI;
	OSStatus	theErr;



	// Get the state we need
	theFile = GetFile();
	theUTI  = GetFile().GetUTI();
	


	// Save the image
	theData = NCoreGraphicsUtilities::CreateDataFromImage(mImageView->GetImage(), theUTI);
	theErr  = NFileUtilities::SetFileData(theFile, theData);

	return(theErr);
}





//============================================================================
//		CWindowDocument::GetPrintView : Get the view to print.
//----------------------------------------------------------------------------
NHIView *CWindowDocument::GetPrintView(void)
{


	// Get the view to print
	return(mImageView);
}





//============================================================================
//		CWindowDocument::CommandRotate : Rotate the image.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowDocument::CommandRotate(bool rotateLeft)
{	NCGImage	theImage;
	float		rotateBy;
	OSStatus	theErr;



	// Record the undo
	if (rotateLeft)
		RecordUndo(BindSelf(CWindowDocument::CommandRotate, false), kStringUndoRotateLeft);
	else
		RecordUndo(BindSelf(CWindowDocument::CommandRotate,  true), kStringUndoRotateRight);



	// Rotate the image
	theErr   = memFullErr;
	rotateBy = rotateLeft ? 90.0f : -90.0f;

	if (theImage.Set(NCoreGraphicsUtilities::CreateRotatedImage(mImageView->GetImage(), rotateBy)))
		theErr = SetImage(theImage);

	return(theErr);
}





//============================================================================
//		CWindowDocument::CommandFlip : Flip the image.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::CommandFlip(bool flipHoriz)
{	NCGImage	theImage;
	OSStatus	theErr;



	// Record the undo
	if (flipHoriz)
		RecordUndo(BindSelf(CWindowDocument::CommandFlip,  true), kStringUndoFlipHoriz);
	else
		RecordUndo(BindSelf(CWindowDocument::CommandFlip, false), kStringUndoFlipVert);



	// Flip the image
	theErr = memFullErr;

	if (theImage.Set(NCoreGraphicsUtilities::CreateFlippedImage(mImageView->GetImage(), flipHoriz)))
		theErr = SetImage(theImage);

	return(theErr);
}





//============================================================================
//		CWindowDocument::CommandActualSize : Set the image to actual size.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::CommandActualSize(void)
{


	// Adjust the scale
	mImageView->SetScaling(kScaleNone);

	return(SetZoomLevel(kZoomDefault));
}





//============================================================================
//		CWindowDocument::CommandAutoSize : Set the image to auto-size.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::CommandAutoSize(void)
{


	// Adjust the scale
	mImageView->SetScaling(kScaleProportionally);
	
	return(SetZoomLevel(kZoomDefault));
}





//============================================================================
//		CWindowDocument::CommandZoomToFit : Zoom the image to fit.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::CommandZoomToFit(void)
{	HIRect	theRect;



	// Calculate the required size
	theRect = mImageView->GetImage().GetBounds();
	NGeometryUtilities::ScaleToFit(theRect, mImageView->GetBounds());



	// Adjust the scale
	mImageView->SetScaling(kScaleToImage);
	mImageView->SetImageSize(theRect.size);
	
	return(SetZoomLevel(kZoomDefault));
}





//============================================================================
//		CWindowDocument::CommandZoomIn : Zoom in.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::CommandZoomIn(void)
{


	// Make sure we have a fixed-size image
	if (mImageView->GetScaling() == kScaleProportionally)
		CommandZoomToFit();



	// Zoom in
	return(SetZoomLevel(mImageView->GetZoomLevel() - 1));
}





//============================================================================
//		CWindowDocument::CommandZoomOut : Zoom out.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::CommandZoomOut(void)
{


	// Make sure we have a fixed-size image
	if (mImageView->GetScaling() == kScaleProportionally)
		CommandZoomToFit();



	// Zoom out
	return(SetZoomLevel(mImageView->GetZoomLevel() + 1));
}





//============================================================================
//		CWindowDocument::SetZoomLevel : Set the zoom level.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::SetZoomLevel(UInt32 theLevel)
{	NMenuItem		menuItem(kCmdDocumentAutoSize);
	HISize			zoomedSize;
	UniChar			autoSize;



	// Set the zoom level
	mImageView->SetZoomLevel(theLevel);



	// Update our state
	autoSize   = (mImageView->GetScaling() == kScaleProportionally) ? (UniChar) checkMark : (UniChar) noMark;
	zoomedSize = mImageView->ConvertImageToView(mImageView->GetImageSize());
	
	menuItem.SetMark(autoSize);
	GetWindow()->SetIdealSize(zoomedSize);

	UpdateToolbarStatus();

	return(noErr);
}





//============================================================================
//		CWindowDocument::SetImage : Set the image.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::SetImage(const NCGImage &theImage)
{


	// Set the image
	mImageView->SetImage(theImage);



	// Update our state
	GetWindow()->SetIdealSize(theImage.GetSize());
	GetWindow()->Zoom();
	
	return(noErr);
}

