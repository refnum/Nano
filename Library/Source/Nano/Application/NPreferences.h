/*	NAME:
		NPreferences.h

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
#ifndef NPREFERENCES_H
#define NPREFERENCES_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NBroadcast.h"
#include "NBroadcastQueue.h"
#include "NMutex.h"
#include "NPreferenceFile.h"

// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NPreferenceFile;
class NAny;
class NArray;
class NData;
class NDictionary;
class NNumber;
class NString;
class NTime;


// Preferences
struct NPreferenceLayer
{
	size_t          thePriority;
	NPreferenceFile thePrefs;
};

using NVectorPreferenceLayers = std::vector<NPreferenceLayer>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NPreferences
{
public:
										NPreferences();


	// Is a key present?
	static bool                         HasKey(const NString& theKey);


	// Remove a key
	static void                         RemoveKey(const NString& theKey);


	// Get a value
	//
	// A value may only be fetched as a type if it is of that type.
	//
	// Returns 0/empty if there is no value for the key.
	static NAny                         GetValue(const NString& theKey);

	static bool                         GetBool(   const NString& theKey);
	static uint32_t                     GetUInt32( const NString& theKey);
	static uint64_t                     GetUInt64( const NString& theKey);
	static int32_t                      GetInt32(  const NString& theKey);
	static int64_t                      GetInt64(  const NString& theKey);
	static float32_t                    GetFloat32(const NString& theKey);
	static float64_t                    GetFloat64(const NString& theKey);

	static NArray                       GetArray(     const NString& theKey);
	static NData                        GetData(      const NString& theKey);
	static NDictionary                  GetDictionary(const NString& theKey);
	static NNumber                      GetNumber(    const NString& theKey);
	static NString                      GetString(    const NString& theKey);
	static NTime                        GetTime(      const NString& theKey);


	// Set a value
	//
	// The value must be an NArray, NData, NDictionary, NNumber, NString, NTime,
	// or a type that can be converted to NNumber.
	static void                         SetValue(const NString& theKey, const NAny& theValue);


	// Add a default layer
	//
	// Default layers provide values that are returned if a key does
	// not exist in the application preferences.
	//
	// Layers are ordered by ascending priority, where the highest
	// priority is a value of 1.
	static void                         AddLayer(const NPreferenceLayer& theLayer);


	// Get the change message
	//
	// A change message is broadcast whenever a key is added, removed, or modified.
	//
	// The name of the change message is derived from the key.
	//
	// The value of the change message is the new value for the key, if any.
	//
	// Change messages are sent on a background thread.
	static NString                      GetChangeMessage(const NString& theKey);
	static NString                      GetChangeKey(    const NString& theMessage);


private:
	static NPreferences*                Get();

	void                                Load();
	void                                KeyChanged(const NString& theKey);


private:
	NMutex                              mLock;
	NVectorPreferenceLayers             mLayers;
	NBroadcastQueue                     mChanges;
};



#endif // NPREFERENCES_H
