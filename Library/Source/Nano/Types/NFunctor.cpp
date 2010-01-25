/*	NAME:
		NFunctor.cpp

	DESCRIPTION:
		Functor object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"





//============================================================================
//		FunctorNull : The "null" functor.
//----------------------------------------------------------------------------
void FunctorNull(void)
{
}





//============================================================================
//		FunctorList : Execute a list of functors.
//----------------------------------------------------------------------------
void FunctorList(const NFunctorList &theList)
{	NFunctorListConstIterator	theIter;



	// Execute the functors
	for (theIter = theList.begin(); theIter != theList.end(); theIter++)
		{
		if (*theIter != NULL)
			(*theIter)();
		}
}

