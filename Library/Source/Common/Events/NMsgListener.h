/*	NAME:
		NMsgListener.h

	DESCRIPTION:
		Message listener.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMSGLISTENER_HDR
#define NMSGLISTENER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NMsgBroadcaster *kBroadcasterAny					= NULL;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists
typedef std::map<NMsgBroadcaster*, UInt8>					NMsgBroadcasterList;
typedef NMsgBroadcasterList::iterator						NMsgBroadcasterListIterator;
typedef NMsgBroadcasterList::const_iterator					NMsgBroadcasterListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMsgListener {
friend class NMsgBroadcaster;
public:
										NMsgListener(const NMsgListener &theListener);

										 NMsgListener(void);
	virtual								~NMsgListener(void);


	// Get/set the listening state
	bool								IsListening(void) const;
	void								SetListening(bool isListening);


	// Are we listening to a broadcaster/anyone?
	bool								IsListeningTo(const NMsgBroadcaster *theBroadcaster=kBroadcasterAny) const;
	

	// Remove all broadcasters
	void								RemoveFromBroadcasters(void);


	// Operators
	const NMsgListener					&operator = (const NMsgListener &theListener);


protected:
	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, void *msgData) = 0;


	// Add/remove a broadcaster
	//
	// Private methods invoked by NMsgBroadcaster
	void								AddBroadcaster(   NMsgBroadcaster *theBroadcaster);
	void								RemoveBroadcaster(NMsgBroadcaster *theBroadcaster);


private:
	void								Clone(const NMsgListener &theListener);


private:
	bool								mIsListening;
	NMsgBroadcasterList					mBroadcasters;
};





#endif // NMSGLISTENER_HDR


