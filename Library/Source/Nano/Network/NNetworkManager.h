/*	NAME:
		NNetworkManager.h

	DESCRIPTION:
		Network manager.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNETWORKMANAGER_HDR
#define NNETWORKMANAGER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NURL.h"
#include "NURLRequest.h"
#include "NURLResponse.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkManager {
public:
										NNetworkManager(void);
	virtual							   ~NNetworkManager(void);


	// Is a URL reachable?
	bool								IsReachable(const NURL &theURL);


    // Get the instance
    static NNetworkManager			   *Get(void);


private:


};





#endif // NNETWORKMANAGER_HDR


