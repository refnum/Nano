/*	NAME:
		NMsgBroadcaster.h

	DESCRIPTION:
		Message broadcaster.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMSGBROADCASTER_HDR
#define NMSGBROADCASTER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Messages
typedef UInt32 BroadcastMsg;


// Lists
class NMsgListener;

typedef std::vector<NMsgListener*>							NMsgListenerList;
typedef NMsgListenerList::iterator							NMsgListenerListIterator;
typedef NMsgListenerList::const_iterator					NMsgListenerListConstIterator;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgBroadcasterDestroyed					= 'dead';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMsgBroadcaster {
public:
										NMsgBroadcaster(void);
	virtual								~NMsgBroadcaster(void);


	// Get/set the broadcasting state
	bool								IsBroadcasting(void);
	void								SetBroadcasting(bool isBroadcasting);


	// Add/remove a listener
	void								AddListener(   NMsgListener *theListener);
	void								RemoveListener(NMsgListener *theListener);


	// Broadcast a message
	void								BroadcastMessage(BroadcastMsg theMsg, void *msgData=NULL);


private:
	void								RemoveBroadcaster(void);


private:
	bool								mIsBroadcasting;
	NMsgListenerList					mListeners;

	bool								*mIsDead;
};





#endif // NMSGBROADCASTER_HDR


