/*	NAME:
		TCache.cpp

	DESCRIPTION:
		NCache tests.

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
#include "NCache.h"
#include "NString.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TCache)
{
	NCache<NString, uint64_t> theCache;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "Default")
{


	// Perform the test
	REQUIRE(theCache.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "Clear")
{


	// Perform the test
	theCache.SetValue("one", 100);
	REQUIRE(!theCache.IsEmpty());

	theCache.Clear();
	REQUIRE(theCache.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "GetCost")
{


	// Perform the test
	theCache.SetValue("one", 100, 999);
	REQUIRE(theCache.GetCost() == 999);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "GetSize")
{


	// Perform the test
	theCache.SetValue("one", 100);
	REQUIRE(theCache.GetSize() == 1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "HasKey")
{


	// Perform the test
	theCache.SetValue("one", 100);
	REQUIRE(theCache.HasKey("one"));
	REQUIRE(!theCache.HasKey("two"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "RemoveKey")
{


	// Perform the test
	theCache.SetValue("one", 100);
	REQUIRE(theCache.HasKey("one"));

	theCache.RemoveKey("one");
	REQUIRE(!theCache.HasKey("one"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "GetValue")
{


	// Perform the test
	theCache.SetValue("one", 100);
	REQUIRE(*(theCache.GetValue("one")) == 100);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "SetValue")
{


	// Perform the test
	theCache.SetValue("one", std::make_shared<uint64_t>(100));
	theCache.SetValue("two", 200);

	REQUIRE(*(theCache.GetValue("one")) == 100);
	REQUIRE(*(theCache.GetValue("two")) == 200);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "GetMaxCost")
{


	// Perform the test
	REQUIRE(theCache.GetMaxCost() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "SetMaxCost")
{


	// Perform the test
	theCache.SetMaxCost(300);
	REQUIRE(theCache.GetMaxCost() == 300);

	theCache.SetValue("one", std::make_shared<uint64_t>(100), 100);
	REQUIRE(theCache.GetSize() == 1);

	theCache.SetValue("two", std::make_shared<uint64_t>(200), 200);
	REQUIRE(theCache.GetSize() == 2);

	theCache.SetValue("three", std::make_shared<uint64_t>(300), 300);
	REQUIRE(theCache.GetSize() == 2);
	REQUIRE(theCache.HasKey("one"));
	REQUIRE(theCache.HasKey("two"));
	REQUIRE(!theCache.HasKey("three"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "GetMaxSize")
{


	// Perform the test
	REQUIRE(theCache.GetMaxSize() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCache, "SetMaxSize")
{


	// Perform the test
	theCache.SetMaxSize(3);
	REQUIRE(theCache.GetMaxSize() == 3);

	theCache.SetValue("one", std::make_shared<uint64_t>(100));
	REQUIRE(theCache.GetSize() == 1);

	theCache.SetValue("two", std::make_shared<uint64_t>(200));
	REQUIRE(theCache.GetSize() == 2);

	theCache.SetValue("three", std::make_shared<uint64_t>(300));
	REQUIRE(theCache.GetSize() == 3);

	theCache.SetValue("four", std::make_shared<uint64_t>(400));
	REQUIRE(theCache.GetSize() == 3);
	REQUIRE(!theCache.HasKey("one"));
	REQUIRE(theCache.HasKey("two"));
	REQUIRE(theCache.HasKey("three"));
	REQUIRE(theCache.HasKey("four"));
}
