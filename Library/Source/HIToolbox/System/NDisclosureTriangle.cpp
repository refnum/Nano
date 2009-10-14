/*	NAME:
		NDisclosureTriangle.cpp

	DESCRIPTION:
		Disclosure triangle control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDisclosureTriangle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const SInt32 kControlDisclosureTriangleClosed					= 0;
static const SInt32 kControlDisclosureTriangleDisclosed					= 1;





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NDisclosureTriangle, kHIDisclosureTriangleClassID, NULL);





//============================================================================
//		NDisclosureTriangle::NDisclosureTriangle : Constructor.
//----------------------------------------------------------------------------
NDisclosureTriangle::NDisclosureTriangle(HIWindowRef								theWindow,
											const HIRect							&theRect,
											const NString							&theTitle,
											ControlDisclosureTriangleOrientation	theOrientation,
											bool									isOpen,
											bool									autoToggle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateDisclosureTriangleControl(NULL, &kQDRectZero,
												theOrientation,
												theTitle,
												isOpen ? kControlDisclosureTriangleDisclosed : kControlDisclosureTriangleClosed,
												theTitle.IsNotEmpty(),
												autoToggle,
												&theView);

	NN_ASSERT_NOERR(theErr);

	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NDisclosureTriangle::~NDisclosureTriangle : Destructor.
//----------------------------------------------------------------------------
NDisclosureTriangle::~NDisclosureTriangle(void)
{
}





//============================================================================
//		NDisclosureTriangle::IsOpen : Is the Triangle in the open state?
//----------------------------------------------------------------------------
bool NDisclosureTriangle::IsOpen(void) const
{


	// Get the state
	return(GetCurrentValue() != kControlDisclosureTriangleClosed);
}





//============================================================================
//		NDisclosureTriangle::SetOpen : Set the open state.
//----------------------------------------------------------------------------
void NDisclosureTriangle::SetOpen(bool isOpen)
{


	// Set the value
	SetCurrentValue(isOpen ? kControlDisclosureTriangleDisclosed : kControlDisclosureTriangleClosed);
}
