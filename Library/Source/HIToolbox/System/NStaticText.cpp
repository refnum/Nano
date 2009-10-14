/*	NAME:
		NStaticText.cpp

	DESCRIPTION:
		Static text control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStaticText.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NStaticText, kHIStaticTextViewClassID, NULL);





//============================================================================
//		NStaticText::NStaticText : Constructor.
//----------------------------------------------------------------------------
NStaticText::NStaticText(HIWindowRef					theWindow,
							const HIRect				&theRect,
							const NString				&theValue,
							const ControlFontStyleRec	*theStyle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateStaticTextControl(NULL, &kQDRectZero, theValue, theStyle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NStaticText::~NStaticText : Destructor.
//----------------------------------------------------------------------------
NStaticText::~NStaticText(void)
{
}





//============================================================================
//		NStaticText::GetTextHeight : Get the text height.
//----------------------------------------------------------------------------
UInt16 NStaticText::GetTextHeight(void) const
{


	// Get the height
	return(GetControlUInt16(kControlEntireControl, kControlStaticTextTextHeightTag));
}





//============================================================================
//		NStaticText::GetTruncation : Get the truncation mode.
//----------------------------------------------------------------------------
TruncCode NStaticText::GetTruncation(void) const
{


	// Get the mode
	return(GetControlUInt16(kControlEntireControl, kControlStaticTextTruncTag));
}





//============================================================================
//		NStaticText::SetTruncation : Set the truncation mode.
//----------------------------------------------------------------------------
void NStaticText::SetTruncation(TruncCode truncMode)
{


	// Set the mode
	SetControlUInt16(kControlEntireControl, kControlStaticTextTruncTag, truncMode);
}





//============================================================================
//		NStaticText::IsMultiLine : Is this a mult-line text field?
//----------------------------------------------------------------------------
bool NStaticText::IsMultiLine(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlStaticTextIsMultilineTag));
}





//============================================================================
//		NStaticText::SetMultiLine : Set the multi-line state.
//----------------------------------------------------------------------------
void NStaticText::SetMultiLine(bool multiLine)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlStaticTextIsMultilineTag, multiLine);
}


