/*	NAME:
		NMacDebug.cpp

	DESCRIPTION:
		Mac debugging support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDebug.h"





//============================================================================
//      NDebug::LogMessage : Log a message.
//----------------------------------------------------------------------------
void NDebug::LogMessage(const std::string &theMsg)
{


	// Log the message
	fprintf(stderr, "%s", theMsg.c_str());
}



