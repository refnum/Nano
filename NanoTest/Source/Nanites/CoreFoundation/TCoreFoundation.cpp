/*	NAME:
		TCoreFoundation.cpp

	DESCRIPTION:
		NCoreFoundation tests.

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
#include "NCoreFoundation.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TCoreFoundation){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NAny")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NArray")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NData")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NDate")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NDictionary")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NNumber")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NRange")
{


	// Perform the test
	REQUIRE(ToCF(NRange(0, 0)) == CFRange{0, 0});
	REQUIRE(ToCF(NRange(0, 1)) == CFRange{0, 1});
	REQUIRE(ToCF(NRange(1, 0)) == CFRange{1, 0});
	REQUIRE(ToCF(NRange(1, 1)) == CFRange{1, 1});
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NString")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToCF/NURL")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFTypeRef")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFArrayRef")
{


	// Perform the test
	REQUIRE(ToNN(CFArrayRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFMutableArrayRef")
{


	// Perform the test
	REQUIRE(ToNN(CFMutableArrayRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFDataRef")
{


	// Perform the test
	REQUIRE(ToNN(CFDataRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFMutableDataRef")
{


	// Perform the test
	REQUIRE(ToNN(CFMutableDataRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFDateRef")
{


	// Perform the test
	REQUIRE(ToNN(CFDateRef(nullptr)) == 0.0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFDictionaryRef")
{


	// Perform the test
	REQUIRE(ToNN(CFDictionaryRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFMutableDictionaryRef")
{


	// Perform the test
	REQUIRE(ToNN(CFMutableDictionaryRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFNumberRef")
{


	// Perform the test
	REQUIRE(ToNN(CFNumberRef(nullptr)).GetUInt8() == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFRange")
{


	// Perform the test
	REQUIRE(ToNN(CFRange{0, 0}) == NRange(0, 0));
	REQUIRE(ToNN(CFRange{0, 1}) == NRange(0, 1));
	REQUIRE(ToNN(CFRange{1, 0}) == NRange(1, 0));
	REQUIRE(ToNN(CFRange{1, 1}) == NRange(1, 1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFStringRef")
{


	// Perform the test
	REQUIRE(ToNN(CFStringRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFMutableStringRef")
{


	// Perform the test
	REQUIRE(ToNN(CFMutableStringRef(nullptr)).IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "ToNN/CFURLRef")
{


	// Perform the test
	REQUIRE(!ToNN(CFURLRef(nullptr)).IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "Equality")
{


	// Perform the test
	CFRange rangeA{10, 20};
	CFRange rangeB{20, 30};

	REQUIRE(rangeA == rangeA);
	REQUIRE(rangeB == rangeB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCoreFoundation, "Inqquality")
{


	// Perform the test
	CFRange rangeA{10, 20};
	CFRange rangeB{20, 30};

	REQUIRE(rangeA != rangeB);
	REQUIRE(rangeB != rangeA);
}
