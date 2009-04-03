/*	NAME:
		NMacTarget.h

	DESCRIPTION:
		Mac target support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMAC_TARGET_HDR
#define NMAC_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMacTarget {
public:
	// Convert an error code
	static NStatus						GetStatus(OSStatus theErr);
};





#endif // NMAC_TARGET_HDR
