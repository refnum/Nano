/*	NAME:
		LinuxNFileUtils.cpp

	DESCRIPTION:
		Linux file utilities.

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
#include "NFileUtils.h"

// Nano
#include "NCommonLinux.h"
#include "NCommonPOSIX.h"
#include "NSystem.h"





//=============================================================================
//		NFileUtils::PathCreate : Create a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathCreate(const NFilePath& thePath)
{


	// Create the path
	return NCommonPOSIX::PathCreate(thePath);
}





//=============================================================================
//		NFileUtils::PathDelete : Delete a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathDelete(const NFilePath& thePath, NFileAction /*theAction*/)
{


	// Delete the path
	return NCommonPOSIX::PathDelete(thePath);
}





//=============================================================================
//		NFileUtils::PathChildren : Get the children of a path.
//-----------------------------------------------------------------------------
NVectorFilePath NFileUtils::PathChildren(const NFilePath& thePath)
{


	// Get the children
	return NCommonPOSIX::PathChildren(thePath);
}





//=============================================================================
//		NFileUtils::PathRename : Atomically rename a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathRename(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Rename the path
	return NCommonLinux::PathRename(oldPath, newPath);
}





//=============================================================================
//		NFileUtils::PathExchange : Atomically exchange two paths.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathExchange(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Exchange the paths
	return NCommonLinux::PathExchange(oldPath, newPath);
}





//=============================================================================
//		NFileUtils::PathLocation : Get the path to a location.
//-----------------------------------------------------------------------------
NFilePath NFileUtils::PathLocation(NFileLocation theLocation)
{


	// Get the location
	NFilePath thePath;

	switch (theLocation)
	{
		case NFileLocation::AppCaches:
			thePath = NSystem::GetEnv("XDG_CACHE_HOME");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/.cache";
			}
			break;

		case NFileLocation::AppSupport:
			thePath = NSystem::GetEnv("XDG_DATA_HOME");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/.local/share";
			}
			break;

		case NFileLocation::AppTemporaries:
			thePath = NSystem::GetEnv("XDG_RUNTIME_DIR");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("TMPDIR");
			}

			if (!thePath.IsValid())
			{
				thePath = NString("/tmp");
			}
			break;

		case NFileLocation::SharedSupport:
			thePath = NSystem::GetEnv("XDG_DATA_DIRS");
			if (thePath.IsValid() && thePath.GetPath().Contains(":"))
			{
				thePath = thePath.GetPath().Split(":").front();
			}

			if (!thePath.IsValid())
			{
				thePath = NString("/usr/local/share");
			}
			break;

		case NFileLocation::UserDesktop:
			thePath = NSystem::GetEnv("XDG_DESKTOP_DIR");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/Desktop";
			}
			break;

		case NFileLocation::UserDocuments:
			thePath = NSystem::GetEnv("XDG_DOCUMENTS_DIR");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/Documents";
			}
			break;

		case NFileLocation::UserDownloads:
			thePath = NSystem::GetEnv("XDG_DOWNLOAD_DIR");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/Downloads";
			}
			break;

		case NFileLocation::UserHome:
			thePath = NSystem::GetEnv("HOME");
			break;

		case NFileLocation::UserLogs:
			thePath = NSystem::GetEnv("XDG_DATA_HOME");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/.local/share/logs";
			}
			break;

		case NFileLocation::UserPictures:
			thePath = NSystem::GetEnv("XDG_PICTURES_DIR");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/Pictures";
			}
			break;

		case NFileLocation::UserPreferences:
			thePath = NSystem::GetEnv("XDG_CONFIG_HOME");
			if (!thePath.IsValid())
			{
				thePath = NSystem::GetEnv("HOME") + "/.config";
			}
			break;
	}

	return thePath;
}
