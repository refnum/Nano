/*	NAME:
		NTargetNetwork.h

	DESCRIPTION:
		Network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_NETWORK_HDR
#define NTARGET_NETWORK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NURL.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetNetwork {
public:
	// Is a URL reachable?
	static bool							IsReachable(const NURL &theURL);
};





#endif // NTARGET_NETWORK_HDR
