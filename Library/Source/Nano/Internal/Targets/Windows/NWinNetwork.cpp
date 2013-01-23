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
#include <Winhttp.h>
#include <Winsock2.h>
#include <Mswsock.h>

#include "NThread.h"
#include "NWindows.h"
#include "NAtomicList.h"
#include "NTargetThread.h"
#include "NTargetNetwork.h"





//============================================================================
//		Windows
//----------------------------------------------------------------------------
// Work around Visual Studio header bug:
//
//	http://social.msdn.microsoft.com/Forums/en/windowssdk/thread/8f468d9f-3f15-452c-803d-fc63ab3f684e
//
// Wininet.h and Winhttp.h can't be included in the same scope, due to the
// same types being declared twice.
//
// We only need one function from Wininet.h, so declare our own prototype for it.
EXTERN_C DECLSPEC_IMPORT BOOL STDAPICALLTYPE InternetGetConnectedState(LPDWORD lpdwFlags, DWORD dwReserved);





//============================================================================
//		DNS-SD
//----------------------------------------------------------------------------
#define uint16_t													UInt16
#define uint32_t													UInt32
#define int32_t														SInt32

typedef struct _DNSServiceRef_t	*DNSServiceRef;
typedef uint32_t				 DNSServiceFlags;
typedef int32_t					 DNSServiceErrorType;

#define DNSSD_API													__stdcall
#define kDNSServiceErr_NoError										0
#define kDNSServiceInterfaceIndexAny								0
#define kDNSServiceFlagsAdd											0x2

typedef void (DNSSD_API *DNSServiceRegisterReply)
(
	DNSServiceRef						sdRef,
	DNSServiceFlags						flags,
	DNSServiceErrorType					errorCode,
	const char							*name,
	const char							*regtype,
	const char							*domain,
	void								*context
);

typedef void (DNSSD_API *DNSServiceBrowseReply)
(
	DNSServiceRef						sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	DNSServiceErrorType					errorCode,
	const char							*serviceName,
	const char							*regtype,
	const char							*replyDomain,
	void								*context
);

typedef void (DNSSD_API *DNSServiceResolveReply)
(
	DNSServiceRef						sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	DNSServiceErrorType					errorCode,
	const char							*fullname,
	const char							*hosttarget,
	uint16_t							port,
	uint16_t							txtLen,
	const unsigned char					*txtRecord,
	void								*context
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceRegisterProc)
(
	DNSServiceRef						*sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	const char							*name,
	const char							*regtype,
	const char							*domain,
	const char							*host,
	uint16_t							port,
	uint16_t							txtLen,
	const void							*txtRecord,
	DNSServiceRegisterReply				callBack,
	void								*context
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceBrowseProc)
(
	DNSServiceRef						*sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	const char							*regtype,
	const char							*domain,
	DNSServiceBrowseReply				callBack,
	void								*context
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceResolveProc)
(
	DNSServiceRef						*sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	const char							*name,
	const char							*regtype,
	const char							*domain,
	DNSServiceResolveReply				callBack,
	void								*context
);

typedef int (DNSSD_API *DNSServiceRefSockFDProc)
(
	DNSServiceRef						sdRef
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceProcessResultProc)
(
	DNSServiceRef						sdRef
);

typedef void (DNSSD_API *DNSServiceRefDeallocateProc)
(
	DNSServiceRef						sdRef
);





//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
// Misc
static const NTime kNetworkSleepTime								= 0.1f;


// URL Response
static const NIndex kHTTP_BufferSize								= 16 * kNKilobyte;
static const TCHAR *kHTTP_AllTypes[]								= { L"*/*", NULL };
static const TCHAR *kHTTP_GET										= L"GET";
static const TCHAR *kHTTP_HEAD										= L"HEAD";
static const TCHAR *kHTTP_POST										= L"POST";


// Network services
static const NTime kNServiceBrowseUpdateTime						= 1.3;


// Sockets
static const NIndex kNSocketAcceptAddressSize						= sizeof(SOCKADDR_IN) + 16;
static const NIndex kNSocketWriteBufferSize							= 64 * kNKilobyte;





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// URL Response
typedef struct {
	NURLResponse				   *theResponse;

	HINTERNET						hSession;
	HINTERNET						hConnect;
	HINTERNET						hRequest;

	bool							didCancel;
	UInt8							theBuffer[kHTTP_BufferSize];
} WinURLResponse;


// Network services
typedef struct NServiceAdvertiser {
	DNSServiceRef					theService;
} NServiceAdvertiser;


typedef struct NServiceBrowser {
	DNSServiceRef					theService;
	NNetworkBrowserEventFunctor		theFunctor;
} NServiceBrowser;

typedef std::vector<NServiceBrowser*>								NServiceBrowserList;
typedef NServiceBrowserList::iterator								NServiceBrowserListListIterator;
typedef NServiceBrowserList::const_iterator							NServiceBrowserListConstIterator;



// Sockets
typedef struct NSocketInfo {
	NSocket						   *nanoSocket;
	SOCKET							nativeSocket;
	SOCKET							acceptSocket;
	NSocketRef						parentSocket;
	
	LONG							isClosing;
	LONG							canRead;
	LONG							canWrite;

	OVERLAPPED						eventAccept;
	OVERLAPPED						eventOpen;
	OVERLAPPED						eventRead;
	OVERLAPPED						eventWrite;

	WSABUF							bufferReadZero;
	WSABUF							bufferWriteZero;
	NData							bufferWrite;
	char							bufferAccept[kNSocketAcceptAddressSize * 2];
} NSocketInfo;





//============================================================================
//      Internal globals
//----------------------------------------------------------------------------
// Network services
static NSpinLock			gDNSServiceLock;
static NServiceBrowserList	gDNSServiceBrowsers;
static bool					gDNSServiceInitialised					= false;
static NTimer			   *gDNSServiceTimer						= NULL;

static DNSServiceRegisterProc		gDNSServiceRegister				= NULL;
static DNSServiceBrowseProc			gDNSServiceBrowse				= NULL;
static DNSServiceResolveProc		gDNSServiceResolve				= NULL;
static DNSServiceRefSockFDProc		gDNSServiceRefSockFD			= NULL;
static DNSServiceProcessResultProc	gDNSServiceProcessResult		= NULL;
static DNSServiceRefDeallocateProc	gDNSServiceRefDeallocate		= NULL;


