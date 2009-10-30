/*	NAME:
		NFontPanel.cpp

	DESCRIPTION:
		Font panel.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NFontPanel.h"





//============================================================================
//		NFontPanel::IsVisible : Is the panel visible?
//----------------------------------------------------------------------------
bool NFontPanel::IsVisible(void)
{


	// Check our state
	return(FPIsFontPanelVisible());
}





//============================================================================
//		NFontPanel::SetVisible : Set the panel visibility.
//----------------------------------------------------------------------------
void NFontPanel::SetVisible(bool isVisible)
{	SInt32		errorFD;
	OSStatus	theErr;



	// Work around radr://4244101
	//
	// FPShowHideFontPanel emits a spurious "Unknown class NSTypographyPanelSlider" message
	// when the font panel is first made visible (so we turn redirect stderr to hide this).
	errorFD = -1;
	if (isVisible)
		errorFD = NFileUtilities::RedirectStderr(-1);



	// Set our state
	if (IsVisible() != isVisible)
		{
		theErr = FPShowHideFontPanel();
		NN_ASSERT_NOERR(theErr);
		}



	// Restore stderr
	if (isVisible)
		NFileUtilities::RedirectStderr(errorFD);
}




