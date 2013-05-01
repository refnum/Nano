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
#include "NString.h"

#include "CTestUtilities.h"
#include "TStringFormatter.h"





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

static const char *kTestPtrChar										= "text";
static const void *kTestPtrVoid										= (void *) 0xDEADBEEF;

static const NString kResultMissingType								= ": Missing type: found '%' without type in 'NoSpecifier [% 12345]'\n";
static const NString kResultTooFewArgs								= ": Invalid index: '%d'\n";

static const NString kResultFormat64Value32							= "32-bit value with 64-bit length modifier";
static const NString kResultFormat32Value64							= "64-bit value with 32-bit length modifier";





//============================================================================
//		TStringFormatter::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TStringFormatter::Execute(void)
{	NString				theResult, theAssert;
	NStringFormatter	theFormatter;
	SInt32				valueInt;



	// Primitives
	theResult = theFormatter.Format("ArgChar [%c]", 'z');
	NN_ASSERT(theResult == "ArgChar [z]");

	theResult = theFormatter.Format("ArgUInt8 [%d]", kTestUInt8);
	NN_ASSERT(theResult == "ArgUInt8 [8]");

	theResult = theFormatter.Format("ArgUInt16 [%d]", kTestUInt16);
	NN_ASSERT(theResult == "ArgUInt16 [1616]");

	theResult = theFormatter.Format("ArgUInt32 [%d]", kTestUInt32);
	NN_ASSERT(theResult == "ArgUInt32 [32323232]");

	theResult = theFormatter.Format("ArgUInt32 [%ld]", kTestUInt32);
	NN_ASSERT(theResult == "ArgUInt32 [32323232]");

	theResult = theFormatter.Format("ArgUInt64 [%lld]", kTestUInt64);
	NN_ASSERT(theResult == "ArgUInt64 [6464646464646464]");


	theResult = theFormatter.Format("ArgSInt8 [%d]", kTestSInt8);
	NN_ASSERT(theResult == "ArgSInt8 [-8]");

	theResult = theFormatter.Format("ArgSInt16 [%d]", kTestSInt16);
	NN_ASSERT(theResult == "ArgSInt16 [-1616]");

	theResult = theFormatter.Format("ArgSInt32 [%d]", kTestSInt32);
	NN_ASSERT(theResult == "ArgSInt32 [-32323232]");

	theResult = theFormatter.Format("ArgSInt32 [%ld]", kTestSInt32);
	NN_ASSERT(theResult == "ArgSInt32 [-32323232]");

	theResult = theFormatter.Format("ArgSInt64 [%lld]", kTestSInt64);
	NN_ASSERT(theResult == "ArgSInt64 [-6464646464646464]");


	theResult = theFormatter.Format("ArgFloat32 [%.6f]", kTestFloat32);
	NN_ASSERT(theResult == "ArgFloat32 [3.141593]");

	theResult = theFormatter.Format("ArgFloat64 [%.15f]", kTestFloat64);
	NN_ASSERT(theResult == "ArgFloat64 [3.141592653589793]");


	theResult = theFormatter.Format("ArgPtrChar [%s]", kTestPtrChar);
	NN_ASSERT(theResult == "ArgPtrChar [text]");

	theResult = theFormatter.Format("ArgPtrVoid [%p]", kTestPtrVoid);
	NN_ASSERT(	theResult == "ArgPtrVoid [0xdeadbeef]" ||			// gcc
				theResult == "ArgPtrVoid [DEADBEEF]");				// VS


	theResult = theFormatter.Format("ArgHex [%x]", kTestUInt32);
	NN_ASSERT(theResult == "ArgHex [1ed36a0]");

	theResult = theFormatter.Format("ArgHEX [%X]", kTestUInt32);
	NN_ASSERT(theResult == "ArgHEX [1ED36A0]");



	// Positional arguments
	theResult = theFormatter.Format("ArgPos [%3$d] [%1$d] [%d]", 11, 22, 33);
	NN_ASSERT(theResult == "ArgPos [33] [11] [22]");



	// Width specifier
	valueInt  = 10;
	theResult = theFormatter.Format("ArgSInt16 [%*d]", valueInt, kTestSInt16);
	NN_ASSERT(theResult == "ArgSInt16 [     -1616]");

	theResult = theFormatter.Format("ArgSInt16 [%*2$d]", kTestSInt16, valueInt);
	NN_ASSERT(theResult == "ArgSInt16 [     -1616]");

	theResult = theFormatter.Format("ArgSInt16 [%10d]", kTestSInt16);
	NN_ASSERT(theResult == "ArgSInt16 [     -1616]");



	// Precision specifier
	valueInt  = 4;
	theResult = theFormatter.Format("ArgFloat32 [%.*f]", valueInt, kTestFloat32);
	NN_ASSERT(theResult == "ArgFloat32 [3.1416]");

	theResult = theFormatter.Format("ArgFloat32 [%.*2$f]", kTestFloat32, valueInt);
	NN_ASSERT(theResult == "ArgFloat32 [3.1416]");

	theResult = theFormatter.Format("ArgFloat32 [%.4f]", kTestFloat32);
	NN_ASSERT(theResult == "ArgFloat32 [3.1416]");

	valueInt  = -2;
	theResult = theFormatter.Format("NegPrecisionIsDiscarded [%.*f]", valueInt, kTestFloat64);
	NN_ASSERT(theResult == "NegPrecisionIsDiscarded [3.141593]");
	


	// Counts
	theResult = theFormatter.Format("OneArg [%d]", 42);
	NN_ASSERT(theResult == "OneArg [42]");

	theResult = theFormatter.Format("TwoArgs [%d] [%d]", 42, 23);
	NN_ASSERT(theResult == "TwoArgs [42] [23]");

	theResult = theFormatter.Format("ThreeArgs [%d] [%d] [%d]", 42, 23, 101);
	NN_ASSERT(theResult == "ThreeArgs [42] [23] [101]");



	// Special cases
	theResult = theFormatter.Format("PercentArg [%%]");
	NN_ASSERT(theResult == "PercentArg [%]");

	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("NoSpecifier [% 12345]");
	theAssert = CTestUtilities::SetDebugCapture(false);
	NN_ASSERT(theResult == "NoSpecifier [% 12345]");
	NN_ASSERT(theAssert.EndsWith(kResultMissingType));

	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("TooFewArgs [%d] [%d]", 42);
	theAssert = CTestUtilities::SetDebugCapture(false);
	NN_ASSERT(theResult == "TooFewArgs [42] []");
	NN_ASSERT(theAssert.EndsWith(kResultTooFewArgs));

	theResult = theFormatter.Format("TooManyArgs [%d] [%d]", 42, 23, 101);
	NN_ASSERT(theResult == "TooManyArgs [42] [23]");

	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("Format64Value32 [%lld]", kTestUInt32);
	theAssert = CTestUtilities::SetDebugCapture(false);
	NN_ASSERT(theAssert.Contains(kResultFormat64Value32));

	CTestUtilities::SetDebugCapture(true);
	theResult = theFormatter.Format("Format32Value64 [%ld]", kTestUInt64);
	theAssert = CTestUtilities::SetDebugCapture(false);
	NN_ASSERT(theAssert.Contains(kResultFormat32Value64));
}


