/*	NAME:
		NHIScrollView.cpp

	DESCRIPTION:
		Scroll view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIScrollView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// HIScrollView tags
//
// Documented at http://lists.apple.com/archives/carbon-dev/2005/Mar/msg00311.html
static const OSType kHIScrollViewHorizontalIndentTag				= 'hsin';
static const OSType kHIScrollViewVerticalIndentTag					= 'vsin';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHIScrollView, kHIScrollViewClassID, NULL);





//============================================================================
//		NHIScrollView::NHIScrollView : Constructor.
//----------------------------------------------------------------------------
NHIScrollView::NHIScrollView(HIWindowRef theWindow, const HIRect &theRect, OptionBits theOptions)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = HIScrollViewCreate(theOptions, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NHIScrollView::~NHIScrollView : Destructor.
//----------------------------------------------------------------------------
NHIScrollView::~NHIScrollView(void)
{
}




						
//============================================================================
//		NHIScrollView::CanNavigate : Is a navigation action possible?
//----------------------------------------------------------------------------
bool NHIScrollView::CanNavigate(HIScrollViewAction theAction) const
{


	// Check the action
	return(HIScrollViewCanNavigate(*this, theAction));
}




						
//============================================================================
//		NHIScrollView::Navigate : Navigate the view.
//----------------------------------------------------------------------------
void NHIScrollView::Navigate(HIScrollViewAction theAction)
{	OSStatus	theErr;



	// Navigate the view
	theErr = HIScrollViewNavigate(*this, theAction);
	NN_ASSERT_NOERR(theErr);
}




						
//============================================================================
//		NHIScrollView::IsScrollBarAutoHide : Are the scroll bars auto-hiding?
//----------------------------------------------------------------------------
bool NHIScrollView::IsScrollBarAutoHide(void) const
{


	// Get the state
	return(HIScrollViewGetScrollBarAutoHide(*this));
}




						
//============================================================================
//		NHIScrollView::SetScrollBarAutoHide : Set the auto-hide state.
//----------------------------------------------------------------------------
void NHIScrollView::SetScrollBarAutoHide(bool autoHide)
{	OSStatus	theErr;



	// Set the state
	theErr = HIScrollViewSetScrollBarAutoHide(*this, autoHide);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHIScrollView::GetHorizontalIndent : Get the horizontal indent.
//----------------------------------------------------------------------------
UInt32 NHIScrollView::GetHorizontalIndent(void) const
{


	// Get the value
	//
	// This tag is a float, however we expose it as a UInt32.
	return((UInt32) GetControlFloat32(kControlEntireControl, kHIScrollViewHorizontalIndentTag));
}





//============================================================================
//		NHIScrollView::SetHorizontalIndent : Set the horizontal indent.
//----------------------------------------------------------------------------
void NHIScrollView::SetHorizontalIndent(UInt32 theValue)
{


	// Set the value
	//
	// This tag is a float, however we expose it as a UInt32.
	SetControlFloat32(kControlEntireControl, kHIScrollViewHorizontalIndentTag, (Float32) theValue);
}




						
//============================================================================
//		NHIScrollView::GetVerticalIndent : Get the vertical indent.
//----------------------------------------------------------------------------
UInt32 NHIScrollView::GetVerticalIndent(void) const
{


	// Get the value
	//
	// This tag is a float, however we expose it as a UInt32.
	return((UInt32) GetControlFloat32(kControlEntireControl, kHIScrollViewVerticalIndentTag));
}





//============================================================================
//		NHIScrollView::SetVerticalIndent : Set the vertical indent.
//----------------------------------------------------------------------------
void NHIScrollView::SetVerticalIndent(UInt32 theValue)
{


	// Set the value
	//
	// This tag is a float, however we expose it as a UInt32.
	SetControlFloat32(kControlEntireControl, kHIScrollViewVerticalIndentTag, (Float32) theValue);
}

