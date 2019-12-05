/*	NAME:
		TThread.cpp

	DESCRIPTION:
		NThread tests.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NThread.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Nano 3.x
#if 0
static void * const kValueLocal                             = (void *) 0x12345678;
static const NString kThreadName                            = "Main Thread";
#endif





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TThread){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TThread, "Yield")
{


	// Perform the test
	NThread::Yield();
	NThread::Pause();
}



// Nano 3.x
#if 0





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
TEST_NTHREAD("ID")
{
  NThreadID theID;



  // Perform the test
  theID = NThread::GetID();
  REQUIRE( NThread::AreEqual(theID, theID));
  REQUIRE(!NThread::AreEqual(theID, kNThreadIDNone));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
TEST_NTHREAD("Name")
{


  // Perform the test
  NThread::SetName(kThreadName);
  REQUIRE(NThread::GetName() == kThreadName);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
TEST_NTHREAD("TLS")
{
  void                *theValue;
  NThreadLocalRef localRef;



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


#endif
