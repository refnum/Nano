/*	NAME:
		NRectangle.cpp

	DESCRIPTION:
		Rectangle object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRectangle.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
const NRectangle kNRectangleZero;





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const Float32 x, Float32 y, Float32 width, Float32 height)
{


	// Initialize ourselves
	origin = NPoint(x, y);
	size   = NSize(width, height);
}





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const NPoint &valOrigin, const NSize &valSize)
{


	// Initialize ourselves
	origin = valOrigin;
	size   = valSize;
}





//============================================================================
//		NRectangle::~NRectangle : Destructor.
//----------------------------------------------------------------------------
NRectangle::~NRectangle(void)
{
}





//============================================================================
//		NRectangle::Clear : Clear the Rectangle.
//----------------------------------------------------------------------------
void NRectangle::Clear(void)
{


	// Clear the Rectangle
	origin.Clear();
	size.Clear();
}





//============================================================================
//		NRectangle::IsEmpty : Is the rectangle empty?
//----------------------------------------------------------------------------
bool NRectangle::IsEmpty(void) const
{


	// Test the rectangle
	return(size.IsEmpty());
}





//============================================================================
//		NRectangle::IsNotEmpty : Is the rectangle non-empty?
//----------------------------------------------------------------------------
bool NRectangle::IsNotEmpty(void) const
{


	// Test the rectangle
	return(size.IsNotEmpty());
}





//============================================================================
//		NRectangle::== : Equality operator
//----------------------------------------------------------------------------
bool NRectangle::operator == (const NRectangle &theRectangle) const
{


	// Compare the values
    return(origin == theRectangle.origin && size == theRectangle.size);
}





//============================================================================
//		NRectangle::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NRectangle::operator != (const NRectangle &theRectangle) const
{


	// Compare the values
    return(origin != theRectangle.origin || size != theRectangle.size);
}

