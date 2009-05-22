/*	NAME:
		CTestListener.h

	DESCRIPTION:
		Test listener.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
static const BroadcastMsg kTestMsgNone									= 0;
static const BroadcastMsg kTestMsgValue									= 123;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CTestListener : public NListener {
public:
										 CTestListener(void);
	virtual								~CTestListener(void);


	// Set the target
	void								SetTarget(BroadcastMsg *theTarget);


	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


private:
	BroadcastMsg						*mTarget;
};





#endif // CTESTLISTENER_HDR
