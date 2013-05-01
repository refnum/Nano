/*	NAME:
		NNSAutoReleasePool.mm

	DESCRIPTION:
		Auto-release pool.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
	mPool = [[NSAutoreleasePool alloc] init]; 
}





//============================================================================
//		StAutoReleasePool::~StAutoReleasePool : Destructor.
//----------------------------------------------------------------------------
StAutoReleasePool::~StAutoReleasePool(void)
{


	// Clean up
	//
	// Although NSAutoreleasePool objects can be nested, the way in which this is
	// implemented can cause problems for a RAII wrappper.
	//
	// NSAutoreleasePool maintains an internal stack of pools, and each pool pushes
	// itself to this stack when created. When a pool is destroyed, it destroys not
	// only itself but also any pools above it in the stack.
	//
	//
	// This could have been done via reference counting, with each pool having a
	// reference to nested pools which it releases before releasing itself.
	//
	// However, NSAutoreleasePool objects can not be retained - and so destruction
	// is done by simply walking up the stack and dealloc'ing each pool in turn.
	//
	//
	// This means that when a StAutoReleasePool releases its pool, it has no way to
	// tell if that pool is still valid.
	//
	// If NSAutoreleasePool used reference counting then even if a nested pool was
	// destroyed by an enclosing pool the reference held by StAutoReleasePool would
	// ensure the nested pool survived until the StAutoReleasePool was destroyed.
	//
	//
	// As this is not the case, StAutoReleasePool objects should not persist beyond
	// the scope that they are declared in.
	//
	// Otherwise a StAutoReleasePool which is allocated on the heap can end up with
	// a stale reference to a NSAutoreleasePool (which has been destroyed by some
	// enclosing NSAutoreleasePool), causing a crash when the pool is double-released.
	NN_ASSERT([mPool retainCount] == 1);

	[mPool release];
}

