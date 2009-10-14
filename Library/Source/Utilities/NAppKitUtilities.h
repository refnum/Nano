/*	NAME:
		NAppKitUtilities.h

	DESCRIPTION:
		AppKit utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NAPPKITUTILITIES_HDR
#define NAPPKITUTILITIES_HDR
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
class StAutoReleasePool {
public:
										 StAutoReleasePool(void);
	virtual								~StAutoReleasePool(void);


private:
	static bool							&InsidePool(void);


private:
	NSAutoreleasePool					*mPool;
};




#endif // NAPPKITUTILITIES_HDR


