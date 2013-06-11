/*	NAME:
		TCommandLine.cpp

	DESCRIPTION:
		NCommandLine tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NCommandLine.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
#define kTestString													"s t r i n g"

static const char *kTestArgList[]									= {	"app",
																		"-arg1",
																		"-arg2=-2",
																		"--arg3=3.0",
																		"--arg4=\"" kTestString "\"",
																		"--arg5="   kTestString };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NCOMMANDLINE(_name, _desc)								NANO_TEST(TCommandLine, _name, _desc)

NANO_FIXTURE(TCommandLine)
{
	NCommandLine	cmdLine;

	SETUP
	{
		cmdLine.SetArguments(NN_ARRAY_SIZE(kTestArgList), kTestArgList);
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCOMMANDLINE("Presence", "Check for arguments")
{


	// Perform the test
	REQUIRE( cmdLine.HasArgument("arg1"));
	REQUIRE( cmdLine.HasArgument("arg2"));
	REQUIRE( cmdLine.HasArgument("arg3"));
	REQUIRE( cmdLine.HasArgument("arg4"));
	REQUIRE( cmdLine.HasArgument("arg5"));
	REQUIRE(!cmdLine.HasArgument("arg99"));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCOMMANDLINE("Flags", "Test flag arguments")
{


	// Perform the test
	REQUIRE(cmdLine.GetFlagSInt64("arg2") == -2);
	REQUIRE(NMathUtilities::AreEqual(cmdLine.GetFlagFloat64("arg3"), 3.0));
	REQUIRE(cmdLine.GetFlagString("arg4") == "\"" kTestString "\"");
	REQUIRE(cmdLine.GetFlagString("arg5") ==      kTestString);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCOMMANDLINE("Manipulation", "Manipulate the command line")
{	NStringList		theArgs;



	// Perform the test
	theArgs = cmdLine.GetArguments();
	REQUIRE(theArgs.size() == 6);
	REQUIRE(theArgs[0] == kTestArgList[0]);
	REQUIRE(theArgs[1] == kTestArgList[1]);
	REQUIRE(theArgs[2] == kTestArgList[2]);
	REQUIRE(theArgs[3] == kTestArgList[3]);
	REQUIRE(theArgs[4] == kTestArgList[4]);
	REQUIRE(theArgs[5] == kTestArgList[5]);
	
	cmdLine.Clear();
	REQUIRE(cmdLine.GetArguments().empty());
	
	reverse(theArgs);
	cmdLine.SetArguments(theArgs);
	theArgs = cmdLine.GetArguments();
	REQUIRE(theArgs.size() == 6);
	REQUIRE(theArgs[0] == kTestArgList[5]);
	REQUIRE(theArgs[1] == kTestArgList[4]);
	REQUIRE(theArgs[2] == kTestArgList[3]);
	REQUIRE(theArgs[3] == kTestArgList[2]);
	REQUIRE(theArgs[4] == kTestArgList[1]);
	REQUIRE(theArgs[5] == kTestArgList[0]);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCOMMANDLINE("Application", "Verify the application name")
{	NString			theValue;
	NStringList		theArgs;



	// Perform the test
	theArgs = NCommandLine::Get()->GetArguments();
	REQUIRE(theArgs.size() >= 1);
	
	theValue = theArgs[0];
	REQUIRE(theValue.GetLower().Contains("nanotest"));
}



