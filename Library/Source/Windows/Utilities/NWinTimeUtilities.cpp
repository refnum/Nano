/*	NAME:
		NWinTimeUtilities.cpp

	DESCRIPTION:
		Windows time utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <time.h>

#include "NTimeUtilities.h"





//============================================================================
//		NTimeUtilities::GetTime : Get the time.
//----------------------------------------------------------------------------
NTime NTimeUtilities::GetTime(void)
{

	// dair, to do; use microsecond counter
	return(((NTime) GetTickCount()) * kNTimeMillisecond);
}




