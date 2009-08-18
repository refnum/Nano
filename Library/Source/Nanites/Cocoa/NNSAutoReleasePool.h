/*	NAME:
		NNSAutoReleasePool.h

	DESCRIPTION:
		Auto-release pool.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNSAUTORELEASEPOOL_HDR
#define NNSAUTORELEASEPOOL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFObject.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
DECLARE_OBJC_CLASS(NSAutoreleasePool);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StAutoReleasePool {
public:
										 StAutoReleasePool(void);
	virtual								~StAutoReleasePool(void);


private:
	static bool							&InsidePool(void);


private:
	NSAutoreleasePool					*mPool;
};






#endif // NNSAUTORELEASEPOOL_HDR





