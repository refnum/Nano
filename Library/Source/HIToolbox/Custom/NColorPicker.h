/*	NAME:
		NColorPicker.h

	DESCRIPTION:
		Color picker view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOLORPICKER_HDR
#define NCOLORPICKER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NColorPanel.h"
#include "NMsgListener.h"
#include "NColor.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NColorPicker : public NHIView,
					 public NMsgBroadcaster,
					 public NMsgListener {
public:
										DECLARE_HIVIEW_SUBCLASS(NColorPicker);

										NColorPicker(HIWindowRef	theWindow,
													const HIRect	&theRect,
													const NColor	&theColor = kColorBlack);

	virtual								~NColorPicker(void);


	// Get/set the alpha visibility
	bool								IsAlphaVisible(void) const;
	void								SetAlphaVisible(bool isVisible=true);


	// Get/set the color
	NColor								GetColor(void) const;
	void								SetColor(const NColor &theColor);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle events
	OSStatus							DoEventCommandProcess(          NCarbonEvent &theEvent);
	OSStatus							DoEventControlDraw(             NCarbonEvent &theEvent);
	OSStatus							DoEventControlVisibilityChanged(NCarbonEvent &theEvent);


	// Handle drags
	DragActions							DoDragReceive(const NDrag &theDrag);


private:
	bool								IsActivePicker(void);
	OSStatus							SetActivePicker(NColorPicker *thePicker);

	void								DoPanelChange(BroadcastMsg theMsg);


private:
	static NColorPicker					*mActivePicker;
	
	bool								mAlphaVisible;
	NColor								mColor;
};





#endif // NColorPicker_HDR


