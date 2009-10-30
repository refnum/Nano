/*	NAME:
		NScrollableView.cpp

	DESCRIPTION:
		Scrollable view.
		
		The scrollable view is an HIView sub-class that supports scrolling and
		zooming, and automatically allows its content to float within the view.

		Views that wish to scroll should subclass HIView as before, but derive
		from NScrollableView rather than NHIView, assign themselves an image
		size based on their content, and implement DrawContent to draw that
		content to the view.
		
		Views that wish to zoom should also assign a list of zoom levels, and
		then adjust the zoom level as required.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIViewUtilities.h"
#include "NCGContext.h"
#include "NScrollableView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagEmbedInScrollView							= 'embd';
static const OSType kTagFrameStyle									= 'fram';


// Misc
static const HISize kDefaultViewLineSize							= { 24.0f, 24.0f };





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NScrollableView, "com.nano.nscrollableview", kHIViewClassID);





//============================================================================
//		NScrollableView::NScrollableView : Constructor.
//----------------------------------------------------------------------------
NScrollableView::NScrollableView(HIWindowRef theWindow, const HIRect &theRect)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);
}





//============================================================================
//		NScrollableView::~NScrollableView : Destructor.
//----------------------------------------------------------------------------
NScrollableView::~NScrollableView(void)
{
}





//============================================================================
//		NScrollableView::GetImageSize : Get the image size.
//----------------------------------------------------------------------------
HISize NScrollableView::GetImageSize(void) const
{


	// Get the image size
	return(mImageSize);
}





//============================================================================
//		NScrollableView::SetImageSize : Set the image size.
//----------------------------------------------------------------------------
void NScrollableView::SetImageSize(const HISize &theSize)
{


	// Set the image size
	mImageSize = theSize;

	UpdateScrollView();
}





//============================================================================
//		NScrollableView::GetViewLineSize : Get the view line size.
//----------------------------------------------------------------------------
HISize NScrollableView::GetViewLineSize(void) const
{


	// Get the view line size
	return(mViewLineSize);
}





//============================================================================
//		NScrollableView::SetViewLineSize : Set the view line size.
//----------------------------------------------------------------------------
void NScrollableView::SetViewLineSize(const HISize &theSize)
{


	// Set the view line size
	mViewLineSize = theSize;

	UpdateScrollView();
}





//============================================================================
//		NScrollableView::GetImageOrigin : Get the image origin.
//----------------------------------------------------------------------------
HIPoint NScrollableView::GetImageOrigin(void) const
{


	// Get the image origin
	return(mImageOrigin);
}





//============================================================================
//		NScrollableView::SetImageOrigin : Set the image origin.
//----------------------------------------------------------------------------
void NScrollableView::SetImageOrigin(const HIPoint &theOrigin)
{	HIPoint		maxOrigin;
	HIRect		theRect;



	// Calculate the maximum origin
	theRect = GetVisibleBounds();
	
	maxOrigin.x = mImageSize.width  - theRect.size.width;
	maxOrigin.y = mImageSize.height - theRect.size.height;



	// Set the image origin
	//
	// The origin is clamped to keep us within our image.
	mImageOrigin.x = std::max(std::min(maxOrigin.x, theOrigin.x), 0.0f);
	mImageOrigin.y = std::max(std::min(maxOrigin.y, theOrigin.y), 0.0f);

	UpdateScrollView();
}





//============================================================================
//		NScrollableView::GetImageBounds : Get the image bounds.
//----------------------------------------------------------------------------
HIRect NScrollableView::GetImageBounds(void) const
{	HIRect		theRect;



	// Get the bounds
	theRect.origin = kHIPointZero;
	theRect.size   = mImageSize;
	
	return(theRect);
}





//============================================================================
//		NScrollableView::GetVisibleBounds : Get the visible bounds.
//----------------------------------------------------------------------------
HIRect NScrollableView::GetVisibleBounds(void) const
{	HISize		viewSize;
	HIRect		theRect;



	// Get the state we need
	viewSize = ConvertViewToImage(GetBounds().size);



	// Get the visible portion of the image
	//
	// If the image is larger than the view, the visible portion of the image
	// is the area bounded by the view starting at the scroll origin.
	//
	// If the image is smaller than the view, the entire image is visible.
	theRect.origin      = mImageOrigin;
	theRect.size.width  = std::min(viewSize.width,  mImageSize.width);
	theRect.size.height = std::min(viewSize.height, mImageSize.height);
	
	return(theRect);
}





//============================================================================
//		NScrollableView::GetVisibleCenter : Get the visible center.
//----------------------------------------------------------------------------
HIPoint NScrollableView::GetVisibleCenter(void) const
{	HIPoint		thePoint;
	HIRect		theRect;



	// Get the visible center
	theRect = GetVisibleBounds();

	thePoint.x = CGRectGetMidX(theRect);
	thePoint.y = CGRectGetMidY(theRect);
	
	return(thePoint);
}





//============================================================================
//		NScrollableView::SetVisibleCenter : Set the visible center.
//----------------------------------------------------------------------------
void NScrollableView::SetVisibleCenter(const HIPoint &thePoint)
{	HIPoint		theOrigin;
	HIRect		theRect;



	// Set the visible center
	theRect = GetVisibleBounds();

	theOrigin.x = thePoint.x - (theRect.size.width  / 2.0f);
	theOrigin.y = thePoint.y - (theRect.size.height / 2.0f);
	
	SetImageOrigin(theOrigin);
}





//============================================================================
//		NScrollableView::GetFrameStyle : Get the frame style.
//----------------------------------------------------------------------------
FrameStyle NScrollableView::GetFrameStyle(void) const
{


	// Get the frame style
	return(mFrameStyle);
}





//============================================================================
//		NScrollableView::SetFrameStyle : Set the frame style.
//----------------------------------------------------------------------------
void NScrollableView::SetFrameStyle(FrameStyle theStyle)
{


	// Set the frame style
	mFrameStyle = theStyle;
	
	SetNeedsDisplay();
}





//============================================================================
//		NScrollableView::GetBackgroundColor : Get the background color.
//----------------------------------------------------------------------------
NColor NScrollableView::GetBackgroundColor(void) const
{


	// Get the background color
	return(mBackground);
}





//============================================================================
//		NScrollableView::SetBackgroundColor : Set the background color.
//----------------------------------------------------------------------------
void NScrollableView::SetBackgroundColor(const NColor &theColor)
{


	// Set the background color
	mBackground = theColor;
	
	SetNeedsDisplay();
}





//============================================================================
//		NScrollableView::GetZoomLevel : Get the zoom level.
//----------------------------------------------------------------------------
UInt32 NScrollableView::GetZoomLevel(void) const
{


	// Get the zoom level
	return(mZoomLevel);
}





//============================================================================
//		NScrollableView::SetZoomLevel : Set the zoom level.
//----------------------------------------------------------------------------
void NScrollableView::SetZoomLevel(UInt32 theLevel)
{	HIPoint		theCenter;



	// Validate our parameters
	NN_ASSERT(theLevel < mZoomLevels.size());



	// Set the zoom level
	//
	// We preserve the current center when zooming.
	theCenter  = GetVisibleCenter();
	mZoomLevel = theLevel;
	
	UpdateScrollView();

	SetVisibleCenter(theCenter);
}





//============================================================================
//		NScrollableView::GetZoomLevels : Get the zoom levels.
//----------------------------------------------------------------------------
ZoomLevels NScrollableView::GetZoomLevels(void) const
{


	// Get the zoom levels
	return(mZoomLevels);
}





//============================================================================
//		NScrollableView::SetZoomLevels : Set the zoom levels.
//----------------------------------------------------------------------------
void NScrollableView::SetZoomLevels(const ZoomLevels &theLevels)
{


	// Validate our parameters
	NN_ASSERT(theLevels.front() >= theLevels.back());
	NN_ASSERT(theLevels.front() >= 1.0f);
	NN_ASSERT(theLevels.back()  <= 1.0f);



	// Set the zoom levels
	mZoomLevels = theLevels;



	// Clamp the zoom level
	if (mZoomLevel >= mZoomLevels.size())
		SetZoomLevel(mZoomLevels.size() - 1);
}





//============================================================================
//		NScrollableView::CanZoomIn : Can the view be zoomed further in?
//----------------------------------------------------------------------------
bool NScrollableView::CanZoomIn(void) const
{


	// Check the zoom level
	return(mZoomLevel > 0);
}





//============================================================================
//		NScrollableView::CanZoomOut : Can the view be zoomed further out?
//----------------------------------------------------------------------------
bool NScrollableView::CanZoomOut(void) const
{


	// Check the zoom level
	return(mZoomLevel < (mZoomLevels.size() - 1));
}





//============================================================================
//		NScrollableView::GetCanvasView : Get the canvas view.
//----------------------------------------------------------------------------
NHIView *NScrollableView::GetCanvasView(bool canCreate)
{	UInt32						n, numChildren;
	NHIViewList					theChildren;
	NHIViewListConstIterator	theIter;



	// Check our state
	if (mCanvas != NULL || !canCreate)
		return(mCanvas);



	// Get the state we need
	numChildren = GetSubViewCount();

	for (n = 0; n < numChildren; n++)
		theChildren.push_back(GetSubView(n));



	// Create the canvas view
	//
	// We turn on embedding for the canvas view, since the canvas-based scroll
	// model is typically used to embed HIViews within a scrollable view.
	//
	// Ideally canvas mode would be supported by HIScrollView, allowing it to
	// support both "view manages the size/origin" and "system moves your view"
	// modes of scrolling.
	//
	// Until then (rdar://4803121) we implement the latter ourselves, by placing
	// a canvas view within the scrolling view which is positioned to reflect the
	// scrolling view's image state.
	mCanvas = NHIViewUtilities::CreateHIView();
	if (mCanvas != NULL)
		{
		mCanvas->SetFeatures(kHIViewFeatureAllowsSubviews);
		AddSubView(mCanvas);
		}
	
	
	
	// Move any existing children to the canvas
	//
	// A scrollable view can be created in a nib with views embedded within it,
	// and we will automatically move the embedded views into the canvas.
	//
	// This allows IB to be used to set up the layout for the scrollable view,
	// and the canvas to take care of positioning those views as the user scrolls.
	for (theIter = theChildren.begin(); theIter != theChildren.end(); theIter++)
		mCanvas->AddSubView(*theIter);



	// Update our state
	SetImageSize(GetBounds().size);
	UpdateCanvasView();

	return(mCanvas);
}





//============================================================================
//		NScrollableView::ConvertViewToImage : Convert view to image coordinates.
//----------------------------------------------------------------------------
HIPoint NScrollableView::ConvertViewToImage(const HIPoint &theValue) const
{	HIPoint		theResult;
	float		theScale;



	// Convert the value
	theScale = GetZoomScale();
	
	theResult.x = ((mImageOrigin.x * theScale) + theValue.x) / theScale;
	theResult.y = ((mImageOrigin.y * theScale) + theValue.y) / theScale;

	return(theResult);
}





//============================================================================
//		NScrollableView::ConvertViewToImage : Convert view to image coordinates.
//----------------------------------------------------------------------------
HISize NScrollableView::ConvertViewToImage(const HISize &theValue) const
{	HISize		theResult;
	float		theScale;



	// Convert the value
	theScale = GetZoomScale();
	
	theResult.width  = theValue.width  / theScale;
	theResult.height = theValue.height / theScale;
	
	return(theResult);
}





//============================================================================
//		NScrollableView::ConvertViewToImage : Convert view to image coordinates.
//----------------------------------------------------------------------------
HIRect NScrollableView::ConvertViewToImage(const HIRect &theValue) const
{	HIRect		theResult;



	// Convert the value
	theResult.origin = ConvertViewToImage(theValue.origin);
	theResult.size   = ConvertViewToImage(theValue.size);
	
	return(theResult);
}





//============================================================================
//		NScrollableView::ConvertImageToView : Convert image to view coordinates.
//----------------------------------------------------------------------------
HIPoint NScrollableView::ConvertImageToView(const HIPoint &theValue) const
{	HIPoint		theResult;
	float		theScale;



	// Convert the value
	theScale = GetZoomScale();
	
	theResult.x = (theValue.x - mImageOrigin.x) * theScale;
	theResult.y = (theValue.y - mImageOrigin.y) * theScale;

	return(theResult);
}





//============================================================================
//		NScrollableView::ConvertImageToView : Convert image to view coordinates.
//----------------------------------------------------------------------------
HISize NScrollableView::ConvertImageToView(const HISize &theValue) const
{	HISize		theResult;
	float		theScale;



	// Convert the value
	theScale = GetZoomScale();
	
	theResult.width  = theValue.width  * theScale;
	theResult.height = theValue.height * theScale;

	return(theResult);
}





//============================================================================
//		NScrollableView::ConvertImageToView : Convert image to view coordinates.
//----------------------------------------------------------------------------
HIRect NScrollableView::ConvertImageToView(const HIRect &theValue) const
{	HIRect		theResult;



	// Convert the value
	theResult.origin = ConvertImageToView(theValue.origin);
	theResult.size   = ConvertImageToView(theValue.size);
	
	return(theResult);
}





//============================================================================
//		NScrollableView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NScrollableView::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl,    kEventControlDraw        },
										{ kEventClassScrollable, kEventScrollableGetInfo  },
										{ kEventClassScrollable, kEventScrollableScrollTo },
										{ kEventClassNone,		 kEventKindNone           } };



	// Register for events
	AddEvents(theEvents);



	// Initialize ourselves
	mImageSize    = kHISizeZero;
	mImageOrigin  = kHIPointZero;
	mViewLineSize = kDefaultViewLineSize;

	mFrameStyle = kFrameThin;
	mBackground = kColorClear;
	
	mZoomLevel = 0;
	mZoomLevels.push_back(1.0f);
	
	mCanvas = NULL;



	// Apply our properties
	if (GetPropertyBoolean(kPropertyNano, kTagEmbedInScrollView))
		NHIViewUtilities::EmbedInScrollView(this);

	if (HasProperty(                                 kPropertyNano, kTagFrameStyle))
		SetFrameStyle((FrameStyle) GetPropertyOSType(kPropertyNano, kTagFrameStyle));
}





//============================================================================
//		NScrollableView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NScrollableView::DoEventControlDraw(NCarbonEvent &theEvent)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);
	float				theScale;
	HIRect				theRect;



	// Get the state we need
	theRect  = GetContentBounds();
	theScale = GetZoomScale();



	// Don't draw in canvas mode
	//
	// If we're providing a canvas model, drawing is handled by the views that
	// embedded within the canvas.
	if (mCanvas != NULL)
		return(eventNotHandledErr);



	// Draw the background
	//
	// The background is drawn in view coordinates, since it may need to draw
	// a frame around the content if the content is floating within the view.
	DrawBackground(theEvent, theRect);



	// Adjust the coordinate system
	//
	// The content is drawn in image coordinates, translating to account for the current
	// origin due to scrolling (or the offset used to center the content if the content
	// was smaller than the view), and scaling to account for the current zoom level.
	//
	// The result is that DrawContent is called, filling GetImageBounds() will fill the
	// entire image.
	cgContext.TranslateCTM(theRect.origin.x, theRect.origin.y);
	cgContext.ScaleCTM(theScale, theScale);

	cgContext.ClipToRect(GetImageBounds());



	// Draw the content
	DrawContent(theEvent);
	
	return(noErr);
}





//============================================================================
//		NScrollableView::DoEventHIObjectInitialize : Handle kEventHIObjectInitialize.
//----------------------------------------------------------------------------
OSStatus NScrollableView::DoEventHIObjectInitialize(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Handle the event
	theErr = NHIView::DoEventHIObjectInitialize(theEvent);
	
	
	
	// Configure the view
	//
	// To support auto-detection of canvas mode, we need to enable embedding for
	// the view prior to initialisating it or any embedded views within us will
	// be removed (presumably as a sanity check).
	//
	// Unfortunately IB does not allow an HIView to have this flag turned on,
	// even though it allows views to be embedded.
	if (theErr == noErr)
		SetFeatures(kHIViewFeatureAllowsSubviews);

	return(theErr);
}





//============================================================================
//		NScrollableView::DoEventScrollableGetInfo : Handle kEventScrollableGetInfo.
//----------------------------------------------------------------------------
OSStatus NScrollableView::DoEventScrollableGetInfo(NCarbonEvent &theEvent)
{	HIPoint		theOrigin;



	// Calculate the origin
	//
	// The scroll view expects the image origin in view coordinates (i.e., without
	// any scaling due to the current zoom level), and as a positive value that
	// gives the delta from the absolute origin.
	theOrigin   = ConvertImageToView(kHIPointZero);
	theOrigin.x = -theOrigin.x;
	theOrigin.y = -theOrigin.y;



	// Update the event
	theEvent.SetParameterHISize( kEventParamViewSize,  GetBounds().size);
	theEvent.SetParameterHISize( kEventParamImageSize, ConvertImageToView(mImageSize));
	theEvent.SetParameterHIPoint(kEventParamOrigin,    theOrigin);
	theEvent.SetParameterHISize( kEventParamLineSize,  mViewLineSize);
	
	return(noErr);
}





//============================================================================
//		NScrollableView::DoEventScrollableScrollTo : Handle kEventScrollableScrollTo.
//----------------------------------------------------------------------------
OSStatus NScrollableView::DoEventScrollableScrollTo(NCarbonEvent &theEvent)
{	HIPoint		theOrigin;
	float		theScale;



	// Scroll the view
	//
	// We don't use SetImageOrigin to change the origin, since this would update
	// the scroll view and cause us to be called recursively.
	//
	// The scroll view sends us the origin in view coordinates (i.e., without any
	// scaling due to the current zoom level), but since ConvertViewToImage adds
	// the current origin we need to translate to image coordinates ourselves.
	theOrigin = theEvent.GetParameterHIPoint(kEventParamOrigin);
	theScale  = GetZoomScale();
	 
	mImageOrigin.x = theOrigin.x / theScale;
	mImageOrigin.y = theOrigin.y / theScale;



	// Update our state
	UpdateCanvasView();

	SetNeedsDisplay();
	
	return(noErr);
}





//============================================================================
//		NScrollableView::DrawBackground : Draw the background.
//----------------------------------------------------------------------------
//		Note :	The base class provides a default implementation that clears
//				the view and frames the content.
//----------------------------------------------------------------------------
void NScrollableView::DrawBackground(NCarbonEvent &theEvent, const HIRect &theContent)
{	NCGContext			cgContext(theEvent);
	StContextState		cgState(cgContext);



	// Draw the background
	cgContext.SetFillColor(mBackground);
	cgContext.FillRect(GetBounds());



	// Draw the frame
	switch (mFrameStyle) {
		case kFrameNone:
			// Nothing to draw
			break;
		
		case kFrameThin:
			cgContext.SetLineWidth(1.0f);
			cgContext.SetStrokeColor(kColorBlack);
			cgContext.StrokeRect(theContent);
			break;
		
		default:
			NN_LOG("Unknown frame: %d", mFrameStyle);
			break;
		}
}





//============================================================================
//		NScrollableView::DrawContent : Draw the content.
//----------------------------------------------------------------------------
//		Note :	The base class provides a default implementation that does
//				nothing, but most sub-classes will override this method to
//				draw their content.
//
//				Canvas-based scroll views will typically not need to implement
//				this method, since their content is normally a set of embedded
//				HIViews (which will handle their own drawing).
//----------------------------------------------------------------------------
void NScrollableView::DrawContent(NCarbonEvent &/*theEvent*/)
{
}





