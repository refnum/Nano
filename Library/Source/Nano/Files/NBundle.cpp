/*	NAME:
		NBundle.cpp

	DESCRIPTION:
		macOS/iOS-style directory bundle.

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
#include "NBundle.h"

// Nano
#include "NArray.h"
#include "NFileHandle.h"
#include "NFilePath.h"
#include "NFormat.h"
#include "NMutex.h"
#include "NPropertyList.h"
#include "NScopedLock.h"
#include "NString.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NString kStringsExtension                      = ".strings";
static const NString kStringsDefaultLanguage                = "en.lproj";
static const NString kStringsDefaultTable                   = "Localizable";





//=============================================================================
//		NBundle::NBundle : Constructor.
//-----------------------------------------------------------------------------
NBundle::NBundle(const NFile& theRoot)
	: mRoot(theRoot)
	, mInfo()
{


	// Validate our parameters
	NN_REQUIRE(mRoot.IsDirectory());
}





//=============================================================================
//		NBundle::NBundle : Constructor.
//-----------------------------------------------------------------------------
NBundle::NBundle(const NString& bundleID)
	: mRoot()
	, mInfo()
{


	// Initialize ourselves
	mRoot = GetBundle(bundleID);
	NN_REQUIRE(mRoot.IsDirectory());
}





//=============================================================================
//		NBundle::IsValid : Is the bundle valid?
//-----------------------------------------------------------------------------
bool NBundle::IsValid() const
{


	// Check our state
	return mRoot.IsDirectory();
}





//=============================================================================
//		NBundle::GetIdentifier : Get the identifier.
//-----------------------------------------------------------------------------
NString NBundle::GetIdentifier() const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the identifier
	return GetInfoString(kNBundleIdentifierKey);
}





//=============================================================================
//		NBundle::GetRoot : Get the root directory.
//-----------------------------------------------------------------------------
NFile NBundle::GetRoot() const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the root
	return mRoot;
}





//=============================================================================
//		NBundle::GetResources : Get the resources directory.
//-----------------------------------------------------------------------------
NFile NBundle::GetResources() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the resources
	return GetResources(mRoot);
}





//=============================================================================
//		NBundle::GetInfoBool : Get an Info.plist boolean.
//-----------------------------------------------------------------------------
bool NBundle::GetInfoBool(const NString& theKey) const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the value
	return GetInfoDictionary().GetBool(theKey);
}





//=============================================================================
//		NBundle::GetInfoString : Get an Info.plist string.
//-----------------------------------------------------------------------------
NString NBundle::GetInfoString(const NString& theKey) const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the value
	return GetInfoDictionary().GetString(theKey);
}





//=============================================================================
//		NBundle::GetInfoArray : Get an Info.plist array.
//-----------------------------------------------------------------------------
NArray NBundle::GetInfoArray(const NString& theKey) const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the value
	return GetInfoDictionary().GetArray(theKey);
}





//=============================================================================
//		NBundle::GetInfoDictionary : Get an Info.plist dictionary.
//-----------------------------------------------------------------------------
NDictionary NBundle::GetInfoDictionary(const NString& theKey) const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Load the dictionary
	if (mInfo.IsEmpty())
	{
		mInfo = GetInfoDictionary(mRoot);
	}



	// Get the value
	if (theKey.IsEmpty())
	{
		return mInfo;
	}
	else
	{
		return mInfo.GetDictionary(theKey);
	}
}





//=============================================================================
//		NBundle::GetExecutable : Get an executable.
//-----------------------------------------------------------------------------
NFile NBundle::GetExecutable(const NString& theName) const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the state we need
	NString exeName = theName;

	if (exeName.IsEmpty())
	{
		exeName = GetInfoString(kNBundleExecutableKey);
	}



	// Get the executable
	return GetExecutable(mRoot, exeName);
}





//=============================================================================
//		NBundle::GetResource : Get a resource from the bundle.
//-----------------------------------------------------------------------------
NFile NBundle::GetResource(const NString& theName,
						   const NString& theType,
						   const NString& subDir) const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Check our parameters
	if (theName.IsEmpty())
	{
		return {};
	}



	// Get an absolute "resource"
	//
	// We allow bundle resources to be specified with an absolute path,
	// as this allows us to reference 'resources' outside the bundle.
	if (theName.StartsWith("/"))
	{
		return NFilePath(theName);
	}



	// Get an internal resource
	//
	// Relative paths are relative to the bundle resources directory.
	NFilePath thePath = GetResources().GetPath();

	if (subDir.IsEmpty())
	{
		thePath = thePath.GetChild(theName);
	}
	else
	{
		thePath = thePath.GetChild(subDir).GetChild(theName);
	}

	if (!theType.IsEmpty())
	{
		thePath.SetExtension(theType);
	}

	return thePath;
}





//=============================================================================
//		NBundle::GetString : Get a string from the bundle.
//-----------------------------------------------------------------------------
NString NBundle::GetString(const NString& theKey,
						   const NString& defaultValue,
						   const NString& tableName) const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	NScopedLock   acquireLock(GetCacheLock());
	NBundleCache& theCache = GetCache();


	NString theTable = kStringsDefaultTable;
	if (!tableName.IsEmpty())
	{
		theTable = tableName;
		theTable.Replace(kStringsExtension, "");
	}



	NString tableID    = GetIdentifier() + "." + theTable;
	auto    cacheEntry = theCache.GetValue(tableID);
	NString theValue;

	if (cacheEntry)
	{
		theValue = cacheEntry->GetString(theKey);
	}
	else
	{
		NDictionary theStrings = GetStringTable(tableName);
		theValue               = theStrings.GetString(theKey);

		theCache.SetValue(theKey, theStrings);
	}



	if (theValue.IsEmpty())
	{
		theValue = defaultValue;
	}

	return theValue;
}





#pragma mark private
//=============================================================================
//		NBundle::GetStringTable : Get a string table.
//-----------------------------------------------------------------------------
NDictionary NBundle::GetStringTable(const NString& theTable) const
{


	// Validate our parameters
	NN_REQUIRE(!theTable.IsEmpty());



	// Get the state we need
	NFile theFile = GetResource(theTable, kStringsExtension, kStringsDefaultLanguage);
	NData theData = NFileHandle::ReadData(theFile);



	// Parse the strings file
	//
	// Strings files are a text file, where each line defines a key/value pair.
	//
	// In traditional bundles these files are copied directly into the bundle,
	// without changing their format.
	//
	// In flattened bundles these files are converted into a property list,
	// although they retain their ".strings" extension.
	//
	NPropertyListFormat theFormat = NPropertyList::GetFormat(theData);
	NDictionary         theResult;

	if (theFormat != NPropertyListFormat::Unknown)
	{
		theResult = NPropertyList::Decode(theData);
	}
	else
	{
		for (const auto& theLine : NFileHandle::ReadText(theFile).GetLines())
		{
			NVectorString theMatches = theLine.GetMatches("(.*?)\\s*=\\s*\"(.*)\";");

			if (theMatches.size() == 2)
			{
				NString theKey   = theMatches[0];
				NString theValue = theMatches[1];

				theValue.ReplaceAll("\\n", "\n");
				theValue.ReplaceAll("\\\"", "\"");
				theValue.ReplaceAll("\\\\", "\\");

				theResult[theKey] = theValue;
			}
		}
	}

	return theResult;
}





//=============================================================================
//		NBundle::GetCacheLock : Get the cache lock.
//-----------------------------------------------------------------------------
NMutex& NBundle::GetCacheLock()
{


	// Get the lock
	static NMutex sLock;

	return sLock;
}





//=============================================================================
//		NBundle::GetCache : Get the cache.
//-----------------------------------------------------------------------------
NBundleCache& NBundle::GetCache()
{


	// Validate our state
	NN_REQUIRE(GetCacheLock().IsLocked());



	// Get the cache
	static NBundleCache sCache;

	return sCache;
}
