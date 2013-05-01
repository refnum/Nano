/*	NAME:
		NWinTarget.h

	DESCRIPTION:
		Windows target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWIN_TARGET_HDR
#define NWIN_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileMap.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWinTarget {
public:
	// Get the last error
	static NStatus						GetLastError(void);


	// Convert a status code
	static NStatus						ConvertHRESULT(HRESULT winErr);


	// Convert file state
	static UINT							ConvertFilePermission(NFilePermission thePermission);
	static DWORD						ConvertFilePosition(  NFilePosition   thePosition);
	static DWORD						ConvertFileMapAccess( NMapAccess      theAccess, bool forPage);


	// Convert times
	static DWORD						ConvertTimeMS(        NTime      theTime);
	static FILETIME						ConvertTimeFile(      NTime      theTime);
	static NTime						ConvertFILETIME(const FILETIME &fileTime);
};





#endif // NWIN_TARGET_HDR
