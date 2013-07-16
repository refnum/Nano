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
#include "NTestFixture.h"
#include "NThread.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static void * const  kValueLocal									= (void *) 0x12345678;
static const NString kThreadName									= "Main Thread";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTHREAD(...)											TEST_NANO(TThread, ##__VA_ARGS__)

FIXTURE_NANO(TThread)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTHREAD("ID")
{	NThreadID			theID;



	// Perform the test
	theID = NThread::GetID();
	REQUIRE( NThread::AreEqual(theID, theID));
	REQUIRE(!NThread::AreEqual(theID, kNThreadIDNone));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTHREAD("Name")
{


	// Perform the test
	NThread::SetName(kThreadName);
	REQUIRE(NThread::GetName() == kThreadName);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTHREAD("TLS")
{	void				*theValue;
	NThreadLocalRef		localRef;



	// Perform the test
	localRef = NThread::CreateLocal();
	REQUIRE(localRef != kNThreadLocalRefNone);

	theValue = NThread::GetLocalValue(localRef);
	REQUIRE(theValue == (void *) NULL);

	NThread::SetLocalValue(localRef, kValueLocal);
	theValue = NThread::GetLocalValue(localRef);
	REQUIRE(theValue == kValueLocal);

	NThread::SetLocalValue(localRef, NULL);
	theValue = NThread::GetLocalValue(localRef);
	REQUIRE(theValue == (void *) NULL);
	
	NThread::DestroyLocal(localRef);
}


