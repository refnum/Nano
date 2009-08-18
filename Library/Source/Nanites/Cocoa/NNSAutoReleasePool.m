/*	NAME:
		NNSAutoReleasePool.m

	DESCRIPTION:
		Auto-release pool.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNSAutoReleasePool.h"





//============================================================================
//		StAutoReleasePool::StAutoReleasePool : Constructor.
//----------------------------------------------------------------------------
StAutoReleasePool::StAutoReleasePool(void)
{


	// Initialize ourselves
	mPool = NULL;
	
	if (!InsidePool())
		{
		mPool        = [[NSAutoreleasePool alloc] init]; 
		InsidePool() = (mPool != NULL);
		}
}





//============================================================================
//		StAutoReleasePool::~StAutoReleasePool : Destructor.
//----------------------------------------------------------------------------
StAutoReleasePool::~StAutoReleasePool(void)
{


	// Clean up
	if (mPool != NULL)
		{
		NN_ASSERT(InsidePool());
		InsidePool() = false;

		[mPool release];
		}
}





//============================================================================
//		StAutoReleasePool::InsidePool : Are we inside a pool?
//----------------------------------------------------------------------------
#pragma mark -
bool &StAutoReleasePool::InsidePool(void)
{	static bool		sInsidePool = false;



	// Get the flag
	//
	// Auto-release pools can't be nested, since they use a simple stack approach
	// to tracking the current pool.
	//
	// This means that releasing a pool while another pool is active can have
	// unexpected results, depending on which pool was created first.
	//
	// By maintaining a flag shared by all StAutoReleasePools, we can avoid this
	// by not creating a pool if one is currently in effect.
	//
	//
	// This approach is not thread-safe; it should use thread-local storage to
	// store the flag, so that we can detect overlapping pools within a single
	// thread.
	return(sInsidePool);
}






