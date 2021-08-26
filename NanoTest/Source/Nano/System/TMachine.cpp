/*	NAME:
		TMachine.cpp

	DESCRIPTION:
		NMachine tests.

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
#include "NMachine.h"
#include "NString.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMachine){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMachine, "GetCores")
{


	// Perform the test
	size_t numLogical  = NMachine::GetCores();
	size_t numPhysical = NMachine::GetCores(NCoreType::Physical);

	REQUIRE(numLogical >= 1);
	REQUIRE(numPhysical >= 1);
	REQUIRE(numLogical >= numPhysical);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMachine, "GetMemory")
{


	// Perform the test
	float64_t sizeBytes = NMachine::GetMemory();
	REQUIRE(sizeBytes > 0.0);

	float64_t sizeGB = NMachine::GetMemory(kNGibibyte);
	REQUIRE(sizeGB > 0.0);
	REQUIRE(uint64_t(sizeGB) == uint64_t(sizeBytes / kNGibibyte));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMachine, "GetCPUSpeed")
{


	// Perform the test
	float64_t speedHertz = NMachine::GetCPUSpeed();
	REQUIRE(speedHertz > 0.0);

	float64_t speedGhz = NMachine::GetCPUSpeed(kNGigahertz);
	REQUIRE(speedGhz > 0.0);
	REQUIRE(uint64_t(speedGhz) == uint64_t(speedHertz / kNGigahertz));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMachine, "GetCPUName")
{


	// Perform the test
	NString theName = NMachine::GetCPUName();
	REQUIRE((!theName.IsEmpty() && theName != "Unknown"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMachine, "GetCPUVendor")
{


	// Perform the test
	NString theVendor = NMachine::GetCPUVendor();
	REQUIRE((!theVendor.IsEmpty() && theVendor != "Unknown"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMachine, "GetCPUArchitecture")
{


	// Perform the test
	NString theArch = NMachine::GetCPUArchitecture();
	REQUIRE((theArch == "arm64" || theArch == "arm" || theArch == "x86_64" || theArch == "x86"));
}
