/*	NAME:
		NHIViewUtilities.h

	DESCRIPTION:
		HIView utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIVIEWUTILITIES_HDR
#define NHIVIEWUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NHIScrollView.h"
#include "NHIView.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const OptionBits kBindNone								= 0;
static const OptionBits kBindTop								= (1 << 0);
static const OptionBits kBindBottom								= (1 << 1);
static const OptionBits kBindLeft								= (1 << 2);
static const OptionBits kBindRight								= (1 << 3);
static const OptionBits kBindTopBottom							= kBindTop       | kBindBottom;
static const OptionBits kBindLeftRight							= kBindLeft      | kBindRight;
static const OptionBits kBindAllSides							= kBindTopBottom | kBindLeftRight;

static const OptionBits kPositionNone							= 0;
static const OptionBits kPositionTop							= (1 << 0);
static const OptionBits kPositionBottom							= (1 << 1);
static const OptionBits kPositionLeft							= (1 << 2);
static const OptionBits kPositionRight							= (1 << 3);
static const OptionBits kPositionCentreHorizontal				= kPositionLeft             | kPositionRight;
static const OptionBits kPositionCentreVertical					= kPositionTop              | kPositionBottom;
static const OptionBits kPositionCentre							= kPositionCentreHorizontal | kPositionCentreVertical;





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::vector<HIViewRef>									HIViewRefList;
typedef HIViewRefList::iterator									HIViewRefListIterator;
typedef HIViewRefList::const_iterator							HIViewRefListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIViewUtilities {
public:
	// Get an NHIView for an HIViewRef
	static NHIView						*GetView(HIViewRef hiView);


	// Create an NHIView
	//
	// Creates a basic NHIView object. Should be used instead of 'new NHIView',
	// in order to ensure the newly created view is correctly initialized.
	static NHIView						*CreateHIView(HIWindowRef	theWindow = NULL,
													  const HIRect	&theRect  = kHIRectZero,
													  bool			isVisible = true);


	// Get an HILayoutInfo
	static HILayoutInfo					GetLayoutInfo(OptionBits bindInfo, OptionBits positionInfo=kPositionNone);


	// Get the "clickable" part for a view
	static HIViewPartCode				GetClickablePart(NHIView *theView);


	// Embed a view into a scroll view
	static NHIScrollView				*EmbedInScrollView(NHIView *theView, OptionBits theOptions=kHIScrollViewOptionsDefault);


	// Embed a view into a container view
	//
	// The view is sized to fit the container, bound to its sides, and replaces any
	// existing views within the container.
	static void							EmbedInContainerView(NHIView *theView, NHIView *theContainer);


	// Wrap the system views
	//
	// Instantiates wrapper objects for those system views where the NHIView wrapper
	// provides useful bug fixes or additional features (e.g., spell checking).
	//
	// Although views are typically instantiated with a NIB_VIEW, this method can be
	// used to instantiate wrappers for an arbitrary HIWindowRef that may have been
	// created by the system (e.g., a NavServices dialog).
	static void							WrapSystemViews(HIWindowRef theWindow);


	// Get the children of a view
	static HIViewRefList				GetChildren(HIViewRef theView, bool canRecurse);
};





#endif // NHIVIEWUTILITIES_HDR


