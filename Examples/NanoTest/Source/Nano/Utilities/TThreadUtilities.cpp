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
#include "NTestFixture.h"





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
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTHREADUTILITIES(...)									TEST_NANO(TThreadUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TThreadUtilities)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTHREADUTILITIES("Atomic")
{	UInt32		theValue;



	// Perform the test
	theValue = kValueTest;
	NThreadUtilities::AtomicAnd32(theValue, kAndMask);
	REQUIRE(theValue == kAndResult);

	theValue = kValueTest;
	NThreadUtilities::AtomicOr32(theValue, kOrMask);
	REQUIRE(theValue == kOrResult);

	theValue = kValueTest;
	NThreadUtilities::AtomicXor32(theValue, kXorMask);
	REQUIRE(theValue == kXorResult);
}


