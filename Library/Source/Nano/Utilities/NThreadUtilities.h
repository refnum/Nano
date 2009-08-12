/*	NAME:
		NThreadUtilities.h

	DESCRIPTION:
		Thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREADUTILITIES_HDR
#define NTHREADUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadUtilities {
public:
	// Get the number of CPUs
	static UInt32						GetCPUCount(void);


	// Atomic operations
	static bool							AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue);
	static void							AtomicAdd32(           SInt32 &theValue, SInt32 theDelta);

	static void							AtomicAnd32(UInt32 &theValue, UInt32 theMask);
	static void							AtomicOr32( UInt32 &theValue, UInt32 theMask);
	static void							AtomicXor32(UInt32 &theValue, UInt32 theMask);
};





#endif // NTHREADUTILITIES_HDR


