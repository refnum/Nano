/*	NAME:
		NNetworkUtilities.h

	DESCRIPTION:
		Network utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNETWORKUTILITIES_HDR
#define NNETWORKUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkUtilities {
public:
	// Is a host reachable?
	static bool							IsHostReachable(const NString &theHost);


};

	
	


#endif // NNETWORKUTILITIES_HDR


