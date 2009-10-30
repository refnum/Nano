/*	NAME:
		NReflectionView.cpp

	DESCRIPTION:
		Reflection view.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
static const OSType kTagAlpha										= 'alph';





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
//		NReflectionView::GetAlpha : Get the alpha.
//----------------------------------------------------------------------------
float NReflectionView::GetAlpha(void) const
{


	// Get the alpha
	return(mAlpha);
}





//============================================================================
//		NReflectionView::SetAlpha : Set the alpha.
//----------------------------------------------------------------------------
void NReflectionView::SetAlpha(float theValue)
{


	// Set the alpha
	mAlpha     = theValue;

	mMask.Clear();
	SetNeedsDisplay();
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
	mSourceID  = theID;

	mMask.Clear();
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
	mAlpha     = 0.5f;
	mSourceID  = kHIViewIDNone;



	// Register for events
	AddEvents(theEvents);



	// Apply our properties
	if (HasProperty(                            kPropertyNano, kTagSourceSignature))
		mSourceID.signature = GetPropertyOSType(kPropertyNano, kTagSourceSignature);

	if (HasProperty(                            kPropertyNano, kTagSourceID))
		mSourceID.id        = GetPropertyUInt32(kPropertyNano, kTagSourceID);

	if (HasProperty(                            kPropertyNano, kTagAlpha))
		mAlpha             = GetPropertyFloat32(kPropertyNano, kTagAlpha);
}





//============================================================================
//		NReflectionView::DoEventControlBoundsChanged : Handle kEventControlBoundsChanged.
//----------------------------------------------------------------------------
OSStatus NReflectionView::DoEventControlBoundsChanged(NCarbonEvent &/*theEvent*/)
{


	// Update our state
	mMask.Clear();
	SetNeedsDisplay();
	
	return(eventNotHandledErr);
}





//============================================================================
//		NReflectionView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NReflectionView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);
	HIViewRef			srcView;
	HIRect				theRect;



	// Get the state we need
	theRect = GetBounds();
	srcView = NULL;
	
	if (mSourceID != kHIViewIDNone)
		srcView = NHIWindow(GetHIWindow()).GetHIView(mSourceID);



	// Update the mask
	if (!mMask.IsValid())
		mMask.Set(NCoreGraphicsUtilities::CreateReflectionMask(GetBounds().size, mAlpha));
	
	if (!mMask.IsValid())
		srcView = NULL;



	// Draw the reflection
	//
	// To draw our reflection, we flip the context and ask the source view
	// to render itself into our context.
	if (srcView != NULL)
		{
		cgContext.TranslateCTM(0, theRect.size.height);
		cgContext.ScaleCTM(1.0f, -1.0f);

		cgContext.ClipToMask(theRect, mMask);
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

