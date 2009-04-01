/*	NAME:
		NMsgBroadcaster.h

	DESCRIPTION:
		Message broadcaster.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMSGBROADCASTER_HDR
#define NMSGBROADCASTER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Messages
typedef UInt32 BroadcastMsg;


// Lists
class NMsgListener;

typedef std::map<NMsgListener*, UInt8>						NMsgListenerList;
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
										NMsgBroadcaster(const NMsgBroadcaster &theBroadcaster);

										NMsgBroadcaster(void);
	virtual								~NMsgBroadcaster(void);


	// Get/set the broadcasting state
	bool								IsBroadcasting(void) const;
	void								SetBroadcasting(bool isBroadcasting);


	// Do we have any listeners?
	bool								HasListeners(void) const;
	

	// Add/remove a listener
	void								AddListener(   NMsgListener *theListener);
	void								RemoveListener(NMsgListener *theListener);


	// Remove all listeners
	void								RemoveListeners(void);
	

	// Broadcast a message
	void								BroadcastMessage(BroadcastMsg theMsg, void *msgData=NULL);


	// Operators
	const NMsgBroadcaster				&operator = (const NMsgBroadcaster &theBroadcaster);


private:
	void								Clone(const NMsgBroadcaster &theBroadcaster);


private:
	bool								mIsBroadcasting;
	NMsgListenerList					mListeners;

	bool								*mIsDead;
};





#endif // NMSGBROADCASTER_HDR


