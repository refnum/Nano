/*	NAME:
		NReflectionView.cpp

	DESCRIPTION:
		Reflection view.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NSystemUtilities.h"
#include "NCGContext.h"
#include "NReflectionView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagSourceSignature								= 'rsig';
static const OSType kTagSourceID									= 'rsid';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NReflectionView, "com.nano.nreflectionview", kHIViewClassID);





//============================================================================
//		NReflectionView::NReflectionView : Constructor.
//----------------------------------------------------------------------------
NReflectionView::NReflectionView(HIWindowRef		theWindow,
									const HIRect	&theRect,
									const HIViewID	&theID)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetSource(theID);
}





//============================================================================
//		NReflectionView::~NReflectionView : Destructor.
//----------------------------------------------------------------------------
NReflectionView::~NReflectionView(void)
{
}





//============================================================================
//		NReflectionView::GetSource : Get the source.
//----------------------------------------------------------------------------
HIViewID NReflectionView::GetSource(void) const
{


	// Get the source
	return(mSourceID);
}





//============================================================================
//		NReflectionView::SetSource : Set the source.
//----------------------------------------------------------------------------
void NReflectionView::SetSource(const HIViewID &theID)
{


	// Set the source
	mIsValid  = false;
	mSourceID = theID;
	
	SetNeedsDisplay();
}





//============================================================================
//		NReflectionView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NReflectionView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlBoundsChanged },
										{ kEventClassControl, kEventControlDraw			 },
										{ kEventClassNone,    kEventKindNone			 } };



	// Initialize ourselves
	mIsValid  = false;
	mSourceID = kHIViewIDNone;



	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	if (HasProperty(                            kPropertyNano, kTagSourceSignature))
		mSourceID.signature = GetPropertyOSType(kPropertyNano, kTagSourceSignature);

	if (HasProperty(                            kPropertyNano, kTagSourceID))
		mSourceID.id        = GetPropertyUInt32(kPropertyNano, kTagSourceID);
}





//============================================================================
//		NReflectionView::DoEventControlBoundsChanged : Handle kEventControlBoundsChanged.
//----------------------------------------------------------------------------
OSStatus NReflectionView::DoEventControlBoundsChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	mIsValid = false;
	SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NReflectionView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NReflectionView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext		cgContext(theEvent);
	HIViewRef		srcView;
	HIRect			theRect;



	// Get the state we need
	theRect = GetBounds();
	srcView = NULL;
	
	if (mSourceID != kHIViewIDNone)
		HIViewFindByID(HIViewGetRoot(GetHIWindow()), mSourceID, &srcView);



	// Update the mask
	UpdateMask();
	
	if (!mIsValid)
		srcView = NULL;



	// Draw the reflection
	//
	// To draw our reflection, we flip the context and ask the source view
	// to render itself into our context.
	if (srcView != NULL)
		{
		cgContext.TranslateCTM(0, theRect.size.height);
		cgContext.ScaleCTM(1.0f, -1.0f);

		CGContextClipToMask(cgContext, theRect, mMask);
		theEvent.SendToHIView(srcView, kEventTargetDontPropagate);
		}


	// Or a frame in debug builds
	else if (NN_DEBUG)
		{
		cgContext.SetStrokeColor(kColorRed);
		cgContext.StrokeRect(theRect);
		}

	return(noErr);
}





//============================================================================
//		NReflectionView::UpdateMask : Update the mask.
//----------------------------------------------------------------------------
#pragma mark -
void NReflectionView::UpdateMask(void)
{	UInt32		y, theWidth, theHeight, grayByte;
	float		theAlpha;
	NData		theData;
	HIRect		theRect;
	UInt8		*rowPtr;



	// Check our state
	if (mIsValid)
		return;



	// Get the state we need
	theRect   = GetBounds();
	theWidth  = (UInt32) theRect.size.width;
	theHeight = (UInt32) theRect.size.height;



	// Prepare the mask
	if (!theData.SetSize(theWidth * theHeight))
		return;

	rowPtr = theData.GetData();

	for (y = 0; y < theHeight; y++)
		{
		theAlpha = (float) (theHeight - y) / (float) theHeight;
		theAlpha = (theAlpha - 0.3f) * 0.5f;

		theAlpha = GET_CLAMPED_VALUE(theAlpha, 0.0f, 1.0f);
		grayByte = (UInt8) (theAlpha * 255.0f);

		memset(rowPtr, grayByte, theWidth);
		rowPtr += theWidth;
		}



	// Create the image
	mMask.Set(NCoreGraphicsUtilities::CreateImageFromData(theData,
															theWidth, theHeight,
															theWidth, 8, 8,
															kCGImageAlphaNone,
															NCoreGraphicsUtilities::GetColorSpaceGray()));

	mIsValid = mMask.IsValid();
}

