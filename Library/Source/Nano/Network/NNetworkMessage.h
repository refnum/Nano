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
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef UInt32 NetworkMessageType;

static const NetworkMessageType kNetworkMessageNone					= 0;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NNetworkMessage;


// Lists
typedef std::vector<NNetworkMessage>								NNetworkMessageList;
typedef NNetworkMessageList::iterator								NNetworkMessageListIterator;
typedef NNetworkMessageList::const_iterator							NNetworkMessageListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NNetworkMessage {
public:
										NNetworkMessage(NetworkMessageType theType, const NData &theData);

										NNetworkMessage(void);
	virtual							   ~NNetworkMessage(void);


	// Is a message of a type?
	bool								IsType(NetworkMessageType theType) const;


	// Get/set the type
	NetworkMessageType					GetType(void) const;
	void								SetType(NetworkMessageType theType);


	// Get/set the data
	NData								GetData(void) const;
	void								SetData(const NData &theData);


private:
	NetworkMessageType					mType;
	NData								mData;
};





#endif // NNETWORKMESSAGE_HDR


