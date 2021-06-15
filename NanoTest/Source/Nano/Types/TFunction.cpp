/*	NAME:
		TFunction.cpp

	DESCRIPTION:
		NFunction tests.

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
#include "NFunction.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal class declaration
//-----------------------------------------------------------------------------
class TestClass
{
public:
	void TestMethod(size_t /*a*/, size_t /*b*/)
	{
		auto result = NBindSelf(TestClass::TestMethod, 1, 2);
		REQUIRE(std::is_bind_expression<decltype(result)>::value);
	}
};





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
static void TestFunction(size_t /*a*/, size_t /*b*/)
{
}





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TFunction){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFunction, "NBind")
{


	// Perform the test
	auto result = NBind(TestFunction, 1, 2);
	REQUIRE(std::is_bind_expression<decltype(result)>::value);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFunction, "NBindMethod")
{


	// Perform the test
	TestClass theObject;

	auto result = NBindMethod(&theObject, TestClass::TestMethod, 1, 2);
	REQUIRE(std::is_bind_expression<decltype(result)>::value);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TFunction, "NBindSelf")
{


	// Perform the test
	TestClass theObject;

	theObject.TestMethod(1, 2);
}
