/*	NAME:
		NWinTarget.cpp

	DESCRIPTION:
		Windows target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NWinTarget.h"





//============================================================================
//		NWinTarget::GetLastError : Get the last error.
//----------------------------------------------------------------------------
NStatus NWinTarget::GetLastError(void)
{


	// Get the error
	return(ConvertHRESULT(::GetLastError()));
}





//============================================================================
//		NWinTarget::ConvertHRESULT : Convert an HRESULT.
//----------------------------------------------------------------------------
NStatus NWinTarget::ConvertHRESULT(HRESULT winErr)
{	NStatus		theErr;



	// Convert the value
	switch (winErr) {
		case ERROR_SUCCESS:				theErr = kNoErr;				break;
		case ERROR_ALREADY_EXISTS:		theErr = kNErrDuplicate;		break;
		case ERROR_PATH_NOT_FOUND:		theErr = kNErrNotFound;			break;
		case ERROR_FILE_NOT_FOUND:		theErr = kNErrNotFound;			break;
		case ERROR_HANDLE_EOF:			theErr = kNErrExhaustedSrc;		break;
		case ERROR_INVALID_HANDLE:		theErr = kNErrParam;			break;

		default:
			NN_LOG("Unable to convert %ld", winErr);
			theErr = kNErrParam;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NWinTarget::ConvertFilePermission : Convert file permissions.
//----------------------------------------------------------------------------
UINT NWinTarget::ConvertFilePermission(NFilePermission thePermission)
{	UINT	theResult;



	// Convert the value
	//
	// To obtain append-rather-than-overwrite behaviour when writing,
	// we need to request FILE_APPEND_DATA rather than FILE_WRITE_DATA.
	switch (thePermission) {
		case kNPermissionRead:			theResult = FILE_READ_DATA;							break;
		case kNPermissionWrite:			theResult = FILE_APPEND_DATA;						break;
		case kNPermissionReadWrite:		theResult = FILE_READ_DATA | FILE_APPEND_DATA;		break;

		default:
			NN_LOG("Unable to convert %ld", thePermission);
			theResult = OF_READWRITE;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NWinTarget::ConvertFileMapAccess : Convert file map access.
//----------------------------------------------------------------------------
DWORD NWinTarget::ConvertFileMapAccess(NMapAccess theAccess, bool forPage)
{	DWORD	theResult;



	// Convert the value
	//
	// Confusingly, Windows provides two sets of constant for creating a file
	// map (PAGE_xxxx) and for mapping pages within that file (FILE_MAP_xxxx).
	//
	// Even more confusingly, the names are reversed - PAGE_xxx is for the file,
	// and FILE_MAP_xxxx is for the pages.
	//
	// We can convert our access to either form, based on the request being for
	// the file or for pages within the file.
	switch (theAccess) {
		case kNAccessRead:			theResult = forPage ? FILE_MAP_READ  : PAGE_READONLY;			break;
		case kNAccessReadWrite:		theResult = forPage ? FILE_MAP_WRITE : PAGE_READWRITE;			break;
		case kNAccessCopyOnWrite:	theResult = forPage ? FILE_MAP_COPY  : PAGE_WRITECOPY;			break;

		default:
			NN_LOG("Unable to convert %ld", theAccess);
			theResult = PAGE_READONLY;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NWinTarget::ConvertFilePosition : Convert file positions.
//----------------------------------------------------------------------------
DWORD NWinTarget::ConvertFilePosition(NFilePosition thePosition)
{	DWORD		theResult;



	// Convert the value
	switch (thePosition) {
		case kNPositionFromStart:		theResult = FILE_BEGIN;		break;
		case kNPositionFromMark:		theResult = FILE_CURRENT;	break;
		case kNPositionFromEnd:			theResult = FILE_END;		break;

		default:
			NN_LOG("Unable to convert %ld", thePosition);
			theResult = FILE_BEGIN;
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NWinTarget::ConvertTimeMS : Convert a timeout to milliseconds.
//----------------------------------------------------------------------------
DWORD NWinTarget::ConvertTimeMS(NTime theTime)
{	DWORD		timeMS;



	// Convert the time
	if (theTime < 0.0)
		timeMS = INFINITE;
	else
		timeMS = (DWORD) (theTime / kNTimeMillisecond);
	
	return(timeMS);
}





