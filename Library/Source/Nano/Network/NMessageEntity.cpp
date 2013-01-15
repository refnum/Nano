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
//		Build constants
//----------------------------------------------------------------------------
#define DEBUG_MESSAGES												0





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
//		NMessageEntity::CreateMessage : Create a message.
//----------------------------------------------------------------------------
NNetworkMessage NMessageEntity::CreateMessage(NMessageType theType, NEntityID dstID)
{	NNetworkMessage		theMsg;



	// Validate our parameters
	NN_ASSERT(theType != kNMessageInvalidMsg);
	NN_ASSERT(dstID   != kNEntityInvalid);



	// Create the message
	theMsg.SetType(theType);
	theMsg.SetSource(     GetID());
	theMsg.SetDestination(dstID);

	return(theMsg);
}





//============================================================================
//		NMessageEntity::CreateHandshake : Create a handshake.
//----------------------------------------------------------------------------
#pragma mark -
NMessageHandshake NMessageEntity::CreateHandshake(void) const
{	NMessageHandshake		theHandshake;



	// Create the handshake
	theHandshake.theMagic   = kNMessageHandshakeMagic;
	theHandshake.theVersion = kNMessageHandshakeVersionCurrent;

	return(theHandshake);
}





//============================================================================
//		NMessageEntity::ValidateHandshake : Validate a handshake.
//----------------------------------------------------------------------------
NStatus NMessageEntity::ValidateHandshake(const NMessageHandshake &theHandshake)
{	NStatus		theErr;



	// Validate the handshake
	if (theHandshake.theMagic != kNMessageHandshakeMagic)
		theErr = kNErrMalformed;

	else if (theHandshake.theVersion > kNMessageHandshakeVersionCurrent)
		theErr = kNErrVersionTooNew;

	else if (theHandshake.theVersion < kNMessageHandshakeVersionCurrent)
		theErr = kNErrVersionTooOld;

	else
		theErr = kNoErr;
	
	return(theErr);
}





//============================================================================
//		NMessageEntity::EncryptPassword : Encrypt a password.
//----------------------------------------------------------------------------
NString NMessageEntity::EncryptPassword(const NString &thePassword)
{	NDataDigest		dataDigest;
	NString			theResult;
	NDigestSHA1		theHash;



	// Encrypt the password
	//
	// Passwords are not salted, since the password is shared by all clients.
	theHash   = dataDigest.GetSHA1(thePassword.GetData());
	theResult = dataDigest.GetString(theHash);

	return(theResult);
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
NStatus NMessageEntity::WriteHandshake(NSocket *theSocket, const NMessageHandshake &theHandshake)
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



	// Debug hook
#if DEBUG_MESSAGES
	DumpMessage(theErr, true, &theHeader);
#endif

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



	// Debug hook
#if DEBUG_MESSAGES
	DumpMessage(theErr, false, (const NMessageHeader *) theData.GetData());
#endif

	return(theErr);
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
			ProcessMessage(theMsg);
		}
	while (theErr == kNoErr);
}





//============================================================================
//		NMessageEntity::DumpMessage : Dump a message.
//----------------------------------------------------------------------------
void NMessageEntity::DumpMessage(NStatus theErr, bool isRead, const NMessageHeader *rawHeader)
{	NMessageHeader		theHeader;



	// Get the state we need
	//
	// Read data has been decoded, but gets cleared if the read failed.
	//
	// Written data has been encoded, so must be decoded back.
	memcpy(&theHeader, rawHeader, sizeof(theHeader));

	if (isRead)
		{
		if (theErr != kNoErr)
			memset(&theHeader, 0x00, sizeof(theHeader));
		}
	else
		NBYTESWAP_DECODE(1, NMessageHeader, &theHeader);



	// Dump the message
	NN_LOG("[%02d] %s msg: err=%-3d type=%-3d src=%-2d dst=%-4d attr=0x%02X  size=%d",
				mID,
				isRead ? "read " : "wrote",
				theErr,
				theHeader.msgType,
				theHeader.msgSrcID,
				theHeader.msgDstID,
				theHeader.msgAttributes,
				theHeader.bodySize);
}