// Sockets
static LPFN_ACCEPTEX	gAcceptEx									= NULL;
static LPFN_CONNECTEX	gConnectEx									= NULL;

static NSpinLock    gSocketLock;
static NIndex       gSocketCount									= 0;
static SOCKET       gSocketFuncs                                    = INVALID_SOCKET;
static HANDLE       gSocketIOCP										= NULL;
static LONG			gSocketRunning									= false;





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
static NSocketRef SocketCreate(NSocket *nanoSocket, SOCKET nativeSocket, NSocketRef parentSocket);
static void       SocketTerminate(void);
static bool       SocketThreadAdd(void);





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      GetHttpCommand : Get an HTTP command.
//----------------------------------------------------------------------------
static const TCHAR *GetHttpCommand(NURLCommand theCmd)
{	const TCHAR		*theResult;



	// Get the command
	switch (theCmd) {
		case kURLGet:		theResult = kHTTP_GET;		break;
		case kURLHead:		theResult = kHTTP_HEAD;		break;
		case kURLPost:		theResult = kHTTP_POST;		break;
		default:
			NN_LOG("Unknown http command: %d", theCmd);
			theResult = kHTTP_GET;
			break;
		}
	
	return(theResult);
}





//============================================================================
//      WinURLResponseCallback : WinURLResponse callback.
//----------------------------------------------------------------------------
static void CALLBACK WinURLResponseCallback(HINTERNET /*hInternet*/, DWORD_PTR theContext, DWORD theEvent, void * /*statusInfo*/, DWORD dwStatusSize)
{	WinURLResponse			*winResponse = (WinURLResponse *) theContext;
	NData					theData;
	BOOL					wasOK;



	// Handle the event
	switch (theEvent) {
		case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
			// Request the headers
			wasOK = WinHttpReceiveResponse(winResponse->hRequest, NULL);
			NN_ASSERT(wasOK);
			break;


		case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
			// Request the content
			wasOK = WinHttpQueryDataAvailable(winResponse->hRequest, NULL);
			NN_ASSERT(wasOK);
			break;


		case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
			// Request the data
			wasOK = WinHttpReadData(winResponse->hRequest, winResponse->theBuffer, kHTTP_BufferSize, NULL);
			NN_ASSERT(wasOK);
			break;


		case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
			// Read the data
			//
			// The arrival of each chunk of data triggers the read of the next chunk.
			// Once the final chunk of data arrives, the next request will return 0.
			if (dwStatusSize == 0)
				winResponse->theResponse->DelegateFinished(kNoErr);
			else
				{
				NN_ASSERT(dwStatusSize >= 1 && dwStatusSize <= kHTTP_BufferSize);
				winResponse->theResponse->DelegateData(dwStatusSize, winResponse->theBuffer);

				wasOK = WinHttpReadData(winResponse->hRequest, winResponse->theBuffer, kHTTP_BufferSize, NULL);
				NN_ASSERT(wasOK);
				}
			break;


		case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
			// Cancel the response
			//
			// The handle must be cleared first, since invoking the delegate may
			// destroy the response.
			if (winResponse->didCancel)
				{
				winResponse->hRequest = NULL;
				winResponse->theResponse->DelegateFinished(kNErrCancelled);
				}
			break;


		default:
			break;
		}
}





//============================================================================
//		NetworkServiceUpdateBrowsers : Update the browsers.
//----------------------------------------------------------------------------
//		Note :	The Windows Bonjour API does not provide a run-loop style
//				mechanism for handling events, so we need to either poll or
//				use a blocking window/thread.
//
//				For now we poll, as this allows us to handle multiple browsers
//				without having a window/thread per browser.
//----------------------------------------------------------------------------
static void NetworkServiceUpdateBrowsers(void)
{	int									numFDs, sysErr, theFD;
	struct timeval						theTimeout;
	NServiceBrowserListConstIterator	theIter;
	fd_set								readFDs;
	DNSServiceErrorType					dnsErr;



	// Get the state we need
	memset(&theTimeout, 0x00, sizeof(theTimeout));

	FD_ZERO(&readFDs);
	numFDs = 0;



	// Prepare the file descriptors
	for (theIter = gDNSServiceBrowsers.begin(); theIter != gDNSServiceBrowsers.end(); theIter++)
		{
		// Get the state we need
		theFD = gDNSServiceRefSockFD((*theIter)->theService);
		NN_ASSERT(theFD != -1);


		// Set the FD
		#pragma warning(disable: 4389)
			FD_SET(theFD, &readFDs);
			numFDs = std::max(numFDs, theFD+1);
		#pragma warning(default: 4389)
		}



	// Check for events
	sysErr = select(numFDs, &readFDs, NULL, NULL, &theTimeout);
	if (sysErr <= 0)
		return;



	// Process the results
	for (theIter = gDNSServiceBrowsers.begin(); theIter != gDNSServiceBrowsers.end(); theIter++)
		{
		// Get the state we need
		theFD = gDNSServiceRefSockFD((*theIter)->theService);
		NN_ASSERT(theFD != -1);
		
		
		// Process the result
		if (FD_ISSET(theFD, &readFDs))
			{
			dnsErr = gDNSServiceProcessResult((*theIter)->theService);
			NN_ASSERT_NOERR(dnsErr);
			}
		}
}





//============================================================================
//		NetworkServiceRegisterReply : Handle a register reply.
//----------------------------------------------------------------------------
static void DNSSD_API NetworkServiceRegisterReply(	DNSServiceRef			/*theService*/,
													DNSServiceFlags			/*theFlags*/,
													DNSServiceErrorType		/*dnsErr*/,
													const char				* /*serviceName*/,
													const char				* /*regType*/,
													const char				* /*replyDomain*/,
													void					* /*userData*/)
{
}





//============================================================================
//		NetworkServiceResolveReply : Handle a resolve reply.
//----------------------------------------------------------------------------
static void DNSSD_API NetworkServiceResolveReply(	DNSServiceRef			/*theService*/,
													DNSServiceFlags			/*theFlags*/,
													uint32_t				/*interfaceIndex*/,
													DNSServiceErrorType		dnsErr,
													const char				* /*fullName*/,
													const char				*hostName,
													uint16_t				thePort,
													uint16_t				/*textSize*/,
													const unsigned char		* /*textRecord*/,
													void					*userData)
{	NNetworkBrowserEvent	*theEvent = (NNetworkBrowserEvent *) userData;



	// Check our parameters
	if (dnsErr != kDNSServiceErr_NoError)
		return;



	// Update the event
	theEvent->hostName = NString(hostName, kNStringLength);
	theEvent->hostPort = NSwapUInt16_BtoN(thePort);
}





