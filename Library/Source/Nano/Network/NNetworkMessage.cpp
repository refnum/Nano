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
	NBYTESWAP_L_SInt16		(msgType)
	NBYTESWAP_L_UInt16		(msgFlags)
	NBYTESWAP_L_UInt32		(bodySize)
NBYTESWAP_END





//============================================================================
//		NNetworkMessage::NNetworkMessage : Constructor.
//----------------------------------------------------------------------------
NNetworkMessage::NNetworkMessage(const NMessageHeader &theHeader, const NData &theBody)
{


	// Validate our parameters
	NN_ASSERT(theHeader.bodySize == (UInt32) theBody.GetSize());



	// Initialise ourselves
	mHeader = theHeader;
	mBody   = theBody;
}





//============================================================================
//		NNetworkMessage::NNetworkMessage : Constructor.
//----------------------------------------------------------------------------
NNetworkMessage::NNetworkMessage(void)
{


	// Initialise ourselves
	memset(&mHeader, 0x00, sizeof(mHeader));
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
//		NNetworkMessage::GetFlags : Get the flags.
//----------------------------------------------------------------------------
NMessageFlags NNetworkMessage::GetFlags(void) const
{


	// Get the flags
	return(mHeader.msgFlags);
}





//============================================================================
//		NNetworkMessage::SetFlags : Set the flags.
//----------------------------------------------------------------------------
void NNetworkMessage::SetFlags(NMessageFlags setThese, NMessageFlags clearThese)
{


	// Set the flags
	mHeader.msgFlags |=    setThese;
	mHeader.msgFlags &= ~clearThese;
}





//============================================================================
//		NNetworkMessage::GetMessage : Get the message.
//----------------------------------------------------------------------------
NData NNetworkMessage::GetMessage(void) const
{	NMessageHeader		theHeader;
	NData				theMsg;



	// Get the state we need
	theHeader = mHeader;

	NBYTESWAP_ENCODE(1, NMessageHeader, &theHeader);



	// Build the message
	theMsg.Reserve(sizeof(theHeader) + mBody.GetSize());

	theMsg.AppendData(sizeof(mHeader), &mHeader);
	theMsg.AppendData(mBody);
	
	return(theMsg);
}





//============================================================================
//		NNetworkMessage::GetMessageData : Get the message data.
//----------------------------------------------------------------------------
NData NNetworkMessage::GetMessageData(void) const
{


	// Get the data
	return(mBody);
}





//============================================================================
//		NNetworkMessage::GetMessageProperties : Get the message properties.
//----------------------------------------------------------------------------
NDictionary NNetworkMessage::GetMessageProperties(void) const
{	NPropertyList	propertyList;



	// Get the properties
	return(propertyList.Decode(mBody));
}





//============================================================================
//		NNetworkMessage::SetMessageData : Set the message data.
//----------------------------------------------------------------------------
void NNetworkMessage::SetMessageData(const NData &theData)
{


	// Set the data
	mBody = theData;

	SetFlags(kNMessageNone, kNMessageHasProperties);
}





//============================================================================
//		NNetworkMessage::SetMessageProperties : Set the message properties.
//----------------------------------------------------------------------------
void NNetworkMessage::SetMessageProperties(const NDictionary &theProperties)
{	NPropertyList	propertyList;



	// Set the properties
	mBody = propertyList.Encode(theProperties);

	SetFlags(kNMessageHasProperties);
}




