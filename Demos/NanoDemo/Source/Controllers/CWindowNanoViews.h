/*	NAME:
		CWindowNanoViews.h

	DESCRIPTION:
		NanoDemo custom views window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CWINDOWNANOVIEWS_HDR
#define CWINDOWNANOVIEWS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPDFView.h"
#include "NRatingBar.h"
#include "NOpenGLView.h"
#include "NShadingView.h"
#include "NProgressPie.h"
#include "NReflectionView.h"
#include "NLevelIndicator.h"
#include "NFlatButton.h"
#include "NGlassButton.h"
#include "NImageButton.h"
#include "NMembraneButton.h"
#include "NDialView.h"
#include "NBezelView.h"
#include "NWindowController.h"
#include "NTimerTask.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CWindowNanoViews : public NWindowController, public NTimerTask {
public:
										DECLARE_WINDOW_CONTROLLER(CWindowNanoViews);

										CWindowNanoViews(void);
	virtual								~CWindowNanoViews(void);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


	// Handle a timer
	void								DoTimer(NTimerID theTimer, TimerMsg theMsg);


private:
	void								UpdateViews(void);


private:
	NIB_VIEW('npdf', NPDFView,			PDF);
	NIB_VIEW('nogl', NOpenGLView,		OpenGL);
	NIB_VIEW('shad', NShadingView,		Shading);
	NIB_VIEW('refl', NReflectionView,	Reflection);
	NIB_VIEW('npie', NProgressPie,		ProgressPie);
	NIB_VIEW('lcon', NLevelIndicator,	LevelContinuous);
	NIB_VIEW('ldis', NLevelIndicator,	LevelDiscrete);
	NIB_VIEW('lrat', NRatingBar,		Rating);
	NIB_VIEW('bflt', NFlatButton,		ButtonFlat);
	NIB_VIEW('bgla', NGlassButton,		ButtonGlass);
	NIB_VIEW('bmem', NMembraneButton,	ButtonMembrane);
	NIB_VIEW('bimg', NImageButton,		ButtonImage);
	NIB_VIEW('dial', NDialView,			Dial);
	NIB_VIEW('bez1', NBezelView,		Bezel1);
	NIB_VIEW('bez2', NBezelView,		Bezel2);
	NIB_VIEW('bez3', NBezelView,		Bezel3);

	UInt32								mProgress;
};





#endif // CWINDOWNANOVIEWS_HDR