//============================================================================
//		NetworkServiceBrowseReply : Handle a browse reply.
//----------------------------------------------------------------------------
static void DNSSD_API NetworkServiceBrowseReply(	DNSServiceRef			/*theService*/,
													DNSServiceFlags			theFlags,
													uint32_t				interfaceIndex,
													DNSServiceErrorType		dnsErr,
													const char              *serviceName,
													const char              *regType,
													const char              *replyDomain,
													void                    *userData)
{	NServiceBrowserRef			theBrowser = (NServiceBrowserRef) userData;
	DNSServiceRef				theResolver;
	NNetworkBrowserEvent		theEvent;



	// Check our parameters
	if (dnsErr != kDNSServiceErr_NoError)
		return;



	// Prepare the event
	theEvent.eventType = (theFlags & kDNSServiceFlagsAdd) ? kNServiceWasAdded : kNServiceWasRemoved;

	theEvent.serviceType   = NString(regType,     kNStringLength);
	theEvent.serviceName   = NString(serviceName, kNStringLength);
	theEvent.serviceDomain = NString(replyDomain, kNStringLength);
	theEvent.hostName      = "";
	theEvent.hostPort      = 0;



	// Resolve the name
	dnsErr = gDNSServiceResolve(&theResolver,
								0, interfaceIndex,
								serviceName, regType, replyDomain,
								NetworkServiceResolveReply,
								&theEvent);

	if (dnsErr == kDNSServiceErr_NoError)
		{
		dnsErr = gDNSServiceProcessResult(theResolver);
		gDNSServiceRefDeallocate(         theResolver);

		NN_ASSERT_NOERR(dnsErr);
		}



	// Dispatch the event
	theBrowser->theFunctor(theEvent);
}





//============================================================================
//		SocketErr : Process a socket error.
//----------------------------------------------------------------------------
int SocketErr(int winErr)
{


	// Process the error
	if (winErr == SOCKET_ERROR)
		winErr = WSAGetLastError();

	return(winErr);
}





//============================================================================
//		SocketInitialise : Initialise socket support.
//----------------------------------------------------------------------------
static bool SocketInitialise(void)
{	GUID		guidAcceptEx  = WSAID_ACCEPTEX;
	GUID		guidConnectEx = WSAID_CONNECTEX;
	WSADATA		winSockInfo;
	DWORD		theSize;
	int			winErr;



	// Initialise WinSock
	winErr = WSAStartup(MAKEWORD(2, 2), &winSockInfo);
	if (winErr != ERROR_SUCCESS)
		return(false);



	// Load the extension functions
	//
	// We assume that extension functions can be shared between sockets; as
	// all of our sockets use the same version of WinSock, this should be OK.
	gSocketFuncs = socket(AF_INET, SOCK_STREAM, 0);
	winErr       = (gSocketFuncs != INVALID_SOCKET) ? ERROR_SUCCESS : WSAGetLastError();

	winErr |= WSAIoctl(gSocketFuncs, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx,  sizeof(guidAcceptEx),  &gAcceptEx,  sizeof(gAcceptEx),  &theSize, NULL, NULL);
	winErr |= WSAIoctl(gSocketFuncs, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidConnectEx, sizeof(guidConnectEx), &gConnectEx, sizeof(gConnectEx), &theSize, NULL, NULL);

	if (winErr != ERROR_SUCCESS)
		{
		SocketTerminate();
		return(false);
		}

	return(true);
}





//============================================================================
//		SocketTerminate : Terminate socket support.
//----------------------------------------------------------------------------
static void SocketTerminate(void)
{	int		winErr;



	// Clean up
	if (gSocketFuncs != INVALID_SOCKET)
		{
		winErr = SocketErr(closesocket(gSocketFuncs));
		NN_ASSERT_NOERR(winErr);
		}

	winErr = SocketErr(WSACleanup());
	NN_ASSERT_NOERR(winErr);



	// Update our state
	gAcceptEx    = NULL;
	gConnectEx   = NULL;
	gSocketFuncs = INVALID_SOCKET;
}





//============================================================================
//		SocketAccept : Accept new requests on a socket.
//----------------------------------------------------------------------------
static int SocketAccept(NSocketRef theSocket)
{	int		winErr;



	// Validate our parameters
	NN_ASSERT(theSocket->acceptSocket == INVALID_SOCKET);



	// Create a new socket
	theSocket->acceptSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (theSocket->acceptSocket == INVALID_SOCKET)
		return(WSAGetLastError());



	// Perform the accept
	winErr = ERROR_SUCCESS;
	memset(&theSocket->eventAccept, 0x00, sizeof(theSocket->eventAccept));

	if (!gAcceptEx(	theSocket->nativeSocket,
					theSocket->acceptSocket,
					&theSocket->bufferAccept, 0,
					kNSocketAcceptAddressSize,
					kNSocketAcceptAddressSize,
					NULL, &theSocket->eventAccept))
		{
		winErr = WSAGetLastError();
		if (winErr == ERROR_IO_PENDING)
			winErr = ERROR_SUCCESS;
		}



	// Handle failure
	if (winErr != ERROR_SUCCESS)
		{
		closesocket(theSocket->acceptSocket);
		theSocket->acceptSocket = INVALID_SOCKET;
		}

	return(winErr);
}





//============================================================================
//		SocketTriggerRead : Trigger a read.
//----------------------------------------------------------------------------
static void SocketTriggerRead(NSocketRef theSocket)
{	DWORD	theFlags;
	int		winErr;



	// Validate our state
	NN_ASSERT(!theSocket->canRead);
	NN_ASSERT(HasOverlappedIoCompleted(&theSocket->eventRead));



	// Check our state
	if (theSocket->isClosing)
		return;



	// Trigger the read
	//
	// A zero-byte read will trigger a completion when the socket can return data.
	memset(&theSocket->eventRead, 0x00, sizeof(theSocket->eventRead));

	theFlags = 0;
	winErr   = SocketErr(WSARecv(theSocket->nativeSocket, &theSocket->bufferReadZero, 1, NULL, &theFlags, &theSocket->eventRead, NULL));

	switch (winErr) {
		case ERROR_SUCCESS:
		case WSA_IO_PENDING:
			break;

		default:
			NN_LOG("Unable to trigger read: %d", winErr);
			break;
		}
}





