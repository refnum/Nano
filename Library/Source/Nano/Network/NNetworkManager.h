/*	NAME:
		NNetworkManager.h

	DESCRIPTION:
		Network manager.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
#include "NSingleton.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkManager : public NSingleton {
public:
										NNetworkManager(void);
	virtual							   ~NNetworkManager(void);


	// Is a URL reachable?
	bool								IsReachable(const NURL &theURL);


	// Get/set the user agent
	NString								GetUserAgent(void) const;
	void								SetUserAgent(const NString &userAgent);


    // Get the instance
    static NNetworkManager			   *Get(void);


private:
	NString								GetDefaultUserAgent(void);


private:
	NString								mUserAgent;
};





#endif // NNETWORKMANAGER_HDR


