/*	NAME:
		TNumber.cpp

	DESCRIPTION:
		NNumber tests.

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
#include "NNumber.h"
#include "NTestFixture.h"





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TNumber)
{
	NNumber theNumber;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TNumber, "Default")
{


	// Perform the test
	REQUIRE(theNumber.IsInteger());
	REQUIRE(!theNumber.IsReal());
	REQUIRE(!theNumber.IsSigned());
	REQUIRE(theNumber.IsPositive());
	REQUIRE(!theNumber.IsNegative());

	REQUIRE(theNumber.GetUInt8() == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TNumber, "uint64_t")
{


	// Perform the test
	uint64_t theValue = 1;
	theNumber.SetUInt64(theValue);

	REQUIRE(theNumber.IsInteger());
	REQUIRE(!theNumber.IsReal());
	REQUIRE(!theNumber.IsSigned());
	REQUIRE(theNumber.IsPositive());
	REQUIRE(!theNumber.IsNegative());

	REQUIRE(theNumber.GetUInt8() == uint8_t(theValue));
	REQUIRE(theNumber.GetUInt16() == uint16_t(theValue));
	REQUIRE(theNumber.GetUInt32() == uint32_t(theValue));
	REQUIRE(theNumber.GetUInt64() == uint64_t(theValue));

	REQUIRE(theNumber.GetInt8() == int8_t(theValue));
	REQUIRE(theNumber.GetInt16() == int16_t(theValue));
	REQUIRE(theNumber.GetInt32() == int32_t(theValue));
	REQUIRE(theNumber.GetInt64() == int64_t(theValue));

	REQUIRE(theNumber.GetFloat32() == float32_t(theValue));
	REQUIRE(theNumber.GetFloat64() == float64_t(theValue));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TNumber, "int64_t")
{


	// Perform the test
	int64_t theValue = 1;
	theNumber.SetInt64(theValue);

	REQUIRE(theNumber.IsInteger());
	REQUIRE(!theNumber.IsReal());
	REQUIRE(theNumber.IsSigned());
	REQUIRE(theNumber.IsPositive());
	REQUIRE(!theNumber.IsNegative());

	REQUIRE(theNumber.GetUInt8() == uint8_t(theValue));
	REQUIRE(theNumber.GetUInt16() == uint16_t(theValue));
	REQUIRE(theNumber.GetUInt32() == uint32_t(theValue));
	REQUIRE(theNumber.GetUInt64() == uint64_t(theValue));

	REQUIRE(theNumber.GetInt8() == int8_t(theValue));
	REQUIRE(theNumber.GetInt16() == int16_t(theValue));
	REQUIRE(theNumber.GetInt32() == int32_t(theValue));
	REQUIRE(theNumber.GetInt64() == int64_t(theValue));

	REQUIRE(theNumber.GetFloat32() == float32_t(theValue));
	REQUIRE(theNumber.GetFloat64() == float64_t(theValue));


	theValue = -theValue;
	theNumber.SetInt64(theValue);

	REQUIRE(theNumber.IsInteger());
	REQUIRE(!theNumber.IsReal());
	REQUIRE(theNumber.IsSigned());
	REQUIRE(!theNumber.IsPositive());
	REQUIRE(theNumber.IsNegative());

	REQUIRE(theNumber.GetInt8() == int8_t(theValue));
	REQUIRE(theNumber.GetInt16() == int16_t(theValue));
	REQUIRE(theNumber.GetInt32() == int32_t(theValue));
	REQUIRE(theNumber.GetInt64() == int64_t(theValue));

	REQUIRE(theNumber.GetFloat32() == float32_t(theValue));
	REQUIRE(theNumber.GetFloat64() == float64_t(theValue));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TNumber, "float64_t")
{


	// Perform the test
	float64_t theValue = 1.5;
	theNumber.SetFloat64(theValue);

	REQUIRE(!theNumber.IsInteger());
	REQUIRE(theNumber.IsReal());
	REQUIRE(theNumber.IsSigned());
	REQUIRE(theNumber.IsPositive());
	REQUIRE(!theNumber.IsNegative());

	REQUIRE(theNumber.GetFloat32() == float32_t(theValue));
	REQUIRE(theNumber.GetFloat64() == float64_t(theValue));


	theValue = -theValue;
	theNumber.SetFloat64(theValue);

	REQUIRE(!theNumber.IsInteger());
	REQUIRE(theNumber.IsReal());
	REQUIRE(theNumber.IsSigned());
	REQUIRE(!theNumber.IsPositive());
	REQUIRE(theNumber.IsNegative());

	REQUIRE(theNumber.GetFloat32() == float32_t(theValue));
	REQUIRE(theNumber.GetFloat64() == float64_t(theValue));
}
