/*	NAME:
		TLockable.cpp

	DESCRIPTION:
		NLockable tests.

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
// Nano
#include "NLockable.h"
#include "NScopedLock.h"
#include "NString.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TLockable){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "Default")
{


	// Perform the test
	NLockable<NString> theObject;
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "Constructor")
{


	// Perform the test
	NLockable<NString> objectA("Testing");

	NLockable<NVectorString> objectB{"1", "2", "3"};

	NLockable<NVectorUInt32> objectC{1u, 2u, 3u};
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "IsLocked")
{


	// Perform the test
	NLockable<NString> theObject;
	REQUIRE(!theObject.IsLocked());

	theObject.Lock();
	REQUIRE(theObject.IsLocked());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "LockUnlock")
{


	// Perform the test
	NLockable<NString> theObject;
	REQUIRE(!theObject.IsLocked());

	theObject.Lock();
	REQUIRE(theObject.IsLocked());

	theObject.Unlock();
	REQUIRE(!theObject.IsLocked());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "GetValue")
{


	// Perform the test
	NLockable<NString> theObject;

	REQUIRE(theObject.GetValue().IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "SetValue")
{


	// Perform the test
	NLockable<NString> theObject("Testing");

	REQUIRE(theObject.GetValue() == "Testing");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "Get")
{


	// Perform the test
	NLockable<NString> theObject("Testing");
	NScopedLock        acquireLock(theObject);

	REQUIRE(theObject.Get() == "Testing");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "GetMutable")
{


	// Perform the test
	NLockable<NString> theObject;
	NScopedLock        acquireLock(theObject);

	theObject.GetMutable() = "Testing";
	REQUIRE(theObject.Get() == "Testing");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "Access")
{


	// Perform the test
	NLockable<NString> theObject("Testing");
	NScopedLock        acquireLock(theObject);

	REQUIRE((*theObject).EqualTo("TESTING"));
	REQUIRE(theObject->EqualTo("TESTING"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TLockable, "Scoped")
{


	// Perform the test
	NLockable<NString> theObject;
	REQUIRE(!theObject.IsLocked());

	if (true)
	{
		NScopedLock acquireLock(theObject);
		REQUIRE(theObject.IsLocked());
		REQUIRE(theObject.Get().IsEmpty());

		theObject.GetMutable() = "Hello";
		REQUIRE(theObject.Get() == "Hello");
	}

	REQUIRE(!theObject.IsLocked());
	REQUIRE(theObject.GetValue() == "Hello");
}



// forwarding constructor
