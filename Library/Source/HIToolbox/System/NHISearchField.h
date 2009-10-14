/*	NAME:
		NHISearchField.h

	DESCRIPTION:
		Search field view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHISEARCHFIELD_HDR
#define NHISEARCHFIELD_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgSearchFieldAccepted						= 'sfok';
static const BroadcastMsg kMsgSearchFieldChanged						= 'sfch';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHISearchField : public NHIView, public NMsgBroadcaster {
public:
										DECLARE_HIVIEW_SUBCLASS(NHISearchField);

										NHISearchField(HIWindowRef			theWindow,
														const HIRect		&theRect,
														const NString		&theDescription = "",
														OptionBits			theAttributes   = kHISearchFieldAttributesSearchIcon,
														MenuRef				theMenu         = NULL);
	virtual								~NHISearchField(void);


	// Get/set the description
	NString								GetDescription(void) const;
	void								SetDescription(const NString &theValue);


	// Get/set the attributes
	OptionBits							GetAttributes(void) const;
	void								SetAttributes(OptionBits setThese, OptionBits clearThese=kHISearchFieldNoAttributes);


	// Get/set the menu
	MenuRef								GetMenu(void) const;
	void								SetMenu(MenuRef theMenu);


protected:
	// Initialize the view
	void								InitializeView(void);
	

	// Handle events
	OSStatus							DoEventTextAccepted( NCarbonEvent &theEvent);
	OSStatus							DoEventTextDidChange(NCarbonEvent &theEvent);


private:


};





#endif // NHISEARCHFIELD_HDR


