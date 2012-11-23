/*	NAME:
		TNetworkService.h

	DESCRIPTION:
		NNetworkService tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef TNETWORKSERVICE_HDR
#define TNETWORKSERVICE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkService.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class TNetworkService {
public:
	// Execute the tests
	static void							Execute(void);


private:
	static void							BrowseEvent(const NServiceInfo &theInfo, NServiceEvent theEvent, NString *theResult);
};





#endif // TNETWORKSERVICE_HDR
