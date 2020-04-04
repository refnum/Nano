/*	NAME:
		NFilePath.cpp

	DESCRIPTION:
		File path.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
// Part patterns
#if NN_TARGET_WINDOWS
static constexpr const char* kNPatternParent                = "(.*)\\\\.*?$";
static constexpr const char* kNPatternNameWithExtension     = ".*\\\\(.*?$)";
static constexpr const char* kNPatternNameWithoutExtension  = ".*\\\\(.*?)(\\..*)?$";
static constexpr const char* kNPatternExtension             = "\\.(.*?$)";

#else
static constexpr const char* kNPatternParent                = "(.*)\\/.*?$";
static constexpr const char* kNPatternNameWithExtension     = ".*\\/(.*?$)";
static constexpr const char* kNPatternNameWithoutExtension  = ".*\\/(.*?)(\\..*)?$";
static constexpr const char* kNPatternExtension             = "\\.(.*?)$";
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
//		NFilePath::Clear : Clear the path.
//-----------------------------------------------------------------------------
void NFilePath::Clear()
{


	// Reset our state
	mPath.Clear();
}





//=============================================================================
//		NFilePath::GetPath : Get the path.
//-----------------------------------------------------------------------------
NString NFilePath::GetPath() const
{


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
//		NFilePath::GetParent : Get the parent.
//-----------------------------------------------------------------------------
NString NFilePath::GetParent() const
{


	// Get the parent
	return GetPathPart(mPath, kNPatternParent);
}





//=============================================================================
//		NFilePath::GetName : Get the name.
//-----------------------------------------------------------------------------
NString NFilePath::GetName(bool withExtension) const
{


	// Get the parent
	if (withExtension)
	{
		return GetPathPart(mPath, kNPatternNameWithExtension);
	}
	else
	{
		return GetPathPart(mPath, kNPatternNameWithoutExtension);
	}
}





//=============================================================================
//		NFilePath::GetExtension : Get the extension.
//-----------------------------------------------------------------------------
NString NFilePath::GetExtension() const
{


	// Get the extension
	return GetPathPart(mPath, kNPatternExtension);
}





#pragma mark NMixinAppendable
//=============================================================================
//		NFilePath::Append : Append a value.
//-----------------------------------------------------------------------------
void NFilePath::Append(const NFilePath& thePath)
{


	// Append the path
	mPath.Append(thePath.mPath);
}





#pragma mark NMixinComparable
//=============================================================================
//		NFilePath::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NFilePath::CompareEqual(const NFilePath& thePath) const
{


	// Compare the path
	return mPath.CompareEqual(thePath.mPath);
}





//=============================================================================
//		NFilePath::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NFilePath::CompareOrder(const NFilePath& thePath) const
{


	// Compare the path
	return mPath.CompareOrder(thePath.mPath);
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
//		NFilePath::GetPathPart : Get part of a path.
//-----------------------------------------------------------------------------
NString NFilePath::GetPathPart(const NString& thePath, const NString& thePattern) const
{


	// Get the part
	NPatternGroup theMatch = thePath.FindGroup(thePattern, kNStringPattern);
	NString       theResult;

	if (!theMatch.theGroups.empty())
	{
		theResult = thePath.GetSubstring(theMatch.theGroups[0]);
	}

	return theResult;
}
