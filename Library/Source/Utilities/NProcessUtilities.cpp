/*	NAME:
		NProcessUtilities.cpp

	DESCRIPTION:
		Process utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEvent.h"
#include "NProcess.h"
#include "NTask.h"
#include "NProcessUtilities.h"





//============================================================================
//		NProcessUtilities::GetProcesses : Get the process list.
//----------------------------------------------------------------------------
NProcessList NProcessUtilities::GetProcesses(void)
{	ProcessSerialNumber		thePSN = { 0, kNoProcess };
	NProcessList			theList;
	OSStatus				theErr;



	// Build the process list
	do
		{
		theErr = GetNextProcess(&thePSN);
		NN_ASSERT(theErr == noErr || theErr == procNotFound);

		if (theErr == noErr)
			theList.push_back(thePSN);
		}
	while (theErr == noErr);



	// Make sure we found ourselves!
	NN_ASSERT(!theList.empty());

	return(theList);
}





//============================================================================
//		NProcessUtilities::Transform : Transform the current process.
//----------------------------------------------------------------------------
void NProcessUtilities::Transform(ProcessApplicationTransformState theState)
{	NProcess	thisApp;
	OSStatus	theErr;



	// Transform the process
	theErr = TransformProcessType(thisApp, theState);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NProcessUtilities::LaunchReplacement : Launch a replacement app.
//----------------------------------------------------------------------------
void NProcessUtilities::LaunchReplacement(const NString &thePath)
{	NTask		*theTask;
	NString		appPath;
	NProcess	thisApp;
	OSStatus	theErr;



	// Get the state we need
	theTask = new NTask;
	appPath = thePath.IsNotEmpty() ? thePath : thisApp.GetInfo().GetValueString(kProcessBundlePathKey);
	NN_ASSERT(appPath.IsNotEmpty());



	// Launch the new app
	setenv("PATH_NEW_APP", appPath.GetUTF8(), 1);

	theTask->SetCommand("/bin/bash");
	theTask->SetArguments("-c", "{\n"
								 "for (( n = 0; n < 240; n++ ))\n"
								 "do\n"
								 "    /bin/sleep 0.25\n"
								 "    if [[ $(/bin/ps -xp $PPID | /usr/bin/wc -l) -ne 2 ]]\n"
								 "    then\n"
								 "        /usr/bin/open \"${PATH_NEW_APP}\"\n"
								 "        exit\n"
								 "    fi\n"
								 "done\n"
								 "}\n", "&>/dev/null", "&", NULL);

	theErr = theTask->Launch();
	NN_ASSERT_NOERR(theErr);



	// Quit the current app
	if (theErr == noErr)
		NCarbonEvent::CreateCommand(kHICommandQuit).PostToQueue();
}


