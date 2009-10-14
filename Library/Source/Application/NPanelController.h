/*	NAME:
		NPanelController.h

	DESCRIPTION:
		Panel controller.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPANELCONTROLLER_HDR
#define NPANELCONTROLLER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIImageView.h"
#include "NTimerTask.h"
#include "NHIView.h"
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NPanelController : public NTimerTask {
public:
										NPanelController(void);
	virtual								~NPanelController(void);


	// Get/set the nib file
	IBNibRef							GetPanelNib(void) const;
	void								SetPanelNib(IBNibRef theNib);


	// Get/set the container view
	NHIView								*GetPanelContainer(void) const;
	void								SetPanelContainer(NHIView *theView);


	// Get/set the panel
	//
	// The container view, and panel nib, must be assigned before a panel can be set.
	NString								GetPanel(void) const;
	void								SetPanel(const NString &thePanel);


	// Get/set the transition time
	EventTime							GetTransitionTime(void) const;
	void								SetTransitionTime(EventTime theTime);


protected:
	// Handle a timer
	virtual void						DoTimer(NTimerID theTimer, TimerMsg theMsg);
	

	// Handle panel changes
	virtual void						DoPanelChanging(void);
	virtual void						DoPanelChanged( void);


private:
	NHIImageView						*RenderPanel(void);
	

private:
	IBNibRef							mNib;
	NHIView								*mContainer;

	NString								mPanel;
	EventTime							mTransitionTime;

	float								mFadeAlpha;
	float								mFadeDelta;
	NHIImageView						*mFadeOld;
	NHIImageView						*mFadeNew;
};





#endif // NPanelCONTROLLER_HDR


