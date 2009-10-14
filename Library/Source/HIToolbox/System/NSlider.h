/*	NAME:
		NSlider.h

	DESCRIPTION:
		Slider control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSLIDER_HDR
#define NSLIDER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgSliderTrackBegin							= 'sbeg';
static const BroadcastMsg kMsgSliderTrackEnd							= 'send';
static const BroadcastMsg kMsgSliderChanged								= 'scha';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSlider : public NHIView, public NMsgBroadcaster {
public:
										DECLARE_HIVIEW_SUBCLASS(NSlider);

										NSlider(HIWindowRef					theWindow,
												const HIRect				&theRect,
												SInt32						valueInitial   = 0,
												SInt32						valueMinimum   = 0,
												SInt32						valueMaximum   = 100,
												ControlSliderOrientation	theOrientation = kControlSliderPointsDownOrRight,
												UInt16						numTicks       = 10,
												bool						isLive         = true);

	virtual								~NSlider(void);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlTrack(NCarbonEvent &theEvent);


private:
	static void							ControlAction(ControlRef theControl, ControlPartCode partCode);


private:
	static ControlActionUPP				mControlAction;
};





#endif // NSLIDER_HDR