//============================================================================
//		SocketTriggerWrite : Trigger a write.
//----------------------------------------------------------------------------
static void SocketTriggerWrite(NSocketRef theSocket)
{	int		winErr;



	// Validate our state
	NN_ASSERT(!theSocket->canWrite);
	NN_ASSERT(HasOverlappedIoCompleted(&theSocket->eventWrite));



	// Check our state
	if (theSocket->isClosing)
		return;



	// Trigger the write
	//
	// A zero-byte write will trigger a completion when the socket can accept data.
	memset(&theSocket->eventWrite, 0x00, sizeof(theSocket->eventWrite));

	winErr = SocketErr(WSASend(theSocket->nativeSocket, &theSocket->bufferWriteZero, 1, NULL, 0, &theSocket->eventWrite, NULL));

	switch (winErr) {
		case ERROR_SUCCESS:
		case WSA_IO_PENDING:
			break;

		default:
			NN_LOG("Unable to trigger write: %d", winErr);
			break;
		}
}





//============================================================================
//		SocketThread : Socket thread.
//----------------------------------------------------------------------------
static void SocketThread(void)
{	NSocketRef			theSocket, newSocket;
	DWORD				numBytes, theFlags;
	LPOVERLAPPED		theEvent;
	int					winErr;
	BOOL				wasOK;



	// Validate our state
	NN_ASSERT(gSocketIOCP != NULL);



	// Update our state
	InterlockedExchange(&gSocketRunning, TRUE);



	// Run the thread
	//
	// NSocket is a 'reactor' model (notifications indicate when an operation can be
	// performed), whereas IOCP is a 'proactor' model (notifications indicate when an
	// operation has completed).
	//
	// To convert IOCP to a reactor model we use a zero-byte read/write after opening,
	// which lets us know when a socket is ready to read/write data.
	//
	// Data reads are always performed synchronously, and after the available data has
	// been drained we generate a final zero-byte read. Once that 'read' completes we
	// know that the socket has more data and is ready for reading again.
	//
	// Data writes are always performed asynchronously, via a per-socket write buffer
	// that holds the user's data while the write is active. Once the write has completed
	// we can mark the socket as ready for writing again.
	//
	//
	// The per-socket write buffer grows on demand to hold the largest write seen, but
	// is flushed after a write if it becomes "too large" (to ensure a 50Mb write doesn't
	// keep a 50Mb buffer around until the socket is closed).
	//
	// Since the IOCP operations run on a separate thread, we use a memory-barrier bool
	// to change the socket state. This is ensures the socket is put into the "can't
	// accept writes" state before SocketWrite() calls WSASend() (similarly for reads).
	//
	// The call to WSASend may complete (on the IOCP thread) before WASend returns to
	// SocketWrite, so we need to ensure that SocketWrite's clearing of the canWrite
	// flag happens behind a memory barrier and is never re-ordered.
	while (true)
		{
		// Wait for an event
		//
		// GQCS returns four different results:
		//
		//		wasOK == false, theEvent == NULL		=> 1. Failed to dequeue a completion packet
		//		wasOK == false, theEvent != NULL		=> 2. Dequeued a failed   completion packet
		//		wasOK == true,	theEvent != NULL		=> 3. Dequeued a valid    completion packet
		//		wasOK == true,	theEvent == NULL		=> 4. SocketThreadRemove() is stopping us
		//
		// The docs are not clear if a further test on valid packets using WSAGetOverlappedResult
		// is really necessary (or if these are indicated via the second case), but for now we
		// assume we must test twice.
		wasOK = GetQueuedCompletionStatus(gSocketIOCP, &numBytes, (PULONG_PTR) &theSocket, &theEvent, INFINITE);



		// Stop the thread
		//
		// 1. Failed to dequeue a completion packet
		// 4. SocketThreadRemove() is stopping us
		if (theEvent == NULL)
			break;



		// Ignore the event
		//
		// 2. Dequeued a failed   completion packet
		// 3. Dequeued a valid    completion packet
		if (!wasOK || !WSAGetOverlappedResult(theSocket->nativeSocket, theEvent, &numBytes, TRUE, &theFlags))
			continue;



		// Handle the event
		if (theEvent == &theSocket->eventAccept)
			{
			// Create a new socket
			//
			// The accept socket represents the connection which was just accepted.
			winErr = setsockopt(theSocket->acceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char *) &theSocket->nativeSocket, sizeof(theSocket->nativeSocket));
			NN_ASSERT_NOERR(winErr);

			newSocket             = SocketCreate(NULL, theSocket->acceptSocket, theSocket);
			newSocket->nanoSocket = new NSocket(newSocket, theSocket->nanoSocket);

			if (CreateIoCompletionPort((HANDLE) newSocket->nativeSocket, gSocketIOCP, (ULONG_PTR) newSocket, 0) == NULL)
				NN_LOG("Unable to re-attach accepte socket to IOCP!");

			SocketThreadAdd();
			theSocket->nanoSocket->SocketEvent(kNSocketHasConnection, (UIntPtr) newSocket->nanoSocket);


			// Accept further connections
			//
			// A new acceptance socket is needed to accept subsequent connections.
			theSocket->acceptSocket = INVALID_SOCKET;
			winErr = SocketAccept(theSocket);
			NN_ASSERT_NOERR(winErr);


			// Open the new socket
			newSocket->nanoSocket->SocketEvent(kNSocketDidOpen);
			SocketTriggerRead( newSocket);
			SocketTriggerWrite(newSocket);
			}


		else if (theEvent == &theSocket->eventOpen)
			{
			// Open the socket
			theSocket->nanoSocket->SocketEvent(kNSocketDidOpen);
			SocketTriggerRead( theSocket);
			SocketTriggerWrite(theSocket);
			}


		else if (theEvent == &theSocket->eventRead)
			{
			// The socket can be read from
			NN_ASSERT(!theSocket->canRead);
			InterlockedExchange(&theSocket->canRead, TRUE);

			theSocket->nanoSocket->SocketEvent(kNSocketCanRead);
			}


		else if (theEvent == &theSocket->eventWrite)
			{
			// Limit the buffer
			if (theSocket->bufferWrite.GetSize() > kNSocketWriteBufferSize)
				theSocket->bufferWrite.SetSize(kNSocketWriteBufferSize);


			// The socket can be written to
			NN_ASSERT(!theSocket->canWrite);
			InterlockedExchange(&theSocket->canWrite, TRUE);

			theSocket->nanoSocket->SocketEvent(kNSocketCanWrite);
			}

		else
			NN_LOG("Unknown socket event!");
		}



	// Update our state
	InterlockedExchange(&gSocketRunning, FALSE);
}





