/*	NAME:
		TTask.cpp

	DESCRIPTION:
		NTask tests.

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
#include "NTask.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Misc
#if NN_TARGET_MAC
	static const NString kTaskCmd									= "/usr/bin/tee";
	static const NString kTaskArgs									= "-a -i";
	static const NString kTaskInput									= "Hello World\n";

#elif NN_TARGET_WINDOWS
	static const NString kTaskCmd									= "c:\\windows\\system32\\cmd.exe";
	static const NString kTaskArgs									= "Hello World";
	static const NString kTaskInput									= "Hello World\n";

#elif NN_TARGET_LINUX
	static const NString kTaskCmd									= "/usr/bin/tee";
	static const NString kTaskArgs									= "-a -i";
	static const NString kTaskInput									= "Hello World\n";

#else
	UNKNOWN TARGET
#endif





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTASK(...)												TEST_NANO(TTask, ##__VA_ARGS__)

FIXTURE_NANO(TTask)
{
	NTask		theTask;

	SETUP
	{
		theTask.SetCommand(  kTaskCmd);
		theTask.SetArguments(kTaskArgs.Split(kNStringWhitespace));
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTASK("Task")
{	NString		theInput, theOutput;
	NStatus		theErr;
	NIndex		n;



	// Perform the test
	theErr = theTask.Launch();
	REQUIRE_NOERR(theErr);



	// Write
	for (n = 0; n < 3; n++)
		{
		theTask.WriteInput(kTaskInput);
		theInput += kTaskInput;
		}



	// Read
	for (n = 0; n < 5; n++)
		{
		theOutput += theTask.ReadOutput() + theTask.ReadError();
		if (theOutput == theInput)
			break;

		theTask.WaitForTask(1);
		}

	REQUIRE(theOutput == theInput);



	// Terminate
	REQUIRE(theTask.IsRunning());
	theTask.Terminate();

	for (n = 0; n < 5; n++)
		{
		if (!theTask.IsRunning())
			break;

		theTask.WaitForTask(1);
		}

	REQUIRE(!theTask.IsRunning());
}











