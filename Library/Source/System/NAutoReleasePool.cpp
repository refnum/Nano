/*	NAME:
		NAutoReleasePool.cpp

	DESCRIPTION:
		Auto-release pool.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAutoReleasePool.h"





//============================================================================
//		NAutoReleasePool::NAutoReleasePool : Constructor.
//----------------------------------------------------------------------------
NAutoReleasePool::NAutoReleasePool(void)
{


	// Allocate the pool
	NSApplicationLoad();

	mPool = [[NSAutoreleasePool alloc] init]; 
}





//============================================================================
//		NAutoReleasePool::~NAutoReleasePool : Destructor.
//----------------------------------------------------------------------------
NAutoReleasePool::~NAutoReleasePool(void)
{


	// Release the pool
	[mPool release];
}



