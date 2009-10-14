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
{	NSlider	*thisPtr = dynamic_cast<NSlider*>(NHIViewUtilities::GetHIView(theControl));



	// Validate our parameters
	NN_ASSERT(thisPtr != NULL);



	// Dispatch the message
	thisPtr->BroadcastMessage(kMsgSliderChanged, thisPtr);
}


