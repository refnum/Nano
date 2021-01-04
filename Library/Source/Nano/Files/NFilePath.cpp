/*	NAME:
		NFilePath.cpp

	DESCRIPTION:
		File path.

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
#include "NFilePath.h"





//=============================================================================
//		Internal Cnstants
//-----------------------------------------------------------------------------
// Separator
#if NN_TARGET_WINDOWS
static constexpr const char* kNPathSeparator                = "\\";
#else
static constexpr const char* kNPathSeparator                = "/";
#endif


// Components
#if NN_TARGET_WINDOWS
static constexpr const char* kNAbsolutePrefix               = "[A-Za-z]:\\\\";
static constexpr const char* kNRootSuffix                   = ":\\";

static constexpr const char* kNPartRoot                     = "^([A-Za-z]:\\\\)";
static constexpr const char* kNPartParent                   = "(.*)\\\\.*?$";
static constexpr const char* kNPartFilename                 = ".*\\\\(.*?$)";
static constexpr const char* kNPartStem                     = ".*\\\\(.*?)(\\..*)?$";
static constexpr const char* kNPartStemSingle               = "(.*?)(\\..*)?$";
static constexpr const char* kNPartExtension                = "\\.(.*?$)";

#else
static constexpr const char* kNAbsolutePrefix               = "\\/";
static constexpr const char* kNRootSuffix                   = "/";

static constexpr const char* kNPartRoot                     = "^(\\/)";
static constexpr const char* kNPartParent                   = "(.*)\\/.*?$";
static constexpr const char* kNPartFilename                 = ".*\\/(.*?$)";
static constexpr const char* kNPartStem                     = ".*\\/(.*?)(\\..*)?$";
static constexpr const char* kNPartStemSingle               = "(.*?)(\\..*)?$";
static constexpr const char* kNPartExtension                = "\\.(.*?)$";
#endif





#pragma mark NFilePath
//=============================================================================
//		NFilePath::NFilePath : Constructor.
//-----------------------------------------------------------------------------
NFilePath::NFilePath(const NString& thePath)
	: mPath()
{


	// Set the path
	SetPath(thePath);
}





//=============================================================================
//		NFilePath::IsValid : Is the path valid?
//-----------------------------------------------------------------------------
bool NFilePath::IsValid() const
{


	// Check the state
	return !mPath.IsEmpty();
}





//=============================================================================
//		NFilePath::IsAbsolute : Is this an absolute path?
//-----------------------------------------------------------------------------
bool NFilePath::IsAbsolute() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Check the state
	return mPath.StartsWith(kNAbsolutePrefix, kNStringPattern);
}





//=============================================================================
//		NFilePath::IsRelative : Is this a relative path?
//-----------------------------------------------------------------------------
bool NFilePath::IsRelative() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Check the state
	return !IsAbsolute();
}





//=============================================================================
//		NFilePath::IsRoot : Is this a root path?
//-----------------------------------------------------------------------------
bool NFilePath::IsRoot() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Check the state
	return mPath.EndsWith(kNRootSuffix);
}





//=============================================================================
//		NFilePath::Clear : Clear the path.
//-----------------------------------------------------------------------------
void NFilePath::Clear()
{


	// Reset our state
	mPath.Clear();
}





//=============================================================================
//		NFilePath::GetRoot : Get the root path.
//-----------------------------------------------------------------------------
NFilePath NFilePath::GetRoot() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the root path
	NString theRoot = GetPart(kNPartRoot);

	NN_REQUIRE((IsRelative() && theRoot.IsEmpty()) || (IsAbsolute() && !theRoot.IsEmpty()));

	return theRoot;
}





//=============================================================================
//		NFilePath::GetParent : Get the parent.
//-----------------------------------------------------------------------------
NFilePath NFilePath::GetParent() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the parent
	if (IsRoot())
	{
		// A root is its own parent
		return *this;
	}
	else
	{
		// Get the parent
		//
		// The root is the part that remains when there are no more parents.
		NString theParent = GetPart(kNPartParent);
		if (!theParent.Contains(kNPathSeparator))
		{
			theParent = GetPart(kNPartRoot);
		}

		return theParent;
	}
}





//=============================================================================
//		NFilePath::GetChild : Get a child of the path.
//-----------------------------------------------------------------------------
NFilePath NFilePath::GetChild(const NString& theName) const
{


	// Validate our parameters and state
	NN_REQUIRE(!theName.IsEmpty());
	NN_REQUIRE(IsValid());



	// Get the child
	NFilePath theChild(*this);

	if (!theChild.IsRoot())
	{
		NN_REQUIRE(!theChild.mPath.EndsWith(kNPathSeparator));
		theChild.mPath += kNPathSeparator;
	}

	theChild.mPath += theName;

	return theChild;
}





//=============================================================================
//		NFilePath::GetFilename : Get the filename.
//-----------------------------------------------------------------------------
NString NFilePath::GetFilename() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the filename
	//
	// If we fail to find a separator we must have a single, relative, part.
	NString theName = GetPart(kNPartFilename);

	if (theName.IsEmpty())
	{
		NN_REQUIRE(IsRelative());

		theName = mPath;
	}

	return theName;
}





//=============================================================================
//		NFilePath::SetFilename : Set the filename.
//-----------------------------------------------------------------------------
void NFilePath::SetFilename(const NString& theName)
{


	// Validate our parameters and state
	NN_REQUIRE(!theName.IsEmpty());
	NN_REQUIRE(IsValid());



	// Set the filename
	//
	// If we fail to find a separator we must have a single, relative, part.
	if (!SetPart(kNPartFilename, theName))
	{
		NN_REQUIRE(IsRelative());
		mPath = theName;
	}



	// Validate our state
	NN_REQUIRE(GetFilename() == theName);
}





//=============================================================================
//		NFilePath::GetStem : Get the stem.
//-----------------------------------------------------------------------------
NString NFilePath::GetStem() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the stem
	//
	// If we fail to find a separator we must have a single, relative, part.
	NString theStem = GetPart(kNPartStem);

	if (theStem.IsEmpty())
	{
		NN_REQUIRE(IsRelative());

		theStem = GetPart(kNPartStemSingle);
		NN_REQUIRE(!theStem.IsEmpty());
	}

	return theStem;
}





//=============================================================================
//		NFilePath::SetStem : Set the stem.
//-----------------------------------------------------------------------------
void NFilePath::SetStem(const NString& theStem)
{


	// Validate our parameters and state
	NN_REQUIRE(!theStem.IsEmpty());
	NN_REQUIRE(IsValid());



	// Set the stem
	//
	// If we fail to find a separator we must have a single, relative, part.
	if (!SetPart(kNPartStem, theStem))
	{
		NN_REQUIRE(IsRelative());

		bool didSet = SetPart(kNPartStemSingle, theStem);
		NN_REQUIRE(didSet);
	}



	// Validate our state
	NN_REQUIRE(GetStem() == theStem);
}





//=============================================================================
//		NFilePath::GetExtension : Get the extension.
//-----------------------------------------------------------------------------
NString NFilePath::GetExtension() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the extension
	return GetPart(kNPartExtension);
}





//=============================================================================
//		NFilePath::SetExtension : Set the extension.
//-----------------------------------------------------------------------------
void NFilePath::SetExtension(const NString& theExtension)
{


	// Validate our parameters and state
	NN_REQUIRE(!theExtension.StartsWith("."));
	NN_REQUIRE(IsValid());



	// Set the extension
	bool didSet = SetPart(kNPartExtension, theExtension);
	if (didSet)
	{
		// Remove the extension
		//
		// If we had an extension then we will have replaced everything
		// after the last period.
		//
		// If the new extension is empty we must remove this period too.
		if (theExtension.IsEmpty())
		{
			NN_REQUIRE(mPath.EndsWith("."));
			mPath.RemoveSuffix(1);
		}
	}
	else
	{
		// Add an extension
		//
		// If we had no extension, but we want to add one, then we must
		// also add the last period.
		NN_REQUIRE(!mPath.EndsWith("."));

		if (!theExtension.IsEmpty())
		{
			mPath += "." + theExtension;
		}
	}



	// Validate our state
	NN_REQUIRE(GetExtension() == theExtension);
}





//=============================================================================
//		NFilePath::GetParts : Get the parts.
//-----------------------------------------------------------------------------
NVectorString NFilePath::GetParts() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the parts
	NVectorString theParts;

	if (IsRoot())
	{
		// The root path contains the root
		theParts.push_back(mPath);
	}
	else
	{
		// A non-root path is split on separators
		//
		// If the path was absolute then the root part's separator
		// will have been lost, so this must be added back on.
		theParts = mPath.Split(kNPathSeparator);
		NN_REQUIRE(!theParts.empty());

		if (IsAbsolute())
		{
			theParts[0] += kNPathSeparator;
		}
	}

	return theParts;
}





//=============================================================================
//		NFilePath::SetParts : Set the parts.
//-----------------------------------------------------------------------------
void NFilePath::SetParts(const NVectorString& theParts)
{


	// Get the state we need
	bool   isAbsolute = false;
	size_t numParts   = theParts.size();
	size_t n          = 0;

	mPath.Clear();



	// Set the parts
	for (const auto& thePart : theParts)
	{
		// Update our state
		mPath += thePart;

		if (n == 0)
		{
			isAbsolute = IsAbsolute();
		}


		// Add a separator
		//
		// We don't add a separator after the last part.
		//
		// If we have an absolute path then the root already contains
		// a separator so we don't add one after the first part either.
		if (n == (numParts - 1))
		{
			// Don't add separator after last part
		}
		else if (isAbsolute && n == 0)
		{
			// Don't add separator after root part
		}
		else
		{
			mPath += kNPathSeparator;
		}

		n++;
	}
}





//=============================================================================
//		NFilePath::GetPath : Get the path.
//-----------------------------------------------------------------------------
NString NFilePath::GetPath() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the path
	return mPath;
}





//=============================================================================
//		NFilePath::SetPath : Set the path.
//-----------------------------------------------------------------------------
void NFilePath::SetPath(const NString& thePath)
{


	// Set the path
	mPath = thePath;
}





//=============================================================================
//		NFilePath::GetUTF8 : Get the path as a UTF8 string.
//-----------------------------------------------------------------------------
const utf8_t* NFilePath::GetUTF8() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the path
	return mPath.GetUTF8();
}





//=============================================================================
//		NFilePath::GetUTF16 : Get the path as a UTF16 string.
//-----------------------------------------------------------------------------
const utf16_t* NFilePath::GetUTF16() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the path
	return mPath.GetUTF16();
}





#pragma mark NMixinAppendable
//=============================================================================
//		NFilePath::Append : Append a value.
//-----------------------------------------------------------------------------
void NFilePath::Append(const NFilePath& thePath)
{


	// Append the path
	*this = GetChild(thePath.mPath);
}





#pragma mark NMixinComparable
//=============================================================================
//		NFilePath::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NFilePath::CompareEqual(const NFilePath& thePath) const
{


	// Compare the path
	return CompareOrder(thePath) == NComparison::EqualTo;
}





//=============================================================================
//		NFilePath::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NFilePath::CompareOrder(const NFilePath& thePath) const
{


	// Compare the path
	//
	// File paths are considered to be case-insensitive but case-preserving.
	return mPath.Compare(thePath.mPath, kNStringNoCase);
}





#pragma mark NMixinHashable
//=============================================================================
//		NFilePath::HashGet : Get the hash.
//-----------------------------------------------------------------------------
size_t NFilePath::HashGet() const
{


	// Get the hash
	return mPath.HashGet();
}





//=============================================================================
//		NFilePath::HashUpdate : Update the hash.
//-----------------------------------------------------------------------------
size_t NFilePath::HashUpdate()
{


	// Update the hash
	return mPath.HashUpdate();
}





//=============================================================================
//		NFilePath::HashClear : Clear the hash.
//-----------------------------------------------------------------------------
void NFilePath::HashClear()
{


	// Clear the hash
	mPath.HashClear();
}





#pragma mark private
//=============================================================================
//		NFilePath::GetPart : Get part of the path.
//-----------------------------------------------------------------------------
NString NFilePath::GetPart(const NString& thePattern) const
{


	// Get the part
	NPatternGroup theMatch = mPath.FindGroup(thePattern, kNStringPattern);
	NString       theResult;

	if (!theMatch.theGroups.empty())
	{
		theResult = mPath.GetSubstring(theMatch.theGroups[0]);
	}

	return theResult;
}





//=============================================================================
//		NFilePath::SetPart : Set part of the path.
//-----------------------------------------------------------------------------
bool NFilePath::SetPart(const NString& thePattern, const NString& theValue)
{


	// Set the part
	NPatternGroup theMatch = mPath.FindGroup(thePattern, kNStringPattern);
	bool          didSet   = !theMatch.theGroups.empty();

	if (didSet)
	{
		mPath.Replace(theMatch.theGroups[0], theValue);
	}

	return didSet;
}
