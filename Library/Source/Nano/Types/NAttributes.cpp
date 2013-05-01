/*	NAME:
		NAttributes.cpp

	DESCRIPTION:
		Mix-in class for objects with attributes.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAttributes.h"





//============================================================================
//		NAttributes::NAttributes : Constructor.
//----------------------------------------------------------------------------
NAttributes::NAttributes(void)
{


	// Initialize ourselves
	mAttributes = kNAttributesNone;
}





//============================================================================
//		NAttributes::~NAttributes : Destructor.
//----------------------------------------------------------------------------
NAttributes::~NAttributes(void)
{
}





//============================================================================
//		NAttributes::HasAttribute : Is an attribute set?
//----------------------------------------------------------------------------
bool NAttributes::HasAttribute(NBitfield theAttribute) const
{


	// Check our state
	return((mAttributes & theAttribute) != kNAttributesNone);
}





//============================================================================
//		NAttributes::ClearAttributes : Clear attributes.
//----------------------------------------------------------------------------
void NAttributes::ClearAttributes(NBitfield theAttributes)
{


	// Clear the attributes
	SetAttributes(kNAttributesNone, theAttributes);
}





//============================================================================
//		NAttributes::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
NBitfield NAttributes::GetAttributes(void) const
{


	// Get the attributes
	return(mAttributes);
}





//============================================================================
//		NAttributes::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NAttributes::SetAttributes(NBitfield setThese, NBitfield clearThese)
{


	// Set the attributes
	mAttributes &= ~clearThese;
	mAttributes |=    setThese;
}



