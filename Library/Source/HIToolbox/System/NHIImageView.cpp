/*	NAME:
		NHIImageView.cpp

	DESCRIPTION:
		Image view.
		
		Although HIImageView responds to kEventClassScrollable itself, we
		replace that behaviour with that of NScrollableView.
		
		NScrollableView gives us additional capabilities over the system image
		view (e.g., floating or zooming the content), and by handling drawing
		we can also implement features such as proportional scaling.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NTextUtilities.h"
#include "NPasteboard.h"
#include "NCFBundle.h"
#include "NCGContext.h"
#include "NHIImageView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagImage										= 'imag';
static const OSType kTagImageScaling								= 'scal';
static const OSType kTagImageEditable								= 'edit';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHIImageView, kHIImageViewClassID, NULL);





//============================================================================
//		NHIImageView::NHIImageView : Constructor.
//----------------------------------------------------------------------------
NHIImageView::NHIImageView(HIWindowRef theWindow, const HIRect &theRect, const NCGImage *theImage)
{	CGImageRef	cgImage;
	HIViewRef	theView;
	OSStatus	theErr;



	// Get the image
	cgImage = NULL;
	
	if (theImage != NULL)
		cgImage = *theImage;



	// Create the view
	theErr = HIImageViewCreate(cgImage, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NHIImageView::~NHIImageView : Destructor.
//----------------------------------------------------------------------------
NHIImageView::~NHIImageView(void)
{
}





//============================================================================
//		NHIImageView::GetImage : Get the image.
//----------------------------------------------------------------------------
NCGImage NHIImageView::GetImage(void) const
{	NCGImage	theImage;



	// Get the image
	theImage.Set(HIImageViewCopyImage(*this));
	
	return(theImage);
}





//============================================================================
//		NHIImageView::SetImage : Set the image.
//----------------------------------------------------------------------------
void NHIImageView::SetImage(const NCGImage &theImage)
{	OSStatus	theErr;



	// Set the image
	theErr = HIImageViewSetImage(*this, theImage);
	NN_ASSERT_NOERR(theErr);



	// Update our state
	//
	// We need to re-apply the current scaling, since we may need to adjust
	// our scrollable image size to be different from the image's actual size.
	SetImageSize(theImage.GetSize());
	
	SetScaling(GetScaling());
}





//============================================================================
//		NHIImageView::GetAlpha : Get the image alpha.
//----------------------------------------------------------------------------
float NHIImageView::GetAlpha(void) const
{


	// Get the alpha value
	return(HIImageViewGetAlpha(*this));
}





//============================================================================
//		NHIImageView::SetAlpha : Set the image alpha.
//----------------------------------------------------------------------------
void NHIImageView::SetAlpha(float theAlpha)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theAlpha >= 0.0f && theAlpha <= 1.0f);
	


	// Set the alpha value
	theErr = HIImageViewSetAlpha(*this, theAlpha);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIImageView::GetScaling : Get the image scaling.
//----------------------------------------------------------------------------
ImageScaling NHIImageView::GetScaling(void) const
{


	// Get the image scaling
	return(mScaling);
}





//============================================================================
//		NHIImageView::SetScaling : Set the image scaling.
//----------------------------------------------------------------------------
void NHIImageView::SetScaling(ImageScaling theScaling)
{	HISize	theSize;



	// Set the image scaling
	//
	// We handle all scaling for the view, so turn off HIImageView's scaling.
	HIImageViewSetScaleToFit(*this, false);

	mScaling = theScaling;



	// Update the view
	//
	// The size of image exposed to any enclosing scroll view is determined by
	// the current scaling mode - when scaling, the image is our view bounds.
	if (theScaling == kScaleNone || theScaling == kScaleToImage)
		theSize = GetImage().GetSize();
	else
		theSize = GetBounds().size;

	SetImageSize(theSize);
}





//============================================================================
//		NHIImageView::IsOpaque : Is the image opaque?
//----------------------------------------------------------------------------
bool NHIImageView::IsOpaque(void) const
{


	// Get the opaque state
	return(HIImageViewIsOpaque(*this));
}





//============================================================================
//		NHIImageView::SetOpaque : Set the opaque state.
//----------------------------------------------------------------------------
void NHIImageView::SetOpaque(bool isOpaque)
{	OSStatus	theErr;



	// Set the opaque value
	theErr = HIImageViewSetOpaque(*this, isOpaque);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIImageView::IsEditable : Is the image editable?
//----------------------------------------------------------------------------
bool NHIImageView::IsEditable(void) const
{


	// Get the editable state
	return(mIsEditable);
}





//============================================================================
//		NHIImageView::SetEditable : Set the editable state.
//----------------------------------------------------------------------------
void NHIImageView::SetEditable(bool isEditable)
{	NArray		theTypes;



	// Set the editable value
	mIsEditable = isEditable;



	// Enable/disable drag+drop
	if (mIsEditable)
		theTypes = NArray(kUTNTypeFileURL, kUTNTypeImage, NULL);
	
	SetDraggedTypes(theTypes);
}





//============================================================================
//		NHIImageView::GetEditStatus : Get the status of an edit command.
//----------------------------------------------------------------------------
CommandStatus NHIImageView::GetEditStatus(UInt32 theCmd)
{	NPasteboard		thePasteboard;
	CommandStatus	theStatus;
	bool			hasImage;



	// Check our state
	if (!IsEditable())
		return(kCommandDisable);



	// Get the command status
	hasImage = GetImage().IsValid();
	
	switch (theCmd) {
		case kHICommandCut:
		case kHICommandCopy:
		case kHICommandClear:
			theStatus = EnableIf(hasImage);
			break;

		case kHICommandPaste:
			theStatus = EnableIf(thePasteboard.HasFlavor(kUTNTypeImage));
			break;
			
		default:
			NN_LOG("Unknown edit command: %s", NTextUtilities::FromOSType(theCmd).GetUTF8());
			theStatus = kCommandDisable;
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		NHIImageView::DoEditCommand : Perform an edit command.
//----------------------------------------------------------------------------
OSStatus NHIImageView::DoEditCommand(UInt32 theCmd)
{	NPasteboard		thePasteboard;
	NCGImage		theImage;



	// Validate our state
	NN_ASSERT(IsEditable());



	// Perform the command
	switch (theCmd) {
		case kHICommandCut:
			DoEditCommand(kHICommandCopy);
			DoEditCommand(kHICommandClear);
			break;

		case kHICommandCopy:
			thePasteboard.AddValueImage(GetImage());
			break;

		case kHICommandPaste:
			theImage = thePasteboard.GetValueImage();
			if (theImage.IsValid())
				SetImage(theImage);
			else
				SysBeep(30);
			break;

		case kHICommandClear:
			SetImage(NULL);
			break;
			
		default:
			NN_LOG("Unknown edit command: %s", NTextUtilities::FromOSType(theCmd).GetUTF8());
			break;
		}
	
	return(noErr);
}





//============================================================================
//		NHIImageView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NHIImageView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlBoundsChanged },
										{ kEventClassNone,    kEventKindNone             } };



	// Initialize the base class
	NScrollableView::InitializeView();



	// Initialize ourselves
	mScaling    = kScaleNone;
	mIsEditable = false;



	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	//
	// We treat the IB "scale to fit" state as a psuedo-property, honouring the IB
	// checkbox and then overriding it with the scaling property if present.
	SetScaling(HIImageViewGetScaleToFit(*this) ? kScaleToFit : kScaleNone);

	if (HasProperty(                               kPropertyNano, kTagImage))
		SetImage(NBundleResource(GetPropertyString(kPropertyNano, kTagImage)));

	if (HasProperty(                                kPropertyNano, kTagImageScaling))
		SetScaling((ImageScaling) GetPropertyOSType(kPropertyNano, kTagImageScaling));

	if (HasProperty(                   kPropertyNano, kTagImageEditable))
		SetEditable(GetPropertyBoolean(kPropertyNano, kTagImageEditable));
}





//============================================================================
//		NHIImageView::DoEventControlBoundsChanged : Handle kEventControlBoundsChanged.
//----------------------------------------------------------------------------
OSStatus NHIImageView::DoEventControlBoundsChanged(NCarbonEvent &theEvent)
{	UInt32		theAttributes;
	HIRect		theRect;



	// Update the image size
	//
	// If the image is being scaled, the image size needs to be kept in sync
	// with the view size so that our scroll bars show/hide as necessary.
	if (mScaling == kScaleToFit || mScaling == kScaleProportionally)
		{
		theAttributes = theEvent.GetParameterUInt32(kEventParamAttributes);
		if (theAttributes & kControlBoundsChangeSizeChanged)
			{
			theRect = theEvent.GetParameterHIRect(kEventParamCurrentBounds);
			SetImageSize(theRect.size);
			}
		}

	return(eventNotHandledErr);
}





//============================================================================
//		NHIImageView::DoDragReceive : Receive a drag.
//----------------------------------------------------------------------------
DragActions NHIImageView::DoDragReceive(DragRef theDrag)
{	NPasteboard		pasteBoard(theDrag);
	NCGImage		theImage;
	NFile			theFile;



	// Accept dragged images
	theImage = pasteBoard.GetValueImage();
	if (theImage.IsValid())
		{
		SetImage(theImage);
		return(kDragActionGeneric);
		}



	// Accept dragged files
	theFile = pasteBoard.GetValueFile();
	if (theFile.IsValid())
		{
		theImage = NCoreGraphicsUtilities::CreateImageFromFile(theFile);
		if (theImage.IsValid())
			{
			SetImage(theImage);
			BroadcastMessage(kMsgImageReceivedFile, &theFile);

			return(kDragActionGeneric);
			}
		}



	// Reject anything else
	SysBeep(30);

	return(kDragActionNothing);
}





//============================================================================
//		NHIImageView::DrawContent : Draw the view.
//----------------------------------------------------------------------------
void NHIImageView::DrawContent(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	HIRect			theBounds, imageRect;
	NCGImage		cgImage;



	// Get the state we need
	cgImage = GetImage();
	if (!cgImage.IsValid())
		return;
	
	theBounds = GetBounds();
	imageRect = cgImage.GetBounds();



	// Position the image
	switch (mScaling) {
		case kScaleNone:
			break;

		case kScaleToImage:
			imageRect.size = GetImageSize();
			break;

		case kScaleToFit:
			imageRect = theBounds;
			break;

		case kScaleProportionally:
			NGeometryUtilities::ScaleToFit(imageRect, theBounds);
			break;

		default:
			NN_LOG("Unknown scale: %s", NTextUtilities::FromOSType(mScaling).GetUTF8());
			break;
		}



	// Draw the image
	cgContext.SetAlpha(IsOpaque() ? 1.0f : GetAlpha());
	cgContext.DrawImage(cgImage, imageRect);



	// Draw the drag hilite
	if (IsDragTarget())
		cgContext.DrawDragHilite(GetVisibleBounds());
}



