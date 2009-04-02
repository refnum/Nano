/*	NAME:
		NHashable.cpp

	DESCRIPTION:
		Mix-in class for hashable objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHashable.h"





//============================================================================
//		NHashable::NHashable : Constructor.
//----------------------------------------------------------------------------
NHashable::NHashable(void)
{


	// Initalize ourselves
	mHashCode = kNHashCodeNone;
}





//============================================================================
//		NHashable::~NHashable : Destructor.
//----------------------------------------------------------------------------
NHashable::~NHashable(void)
{
}





//============================================================================
//		NHashable::GetHash : Get the hash code.
//----------------------------------------------------------------------------
NHashCode NHashable::GetHash(void) const
{


	// Update the hash codde
	if (mHashCode == kNHashCodeNone)
		mHashCode = CalculateHash();



	// Get the hash code
	return(mHashCode);
}





//============================================================================
//		NHashable::ClearHash : Clear the hash code.
//----------------------------------------------------------------------------
#pragma mark -
void NHashable::ClearHash(void)
{


	// Clear the hash code
	mHashCode = kNHashCodeNone;
}





//============================================================================
//		NHashable::CalculateHash : Calculate a hash code.
//----------------------------------------------------------------------------
NHashCode NHashable::CalculateHash(NIndex theSize, const void *thePtr) const
{	NHashCode		theResult;
	const UInt8		*theData;
	NIndex			n;



	// Calculate the hash code
	theResult = kNHashCodeNone;
	theData   = (const UInt8 *) thePtr;

	for (n = 0; n < theSize; n++)
		theResult += (theData[n] * 0x31415926);

	return(theResult);
}
