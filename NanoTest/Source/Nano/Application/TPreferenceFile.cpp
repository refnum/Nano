/*	NAME:
		TPreferenceFile.cpp

	DESCRIPTION:
		NPreferenceFile tests.

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
#include "NArray.h"
#include "NData.h"
#include "NDictionary.h"
#include "NNumber.h"
#include "NPreferenceFile.h"
#include "NStdAlgorithm.h"
#include "NStdContainer.h"
#include "NString.h"
#include "NTestFixture.h"
#include "NTime.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetTestDictionary : Get a test dictionary.
//-----------------------------------------------------------------------------
static NDictionary GetTestDictionary()
{


	// Get the dictionary
	NDictionary theResult;

	theResult["one"]   = 1;
	theResult["two"]   = "two";
	theResult["three"] = NNumber(3);

	return theResult;
}





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Keys
static const NString kKeyBool                               = "TPreferenceFile/Boolean";
static const NString kKeyUInt32                             = "TPreferenceFile/UInt32";
static const NString kKeyUInt64                             = "TPreferenceFile/UInt64";
static const NString kKeyInt32                              = "TPreferenceFile/Int32";
static const NString kKeyInt64                              = "TPreferenceFile/Int64";
static const NString kKeyFloat32                            = "TPreferenceFile/Float32";
static const NString kKeyFloat64                            = "TPreferenceFile/Float64";

static const NString kKeyArray                              = "TPreferenceFile/Array";
static const NString kKeyData                               = "TPreferenceFile/Data";
static const NString kKeyDictionary                         = "TPreferenceFile/Dictionary";
static const NString kKeyNumber                             = "TPreferenceFile/Number";
static const NString kKeyString                             = "TPreferenceFile/String";
static const NString kKeyTime                               = "TPreferenceFile/Time";

// Values
static const uint8_t kValueUInt8s[]{0xA1, 0xB1, 0xC1, 0xD1, 0xA2, 0xB2, 0xC2, 0xD2};

static constexpr bool      kValueBool                       = true;
static constexpr uint32_t  kValueUInt32                     = 132;
static constexpr uint64_t  kValueUInt64                     = 164;
static constexpr int32_t   kValueInt32                      = -132;
static constexpr int64_t   kValueInt64                      = -164;
static constexpr float32_t kValueFloat32                    = 32.5;
static constexpr float64_t kValueFloat64                    = 64.5;

static const NArray      kValueArray(nstd::vector(kValueUInt8s));
static const NData       kValueData(sizeof(kValueUInt8s), kValueUInt8s);
static const NDictionary kValueDictionary(GetTestDictionary());
static const NNumber     kValueNumber(100);
static const NString     kValueString("Testing");
static constexpr NTime   kValueTime(100);





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TPreferenceFile)
{
	NPreferenceFile theFile;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "Load")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "Save")
{


	// Perform the test
	NN_LOG_UNIMPLEMENTED();
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "HasKey")
{


	// Perform the test
	REQUIRE(!theFile.HasKey(kKeyBool));

	theFile.SetValue(kKeyBool, kValueBool);
	REQUIRE(theFile.HasKey(kKeyBool));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "RemoveKey")
{


	// Perform the test
	REQUIRE(!theFile.HasKey(kKeyBool));

	theFile.SetValue(kKeyBool, kValueBool);
	REQUIRE(theFile.HasKey(kKeyBool));

	theFile.RemoveKey(kKeyBool);
	REQUIRE(!theFile.HasKey(kKeyBool));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "GetKeys")
{


	// Perform the test
	theFile.SetValue(kKeyBool, kValueBool);
	theFile.SetValue(kKeyUInt32, kValueUInt32);
	theFile.SetValue(kKeyUInt64, kValueUInt64);
	theFile.SetValue(kKeyFloat32, kValueFloat32);
	theFile.SetValue(kKeyFloat64, kValueFloat64);

	NVectorString theKeys = theFile.GetKeys();

	REQUIRE(theKeys.size() == 5);
	REQUIRE(nstd::contains(theKeys, kKeyBool));
	REQUIRE(nstd::contains(theKeys, kKeyUInt32));
	REQUIRE(nstd::contains(theKeys, kKeyUInt64));
	REQUIRE(nstd::contains(theKeys, kKeyFloat32));
	REQUIRE(nstd::contains(theKeys, kKeyFloat64));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "GetValue")
{


	// Perform the test
	NAny theValue;

	theFile.SetValue(kKeyBool, kValueBool);
	theFile.SetValue(kKeyUInt64, kValueUInt64);
	theFile.SetValue(kKeyFloat64, kValueFloat64);

	theValue = theFile.GetValue(kKeyBool);
	REQUIRE(theValue.IsNumber());
	REQUIRE(theValue.GetNumber().GetBool() == kValueBool);

	theValue = theFile.GetValue(kKeyUInt64);
	REQUIRE(theValue.IsNumber());
	REQUIRE(theValue.GetNumber().GetUInt64() == kValueUInt64);

	theValue = theFile.GetValue(kKeyFloat64);
	REQUIRE(theValue.IsNumber());
	REQUIRE(theValue.GetNumber().GetFloat64() == kValueFloat64);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "GetValue/Typed")
{


	// Perform the test
	theFile.SetValue(kKeyBool, kValueBool);
	theFile.SetValue(kKeyUInt32, kValueUInt32);
	theFile.SetValue(kKeyUInt64, kValueUInt64);
	theFile.SetValue(kKeyInt32, kValueInt32);
	theFile.SetValue(kKeyInt64, kValueInt64);
	theFile.SetValue(kKeyFloat32, kValueFloat32);
	theFile.SetValue(kKeyFloat64, kValueFloat64);
	theFile.SetValue(kKeyArray, kValueArray);
	theFile.SetValue(kKeyData, kValueData);
	theFile.SetValue(kKeyDictionary, kValueDictionary);
	theFile.SetValue(kKeyNumber, kValueNumber);
	theFile.SetValue(kKeyString, kValueString);
	theFile.SetValue(kKeyTime, kValueTime);


	REQUIRE(theFile.GetBool(kKeyBool) == kValueBool);
	REQUIRE(theFile.GetUInt32(kKeyUInt32) == kValueUInt32);
	REQUIRE(theFile.GetUInt64(kKeyUInt64) == kValueUInt64);
	REQUIRE(theFile.GetInt32(kKeyInt32) == kValueInt32);
	REQUIRE(theFile.GetInt64(kKeyInt64) == kValueInt64);
	REQUIRE(theFile.GetFloat32(kKeyFloat32) == kValueFloat32);
	REQUIRE(theFile.GetFloat64(kKeyFloat64) == kValueFloat64);
	REQUIRE(theFile.GetArray(kKeyArray) == kValueArray);
	REQUIRE(theFile.GetData(kKeyData) == kValueData);
	REQUIRE(theFile.GetDictionary(kKeyDictionary) == kValueDictionary);
	REQUIRE(theFile.GetNumber(kKeyNumber) == kValueNumber);
	REQUIRE(theFile.GetString(kKeyString) == kValueString);
	REQUIRE(theFile.GetTime(kKeyTime) == kValueTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferenceFile, "SetValue")
{


	// Perform the test
	theFile.SetValue(kKeyBool, kValueBool);
	theFile.SetValue(kKeyUInt32, kValueUInt32);
	theFile.SetValue(kKeyUInt64, kValueUInt64);
	theFile.SetValue(kKeyInt32, kValueInt32);
	theFile.SetValue(kKeyInt64, kValueInt64);
	theFile.SetValue(kKeyFloat32, kValueFloat32);
	theFile.SetValue(kKeyFloat64, kValueFloat64);
	theFile.SetValue(kKeyArray, kValueArray);
	theFile.SetValue(kKeyData, kValueData);
	theFile.SetValue(kKeyDictionary, kValueDictionary);
	theFile.SetValue(kKeyNumber, kValueNumber);
	theFile.SetValue(kKeyString, kValueString);
	theFile.SetValue(kKeyTime, kValueTime);

	REQUIRE(theFile.GetKeys().size() == 13);
}
