/*	NAME:
		NMacTimeUtilities.cpp

	DESCRIPTION:
		Mac time utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"





//============================================================================
//		NTimeUtilities::GetTime : Get the time.
//----------------------------------------------------------------------------
NTime NTimeUtilities::GetTime(void)
{


	// Get the time
	return(GetCurrentEventTime());
}
