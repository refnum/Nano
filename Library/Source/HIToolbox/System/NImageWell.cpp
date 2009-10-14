/*	NAME:
		NImageWell.cpp

	DESCRIPTION:
		Image well control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NCFBundle.h"
#include "NImageWell.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagIcon										= 'icon';
static const OSType kTagImage										= 'imag';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NImageWell, kHIImageWellClassID, NULL);





//============================================================================
//		NImageWell::NImageWell : Constructor.
//----------------------------------------------------------------------------
NImageWell::NImageWell(HIWindowRef theWindow, const HIRect &theRect, const NIcon &theIcon)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateImageWellControl(NULL, &kQDRectZero, &kControlContentNone, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetIcon(theIcon);
}





//============================================================================
//		NImageWell::NImageWell : Constructor.
//----------------------------------------------------------------------------
NImageWell::NImageWell(HIWindowRef theWindow, const HIRect &theRect, const NCGImage &theImage)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateImageWellControl(NULL, &kQDRectZero, &kControlContentNone, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetImage(theImage);
}





//============================================================================
//		NImageWell::~NImageWell : Destructor.
//----------------------------------------------------------------------------
NImageWell::~NImageWell(void)
{
}





//============================================================================
//		NImageWell::GetIcon : Get the icon.
//----------------------------------------------------------------------------
NIcon NImageWell::GetIcon(void) const
{


	// Get the icon
	return(GetContentIcon(kControlEntireControl, kControlImageWellContentTag));
}





//============================================================================
//		NImageWell::SetIcon : Set the icon.
//----------------------------------------------------------------------------
void NImageWell::SetIcon(const NIcon &theIcon)
{


	// Set the icon
	SetContentIcon(kControlEntireControl, kControlImageWellContentTag, theIcon);
}





//============================================================================
//		NImageWell::GetImage : Get the image.
//----------------------------------------------------------------------------
NCGImage NImageWell::GetImage(void) const
{


	// Validate our state
	NN_ASSERT(NSystemUtilities::SystemIsAtLeast(kSystemTenFour));



	// Get the image
	return(GetContentImage(kControlEntireControl, kControlImageWellContentTag));
}





//============================================================================
//		NImageWell::SetImage : Set the image.
//----------------------------------------------------------------------------
void NImageWell::SetImage(const NCGImage &theImage)
{


	// Validate our state
	NN_ASSERT(NSystemUtilities::SystemIsAtLeast(kSystemTenFour));



	// Set the image
	SetContentImage(kControlEntireControl, kControlImageWellContentTag, theImage);
}





//============================================================================
//		NImageWell::GetTransform : Get the transform.
//----------------------------------------------------------------------------
IconTransformType NImageWell::GetTransform(void) const
{


	// Get the transform
	return(GetControlUInt16(kControlEntireControl, kControlIconTransformTag));
}





//============================================================================
//		NImageWell::SetTransform : Set the transform.
//----------------------------------------------------------------------------
void NImageWell::SetTransform(IconTransformType theTransform)
{


	// Set the transform
	SetControlUInt16(kControlEntireControl, kControlIconTransformTag, theTransform);
}





//============================================================================
//		NImageWell::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NImageWell::InitializeView(void)
{	NCFBundle	appBundle;



	// Apply our properties
	if (HasProperty(                                    kPropertyNano, kTagIcon))
		SetIcon(appBundle.GetResource(GetPropertyString(kPropertyNano, kTagIcon)));

	if (HasProperty(                                     kPropertyNano, kTagImage))
		SetImage(appBundle.GetResource(GetPropertyString(kPropertyNano, kTagImage)));
}



