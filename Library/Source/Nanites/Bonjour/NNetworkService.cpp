/*	NAME:
		NNetworkService

	DESCRIPTION:
		Bonjour-based network service discovery.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkService.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NTime kNServiceBrowseUpdateTime						= 1.1;





//============================================================================
//		NNetworkService::NNetworkService : Constructor.
//----------------------------------------------------------------------------
NNetworkService::NNetworkService(void)
{


	// Validate our parameters
	NN_ASSERT(sizeof(DNSServiceRef) == sizeof(NServiceRef));
}





//============================================================================
//		NNetworkService::~NNetworkService : Destructor.
//----------------------------------------------------------------------------
NNetworkService::~NNetworkService(void)
{
}





//============================================================================
//		NNetworkService::RegisterService : Register a service.
//----------------------------------------------------------------------------
NStatus NNetworkService::RegisterService(const NString &serviceType, UInt16 thePort, const NString &theName)
{	DNSServiceRef			theService;
	DNSServiceErrorType		dnsErr;
	NString					theID;



	// Validate our parameters
	NN_ASSERT(!serviceType.IsEmpty());
	NN_ASSERT(thePort != 0);



	// Get the state we need
	theID      = GetServiceID(serviceType, thePort);
	theService = NULL;



	// Check our state
	if (mServices.find(theID) != mServices.end())
		return(kNErrDuplicate);



	// Register the service
	dnsErr = DNSServiceRegister(	&theService,
									0, 0,
									theName.IsEmpty() ? NULL : theName.GetUTF8(),
									serviceType.GetUTF8(),
									NULL, NULL,
									NSwapUInt16_NtoB(thePort),
									0, NULL,
									NNetworkService::RegisterReply, this);

	if (dnsErr == kDNSServiceErr_NoError)
		dnsErr = DNSServiceProcessResult(theService);



	// Update our state
	if (dnsErr == kDNSServiceErr_NoError)
		mServices[theID] = theService;

	return(GetStatus(dnsErr));
}





//============================================================================
//		NNetworkService::UnregisterService : Unregister a service.
//----------------------------------------------------------------------------
void NNetworkService::UnregisterService(const NString &serviceType, UInt16 thePort)
{	NServiceMapIterator		theIter;
	NString					theID;



	// Validate our parameters
	NN_ASSERT(!serviceType.IsEmpty());
	NN_ASSERT(thePort != 0);



	// Get the state we need
	theID   = GetServiceID(serviceType, thePort);
	theIter = mServices.find(theID);
	NN_ASSERT(theIter != mServices.end());



	// Unregister the service
	if (theIter != mServices.end())
		DNSServiceRefDeallocate(theIter->second);

	mServices.erase(theIter);
}





//============================================================================
//		NNetworkService::AddBrowser : Add a browser.
//----------------------------------------------------------------------------
NServiceRef NNetworkService::AddBrowser(const NString &serviceType, const NServiceBrowserFunctor &theFunctor)
{	DNSServiceRef			theService;
	DNSServiceErrorType		dnsErr;



	// Validate our parameters
	NN_ASSERT(!serviceType.IsEmpty());



	// Create the browser
	theService = NULL;
	dnsErr     = DNSServiceBrowse(	&theService,
									0, kDNSServiceInterfaceIndexAny,
									serviceType.GetUTF8(), NULL,
									NNetworkService::BrowseReply, this);



	// Update our state
	if (dnsErr == kDNSServiceErr_NoError)
		{
		NN_ASSERT(mBrowsers.find(theService) == mBrowsers.end());
		mBrowsers[theService] = theFunctor;
		
		if (!mTimer.HasTimer())
			mTimer.AddTimer(BindSelf(NNetworkService::UpdateBrowsers), kNServiceBrowseUpdateTime);
		}

	return(theService);
}





//============================================================================
//		NNetworkService::RemoveBrowser : Remove a browser.
//----------------------------------------------------------------------------
void NNetworkService::RemoveBrowser(NServiceRef serviceRef)
{	DNSServiceRef					theService;
	NServiceBrowserMapIterator		theIter;



	// Get the state we need
	theService = (DNSServiceRef) serviceRef;
	theIter    = mBrowsers.find(theService);

	NN_ASSERT(theIter != mBrowsers.end());



	// Remove the browser
	DNSServiceRefDeallocate(theService);
	mBrowsers.erase(theIter);

	if (mBrowsers.empty())
		mTimer.RemoveTimer();
}





//============================================================================
//		NNetworkService::UpdateBrowsers : Update the browsers.
//----------------------------------------------------------------------------
//		Note :	Service updates are currently driven by polling.
//
//				This is not ideal, but avoids having to introduce
//				platform-dependent runloops into this code.
//
//				If this class ever moves out of being a Nanite and into Nano
//				proper, we could have some platform-specific hook that allows
//				us to bind to the main runloop/window window handle and switch
//				to notifications.
//----------------------------------------------------------------------------
void NNetworkService::UpdateBrowsers(void)
{	int									numFDs, sysErr, theFD;
	struct timeval						theTimeout;
	NServiceBrowserMapConstIterator		theIter;
	fd_set								readFDs;
	DNSServiceErrorType					dnsErr;



	// Get the state we need
	memset(&theTimeout, 0x00, sizeof(theTimeout));

	FD_ZERO(&readFDs);
	numFDs = 0;



	// Prepare the file descriptors
	for (theIter = mBrowsers.begin(); theIter != mBrowsers.end(); theIter++)
		{
		// Get the state we need
		theFD = DNSServiceRefSockFD(theIter->first);
		NN_ASSERT(theFD != -1);


		// Set the FD
		FD_SET(theFD, &readFDs);
		numFDs = std::max(numFDs, theFD+1);
		}



	// Check for events
	sysErr = select(numFDs, &readFDs, NULL, NULL, &theTimeout);
	
	if (sysErr <= 0)
		return;



	// Process the results
	for (theIter = mBrowsers.begin(); theIter != mBrowsers.end(); theIter++)
		{
		// Get the state we need
		theFD = DNSServiceRefSockFD(theIter->first);
		NN_ASSERT(theFD != -1);
		
		
		// Process the result
		if (FD_ISSET(theFD, &readFDs))
			{
			dnsErr = DNSServiceProcessResult(theIter->first);
			NN_ASSERT_NOERR(dnsErr);
			}
		}
}





//============================================================================
//		NNetworkService::Get : Get the instance.
//----------------------------------------------------------------------------
NNetworkService *NNetworkService::Get(void)
{	static NNetworkService		*sInstance = NULL;



	// Get the instance
	return(CreateInstance<NNetworkService>(&sInstance));
}





//============================================================================
//		NNetworkService::GetServiceID : Get a service ID.
//----------------------------------------------------------------------------
#pragma mark -
NString NNetworkService::GetServiceID(const NString &serviceType, UInt16 thePort)
{	NString		theID;



	// Validate our parameters
	NN_ASSERT(!serviceType.IsEmpty());
	NN_ASSERT(thePort != 0);



	// Get the ID
	theID.Format("%@:%d", serviceType, thePort);
	
	return(theID);
}





//============================================================================
//		NNetworkService::GetStatus : Get a status code.
//----------------------------------------------------------------------------
NStatus NNetworkService::GetStatus(DNSServiceErrorType dnsErr)
{	NStatus		theErr;



	// Get the status
	switch (dnsErr) {
		case kDNSServiceErr_NoError:			theErr = kNoErr;				break;
		case kDNSServiceErr_BadFlags:			theErr = kNErrChecksum;			break;
		case kDNSServiceErr_AlreadyRegistered:	theErr = kNErrDuplicate;		break;
		case kDNSServiceErr_NameConflict:		theErr = kNErrDuplicate;		break;
		case kDNSServiceErr_BadState:			theErr = kNErrMalformed;		break;
		case kDNSServiceErr_BadReference:		theErr = kNErrMalformed;		break;
		case kDNSServiceErr_NoMemory:			theErr = kNErrMemory;			break;
		case kDNSServiceErr_NoSuchRecord:		theErr = kNErrNotFound;			break;
		case kDNSServiceErr_NoAuth:				theErr = kNErrNotFound;			break;
		case kDNSServiceErr_NoSuchKey:			theErr = kNErrNotFound;			break;
		case kDNSServiceErr_NoSuchName:			theErr = kNErrNotFound;			break;
		case kDNSServiceErr_Unsupported:		theErr = kNErrNotSupported;		break;
		case kDNSServiceErr_BadParam:			theErr = kNErrParam;			break;
		case kDNSServiceErr_Timeout:			theErr = kNErrTimeout;			break;
		default:
			theErr = kNErrInternal;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NNetworkService::RegisterReply : Handle a register reply.
//----------------------------------------------------------------------------
void NNetworkService::RegisterReply(DNSServiceRef			/*theService*/,
									DNSServiceFlags			/*theFlags*/,
									DNSServiceErrorType		/*dnsErr*/,
									const char				* /*serviceName*/,
									const char				* /*regType*/,
									const char				* /*replyDomain*/,
									void					* /*userData*/)
{
}





