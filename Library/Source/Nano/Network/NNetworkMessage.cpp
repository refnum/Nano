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
	NBYTESWAP_B_SInt16		(theType)
	NBYTESWAP_B_UInt16		(theAttributes)
	NBYTESWAP_B_UInt32		(bodySize)
NBYTESWAP_END





//============================================================================
//		NNetworkMessage::NNetworkMessage : Constructor.
//----------------------------------------------------------------------------
NNetworkMessage::NNetworkMessage(void)
{


	// Initialise ourselves
	mType = kNMessageInvalid;
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
	return(mType);
}





//============================================================================
//		NNetworkMessage::SetType : Set the type.
//----------------------------------------------------------------------------
void NNetworkMessage::SetType(NMessageType theType)
{


	// Set the type
	mType = theType;
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



	// Get the state we need
	theAttributes = GetAttributes();

	NN_ASSERT((theAttributes & kNMessageAttributeMask) == theAttributes);



	// Prepare the body
	theProperties = GetProperties();

	if (theProperties.GetSize() == 1 && theProperties.HasKey(kNMessageDataKey))
		theBody = theProperties.GetValueData(kNMessageDataKey);
	else
		{
		theBody        = propertyList.Encode(theProperties);
		theAttributes |= kNMessageHasProperties;
		}



	// Prepare the header
	theHeader.theType       = mType;
	theHeader.theAttributes = theAttributes;
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
	NN_ASSERT(theHeader.bodySize == (UInt32) theBody.GetSize());



	// Get the state we need
	if (theHeader.theAttributes & kNMessageHasProperties)
		theProperties = propertyList.Decode(theBody);
	else
		theProperties.SetValue(kNMessageDataKey, theBody);



	// Set the message
	SetType(      theHeader.theType);
	SetAttributes(theHeader.theAttributes);
	SetProperties(theProperties);
}




