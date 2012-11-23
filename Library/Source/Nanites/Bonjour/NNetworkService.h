/*	NAME:
		NNetworkService.h

	DESCRIPTION:
		Bonjour-based network service discovery.
	
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
#include "dns_sd.h"

#include "NSingleton.h"
#include "NString.h"
#include "NTimer.h"





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


// Events
typedef enum {
	kNServiceAdded,
	kNServiceRemoved
} NServiceEvent;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Info
typedef struct {
	NString			serviceType;
	NString			serviceName;
	NString			serviceDomain;
	NString			hostName;
	UInt16			hostPort;
} NServiceInfo;


// Types
typedef void *NServiceRef;

typedef nfunctor<void (const NServiceInfo &theInfo, NServiceEvent theEvent)>	NServiceBrowserFunctor;


// Lists
typedef std::map<NString, DNSServiceRef, NStringHashCompare>		NServiceMap;
typedef NServiceMap::iterator										NServiceMapIterator;
typedef NServiceMap::const_iterator									NServiceMapConstIterator;

typedef std::map<DNSServiceRef, NServiceBrowserFunctor>				NServiceBrowserMap;
typedef NServiceBrowserMap::iterator								NServiceBrowserMapIterator;
typedef NServiceBrowserMap::const_iterator							NServiceBrowserMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkService : public NSingleton {
public:
										NNetworkService(void);
	virtual							   ~NNetworkService(void);


	// Register/unregister a service
	NStatus								RegisterService(  const NString &serviceType, UInt16 thePort, const NString &theName="");
	void								UnregisterService(const NString &serviceType, UInt16 thePort);


	// Add/remove a browser
	NServiceRef							AddBrowser(const NString &serviceType, const NServiceBrowserFunctor &theFunctor);
	void								RemoveBrowser(NServiceRef serviceRef);


	// Update the browsers
	void								UpdateBrowsers(void);


    // Get the instance
    static NNetworkService			   *Get(void);


private:
	NString								GetServiceID(const NString &serviceType, UInt16 thePort);
	NStatus								GetStatus(DNSServiceErrorType dnsErr);

	static void							RegisterReply(	DNSServiceRef			theService,
														DNSServiceFlags			theFlags,
														DNSServiceErrorType		dnsErr,
														const char				*serviceName,
														const char				*regType,
														const char				*replyDomain,
														void					*userData);

	static void							BrowseReply(	DNSServiceRef			theService,
														DNSServiceFlags			theFlags,
														uint32_t				interfaceIndex,
														DNSServiceErrorType		dnsErr,
														const char              *serviceName,
														const char              *regType,
														const char              *replyDomain,
														void                    *userData);

	static void							ResolveReply(	DNSServiceRef			theService,
														DNSServiceFlags			theFlags,
														uint32_t				interfaceIndex,
														DNSServiceErrorType		dnsErr,
														const char				*fullName,
														const char				*hostName,
														uint16_t				thePort,
														uint16_t				textSize,
														const unsigned char		*textRecord,
														void					*userData);


private:
	NTimer								mTimer;
	NServiceMap							mServices;
	NServiceBrowserMap					mBrowsers;
};




#endif // NNETWORKSERVICE_HDR






