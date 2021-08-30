/*	NAME:
		NPreferenceFile.cpp

	DESCRIPTION:
		Preference file.

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
#include "NPreferenceFile.h"

// Nano
#include "NArray.h"
#include "NData.h"
#include "NNumber.h"
#include "NScopedLock.h"
#include "NStdAlgorithm.h"
#include "NString.h"
#include "NTime.h"





//=============================================================================
//		NPreferenceFile::NPreferenceFile : Constructor.
//-----------------------------------------------------------------------------
NPreferenceFile::NPreferenceFile()
	: mFile()
	, mDictionary()
	, mIsDirty(false)
{
}





//=============================================================================
//		NPreferenceFile::~NPreferenceFile : Destructor.
//-----------------------------------------------------------------------------
NPreferenceFile::~NPreferenceFile()
{


	// Save the preferences
	NStatus theErr = Save();
	NN_EXPECT_NOT_ERR(theErr);
}





//=============================================================================
//		NPreferenceFile::Load : Load the preferences.
//-----------------------------------------------------------------------------
NStatus NPreferenceFile::Load(const NFilePath& theFile)
{


	// Load the preferences
	NN_LOG_UNIMPLEMENTED();
	NN_UNUSED(theFile);

	return NStatus::NotSupported;
}





//=============================================================================
//		NPreferenceFile::Save : Save the preferences.
//-----------------------------------------------------------------------------
NStatus NPreferenceFile::Save()
{


	// Validate our state
	NN_REQUIRE(!mFile.IsValid());



	// Save the preferences
	NStatus theErr = NStatus::OK;

	if (mIsDirty)
	{
		NN_LOG_UNIMPLEMENTED();
		theErr = NStatus::NotSupported;
	}

	return theErr;
}





//=============================================================================
//		NPreferenceFile::HasKey : Is a key present?
//-----------------------------------------------------------------------------
bool NPreferenceFile::HasKey(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Check our state
	return mDictionary.HasKey(theKey);
}





//=============================================================================
//		NPreferenceFile::RemoveKey : Remove a key.
//-----------------------------------------------------------------------------
void NPreferenceFile::RemoveKey(const NString& theKey)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Remove the key
	mDictionary.RemoveKey(theKey);
}





//=============================================================================
//		NPreferenceFile::GetKeys : Get the keys.
//-----------------------------------------------------------------------------
NVectorString NPreferenceFile::GetKeys() const
{


	// Get the keys
	return mDictionary.GetKeys();
}





//=============================================================================
//		NPreferenceFile::GetValue : Get a value.
//-----------------------------------------------------------------------------
NAny NPreferenceFile::GetValue(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());


	// Get the value
	return nstd::fetch(mDictionary, theKey, {});
}





//=============================================================================
//		NPreferenceFile::GetBool : Get a bool value.
//-----------------------------------------------------------------------------
bool NPreferenceFile::GetBool(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey).GetBool();
}





//=============================================================================
//		NPreferenceFile::GetUInt32 : Get a uint32_t value.
//-----------------------------------------------------------------------------
uint32_t NPreferenceFile::GetUInt32(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey).GetUInt32();
}





//=============================================================================
//		NPreferenceFile::GetUInt64 : Get a uint64_t value.
//-----------------------------------------------------------------------------
uint64_t NPreferenceFile::GetUInt64(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey).GetUInt64();
}





//=============================================================================
//		NPreferenceFile::GetInt32 : Get an int32_t value.
//-----------------------------------------------------------------------------
int32_t NPreferenceFile::GetInt32(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey).GetInt32();
}





//=============================================================================
//		NPreferenceFile::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
int64_t NPreferenceFile::GetInt64(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey).GetInt64();
}





//=============================================================================
//		NPreferenceFile::GetFloat32 : Get a float32_t value.
//-----------------------------------------------------------------------------
float32_t NPreferenceFile::GetFloat32(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey).GetFloat32();
}





//=============================================================================
//		NPreferenceFile::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
float64_t NPreferenceFile::GetFloat64(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey).GetFloat64();
}





//=============================================================================
//		NPreferenceFile::GetArray : Get an NArray.
//-----------------------------------------------------------------------------
NArray NPreferenceFile::GetArray(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetArray(theKey);
}





//=============================================================================
//		NPreferenceFile::GetData : Get an NData.
//-----------------------------------------------------------------------------
NData NPreferenceFile::GetData(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetData(theKey);
}





//=============================================================================
//		NPreferenceFile::GetDictionary : Get an NDictionary.
//-----------------------------------------------------------------------------
NDictionary NPreferenceFile::GetDictionary(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetDictionary(theKey);
}





//=============================================================================
//		NPreferenceFile::GetNumber : Get an NNumber.
//-----------------------------------------------------------------------------
NNumber NPreferenceFile::GetNumber(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetNumber(theKey);
}





//=============================================================================
//		NPreferenceFile::GetString : Get an NString.
//-----------------------------------------------------------------------------
NString NPreferenceFile::GetString(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetString(theKey);
}





//=============================================================================
//		NPreferenceFile::GetTime : Get an NTime.
//-----------------------------------------------------------------------------
NTime NPreferenceFile::GetTime(const NString& theKey) const
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());



	// Get the value
	return mDictionary.GetTime(theKey);
}





//=============================================================================
//		NPreferenceFile::SetValue : Set a value.
//-----------------------------------------------------------------------------
void NPreferenceFile::SetValue(const NString& theKey, const NAny& theValue)
{


	// Validate our parameters
	NN_REQUIRE(!theKey.IsEmpty());
	NN_REQUIRE(!theValue.IsEmpty());



	// Set the value
	if (theValue.Is<NArray>() || theValue.Is<NData>() || theValue.Is<NDictionary>() ||
		theValue.Is<NNumber>() || theValue.Is<NString>() || theValue.Is<NTime>())
	{
		mDictionary[theKey] = theValue;
	}

	else
	{
		NNumber theNumber;

		bool wasOK = theNumber.SetValue(theValue);
		NN_REQUIRE(wasOK);

		mDictionary[theKey] = theNumber;
	}
}
