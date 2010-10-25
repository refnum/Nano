/*	NAME:
		NBroadcaster.h

	DESCRIPTION:
		Message broadcaster.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NBROADCASTER_HDR
#define NBROADCASTER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Messages
typedef UInt32 NBroadcastMsg;


// Lists
class NListener;

typedef std::map<NListener*, UInt8>									NListenerMap;
typedef NListenerMap::iterator										NListenerMapIterator;
typedef NListenerMap::const_iterator								NListenerMapConstIterator;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const NBroadcastMsg kMsgNBroadcasterDestroyed				= 0xDEADDEAD;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBroadcaster {
public:
										NBroadcaster(const NBroadcaster &theBroadcaster);

										NBroadcaster(void);
	virtual							   ~NBroadcaster(void);


	// Get/set the broadcasting state
	bool								IsBroadcasting(void) const;
	void								SetBroadcasting(bool isBroadcasting);


	// Do we have any listeners?
	bool								HasListeners(void) const;
	

	// Add/remove a listener
	void								AddListener(   NListener *theListener);
	void								RemoveListener(NListener *theListener);


	// Remove all listeners
	void								RemoveListeners(void);
	

	// Broadcast a message
	void								BroadcastMessage(NBroadcastMsg theMsg, const void *msgData=NULL);


	// Operators
	const NBroadcaster					&operator = (const NBroadcaster &theBroadcaster);


private:
	void								CloneBroadcaster(const NBroadcaster &theBroadcaster);


private:
	bool								mIsBroadcasting;
	NListenerMap						mListeners;
	NListenerMap						mRecipients;

	bool								*mIsDead;
};





#endif // NBROADCASTER_HDR


