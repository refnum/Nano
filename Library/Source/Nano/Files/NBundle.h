/*	NAME:
		NBundle.h

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
#ifndef NBUNDLE_H
#define NBUNDLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCache.h"
#include "NDictionary.h"
#include "NFile.h"
#include "NImage.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Info.plist keys
inline constexpr const utf8_t* kNBundleExecutableKey        = "CFBundleExecutable";
inline constexpr const utf8_t* kNBundleIdentifierKey        = "CFBundleIdentifier";
inline constexpr const utf8_t* kNBundleVersionKey           = "CFBundleVersion";
inline constexpr const utf8_t* kNBundleNameKey              = "CFBundleName";





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NArray;
class NMutex;
class NString;

// Cache
using NBundleCache = NCache<NString, NDictionary>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NBundle
{
public:
										NBundle(const NFile& theRoot);
										NBundle(const NString& bundleID = "");


	// Is the bundle valid?
	bool                                IsValid() const;


	// Get the bundle identifier
	NString                             GetIdentifier() const;


	// Get the root directory
	NFile                               GetRoot() const;


	// Get the resources directory
	NFile                               GetResources() const;


	// Get an Info.plist value
	//
	// GetInfoDictionary returns the Info.plist dictionary if  no key is supplied.
	bool                                GetInfoBool(      const NString& theKey)      const;
	NString                             GetInfoString(    const NString& theKey)      const;
	NArray                              GetInfoArray(     const NString& theKey)      const;
	NDictionary                         GetInfoDictionary(const NString& theKey = "") const;


	// Get an executable
	//
	// Returns the active executable if no name is supplied.
	NFile                               GetExecutable(const NString& theName = "") const;


	// Get a resource
	NFile                               GetResource(const NString& theName,
													const NString& theType = "",
													const NString& subDir  = "") const;


	// Get a string
	NString                             GetString(const NString& theKey,
												  const NString& defaultValue = "",
												  const NString& tableName    = "") const;


private:
	NFile                               GetBundle(        const NString& bundleID) const;
	NFile                               GetResources(     const NFile& theBundle)  const;
	NDictionary                         GetInfoDictionary(const NFile& theBundle)  const;
	NFile                               GetExecutable(    const NFile& theBundle, const NString& theName) const;

	NDictionary                         GetStringTable(const NString& theTable) const;
	static NMutex&                      GetCacheLock();
	static NBundleCache&                GetCache();



private:
	NFile                               mRoot;
	mutable NDictionary                 mInfo;
};





//=============================================================================
//		Function Prototypes
//-----------------------------------------------------------------------------
// Get a resource from the application bundle
NFile NBundleResource(const NString& theName,
					  const NString& theType = "",
					  const NString& subDir  = "");


// Get an image from the application bundle
NImage NBundleImage(const NString& theName,
					const NString& theType = "",
					const NString& subDir  = "");


// Get a string from the application bundle
NString NBundleString(const NString& theKey,
					  const NString& defaultValue = "",
					  const NString& tableName    = "");





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NBundle.inl"



#endif // NBUNDLE_H
