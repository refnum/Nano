/*	NAME:
		TCommandLine.cpp

	DESCRIPTION:
		NCommandLine tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NSTLUtilities.h"
#include "NCommandLine.h"

#include "TCommandLine.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
#define kValueString											"s t r i n g"

static const char *kArgList[]									= {	"app",
																	"-arg1",
																	"-arg2=-2",
																	"--arg3=3.0",
																	"--arg4=\"" kValueString "\"",
																	"--arg5="   kValueString };





//============================================================================
//		TCommandLine::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TCommandLine::Execute(void)
{	NCommandLine		cmdLine(GET_ARRAY_SIZE(kArgList), kArgList);
	NString				theValue;
	NStringList			theArgs;



	// Presence
	NN_ASSERT( cmdLine.HasArgument("arg1"));
	NN_ASSERT( cmdLine.HasArgument("arg2"));
	NN_ASSERT( cmdLine.HasArgument("arg3"));
	NN_ASSERT( cmdLine.HasArgument("arg4"));
	NN_ASSERT( cmdLine.HasArgument("arg5"));
	NN_ASSERT(!cmdLine.HasArgument("arg99"));



	// Flags
	NN_ASSERT(cmdLine.GetFlagSInt32("arg2") == -2);
	NN_ASSERT(NMathUtilities::AreEqual(cmdLine.GetFlagFloat32("arg3"), 3.0f));
	NN_ASSERT(cmdLine.GetFlagString("arg4") == "\"" kValueString "\"");
	NN_ASSERT(cmdLine.GetFlagString("arg5") ==      kValueString);



	// Manipulation
	theArgs = cmdLine.GetArguments();
	NN_ASSERT(theArgs.size() == 6);
	NN_ASSERT(theArgs[0] == kArgList[0]);
	NN_ASSERT(theArgs[1] == kArgList[1]);
	NN_ASSERT(theArgs[2] == kArgList[2]);
	NN_ASSERT(theArgs[3] == kArgList[3]);
	NN_ASSERT(theArgs[4] == kArgList[4]);
	NN_ASSERT(theArgs[5] == kArgList[5]);
	
	cmdLine.Clear();
	NN_ASSERT(cmdLine.GetArguments().empty());
	
	reverse(theArgs);
	cmdLine.SetArguments(theArgs);
	theArgs = cmdLine.GetArguments();
	NN_ASSERT(theArgs.size() == 6);
	NN_ASSERT(theArgs[0] == kArgList[5]);
	NN_ASSERT(theArgs[1] == kArgList[4]);
	NN_ASSERT(theArgs[2] == kArgList[3]);
	NN_ASSERT(theArgs[3] == kArgList[2]);
	NN_ASSERT(theArgs[4] == kArgList[1]);
	NN_ASSERT(theArgs[5] == kArgList[0]);



	// Application
	theArgs = NCommandLine::Get()->GetArguments();
	NN_ASSERT(theArgs.size() >= 1);
	
	theValue = theArgs[0];
	NN_ASSERT(theValue.GetLower().Contains("nanotest"));
}

