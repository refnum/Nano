/*	NAME:
		NSplitterView.h

	DESCRIPTION:
		Splitter view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NSPLITTERVIEW_HDR
#define NSPLITTERVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGShading.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef enum {
	kSplitterThumb,											// Thumb only
	kSplitterStandard,										// Bevel + thumb
	kSplitterHairline,										// Single pixel
	kSplitterTransparent									// Invisible
} SplitterKind;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSplitterView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NSplitterView);

										NSplitterView(HIWindowRef theWindow, const HIRect &theRect);
	virtual								~NSplitterView(void);


	// Get/set the kind
	SplitterKind						GetKind(void) const;
	void								SetKind(SplitterKind theKind);


	// Get/set the orientation
	bool								IsVertical(void) const;
	void								SetVertical(bool isVertical);


	// Get/set the ratio
	//
	// The ratio controls the size of view A relative to the size of the splitter
	// view, ranging from 0 (view A is collapsed) to 1 (view B is collapsed).
	float								GetRatio(void) const;
	void								SetRatio(float theRatio);


	// Get/set the ratio limit
	//
	// The ratio limit must be >= 0 and < 1.0.
	float								GetRatioLimit(void) const;
	void								SetRatioLimit(float theRatio);


	// Get/set the divider size
	UInt32								GetDividerSize(void) const;
	void								SetDividerSize(UInt32 theSize);


	// Get/set the views
	//
	// View A is on the top/left, view B is on the bottom/right.
	NHIView								*GetViewA(void) const;
	NHIView								*GetViewB(void) const;

	void								SetViewA(NHIView *theView);
	void								SetViewB(NHIView *theView);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlBoundsChanged(        NCarbonEvent &theEvent);
	OSStatus							DoEventControlDraw(                 NCarbonEvent &theEvent);
	OSStatus							DoEventControlInterceptSubviewClick(NCarbonEvent &theEvent);
	OSStatus							DoEventControlTrackingAreaEntered(  NCarbonEvent &theEvent);


	// Handle mouse tracking
	bool								DoTrackMouse(NCarbonEvent				&theEvent,
														const HIPoint			&theMouse,
														MouseTrackingResult		theTrack);


private:
	void								UpdateCursor(bool inDivider);
	void								UpdateSplitter(void);
	void								UpdateLayout(void);


private:
	SplitterKind						mKind;
	bool								mVertical;
	float								mRatio;
	float								mRatioLimit;
	UInt32								mDividerSize;

	NHIView								*mViewA;
	NHIView								*mViewB;
	
	HIRect								mRectA;
	HIRect								mRectB;
	HIRect								mRectD;
	
	HIPoint								mDragLast;
	float								mDragSize;
	float								mDragOffsetMin;
	float								mDragOffsetMax;
	bool								mDragCursor;
	
	NCGShading							mShading;
};





#endif // NSPLITTERVIEW_HDR


