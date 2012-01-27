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


// Listeners
class NListener;

typedef std::map<NListener*, UInt8>									NListenerMap;
typedef NListenerMap::iterator										NListenerMapIterator;
typedef NListenerMap::const_iterator								NListenerMapConstIterator;


// State
typedef struct {
	bool				isDead;
	NListenerMap		theRecipients;
} NBroadcastState;


typedef std::vector<NBroadcastState *>								NBroadcastStateList;
typedef NBroadcastStateList::iterator								NBroadcastStateListIterator;
typedef NBroadcastStateList::const_iterator							NBroadcastStateListConstIterator;





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
	NBroadcastStateList					mBroadcasts;
	NListenerMap						mListeners;
};





#endif // NBROADCASTER_HDR


