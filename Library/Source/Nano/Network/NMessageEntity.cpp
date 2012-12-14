/*	NAME:
		NMessageEntity.cpp

	DESCRIPTION:
		Message entity.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"
#include "NMessageEntity.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
NBYTESWAP_BEGIN(NMessageHandshake)
	NBYTESWAP_B_UInt32		(theMagic)
	NBYTESWAP_B_UInt32		(theVersion)
NBYTESWAP_END





//============================================================================
//		NMessageEntity::NMessageEntity : Constructor.
//----------------------------------------------------------------------------
NMessageEntity::NMessageEntity(void)
{
}





//============================================================================
//		NMessageEntity::~NMessageEntity : Destructor.
//----------------------------------------------------------------------------
NMessageEntity::~NMessageEntity(void)
{
}





//============================================================================
//		NMessageEntity::GetHandshake : Get a handshake.
//----------------------------------------------------------------------------
#pragma mark -
NMessageHandshake NMessageEntity::GetHandshake(void) const
{	NMessageHandshake		theHandshake;



	// Get the handshake
	theHandshake.theMagic   = kNMessageHandshakeMagic;
	theHandshake.theVersion = kNMessageHandshakeVersionCurrent;

	return(theHandshake);
}





//============================================================================
//		NMessageEntity::ReadHandshake : Read a handshake.
//----------------------------------------------------------------------------
NStatus NMessageEntity::ReadHandshake(NSocket *theSocket, NMessageHandshake &theHandshake)
{	NStatus		theErr;



	// Read the handshake
	theErr = theSocket->ReadData(sizeof(theHandshake), &theHandshake);
	NN_ASSERT_NOERR(theErr);

	if (theErr == kNoErr)
		NBYTESWAP_DECODE(1, NMessageHandshake, &theHandshake);
		
	return(theErr);
}





//============================================================================
//		NMessageEntity::WriteHandshake : Write a handshake.
//----------------------------------------------------------------------------
NStatus NMessageEntity::WriteHandshake(NSocket *theSocket, NMessageHandshake &theHandshake)
{	NMessageHandshake		wireHandshake;
	NStatus					theErr;



	// Get the state we need
	wireHandshake = theHandshake;
	NBYTESWAP_ENCODE(1, NMessageHandshake, &wireHandshake);



	// Write the handshake
	theErr = theSocket->WriteData(sizeof(wireHandshake), &wireHandshake);
	NN_ASSERT_NOERR(theErr);
		
	return(theErr);
}





//============================================================================
//		NMessageEntity::ReadMessage : Read a message.
//----------------------------------------------------------------------------
NStatus NMessageEntity::ReadMessage(NSocket *theSocket, NNetworkMessage &theMsg)
{	NMessageHeader		theHeader;
	NData				theBody;
	NStatus				theErr;



	// Read the header
	theErr = theSocket->ReadData(sizeof(theHeader), &theHeader);
	if (theErr == kNoErr)
		NBYTESWAP_DECODE(1, NMessageHeader, &theHeader);



	// Read the body
	if (theErr == kNoErr && theHeader.bodySize > 0)
		theErr = theSocket->ReadData(theHeader.bodySize, theBody);



	// Construct the message
	if (theErr == kNoErr)
		theMsg.SetPayload(theHeader, theBody);
	
	return(theErr);
}





//============================================================================
//		NMessageEntity::WriteMessage : Write a message.
//----------------------------------------------------------------------------
NStatus NMessageEntity::WriteMessage(NSocket *theSocket, const NNetworkMessage &theMsg, bool sendSync)
{	NData		theData;
	NStatus		theErr;



	// Get the state we need
	theData = theMsg.GetPayload();
	theErr  = kNoErr;



	// Write the message
	if (sendSync)
		theErr = theSocket->WriteData(theData);
	else
		theSocket->Write(theData.GetSize(), theData.GetData());

	return(theErr);
}





//============================================================================
//		NMessageEntity::ReceiveMessages : Receive messages.
//----------------------------------------------------------------------------
void NMessageEntity::ReceiveMessages(NSocket *theSocket)
{	NNetworkMessage		theMsg;
	NStatus				theErr;



	// Receive messages
	do
		{
		theErr = ReadMessage(theSocket, theMsg);
		if (theErr == kNoErr)
			ReceivedMessage(theMsg);
		}
	while (theErr == kNoErr);
}




