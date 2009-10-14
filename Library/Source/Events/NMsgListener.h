/*	NAME:
		NMsgListener.h

	DESCRIPTION:
		Message listener.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMSGLISTENER_HDR
#define NMSGLISTENER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NMsgBroadcaster.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists
typedef std::vector<NMsgBroadcaster*>						NMsgBroadcasterList;
typedef NMsgBroadcasterList::iterator						NMsgBroadcasterListIterator;
typedef NMsgBroadcasterList::const_iterator					NMsgBroadcasterListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMsgListener {
friend class NMsgBroadcaster;
public:
										NMsgListener(void);
	virtual								~NMsgListener(void);


	// Get/set the listening state
	bool								IsListening(void);
	void								SetListening(bool isListening);


	// Are we listening to a broadcaster?
	bool								IsListeningTo(NMsgBroadcaster *theBroadcaster);
	

protected:
	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, void *msgData) = 0;


	// Add/remove a broadcaster
	//
	// Private methods invoked by NMsgBroadcaster
	void								AddBroadcaster(   NMsgBroadcaster *theBroadcaster);
	void								RemoveBroadcaster(NMsgBroadcaster *theBroadcaster);


private:
	void								RemoveListener(void);


private:
	bool								mIsListening;
	NMsgBroadcasterList					mBroadcasters;
};





#endif // NMSGLISTENER_HDR


