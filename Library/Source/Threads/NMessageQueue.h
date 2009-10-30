/*	NAME:
		NMessageQueue.h

	DESCRIPTION:
		Message queue.
	
	COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMESSAGEQUEUE_HDR
#define NMESSAGEQUEUE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Thread messages
//
// Messages are arbitrary values, however some typical constants are provided
// for producer/consumer or batch operations.
typedef UInt32 NThreadMsg;

static const NThreadMsg kThreadMsgNone									= 0;
static const NThreadMsg kThreadMsgStart									= 1;
static const NThreadMsg kThreadMsgStop									= 2;
static const NThreadMsg kThreadMsgWork									= 3;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMessageQueue {
public:
										NMessageQueue(void);
	virtual								~NMessageQueue(void);


	// Post a message to a queue
	void								PostMessage(NThreadMsg theMsg);

	void								PostMessage(void *param1,
													void *param2 = NULL,
													void *param3 = NULL);


	// Wait for a message
	//
	// When waiting for a NThreadMsg message, kThreadMsgNone is returned on timeout.
	NThreadMsg							WaitForMessage(EventTime theTime = kEventDurationForever);

	bool								WaitForMessage(	void    **param1,
														void    **param2  = NULL,
														void    **param3  = NULL,
														EventTime theTime = kEventDurationForever);


private:
	MPQueueID							mQueue;
};



#endif // NMESSAGEQUEUE_HDR


