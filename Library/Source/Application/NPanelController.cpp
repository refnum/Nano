/*	NAME:
		NPanelController.cpp

	DESCRIPTION:
		Panel controller.
		
		A panel controller is a mix-in class that allows a "swappable view",
		where the contents of some view are selected from one or more views
		obtained from a nib file.
		
		Each view corresponds to a single panel, and by selecting different
		panels the contents of the container view can be swapped at runtime.
	
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
#include "NIBUtilities.h"
#include "NMathUtilities.h"
#include "NPanelController.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const float kTransitionSteps										= 5.0f;


// Timers
static const NTimerID kTimerTransition									= 0;





//============================================================================
//		NPanelController::NPanelController : Constructor.
//----------------------------------------------------------------------------
NPanelController::NPanelController(void)
{


	// Initialize ourselves
	mNib       = NULL;
	mContainer = NULL;
	
	mTransitionTime = 0.0f;

	mFadeAlpha = 0.0f;
	mFadeDelta = 0.0f;
	mFadeOld   = NULL;
	mFadeNew   = NULL;
}





//============================================================================
//		NPanelController::~NPanelController : Destructor.
//----------------------------------------------------------------------------
NPanelController::~NPanelController(void)
{
}





//============================================================================
//		NPanelController::GetPanelNib : Get the nib file.
//----------------------------------------------------------------------------
IBNibRef NPanelController::GetPanelNib(void) const
{


	// Get the nib file
	return(mNib);
}





//============================================================================
//		NPanelController::SetPanelNib : Set the nib file.
//----------------------------------------------------------------------------
void NPanelController::SetPanelNib(IBNibRef theNib)
{


	// Validate our parameters
	NN_ASSERT(theNib != NULL);
	


	// Set the nib file
	mNib = theNib;
}





//============================================================================
//		NPanelController::GetPanelContainer : Get the container view.
//----------------------------------------------------------------------------
NHIView *NPanelController::GetPanelContainer(void) const
{


	// Get the container view
	return(mContainer);
}





//============================================================================
//		NPanelController::SetPanelContainer : Set the container view.
//----------------------------------------------------------------------------
void NPanelController::SetPanelContainer(NHIView *theView)
{


	// Validate our parameters
	NN_ASSERT(theView != NULL);
	


	// Set the container view
	mContainer = theView;
}





//============================================================================
//		NPanelController::GetPanel : Get the panel.
//----------------------------------------------------------------------------
NString NPanelController::GetPanel(void) const
{


	// Get the panel
	return(mPanel);
}





//============================================================================
//		NPanelController::SetPanel : Set the panel.
//----------------------------------------------------------------------------
void NPanelController::SetPanel(const NString &thePanel)
{	float			timeDelta;
	NHIView			*theView;
	OSStatus		theErr;



	// Validate our parameters an state
	NN_ASSERT(thePanel.IsNotEmpty());

	NN_ASSERT(mNib       != NULL);
	NN_ASSERT(mContainer != NULL);

	NN_ASSERT(mFadeOld == NULL);
	NN_ASSERT(mFadeNew == NULL);



	// Check we need to change
	if (mPanel == thePanel)
		return;



	// Prepare to change
	DoPanelChanging();
	
	if (mContainer->IsVisible() && NMathUtilities::NotZero(mTransitionTime))
		mFadeOld = RenderPanel();



	// Change the panel
	theView = NHIViewUtilities::GetHIView(NIBUtilities::CreateView(mNib, thePanel));
	if (theView != NULL)
		{
		mContainer->RemoveSubViews();
		mContainer->AddSubView(theView);

		theView->SetFrame(mContainer->GetBounds());
		mPanel = thePanel;
		}

	DoPanelChanged();



	// Perform the transition
	if (mContainer->IsVisible() && NMathUtilities::NotZero(mTransitionTime))
		mFadeNew = RenderPanel();

	if (mFadeOld != NULL && mFadeNew != NULL)
		{
		mFadeAlpha = 0.0f;
		mFadeDelta = 1.0 / kTransitionSteps;
		timeDelta  = mTransitionTime / kTransitionSteps;
		
		mFadeOld->SetAlpha(1.0f);
		mFadeNew->SetAlpha(0.0f);
		mContainer->SetVisible(false);

		AddTimer(0.0f, timeDelta, kTimerTransition);
		while (HasTimer(kTimerTransition))
			{
			theErr = RunCurrentEventLoop(timeDelta / 2.0f);
			NN_ASSERT(theErr == noErr || theErr == eventLoopTimedOutErr);
			}
		}

	delete mFadeOld;
	delete mFadeNew;
	
	mFadeOld = NULL;
	mFadeNew = NULL;
}





//============================================================================
//		NPanelController::GetTransitionTime : Get the transition time.
//----------------------------------------------------------------------------
EventTime NPanelController::GetTransitionTime(void) const
{


	// Get the transition time
	return(mTransitionTime);
}





//============================================================================
//		NPanelController::SetTransitionTime : Set the transition time.
//----------------------------------------------------------------------------
void NPanelController::SetTransitionTime(EventTime theTime)
{


	// Validate our parameters
	NN_ASSERT(theTime >= 0.0);



	// Set the transition time
	mTransitionTime = theTime;
}





//============================================================================
//		NPanelController::DoTimer : Handle a timer.
//----------------------------------------------------------------------------
#pragma mark -
void NPanelController::DoTimer(NTimerID theTimer, TimerMsg theMsg)
{


	// Handle the timer
	//
	// We use a repeating timer to animate panel transitions.
	switch (theTimer) {
		case kTimerTransition:
			// Update the transition
			mFadeOld->SetAlpha(1.0f - mFadeAlpha);
			mFadeNew->SetAlpha(mFadeAlpha);
			
			mFadeAlpha += mFadeDelta;
			if (mFadeAlpha >= 1.0f)
				{
				mFadeOld->SetVisible(false);
				mFadeNew->SetVisible(false);

				mContainer->SetVisible(true);
				RemoveTimer(kTimerTransition);
				}
			break;
		
		default:
			NTimerTask::DoTimer(theTimer, theMsg);
			break;
		}
}





//============================================================================
//		NPanelController::DoPanelChanging : The panel is about to change.
//----------------------------------------------------------------------------
//		Note : Sub-classes should override to prepare for a panel change.
//----------------------------------------------------------------------------
void NPanelController::DoPanelChanging(void)
{
}





//============================================================================
//		NPanelController::DoPanelChanged : The panel has changed.
//----------------------------------------------------------------------------
//		Note : Sub-classes should override to handle a panel change.
//----------------------------------------------------------------------------
void NPanelController::DoPanelChanged(void)
{
}





//============================================================================
//		NPanelController::RenderPanel : Render a panel.
//----------------------------------------------------------------------------
NHIImageView *NPanelController::RenderPanel(void)
{	NHIImageView	*theView;
	NCGImage		theImage;



	// Create the image view
	theImage = mContainer->GetViewImage();
	theView  = new NHIImageView(mContainer->GetHIWindow(), mContainer->GetBounds(), &theImage);
	
	return(theView);
}



