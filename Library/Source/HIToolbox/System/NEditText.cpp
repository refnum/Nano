/*	NAME:
		NEditText.cpp

	DESCRIPTION:
		Edit text control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEditText.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NEditText, kHITextFieldClassID, NULL);





//============================================================================
//		NEditText::NEditText : Constructor.
//----------------------------------------------------------------------------
NEditText::NEditText(HIWindowRef					theWindow,
						const HIRect				&theRect,
						const NString				&theValue,
						bool						isPassword,
						const ControlFontStyleRec	*theStyle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateEditUnicodeTextControl(NULL, &kQDRectZero, theValue, isPassword, theStyle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NEditText::~NEditText : Destructor.
//----------------------------------------------------------------------------
NEditText::~NEditText(void)
{
}





//============================================================================
//		NEditText::IsEmpty : Is the edit field empty?
//----------------------------------------------------------------------------
bool NEditText::IsEmpty(void) const
{


	// Check our contents
	return(GetTextValue().IsEmpty());
}





//============================================================================
//		NEditText::IsLocked : Is the edit field locked?
//----------------------------------------------------------------------------
bool NEditText::IsLocked(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlEditTextLockedTag));
}





//============================================================================
//		NEditText::SetLocked : Set the locked state.
//----------------------------------------------------------------------------
void NEditText::SetLocked(bool isLocked)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlEditTextLockedTag, isLocked);
}





//============================================================================
//		NEditText::IsMultiLine : Is this a mult-line text field?
//----------------------------------------------------------------------------
bool NEditText::IsMultiLine(void) const
{


	// Get the flag
	//
	// Note that the tag meaning is inverted from the static text control,
	// and so we negate it to expose the same interface as NStaticText.
	return(!GetControlBoolean(kControlEntireControl, kControlEditTextSingleLineTag));
}





//============================================================================
//		NEditText::SetMultiLine : Set the multi-line state.
//----------------------------------------------------------------------------
void NEditText::SetMultiLine(bool multiLine)
{


	// Set the flag
	//
	// Note that the tag meaning is inverted from the static text control,
	// and so we negate it to expose the same interface as NStaticText.
	SetControlBoolean(kControlEntireControl, kControlEditTextSingleLineTag, !multiLine);
}





//============================================================================
//		NEditText::GetPasswordValue : Get the password string.
//----------------------------------------------------------------------------
NString NEditText::GetPasswordValue(void) const
{


	// Get the password text
	return(GetControlString(kControlEntireControl, kControlEditTextPasswordCFStringTag));
}





//============================================================================
//		NEditText::GetSelection : Get the selection.
//----------------------------------------------------------------------------
ControlEditTextSelectionRec NEditText::GetSelection(void) const
{	ControlEditTextSelectionRec		theSelection;



	// Get the selection
	GetControlData(kControlEntireControl, kControlEditTextSelectionTag, sizeof(theSelection), &theSelection);
	
	return(theSelection);
}





//============================================================================
//		NEditText::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NEditText::SetSelection(const ControlEditTextSelectionRec &theSelection)
{


	// Set the selection
	SetControlData(kControlEntireControl, kControlEditTextSelectionTag, sizeof(theSelection), &theSelection);
}





//============================================================================
//		NEditText::SetSelection : Set the selection.
//----------------------------------------------------------------------------
void NEditText::SetSelection(SInt16 selStart, SInt16 selEnd)
{	ControlEditTextSelectionRec		theSelection = { selStart, selEnd };



	// Set the selection
	SetSelection(theSelection);
}
