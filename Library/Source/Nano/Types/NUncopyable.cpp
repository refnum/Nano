/*	NAME:
		NUncopyable.cpp

	DESCRIPTION:
		Base class for objects that can not be copied.
		
		This is enforced at compile-time, by making our assignment operator
		and copy constructor private.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUncopyable.h"





//============================================================================
//		NUncopyable::NUncopyable : Constructor.
//----------------------------------------------------------------------------
NUncopyable::NUncopyable(void)
{
}
