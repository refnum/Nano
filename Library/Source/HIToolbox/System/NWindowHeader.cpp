/*	NAME:
		NWindowHeader.cpp

	DESCRIPTION:
		Window header view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWindowHeader.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NWindowHeader, kHIWindowHeaderViewClassID, NULL);





//============================================================================
//		NWindowHeader::NWindowHeader : Constructor.
//----------------------------------------------------------------------------
NWindowHeader::NWindowHeader(HIWindowRef theWindow, const HIRect &theRect, bool isList)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateWindowHeaderControl(NULL, &kQDRectZero, isList, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NWindowHeader::~NWindowHeader : Destructor.
//----------------------------------------------------------------------------
NWindowHeader::~NWindowHeader(void)
{
}





//============================================================================
//		NWindowHeader::IsListHeader : Is this a list header?
//----------------------------------------------------------------------------
bool NWindowHeader::IsListHeader(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlWindowHeaderIsListHeaderTag));
}





//============================================================================
//		NWindowHeader::SetListHeader : Set the list header state.
//----------------------------------------------------------------------------
void NWindowHeader::SetListHeader(bool isList)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlWindowHeaderIsListHeaderTag, isList);
}


