/*	NAME:
		NListener.h

	DESCRIPTION:
		Message listener.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLISTENER_HDR
#define NLISTENER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBroadcaster.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NBroadcaster *kBroadcasterAny							= NULL;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Lists	
typedef std::map<NBroadcaster*, UInt8>								NBroadcasterList;
typedef NBroadcasterList::iterator									NBroadcasterListIterator;
typedef NBroadcasterList::const_iterator							NBroadcasterListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NListener {
friend class NBroadcaster;
public:
										NListener(const NListener &theListener);

										 NListener(void);
	virtual								~NListener(void);


	// Get/set the listening state
	bool								IsListening(void) const;
	void								SetListening(bool isListening);


	// Are we listening to a broadcaster/anyone?
	bool								IsListeningTo(const NBroadcaster *theBroadcaster=kBroadcasterAny) const;
	

	// Remove all broadcasters
	void								RemoveFromBroadcasters(void);


	// Operators
	const NListener						&operator = (const NListener &theListener);


protected:
	// Handle messages
	virtual void						DoMessage(BroadcastMsg theMsg, const void *msgData) = 0;


	// Add/remove a broadcaster
	//
	// Private methods invoked by NBroadcaster
	void								AddBroadcaster(   NBroadcaster *theBroadcaster);
	void								RemoveBroadcaster(NBroadcaster *theBroadcaster);


private:
	void								Clone(const NListener &theListener);


private:
	bool								mIsListening;
	NBroadcasterList					mBroadcasters;
};





#endif // NLISTENER_HDR


