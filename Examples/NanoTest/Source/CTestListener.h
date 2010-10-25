/*	NAME:
		CTestListener.h

	DESCRIPTION:
		Test listener.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CTESTLISTENER_HDR
#define CTESTLISTENER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NListener.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const NBroadcastMsg kTestMsgNone									= 0;
static const NBroadcastMsg kTestMsgSetUInt32								= 1;
static const NBroadcastMsg kTestMsgRemoveListener						= 2;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CTestListener : public NListener {
public:
										CTestListener(void);
	virtual							   ~CTestListener(void);


	// Set the target
	void								SetTargetUInt32(  UInt32    *theTarget);
	void								SetTargetListener(NListener *theTarget);


	// Handle messages
	void								DoMessage(NBroadcastMsg theMsg, const void *msgData);


private:
	UInt32							   *mTargetUInt32;
	NListener						   *mTargetListener;
};





#endif // CTESTLISTENER_HDR
