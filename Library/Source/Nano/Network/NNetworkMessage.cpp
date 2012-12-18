/*	NAME:
		NNetworkMessage.cpp

	DESCRIPTION:
		Network message.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkMessage.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
NBYTESWAP_BEGIN(NMessageHeader)
	NBYTESWAP_B_SInt16		(msgType)
	NBYTESWAP_B_UInt8		(msgSrcID)
	NBYTESWAP_B_UInt8		(msgDstID)
	NBYTESWAP_B_UInt8		(msgAttributes)
	NBYTESWAP_B_UInt8		(reserved1)
	NBYTESWAP_B_UInt8		(reserved2)
	NBYTESWAP_B_UInt8		(reserved3)
	NBYTESWAP_B_UInt32		(bodySize)
NBYTESWAP_END





//============================================================================
//		NNetworkMessage::NNetworkMessage : Constructor.
//----------------------------------------------------------------------------
NNetworkMessage::NNetworkMessage(void)
{


	// Initialise ourselves
	memset(&mHeader, 0x00, sizeof(mHeader));

	mHeader.msgType  = kNMessageInvalid;
	mHeader.msgSrcID = kNEntityInvalid;
	mHeader.msgDstID = kNEntityInvalid;
}





//============================================================================
//		NNetworkMessage::~NNetworkMessage : Destructor.
//----------------------------------------------------------------------------
NNetworkMessage::~NNetworkMessage(void)
{
}





//============================================================================
//		NNetworkMessage::GetType : Get the type.
//----------------------------------------------------------------------------
NMessageType NNetworkMessage::GetType(void) const
{


	// Get the type
	return(mHeader.msgType);
}





//============================================================================
//		NNetworkMessage::SetType : Set the type.
//----------------------------------------------------------------------------
void NNetworkMessage::SetType(NMessageType theType)
{


	// Set the type
	mHeader.msgType = theType;
}





//============================================================================
//		NNetworkMessage::GetSource : Get the source.
//----------------------------------------------------------------------------
NEntityID NNetworkMessage::GetSource(void) const
{


	// Get the source
	return(mHeader.msgSrcID);
}





//============================================================================
//		NNetworkMessage::SetSource : Set the source.
//----------------------------------------------------------------------------
void NNetworkMessage::SetSource(NEntityID theID)
{


	// Set the source
	mHeader.msgSrcID = theID;
}





//============================================================================
//		NNetworkMessage::GetDestination : Get the destination.
//----------------------------------------------------------------------------
NEntityID NNetworkMessage::GetDestination(void) const
{


	// Get the destination
	return(mHeader.msgDstID);
}





//============================================================================
//		NNetworkMessage::SetDestination : Set the destination.
//----------------------------------------------------------------------------
void NNetworkMessage::SetDestination(NEntityID theID)
{


	// Set the destination
	mHeader.msgDstID = theID;
}





//============================================================================
//		NNetworkMessage::GetPayload : Get the payload.
//----------------------------------------------------------------------------
NData NNetworkMessage::GetPayload(void) const
{	NData				theBody, theMsg;
	NDictionary			theProperties;
	NBitfield			theAttributes;
	NPropertyList		propertyList;
	NMessageHeader		theHeader;



	// Validate our state
	NN_ASSERT(mHeader.msgSrcID != kNEntityEveryone);
	NN_ASSERT(mHeader.msgDstID != kNEntityInvalid);
	NN_ASSERT(!HasAttribute(~kNMessageAttributeMask));



	// Get the state we need
	theAttributes = GetAttributes();



	// Prepare the body
	theProperties = GetProperties();

	if (theProperties.GetSize() == 1 && theProperties.HasKey(kNMessageDataKey))
		{
		theBody        = theProperties.GetValueData(kNMessageDataKey);
		theAttributes &= ~kNMessageHasProperties;
		}
	else
		{
		theBody        = propertyList.Encode(theProperties);
		theAttributes |=  kNMessageHasProperties;
		}



	// Prepare the header
	theHeader               = mHeader;
	theHeader.msgAttributes = (UInt8) (theAttributes & kNMessageAttributeMask);
	theHeader.bodySize      = theBody.GetSize();



	// Build the payload
	theMsg.Reserve(   sizeof(theHeader) + theHeader.bodySize);
	theMsg.AppendData(sizeof(theHeader), &theHeader);
	theMsg.AppendData(theBody);

	NBYTESWAP_ENCODE(1, NMessageHeader, theMsg.GetData());

	return(theMsg);
}





//============================================================================
//		NNetworkMessage::SetPayload : Set the payload.
//----------------------------------------------------------------------------
void NNetworkMessage::SetPayload(const NMessageHeader &theHeader, const NData &theBody)
{	NDictionary			theProperties;
	NPropertyList		propertyList;



	// Validate our parameters
	NN_ASSERT(theHeader.msgSrcID != kNEntityEveryone);
	NN_ASSERT(theHeader.msgDstID != kNEntityInvalid);
	NN_ASSERT(theHeader.bodySize == (UInt32) theBody.GetSize());



	// Get the state we need
	if (theHeader.msgAttributes & kNMessageHasProperties)
		theProperties = propertyList.Decode(theBody);
	else
		theProperties.SetValue(kNMessageDataKey, theBody);



	// Set the message
	mHeader = theHeader;

	SetAttributes(theHeader.msgAttributes);
	SetProperties(theProperties);
}




