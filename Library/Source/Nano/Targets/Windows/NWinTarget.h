/*	NAME:
		NWinTarget.h

	DESCRIPTION:
		Windows target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NWIN_TARGET_HDR
#define NWIN_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NWinTarget {
public:
	// Convert an error code
	static NStatus						GetStatus(HRESULT theErr);
};





#endif // NWIN_TARGET_HDR
