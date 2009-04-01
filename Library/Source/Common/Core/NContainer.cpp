/*	NAME:
		NContainer.cpp

	DESCRIPTION:
		Mix-in class for container objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
//		NContainer::IsNotEmpty : Is the container non-empty?
//----------------------------------------------------------------------------
bool NContainer::IsNotEmpty(void) const
{


	// Check our size
	return(GetSize() != 0);
}


