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
//		Macros
//----------------------------------------------------------------------------
// Obj-C class declaration
#ifdef __OBJC__
	#define DECLARE_OBJC_CLASS(_name)				@class			_name
#else
	#define DECLARE_OBJC_CLASS(_name)				typedef void *	_name
#endif





//============================================================================
//		Types
//----------------------------------------------------------------------------
DECLARE_OBJC_CLASS(NSAutoreleasePool);

#ifndef __OBJC__
	typedef CFRange		NSRange;
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





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline CFRange CFRangeMake(const NSRange &nsRange)
{
	return(CFRangeMake(nsRange.location, nsRange.length));
}





#endif // NAPPKITUTILITIES_HDR


