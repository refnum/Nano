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
#include "NCoreGraphicsUtilities.h"
#include "NContainer.h"
#include "NHIView.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStaticText : public NHIView,
					public NContainer{
public:
										DECLARE_HIVIEW_SUBCLASS(NStaticText);

										NStaticText(HIWindowRef						theWindow,
														const HIRect				&theRect,
														const NString				&theValue = "",
														const ControlFontStyleRec	*theStyle = NULL);
	virtual								~NStaticText(void);


	// Get the size
	UInt32								GetSize(void) const;


	// Get the text height
	float								GetTextHeight(void) const;


	// Get/set the truncation mode
	TruncCode							GetTruncation(void) const;
	void								SetTruncation(TruncCode truncMode);


	// Get/set the multi-line state
	bool								IsMultiLine(void) const;
	void								SetMultiLine(bool multiLine);


	// Get/set the shadow state
	CGShadowInfo						GetShadow(void) const;
	void								SetShadow(const CGShadowInfo &theShadow);


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle events
	OSStatus							DoEventControlDraw(NCarbonEvent &theEvent);


private:
	CGShadowInfo						mShadow;
};





#endif // NSTATICTEXT_HDR


