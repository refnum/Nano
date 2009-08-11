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
//		NSize::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NSize::Compare(const NSize &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(width, theValue.width);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(height, theValue.height);
	
	return(theResult);
}

