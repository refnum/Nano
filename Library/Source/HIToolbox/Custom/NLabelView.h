/*	NAME:
		NLabelView.h

	DESCRIPTION:
		Finder-style "label color" view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved.
	__________________________________________________________________________
*/
#ifndef NLABELVIEW_HDR
#define NLABELVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NCGContext.h"
#include "NMenuItem.h"
#include "NTimerTask.h"
#include "NMenuItem.h"
#include "NMenuItemView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgLabelSelectionChanged					= 'lsel';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLabelView :	public NMenuItemView,
					public NMsgBroadcaster,
					public NTimerTask {
public:
										DECLARE_HIVIEW_SUBCLASS(NLabelView);

										NLabelView(const NMenuItem &menuItem,
													ThemeMenuType	menuType = kThemeMenuTypePullDown);

	virtual								~NLabelView(void);


	// Get/set the selection
	OptionBits							GetSelection(void) const;
	void								SetSelection(OptionBits theSelection);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle changes to the menu item
	void								DoMenuItemChanged( void);
	void								DoMenuItemPopulate(void);
	HIViewPartCode						DoMenuItemHitTest(const HIPoint &thePoint);


	// Handle a timer
	void								DoTimer(NTimerID theTimer, TimerMsg theMsg);


	// Handle events
	OSStatus							DoEventCommandProcess(          NCarbonEvent &theEvent);
	OSStatus							DoEventControlDraw(             NCarbonEvent &theEvent);
	OSStatus							DoEventControlGetOptimalBounds( NCarbonEvent &theEvent);
	OSStatus							DoEventControlGetPartRegion(    NCarbonEvent &theEvent);
	OSStatus							DoEventControlHit(              NCarbonEvent &theEvent);
	OSStatus							DoEventControlSimulateHit(      NCarbonEvent &theEvent);
	OSStatus							DoEventControlTrack(            NCarbonEvent &theEvent);
	OSStatus							DoEventControlVisibilityChanged(NCarbonEvent &theEvent);
	OSStatus							DoEventMouseDragged(            NCarbonEvent &theEvent);
	OSStatus							DoEventMouseMoved(              NCarbonEvent &theEvent);


private:
	HIRect								GetTitleBounds(void);
	float								GetTitleHeight(float *theBaseline=NULL);
	float								GetTitleLeft(void);

	HIRect								GetPartBounds(HIViewPartCode thePart);
	NString								GetPartName(  HIViewPartCode thePart);

	HIRect								GetLabelBounds(void);

	void								ResetMetrics(void);
	void								ResetRollOver(void);

	void								UpdateRollOver(void);
	OSStatus							UpdateMouse(NCarbonEvent &theEvent);

	void								DrawPartSelection(NCGContext &cgContext, const HIPoint &thePoint, bool isHilighted);
	void								DrawPartGlyph(    NCGContext &cgContext, const HIPoint &thePoint, HIViewPartCode thePart);


private:
	OptionBits							mSelection;
	
	float								mTitleBaseline;
	float								mTitleHeight;
	float								mTitleLeft;

	HIViewPartCode						mRollOver;
	HIViewPartCode						mRollOverNew;
	HIViewPartCode						mRollOverText;
	HIViewPartCode						mRollOverPart;

	NStringList							mPartNames;
};





#endif // NLABELVIEW_HDR


