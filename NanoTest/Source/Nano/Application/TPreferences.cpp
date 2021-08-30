/*	NAME:
		TPreferences.cpp

	DESCRIPTION:
		NPreferences tests.

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
#include "NPreferences.h"
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
static const NString kKeyBool                               = "TPreferences/Boolean";
static const NString kKeyUInt32                             = "TPreferences/UInt32";
static const NString kKeyUInt64                             = "TPreferences/UInt64";
static const NString kKeyInt32                              = "TPreferences/Int32";
static const NString kKeyInt64                              = "TPreferences/Int64";
static const NString kKeyFloat32                            = "TPreferences/Float32";
static const NString kKeyFloat64                            = "TPreferences/Float64";

static const NString kKeyArray                              = "TPreferences/Array";
static const NString kKeyData                               = "TPreferences/Data";
static const NString kKeyDictionary                         = "TPreferences/Dictionary";
static const NString kKeyNumber                             = "TPreferences/Number";
static const NString kKeyString                             = "TPreferences/String";
static const NString kKeyTime                               = "TPreferences/Time";

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
NANO_FIXTURE(TPreferences){

	SETUP{RemoveKeys();
}

TEARDOWN
{
	RemoveKeys();
}

void RemoveKeys()
{
	NPreferences::RemoveKey(kKeyBool);
	NPreferences::RemoveKey(kKeyUInt32);
	NPreferences::RemoveKey(kKeyUInt64);
	NPreferences::RemoveKey(kKeyInt32);
	NPreferences::RemoveKey(kKeyInt64);
	NPreferences::RemoveKey(kKeyFloat32);
	NPreferences::RemoveKey(kKeyFloat64);
	NPreferences::RemoveKey(kKeyArray);
	NPreferences::RemoveKey(kKeyData);
	NPreferences::RemoveKey(kKeyDictionary);
	NPreferences::RemoveKey(kKeyNumber);
	NPreferences::RemoveKey(kKeyString);
	NPreferences::RemoveKey(kKeyTime);
}
}
;





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferences, "HasKey")
{


	// Perform the test
	REQUIRE(!NPreferences::HasKey(kKeyBool));

	NPreferences::SetValue(kKeyBool, kValueBool);
	REQUIRE(NPreferences::HasKey(kKeyBool));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferences, "RemoveKey")
{


	// Perform the test
	REQUIRE(!NPreferences::HasKey(kKeyBool));

	NPreferences::SetValue(kKeyBool, kValueBool);
	REQUIRE(NPreferences::HasKey(kKeyBool));

	NPreferences::RemoveKey(kKeyBool);
	REQUIRE(!NPreferences::HasKey(kKeyBool));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferences, "GetValue")
{


	// Perform the test
	NAny theValue;

	NPreferences::SetValue(kKeyBool, kValueBool);
	NPreferences::SetValue(kKeyUInt64, kValueUInt64);
	NPreferences::SetValue(kKeyFloat64, kValueFloat64);

	theValue = NPreferences::GetValue(kKeyBool);
	REQUIRE(theValue.IsNumber());
	REQUIRE(theValue.GetNumber().GetBool() == kValueBool);

	theValue = NPreferences::GetValue(kKeyUInt64);
	REQUIRE(theValue.IsNumber());
	REQUIRE(theValue.GetNumber().GetUInt64() == kValueUInt64);

	theValue = NPreferences::GetValue(kKeyFloat64);
	REQUIRE(theValue.IsNumber());
	REQUIRE(theValue.GetNumber().GetFloat64() == kValueFloat64);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferences, "GetValue/Typed")
{


	// Perform the test
	NPreferences::SetValue(kKeyBool, kValueBool);
	NPreferences::SetValue(kKeyUInt32, kValueUInt32);
	NPreferences::SetValue(kKeyUInt64, kValueUInt64);
	NPreferences::SetValue(kKeyInt32, kValueInt32);
	NPreferences::SetValue(kKeyInt64, kValueInt64);
	NPreferences::SetValue(kKeyFloat32, kValueFloat32);
	NPreferences::SetValue(kKeyFloat64, kValueFloat64);
	NPreferences::SetValue(kKeyArray, kValueArray);
	NPreferences::SetValue(kKeyData, kValueData);
	NPreferences::SetValue(kKeyDictionary, kValueDictionary);
	NPreferences::SetValue(kKeyNumber, kValueNumber);
	NPreferences::SetValue(kKeyString, kValueString);
	NPreferences::SetValue(kKeyTime, kValueTime);


	REQUIRE(NPreferences::GetBool(kKeyBool) == kValueBool);
	REQUIRE(NPreferences::GetUInt32(kKeyUInt32) == kValueUInt32);
	REQUIRE(NPreferences::GetUInt64(kKeyUInt64) == kValueUInt64);
	REQUIRE(NPreferences::GetInt32(kKeyInt32) == kValueInt32);
	REQUIRE(NPreferences::GetInt64(kKeyInt64) == kValueInt64);
	REQUIRE(NPreferences::GetFloat32(kKeyFloat32) == kValueFloat32);
	REQUIRE(NPreferences::GetFloat64(kKeyFloat64) == kValueFloat64);
	REQUIRE(NPreferences::GetArray(kKeyArray) == kValueArray);
	REQUIRE(NPreferences::GetData(kKeyData) == kValueData);
	REQUIRE(NPreferences::GetDictionary(kKeyDictionary) == kValueDictionary);
	REQUIRE(NPreferences::GetNumber(kKeyNumber) == kValueNumber);
	REQUIRE(NPreferences::GetString(kKeyString) == kValueString);
	REQUIRE(NPreferences::GetTime(kKeyTime) == kValueTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferences, "SetValue")
{


	// Perform the test
	NPreferences::SetValue(kKeyBool, kValueBool);
	NPreferences::SetValue(kKeyUInt32, kValueUInt32);
	NPreferences::SetValue(kKeyUInt64, kValueUInt64);
	NPreferences::SetValue(kKeyInt32, kValueInt32);
	NPreferences::SetValue(kKeyInt64, kValueInt64);
	NPreferences::SetValue(kKeyFloat32, kValueFloat32);
	NPreferences::SetValue(kKeyFloat64, kValueFloat64);
	NPreferences::SetValue(kKeyArray, kValueArray);
	NPreferences::SetValue(kKeyData, kValueData);
	NPreferences::SetValue(kKeyDictionary, kValueDictionary);
	NPreferences::SetValue(kKeyNumber, kValueNumber);
	NPreferences::SetValue(kKeyString, kValueString);
	NPreferences::SetValue(kKeyTime, kValueTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferences, "AddLayer")
{


	// Perform the test
	NPreferenceLayer layer1, layer2;

	layer1.thePriority = 1;
	layer1.thePrefs.SetValue(kKeyUInt32, kValueUInt32);
	layer1.thePrefs.SetValue(kKeyUInt64, kValueUInt64);

	layer2.thePriority = 2;
	layer2.thePrefs.SetValue(kKeyFloat32, kValueFloat32);
	layer2.thePrefs.SetValue(kKeyFloat64, kValueFloat64);

	REQUIRE(!NPreferences::HasKey(kKeyBool));
	REQUIRE(!NPreferences::HasKey(kKeyUInt32));
	REQUIRE(!NPreferences::HasKey(kKeyUInt64));
	REQUIRE(!NPreferences::HasKey(kKeyFloat32));
	REQUIRE(!NPreferences::HasKey(kKeyFloat64));

	NPreferences::SetValue(kKeyBool, kValueBool);
	REQUIRE(NPreferences::HasKey(kKeyBool));
	REQUIRE(!NPreferences::HasKey(kKeyUInt32));
	REQUIRE(!NPreferences::HasKey(kKeyUInt64));
	REQUIRE(!NPreferences::HasKey(kKeyFloat32));
	REQUIRE(!NPreferences::HasKey(kKeyFloat64));

	NPreferences::AddLayer(layer2);
	REQUIRE(NPreferences::HasKey(kKeyBool));
	REQUIRE(!NPreferences::HasKey(kKeyUInt32));
	REQUIRE(!NPreferences::HasKey(kKeyUInt64));
	REQUIRE(NPreferences::HasKey(kKeyFloat32));
	REQUIRE(NPreferences::HasKey(kKeyFloat64));

	NPreferences::AddLayer(layer1);
	REQUIRE(NPreferences::HasKey(kKeyBool));
	REQUIRE(NPreferences::HasKey(kKeyUInt32));
	REQUIRE(NPreferences::HasKey(kKeyUInt64));
	REQUIRE(NPreferences::HasKey(kKeyFloat32));
	REQUIRE(NPreferences::HasKey(kKeyFloat64));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreferences, "GetChangeMessage")
{


	// Perform the test
	NString theMessage, theKey;

	theMessage = NPreferences::GetChangeMessage(kKeyBool);
	REQUIRE(!theMessage.IsEmpty());

	theKey = NPreferences::GetChangeKey(theMessage);
	REQUIRE(theKey == kKeyBool);
}