//============================================================================
//		SocketThreadAdd : Add the socket thread.
//----------------------------------------------------------------------------
static bool SocketThreadAdd(void)
{	StLock		acquireLock(gSocketLock);
	NStatus		theErr;



	// Validate our state
	NN_ASSERT(gSocketCount >= 0);



	// Increment the instance count
	gSocketCount++;



	// Start the thread
	if (gSocketCount == 1)
		{
		// Initialise WinSock
		if (!SocketInitialise())
			{
			gSocketCount = 0;
			return(false);
			}



		// Initialise the port			
		NN_ASSERT(gSocketIOCP == NULL);

		gSocketIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (gSocketIOCP == NULL)
			{
			SocketTerminate();

			gSocketCount = 0;
			return(false);
			}



		// Start the thread
		if (gSocketIOCP != NULL)
			{
			theErr = NTargetThread::ThreadCreate(BindFunction(SocketThread));

			while (theErr == kNoErr && !gSocketRunning)
				NTargetThread::ThreadSleep(kNetworkSleepTime);
			}
		}

	return(gSocketRunning == TRUE);
}





//============================================================================
//		SocketThreadRemove : Remove the socket thread.
//----------------------------------------------------------------------------
static void SocketThreadRemove(void)
{	StLock		acquireLock(gSocketLock);
	bool		wasOK;



	// Validate our state
	NN_ASSERT(gSocketCount >= 1);
	NN_ASSERT(gSocketIOCP  != NULL);



	// Decrement the instance count
	gSocketCount--;



	// Stop the thread
	if (gSocketCount == 0)
		{
		// Stop the thread
		PostQueuedCompletionStatus(gSocketIOCP, 0, NULL, NULL);

		while (gSocketRunning)
			NTargetThread::ThreadSleep(kNetworkSleepTime);



		// Clean up
		wasOK = (CloseHandle(gSocketIOCP) == TRUE);
		NN_ASSERT(wasOK);
		
		gSocketIOCP = NULL;
		SocketTerminate();
		}
}





//============================================================================
//		SocketCreate : Create a socket reference.
//----------------------------------------------------------------------------
static NSocketRef SocketCreate(NSocket *nanoSocket, SOCKET nativeSocket, NSocketRef parentSocket)
{	NSocketRef		theSocket;



	// Create the socket
	theSocket               = new NSocketInfo;
	theSocket->nanoSocket   = nanoSocket;
	theSocket->nativeSocket = nativeSocket;
	theSocket->acceptSocket = INVALID_SOCKET;
	theSocket->parentSocket = parentSocket;

	theSocket->isClosing = FALSE;
	theSocket->canRead   = FALSE;
	theSocket->canWrite  = FALSE;

	memset(&theSocket->eventAccept, 0x00, sizeof(theSocket->eventAccept));
	memset(&theSocket->eventOpen,   0x00, sizeof(theSocket->eventOpen));
	memset(&theSocket->eventRead,   0x00, sizeof(theSocket->eventRead));
	memset(&theSocket->eventWrite,  0x00, sizeof(theSocket->eventWrite));

	memset(&theSocket->bufferReadZero,  0x00, sizeof(theSocket->bufferReadZero));
	memset(&theSocket->bufferWriteZero, 0x00, sizeof(theSocket->bufferWriteZero));
	memset(&theSocket->bufferAccept,    0x00, sizeof(theSocket->bufferAccept));

	theSocket->bufferReadZero.buf  = (CHAR *) &theSocket->bufferReadZero;
	theSocket->bufferWriteZero.buf = (CHAR *) &theSocket->bufferWriteZero;

	return(theSocket);
}





//============================================================================
//		SocketCreateSocket : Create a socket.
//----------------------------------------------------------------------------
static int SocketCreateSocket(NSocketRef theSocket)
{


	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket == INVALID_SOCKET);



	// Create the socket
	theSocket->nativeSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (theSocket->nativeSocket == INVALID_SOCKET)
		return(WSAGetLastError());



	// Register for events
	if (CreateIoCompletionPort((HANDLE) theSocket->nativeSocket, gSocketIOCP, (ULONG_PTR) theSocket, 0) == NULL)
		return(GetLastError());

	return(ERROR_SUCCESS);
}





//============================================================================
//		SocketCreateListening : Create a listening socket.
//----------------------------------------------------------------------------
static bool SocketCreateListening(NSocketRef theSocket, UInt16 thePort)
{	SOCKADDR_IN		localAddress;
	BOOL			valueBOOL;



	// Create the socket
	if (SocketCreateSocket(theSocket) != ERROR_SUCCESS)
		return(false);

	valueBOOL = TRUE;
	setsockopt(theSocket->nativeSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &valueBOOL, sizeof(valueBOOL));



	// Bind the socket
	memset(&localAddress, 0x00, sizeof(localAddress));

	localAddress.sin_family      = AF_INET;
	localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddress.sin_port        = htons(thePort);

	if (bind(theSocket->nativeSocket, (sockaddr *) &localAddress, sizeof(localAddress)) != 0)
		return(false);



	// Listen for connections
	if (listen(theSocket->nativeSocket, SOMAXCONN) != 0)
		return(false);

	if (SocketAccept(theSocket) != ERROR_SUCCESS)
		return(false);



	// Open the socket
	theSocket->nanoSocket->SocketEvent(kNSocketDidOpen);

	return(true);
}





