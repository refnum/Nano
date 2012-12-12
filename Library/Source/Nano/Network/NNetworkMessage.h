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
#include "NEncodable.h"
#include "NByteswap.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
typedef SInt16 NMessageType;
typedef UInt16 NMessageFlags;
typedef UInt8  NMessageField;

static const NMessageType  kNMessageInvalid							= 0;
static const NMessageType  kNMessagePrivateStart					= 1000;

static const NMessageFlags kNMessageNone							= 0;
static const NMessageFlags kNMessageHasProperties					= (1 << 0);





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NNetworkMessage;


// Message header
typedef struct {
	NMessageType			msgType;
	NMessageFlags			msgFlags;
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
class NNetworkMessage {
public:
										NNetworkMessage(const NMessageHeader &theHeader, const NData &theBody);

										NNetworkMessage(void);
	virtual							   ~NNetworkMessage(void);


	// Get/set the type
	NMessageType						GetType(void) const;
	void								SetType(NMessageType theType);


	// Get/set the flags
	NMessageFlags						GetFlags(void) const;
	void								SetFlags(NMessageFlags setThese, NMessageFlags clearThese=kNMessageNone);


	// Get the message
	NData								GetMessage(void) const;


	// Get/set the message body
	//
	// Messages can carry either raw data or a set of properties.
	NData								GetMessageData(      void) const;
	NDictionary							GetMessageProperties(void) const;

	void								SetMessageData(      const NData       &theData);
	void								SetMessageProperties(const NDictionary &theProperties);


private:
	NMessageHeader						mHeader;
	NData								mBody;
};





#endif // NNETWORKMESSAGE_HDR


