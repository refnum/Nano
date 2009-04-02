/*	NAME:
		NBroadcaster.h

	DESCRIPTION:
		Message broadcaster.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
typedef UInt32 BroadcastMsg;


// Lists
class NListener;

typedef std::map<NListener*, UInt8>									NListenerList;
typedef NListenerList::iterator										NListenerListIterator;
typedef NListenerList::const_iterator								NListenerListConstIterator;





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgBroadcasterDestroyed					= 'dead';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBroadcaster {
public:
										NBroadcaster(const NBroadcaster &theBroadcaster);

										NBroadcaster(void);
	virtual								~NBroadcaster(void);


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
	void								BroadcastMessage(BroadcastMsg theMsg, void *msgData=NULL);


	// Operators
	const NBroadcaster					&operator = (const NBroadcaster &theBroadcaster);


private:
	void								Clone(const NBroadcaster &theBroadcaster);


private:
	bool								mIsBroadcasting;
	NListenerList						mListeners;

	bool								*mIsDead;
};





#endif // NBROADCASTER_HDR


