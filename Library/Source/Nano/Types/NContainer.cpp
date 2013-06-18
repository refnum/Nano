/*	NAME:
		NContainer.cpp

	DESCRIPTION:
		Mix-in class for container objects.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NContainer.h"





//============================================================================
//		NContainer::NContainer : Constructor.
//----------------------------------------------------------------------------
NContainer::NContainer(void)
{
}





//============================================================================
//		NContainer::~NContainer : Destructor.
//----------------------------------------------------------------------------
NContainer::~NContainer(void)
{
}





//============================================================================
//		NContainer::IsEmpty : Is the container empty?
//----------------------------------------------------------------------------
bool NContainer::IsEmpty(void) const
{


	// Check our size
	return(GetSize() == 0);
}





//============================================================================
//      NContainer::IsFullRange : Does a range match the container?
//----------------------------------------------------------------------------
bool NContainer::IsFullRange(const NRange &theRange) const
{	bool	isFull;



	// Check the range
	isFull = (theRange == kNRangeAll);
	
	if (!isFull)
		isFull = (theRange.GetLocation() == 0 && theRange.GetSize() == GetSize());
	
	return(isFull);
}





//============================================================================
//      NContainer::GetNormalized : Get a normalized range.
//----------------------------------------------------------------------------
NRange NContainer::GetNormalized(const NRange &theRange) const
{


	// Get the range
	return(theRange.GetNormalized(GetSize()));
}




