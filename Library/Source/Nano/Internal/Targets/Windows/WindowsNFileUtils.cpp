/*	NAME:
		WindowsNFileUtils.cpp

	DESCRIPTION:
		Windows file utilities.

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
#include "NCommonWindows.h"
#include "NData.h"
#include "NFileInfo.h"
#include "NString.h"

// System
#include <Shlobj.h>
#include <ktmw32.h>
#include <shellapi.h>





//=============================================================================
//		Libraries
//-----------------------------------------------------------------------------
#pragma comment(lib, "KtmW32.lib")





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//      GetSHKnownFolderPath : Get a known folder location.
//----------------------------------------------------------------------------
static NFilePath GetSHKnownFolderPath(REFKNOWNFOLDERID theID)
{


	// Get the folder
	NFilePath thePath;
	PWSTR     winPath;

	if (SUCCEEDED(SHGetKnownFolderPath(theID, KF_FLAG_CREATE, nullptr, &winPath)))
	{
		thePath = NString(reinterpret_cast<const utf16_t*>(winPath));
	}

	return thePath;
}





//=============================================================================
//		CreateTransaction : Create a transaction.
//-----------------------------------------------------------------------------
NStatus CreateTransaction(HANDLE* hTransaction)
{


	// Create the transaction
	NStatus theErr = NStatus::OK;
	*hTransaction  = ::CreateTransaction(nullptr,
										0,
										TRANSACTION_DO_NOT_PROMOTE,
										0,
										0,
										INFINITE,
										L"WindowsNFileUtils Transaction");

	if (*hTransaction == nullptr)
	{
		theErr = NStatus::NotSupported;
	}

	return theErr;
}





//=============================================================================
//		RenameTransacted : Rename a path within a transaction.
//-----------------------------------------------------------------------------
NStatus RenameTransacted(HANDLE hTransaction, const NFilePath& oldPath, const NFilePath& newPath)
{


	// Rename the path
	BOOL wasOK = MoveFileTransactedW(LPCWSTR(oldPath.GetUTF16()),
									 LPCWSTR(newPath.GetUTF16()),
									 nullptr,
									 nullptr,
									 MOVEFILE_COPY_ALLOWED,
									 hTransaction);
	NN_EXPECT(wasOK);

	return NCommonWindows::StatusLastError(wasOK);
}





#pragma mark NFileUtils
//=============================================================================
//		NFileUtils::PathCreate : Create a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathCreate(const NFilePath& thePath)
{


	// Create the path
	BOOL wasOK = ::CreateDirectoryW(LPCWSTR(thePath.GetUTF16()), nullptr);
	NN_EXPECT(wasOK);

	return NCommonWindows::StatusLastError(wasOK);
}





//=============================================================================
//		NFileUtils::PathDelete : Delete a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathDelete(const NFilePath& thePath, NFileAction theAction)
{


	// Get the state we need
	NStatus theErr = NStatus::OK;



	// Move to trash
	//
	// The SHFILEOPSTRUCTW structure takes a null-terminated list of
	// null-terminated strings, so we must copy the path and add two
	// terminators.
	if (theAction == NFileAction::CanTrash)
	{
		NData winPath(thePath.GetPath().GetData(NStringEncoding::UTF16));
		winPath.SetSize(winPath.GetSize() + (sizeof(WCHAR) * 2));

		SHFILEOPSTRUCTW fileOp{};

		fileOp.wFunc  = FO_DELETE;
		fileOp.pFrom  = LPCWSTR(winPath.GetData());
		fileOp.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;

		int shErr = SHFileOperationW(&fileOp);
		NN_EXPECT_NOT_ERR(shErr);

		if (shErr != 0)
		{
			theErr = NStatus::NotFound;
		}
	}


	// Delete the path
	else
	{
		LPCWSTR winPath = LPCWSTR(thePath.GetUTF16());
		BOOL    wasOK   = true;

		if (NFileInfo(thePath).IsDirectory())
		{
			wasOK = RemoveDirectory(winPath);
			NN_EXPECT(wasOK);
		}
		else
		{
			wasOK = DeleteFile(winPath);
			NN_EXPECT(wasOK);
		}

		theErr = NCommonWindows::StatusLastError(wasOK);
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::PathChildren : Get the children of a path.
//-----------------------------------------------------------------------------
NVectorFilePath NFileUtils::PathChildren(const NFilePath& thePath)
{


	// Open the directory
	WIN32_FIND_DATA dirEntry{};
	NVectorFilePath theChildren;

	NString thePattern = thePath.GetPath() + "\\*";
	HANDLE  theDir     = FindFirstFile(LPCWSTR(thePattern.GetUTF16()), &dirEntry);

	if (theDir == INVALID_HANDLE_VALUE)
	{
		return theChildren;
	}



	// Collect the children
	do
	{
		if (wcscmp(dirEntry.cFileName, L".") != 0 && wcscmp(dirEntry.cFileName, L"..") != 0)
		{
			NString fileName(reinterpret_cast<const utf16_t*>(dirEntry.cFileName));
			theChildren.emplace_back(thePath.GetChild(fileName));
		}
	} while (FindNextFile(theDir, &dirEntry));



	// Clean up
	FindClose(theDir);

	return theChildren;
}





//=============================================================================
//		NFileUtils::PathRename : Atomically rename a path.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathRename(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Get the state we need
	HANDLE  hTransaction = nullptr;
	NStatus theErr       = CreateTransaction(&hTransaction);



	// Rename the path
	if (theErr == NStatus::OK)
	{
		theErr = RenameTransacted(hTransaction, oldPath, newPath);

		if (theErr == NStatus::OK)
		{
			BOOL wasOK = CommitTransaction(hTransaction);
			theErr     = NCommonWindows::StatusLastError(wasOK);
			NN_EXPECT_NOT_ERR(theErr);
		}


		// Clean up
		BOOL wasOK = CloseHandle(hTransaction);
		NN_EXPECT(wasOK);
	}

	return theErr;
}





//=============================================================================
//		NFileUtils::PathExchange : Atomically exchange two paths.
//-----------------------------------------------------------------------------
NStatus NFileUtils::PathExchange(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Get the state we need
	HANDLE  hTransaction = nullptr;
	NStatus theErr       = CreateTransaction(&hTransaction);



	// Exchange the files
	if (theErr == NStatus::OK)
	{
		NFilePath tmpPath = newPath;
		tmpPath.SetExtension("nfileutils_exchange");

		theErr = RenameTransacted(hTransaction, oldPath, tmpPath);

		if (theErr == NStatus::OK)
		{
			theErr = RenameTransacted(hTransaction, newPath, oldPath);
		}

		if (theErr == NStatus::OK)
		{
			theErr = RenameTransacted(hTransaction, tmpPath, newPath);
		}

		if (theErr == NStatus::OK)
		{
			BOOL wasOK = CommitTransaction(hTransaction);
			theErr     = NCommonWindows::StatusLastError(wasOK);
			NN_EXPECT_NOT_ERR(theErr);
		}


		// Clean up
		BOOL wasOK = CloseHandle(hTransaction);
		NN_EXPECT(wasOK);
	}

	return theErr;
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
			thePath = GetSHKnownFolderPath(FOLDERID_InternetCache);
			break;

		case NFileLocation::AppSupport:
			thePath = GetSHKnownFolderPath(FOLDERID_LocalAppData);
			break;

		case NFileLocation::AppTemporaries:
		{
			WCHAR tmpPath[MAX_PATH];

			DWORD winErr = GetTempPathW(MAX_PATH, tmpPath);
			if (winErr != 0 && winErr < MAX_PATH)
			{
				tmpPath[winErr - 1] = WCHAR(0);
				thePath             = NString(reinterpret_cast<const utf16_t*>(&tmpPath[0]));
			}

			if (!thePath.IsValid())
			{
				thePath = GetSHKnownFolderPath(FOLDERID_InternetCache);
			}
		}
		break;

		case NFileLocation::SharedSupport:
			thePath = GetSHKnownFolderPath(FOLDERID_ProgramData);
			break;

		case NFileLocation::UserDesktop:
			thePath = GetSHKnownFolderPath(FOLDERID_Desktop);
			break;

		case NFileLocation::UserDocuments:
			thePath = GetSHKnownFolderPath(FOLDERID_Documents);
			break;

		case NFileLocation::UserDownloads:
			thePath = GetSHKnownFolderPath(FOLDERID_Downloads);
			break;

		case NFileLocation::UserHome:
			thePath = GetSHKnownFolderPath(FOLDERID_Profile);
			break;

		case NFileLocation::UserLogs:
			thePath = GetSHKnownFolderPath(FOLDERID_LocalAppData);
			if (thePath.IsValid())
			{
				thePath = thePath.GetChild("Logs");
			}
			break;

		case NFileLocation::UserPictures:
			thePath = GetSHKnownFolderPath(FOLDERID_Pictures);
			break;

		case NFileLocation::UserPreferences:
			thePath = GetSHKnownFolderPath(FOLDERID_LocalAppData);
			if (thePath.IsValid())
			{
				thePath = thePath.GetChild("Preferences");
			}
			break;
	}

	return thePath;
}
