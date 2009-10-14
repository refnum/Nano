/*	NAME:
		NSlider.cpp

	DESCRIPTION:
		Slider control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIViewUtilities.h"
#include "NSlider.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NSlider, kHISliderClassID, NULL);





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
ControlActionUPP NSlider::mControlAction = NewControlActionUPP(NSlider::ControlAction);





//============================================================================
//		NSlider::NSlider : Constructor.
//----------------------------------------------------------------------------
NSlider::NSlider(HIWindowRef					theWindow,
					const HIRect				&theRect,
					SInt32						valueInitial,
					SInt32						valueMinimum,
					SInt32						valueMaximum,
					ControlSliderOrientation	theOrientation,
					UInt16						numTicks,
					bool						isLive)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateSliderControl(NULL, &kQDRectZero, valueInitial, valueMinimum, valueMaximum,
								theOrientation, numTicks, isLive, mControlAction, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NSlider::~NSlider : Destructor.
//----------------------------------------------------------------------------
NSlider::~NSlider(void)
{
}





//============================================================================
//		NSlider::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NSlider::InitializeView(void)
{	const EventTypeSpec	theEvents[] = { { kEventClassControl, kEventControlTrack },
										{ kEventClassNone,    kEventKindNone     } };



	// Register for events
	AddEvents(theEvents);

	if (GetControlAction(*this) != mControlAction)
		SetControlAction(*this, mControlAction);
}





//============================================================================
//		NSlider::DoEventControlTrack : Handle kEventControlTrack.
//----------------------------------------------------------------------------
OSStatus NSlider::DoEventControlTrack(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Track the control
	//
	// We override this event so that we can broadcast a message before/after
	// the track, and call the next handler to manage the track as normal.
	BroadcastMessage(kMsgSliderTrackBegin, this);

	theErr = CallNextHandler(theEvent);

	BroadcastMessage(kMsgSliderTrackEnd, this);
	
	return(theErr);
}





//============================================================================
//		NSlider::ControlAction : Control action callback.
//----------------------------------------------------------------------------
void NSlider::ControlAction(ControlRef theControl, ControlPartCode /*partCode*/)
{	NSlider		*thisPtr;



	// Validate our parameters
	//
	// Work around for rdar://4057871, where installing an action callback
	// on a non-live slider can result in the callback being invoked with
	// an invalid ControlRef.
	//
	// To detect this case, we check for our action callback.
	if (GetControlAction(theControl) != mControlAction)
		return;



	// Dispatch the message
	thisPtr = dynamic_cast<NSlider*>(NHIViewUtilities::GetView(theControl));
	thisPtr->BroadcastMessage(kMsgSliderChanged, thisPtr);
}


