/*	NAME:
		NMessageEntity.h

	DESCRIPTION:
		Message entity.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMESSAGEENTITY_HDR
#define NMESSAGEENTITY_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSocket.h"
#include "NNetworkMessage.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Handshake
static const UInt32 kNMessageHandshakeMagic							= 0x4d655376;	// 'MeSv'
static const UInt32 kNMessageHandshakeVersionInvalid				= kUInt32Max;
static const UInt32 kNMessageHandshakeVersionCurrent				= 0;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Handshake
typedef struct {
	UInt32		theMagic;
	UInt32		theVersion;
} NMessageHandshake;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMessageEntity {
public:
										NMessageEntity(void);
	virtual							   ~NMessageEntity(void);


	// Get/set the ID
	NEntityID							GetID(void) const;
	void								SetID(NEntityID theID);


protected:
	// Get a handshake
	NMessageHandshake					GetHandshake(void) const;


	// Read/write a handshake
	NStatus								ReadHandshake(  NSocket *theSocket, NMessageHandshake &theHandshake);
	NStatus								WriteHandshake( NSocket *theSocket, NMessageHandshake &theHandshake);


	// Read/write a message
	NStatus								ReadMessage(    NSocket *theSocket,       NNetworkMessage &theMsg);
	NStatus								WriteMessage(   NSocket *theSocket, const NNetworkMessage &theMsg, bool sendSync=true);


	// Prepare a message
	NNetworkMessage						PrepareMessage(NMessageType theType, NEntityID dstID);


	// Process messages
	void								ProcessMessages(NSocket *theSocket);


	// Handle a message
	virtual void						DispatchMessage(const NNetworkMessage &theMsg);
	virtual void						ReceivedMessage(const NNetworkMessage &theMsg) = 0;


private:
	NEntityID							mID;
};




#endif // NMESSAGEENTITY_HDR






