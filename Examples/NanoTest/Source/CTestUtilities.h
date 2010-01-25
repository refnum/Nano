/*	NAME:
		CTestUtilities.h

	DESCRIPTION:
		Test utilities.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CTestUtilities {
public:
	// Enable/disable debug capture
	static NString						SetDebugCapture(bool captureDebug);


private:
	static void							DebugOutputHook(const char *theMsg);


private:
	static NString						mDebugOutput;
};





#endif // CTESTUTILITIES_HDR
