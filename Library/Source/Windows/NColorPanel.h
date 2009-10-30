/*	NAME:
		NColorPanel.h

	DESCRIPTION:
		Color panel.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOLORPANEL_HDR
#define NCOLORPANEL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppKitUtilities.h"
#include "NMsgBroadcaster.h"
#include "NColor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgColorPanelTrackBegin					= 'cptb';
static const BroadcastMsg kMsgColorPanelTrackEnd					= 'cpte';
static const BroadcastMsg kMsgColorPanelChanged						= 'cpch';
static const BroadcastMsg kMsgColorPanelClosed						= 'cpcl';





//============================================================================
//		Types
//----------------------------------------------------------------------------
DECLARE_OBJC_CLASS(NSColorPanel);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NColorPanel {
public:
	// Get/set the panel visibility
	static bool							IsVisible(void);
	static void							SetVisible(bool isVisible=true);


	// Get/set the alpha visibility
	static bool							IsAlphaVisible(void);
	static void							SetAlphaVisible(bool isVisible=true);


	// Get/set the color
	static NColor						GetColor(void);
	static void							SetColor(const NColor &theColor);


	// Get the broadcaster
	//
	// The panel broadcaster can be listened to for kMsgColorPanelXXX messages.
	static NMsgBroadcaster				*GetBroadcaster(void);


private:
	static void							InitializePanel(void);


private:
	static NSColorPanel					*mPanel;
	static NMsgBroadcaster				mBroadcaster;
};








#endif // NCOLORPANEL_HDR


