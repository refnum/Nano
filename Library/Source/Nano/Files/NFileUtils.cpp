/*	NAME:
		NFileUtils.cpp

	DESCRIPTION:
		File utilities.

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
#include "NFileUtils.h"

// Nano
#include "NFileInfo.h"
#include "NString.h"





//=============================================================================
//		Internalonstants
//-----------------------------------------------------------------------------
// Directory separator
#if NN_TARGET_WINDOWS
static constexpr const char* kNPatternParent                = "(.*)\\.*?$";
static constexpr const char* kNPatternName                  = ".*\\(.*?$)";
static constexpr const char* kNPatternExtension             = "\\.(.*?$)";

#else
static constexpr const char* kNPatternParent                = "(.*)\\/.*?$";
static constexpr const char* kNPatternName                  = ".*\\/(.*?$)";
static constexpr const char* kNPatternExtension             = "\\.(.*?)$";
#endif





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetPartPattern : Get the pattern for a part.
//-----------------------------------------------------------------------------
static NString GetPartPattern(NPathPart thePart)
{


	// Get the pattern
	NString thePattern;

	switch (thePart)
	{
		case NPathPart::Parent:
			thePattern = kNPatternParent;
			break;

		case NPathPart::Name:
			thePattern = kNPatternName;
			break;

		case NPathPart::Extension:
			thePattern = kNPatternExtension;
			break;
	}

	NN_REQUIRE(!thePattern.IsEmpty());

	return thePattern;
}





#pragma mark NFileUtils
//=============================================================================
//		NFileUtils::GetPathPart : Get part of a path.
//-----------------------------------------------------------------------------
NString NFileUtils::GetPathPart(const NString& thePath, NPathPart thePart)
{


	// Get the component
	NString       thePattern = GetPartPattern(thePart);
	NPatternGroup theMatch   = thePath.FindGroup(thePattern, kNStringPattern);
	NString       theResult;

	if (!theMatch.theGroups.empty())
	{
		NN_REQUIRE(theMatch.theGroups.size() == 1);
		theResult = thePath.GetSubstring(theMatch.theGroups[0]);
	}

	return theResult;
}





//=============================================================================
//		NFileUtils::CreateDirectories : Create directories.
//-----------------------------------------------------------------------------
NStatus NFileUtils::CreateDirectories(const NString& thePath)
{


	// Get the state we need
	NStatus theErr = NStatus::NotFound;
	NString parentPath;



	// Create the directories
	for (const auto& theParent : thePath.Split(kNPathSeparator, kNStringNone))
	{
		if (!theParent.IsEmpty())
		{
			// Get the state we need
			parentPath += kNPathSeparator + theParent;
			NFileInfo theInfo(parentPath);



			// Create the parent
			//
			// We create missing directories, skip existing directories,
			// and fail if we encounter something that is not a directory.
			if (!theInfo.Exists())
			{
				theErr = CreateDirectory(parentPath);
			}
			else if (!theInfo.IsDirectory())
			{
				theErr = NStatus::Duplicate;
			}
			else
			{
				theErr = NStatus::OK;
			}



			// Handle failure
			if (theErr != NStatus::OK)
			{
				break;
			}
		}
	}

	return theErr;
}
