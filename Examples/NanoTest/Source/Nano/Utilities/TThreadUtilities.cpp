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
static const uint32_t kValueTest									= 0x8917B3ED;
static const uint32_t kAndMask										= 0x10101010;
static const uint32_t kAndResult									= 0x00101000;

static const uint32_t kOrMask										= 0x74747474;
static const uint32_t kOrResult										= 0xFD77F7FD;

static const uint32_t kXorMask										= 0x918DF8E9;
static const uint32_t kXorResult									= 0x189A4B04;





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
{	uint32_t	theValue;



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


