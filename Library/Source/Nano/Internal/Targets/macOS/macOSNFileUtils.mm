/*	NAME:
		macOSNFileUtils.mm

	DESCRIPTION:
		macOS file utilities.

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
#include "NSharedDarwin.h"
#include "NSharedPOSIX.h"

// System
#include <Foundation/Foundation.h>





//=============================================================================
//		NFileUtils::GetChildren : Get the children of a directory.
//-----------------------------------------------------------------------------
NVectorFilePath NFileUtils::GetChildren(const NFilePath& thePath)
{


	// Get the children
	return NSharedPOSIX::GetChildren(thePath);
}





//=============================================================================
//		NFileUtils::Rename : Atomically rename a file.
//-----------------------------------------------------------------------------
NStatus NFileUtils::Rename(const NString& oldPath, const NString& newPath)
{


	// Rename the file
	return NSharedDarwin::FileRename(oldPath, newPath);
}





//=============================================================================
//		NFileUtils::Exchange : Atomically exchange files.
//-----------------------------------------------------------------------------
NStatus NFileUtils::Exchange(const NString& oldPath, const NString& newPath)
{


	// Exchange the files
	return NSharedDarwin::FileExchange(oldPath, newPath);
}





#pragma mark private
//=============================================================================
//		NFileUtils::MakeDirectory : Create a directory.
//-----------------------------------------------------------------------------
NStatus NFileUtils::MakeDirectory(const NFilePath& thePath)
{


	// Create the directory
	return NSharedPOSIX::CreateDirectory(thePath);
}





//=============================================================================
//		NFileUtils::DeletePath : Delete a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::DeletePath(const NFilePath& thePath, bool moveToTrash)
{


	// Get the state we need
	NStatus theErr = NStatus::Permission;


	// Move to trash
	if (moveToTrash)
	{
		@autoreleasepool
		{
			NSString* nsPath = [NSString stringWithUTF8String:thePath.GetUTF8()];
			if (nsPath != nil)
			{
				NSURL* nsURL = [NSURL fileURLWithPath:nsPath];
				bool   wasOK = [[NSFileManager defaultManager] trashItemAtURL:nsURL
														   resultingItemURL:nil
																	  error:nil];
				NN_EXPECT(wasOK);

				if (wasOK)
				{
					theErr = NStatus::OK;
				}
			}
		}
	}


	// Delete the path
	else
	{
		theErr = NSharedPOSIX::DeletePath(thePath);
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::GetLocation : Get a location.
//-----------------------------------------------------------------------------
NStatus NFileUtils::GetLocation(NFileLocation theLocation, NString& thePath)
{


	// Get the location
	return NSharedDarwin::GetLocation(theLocation, thePath);
}
