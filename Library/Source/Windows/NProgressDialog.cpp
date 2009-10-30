/*  NAME:
        NProgressDialog.cpp

    DESCRIPTION:
        Progress dialog.

    COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
    ___________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "Nano Build.h"
#include "NProgressDialog.h"





//============================================================================
//      Controller
//----------------------------------------------------------------------------
DEFINE_WINDOW_CONTROLLER(NProgressDialog, kNanoNibFile, "Progress");





//============================================================================
//		NProgressDialog::NProgressDialog : Constructor.
//----------------------------------------------------------------------------
NProgressDialog::NProgressDialog(void)
{
}





//============================================================================
//		NProgressDialog::~NProgressDialog : Destructor.
//----------------------------------------------------------------------------
NProgressDialog::~NProgressDialog(void)
{
}





//============================================================================
//		NProgressDialog::GetCancelAction : Get the cancel action.
//----------------------------------------------------------------------------
NFunctor NProgressDialog::GetCancelAction(void) const
{


	// Get the action
	return(mCancelAction);
}





//============================================================================
//		NProgressDialog::SetCancelAction : Set the cancel action.
//----------------------------------------------------------------------------
void NProgressDialog::SetCancelAction(const NFunctor &theAction)
{


	// Set the action
	mCancelAction = theAction;
}





//============================================================================
//		NProgressDialog::IsCancelVisible : Get the cancel button visibility.
//----------------------------------------------------------------------------
bool NProgressDialog::IsCancelVisible(void) const
{


	// Get the state
	return(mCancel->IsVisible());
}





//============================================================================
//		NProgressDialog::SetCancelVisible : Set the cancel button visibility.
//----------------------------------------------------------------------------
void NProgressDialog::SetCancelVisible(bool isVisible)
{


	// Set the state
	mCancel->SetVisible(isVisible);
	
	if (isVisible)
		mProgress->SetFrame(mProgressCompact);
	else
		mProgress->SetFrame(mProgressExpanded);
}





//============================================================================
//		NProgressDialog::IsCancelEnabled : Get the cancel button availability.
//----------------------------------------------------------------------------
bool NProgressDialog::IsCancelEnabled(void) const
{


	// Get the state
	return(mCancel->IsEnabled());
}





//============================================================================
//		NProgressDialog::SetCancelEnabled : Set the cancel button availability.
//----------------------------------------------------------------------------
void NProgressDialog::SetCancelEnabled(bool isEnabled)
{


	// Set the state
	mCancel->SetEnabled(isEnabled);
}





//============================================================================
//		NProgressDialog::GetCaption : Get the caption.
//----------------------------------------------------------------------------
NString NProgressDialog::GetCaption(void) const
{


	// Get the caption
	return(mCaption->GetTextValue());
}





//============================================================================
//		NProgressDialog::SetCaption : Set the caption.
//----------------------------------------------------------------------------
void NProgressDialog::SetCaption(const NString &theValue)
{


	// Set the caption
	mCaption->SetTextValue(theValue);
}





//============================================================================
//		NProgressDialog::GetProgress : Get the progress.
//----------------------------------------------------------------------------
float NProgressDialog::GetProgress(void) const
{


	// Get the progress
	return(mProgress->GetPercentValue());
}





//============================================================================
//		NProgressDialog::SetProgress : Set the progress.
//----------------------------------------------------------------------------
void NProgressDialog::SetProgress(float theValue)
{


	// Set the progress
	mProgress->SetIndeterminate(theValue < 0.0f);
	
	if (theValue >= 0.0f)
		mProgress->SetPercentValue(theValue);
}





//============================================================================
//		NProgressDialog::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NProgressDialog::InitializeSelf(void)
{


	// Initialize ourselves
	mCancelVisible = true;
	mCancelEnabled = true;

	mProgressCompact  =             mProgress->GetFrame();
	mProgressExpanded = CGRectUnion(mProgress->GetFrame(), mCancel->GetFrame());

	return(noErr);
}





//============================================================================
//		NProgressDialog::CommandSheet : Handle a sheet command.
//----------------------------------------------------------------------------
OSStatus NProgressDialog::CommandSheet(UInt32 theCmd)
{


	// Handle the command
	switch (theCmd) {
		case kHICommandCancel:
			if (mCancelAction != NULL)
				mCancelAction();
			break;

		default:
			break;
		}
	
	return(NWindowController::CommandSheet(theCmd));
}
