/*	NAME:
		NWinTarget.h

	DESCRIPTION:
		Windows target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
};





#endif // NWIN_TARGET_HDR
