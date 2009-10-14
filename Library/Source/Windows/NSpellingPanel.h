/*	NAME:
		NSpellingPanel.h

	DESCRIPTION:
		Spelling panel.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSPELLINGPANEL_HDR
#define NSPELLINGPANEL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppKitUtilities.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
DECLARE_OBJC_CLASS(NSPanel);
DECLARE_OBJC_CLASS(NSSpellChecker);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSpellingPanel {
public:
	// Get/set the visibility
	static bool							IsVisible(void);
	static void							SetVisible(bool isVisible=true);


	// Set the current mistake
	//
	// Sets the mis-spelled word to show in the panel, beeping for an empty string.
	static void							SetMistake(const NString &theWord, bool showPanel);


private:
	static void							InitializePanel(void);


private:
	static NSSpellChecker				*mSpellChecker;
	static NSPanel						*mPanel;
};








#endif // NSPELLINGPANEL_HDR