//============================================================================
//		NNetworkService::BrowseReply : Handle a browse reply.
//----------------------------------------------------------------------------
void NNetworkService::BrowseReply(	DNSServiceRef			theService,
									DNSServiceFlags			theFlags,
									uint32_t				interfaceIndex,
									DNSServiceErrorType		dnsErr,
									const char              *serviceName,
									const char              *regType,
									const char              *replyDomain,
									void                    * /*userData*/)
{	DNSServiceRef						theResolver;
	NServiceEvent						theEvent;
	NServiceInfo						theInfo;
	NServiceBrowserMapConstIterator		theIter;



	// Check our parameters
	if (dnsErr != kDNSServiceErr_NoError)
		return;



	// Get the state we need
	theIter = Get()->mBrowsers.find(theService);
	NN_ASSERT(theIter != Get()->mBrowsers.end());

	theInfo.serviceType   = NString(regType,     kNStringLength);
	theInfo.serviceName   = NString(serviceName, kNStringLength);
	theInfo.serviceDomain = NString(replyDomain, kNStringLength);
	theInfo.hostName      = "";
	theInfo.hostPort      = 0;

	theEvent = (theFlags & kDNSServiceFlagsAdd) ? kNServiceAdded : kNServiceRemoved;



	// Resolve the name
	dnsErr = DNSServiceResolve(	&theResolver,
								0, interfaceIndex,
								serviceName, regType, replyDomain,
								NNetworkService::ResolveReply,
								&theInfo);

	if (dnsErr == kDNSServiceErr_NoError)
		{
		dnsErr = DNSServiceProcessResult(theResolver);
		DNSServiceRefDeallocate(         theResolver);

		NN_ASSERT_NOERR(dnsErr);
		}



	// Send the event
	theIter->second(theInfo, theEvent);
}





//============================================================================
//		NNetworkService::ResolveReply : Handle a resolve reply.
//----------------------------------------------------------------------------
void NNetworkService::ResolveReply(	DNSServiceRef			/*theService*/,
									DNSServiceFlags			/*theFlags*/,
									uint32_t				/*interfaceIndex*/,
									DNSServiceErrorType		dnsErr,
									const char				* /*fullName*/,
									const char				*hostName,
									uint16_t				thePort,
									uint16_t				/*textSize*/,
									const unsigned char		* /*textRecord*/,
									void					*userData)
{	NServiceInfo	*theInfo;



	// Check our parameters
	if (dnsErr != kDNSServiceErr_NoError)
		return;



	// Update the info
	theInfo = (NServiceInfo *) userData;

	theInfo->hostName = NString(hostName, kNStringLength);
	theInfo->hostPort = NSwapUInt16_BtoN(thePort);
}







