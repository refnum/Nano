/*	NAME:
		NNetworkService.h

	DESCRIPTION:
		Network service advertiser.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNETWORKSERVICE_HDR
#define NNETWORKSERVICE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Services
static const NString kNServiceVNC									= "_vnc._tcp";
static const NString kNServiceSSH									= "_ssh._tcp";
static const NString kNServiceRFB									= "_rfb._tcp";
static const NString kNServiceHTTP									= "_http._tcp";
static const NString kNServiceFTP									= "_ftp._tcp";

static const NString kNServiceServerAppleShare						= "_afpovertcp._tcp";
static const NString kNServiceServerSMB								= "_smb._tcp";

static const NString kNServicePrinterIPP							= "_ipp._tcp";
static const NString kNServicePrinterLPR							= "_printer._tcp";

static const NString kNServiceAirportBaseStation					= "_airport._tcp";
static const NString kNServiceiTunesLibraries						= "_daap._tcp";
static const NString kNServiceiPhotoLibraries						= "_dpap._tcp";
static const NString kNServiceiChatBuddies							= "_presence._tcp";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Internal
typedef struct NServiceAdvertiser *NServiceAdvertiserRef;


// Lists
typedef std::map<NString, NServiceAdvertiserRef>					NServiceAdvertiserMap;
typedef NServiceAdvertiserMap::iterator								NServiceAdvertiserMapIterator;
typedef NServiceAdvertiserMap::const_iterator						NServiceAdvertiserMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkService {
public:
										NNetworkService(void);
	virtual							   ~NNetworkService(void);


	// Has a service been registered?
	bool								HasService(const NString &serviceType, UInt16 thePort) const;


	// Add/remove a service
	NStatus								AddService(   const NString &serviceType, UInt16 thePort, const NString &theName="");
	void								RemoveService(const NString &serviceType, UInt16 thePort);


	// Remove all registered services
	void								RemoveServices(void);


private:
	NString								GetServiceID(const NString &serviceType, UInt16 thePort) const;


private:
	NServiceAdvertiserMap				mServices;
};




#endif // NNETWORKSERVICE_HDR






