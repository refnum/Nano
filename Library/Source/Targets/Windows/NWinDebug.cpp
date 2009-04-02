/*	NAME:
		NWindowsDebug.cpp

	DESCRIPTION:
		Windows debug support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetDebug.h"





//============================================================================
//      NTargetDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
void NTargetDebug::LogMessage(const char *theMsg)
{


	// Log the message
	OutputDebugString(stderr, "%s", theMsg);
}

