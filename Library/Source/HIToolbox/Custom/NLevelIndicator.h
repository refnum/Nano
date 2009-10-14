/*	NAME:
		NLevelIndicator.h

	DESCRIPTION:
		Level indicator view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NLEVELINDICATOR_HDR
#define NLEVELINDICATOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGShading.h"
#include "NCGContext.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLevelIndicator : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NLevelIndicator);

										NLevelIndicator(HIWindowRef		theWindow,
														const HIRect	&theRect,
														bool			isContinuous  = false,
														SInt32			valueInitial  = 0,
														SInt32			valueMinimum  = 0,
														SInt32			valueMaximum  = 100,
														SInt32			valueWarning  = 75,
														SInt32			valueCritical = 90);

	virtual								~NLevelIndicator(void);


	// Is the level indicator continuous?
	bool								IsContinuous(void) const;
	void								SetContinuous(bool isContinuous);


	// Get/set the warning value
	SInt32								GetWarningValue(void) const;
	void								SetWarningValue(SInt32 theValue);


	// Get/set the critical value
	SInt32								GetCriticalValue(void) const;
	void								SetCriticalValue(SInt32 theValue);


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
	void								UpdateShading(void);
	
	void								DrawContinuous(NCGContext &cgContext);
	void								DrawDiscrete(  NCGContext &cgContext);


private:
	bool								mIsContinuous;
	SInt32								mValueWarning;
	SInt32								mValueCritical;
	
	NCGShading							mShadingNormal;
	NCGShading							mShadingWarning;
	NCGShading							mShadingCritical;
	NCGShading							mShadingBackground;
};





#endif // NLEVELINDICATOR_HDR


