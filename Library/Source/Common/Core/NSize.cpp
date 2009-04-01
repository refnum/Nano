/*	NAME:
		NSize.cpp

	DESCRIPTION:
		Size object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NSize.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
const NSize kNSizeZero;





//============================================================================
//		NSize::NSize : Constructor.
//----------------------------------------------------------------------------
NSize::NSize(Float32 valWidth, Float32 valHeight)
{


	// Initialize ourselves
	width  = valWidth;
	height = valHeight;
}





//============================================================================
//		NSize::~NSize : Destructor.
//----------------------------------------------------------------------------
NSize::~NSize(void)
{
}





//============================================================================
//		NSize::Clear : Clear the size.
//----------------------------------------------------------------------------
void NSize::Clear(void)
{


	// Clear the size
	width  = 0.0f;
	height = 0.0f;
}





//============================================================================
//		NSize::IsEmpty : Is the size empty?
//----------------------------------------------------------------------------
bool NSize::IsEmpty(void) const
{


	// Test the size
	return(NMathUtilities::IsZero(width) && NMathUtilities::IsZero(height));
}





//============================================================================
//		NSize::IsNotEmpty : Is the size non-empty?
//----------------------------------------------------------------------------
bool NSize::IsNotEmpty(void) const
{


	// Test the size
	return(NMathUtilities::NotZero(width) || NMathUtilities::NotZero(height));
}





//============================================================================
//		NSize::== : Equality operator
//----------------------------------------------------------------------------
bool NSize::operator == (const NSize &theSize) const
{


	// Compare the values
	return(NMathUtilities::AreEqual(width, theSize.width) && NMathUtilities::AreEqual(height, theSize.height));
}





//============================================================================
//		NSize::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NSize::operator != (const NSize &theSize) const
{


	// Compare the values
	return(NMathUtilities::NotEqual(width, theSize.width) || NMathUtilities::NotEqual(height, theSize.height));
}

