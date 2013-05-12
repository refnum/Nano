/*	NAME:
		TThreadUtilities.cpp

	DESCRIPTION:
		NThreadUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"

#include "TThreadUtilities.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt32 kValueTest										= 0x8917B3ED;
static const UInt32 kAndMask										= 0x10101010;
static const UInt32 kAndResult										= 0x00101000;

static const UInt32 kOrMask											= 0x74747474;
static const UInt32 kOrResult										= 0xFD77F7FD;

static const UInt32 kXorMask										= 0x918DF8E9;
static const UInt32 kXorResult										= 0x189A4B04;





//============================================================================
//		TThreadUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TThreadUtilities::Execute(void)
{	UInt32		theValue;



	// Bit operations
	theValue = kValueTest;
	NThreadUtilities::AtomicAnd32(theValue, kAndMask);
	NN_ASSERT(theValue == kAndResult);

	theValue = kValueTest;
	NThreadUtilities::AtomicOr32(theValue, kOrMask);
	NN_ASSERT(theValue == kOrResult);

	theValue = kValueTest;
	NThreadUtilities::AtomicXor32(theValue, kXorMask);
	NN_ASSERT(theValue == kXorResult);
}


