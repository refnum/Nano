/*	NAME:
		NWindowSplashScreen.h

	DESCRIPTION:
		Splash screen window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWINDOWSPLASHSCREEN_HDR
#define NWINDOWSPLASHSCREEN_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIImageView.h"
#include "NProgressBar.h"
#include "NStaticText.h"
#include "NWindowController.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWindowSplashScreen : public NWindowController {
public:
										NWindowSplashScreen(void);
	virtual								~NWindowSplashScreen(void);


	// Show/hide the window
	void								Show(EventTime fadeTime=0.20f);
	void								Hide(EventTime fadeTime=0.15f);


	// Get/set the image
	//
	// Has no effect if the window does not contain an image.
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theImage, bool flushWindow=true);


	// Get/set the status
	//
	// Has no effect if the window does not contain status text.
	NString								GetStatus(void) const;
	void								SetStatus(const NString &theValue, bool flushWindow=true);


	// Get/set the progress
	//
	// Has no effect if the window does not contain a progress bar.
	float								GetProgress(void) const;
	void								SetProgress(float theValue, bool flushWindow=true);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


private:
	void								FlushWindow(void);
	

private:
	NIB_VIEW('imag', NHIImageView,		Image);
	NIB_VIEW('stat', NStaticText,		Status);
	NIB_VIEW('prog', NProgressBar,		Progress);
};





#endif // NWINDOWSPLASHSCREEN_HDR


