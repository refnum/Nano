/*	NAME:
		TStringFormatter.cpp

	DESCRIPTION:
		NStringFormatter tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
static const UInt8  kValueUInt8										= 8;
static const UInt16	kValueUInt16									= 1616;
static const UInt32	kValueUInt32									= 32323232;
static const UInt64	kValueUInt64									= 6464646464646464ULL;

static const SInt8	kValueSInt8										= -8;
static const SInt16	kValueSInt16									= -1616;
static const SInt32	kValueSInt32									= -32323232;
static const SInt64	kValueSInt64									= -6464646464646464LL;

static const Float32 kValueFloat32									= (Float32) kNPi;
static const Float64 kValueFloat64									= (Float64) kNPi;

static const char *kValuePtrChar									= "text";
static const void *kValuePtrVoid									= (void *) 0xDEADBEEF;

static const NString kResultMissingType								= ": Missing type: found '%' without type in 'NoSpecifier [% 12345]'\n";
static const NString kResultTooFewArgs								= ": Wrong argument count: 'TooFewArgs [%d] [%d]' references 2 arguments, but 1 supplied\n";
static const NString kResultTooManyArgs								= ": Wrong argument count: 'TooManyArgs [%d] [%d]' references 2 arguments, but 3 supplied\n";





//============================================================================
//		TStringFormatter::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TStringFormatter::Execute(void)
{	NString				theResult, theAssert;
	NStringFormatter	testFormatter;



	// Validate types
	theResult = testFormatter.Format("ArgChar [%c]", 'z');
	NN_ASSERT(theResult == "ArgChar [z]");

	theResult = testFormatter.Format("ArgUInt8 [%d]", kValueUInt8);
	NN_ASSERT(theResult == "ArgUInt8 [8]");

	theResult = testFormatter.Format("ArgUInt16 [%d]", kValueUInt16);
	NN_ASSERT(theResult == "ArgUInt16 [1616]");

	theResult = testFormatter.Format("ArgUInt32 [%d]", kValueUInt32);
	NN_ASSERT(theResult == "ArgUInt32 [32323232]");

	theResult = testFormatter.Format("ArgUInt32 [%ld]", kValueUInt32);
	NN_ASSERT(theResult == "ArgUInt32 [32323232]");

	theResult = testFormatter.Format("ArgUInt64 [%lld]", kValueUInt64);
	NN_ASSERT(theResult == "ArgUInt64 [6464646464646464]");


	theResult = testFormatter.Format("ArgSInt8 [%d]", kValueSInt8);
	NN_ASSERT(theResult == "ArgSInt8 [-8]");

	theResult = testFormatter.Format("ArgSInt16 [%d]", kValueSInt16);
	NN_ASSERT(theResult == "ArgSInt16 [-1616]");

	theResult = testFormatter.Format("ArgSInt32 [%d]", kValueSInt32);
	NN_ASSERT(theResult == "ArgSInt32 [-32323232]");

	theResult = testFormatter.Format("ArgSInt32 [%ld]", kValueSInt32);
	NN_ASSERT(theResult == "ArgSInt32 [-32323232]");

	theResult = testFormatter.Format("ArgSInt64 [%lld]", kValueSInt64);
	NN_ASSERT(theResult == "ArgSInt64 [-6464646464646464]");


	theResult = testFormatter.Format("ArgFloat32 [%.6f]", kValueFloat32);
	NN_ASSERT(theResult == "ArgFloat32 [3.141593]");

	theResult = testFormatter.Format("ArgFloat64 [%.15f]", kValueFloat64);
	NN_ASSERT(theResult == "ArgFloat64 [3.141592653589793]");


	theResult = testFormatter.Format("ArgPtrChar [%s]", kValuePtrChar);
	NN_ASSERT(theResult == "ArgPtrChar [text]");

	theResult = testFormatter.Format("ArgPtrVoid [%p]", kValuePtrVoid);
	NN_ASSERT(theResult == "ArgPtrVoid [0xdeadbeef]");


	theResult = testFormatter.Format("ArgHex [%x]", kValueUInt32);
	NN_ASSERT(theResult == "ArgHex [1ed36a0]");

	theResult = testFormatter.Format("ArgHEX [%X]", kValueUInt32);
	NN_ASSERT(theResult == "ArgHEX [1ED36A0]");



	// Validate counts
	theResult = testFormatter.Format("OneArg [%d]", 42);
	NN_ASSERT(theResult == "OneArg [42]");

	theResult = testFormatter.Format("TwoArgs [%d] [%d]", 42, 23);
	NN_ASSERT(theResult == "TwoArgs [42] [23]");

	theResult = testFormatter.Format("ThreeArgs [%d] [%d] [%d]", 42, 23, 101);
	NN_ASSERT(theResult == "ThreeArgs [42] [23] [101]");



	// Validate special cases
	theResult = testFormatter.Format("PercentArg [%%]");
	NN_ASSERT(theResult == "PercentArg [%]");

	CTestUtilities::SetDebugCapture(true);
	theResult = testFormatter.Format("NoSpecifier [% 12345]");
	theAssert = CTestUtilities::SetDebugCapture(false);
	NN_ASSERT(theResult == "NoSpecifier [% 12345]");
	NN_ASSERT(theAssert.EndsWith(kResultMissingType));

	CTestUtilities::SetDebugCapture(true);
	theResult = testFormatter.Format("TooFewArgs [%d] [%d]", 42);
	theAssert = CTestUtilities::SetDebugCapture(false);
	NN_ASSERT(theResult == "TooFewArgs [42] []");
	NN_ASSERT(theAssert.EndsWith(kResultTooFewArgs));

	CTestUtilities::SetDebugCapture(true);
	theResult = testFormatter.Format("TooManyArgs [%d] [%d]", 42, 23, 101);
	theAssert = CTestUtilities::SetDebugCapture(false);
	NN_ASSERT(theResult == "TooManyArgs [42] [23]");
	NN_ASSERT(theAssert.EndsWith(kResultTooManyArgs));

}

