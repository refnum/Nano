/*	NAME:
		NNetworkMessage.h

	DESCRIPTION:
		Network message.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NNETWORKMESSAGE_HDR
#define NNETWORKMESSAGE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPropertyStore.h"
#include "NAttributes.h"
#include "NEncodable.h"
#include "NByteswap.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Entity IDs
//
// Messages are exchanged between entities, which have a fixed ID.
typedef UInt8 NEntityID;

static const NEntityID kNEntityInvalid								= 0;
static const NEntityID kNEntityServer								= 1;
static const NEntityID kNEntityEveryone								= 255;

static const NEntityID kNEntityClientsFirst							= 2;
static const NEntityID kNEntityClientsLast							= 254;
static const NEntityID kNEntityClientsMax							= kNEntityClientsLast - kNEntityClientsFirst + 1;


// Messages
//
// Negative message types are reserved.
typedef SInt16 NMessageType;

static const NMessageType kNMessageInvalidMsg						= -1;
static const NMessageType kNMessageServerInfoMsg					= -2;
static const NMessageType kNMessageConnectRequestMsg				= -3;
static const NMessageType kNMessageConnectResponseMsg				= -4;
static const NMessageType kNMessageClientDisconnectedMsg			= -5;


// Message attributes
//
// Only the lower 8 bits of attribute data are put onto the network.
//
// Higher bits may be used locally, but will be discarded before sending.
static const NBitfield kNMessageAttributeMask						= 0x000000FF;
static const NBitfield kNMessageHasProperties						= (1 << 0);			// On wire
static const NBitfield kNMessageIsCompressed						= (1 << 1);			// On wire
static const NBitfield kNMessageNeverCompress						= (1 << 8);			// Local


// Message keys
//
// Messages that contain a single kNMessageDataKey property will be transmitted
// in a more efficient form than those requiring an arbitrary dictionary.
static const NString kNMessageMaxClientsKey							= "MaxClients";		// SInt32, NIndex
static const NString kNMessageNumClientsKey							= "NumClients";		// SInt32, NIndex
static const NString kNMessagePasswordKey							= "Password";		// NString
static const NString kNMessageStatusKey								= "Status";			// SInt32, NStatus
static const NString kNMessageDataKey								= "Data";			// NData





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Message header
#pragma pack(push, 1)

typedef struct {
	NMessageType			msgType;
	NEntityID				msgSrcID;
	NEntityID				msgDstID;
	UInt8					msgAttributes;
	UInt8					reserved1;
	UInt8					reserved2;
	UInt8					reserved3;
	UInt32					bodySize;
} NMessageHeader;

#pragma pack(pop)

NBYTESWAP_DECLARE(NMessageHeader);





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkMessage : public NPropertyStore,
						public NAttributes {
public:
										NNetworkMessage(void);
	virtual							   ~NNetworkMessage(void);


	// Get/set the type
	NMessageType						GetType(void) const;
	void								SetType(NMessageType theType);


	// Get/set the source
	NEntityID							GetSource(void) const;
	void								SetSource(NEntityID theID);


	// Get/set the destination
	NEntityID							GetDestination(void) const;
	void								SetDestination(NEntityID theID);


	// Get/set the payload
	NData								GetPayload(void) const;
	void								SetPayload(const NMessageHeader &theHeader, const NData &theBody);


private:
	bool								CompressBody(        NData &theBody) const;
	NData								DecompressBody(const NData &theBody) const;


private:
	NMessageHeader						mHeader;
};





#endif // NNETWORKMESSAGE_HDR


