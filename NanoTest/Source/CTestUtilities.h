/*	NAME:
		CTestUtilities.h

	DESCRIPTION:
		Test utilities.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CTESTUTILITIES_HDR
#define CTESTUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NLock.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CTestUtilities {
public:
	// Enable/disable debug capture
	static NString						SetDebugCapture(bool captureDebug);


	// Execute the runloop
	static void							ExecuteRunloop(NTime waitFor);


	// Test a lock
	static bool							TestLock(NLock *theLock);


private:
	static void							DebugOutputHook(const char *theMsg);
	static void							LockUnlock(NLock *theLock, NData *theData, bool *theResult);


private:
	static NString						mDebugOutput;
};





#endif // CTESTUTILITIES_HDR
