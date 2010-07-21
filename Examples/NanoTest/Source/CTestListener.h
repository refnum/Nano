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
static const BroadcastMsg kTestMsgNone									= 0;
static const BroadcastMsg kTestMsgSetUInt32								= 123;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CTestListener : public NListener {
public:
										CTestListener(void);
	virtual							   ~CTestListener(void);


	// Set the target
	void								SetTarget(void *theValue);


	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, const void *msgData);


private:
	void							   *mTarget;
};





#endif // CTESTLISTENER_HDR
