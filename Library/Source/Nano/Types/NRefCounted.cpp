/*	NAME:
		NRefCounted.cpp

	DESCRIPTION:
		Reference-counted object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRefCounted.h"





//============================================================================
//		NRefCounted::NRefCounted : Constructor.
//----------------------------------------------------------------------------
NRefCounted::NRefCounted(void)
{


	// Initialize ourselves
	mRefCount = 1;
}





//============================================================================
//		NRefCounted::~NRefCounted : Destructor.
//----------------------------------------------------------------------------
NRefCounted::~NRefCounted(void)
{


	// Validate our state
	NN_ASSERT(mRefCount == 0);
}





//============================================================================
//		NRefCounted::Retain : Retain the object.
//----------------------------------------------------------------------------
void NRefCounted::Retain(void)
{


	// Retain the object
	mRefCount++;
	NN_ASSERT(mRefCount > 0);
}





//============================================================================
//		NRefCounted::Release : Release the object.
//----------------------------------------------------------------------------
void NRefCounted::Release(void)
{


	// Validate our state
	NN_ASSERT(mRefCount >= 1);



	// Release the object
	if (mRefCount.Decrement() == 0)
		delete this;
}





//============================================================================
//		NRefCounted::GetRetainCount : Get the retain count.
//----------------------------------------------------------------------------
UInt32 NRefCounted::GetRetainCount(void) const
{


	// Get the retain count
	return(mRefCount);
}


