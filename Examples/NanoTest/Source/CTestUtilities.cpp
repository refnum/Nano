/*	NAME:
		CTestUtilities.cpp

	DESCRIPTION:
		Test utilities.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "CTestUtilities.h"





//============================================================================
//		Static variables
//----------------------------------------------------------------------------
NString CTestUtilities::mDebugOutput;





//============================================================================
//		CTestUtilities::SetDebugCapture : Enable/disable debug capture.
//----------------------------------------------------------------------------
NString CTestUtilities::SetDebugCapture(bool captureDebug)
{


	// Enable/disable debug capture
	if (captureDebug)
		{
		mDebugOutput.Clear();
		NDebug::Get()->SetDebugOutput(CTestUtilities::DebugOutputHook);
		}
	else
		NDebug::Get()->SetDebugOutput(NULL);
	
	return(mDebugOutput);
}





//============================================================================
//		CTestUtilities::ExecuteRunloop : Execute the runloop.
//----------------------------------------------------------------------------
void CTestUtilities::ExecuteRunloop(NTime waitFor)
{


#if NN_TARGET_MAC
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, waitFor, false);


#elif NN_TARGET_WINDOWS
	NTime	endTime = NTimeUtilities::GetTime() + waitFor;
	MSG		theMsg;
	
	while (GetMessage(&theMsg, NULL, 0, 0) && NTimeUtilities::GetTime() < endTime)
		{
		TranslateMessage(&theMsg);

		DispatchMessage( &theMsg);

		}



#else
	UNKNOWN RUNTIME
#endif
}





//============================================================================
//		CTestUtilities::DebugOutputHook : Debug output hook.
//----------------------------------------------------------------------------
#pragma mark -
void CTestUtilities::DebugOutputHook(const char *theMsg)
{


	// Capture the output
	mDebugOutput += NString(theMsg, kNStringLength);
}

