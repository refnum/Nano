/*	NAME:
		NWindowSplashScreen.cpp

	DESCRIPTION:
		Splash screen window.
		
		A splash screen window creates a transparent window that contains one
		or more of:

			o Background image
			o Status text
			o Progress bar

		The splash screen can be faded in/out when shown, and can be updated
		without having to run the event loop.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowSplashScreen.h"





//============================================================================
//		NWindowSplashScreen::NWindowSplashScreen : Constructor.
//----------------------------------------------------------------------------
NWindowSplashScreen::NWindowSplashScreen(void)
{
}





//============================================================================
//		NWindowSplashScreen::~NWindowSplashScreen : Destructor.
//----------------------------------------------------------------------------
NWindowSplashScreen::~NWindowSplashScreen(void)
{
}





//============================================================================
//		NWindowSplashScreen::Show : Show the window.
//----------------------------------------------------------------------------
void NWindowSplashScreen::Show(EventTime fadeTime)
{	NHIWindow		*theWindow;



	// Show the window
	theWindow = GetWindow();
	
	theWindow->SetFrame(theWindow->GetFrame(), kWindowContentRgn,
						 kWindowFadeTransitionEffect,
						 kWindowShowTransitionAction, fadeTime, false);
}





//============================================================================
//		NWindowSplashScreen::Hide : Hide the window.
//----------------------------------------------------------------------------
void NWindowSplashScreen::Hide(EventTime fadeTime)
{	NHIWindow		*theWindow;



	// Hide the window
	theWindow = GetWindow();

	theWindow->SetFrame(theWindow->GetFrame(), kWindowContentRgn,
						 kWindowFadeTransitionEffect,
						 kWindowHideTransitionAction, fadeTime, false);
}





//============================================================================
//		NWindowSplashScreen::GetImage : Get the image.
//----------------------------------------------------------------------------
NCGImage NWindowSplashScreen::GetImage(void) const
{


	// Get the image
	if (mImage.IsValid())
		return(mImage->GetImage());

	return(NCGImage());
}





//============================================================================
//		NWindowSplashScreen::SetImage : Set the image.
//----------------------------------------------------------------------------
void NWindowSplashScreen::SetImage(const NCGImage &theImage, bool flushWindow)
{


	// Set the image
	if (mImage.IsValid())
		{
		mImage->SetImage(theImage);
	
		if (flushWindow)
			FlushWindow();
		}
}





//============================================================================
//		NWindowSplashScreen::GetStatus : Get the status text.
//----------------------------------------------------------------------------
NString NWindowSplashScreen::GetStatus(void) const
{


	// Get the status
	if (mStatus.IsValid())
		return(mStatus->GetTextValue());
	
	return("");
}





//============================================================================
//		NWindowSplashScreen::SetStatus : Set the status text.
//----------------------------------------------------------------------------
void NWindowSplashScreen::SetStatus(const NString &theValue, bool flushWindow)
{


	// Set the status
	if (mStatus.IsValid())
		{
		mStatus->SetTextValue(theValue);
	
		if (flushWindow)
			FlushWindow();
		}
}





//============================================================================
//		NWindowSplashScreen::GetProgress : Get the progress value.
//----------------------------------------------------------------------------
float NWindowSplashScreen::GetProgress(void) const
{


	// Get the progress
	if (mProgress.IsValid())
		return(((float) mProgress->GetCurrentValue()) / 100.0f);
	
	return(0.0f);
}





//============================================================================
//		NWindowSplashScreen::SetProgress : Set the progress value.
//----------------------------------------------------------------------------
void NWindowSplashScreen::SetProgress(float theValue, bool flushWindow)
{


	// Set the progress
	if (mProgress.IsValid())
		{
		mProgress->SetCurrentValue((SInt32) (theValue * 100));
	
		if (flushWindow)
			FlushWindow();
		}
}





//============================================================================
//		NWindowSplashScreen::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NWindowSplashScreen::InitializeSelf(void)
{	NHIWindow	*theWindow;



	// Initialize our views
	if (mImage.IsValid())
		mImage->SetFrameStyle(kFrameNone);
	
	if (mProgress.IsValid())
		{
		mProgress->SetMinimumValue(0);
		mProgress->SetMaximumValue(100);
		mProgress->SetCurrentValue(0);
		}



	// Prepare the window
	theWindow = GetWindow();
	
	theWindow->SetContentTransparent(true);
	theWindow->SetAttributes(kWindowNoTitleBarAttribute);

	return(noErr);
}





//============================================================================
//		NWindowSplashScreen::FlushWindow : Flush the window.
//----------------------------------------------------------------------------
#pragma mark -
void NWindowSplashScreen::FlushWindow(void)
{	NHIWindow	*theWindow;
	OSStatus	theErr;



	// Get the state we need
	theWindow = GetWindow();



	// Flush the window
	//
	// Since the splash screen may be updated before the event loop is entered,
	// changes to content are typically flushed directly to the screen.
	theErr  = HIViewRender(theWindow->GetHIView(kHIViewWindowContentID));
	theErr |= HIWindowFlush(*theWindow);
	NN_ASSERT_NOERR(theErr);
}



