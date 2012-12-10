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
//		NNetworkMessage::NNetworkMessage : Constructor.
//----------------------------------------------------------------------------
NNetworkMessage::NNetworkMessage(NetworkMessageType theType, const NData &theData)
{


	// Initialise ourselves
	mType = theType;
	mData = theData;
}





//============================================================================
//		NNetworkMessage::NNetworkMessage : Constructor.
//----------------------------------------------------------------------------
NNetworkMessage::NNetworkMessage(void)
{


	// Initialise ourselves
	mType = kNetworkMessageNone;
}





//============================================================================
//		NNetworkMessage::~NNetworkMessage : Destructor.
//----------------------------------------------------------------------------
NNetworkMessage::~NNetworkMessage(void)
{
}





//============================================================================
//		NNetworkMessage::IsType : Is the message of a type?
//----------------------------------------------------------------------------
bool NNetworkMessage::IsType(NetworkMessageType theType) const
{


	// Check our state
	return(mType == theType);
}





//============================================================================
//		NNetworkMessage::GetType : Get the type.
//----------------------------------------------------------------------------
NetworkMessageType NNetworkMessage::GetType(void) const
{


	// Get the type
	return(mType);
}





//============================================================================
//		NNetworkMessage::SetType : Set the type.
//----------------------------------------------------------------------------
void NNetworkMessage::SetType(NetworkMessageType theType)
{


	// Set the type
	mType = theType;
}





//============================================================================
//		NNetworkMessage::GetData : Get the data.
//----------------------------------------------------------------------------
NData NNetworkMessage::GetData(void) const
{


	// Get the data
	return(mData);
}





//============================================================================
//		NNetworkMessage::SetData : Set the data.
//----------------------------------------------------------------------------
void NNetworkMessage::SetData(const NData &theData)
{


	// Set the data
	mData = theData;
}





