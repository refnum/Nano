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
// Message types
//
// Negative message types are reserved.
typedef SInt16 NMessageType;

static const NMessageType kNMessageInvalid							= -1;
static const NMessageType kNMessageServerInfo						= -2;
static const NMessageType kNMessageJoinRequest						= -3;
static const NMessageType kNMessageJoinResponse						= -4;


// Message attributes
//
// To conserve space, messages only support 16 bits of attribute data.
static const NBitfield kNMessageAttributeMask						= 0xFFFF;
static const NBitfield kNMessageHasProperties						= (1 << 0);


// Message keys
//
// Messages that contain a single data property will be transmitted in
// a more efficient form than those requiring an arbitrary dictionary.
static const NString kNMessageServerMaxClientsKey					= "MaxClients";
static const NString kNMessageServerNumClientsKey					= "NumClients";
static const NString kNMessageClientPasswordKey						= "Password";
static const NString kNMessageStatusKey								= "Status";
static const NString kNMessageDataKey								= "Data";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NNetworkMessage;


// Message header
typedef struct {
	NMessageType			theType;
	UInt16					theAttributes;
	UInt32					bodySize;
} NMessageHeader;

NBYTESWAP_DECLARE(NMessageHeader);


// Lists
typedef std::vector<NNetworkMessage>								NNetworkMessageList;
typedef NNetworkMessageList::iterator								NNetworkMessageListIterator;
typedef NNetworkMessageList::const_iterator							NNetworkMessageListConstIterator;





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


	// Get/set the payload
	NData								GetPayload(void) const;
	void								SetPayload(const NMessageHeader &theHeader, const NData &theBody);


private:
	NMessageType						mType;
};





#endif // NNETWORKMESSAGE_HDR