//============================================================================
//		NScrollableView::GetContentBounds : Get the content bounds.
//----------------------------------------------------------------------------
HIRect NScrollableView::GetContentBounds(void) const
{	HISize		viewSize;
	HIRect		theRect;



	// Get the state we need
	theRect  = ConvertImageToView(GetImageBounds());
	viewSize = GetBounds().size;



	// Position the content
	//
	// When floating the content we clamp coordinates to integers, to ensure
	// that the content is always placed at an integral position within the
	// view and we do not introduce fractional pixels when drawing.
	if (theRect.size.width < viewSize.width)
		theRect.origin.x = floorf((viewSize.width  - theRect.size.width)  / 2.0f);

	if (theRect.size.height < viewSize.height)
		theRect.origin.y = floorf((viewSize.height - theRect.size.height) / 2.0f);

	return(theRect);
}





//============================================================================
//		NScrollableView::UpdateScrollView : Update the scroll view.
//----------------------------------------------------------------------------
#pragma mark -
void NScrollableView::UpdateScrollView(void)
{	NCarbonEvent	theEvent(kEventClassScrollable, kEventScrollableInfoChanged);
	NHIView			*scrollView;
	HISize			theSize;
	OSStatus		theErr;



	// Update our size constraints
	//
	// HIScrollView's auto-hide-scroll-bar support queries the size of the image
	// in order to determine when to show/hide the scroll bars.
	//
	// Unfortunately it currently does this using the maximum size constraint of
	// the view, rather than using kEventScrollableGetInfo to obtain the current
	// kEventParamImageSize value (rdar://4083159).
	//
	// To work around this, and allow the scroll bars to show/hide automatically
	// as the image size is changed, we must set the size constraints to match
	// the image size.
	theSize = ConvertImageToView(mImageSize);
	
	SetSizeConstraints(NULL, &theSize);



	// Inform the scroll view
	scrollView = GetSuperView();
	if (scrollView != NULL && scrollView->IsOfClass(kHIScrollViewClassID))
		{
		theErr = theEvent.SendToHIView(*scrollView);
		NN_ASSERT_NOERR(theErr);
		}



	// Update our state
	UpdateCanvasView();

	SetNeedsDisplay();
}





//============================================================================
//		NScrollableView::UpdateCanvasView : Update the canvas view.
//----------------------------------------------------------------------------
void NScrollableView::UpdateCanvasView(void)
{	HIRect		theRect;



	// Update the canvas view
	//
	// If the canvas view exists, its frame is kept in sync with our image.
	if (mCanvas != NULL)
		{
		theRect.origin = CGPointMake(-mImageOrigin.x, -mImageOrigin.y);
		theRect.size   = mImageSize;
		
		mCanvas->SetFrame(theRect);
		}
}





//============================================================================
//		NScrollableView::GetZoomScale : Get the zoom scale.
//----------------------------------------------------------------------------
float NScrollableView::GetZoomScale(void) const
{	float	theScale;



	// Get the zoom scale
	if (mZoomLevels.empty())
		theScale = 1.0f;
	else
		theScale = mZoomLevels[mZoomLevel];
	
	return(theScale);
}

