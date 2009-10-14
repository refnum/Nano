/*	NAME:
		NStaticText.h

	DESCRIPTION:
		Static text control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTATICTEXT_HDR
#define NSTATICTEXT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStaticText : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NStaticText);

										NStaticText(HIWindowRef						theWindow,
														const HIRect				&theRect,
														const NString				&theValue = "",
														const ControlFontStyleRec	*theStyle = NULL);
	virtual								~NStaticText(void);


	// Get the text height
	UInt16								GetTextHeight(void) const;


	// Get/set the truncation mode
	TruncCode							GetTruncation(void) const;
	void								SetTruncation(TruncCode truncMode);


	// Get/set the multi-line state
	bool								IsMultiLine(void) const;
	void								SetMultiLine(bool multiLine);


private:


};





#endif // NSTATICTEXT_HDR


