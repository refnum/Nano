/*	NAME:
		TStringFormatter.cpp

	DESCRIPTION:
		NStringFormatter tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTestFixture.h"
#include "NString.h"

#include "CTestUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8  kTestUInt8										= 8;
static const UInt16	kTestUInt16										= 1616;
static const UInt32	kTestUInt32										= 32323232;
static const UInt64	kTestUInt64										= 6464646464646464ULL;

static const SInt8	kTestSInt8										= -8;
static const SInt16	kTestSInt16										= -1616;
static const SInt32	kTestSInt32										= -32323232;
static const SInt64	kTestSInt64										= -6464646464646464LL;

static const Float32 kTestFloat32									= (Float32) kNPi;
static const Float64 kTestFloat64									= (Float64) kNPi;

static const char    *kTestPtrChar									= "text";
static const wchar_t *kTestPtrWChar									= L"text";
static const void    *kTestPtrVoid									= (void *) 0xDEADBEEF;

static const NString kResultMissingType								= ": Missing type: found '%' without type in 'NoSpecifier [% 12345]'\n";
static const NString kResultTooFewArgs								= ": Invalid index: '%d'\n";

static const NString kResultFormat64Value32							= "32-bit value with 64-bit length modifier";
static const NString kResultFormat32Value64							= "64-bit value with 32-bit length modifier";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSTRINGFORMATTER(...)									TEST_NANO(TStringFormatter, ##__VA_ARGS__)

FIXTURE_NANO(TStringFormatter)
{
	NStringFormatter	theFormatter;
	NString				theResult;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Primitives")
{


	// Perform the test
	theResult = theFormatter.Format("ArgUInt8 [%d]", kTestUInt8);
	REQUIRE(theResult == "ArgUInt8 [8]");

	theResult = theFormatter.Format("ArgUInt16 [%d]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [1616]");

	theResult = theFormatter.Format("ArgUInt32 [%d]", kTestUInt32);
	REQUIRE(theResult == "ArgUInt32 [32323232]");

	theResult = theFormatter.Format("ArgUInt32 [%ld]", kTestUInt32);
	REQUIRE(theResult == "ArgUInt32 [32323232]");

	theResult = theFormatter.Format("ArgUInt64 [%lld]", kTestUInt64);
	REQUIRE(theResult == "ArgUInt64 [6464646464646464]");


	theResult = theFormatter.Format("ArgSInt8 [%d]", kTestSInt8);
	REQUIRE(theResult == "ArgSInt8 [-8]");

	theResult = theFormatter.Format("ArgSInt16 [%d]", kTestSInt16);
	REQUIRE(theResult == "ArgSInt16 [-1616]");

	theResult = theFormatter.Format("ArgSInt32 [%d]", kTestSInt32);
	REQUIRE(theResult == "ArgSInt32 [-32323232]");

	theResult = theFormatter.Format("ArgSInt32 [%ld]", kTestSInt32);
	REQUIRE(theResult == "ArgSInt32 [-32323232]");

	theResult = theFormatter.Format("ArgSInt64 [%lld]", kTestSInt64);
	REQUIRE(theResult == "ArgSInt64 [-6464646464646464]");


	theResult = theFormatter.Format("ArgFloat32 [%.6f]", kTestFloat32);
	REQUIRE(theResult == "ArgFloat32 [3.141593]");

	theResult = theFormatter.Format("ArgFloat64 [%.15f]", kTestFloat64);
	REQUIRE(theResult == "ArgFloat64 [3.141592653589793]");


	theResult = theFormatter.Format("ArgChar [%c]", kTestPtrChar[0]);
	REQUIRE(theResult == "ArgChar [t]");

	theResult = theFormatter.Format("ArgChar [%lc]", kTestPtrWChar[0]);
	REQUIRE(theResult == "ArgChar [t]");

	theResult = theFormatter.Format("ArgPtrChar [%s]", kTestPtrChar);
	REQUIRE(theResult == "ArgPtrChar [text]");

	theResult = theFormatter.Format("ArgPtrChar [%ls]", kTestPtrWChar);
	REQUIRE(theResult == "ArgPtrChar [text]");


	theResult = theFormatter.Format("ArgPtrVoid [%p]", kTestPtrVoid);
	REQUIRE((	theResult == "ArgPtrVoid [0xdeadbeef]" ||			// gcc/clang
				theResult == "ArgPtrVoid [DEADBEEF]"));				// MSVC


	theResult = theFormatter.Format("ArgHex [%x]", kTestUInt32);
	REQUIRE(theResult == "ArgHex [1ed36a0]");

	theResult = theFormatter.Format("ArgHEX [%X]", kTestUInt32);
	REQUIRE(theResult == "ArgHEX [1ED36A0]");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Position")
{


	// Perform the test
	theResult = theFormatter.Format("ArgPos [%3$d] [%1$d] [%d]", 11, 22, 33);
	REQUIRE(theResult == "ArgPos [33] [11] [22]");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Width")
{	SInt32		valueInt;



	// Perform the test
	valueInt  = 10;
	theResult = theFormatter.Format("ArgSInt16 [%*d]", valueInt, kTestSInt16);
	REQUIRE(theResult == "ArgSInt16 [     -1616]");

	theResult = theFormatter.Format("ArgSInt16 [%*2$d]", kTestSInt16, valueInt);
	REQUIRE(theResult == "ArgSInt16 [     -1616]");

	theResult = theFormatter.Format("ArgSInt16 [%10d]", kTestSInt16);
	REQUIRE(theResult == "ArgSInt16 [     -1616]");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Precision")
{	SInt32		valueInt;



	// Perform the test
	valueInt  = 4;
	theResult = theFormatter.Format("ArgFloat32 [%.*f]", valueInt, kTestFloat32);
	REQUIRE(theResult == "ArgFloat32 [3.1416]");

	theResult = theFormatter.Format("ArgFloat32 [%.*2$f]", kTestFloat32, valueInt);
	REQUIRE(theResult == "ArgFloat32 [3.1416]");

	theResult = theFormatter.Format("ArgFloat32 [%.4f]", kTestFloat32);
	REQUIRE(theResult == "ArgFloat32 [3.1416]");

	valueInt  = -2;
	theResult = theFormatter.Format("NegPrecisionIsDiscarded [%.*f]", valueInt, kTestFloat64);
	REQUIRE(theResult == "NegPrecisionIsDiscarded [3.141593]");

	theResult = theFormatter.Format("EmptyPrecisionIsIgnored [%-7.u]", kTestUInt16);
	REQUIRE(theResult == "EmptyPrecisionIsIgnored [1616   ]");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Alignment")
{


	// Perform the test
	theResult = theFormatter.Format("ArgUInt16 [%7d]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [   1616]");

	theResult = theFormatter.Format("ArgUInt16 [%-7d]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [1616   ]");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Counts")
{


	// Perform the test
	theResult = theFormatter.Format("OneArg [%d]", 42);
	REQUIRE(theResult == "OneArg [42]");

	theResult = theFormatter.Format("TwoArgs [%d] [%d]", 42, 23);
	REQUIRE(theResult == "TwoArgs [42] [23]");

	theResult = theFormatter.Format("ThreeArgs [%d] [%d] [%d]", 42, 23, 101);
	REQUIRE(theResult == "ThreeArgs [42] [23] [101]");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Warnings")
{	NString		theAssert;



	// Perform the test
	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("NoSpecifier [% 12345]");
	theAssert = CTestUtilities::SetDebugCapture(false);
	REQUIRE(theResult == "NoSpecifier [% 12345]");
	REQUIRE(theAssert.EndsWith(kResultMissingType));

	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("TooFewArgs [%d] [%d]", 42);
	theAssert = CTestUtilities::SetDebugCapture(false);
	REQUIRE(theResult == "TooFewArgs [42] []");
	REQUIRE(theAssert.EndsWith(kResultTooFewArgs));

	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("Format64Value32 [%lld]", kTestUInt32);
	theAssert = CTestUtilities::SetDebugCapture(false);
	REQUIRE(theAssert.Contains(kResultFormat64Value32));

	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("Format32Value64 [%ld]", kTestUInt64);
	theAssert = CTestUtilities::SetDebugCapture(false);
	REQUIRE(theAssert.Contains(kResultFormat32Value64));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGFORMATTER("Special")
{


	// Perform the test
	theResult = theFormatter.Format("PercentArg [%%]");
	REQUIRE(theResult == "PercentArg [%]");

	theResult = theFormatter.Format("TooManyArgs [%d] [%d]", 42, 23, 101);
	REQUIRE(theResult == "TooManyArgs [42] [23]");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
#if NN_TARGET_MAC

TEST_NSTRINGFORMATTER("Foundation")
{	StAutoReleasePool		autoRelease;
	NSString				*nsString;
	CFStringRef				cfString;



	// Perform the test
	nsString  = [NSString stringWithUTF8String:"NSString"];
	theResult = theFormatter.Format("Obj-C [%@]", nsString);
	REQUIRE(theResult == "Obj-C [NSString]");

	cfString  = CFSTR("CFString");
	theResult = theFormatter.Format("Obj-C [%@]", cfString);
	REQUIRE(theResult == "Obj-C [CFString]");
}

#endif // NN_TARGET_MAC



