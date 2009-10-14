/*	NAME:
		NRoundButton.cpp

	DESCRIPTION:
		Round button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NIcon.h"
#include "NRoundButton.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NRoundButton, kHIRoundButtonClassID, NULL);





//============================================================================
//		NRoundButton::NRoundButton : Constructor.
//----------------------------------------------------------------------------
NRoundButton::NRoundButton(HIWindowRef theWindow, const HIRect &theRect, ControlRoundButtonSize theSize, bool isHelp)
{	NIcon						helpIcon(kSystemIconsCreator, kHelpIcon);
	ControlButtonContentInfo	theContent = kControlContentNone;
	HIViewRef					theView;
	OSStatus					theErr;



	// Prepare the content
	if (isHelp)
		{
		theContent.contentType = kControlContentIconRef;
		theContent.u.iconRef   = helpIcon;
		}



	// Create the view
	theErr = CreateRoundButtonControl(NULL, &kQDRectZero, theSize, &theContent, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NRoundButton::~NRoundButton : Destructor.
//----------------------------------------------------------------------------
NRoundButton::~NRoundButton(void)
{
}





//============================================================================
//		NRoundButton::GetIcon : Get the icon.
//----------------------------------------------------------------------------
NIcon NRoundButton::GetIcon(void) const
{


	// Get the icon
	return(GetContentIcon(kControlEntireControl, kControlRoundButtonContentTag));
}





//============================================================================
//		NRoundButton::SetIcon : Set the icon.
//----------------------------------------------------------------------------
void NRoundButton::SetIcon(const NIcon &theIcon)
{


	// Set the icon
	SetContentIcon(kControlEntireControl, kControlRoundButtonContentTag, theIcon);
}





//============================================================================
//		NRoundButton::GetSize : Get the button size.
//----------------------------------------------------------------------------
ControlRoundButtonSize NRoundButton::GetSize(void) const
{


	// Get the size
	return(GetControlUInt16(kControlEntireControl, kControlRoundButtonSizeTag));
}





//============================================================================
//		NRoundButton::SetSize : Set the button size.
//----------------------------------------------------------------------------
void NRoundButton::SetSize(ControlRoundButtonSize theSize)
{


	// Set the size
	SetControlUInt16(kControlEntireControl, kControlRoundButtonSizeTag, theSize);
}

