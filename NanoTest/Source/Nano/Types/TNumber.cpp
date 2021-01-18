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
#include "NAny.h"
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
NANO_TEST(TNumber, "Assignment")
{


	// Perform the test
	theNumber = true;
	theNumber = uint8_t(1);
	theNumber = uint16_t(1);
	theNumber = uint32_t(1);
	theNumber = uint64_t(1);
	theNumber = int8_t(-1);
	theNumber = int16_t(-1);
	theNumber = int32_t(-1);
	theNumber = int64_t(-1);
	theNumber = float32_t(1.0f);
	theNumber = float64_t(1.0);

	theNumber = 123;
	theNumber = 123L;
	theNumber = 123LL;
	theNumber = 123UL;
	theNumber = 123ULL;
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TNumber, "uint64_t")
{


	// Perform the test
	uint64_t theValue = 1;
	theNumber         = theValue;

	REQUIRE(theNumber.IsInteger());
	REQUIRE(!theNumber.IsReal());
	REQUIRE(!theNumber.IsSigned());
	REQUIRE(theNumber.IsPositive());
	REQUIRE(!theNumber.IsNegative());

	REQUIRE(theNumber.GetBool() == bool(theValue));
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
	theNumber        = theValue;

	REQUIRE(theNumber.IsInteger());
	REQUIRE(!theNumber.IsReal());
	REQUIRE(theNumber.IsSigned());
	REQUIRE(theNumber.IsPositive());
	REQUIRE(!theNumber.IsNegative());

	REQUIRE(theNumber.GetBool() == bool(theValue));
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


	theValue  = -theValue;
	theNumber = theValue;

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
	theNumber          = theValue;

	REQUIRE(!theNumber.IsInteger());
	REQUIRE(theNumber.IsReal());
	REQUIRE(theNumber.IsSigned());
	REQUIRE(theNumber.IsPositive());
	REQUIRE(!theNumber.IsNegative());

	REQUIRE(theNumber.GetFloat32() == float32_t(theValue));
	REQUIRE(theNumber.GetFloat64() == float64_t(theValue));


	theValue  = -theValue;
	theNumber = theValue;

	REQUIRE(!theNumber.IsInteger());
	REQUIRE(theNumber.IsReal());
	REQUIRE(theNumber.IsSigned());
	REQUIRE(!theNumber.IsPositive());
	REQUIRE(theNumber.IsNegative());

	REQUIRE(theNumber.GetFloat32() == float32_t(theValue));
	REQUIRE(theNumber.GetFloat64() == float64_t(theValue));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TNumber, "NAny")
{


	// Perform the test
	bool      valueBool    = true;
	uint8_t   valueUInt8   = 123;
	uint16_t  valueUInt16  = 123;
	uint32_t  valueUInt32  = 123;
	uint64_t  valueUInt64  = 123;
	int8_t    valueInt8    = -123;
	int16_t   valueInt16   = -123;
	int32_t   valueInt32   = -123;
	int64_t   valueInt64   = -123;
	float32_t valueFloat32 = 3.1415f;
	float64_t valueFloat64 = 3.1415;

	REQUIRE(theNumber.SetValue(NAny(valueBool)));
	REQUIRE(theNumber.GetBool() == valueBool);

	REQUIRE(theNumber.SetValue(NAny(valueUInt8)));
	REQUIRE(theNumber.GetUInt8() == valueUInt8);

	REQUIRE(theNumber.SetValue(NAny(valueUInt16)));
	REQUIRE(theNumber.GetUInt16() == valueUInt16);

	REQUIRE(theNumber.SetValue(NAny(valueUInt32)));
	REQUIRE(theNumber.GetUInt32() == valueUInt32);

	REQUIRE(theNumber.SetValue(NAny(valueUInt64)));
	REQUIRE(theNumber.GetUInt64() == valueUInt64);

	REQUIRE(theNumber.SetValue(NAny(valueInt8)));
	REQUIRE(theNumber.GetInt8() == valueInt8);

	REQUIRE(theNumber.SetValue(NAny(valueInt16)));
	REQUIRE(theNumber.GetInt16() == valueInt16);

	REQUIRE(theNumber.SetValue(NAny(valueInt32)));
	REQUIRE(theNumber.GetInt32() == valueInt32);

	REQUIRE(theNumber.SetValue(NAny(valueInt64)));
	REQUIRE(theNumber.GetInt64() == valueInt64);

	REQUIRE(theNumber.SetValue(NAny(valueFloat32)));
	REQUIRE(theNumber.GetFloat32() == valueFloat32);

	REQUIRE(theNumber.SetValue(NAny(valueFloat64)));
	REQUIRE(theNumber.GetFloat64() == valueFloat64);

	REQUIRE(theNumber.SetValue(NAny(123)));
	REQUIRE(theNumber.GetUInt32() == 123);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TNumber, "Comparison")
{


	// Perform the test
	//
	// Integer limits are offset by a significant amount to account
	// for real numbers being non-consecutive at that magnitude.
	NNumber numberA;
	NNumber numberB;


	numberA = float64_t(kNInt64Min) - 5000.0;
	numberB = kNInt64Min;
	REQUIRE(numberA < numberB);

	numberA = float64_t(kNInt64Min);
	numberB = kNInt64Min;
	REQUIRE(numberA == numberB);

	numberA = float64_t(kNInt64Min) + 5000.0;
	numberB = kNInt64Min;
	REQUIRE(numberA > numberB);

	numberA = kNIntegerSafeMin - 1.0;
	numberB = int64_t(kNIntegerSafeMin);
	REQUIRE(numberA < numberB);

	numberA = kNIntegerSafeMin;
	numberB = int64_t(kNIntegerSafeMin);
	REQUIRE(numberA == numberB);

	numberA = kNIntegerSafeMin + 1.0;
	numberB = int64_t(kNIntegerSafeMin);
	REQUIRE(numberA > numberB);


	numberA = float64_t(kNUInt64Max) - 5000.0;
	numberB = kNUInt64Max;
	REQUIRE(numberA < numberB);

	numberA = float64_t(kNUInt64Max);
	numberB = kNUInt64Max;
	REQUIRE(numberA == numberB);

	numberA = float64_t(kNUInt64Max) + 5000.0;
	numberB = kNUInt64Max;
	REQUIRE(numberA > numberB);

	numberA = kNIntegerSafeMax - 1.0;
	numberB = uint64_t(kNIntegerSafeMax);
	REQUIRE(numberA < numberB);

	numberA = kNIntegerSafeMax;
	numberB = uint64_t(kNIntegerSafeMax);
	REQUIRE(numberA == numberB);

	numberA = kNIntegerSafeMax + 1.0;
	numberB = uint64_t(kNIntegerSafeMax);
	REQUIRE(numberA > numberB);
}
