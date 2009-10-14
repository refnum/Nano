/*	NAME:
		NDialView.h

	DESCRIPTION:
		Dial view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NDIALVIEW_HDR
#define NDIALVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NCGContext.h"
#include "NHIView.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Dial value
typedef struct {
	float	theAngle;
	float	theDistance;
} NDialValue;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgDialChanged								= 'dcha';


// Values
static const NDialValue kDialValueDefault								= { 0.75f, 1.0f };





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDialView : public NHIView, public NMsgBroadcaster {
public:
										DECLARE_HIVIEW_SUBCLASS(NDialView);

										NDialView(HIWindowRef			theWindow,
													const HIRect		&theRect,
													const NDialValue	&theValue = kDialValueDefault);

	virtual								~NDialView(void);


	// Get/set distance tracking
	//
	// A dial value can track both distance and angle by default; if distance
	// tracking is disabled, the distance value will be forced to 1.0.
	bool								CanTrackDistance(void) const;
	void								SetTrackDistance(bool trackDistance);


	// Get/set the dial value
	//
	// Angle and distance are both scalar quantities, ranging from 0.0 to 1.0.
	//
	// An angle of 0.0 or 1.0 corresponds to 12 o'clock, with positive values
	// moving in a clockwise direction.
	//
	// Distance ranges from 0.0 in the center to 1.0 at the perimeter.
	NDialValue							GetDialValue(void) const;
	void								SetDialValue(const NDialValue &theValue);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


	// Handle mouse tracking
	bool								DoTrackMouse(NCarbonEvent				&theEvent,
														const HIPoint			&theMouse,
														MouseTrackingResult		theTrack);


private:
	void								DrawFace( NCGContext &cgContext);
	void								DrawThumb(NCGContext &cgContext);
	
	NDialValue							GetValueForPoint(const HIPoint    &thePoint);
	HIPoint								GetPointForValue(const NDialValue &theValue);
	
	float								GetMaxDistance(const HIRect &theRect);
	void								SyncValueDistance(void);
	

private:
	NDialValue							mValue;
	NDialValue							mTrackStart;
	bool								mTrackDistance;
	
	NCGShading							mShadingFace;
};





#endif // NDIALVIEW_HDR


