/*	NAME:
		NUserNotification.h

	DESCRIPTION:
		User notification.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUSERNOTIFICATION_HDR
#define NUSERNOTIFICATION_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NWindowController.h"
#include "NTimer.h"
#include "NString.h"
#include "NIcon.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Notification message
typedef struct {
	NString		theMsg;
	NIcon		theIcon;
	EventTime	timeExpire;
} NotificationMsg;


// Lists
typedef std::vector<NotificationMsg>						NotificationMsgList;
typedef NotificationMsgList::iterator						NotificationMsgListIterator;
typedef NotificationMsgList::const_iterator					NotificationMsgListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUserNotification : public NWindowController {
public:
										DECLARE_WINDOW_CONTROLLER(NUserNotification);

										NUserNotification( void);
	virtual								~NUserNotification(void);


	// Post a message
	void								PostMessage(const NString &theText);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


private:
	void								UpdateMessages(bool didChange);
	void								UpdateWindow(void);

	bool								PurgeMessages(void);
	void								ApplyMessage(NHIView *theView, const NotificationMsg &theMsg);

	void								ShowWindow(void);
	void								HideWindow(void);
	

private:
	NTimer								mTimer;
	NotificationMsgList					mMessages;
};





#endif // NUSERNOTIFICATION_HDR
