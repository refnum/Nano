/*	NAME:
		NWinNetwork.cpp

	DESCRIPTION:
		Windows network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <Wininet.h>

#include "NTargetNetwork.h"





//============================================================================
//      NTargetNetwork::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
bool NTargetNetwork::IsReachable(const NURL &/*theURL*/)
{	DWORD		dwFlags;



	// Check the status
	return(InternetGetConnectedState(&dwFlags, 0));
}






