/*	NAME:
		NColorPicker.h

	DESCRIPTION:
		Color picker view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOLORPICKER_HDR
#define NCOLORPICKER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAutoReleasePool.h"
#include "NMsgBroadcaster.h"
#include "NColor.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgColorPickerTrackBegin					= 'cptb';
static const BroadcastMsg kMsgColorPickerTrackEnd					= 'cpte';
static const BroadcastMsg kMsgColorPickerChanged					= 'cpch';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NColorPicker : public NHIView, public NMsgBroadcaster {
public:
										DECLARE_HIVIEW_SUBCLASS(NColorPicker);

										NColorPicker(HIWindowRef	theWindow,
													const HIRect	&theRect,
													const NColor	&theColor = kColorBlack);

	virtual								~NColorPicker(void);


	// Get/set the color
	NColor								GetColor(void) const;
	void								SetColor(const NColor &theColor, bool sendMsg=false);


	// Get/set the alpha state
	bool								GetShowAlpha(void) const;
	void								SetShowAlpha(bool showAlpha);
	

protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventCommandProcess(          NCarbonEvent &theEvent);
	OSStatus							DoEventControlDraw(             NCarbonEvent &theEvent);
	OSStatus							DoEventControlVisibilityChanged(NCarbonEvent &theEvent);


	// Handle drags
	DragActions							DoDragReceive(DragRef theDrag);


private:
	OSStatus							SetActivePicker(NColorPicker *thePicker);


private:
	NAutoReleasePool					mPool;

	NColor								mColor;
	bool								mShowAlpha;
};





#endif // NColorPicker_HDR


