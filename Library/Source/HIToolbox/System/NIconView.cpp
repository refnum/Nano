/*	NAME:
		NIconView.cpp

	DESCRIPTION:
		Icon view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"
#include "NIconView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagIcon										= 'icon';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NIconView, kHIIconViewClassID, NULL);





//============================================================================
//		NIconView::NIconView : Constructor.
//----------------------------------------------------------------------------
NIconView::NIconView(HIWindowRef		theWindow,
						const HIRect	&theRect,
						const NIcon		&theIcon,
						bool			trackMouse)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateIconControl(NULL, &kQDRectZero, &kControlContentNone, !trackMouse, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetIcon(theIcon);
}





//============================================================================
//		NIconView::~NIconView : Destructor.
//----------------------------------------------------------------------------
NIconView::~NIconView(void)
{
}





//============================================================================
//		NIconView::GetIcon : Get the icon.
//----------------------------------------------------------------------------
NIcon NIconView::GetIcon(void) const
{


	// Get the icon
	return(GetContentIcon(kControlEntireControl, kControlIconContentTag));
}





//============================================================================
//		NIconView::SetIcon : Set the icon.
//----------------------------------------------------------------------------
void NIconView::SetIcon(const NIcon &theIcon)
{


	// Set the icon
	SetContentIcon(kControlEntireControl, kControlIconContentTag, theIcon);
}





//============================================================================
//		NIconView::GetTransform : Get the transform.
//----------------------------------------------------------------------------
IconTransformType NIconView::GetTransform(void) const
{


	// Get the transform
	return(GetControlUInt16(kControlEntireControl, kControlIconTransformTag));
}





//============================================================================
//		NIconView::SetTransform : Set the transform.
//----------------------------------------------------------------------------
void NIconView::SetTransform(IconTransformType theTransform)
{


	// Set the transform
	SetControlUInt16(kControlEntireControl, kControlIconTransformTag, theTransform);
}





//============================================================================
//		NIconView::GetAlignment : Get the alignment.
//----------------------------------------------------------------------------
IconAlignmentType NIconView::GetAlignment(void) const
{


	// Get the alignment
	return(GetControlUInt16(kControlEntireControl, kControlIconAlignmentTag));
}





//============================================================================
//		NIconView::SetAlignment : Set the alignment.
//----------------------------------------------------------------------------
void NIconView::SetAlignment(IconAlignmentType theAlignment)
{


	// Set the alignment
	SetControlUInt16(kControlEntireControl, kControlIconAlignmentTag, theAlignment);
}





//============================================================================
//		NIconView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NIconView::InitializeView(void)
{


	// Apply our properties
	if (HasProperty(              kPropertyNano, kTagIcon))
		SetIcon(GetPropertyString(kPropertyNano, kTagIcon));
}



