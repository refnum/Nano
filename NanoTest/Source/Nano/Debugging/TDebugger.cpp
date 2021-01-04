/*	NAME:
		TDebugger.cpp

	DESCRIPTION:
		NDebugger tests.

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
#include "NDebugger.h"
#include "NTestFixture.h"





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDebugger){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDebugger, "IsActive")
{


	// Perform the test
	bool isActive = NDebugger::IsActive();
	NN_UNUSED(isActive);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDebugger, "GetBacktrace")
{


	// Perform the test
	NVectorString trace1 = NDebugger::GetBacktrace();
	NVectorString trace2 = NDebugger::GetBacktrace(2);
	NVectorString trace3 = NDebugger::GetBacktrace(4, 40);

	REQUIRE(trace1.size() >= 5);
	REQUIRE(trace2.size() == (trace1.size() - 2));
	REQUIRE(trace3.size() == (trace1.size() - 4));

	REQUIRE(trace2[0] == trace1[2]);
	REQUIRE(trace3[0] == trace1[4]);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDebugger, "GetBacktraceID")
{


	// Perform the test
	NString theID = NDebugger::GetBacktraceID();
	REQUIRE(!theID.IsEmpty());
}
