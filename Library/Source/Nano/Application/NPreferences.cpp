/*	NAME:
		NPreferences.cpp

	DESCRIPTION:
		Preferences system.

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
#include "NPreferences.h"

// Nano
#include "NArray.h"
#include "NBroadcaster.h"
#include "NData.h"
#include "NDictionary.h"
#include "NExecute.h"
#include "NFormat.h"
#include "NNumber.h"
#include "NScopedLock.h"
#include "NSet.h"
#include "NStdAlgorithm.h"
#include "NString.h"
#include "NTime.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline constexpr size_t      kNLayerApp                     = 0;
inline constexpr const char* kNChangeBroadcastPrefix        = "NPreferences.Changed.";





//=============================================================================
//		NPreferences::NPreferences : Constructor.
//-----------------------------------------------------------------------------
NPreferences::NPreferences()
	: mLock()
	, mLayers()
	, mChanges()
{


	// Initialise ourselves
	Load();
}





//=============================================================================
//		NPreferences::HasKey : Is a key present?
//-----------------------------------------------------------------------------
bool NPreferences::HasKey(const NString& theKey)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Is the key present?
	NPreferences* thePrefs = NPreferences::Get();
	NScopedLock   acquireLock(thePrefs->mLock);

	for (const auto& theLayer : thePrefs->mLayers)
	{
		if (theLayer.thePrefs.HasKey(theKey))
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NPreferences::RemoveKey : Remove a key.
//-----------------------------------------------------------------------------
void NPreferences::RemoveKey(const NString& theKey)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Remove the key
	NPreferences* thePrefs = NPreferences::Get();
	NScopedLock   acquireLock(thePrefs->mLock);

	thePrefs->mLayers[kNLayerApp].thePrefs.RemoveKey(theKey);
	thePrefs->KeyChanged(theKey);
}





//=============================================================================
//		NPreferences::GetValue : Get a value.
//-----------------------------------------------------------------------------
NAny NPreferences::GetValue(const NString& theKey)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	NPreferences* thePrefs = NPreferences::Get();
	NScopedLock   acquireLock(thePrefs->mLock);

	for (const auto& theLayer : thePrefs->mLayers)
	{
		if (theLayer.thePrefs.HasKey(theKey))
		{
			return theLayer.thePrefs.GetValue(theKey);
		}
	}

	return {};
}





//=============================================================================
//		NPreferences::GetBool : Get a bool value.
//-----------------------------------------------------------------------------
bool NPreferences::GetBool(const NString& theKey)
{


	// Get the value
	return GetNumber(theKey).GetBool();
}





//=============================================================================
//		NPreferences::GetUInt32 : Get a uint32_t value.
//-----------------------------------------------------------------------------
uint32_t NPreferences::GetUInt32(const NString& theKey)
{


	// Get the value
	return GetNumber(theKey).GetUInt32();
}





//=============================================================================
//		NPreferences::GetUInt64 : Get a uint64_t value.
//-----------------------------------------------------------------------------
uint64_t NPreferences::GetUInt64(const NString& theKey)
{


	// Get the value
	return GetNumber(theKey).GetUInt64();
}





//=============================================================================
//		NPreferences::GetInt32 : Get an int32_t value.
//-----------------------------------------------------------------------------
int32_t NPreferences::GetInt32(const NString& theKey)
{


	// Get the value
	return GetNumber(theKey).GetInt32();
}





//=============================================================================
//		NPreferences::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
int64_t NPreferences::GetInt64(const NString& theKey)
{


	// Get the value
	return GetNumber(theKey).GetInt64();
}





//=============================================================================
//		NPreferences::GetFloat32 : Get a float32_t value.
//-----------------------------------------------------------------------------
float32_t NPreferences::GetFloat32(const NString& theKey)
{


	// Get the value
	return GetNumber(theKey).GetFloat32();
}





//=============================================================================
//		NPreferences::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
float64_t NPreferences::GetFloat64(const NString& theKey)
{


	// Get the value
	return GetNumber(theKey).GetFloat64();
}





//=============================================================================
//		NPreferences::GetArray : Get an NArray.
//-----------------------------------------------------------------------------
NArray NPreferences::GetArray(const NString& theKey)
{


	// Get the value
	return GetValue(theKey).GetArray();
}





//=============================================================================
//		NPreferences::GetData : Get an NData.
//-----------------------------------------------------------------------------
NData NPreferences::GetData(const NString& theKey)
{


	// Get the value
	return GetValue(theKey).GetData();
}





//=============================================================================
//		NPreferences::GetDictionary : Get an NDictionary.
//-----------------------------------------------------------------------------
NDictionary NPreferences::GetDictionary(const NString& theKey)
{


	// Get the value
	return GetValue(theKey).GetDictionary();
}





//=============================================================================
//		NPreferences::GetNumber : Get an NNumber.
//-----------------------------------------------------------------------------
NNumber NPreferences::GetNumber(const NString& theKey)
{


	// Get the value
	return GetValue(theKey).GetNumber();
}





//=============================================================================
//		NPreferences::GetString : Get an NString.
//-----------------------------------------------------------------------------
NString NPreferences::GetString(const NString& theKey)
{


	// Get the value
	return GetValue(theKey).GetString();
}





//=============================================================================
//		NPreferences::GetTime : Get an NTime.
//-----------------------------------------------------------------------------
NTime NPreferences::GetTime(const NString& theKey)
{


	// Get the value
	return GetValue(theKey).GetTime();
}





//=============================================================================
//		NPreferences::SetValue : Set a value.
//-----------------------------------------------------------------------------
void NPreferences::SetValue(const NString& theKey, const NAny& theValue)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());
	NN_REQUIRE(!theValue.IsEmpty());



	// Set the value
	NPreferences* thePrefs = NPreferences::Get();
	NScopedLock   acquireLock(thePrefs->mLock);

	thePrefs->mLayers[kNLayerApp].thePrefs.SetValue(theKey, theValue);
	thePrefs->KeyChanged(theKey);
}





//=============================================================================
//		NPreferences::AddLayer : Add a default layer.
//-----------------------------------------------------------------------------
void NPreferences::AddLayer(const NPreferenceLayer& theLayer)
{


	// Validate our parameters
	NN_REQUIRE(theLayer.thePriority > kNLayerApp);



	// Add the defaults
	NPreferences* thePrefs = NPreferences::Get();
	NScopedLock   acquireLock(thePrefs->mLock);

	bool          didInsert = false;
	NSet<NString> priorKeys;

	for (auto theIter = thePrefs->mLayers.begin(); theIter != thePrefs->mLayers.end(); theIter++)
	{
		didInsert = (theLayer.thePriority < theIter->thePriority);
		if (didInsert)
		{
			thePrefs->mLayers.emplace(theIter, theLayer);
			break;
		}

		for (const auto& theKey : theIter->thePrefs.GetKeys())
		{
			priorKeys.insert(theKey);
		}
	}

	if (!didInsert)
	{
		thePrefs->mLayers.emplace_back(theLayer);
	}



	// Mark the 'new' keys as changed
	//
	// Keys that are present in this layer but were not present in prior
	// layers are effectively new keys that can be returned by getters.
	//
	// These keys must be broadcast to ensure anyone looking for them
	// knows that they now exist.
	for (const auto& theKey : theLayer.thePrefs.GetKeys())
	{
		if (!priorKeys.contains(theKey))
		{
			thePrefs->KeyChanged(theKey);
		}
	}

	NN_LOG_UNIMPLEMENTED("Verify NPreferences::AddLayer insertion");
}





//=============================================================================
//		NPreferences::GetChangeBroadcast : Get a key's change broadcast.
//-----------------------------------------------------------------------------
NString NPreferences::GetChangeBroadcast(const NString& theKey)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());


	// Get the change name
	return NFormat("{}{}", kNChangeBroadcastPrefix, theKey);
}





//=============================================================================
//		NPreferences::GetChangeKey : Get a change broadcast's key.
//-----------------------------------------------------------------------------
NString NPreferences::GetChangeKey(const NString& theName)
{


	// Validate our parameters
	NN_REQUIRE(theName.GetSize() > strlen(kNChangeBroadcastPrefix));



	// Get the key
	return theName.GetSuffix(theName.GetSize() - strlen(kNChangeBroadcastPrefix));
}





#pragma mark private
//=============================================================================
//		NPreferences::Get : Get the instance.
//-----------------------------------------------------------------------------
NPreferences* NPreferences::Get()
{


	// Get the instance
	//
	// The preferences instance is allowed to leak to
	// support usage during static destruction.
	static NPreferences* sInstance = new NPreferences;

	return sInstance;
}





//=============================================================================
//		NPreferences::Load : Load the preferences.
//-----------------------------------------------------------------------------
void NPreferences::Load()
{


	// Load the preferences
	mLayers.resize(1);
}





//=============================================================================
//		NPreferences::KeyChanged : Handle key changes.
//-----------------------------------------------------------------------------
void NPreferences::KeyChanged(const NString& theKey)
{


	// Validate our parameters and state
	NN_REQUIRE(!theKey.IsEmpty());
	NN_REQUIRE(mLock.IsLocked());



	// Enqueue the broadcast
	mChanges.Add({GetChangeBroadcast(theKey), GetValue(theKey)});
}
