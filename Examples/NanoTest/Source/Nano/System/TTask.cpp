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
#include "NTask.h"

#include "TTask.h"



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
//		TTask::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TTask::Execute(void)
{	NString		theInput, theOutput;
	NTask		theTask;
	NStatus		theErr;
	NIndex		n;



	// Launch
	theTask.SetCommand(  kTaskCmd);
	theTask.SetArguments(kTaskArgs.Split(kNStringWhitespace));
	
	theErr = theTask.Launch();
	NN_ASSERT_NOERR(theErr);



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

	NN_ASSERT(theOutput == theInput);



	// Terminate
	NN_ASSERT(theTask.IsRunning());
	theTask.Terminate();

	for (n = 0; n < 5; n++)
		{
		if (!theTask.IsRunning())
			break;

		theTask.WaitForTask(1);
		}

	NN_ASSERT(!theTask.IsRunning());
}











