/*	NAME:
		NAutoReleasePool.h

	DESCRIPTION:
		Auto-release pool.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NAUTORELEASEPOOL_HDR
#define NAUTORELEASEPOOL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Types
//----------------------------------------------------------------------------
#ifdef __OBJC__
	@class NSAutoreleasePool;
#else
	typedef void *NSAutoreleasePool;
#endif





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAutoReleasePool {
public:
										 NAutoReleasePool(void);
	virtual								~NAutoReleasePool(void);


private:
	NSAutoreleasePool					*mPool;
};




#endif // NAUTORELEASEPOOL_HDR


