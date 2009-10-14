/*	NAME:
		NHIViewUtilities.h

	DESCRIPTION:
		HIView utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIVIEWUTILITIES_HDR
#define NHIVIEWUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIScrollView.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const OptionBits kBindNone								= (0 << 0);
static const OptionBits kBindTop								= (1 << 0);
static const OptionBits kBindLeft								= (1 << 1);
static const OptionBits kBindBottom								= (1 << 2);
static const OptionBits kBindRight								= (1 << 3);

static const OptionBits kBindLeftRight							= kBindLeft      | kBindRight;
static const OptionBits kBindTopBottom							= kBindTop       | kBindBottom;
static const OptionBits kBindAllSides							= kBindTopBottom | kBindLeftRight;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIViewUtilities {
public:
	// Get an NHIView for an HIViewRef
	static NHIView						*GetHIView(HIViewRef hiView);


	// Create an NHIView
	//
	// Creates a basic NHIView object. Should be used instead of 'new NHIView',
	// in order to ensure the newly created view is correctly initialized.
	static NHIView						*CreateHIView(HIWindowRef	theWindow = NULL,
													  const HIRect	&theRect  = kHIRectZero,
													  bool			isVisible = true);


	// Get an HILayoutInfo
	static HILayoutInfo					GetLayoutInfo(OptionBits bindTo);


	// Embed a view into a scroll view
	static void							EmbedInScrollView(NHIView		*theView,
															OptionBits	theOptions = kHIScrollViewOptionsDefault);
};





#endif // NHIVIEWUTILITIES_HDR


