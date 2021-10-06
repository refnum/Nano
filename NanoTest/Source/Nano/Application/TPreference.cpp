/*	NAME:
		TPreference.cpp

	DESCRIPTION:
		NPreference tests.

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
#include "NPreference.h"
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
static const NString kKeyBool                               = "TPreference/Boolean";
static const NString kKeyUInt32                             = "TPreference/UInt32";
static const NString kKeyUInt64                             = "TPreference/UInt64";
static const NString kKeyInt32                              = "TPreference/Int32";
static const NString kKeyInt64                              = "TPreference/Int64";
static const NString kKeyFloat32                            = "TPreference/Float32";
static const NString kKeyFloat64                            = "TPreference/Float64";

static const NString kKeyArray                              = "TPreference/Array";
static const NString kKeyData                               = "TPreference/Data";
static const NString kKeyDictionary                         = "TPreference/Dictionary";
static const NString kKeyNumber                             = "TPreference/Number";
static const NString kKeyString                             = "TPreference/String";
static const NString kKeyTime                               = "TPreference/Time";

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
NANO_FIXTURE(TPreference)
{
	NPreferenceBool       prefBool{kKeyBool};
	NPreferenceUInt32     prefUInt32{kKeyUInt32};
	NPreferenceUInt64     prefUInt64{kKeyUInt64};
	NPreferenceInt32      prefInt32{kKeyInt32};
	NPreferenceInt64      prefInt64{kKeyInt64};
	NPreferenceFloat32    prefFloat32{kKeyFloat32};
	NPreferenceFloat64    prefFloat64{kKeyFloat64};
	NPreferenceArray      prefArray{kKeyArray};
	NPreferenceData       prefData{kKeyData};
	NPreferenceDictionary prefDictionary{kKeyDictionary};
	NPreferenceNumber     prefNumber{kKeyNumber};
	NPreferenceString     prefString{kKeyString};
	NPreferenceTime       prefTime{kKeyTime};

	SETUP
	{
		RemoveKeys();
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
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreference, "GetKey")
{


	// Perform the test
	REQUIRE(prefBool.GetKey() == kKeyBool);
	REQUIRE(prefUInt32.GetKey() == kKeyUInt32);
	REQUIRE(prefUInt64.GetKey() == kKeyUInt64);
	REQUIRE(prefInt32.GetKey() == kKeyInt32);
	REQUIRE(prefInt64.GetKey() == kKeyInt64);
	REQUIRE(prefFloat32.GetKey() == kKeyFloat32);
	REQUIRE(prefFloat64.GetKey() == kKeyFloat64);
	REQUIRE(prefArray.GetKey() == kKeyArray);
	REQUIRE(prefData.GetKey() == kKeyData);
	REQUIRE(prefDictionary.GetKey() == kKeyDictionary);
	REQUIRE(prefNumber.GetKey() == kKeyNumber);
	REQUIRE(prefString.GetKey() == kKeyString);
	REQUIRE(prefTime.GetKey() == kKeyTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreference, "Exists")
{


	// Perform the test
	REQUIRE(!prefBool.Exists());
	REQUIRE(!prefUInt32.Exists());
	REQUIRE(!prefUInt64.Exists());
	REQUIRE(!prefInt32.Exists());
	REQUIRE(!prefInt64.Exists());
	REQUIRE(!prefFloat32.Exists());
	REQUIRE(!prefFloat64.Exists());
	REQUIRE(!prefArray.Exists());
	REQUIRE(!prefData.Exists());
	REQUIRE(!prefDictionary.Exists());
	REQUIRE(!prefNumber.Exists());
	REQUIRE(!prefString.Exists());
	REQUIRE(!prefTime.Exists());

	prefBool.Set(kValueBool);
	prefUInt32.Set(kValueUInt32);
	prefUInt64.Set(kValueUInt64);
	prefInt32.Set(kValueInt32);
	prefInt64.Set(kValueInt64);
	prefFloat32.Set(kValueFloat32);
	prefFloat64.Set(kValueFloat64);
	prefArray.Set(kValueArray);
	prefData.Set(kValueData);
	prefDictionary.Set(kValueDictionary);
	prefNumber.Set(kValueNumber);
	prefString.Set(kValueString);
	prefTime.Set(kValueTime);

	REQUIRE(prefBool.Exists());
	REQUIRE(prefUInt32.Exists());
	REQUIRE(prefUInt64.Exists());
	REQUIRE(prefInt32.Exists());
	REQUIRE(prefInt64.Exists());
	REQUIRE(prefFloat32.Exists());
	REQUIRE(prefFloat64.Exists());
	REQUIRE(prefArray.Exists());
	REQUIRE(prefData.Exists());
	REQUIRE(prefDictionary.Exists());
	REQUIRE(prefNumber.Exists());
	REQUIRE(prefString.Exists());
	REQUIRE(prefTime.Exists());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreference, "GetSetGet")
{


	// Perform the test
	REQUIRE(prefBool.Get() == false);
	REQUIRE(prefUInt32.Get() == 0);
	REQUIRE(prefUInt64.Get() == 0);
	REQUIRE(prefInt32.Get() == 0);
	REQUIRE(prefInt64.Get() == 0);
	REQUIRE(prefFloat32.Get() == 0.0f);
	REQUIRE(prefFloat64.Get() == 0.0);
	REQUIRE(prefArray.Get() == NArray());
	REQUIRE(prefData.Get() == NData());
	REQUIRE(prefDictionary.Get() == NDictionary());
	REQUIRE(prefNumber.Get() == NNumber());
	REQUIRE(prefString.Get() == NString());
	REQUIRE(prefTime.Get() == NTime());

	prefBool.Set(kValueBool);
	prefUInt32.Set(kValueUInt32);
	prefUInt64.Set(kValueUInt64);
	prefInt32.Set(kValueInt32);
	prefInt64.Set(kValueInt64);
	prefFloat32.Set(kValueFloat32);
	prefFloat64.Set(kValueFloat64);
	prefArray.Set(kValueArray);
	prefData.Set(kValueData);
	prefDictionary.Set(kValueDictionary);
	prefNumber.Set(kValueNumber);
	prefString.Set(kValueString);
	prefTime.Set(kValueTime);

	REQUIRE(prefBool.Get() == kValueBool);
	REQUIRE(prefUInt32.Get() == kValueUInt32);
	REQUIRE(prefUInt64.Get() == kValueUInt64);
	REQUIRE(prefInt32.Get() == kValueInt32);
	REQUIRE(prefInt64.Get() == kValueInt64);
	REQUIRE(prefFloat32.Get() == kValueFloat32);
	REQUIRE(prefFloat64.Get() == kValueFloat64);
	REQUIRE(prefArray.Get() == kValueArray);
	REQUIRE(prefData.Get() == kValueData);
	REQUIRE(prefDictionary.Get() == kValueDictionary);
	REQUIRE(prefNumber.Get() == kValueNumber);
	REQUIRE(prefString.Get() == kValueString);
	REQUIRE(prefTime.Get() == kValueTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPreference, "GetUpdateGet")
{


	// Perform the test
	REQUIRE(prefBool.Get() == false);
	REQUIRE(prefUInt32.Get() == 0);
	REQUIRE(prefUInt64.Get() == 0);
	REQUIRE(prefInt32.Get() == 0);
	REQUIRE(prefInt64.Get() == 0);
	REQUIRE(prefFloat32.Get() == 0.0f);
	REQUIRE(prefFloat64.Get() == 0.0);
	REQUIRE(prefArray.Get() == NArray());
	REQUIRE(prefData.Get() == NData());
	REQUIRE(prefDictionary.Get() == NDictionary());
	REQUIRE(prefNumber.Get() == NNumber());
	REQUIRE(prefString.Get() == NString());
	REQUIRE(prefTime.Get() == NTime());

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

	REQUIRE(prefBool.Get() == kValueBool);
	REQUIRE(prefUInt32.Get() == kValueUInt32);
	REQUIRE(prefUInt64.Get() == kValueUInt64);
	REQUIRE(prefInt32.Get() == kValueInt32);
	REQUIRE(prefInt64.Get() == kValueInt64);
	REQUIRE(prefFloat32.Get() == kValueFloat32);
	REQUIRE(prefFloat64.Get() == kValueFloat64);
	REQUIRE(prefArray.Get() == kValueArray);
	REQUIRE(prefData.Get() == kValueData);
	REQUIRE(prefDictionary.Get() == kValueDictionary);
	REQUIRE(prefNumber.Get() == kValueNumber);
	REQUIRE(prefString.Get() == kValueString);
	REQUIRE(prefTime.Get() == kValueTime);
}
