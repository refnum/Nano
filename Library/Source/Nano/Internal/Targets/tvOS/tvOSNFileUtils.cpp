/*	NAME:
		tvOSNFileUtils.cpp

	DESCRIPTION:
		tvOS file utilities.

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
#include "NCommonDarwin.h"
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
NStatus NFileUtils::PathRename(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Rename the path
	return NCommonDarwin::PathRename(oldPath, newPath);
}





//=============================================================================
//		NFileUtils::PathExchange : Atomically exchange two paths.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathExchange(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Exchange the paths
	return NCommonDarwin::PathExchange(oldPath, newPath);
}





//=============================================================================
//		NFileUtils::PathLocation : Get the path to a location.
//-----------------------------------------------------------------------------
NFilePath NFileUtils::PathLocation(NFileLocation theLocation)
{


	// Get the location
	return NCommonDarwin::PathLocation(theLocation);
}
