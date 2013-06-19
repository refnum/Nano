/*	NAME:
		NCGColor.cpp

	DESCRIPTION:
		CoreGraphics color.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCGColor.h"





//============================================================================
//		NCGColor::NCGColor : Constructor.
//----------------------------------------------------------------------------
NCGColor::NCGColor(const NColor &theColor)
			: NColor(theColor)
{
}





//============================================================================
//		NCGColor::NCGColor : Constructor.
//----------------------------------------------------------------------------
NCGColor::NCGColor(CGColorRef cfObject, bool takeOwnership)
{


	// Initialize ourselves
	SetObject(cfObject, takeOwnership);
}





//============================================================================
//		NCGColor::NCGColor : Constructor.
//----------------------------------------------------------------------------
NCGColor::NCGColor(void)
{
}





//============================================================================
//		NCGColor::~NCGColor : Destructor.
//----------------------------------------------------------------------------
NCGColor::~NCGColor(void)
{
}





//============================================================================
//		NCGColor::GetObject : Get the object.
//----------------------------------------------------------------------------
NCFObject NCGColor::GetObject(void) const
{	Float32		r, g, b, a;
	NCFObject	theObject;



	// Get the object
	GetColor(r, g, b, a);
	theObject.SetObject(CGColorCreateGenericRGB(r, g, b, a));
	
	return(theObject);
}





//============================================================================
//		NCGColor::SetObject : Set the object.
//----------------------------------------------------------------------------
bool NCGColor::SetObject(CGColorRef cfObject, bool takeOwnership)
{	NCFObject		theObject(cfObject, takeOwnership);
	const CGFloat	*theComponents;
	bool			isValid;



	// Get the state we need
	isValid       = (CGColorGetNumberOfComponents(cfObject) == 4);
	theComponents =  CGColorGetComponents(cfObject);



	// Set the object
	//
	// For now we can only convert to/from the RGB color space.
	if (isValid)
		SetColor(theComponents[0], theComponents[1], theComponents[2], theComponents[3]);

	return(isValid);
}





//============================================================================
//		NCGColor::GetDeviceGray : Get the gray color space.
//----------------------------------------------------------------------------
NCFObject NCGColor::GetDeviceGray(void)
{	NCFObject	theObject;



	// Get the color space
	theObject.SetObject(CGColorSpaceCreateDeviceGray());

	return(theObject);
}





//============================================================================
//		NCGColor::GetDeviceRGB : Get the RGB color space.
//----------------------------------------------------------------------------
NCFObject NCGColor::GetDeviceRGB(void)
{	NCFObject	theObject;



	// Get the color space
	theObject.SetObject(CGColorSpaceCreateDeviceRGB());

	return(theObject);
}




