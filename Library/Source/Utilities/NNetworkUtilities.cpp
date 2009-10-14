/*	NAME:
		NNetworkUtilities.cpp

	DESCRIPTION:
		Network utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <SystemConfiguration/SystemConfiguration.h>

#include "NNetworkUtilities.h"





//============================================================================
//		NNetworkUtilities::IsHostReachable : Is a host reachable?
//----------------------------------------------------------------------------
bool NNetworkUtilities::IsHostReachable(const NString &theHost)
{	bool						isReachable;
	SCNetworkConnectionFlags	theFlags;



	// Get the flags
	theFlags    = 0;
	isReachable = SCNetworkCheckReachabilityByName(theHost.GetUTF8(), &theFlags);



	// Check the reachability
	if (isReachable)
		isReachable = (bool)  (theFlags & kSCNetworkFlagsReachable);
	
	if (isReachable)
		isReachable = (bool) !(theFlags & kSCNetworkFlagsConnectionRequired);
	
	return(isReachable);
}