//============================================================================
//		SocketCreateConnecting : Create a connecting socket.
//----------------------------------------------------------------------------
static bool SocketCreateConnecting(NSocketRef theSocket, const NString &theHost, UInt16 thePort)
{	SOCKADDR_IN			localAddress, remoteAddress;
	struct hostent		*hostInfo;
	int					winErr;



	// Resolve the address
	hostInfo = gethostbyname(theHost.GetUTF8());
	if (hostInfo == NULL)
		return(false);

	memset(&remoteAddress, 0x00, sizeof(remoteAddress));
	memcpy(&remoteAddress.sin_addr.s_addr, hostInfo->h_addr, hostInfo->h_length);

	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_port   = htons(thePort);



	// Create the socket
	if (SocketCreateSocket(theSocket) != ERROR_SUCCESS)
		return(false);



	// Bind the socket
	//
	// Unlike connect(), ConnectEx() requires that the local socket be bound.
	memset(&localAddress, 0x00, sizeof(localAddress));

	localAddress.sin_family      = AF_INET;
	localAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddress.sin_port        = htons(0);

	if (bind(theSocket->nativeSocket, (sockaddr *) &localAddress, sizeof(localAddress)) != 0)
		return(false);



	// Open the socket
	memset(&theSocket->eventOpen, 0x00, sizeof(theSocket->eventOpen));

	if (!gConnectEx(theSocket->nativeSocket, (sockaddr *) &remoteAddress, sizeof(remoteAddress), NULL, 0, NULL, &theSocket->eventOpen))
		{
		winErr = WSAGetLastError();
		if (winErr != ERROR_IO_PENDING)
			return(false);
		}

	return(true);
}





//============================================================================
//      SocketDestroy : Destroy a socket.
//----------------------------------------------------------------------------
static void SocketDestroy(NSocketRef theSocket)
{	int		winErr;



	// Close the socket
	InterlockedExchange(&theSocket->isClosing, TRUE);

	winErr = SocketErr(shutdown(theSocket->nativeSocket, SD_BOTH));
	NN_ASSERT(winErr == ERROR_SUCCESS || winErr == WSAENOTCONN);

	if (theSocket->nativeSocket != INVALID_SOCKET)
		{
		winErr = SocketErr(closesocket(theSocket->nativeSocket));
		NN_ASSERT_NOERR(winErr);
		}

	if (theSocket->acceptSocket != INVALID_SOCKET)
		{
		winErr = SocketErr(closesocket(theSocket->acceptSocket));
		NN_ASSERT_NOERR(winErr);
		}



	// Wait for I/O to finish
	//
	// Although closesocket will cancel any scheduled or active IO operations,
	// the OVERLAPPED structures that those operations are using must continue
	// to exist until the operations have completed.
	//
	// Only once we know they finished can we dispose of their state.
	while (!HasOverlappedIoCompleted(&theSocket->eventAccept))
		NThread::Sleep();

	while (!HasOverlappedIoCompleted(&theSocket->eventRead))
		NThread::Sleep();

	while (!HasOverlappedIoCompleted(&theSocket->eventWrite))
		NThread::Sleep();



	// Delete the socket
	delete theSocket;

	SocketThreadRemove();
}





//============================================================================
//      NTargetNetwork::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetNetwork::IsReachable(const NURL &/*theURL*/)
{	DWORD		dwFlags;



	// Check the status
	return(InternetGetConnectedState(&dwFlags, 0) == TRUE);
}





//============================================================================
//      NTargetNetwork::URLResponseCreate : Create a URL response.
//----------------------------------------------------------------------------
NURLResponseRef NTargetNetwork::URLResponseCreate(NURLResponse *theResponse)
{	NString						userAgent, theHost, thePath;
	bool						wasOK, isSecure;
	WinURLResponse				*winResponse;
	WINHTTP_STATUS_CALLBACK		prevCallback;
	const TCHAR					*theCommand;
	NURLRequest					theRequest;
	INTERNET_PORT				thePort;
	NURL						theURL;



	// Get the state we need
	theRequest = theResponse->GetRequest();
	userAgent  = theRequest.GetValueString(kNURLRequestUserAgentKey);
	theCommand = GetHttpCommand(theRequest.GetCommand());

	theURL   = theRequest.GetURL();
	theHost  = theURL.GetHost();
	thePath  = theURL.GetPath();
	thePort  = (INTERNET_PORT) theURL.GetPort();
	isSecure = (theURL.GetScheme() == "https");



	// Create the response
	winResponse = (WinURLResponse *) calloc(1, sizeof(WinURLResponse));
	if (winResponse == NULL)
		return(NULL);

	winResponse->theResponse = theResponse;
	winResponse->didCancel   = false;



	// Create the request
	wasOK                 = false;
	winResponse->hSession = WinHttpOpen(ToWN(userAgent),
										WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
										WINHTTP_NO_PROXY_NAME,
										WINHTTP_NO_PROXY_BYPASS,
										WINHTTP_FLAG_ASYNC);

	if (winResponse->hSession != NULL)
		winResponse->hConnect = WinHttpConnect(winResponse->hSession, ToWN(theHost), thePort, 0);

	if (winResponse->hConnect != NULL)
		winResponse->hRequest = WinHttpOpenRequest(	winResponse->hConnect,
													theCommand,
													ToWN(thePath),
													NULL, WINHTTP_NO_REFERER,
													kHTTP_AllTypes,
													isSecure ? WINHTTP_FLAG_SECURE : 0);

	if (winResponse->hRequest != NULL)
		{
		prevCallback = WinHttpSetStatusCallback(winResponse->hRequest,
												WinURLResponseCallback,
												WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS,
												NULL);

		wasOK = (prevCallback != WINHTTP_INVALID_STATUS_CALLBACK);
		}



	// Handle falure
	if (!wasOK)
		{
		URLResponseDestroy((NURLResponseRef) winResponse);
		winResponse = NULL;
		}

	return((NURLResponseRef) winResponse);
}





//============================================================================
//      NTargetNetwork::URLResponseDestroy : Destroy a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseDestroy(NURLResponseRef theResponse)
{	WinURLResponse			*winResponse = (WinURLResponse *) theResponse;



	// Clean up
	if (winResponse->hRequest != NULL)
		WinHttpCloseHandle(winResponse->hRequest);

	if (winResponse->hConnect != NULL)
		WinHttpCloseHandle(winResponse->hConnect);

	if (winResponse->hSession != NULL)
		WinHttpCloseHandle(winResponse->hSession);
	
	free(winResponse);
}





