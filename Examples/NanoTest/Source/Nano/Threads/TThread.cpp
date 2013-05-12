/*	NAME:
		TThread.cpp

	DESCRIPTION:
		NThread tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThread.h"

#include "TThread.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static void * const kValueLocal										= (void *) 0x12345678;





//============================================================================
//		TThread::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TThread::Execute(void)
{	void				*theValue;
	NThreadLocalRef		localRef;



	// Thread-local storage
	localRef = NThread::CreateLocal();
	NN_ASSERT(localRef != kNThreadLocalRefNone);

	theValue = NThread::GetLocalValue(localRef);
	NN_ASSERT(theValue == NULL);

	NThread::SetLocalValue(localRef, kValueLocal);
	theValue = NThread::GetLocalValue(localRef);
	NN_ASSERT(theValue == kValueLocal);

	NThread::SetLocalValue(localRef, NULL);
	theValue = NThread::GetLocalValue(localRef);
	NN_ASSERT(theValue == NULL);
	
	NThread::DestroyLocal(localRef);
}

