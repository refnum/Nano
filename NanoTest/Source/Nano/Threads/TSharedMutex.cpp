/*	NAME:
		TSharedMutex.cpp

	DESCRIPTION:
		NSharedMutex tests.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
#include "NSharedMutex.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TSharedMutex)
{
	NSharedMutex theLock;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSharedMutex, "LockWrite")
{


	// Perform the test
	REQUIRE(theLock.Lock());
	theLock.Unlock();

	REQUIRE(theLock.Lock(0.25));
	theLock.Unlock();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSharedMutex, "LockRead")
{


	// Perform the test
	REQUIRE(theLock.LockShared());
	theLock.UnlockShared();

	REQUIRE(theLock.LockShared(0.25));
	theLock.UnlockShared();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TSharedMutex, "LockReadWrite")
{


	// Perform the test
	NUniqueThread threadRead = NThread::Create("TSharedMutexRead",
	[&]()
	{
		for (size_t n = 0; n < 100; n++)
		{
			REQUIRE(theLock.LockShared());
			NThread::Sleep(kNTimeMillisecond);
			theLock.UnlockShared();
		}

		for (size_t n = 0; n < 100; n++)
		{
			REQUIRE(theLock.LockShared(1.0));
			NThread::Sleep(kNTimeMillisecond);
			theLock.UnlockShared();
		}
	});

	NUniqueThread threadWrite = NThread::Create("TSharedMutexWrite",
	[&]()
	{
		for (size_t n = 0; n < 100; n++)
		{
			REQUIRE(theLock.Lock());
			NThread::Sleep(kNTimeMillisecond);
			theLock.Unlock();
		}

		for (size_t n = 0; n < 100; n++)
		{
			REQUIRE(theLock.Lock(1.0));
			NThread::Sleep(kNTimeMillisecond);
			theLock.Unlock();
		}
	});
}