//============================================================================
//      NTargetNetwork::URLResponseStart : Start a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseStart(NURLResponseRef theResponse)
{	WinURLResponse			*winResponse = (WinURLResponse *) theResponse;
	BOOL					wasOK;



	// Start the response
	wasOK = WinHttpSendRequest(	winResponse->hRequest,
								WINHTTP_NO_ADDITIONAL_HEADERS, 0,
								WINHTTP_NO_REQUEST_DATA,       0, 0,
								(DWORD_PTR) winResponse);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetNetwork::URLResponseCancel : Cancel a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseCancel(NURLResponseRef theResponse)
{	WinURLResponse			*winResponse = (WinURLResponse *) theResponse;
	BOOL					wasOK;



	// Cancel the response
	winResponse->didCancel = true;
	
	wasOK = WinHttpCloseHandle(winResponse->hRequest);
	NN_ASSERT(wasOK);



	// Wait for it to cancel
	//
	// Cancelling is an async operation, and so we need to wait
	// until the request has been processed before we can return.
	if (wasOK)
		{
		while (winResponse->hRequest != NULL)
			NThread::Sleep();
		}
}





//============================================================================
//      NTargetNetwork::ServicesAvailable : Are network services available?
//----------------------------------------------------------------------------
bool NTargetNetwork::ServicesAvailable(void)
{	StLock		acquireLock(gDNSServiceLock);
	bool		isAvailable;
	HMODULE		hModule;



	// Load the library
	if (!gDNSServiceInitialised)
		{
		// Load the library
		hModule = LoadLibrary(L"dnssd.dll");
		if (hModule != NULL)
			{
			gDNSServiceRegister      = (DNSServiceRegisterProc)      GetProcAddress(hModule, "DNSServiceRegister");
			gDNSServiceBrowse        = (DNSServiceBrowseProc)        GetProcAddress(hModule, "DNSServiceBrowse");
			gDNSServiceResolve       = (DNSServiceResolveProc)       GetProcAddress(hModule, "DNSServiceResolve");
			gDNSServiceRefSockFD     = (DNSServiceProcessResultProc) GetProcAddress(hModule, "DNSServiceRefSockFD");
			gDNSServiceProcessResult = (DNSServiceProcessResultProc) GetProcAddress(hModule, "DNSServiceProcessResult");
			gDNSServiceRefDeallocate = (DNSServiceRefDeallocateProc) GetProcAddress(hModule, "DNSServiceRefDeallocate");
			}


		// Update our state
		gDNSServiceInitialised = true;
		gDNSServiceTimer       = new NTimer;
		}



	// Check our state
	isAvailable = (	gDNSServiceRegister      != NULL &&
					gDNSServiceBrowse        != NULL &&
					gDNSServiceResolve       != NULL &&
					gDNSServiceRefSockFD     != NULL &&
					gDNSServiceProcessResult != NULL &&
					gDNSServiceRefDeallocate != NULL);

	return(isAvailable);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserCreate : Create a service advertiser.
//----------------------------------------------------------------------------
NServiceAdvertiserRef NTargetNetwork::ServiceAdvertiserCreate(const NString &serviceType, UInt16 thePort, const NString &theName)
{	NServiceAdvertiserRef	theAdvertiser;
	DNSServiceErrorType		dnsErr;



	// Validate our state
	NN_ASSERT(gDNSServiceInitialised);



	// Create the advertiser
	theAdvertiser = new NServiceAdvertiser;



	// Register the service
	dnsErr = gDNSServiceRegister(	&theAdvertiser->theService,
									0, 0,
									theName.IsEmpty() ? NULL : theName.GetUTF8(),
									serviceType.GetUTF8(),
									NULL, NULL,
									NSwapUInt16_NtoB(thePort),
									0, NULL,
									NetworkServiceRegisterReply, theAdvertiser);

	if (dnsErr == kDNSServiceErr_NoError)
		dnsErr = gDNSServiceProcessResult(theAdvertiser->theService);



	// Handle failure
	if (dnsErr != kDNSServiceErr_NoError)
		{
		delete theAdvertiser;
		theAdvertiser = NULL;
		}
	
	return(theAdvertiser);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserDestroy : Destroy a service advertiser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceAdvertiserDestroy(NServiceAdvertiserRef theAdvertiser)
{


	// Validate our state
	NN_ASSERT(gDNSServiceInitialised);



	// Destroy the advertiser
	gDNSServiceRefDeallocate(theAdvertiser->theService);

	delete theAdvertiser;
}





//============================================================================
//      NTargetNetwork::ServiceBrowserCreate : Create a service browser.
//----------------------------------------------------------------------------
NServiceBrowserRef NTargetNetwork::ServiceBrowserCreate(const NString &serviceType, const NNetworkBrowserEventFunctor &theFunctor)
{	StLock					acquireLock(gDNSServiceLock);
	NServiceBrowserRef		theBrowser;
	DNSServiceErrorType		dnsErr;



	// Validate our state
	NN_ASSERT(gDNSServiceInitialised);



	// Create the browser
	theBrowser             = new NServiceBrowser;
	theBrowser->theService = NULL;
	theBrowser->theFunctor = theFunctor;



	// Register the browser
	dnsErr = gDNSServiceBrowse(	&theBrowser->theService,
								0, kDNSServiceInterfaceIndexAny,
								serviceType.GetUTF8(), NULL,
								NetworkServiceBrowseReply, theBrowser);



	// Register for events
	if (dnsErr == kDNSServiceErr_NoError)
		{
		gDNSServiceBrowsers.push_back(theBrowser);
		if (gDNSServiceBrowsers.size() == 1)
			{
			NN_ASSERT(!gDNSServiceTimer->HasTimer());
			gDNSServiceTimer->AddTimer(BindFunction(NetworkServiceUpdateBrowsers), kNServiceBrowseUpdateTime, kNServiceBrowseUpdateTime);
			}
		}



	// Handle failure
	if (dnsErr != kDNSServiceErr_NoError)
		{
		delete theBrowser;
		theBrowser = NULL;
		}
	
	return(theBrowser);
}





//============================================================================
//      NTargetNetwork::ServiceBrowserDestroy : Destroy a service browser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceBrowserDestroy(NServiceBrowserRef theBrowser)
{	StLock		acquireLock(gDNSServiceLock);



	// Validate our state
	NN_ASSERT( gDNSServiceInitialised);
	NN_ASSERT(!gDNSServiceBrowsers.empty());



	// Update our state
	erase(gDNSServiceBrowsers, theBrowser);
	
	if (gDNSServiceBrowsers.empty())
		gDNSServiceTimer->RemoveTimer();



	// Destroy the browser
	gDNSServiceRefDeallocate(theBrowser->theService);

	delete theBrowser;
}





//============================================================================
//      NTargetNetwork::SocketOpen : Open a socket.
//----------------------------------------------------------------------------
NSocketRef NTargetNetwork::SocketOpen(NSocket *nanoSocket, const NString &theHost, UInt16 thePort)
{	NSocketRef		theSocket;
	bool			isOK;



	// Create the thread
	if (!SocketThreadAdd())
		return(NULL);



	// Create the socket
	theSocket = SocketCreate(nanoSocket, INVALID_SOCKET, NULL);

	if (theHost.IsEmpty())
		isOK = SocketCreateListening( theSocket, thePort);
	else
		isOK = SocketCreateConnecting(theSocket, theHost, thePort);



	// Handle failure
	if (!isOK)
		{
		SocketDestroy(theSocket);
		theSocket = NULL;
		}
	
	return(theSocket);
}





//============================================================================
//      NTargetNetwork::SocketClose : Close a socket.
//----------------------------------------------------------------------------
void NTargetNetwork::SocketClose(NSocketRef theSocket)
{


	// Close the socket
	SocketDestroy(theSocket);
}





//============================================================================
//      NTargetNetwork::SocketCanRead : Can a socket be read from?
//----------------------------------------------------------------------------
bool NTargetNetwork::SocketCanRead(NSocketRef theSocket)
{


	// Check our state
	return(theSocket->canRead == TRUE);
}





//============================================================================
//      NTargetNetwork::SocketCanWrite : Can a socket be written to?
//----------------------------------------------------------------------------
bool NTargetNetwork::SocketCanWrite(NSocketRef theSocket)
{


	// Check our state
	return(theSocket->canWrite == TRUE);
}





//============================================================================
//      NTargetNetwork::SocketRead : Read from a socket.
//----------------------------------------------------------------------------
NIndex NTargetNetwork::SocketRead(NSocketRef theSocket, NIndex theSize, void *thePtr)
{	DWORD		numRead, theFlags;
	WSABUF		theBuffer;
	int			winErr;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != INVALID_SOCKET);
	NN_ASSERT(theSocket->canRead);



	// Get the state we need
	theBuffer.len = theSize;
	theBuffer.buf = (CHAR *) thePtr;

	numRead  = 0;
	theFlags = 0;



	// Read from the socket
	//
	// Reads are always blocking; once the socket has returned whatever data it
	// can, we use a zero-byte read to generate an event when more data arrives.
	winErr = SocketErr(WSARecv(theSocket->nativeSocket, &theBuffer, 1, &numRead, &theFlags, NULL, NULL));
	NN_ASSERT(winErr == ERROR_SUCCESS || winErr == WSAEWOULDBLOCK);

	InterlockedExchange(&theSocket->canRead, FALSE);
	SocketTriggerRead(theSocket);

	return(numRead);
}





//============================================================================
//      NTargetNetwork::SocketWrite : Write to a socket.
//----------------------------------------------------------------------------
NIndex NTargetNetwork::SocketWrite(NSocketRef theSocket, NIndex theSize, const void *thePtr)
{	NIndex		numWritten;
	WSABUF		theBuffer;
	int			winErr;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != INVALID_SOCKET);
	NN_ASSERT(theSocket->canWrite);



	// Get the state we need
	if (theSize > theSocket->bufferWrite.GetSize())
		theSocket->bufferWrite.SetSize(theSize);
	
	theBuffer.len = theSize;
	theBuffer.buf = (CHAR *) theSocket->bufferWrite.GetData();

	memcpy(theBuffer.buf, thePtr, theSize);



	// Write to the socket
	//
	// Writes are always non-blocking; the data to write is copied to a per-socket
	// buffer, allowing the socket to write out of that buffer on its own thread.
	//
	// The socket must be marked as non-writeable (using a memory barrier) before
	// the write is performed to ensure that no more writes will be made until the
	// current write has completed (and the write buffer can be re-used).
	InterlockedExchange(&theSocket->canWrite, FALSE);

	memset(&theSocket->eventWrite, 0x00, sizeof(theSocket->eventWrite));

	numWritten = 0;
	winErr     = SocketErr(WSASend(theSocket->nativeSocket, &theBuffer, 1, NULL, 0, &theSocket->eventWrite, NULL));
	
	switch (winErr) {
		case ERROR_SUCCESS:
		case WSA_IO_PENDING:
			numWritten = theSize;
			break;

		default:
			NN_LOG("Unable to write to socket: %ld", winErr);
			break;
		}
	
	return(numWritten);
}





//============================================================================
//      NTargetNetwork::SocketGetOption : Get a socket option.
//----------------------------------------------------------------------------
SInt32 NTargetNetwork::SocketGetOption(NSocketRef theSocket, NSocketOption theOption)
{	int			valueSize;
	BOOL		valueBool;
	SInt32		theValue;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != INVALID_SOCKET);



	// Get the state we need
	theValue = 0;



	// Get the option
	switch (theOption) {
		case kNSocketNoDelay:
			valueBool = false;
			valueSize = sizeof(valueBool);

			if (getsockopt(theSocket->nativeSocket, IPPROTO_TCP, TCP_NODELAY, (char *) &valueBool, &valueSize) == 0)
				theValue = (bool) (valueBool != FALSE);
			break;

		default:
			NN_LOG("Unknown option: %d", theOption);
			break;
		}
	
	return(theValue);
}





//============================================================================
//      NTargetNetwork::SocketSetOption : Set a socket option.
//----------------------------------------------------------------------------
NStatus NTargetNetwork::SocketSetOption(NSocketRef theSocket, NSocketOption theOption, SInt32 theValue)
{	BOOL		valueBool;
	NStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != INVALID_SOCKET);



	// Get the state we need
	theErr = kNErrNotSupported;



	// Set the option
	switch (theOption) {
		case kNSocketNoDelay:
			valueBool = (theValue != 0);

			if (setsockopt(theSocket->nativeSocket, IPPROTO_TCP, TCP_NODELAY, (const char *) &valueBool, sizeof(valueBool)) == 0)
				theErr = kNoErr;
			break;

		default:
			NN_LOG("Unknown option: %d", theOption);
			break;
		}
	
	return(theErr);
}


