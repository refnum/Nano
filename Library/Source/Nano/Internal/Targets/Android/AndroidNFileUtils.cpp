/*	NAME:
		AndroidNFileUtils.cpp

	DESCRIPTION:
		Android file utilities.

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
NStatus NFileUtils::PathRename(const NFilePath& pathOld, const NFilePath& pathNew)
{


	// Rename the path
	return NCommonLinux::PathRename(pathOld, pathNew);
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
			NN_LOG_UNIMPLEMENTED("NFileLocation::AppCaches");
			break;

		case NFileLocation::AppSupport:
			NN_LOG_UNIMPLEMENTED("NFileLocation::AppSupport");
			break;

		case NFileLocation::AppTemporaries:
			NN_LOG_UNIMPLEMENTED("NFileLocation::AppTemporaries");
			break;

		case NFileLocation::SharedSupport:
			NN_LOG_UNIMPLEMENTED("NFileLocation::SharedSupport");
			break;

		case NFileLocation::UserDesktop:
			NN_LOG_UNIMPLEMENTED("NFileLocation::UserDesktop");
			break;

		case NFileLocation::UserDocuments:
			NN_LOG_UNIMPLEMENTED("NFileLocation::UserDocuments");
			break;

		case NFileLocation::UserDownloads:
			NN_LOG_UNIMPLEMENTED("NFileLocation::UserDownloads");
			break;

		case NFileLocation::UserHome:
			NN_LOG_UNIMPLEMENTED("NFileLocation::UserHome");
			break;

		case NFileLocation::UserLogs:
			NN_LOG_UNIMPLEMENTED("NFileLocation::UserLogs");
			break;

		case NFileLocation::UserPictures:
			NN_LOG_UNIMPLEMENTED("NFileLocation::UserPictures");
			break;

		case NFileLocation::UserPreferences:
			NN_LOG_UNIMPLEMENTED("NFileLocation::UserPreferences");
			break;
	}

	return thePath;
}
