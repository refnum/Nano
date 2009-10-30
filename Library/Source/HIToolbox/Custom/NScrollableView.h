/*	NAME:
		NScrollableView.h

	DESCRIPTION:
		Scrollable view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSCROLLABLEVIEW_HDR
#define NSCROLLABLEVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NHIView.h"
#include "NColor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Frame styles
//
// These values are used for kTagFrameStyle, and can never change.
typedef enum {
	kFrameNone					= 'none',
	kFrameThin					= 'thin'
} FrameStyle;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists
typedef std::vector<float>										ZoomLevels;
typedef ZoomLevels::iterator									ZoomLevelsIterator;
typedef ZoomLevels::const_iterator								ZoomLevelsConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NScrollableView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NScrollableView);

										NScrollableView(HIWindowRef theWindow, const HIRect &theRect);
	virtual								~NScrollableView(void);


	// Get/set the image origin
	//
	// The image origin is in image coordinates.
	HIPoint								GetImageOrigin(void) const;
	void								SetImageOrigin(const HIPoint &theOrigin);


	// Get/set the image size
	//
	// The image size defines the size of the image coordinate system.
	HISize								GetImageSize(void) const;
	void								SetImageSize(const HISize &theSize);


	// Get the view line size
	//
	// The view line size is in view coordinates.
	HISize								GetViewLineSize(void) const;
	void								SetViewLineSize(const HISize &theSize);


	// Get the image bounds
	//
	// The image bounds is the image coordinate system.
	HIRect								GetImageBounds(void) const;


	// Get the visible bounds
	//
	// The visible bounds is the visible area of the image in image coordinates.
	//
	// This area can be used by applications to perform their own culling inside
	// DrawContent, since this method gives the visible area within the CGContext's
	// coordinate system at that point.
	HIRect								GetVisibleBounds(void) const;


	// Get/set the visible center
	//
	// The visible center is the center of the visible area, in image coordinates.
	HIPoint								GetVisibleCenter(void) const;
	void								SetVisibleCenter(const HIPoint &thePoint);
	

	// Get/set the frame style
	FrameStyle							GetFrameStyle(void) const;
	void								SetFrameStyle(FrameStyle theStyle);


	// Get/set the background color
	//
	// The background is transparent by default.
	NColor								GetBackgroundColor(void) const;
	void								SetBackgroundColor(const NColor &theColor);


	// Get/set the zoom level
	//
	// Zoom levels are indexed from 0.
	UInt32								GetZoomLevel(void) const;
	void								SetZoomLevel(UInt32 theLevel);


	// Get/set the zoom levels
	//
	// Each zoom level is a scaling factor, with 1.0 being an unzoomed state.
	//
	// Zoom levels should be sorted, with the most zoomed-in value (larger than 1.0) first
	// and the most zoomed-out value (smaller than 1.0) coming last in the list.
	ZoomLevels							GetZoomLevels(void) const;
	void								SetZoomLevels(const ZoomLevels &theLevels);
	
	
	// Can the view be zoomed?
	bool								CanZoomIn( void) const;
	bool								CanZoomOut(void) const;


	// Get the "canvas" view
	//
	// Returns the "canvas" view within the scrollable view, creating if required.
	//
	// The HIView scroll model is that a view (e.g., a subclass of NScrollableView) is
	// placed within an HIScrollView; that view is then sized to fit, and scrolling is
	// performed by incorporating the image origin/size into hit-testing and drawing
	// for the view (i.e., scrolling does not change the NScrollableView frame).
	//
	// The NSView scroll model is that an NSView is placed within an NSScrollView, and
	// the frame of the NSView provides the "image size" for the scrollable area while
	// the "image origin" reflects the offset of the NSView from the NSScrollView.
	//
	//
	// NScrollableView provides a NSView-style interface for scrolling, by embedding
	// an HIView inside itself whose frame is based on the image size and image origin.
	//
	// This allows HIViews to be used to scroll embedded HIViews, by embedding the
	// sub-views into the canvas view within the scrollable view.
	//
	// The canvas view will be created on demand, and automatically kept in sync with
	// the image size/origin once it exists.
	NHIView								*GetCanvasView(bool canCreate=true);


	// Convert between coordinate systems
	//
	// The view coordinate system is the HIView coordinate system, used by the view frame
	// and events. A distance of 1.0 in this coordinate system corresponds to a single
	// pixel on screen, assuming the UI resolution has not been modified.
	//
	// The image coordinate system is the [GS]etImageSize coordinate system, and holds the
	// content that this view exposes to the scroll view.
	//
	//
	// For views which do not zoom, the image coordinate system is simply the view
	// coordinate system translated by the image origin.
	//
	// For views that can be zoomed, the image coordinate system can also be scaled by the
	// current zoom level.
	//
	//
	// This view adjusts the CGContextRef transform prior to invoking DrawContent, and
	// returns an appropriately scaled image size when communicating with the scroll view.
	//
	// These methods are provided for any additional conversion that may be necessary,
	// such as converting mouse events (which arrive in view coordinates) to image
	// coordinates.
	HIPoint								ConvertViewToImage(const HIPoint &theValue) const;
	HISize								ConvertViewToImage(const HISize  &theValue) const;
	HIRect								ConvertViewToImage(const HIRect  &theValue) const;

	HIPoint								ConvertImageToView(const HIPoint &theValue) const;
	HISize								ConvertImageToView(const HISize  &theValue) const;
	HIRect								ConvertImageToView(const HIRect  &theValue) const;


protected:
	// Initialize the view
	//
	// Sub-classes must call through to the base class.
	virtual void						InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(       NCarbonEvent &theEvent);
	OSStatus							DoEventHIObjectInitialize(NCarbonEvent &theEvent);
	OSStatus							DoEventScrollableGetInfo( NCarbonEvent &theEvent);
	virtual OSStatus					DoEventScrollableScrollTo(NCarbonEvent &theEvent);


	// Draw the view
	//
	// The scrollable view handles kEventControlDraw, and positions the content
	// within the view when drawing.
	//
	// Sub-classes should override these two methods to draw the background and
	// content for the view.
	//
	// The context coordinate system is in view coordinates for DrawBackground,
	// and in image coordinates for DrawContent.
	virtual void						DrawBackground(NCarbonEvent &theEvent, const HIRect &theContent);
	virtual void						DrawContent(   NCarbonEvent &theEvent);


	// Get the content bounds
	//
	// If the image bounds are larger than the view bounds then the content fills
	// the entire view, and the content bounds are simply the image bounds in
	// view coordinates.
	//
	// If the image is smaller than the view bounds, the base class will "float"
	// the image within the view by centering it.
	virtual HIRect						GetContentBounds(void) const;


private:
	void								UpdateScrollView(void);
	void								UpdateCanvasView(void);

	float								GetZoomScale(void) const;
	

private:
	HISize								mImageSize;
	HIPoint								mImageOrigin;
	HISize								mViewLineSize;

	FrameStyle							mFrameStyle;
	NColor								mBackground;

	UInt32								mZoomLevel;
	ZoomLevels							mZoomLevels;

	NHIView								*mCanvas;
};





#endif // NScrollableVIEW_HDR


