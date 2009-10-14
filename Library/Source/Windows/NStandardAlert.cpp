/*	NAME:
		NStandardAlert.cpp

	DESCRIPTION:
		Standard alert dialog.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStandardAlert.h"





//============================================================================
//		NStandardAlert::NStandardAlert : Constructor.
//----------------------------------------------------------------------------
NStandardAlert::NStandardAlert(void)
{	OSStatus	theErr;



	// Initialize ourselves
	mParent = NULL;
	mDialog = NULL;
	
	mType  = kAlertNoteAlert;
	theErr = GetStandardAlertDefaultParams(&mState, kStdCFStringAlertVersionOne);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NStandardAlert::~NStandardAlert : Destructor.
//----------------------------------------------------------------------------
NStandardAlert::~NStandardAlert(void)
{


	// Validate our state
	NN_ASSERT(!IsOpen());
}





//============================================================================
//		NStandardAlert::IsOpen : Is the dialog open?
//----------------------------------------------------------------------------
bool NStandardAlert::IsOpen(void) const
{


	// Check our state
	return(mDialog != NULL);
}





//============================================================================
//		NStandardAlert::Open : Open the dialog.
//----------------------------------------------------------------------------
DialogItemIndex NStandardAlert::Open(void)
{	EventTargetRef		theTarget;
	DialogItemIndex		theItem;
	OSStatus			theErr;



	// Initialize ourselves
	theItem = kAlertStdAlertCancelButton;

	SetThemeCursor(kThemeArrowCursor);



	// Prepare the dialog
	mState.defaultText = GetButtonString(mTextDefault);
	mState.cancelText  = GetButtonString(mTextCancel);
	mState.otherText   = GetButtonString(mTextOther);



	// Display a sheet
	if (mParent != NULL)
		{
		theTarget = mParent->GetEventTarget();
		NN_ASSERT(theTarget != NULL);
		
		theErr = CreateStandardSheet(mType, mTextMajor, mTextMinor, &mState, theTarget, &mDialog);
		if (theErr == noErr)
			theErr = ShowSheetWindow(GetDialogWindow(mDialog), mParent->GetHIWindow());
		}
	
	
	// Display a modal dialog
	else
		{
		theErr = CreateStandardAlert(mType, mTextMajor, mTextMinor, &mState, &mDialog);
		if (theErr == noErr)
			theErr = RunStandardAlert(mDialog, NULL, &theItem);
		}
	
	NN_ASSERT_NOERR(theErr);
	
	return(theItem);
}





//============================================================================
//		NStandardAlert::Close : Close the dialog.
//----------------------------------------------------------------------------
void NStandardAlert::Close(UInt32 theCmd)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsOpen());
	NN_ASSERT(GetParent() != NULL);



	// Close the dialog
	theErr = CloseStandardSheet(mDialog, theCmd);
	NN_ASSERT_NOERR(theErr);
	
	mDialog = NULL;
}





//============================================================================
//		NStandardAlert::GetParent : Get the parent.
//----------------------------------------------------------------------------
NHIWindow *NStandardAlert::GetParent(void) const
{


	// Get the parent
	return(mParent);
}





//============================================================================
//		NStandardAlert::SetParent : Set the parent.
//----------------------------------------------------------------------------
void NStandardAlert::SetParent(NHIWindow *theParent)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Set the parent
	mParent = theParent;
}





//============================================================================
//		NStandardAlert::GetType : Get the dialog type.
//----------------------------------------------------------------------------
AlertType NStandardAlert::GetType(void) const
{


	// Get the dialog type
	return(mType);
}





//============================================================================
//		NStandardAlert::SetType : Set the dialog type.
//----------------------------------------------------------------------------
void NStandardAlert::SetType(AlertType theType)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Set the dialog type
	mType = theType;
}





//============================================================================
//		NStandardAlert::HasHelp : Do we have a help button?
//----------------------------------------------------------------------------
bool NStandardAlert::HasHelp(void) const
{


	// Get the help state
	return(mState.helpButton);
}





//============================================================================
//		NStandardAlert::SetHasHelp : Set our help button state.
//----------------------------------------------------------------------------
void NStandardAlert::SetHasHelp(bool hasHelp)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Set the help state
	mState.helpButton = hasHelp;
}





//============================================================================
//		NStandardAlert::GetButtonDefault : Get the default button.
//----------------------------------------------------------------------------
DialogItemIndex NStandardAlert::GetButtonDefault(void) const
{


	// Get the button
	return(mState.defaultButton);
}





//============================================================================
//		NStandardAlert::GetButtonCancel : Get the cancel button.
//----------------------------------------------------------------------------
DialogItemIndex NStandardAlert::GetButtonCancel(void) const
{


	// Get the button
	return(mState.cancelButton);
}





//============================================================================
//		NStandardAlert::SetButtonDefault : Set the default button.
//----------------------------------------------------------------------------
void NStandardAlert::SetButtonDefault(DialogItemIndex theButton)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Set the button
	mState.defaultButton = theButton;
}





//============================================================================
//		NStandardAlert::SetButtonCancel : Set the cancel button.
//----------------------------------------------------------------------------
void NStandardAlert::SetButtonCancel(DialogItemIndex theButton)
{


	// Validate our state
	NN_ASSERT(!IsOpen());



	// Set the button
	mState.cancelButton = theButton;
}





//============================================================================
//		NStandardAlert::GetTextMajor : Get the major text.
//----------------------------------------------------------------------------
NString NStandardAlert::GetTextMajor(void) const
{


	// Get the text
	return(mTextMajor);
}





//============================================================================
//		NStandardAlert::GetTextMinor : Get the minor text.
//----------------------------------------------------------------------------
NString NStandardAlert::GetTextMinor(void) const
{


	// Get the text
	return(mTextMinor);
}





//============================================================================
//		NStandardAlert::GetTextDefault : Get the default button text.
//----------------------------------------------------------------------------
NString NStandardAlert::GetTextDefault(void) const
{


	// Get the text
	return(mTextDefault);
}





//============================================================================
//		NStandardAlert::GetTextCancel : Get the cancel button text.
//----------------------------------------------------------------------------
NString NStandardAlert::GetTextCancel(void) const
{


	// Get the text
	return(mTextCancel);
}





//============================================================================
//		NStandardAlert::GetTextOther : Get the other button text.
//----------------------------------------------------------------------------
NString NStandardAlert::GetTextOther(void) const
{


	// Get the text
	return(mTextOther);
}





//============================================================================
//		NStandardAlert::SetTextMajor : Set the major text.
//----------------------------------------------------------------------------
void NStandardAlert::SetTextMajor(const NString &theText)
{


	// Set the text
	mTextMajor = theText;
}





//============================================================================
//		NStandardAlert::SetTextMinor : Set the minor text.
//----------------------------------------------------------------------------
void NStandardAlert::SetTextMinor(const NString &theText)
{


	// Set the text
	mTextMinor = theText;
}





//============================================================================
//		NStandardAlert::SetTextDefault : Set the default text.
//----------------------------------------------------------------------------
void NStandardAlert::SetTextDefault(const NString &theText)
{


	// Set the text
	mTextDefault = theText;
}





//============================================================================
//		NStandardAlert::SetTextCancel : Set the cancel text.
//----------------------------------------------------------------------------
void NStandardAlert::SetTextCancel(const NString &theText)
{


	// Set the text
	mTextCancel = theText;
}





//============================================================================
//		NStandardAlert::SetTextOther : Set the other text.
//----------------------------------------------------------------------------
void NStandardAlert::SetTextOther(const NString &theText)
{


	// Set the text
	mTextOther = theText;
}





//============================================================================
//		NStandardAlert::DoAlert : Display a simple alert.
//----------------------------------------------------------------------------
DialogItemIndex NStandardAlert::DoAlert(const NString &textMajor,
										const NString &textMinor,
										const NString &textDefault,
										const NString &textCancel,
										const NString &textOther)
{	NStandardAlert		theAlert;



	// Display the alert
	theAlert.SetTextMajor(  textMajor);
	theAlert.SetTextMinor(  textMinor);
	theAlert.SetTextDefault(textDefault);
	theAlert.SetTextCancel( textCancel);
	theAlert.SetTextOther(  textOther);
	
	return(theAlert.Open());
}





//============================================================================
//		NStandardAlert::GetButtonString : Get the text for a button.
//----------------------------------------------------------------------------
#pragma mark -
CFStringRef NStandardAlert::GetButtonString(const NString &theText)
{	CFStringRef		cfString;



	// Empty strings imply no button
	if (theText.IsEmpty())
		cfString = NULL;
	
	
	// Default strings are supplied by the system
	else if (theText == kDefaultAlertText)
		cfString = (CFStringRef) -1;
	
	
	// Custom strings are used unchanged
	else
		cfString = theText;
	
	return(cfString);
}


