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


	// Initialise ourselves
	mID = kNEntityInvalid;
}





//============================================================================
//		NMessageEntity::~NMessageEntity : Destructor.
//----------------------------------------------------------------------------
NMessageEntity::~NMessageEntity(void)
{
}





//============================================================================
//		NMessageEntity::GetID : Get the ID.
//----------------------------------------------------------------------------
NEntityID NMessageEntity::GetID(void) const
{


	// Get the ID
	return(mID);
}





//============================================================================
//		NMessageEntity::SetID : Set the ID.
//----------------------------------------------------------------------------
void NMessageEntity::SetID(NEntityID theID)
{


	// Set the ID
	mID = theID;
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
//		NMessageEntity::PrepareMessage : Prepare a message.
//----------------------------------------------------------------------------
NNetworkMessage NMessageEntity::PrepareMessage(NMessageType theType, NEntityID dstID)
{	NNetworkMessage		theMsg;



	// Validate our parameters
	NN_ASSERT(theType != kNMessageInvalid);
	NN_ASSERT(dstID   != kNEntityInvalid);



	// Prepare the message
	theMsg.SetType(theType);
	theMsg.SetSource(     GetID());
	theMsg.SetDestination(dstID);

	return(theMsg);
}





//============================================================================
//		NMessageEntity::ProcessMessages : Process messages.
//----------------------------------------------------------------------------
void NMessageEntity::ProcessMessages(NSocket *theSocket)
{	NNetworkMessage		theMsg;
	NStatus				theErr;



	// Process messages
	do
		{
		theErr = ReadMessage(theSocket, theMsg);
		if (theErr == kNoErr)
			DispatchMessage(theMsg);
		}
	while (theErr == kNoErr);
}





//============================================================================
//		NMessageEntity::DispatchMessage : Dispatch a message.
//----------------------------------------------------------------------------
void NMessageEntity::DispatchMessage(const NNetworkMessage &theMsg)
{	NEntityID		dstID;



	// Validate our parameters and state
	NN_ASSERT(theMsg.GetSource()      != kNEntityEveryone);
	NN_ASSERT(theMsg.GetSource()      != kNEntityInvalid);
	NN_ASSERT(theMsg.GetDestination() != kNEntityInvalid);



	// Get the state we need
	dstID = theMsg.GetDestination();



	// Dispatch the message
	if (dstID == GetID() || dstID == kNEntityEveryone)
		ReceivedMessage(theMsg);
}







